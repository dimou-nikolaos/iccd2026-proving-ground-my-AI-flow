---
name: cocotb-test-writing
description: Templates and checklist for building Python golden models and writing cocotb tests for RTL designs
---

## Checklist: write tests for a design

### 1. Choose model strategy
- **Control design** (APB, registers, FSM): cycle-accurate model + `_cycle()` checker every clock
- **Datapath design** (streaming, transformation): event-driven model + transaction compare at result boundary

### 2. Build the golden model (from spec ONLY)
- [ ] Read `spec.md`, ignore RTL internals
- [ ] Create model class/function implementing spec algorithm
- [ ] For control: `step()` advances state, `predict()` returns expected outputs
- [ ] For datapath: pure function `(inputs, config) -> expected_output`
- [ ] Include change-request behavior in the model

### 3. Build driver helpers (`<name>_tb.py`)
- [ ] Reset + clock start
- [ ] Interface driver (APB write/read, or streaming send_beats)
- [ ] Monitor coroutine (collect results, randomize ready/valid)
- [ ] Seeded RNG: `rng = random.Random(seed)`

### 4. Write tests by category
- [ ] **Deviation tests**: one per confirmed deviation, discriminating stimulus, FAILS on baseline
- [ ] **CR tests**: use acceptance table values from `change_request.md`
- [ ] **Boundary tests**: saturation edges, zero/empty, max values, off-by-one cases
- [ ] **Address boundary tests**: read/write at the edge of valid/invalid address ranges (e.g., last valid offset, first invalid offset). Kills decode off-by-one mutants.
- [ ] **Strobe discrimination tests**: write to register with WRONG byte lane (e.g., strb=0x2 for a register using strb[0]), verify register NOT updated. Then write with correct lane, verify updated. Kills strobe-check mutations in write decode.
- [ ] **Protocol tests**: backpressure, reset mid-op, output hold, queue depth
- [ ] **Random tests**: 20+ random configs, random data, gaps, multiple seeds
- [ ] **Register-map tests** (if APB): all addresses, PSLVERR, byte strobes, W1C

### 5. Naming
- `test_req<NN>_<description>` or `test_cr<N>_<description>`

### 6. Verify (always use Docker — never host tools)
- [ ] All tests pass on fixed RTL with `--assert`:
```bash
docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-N:/design --entrypoint bash proving-ground/flow:public \
  -c "cd /design/tests && make RTL_ROOT=/design ASSERTIONS=1"
```
- [ ] At least 3 tests fail on baseline RTL
- [ ] Total runtime < 120s
- [ ] Clean `sim_build/`, `results.xml`, `__pycache__` before packaging (tar picks these up otherwise)

## Timing fragility warning

**Do NOT manually assert on status/interrupt registers** when events re-fire. Example: if you write a W1C clear to IRQ_STATUS bit 0, then idle to check it cleared — the period event may RE-FIRE during the idle, re-setting the bit. The model check (cycle-accurate) catches this correctly; manual asserts are fragile.

**If manual asserts are needed**: use `period >> 2*(write_cycles + read_cycles)` so events don't re-fire between action and verification. Use LONG periods (100+) for register tests.

**The model IS the verification**: rely on `_cycle()` model checks. Manual asserts add fragility without adding coverage (the model already checks every cycle).

### 7. Deliverables in `tests/`
- [ ] `test_submission.py` — all tests
- [ ] `<name>_tb.py` — model + driver
- [ ] `Makefile` — with `ASSERTIONS ?= 1` block
- [ ] `run.sh` — **executable** (chmod +x): runs make, then checks results.xml for failures

### Makefile
```makefile
SIM ?= verilator
TOPLEVEL_LANG = verilog
TOPLEVEL = <top>
COCOTB_TOPLEVEL = <top>
COCOTB_TEST_MODULES = test_submission
RTL_ROOT ?= ..
VERILOG_SOURCES = $(addprefix $(abspath $(RTL_ROOT))/,$(shell grep -v '^\s*\(#\|$$\)' $(abspath $(RTL_ROOT))/rtl/filelist.f))
ASSERTIONS ?= 1
ifeq ($(ASSERTIONS),1)
VERILOG_SOURCES += $(wildcard $(abspath ..)/assertions/*.sv)
EXTRA_ARGS += --assert
endif
EXTRA_ARGS += -Wno-fatal --x-assign unique --x-initial unique
include $(shell cocotb-config --makefiles)/Makefile.sim
```

## For deeper context
Read `docs/skills/cocotb-patterns.md` (12KB, model architecture, driver templates, randomization patterns).
