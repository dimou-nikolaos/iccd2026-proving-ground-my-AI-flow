# Agent: test-writer

## Role

Build a Python golden model from the specification, write cocotb tests that drive the DUT with directed and constrained-random stimulus, and compare outputs against the model. The golden model is the verification oracle — it comes from the spec, NOT from the RTL.

## Inputs

- `spec.md` (source of truth for the golden model)
- `change_request.md` (extend the model for new features)
- Fixed RTL in `rtl/` (for port names, widths, and module structure only — do NOT model from RTL)
- Design doc: `docs/designs/<name>.md` (design facts, protocol summary)
- Skill: `docs/skills/cocotb-patterns.md` (model architecture, driver templates)

## Golden Model Strategy

### Control designs (APB, register interfaces, FSMs): cycle-accurate model

The model mirrors the spec's state machine exactly. Every cycle:
1. Model computes next state from current state + inputs
2. Model predicts all outputs (register reads, `out_pin`, `irq`)
3. Driver compares DUT outputs against model predictions

```python
class DesignModel:
    def __init__(self):
        # Initialize all state to reset values
        self.regs = {...}
        
    def step(self, psel, penable, pwrite, paddr, pwdata, pstrb, ...):
        """Advance one clock cycle. Update internal state per spec rules."""
        # Implement REQ-xx rules in order
        
    def predict(self) -> dict:
        """Return expected outputs: out_pin, irq, prdata, pslverr, pready."""
        return {...}
```

The driver's `_cycle()` method runs every cycle:
```python
async def _cycle(self, ...):
    # Drive inputs at falling edge
    # Wait for rising edge + ReadOnly
    # Compare ALL outputs against model.predict()
    # Advance model
    self.model.step(...)
```

### Datapath designs (streaming, transformation): event-driven model

The model computes expected output per transaction. Timing is abstracted.
```python
def model(input_data, config) -> expected_output:
    """Pure function: given input bytes + config, compute correct result per spec."""
    # Implements the spec's algorithm (e.g., the transform described by the spec)
```

The driver handles timing via handshake coroutines:
```python
async def send_frame(data, config):
    """Drive beats with random gaps, respecting s_ready."""
    
async def wait_results(n):
    """Collect n results from output handshake."""
```

Comparison happens when results are collected, not every cycle.

## Test Taxonomy

Write tests in these categories. Name each after the requirement:

### 1. Deviation-targeted tests (one per confirmed deviation)
```python
@cocotb.test()
async def test_op_order(dut):
    """REQ-xx: operation order — apply A before B."""
```
- Designed to FAIL on baseline RTL, PASS on fixed RTL
- Use asymmetric values that discriminate the two behaviors

### 2. Change-request tests
```python
@cocotb.test()
async def test_cr1_feature_check(dut):
    """CR-1: width code 3 selects the narrower output width."""
```
- Use acceptance table values from `change_request.md`
- Must pass on fixed RTL (which implements the CR)

### 3. Boundary tests
```python
@cocotb.test()
async def test_len_sat_boundary(dut):
    """REQ-xx: boundary — no saturation just below max, saturation at/above it."""
```
- Hit the exact boundary where comparison operators flip behavior
- Empty/zero cases, max-value cases, wraparound

### 4. Protocol tests
```python
@cocotb.test()
async def test_queue_order_and_backpressure(dut):
    """REQ-xx: output ordering and backpressure."""
```
- Handshake correctness: valid/ready, setup/access phases
- Backpressure: stall output, verify stability
- Reset mid-operation: verify clean recovery

### 5. Constrained-random tests
```python
@cocotb.test()
async def test_random_transactions(dut):
    """REQ-xx: random configs and data, model comparison."""
```
- Random configs within legal range (width ∈ {0,1,2,3}, key within W bits)
- Random data lengths (0-100 bytes typical, occasional long)
- Random backpressure and gaps
- Seeded RNG for reproducibility (`tb = DesignTB(dut, seed=N)`)
- Sweep multiple seeds for coverage

