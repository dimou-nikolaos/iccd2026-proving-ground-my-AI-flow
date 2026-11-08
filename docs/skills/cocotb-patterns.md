# Cocotb Test Patterns: Models, Drivers, and Randomization

Reference for `@test-writer`. Covers the architecture patterns for building golden models and driving designs under test.

## Golden Model Architecture

### Principle: model from spec, not from RTL

The golden model implements what `spec.md` says. It is the oracle. If the RTL disagrees with the model, the RTL is wrong (or has a seeded deviation). This is how we detect bugs AND how we kill mutants.

### Cycle-accurate model (control / register / FSM designs)

Used for: timer / register-map blocks, APB interfaces, FSMs with register maps.

```python
class DesignModel:
    """Cycle-accurate: mirrors spec state machine step-by-step."""
    
    def __init__(self, cr=True):
        self.reset()
        
    def reset(self):
        """All state to reset values from spec section 3."""
        self.gen_q = 0
        self.prescale_q = 0
        self.presc_cnt = 0
        self.status_q = [0] * 4
        self.enable_q = [0] * 4
        # per channel:
        for n in range(4):
            self.en_q[n] = 0
            self.count_q[n] = 0
            self.period_sh[n] = 0
            self.compare_sh[n] = 0
            self.period_act_q[n] = 0
            self.compare_act_q[n] = 0
            self.pcount_q[n] = 0  # one-shot counter
    
    def tick(self):
        """Tick condition, per the spec."""
        return self.gen_q and (self.presc_cnt >= self.prescale_q)
    
    def active(self, n):
        """Output prediction, per the spec."""
        raw = self.en_q[n] and (self.count_q[n] < self.compare_act_q[n])
        return raw ^ self.pol_q[n]
    
    def irq(self):
        """Interrupt prediction, per the spec."""
        return (self.status_q & self.enable_q) != 0
    
    def read(self, paddr):
        """Expected read data, per the register map."""
        # Address decode, return register value or 0 for unmapped
    
    def step(self, psel, penable, pwrite, paddr, pwdata, pstrb):
        """Advance one clock cycle. Apply the spec's state rules IN ORDER."""
        # Mirror the spec's numbered rules exactly, in the order it lists them
        # (e.g. disable rule, load/update rule, wrap rule, increment rule, hold rule)
        # Then: handle register writes, one-shot, IRQ set/clear
```

**The cycle-checker pattern** (driver calls this every clock cycle):
```python
async def _cycle(self, psel=0, penable=0, pwrite=0, paddr=0, pwdata=0, pstrb=0xF):
    """Drive one cycle, check outputs, advance model."""
    # Drive inputs at falling edge
    await RisingEdge(self.clk)
    await ReadOnly()
    
    # CHECK every output against model prediction
    assert int(self.dut.pready.value) == 1
    for n in range(4):
        exp_active = self.model.active(n)
        assert int(self.dut.out_pin.value) & (1 << n) == exp_active
    assert int(self.dut.irq.value) == self.model.irq()
    
    # Check read data if active read access
    if psel and penable and not pwrite:
        exp = self.model.read(paddr)
        assert int(self.dut.prdata.value) == exp
    
    # Advance model
    self.model.step(psel, penable, pwrite, paddr, pwdata, pstrb)
```

**Key advantage for mutation scoring**: every cycle is checked. Any mutation to a signal feeding `out_pin` or `irq` or register reads will break the check within one or few cycles of the mutated path being exercised.

### Event-driven model (datapath / streaming designs)

Used for: streaming datapath, encoders/decoders, transformation pipelines with valid/ready interfaces.

```python
def golden_model(data_bytes, cfg):
    """Pure function: given input bytes + config, compute the expected output per the spec."""
    mask = (1 << cfg.width) - 1
    acc = cfg.init & mask
    for b in data_bytes:
        if cfg.refin:
            b = reflect8(b)
        acc ^= (b << (cfg.width - 8))
        for _ in range(8):
            if acc & (1 << (cfg.width - 1)):
                acc = ((acc << 1) ^ cfg.key) & mask       # cfg.key = generator polynomial / key constant
            else:
                acc = (acc << 1) & mask
    if cfg.refout:
        acc = reflect_n(acc, cfg.width)
    return (acc ^ (cfg.out_mask & mask)) & mask
```

**The driver + collector pattern**:
```python
class DesignTB:
    def __init__(self, dut, seed=0, ready_prob=0.7, valid_prob=0.8):
        self.dut = dut
        self.rng = random.Random(seed)
        self.ready_prob = ready_prob   # output backpressure randomness
        self.valid_prob = valid_prob   # input gap randomness
        self.results = []
        
    async def start(self):
        """Reset, start clock, launch monitor coroutine."""
        ...
        
    async def send_beats(self, beats, cfg):
        """Drive frame beats with random gaps, respecting s_ready."""
        for (word, keep, last) in beats:
            # Apply config on first beat, optionally scramble on later beats
            while not s_ready:
                await RisingEdge(clk)
            # Drive and advance
            ...
            
    async def wait_results(self, n, timeout=10000):
        """Block until n results collected from output monitor."""
        ...
```