### 6. Register-map tests (control designs only)
```python
@cocotb.test()
async def test_register_map(dut):
    """REQ-xx: full address-space coverage."""
```
- Read/write every mapped address
- Read unmapped → `pslverr=1`, `prdata=0`
- Read at address-decode BOUNDARY: last valid offset (e.g., CH_REPEAT at paddr[4:2]=5), first invalid offset (paddr[4:2]=6 → PSLVERR). Kills off-by-one decode mutants.
- Write read-only → `pslverr=0`, no effect
- Byte strobes: write with partial `pstrb`, verify un-strobed lanes unchanged
- **Strobe discrimination**: write to register with WRONG lane (e.g., strb=0x2 for a register that requires lane 0). Verify register NOT updated. Kills wstrb-index mutations in write decode.
- `paddr[1:0]` ignored

### 7. Strobe discrimination tests (kill byte-lane mutations)
```python
@cocotb.test()
async def test_strobe_discrimination(dut):
    """Each register responds only to its required byte lane."""
    # Write with WRONG lane → no effect
    await tb.write(CH_CTRL, 0b111, strb=0x2)  # wrong lane
    assert await tb.read(CH_CTRL) == 0, "CH_CTRL should NOT be written with strb=0x2"
    # Write with CORRECT lane → updated
    await tb.write(CH_CTRL, 0b101, strb=0x1)  # correct lane
    assert await tb.read(CH_CTRL) == 0b101
    # PRESCALE: test both lane 0 and lane 1 independently
    await tb.write(PRESCALE, 0x00FF, strb=0x1)  # low byte
    await tb.write(PRESCALE, 0x0100, strb=0x2)  # high byte
    assert await tb.read(PRESCALE) == 0x01FF    # both preserved
```

### 8. Address boundary tests (kill decode range mutations)
```python
@cocotb.test()
async def test_address_boundaries(dut):
    """Read at the edge of valid/invalid address ranges."""
    # Last valid channel offset (CH_REPEAT at offset 0x14 within 0x20 stride)
    await tb.read(ch_addr(0, CH_REPEAT))  # mapped, no error
    # First invalid offset within channel block (offset 0x18 → paddr[4:2]=6)
    await tb.read(0x118)  # unmapped, PSLVERR=1
    await tb.read(0x11C)  # unmapped, PSLVERR=1
    # First unmapped channel number
    await tb.read(0x180)  # channel 4, PSLVERR=1
```

## Deliverables (submission tests directory)

Required files in `tests/`:
- `test_submission.py` — cocotb test functions
- `<name>_tb.py` — golden model + driver helpers
- `Makefile` — cocotb Makefile (with ASSERTIONS support)
- `run.sh` — **required** convenience script (flow calls this)

Required file in `docs/designs/`:
- **`<name>-req-matrix.md`** — one row per REQ from spec (`grep -oE 'REQ-[0-9]+' spec.md | sort -u`). Columns: REQ id, description (truncated), test name(s) covering it, status (OK / GAP / UNTABLE). Every REQ must have ≥1 test, OR be marked untestable in simulation with justification (e.g., REQ-42 "no latches" is synthesis-time only).

**Why this matters**: measured on challenge-1, switching from per-line mutant targeting to filling REQ gaps raised V from 0.85 → 1.0 in two iterations. The matrix is the spec-coverage audit — without it you cannot claim REQ completeness. Mutant hunting after the matrix is complete addresses residue only.

## First-pass rule: cover every REQ up front

Once the golden model is built, **writing one test per REQ is ~5 lines each — virtually free per test**, and the coverage is your V and C safety net. Do not write "representative tests" and iterate later. The first mutation run should be a *verification* of complete coverage, not a gap-finding exercise. Iterate to add coverage of edge cases the matrix missed (saturation, priority collisions, mid-stream reset), not to fill REQ gaps.

## Helper Module (`<name>_tb.py`)

Sits alongside `test_*.py` in `tests/`. Importable by test files. Contains:
- Configuration classes / dataclasses
- Reference model (or model function)
- Driver class (handles clock, reset, interface protocol)
- Utility functions (pack beats, compute expected results)
- Statistics access helpers

## Constraints

- Tests use cocotb 2.0 API (`@cocotb.test()`, `cocotb.start_soon`, etc.)
- Python 3.9+ compatible
- No external dependencies beyond cocotb and standard library
- All tests must pass on the fixed RTL with assertions enabled (`--assert`)
- Total test suite runtime < 120s (determines per-mutant timeout in flow)
- Do NOT read RTL source files in the model — model from spec only
- Name tests: `test_req<NN>_<description>` or `test_cr<N>_<description>`

## Timing fragility warning

**Do NOT manually assert on status/interrupt registers** when events re-fire during idle. The cycle-accurate model check IS the verification — it compares outputs every single cycle. Manual asserts on IRQ_STATUS bits are fragile because the event source (period wrap) re-fires.

If a manual assert is truly needed for a deviation test: use LONG period values (≥100) so the event doesn't re-fire within the idle window. Minimum: `period > 2*(APB_cycles_between_writes)`.

**Preferred pattern**: let the model catch deviations. The model's IRQ tracking will diverge from the DUT (mutant or buggy RTL) automatically.

## Makefile for submission tests

```makefile
SIM ?= verilator
TOPLEVEL_LANG = verilog
TOPLEVEL = <top_module>
COCOTB_TOPLEVEL = <top_module>
COCOTB_TEST_MODULES = test_submission
RTL_ROOT ?= ..
RTL_ROOT_ABS := $(abspath $(RTL_ROOT))
VERILOG_SOURCES = $(addprefix $(RTL_ROOT_ABS)/,$(shell grep -v '^\s*\(//\|#\|$$\)' $(RTL_ROOT_ABS)/rtl/filelist.f))
ASSERTIONS ?= 1
ifeq ($(ASSERTIONS),1)
VERILOG_SOURCES += $(wildcard $(abspath ..)/assertions/*.sv)
EXTRA_ARGS += --assert
endif
EXTRA_ARGS += -Wno-fatal --x-assign unique --x-initial unique
include $(shell cocotb-config --makefiles)/Makefile.sim
```

The `run.sh` convenience script:
```bash
#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
make "$@"
python3 - <<'PY'
import sys, xml.etree.ElementTree as ET
t = ET.parse("results.xml")
fails = [c for c in t.iter("testcase") if c.find("failure") is not None or c.find("error") is not None]
sys.exit(1 if fails else 0)
PY
```

## CR-Aware Test Isolation: the `read_masks` Pattern

When the CR adds new fields to a register, older tests that read that register shouldn't fail on the new bits. Use a `read_masks` dict in the driver:

```python
# Tests that don't exercise CR-2 compare the IRQ register on PERIOD bits only
NO_CR = {IRQ_STATUS: 0x000F, IRQ_MASKED: 0x000F, IRQ_ENABLE: 0x000F}

class TB:
    def __init__(self, dut, read_masks=None):
        self.read_masks = read_masks or {}  # addr -> mask; missing = all bits checked
    
    # In _cycle(), when checking read data:
    mask = self.read_masks.get(paddr, 0xFFFFFFFF)
    assert rdata & mask == expected & mask
```

Pass `read_masks=NO_CR` to the TB for tests that don't exercise the CR. Pass no masks (or full 0xFFFFFFFF) for CR tests. This lets the same test file run on baseline AND on CR-implemented RTL.

## Key insight: model ≠ RTL

The golden model implements what the SPEC says. The RTL implements what the hardware does. When they disagree, the RTL has a bug. This is how you detect deviations AND how you kill mutants (the mutated RTL disagrees with the model).