**The comparison pattern** (done at result boundaries, not every cycle):
```python
async def run_frames(tb, frames):
    exps = [expected_result(beats, cfg) for beats, cfg in frames]
    for beats, cfg in frames:
        await tb.send_beats(beats, cfg)
    await tb.wait_results(len(exps))
    for got, exp in zip(tb.results, exps):
        assert got == exp
    tb.results.clear()
```

## Driver Templates

### APB4 completer driver

```python
async def write(self, addr, data, strb=0xF):
    """Two-phase APB write: setup cycle + access cycle."""
    # Setup: psel=1, penable=0
    await self._cycle(psel=1, pwrite=1, paddr=addr, pwdata=data, pstrb=strb)
    # Access: psel=1, penable=1
    await self._cycle(psel=1, penable=1, pwrite=1, paddr=addr, pwdata=data, pstrb=strb)

async def read(self, addr):
    """Two-phase APB read, return prdata."""
    await self._cycle(psel=1, paddr=addr)
    await self._cycle(psel=1, penable=1, paddr=addr)
    return prdata_value
```

### AXI-Stream-like driver (valid/ready with keep/last)

```python
async def send_beats(self, beats, cfg):
    for (word, keep, last) in beats:
        self.apply_cfg(cfg)
        while True:
            await RisingEdge(self.clk)
            if int(self.dut.s_ready.value):
                # Drive valid
                self.dut.s_valid.value = 1
                self.dut.s_data.value = word
                self.dut.s_keep.value = keep
                self.dut.s_last.value = last
                await RisingEdge(self.clk)
                break
            # Random gap: hold valid low
            self.dut.s_valid.value = 0
            if self.rng.random() < self.valid_prob:
                break  # retry next cycle
```

## Randomization Patterns

### Constrained random configurations

```python
@dataclass  
class Cfg:
    width: int = 2
    key: int = 0x12345678        # transform key / generator constant
    init: int = 0xFFFFFFFF
    out_mask: int = 0xFFFFFFFF
    refin: int = 1
    refout: int = 1
    
    @staticmethod
    def random(rng, widths=(0,1,2,3)):
        w = rng.choice(widths)
        bits = {0:8, 1:16, 2:32, 3:24}[w]  # one width is typically added by the CR
        mask = (1 << bits) - 1
        return Cfg(
            width=w,
            key=rng.getrandbits(bits) & mask,
            init=rng.getrandbits(bits) & mask,
            out_mask=rng.getrandbits(bits) & mask,
            refin=rng.randint(0,1),
            refout=rng.randint(0,1),
        )
```

### Backpressure and gap injection

- `ready_prob`: probability the testbench accepts results (drives `m_ready`)
- `valid_prob`: probability of driving valid (creates input gaps)
- `ready_hold`: when True, stall output entirely (tests queue depth)
- `cfg_scramble`: when True, change config between beats mid-frame (tests latching)

### Reproducibility

Always seed the RNG: `rng = random.Random(seed)`. This makes test behavior deterministic, which matters because:
- The flow builds with `+verilator+seed+<derived_from_submission_hash>`
- A test that fails non-deterministically may pass on the flow and the mutant won't be killed

## Test Taxonomy (8 categories, write one or more per REQ)

Name each test after the requirement it pins. Categories by mutation-kill value:

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

## Test Naming Convention

```python
# Deviation-specific
test_req05_op_order

# Change request
test_cr1_feature_check

# Boundary
test_req19_len_sat_boundary

# Protocol
test_queue_order_and_backpressure

# Random regression
test_random_frames_all_widths
```

## Makefile Pattern for tests/

```makefile
SIM ?= verilator
TOPLEVEL_LANG = verilog
TOPLEVEL = <top>
COCOTB_TOPLEVEL = <top>
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

## Anti-Patterns

- **Using RTL as model**: don't read RTL internals to compute expected values. That defeats the purpose.
- **Relying on power-up values**: Flow builds with `--x-assign unique --x-initial unique` then runs with `+verilator+rand+reset+0` (registers start at 0). Always reset explicitly. Never test behavior that depends on uninitialized state.
- **Unseeded randomness**: makes test pass/fail non-deterministic across flow runs. Flow uses a SEED derived from your submission hash. Your RNG must be seeded to match the flow's determinism expectations.
- **Overly long tests**: per-mutant timeout is `min(300, max(30, 2*evidence_seconds + 15))`. Keep total under 120s.
- **Not copying public tests**: if you want the public tests to count as evidence, copy them into `tests/`. They are informational (their failures don't reject you) but they exercise the design — you want them in your mutation kill pool.
- **`//` in filelist**: the Makefile grep pattern handles `//` comments; the flow's filelist parser (`read_filelist()`) also handles them. Don't leave a `//` path entry uncommented.
