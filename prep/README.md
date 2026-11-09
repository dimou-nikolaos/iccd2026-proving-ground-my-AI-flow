# `prep/` — pre-release verification infrastructure for Challenge-2 and Challenge-3

Everything here was built **before** the design zips were released, against public protocol
and ISA standards. None of it depends on a released register map, port name, CSR map or
interrupt source. On release day the work is *adapter* work (names + register map), not
new infrastructure.

All three trees are self-testing. A green self-test means the infrastructure is trustworthy,
so a later DUT disagreement is a DUT problem, not a BFM problem.

| Tree | What it gives you | Self-test |
|---|---|---|
| `bus_vip/` | cocotb APB4 master BFM, AXI4-Lite master BFM, AXI4-Lite slave memory model with latency/error injection, and a loopback DUT to validate them | `make` (in the flow image) |
| `sva/` | bind-style APB4 + AXI4-Lite protocol assertion modules (all-input ports, L25-safe) | bound into the `bus_vip` self-test, plus `make negative` |
| `rviss/` | RV32IM + Zicsr + M-mode trap/interrupt golden ISS, mini assembler (incl. CR-D3 Zba/Zbb), constrained-random program generator | `python3 -m unittest` (host, no cocotb) |
| `rvcore/` | ISA reference package + 20 per-stage SVA contract assertions over a canonical pipeline tap (bind-safe), synthetic 5-stage harness with injectable deviations, retire-stream lockstep suite, operand-divergence diagnostic | `make` and `make negative` (in the flow image) |
| `dma_model.py` | golden transaction-level 4-channel DMA engine (arbiter, pacing, descriptors, W1C/IRQ, CR-D2) | `python3 -m unittest` (host) |
| `dma_vip/` | DMA-shaped cocotb test environment: register-map dataclass + APB4/AXI4-Lite env + 24 per-REQ evidence bodies + SVA (irq/fsm/protocol binds) + synthetic fixture with 8 seeded deviations and a detection matrix | `make` (24/24) and `make negative` (8/8 caught, irq SVA fires) |
| `flowcheck/` | synthetic design + submission that runs the **real scoring pipeline** end to end (build, public/evidence, mutation, PPA); `flowcheck/dma/` is the DMA-shaped variant (V=0.825, P=0.53 rehearsal) | `./run.sh --skip ppa`, then `./run.sh` |

---

## Running the self-tests

The image is `proving-ground/flow:public`. This account is not in the `docker` group, so use
`sudo -n docker` (see `docs/state/prep-plan.md` §1).

### Bus VIP + protocol SVA (Verilator + cocotb)

```bash
cd prep/bus_vip
sudo -n docker run --rm -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/..:/prep -w /prep/bus_vip --entrypoint bash proving-ground/flow:public \
  -c "make"
```

Expected: `TESTS=4 PASS=4 FAIL=0`.

### Prove the assertions are live (negative test)

```bash
# ... same docker invocation ...
  -c "make negative"
```

Expected: `NEGATIVE TEST PASSED: assertion fired`. This builds a deliberately
non-compliant APB master (keeps `PENABLE` high after `PREADY`) and checks that
`APB4: ACCESS continued after PREADY without a new SETUP` fires. Without this test a green
run could silently mean "Verilator dropped the assertions".

### ISS / assembler / random programs (host python, no cocotb)

```bash
cd prep/rviss && python3 -m unittest test_iss test_randprog -v
```

Expected: 29 tests, OK. Covers M-extension corners (div-by-0, `INT_MIN/-1`), shift masking,
load sign extension, branch signedness, JAL/JALR link + bit 0, `x0`, CSR `rs1=x0` no-write,
misaligned policies, trap entry/MRET, interrupt shadow after MRET, and CR-D3 encodings.

### Per-stage core SVA + retire lockstep (Verilator + cocotb)

```bash
sudo -n docker run --rm -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/prep:/prep -w /prep/rvcore --entrypoint bash proving-ground/flow:public \
  -c "make"
# liveness proof (nine injected deviations, one per assertion family):
sudo -n docker run --rm -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/prep:/prep -w /prep/rvcore --entrypoint bash proving-ground/flow:public \
  -c "make negative"
```

Expected: `TESTS=3 PASS=3 FAIL=0` with **no** assertion fired (0.74 s wall), and
`ALL NEGATIVE CASES PASSED`. `make dbg RVSEED=n` prints ISS vs DUT operands at the first
divergent commit. See `prep/rvcore/README.md` for the assertion inventory, the bind
template and the three SystemVerilog gotchas this exercise proved (L36–L38).

### DMA golden model (host python)

```bash
cd prep && python3 -m unittest test_dma_model -v
```

Expected: 13 tests, OK. Covers word counts 1/2/3, address increments, status/IRQ gating,
W1C, unmapped-address errors, `SIZE = count-1` encoding, priority order/swap, per-word
re-arbitration with request deassert, request resume, 2-descriptor chains, chain end.

### Flow dry run (synthetic design, real pipeline)

```bash
prep/flowcheck/run.sh --skip ppa    # build + public/evidence + mutation, ~20 s
prep/flowcheck/run.sh               # also baseline + submission PPA (OpenROAD)
scripts/mutant-report.py runs/flowcheck-out
```

Expected: `V = 0.975` (39/40; one unkillable 16-bit saturation constant), no assertion
fired on the correct RTL, and a `report.md`/`report.json` pair in `runs/flowcheck-out/`.
The full run (with PPA) took 137 s on this host, of which detailed routing was 110 s, and
scored `P = 0.5` exactly against the identical baseline — the parity check that proves the
baseline-mirror discipline.
This is the rehearsal for release day: it proves the archive layout, the `tests/`
discovery rule, the assertion compilation, the mutant schemata build, the per-mutant
timeout formula and the PPA path all work on this machine.

---

## Release-day adoption

Both designs start with `scripts/design-init.sh <N> <released-bundle-dir>`, which copies and
verifies the bundle, builds the pristine `baseline/` mirror, and seeds the design doc, REQ
matrix and progress file. The sections below are the artifact-specific adapter work.

### Challenge-2 (`dma_ctrl`, APB4 slave + AXI4-Lite master)

1. Copy `bus_vip/apb4.py` and `bus_vip/axi4lite.py` into `challenge-2/tb/`.
2. Map the real port names through the `names=` dict — **do not edit the BFMs**:
   ```python
   apb = APB4Master(dut, dut.pclk, dut.presetn, names=dict(
       psel="p_sel", penable="p_enable", ...))
   mem = AXI4LiteSlave(dut, dut.aclk, dut.aresetn, names=dict(
       awvalid="m_axi_awvalid", ...))
   ```
3. Add `bind dma_ctrl apb4_protocol_sva u_apb_sva (.*);` and
   `bind dma_ctrl axi4lite_protocol_sva u_axi_sva (.*);` (with the real port names if the
   design prefixes them — `.*` binds by name, so rename the observer ports or the bind
   pattern instead of renaming the DUT).
4. Fill `dma_model.RegMap.layout`, `ModelConfig` and the `CTRL_*`/`STATUS_*` bit constants
   from the spec. Every guess is marked `TODO(SPEC)`.
5. Use `mem.log` to check addresses, ordering and data of every engine access.

Notes
- The BFM drives at **falling** edges and samples with `ReadOnly()` in the window that ends
  at the next rising edge, then holds the request through the completion edge. This was a
  real bug found while self-testing (sampling post-edge misses handshakes and deasserts
  `PSEL` before the write commits). Do not "simplify" it back.
- `AXI4LiteSlaveConfig(err_ranges=[(lo, hi, 2)])` injects SLVERR (2) / DECERR (3) to test
  the DUT's error path.
- `wait_idle()` is the clean way to drain the slave before checking memory.

### Challenge-3 (`rv_core`, RV32IM + Zicsr + traps)

1. `prep/rviss/` is importable as-is; adapt `RV32Config` in `rv32_iss.py` to the spec
   (misaligned policy, `mtval` on illegal instruction, read-only CSR write behaviour,
   `MPP` on MRET, which CSRs exist).
2. Bind `prep/rvcore/rv_stage_sva.sv` to the released core and write the 20-line tap map
   (see `prep/rvcore/README.md`): fetch/ID/EX/MEM/WB validity + instruction word + the
   datapath values per stage. The observer does not change; only names do. Start with
   `HAZARD_MODE=0` (value contracts need no schedule knowledge) and enable it only if the
   released microarchitecture confirms the squash/flush schedule in its own words.
3. Wire `asm.py` + `randprog.py` to the released imem/dmem interfaces with a cocotb memory
   model, and adopt the retire-stream lockstep from `prep/rvcore/test_stage.py`. Two
   comparison modes, in order of preference:
   - **commit compare** if the design exposes a retire/commit signal;
   - **final-state compare** otherwise: 31 registers + data memory + `mepc`/`mcause`/`mtval`.
4. Random-program loop: generate → run DUT → run ISS → compare → minimise the failing
   program by replaying prefixes. When a committed value diverges, use
   `prep/rvcore/dbg_operand_diff.py` (`make dbg RVSEED=n`): it says whether an *operand*
   or an *operation* is wrong, which decides whether the deviation is in forwarding or in
   the ALU.

Notes
- `randprog.GenConfig(data_base=...)` places the scratch region well above the code so a
  long program cannot fall through into data (this was a real bug: 1000-instruction
  programs ran past `0x1000`).
- The ISS models CR-D3 instructions behind `RV32Config(zba_zbb=...)`, so the *same* test can
  check that the baseline traps them as illegal and the fixed RTL executes them.

## The V loop (runs entirely locally)

`V` is computed on this machine by `pgflow`, and `stages/mutation.json` lists all 40
selected mutants with their exact mutated expression and kill reason. So V is iterated
like a test failure, not like a server lottery:

```bash
# package + full evidence/mutation run (use --skip ppa for speed)
# then, for a real design:
sudo -n docker run ... pgflow run ... --skip ppa --keep-work
scripts/mutant-report.py runs/<design>-out            # survivors + per-class stats
scripts/mutant-report.py runs/<design>-out --md       # table for the progress doc
# add the assertion/test that tells the mutant apart, re-run, repeat
```

Two rules: fix **classes/expressions**, not mutant IDs (IDs re-roll with the archive hash),
and do not chase mutants that need >30 s of simulation to observe (V ceiling ≈ 0.95–1.0).
`docs/skills/flow-internals.md` has the full operator table and selection algorithm.

## Test/assertion sampling discipline (learned the hard way in the flowcheck dry run)

1. **Registered state**: drive on falling edges, sample at the *next* falling edge. Reading
   right after `RisingEdge` returns the pre-edge value (nonblocking updates land later).
2. **Combinational request-time flags** (`overflow`, `underflow`, `ready`, …): sample with
   `await ReadOnly()` inside the asserted window, before the completing edge.
3. **SVA samples preponed values.** In `@(posedge clk)`, `count` is the value *before* the
   edge and `$past(count)` is the value before the previous edge. Relate "state now" to
   "event that produced it" with `$past(event)`, and remember `$stable(x)` compares this
   cycle with the previous one, so `!$stable(rdata) |-> $past(rd_fire)` is the correct
   "changed only because we read" property — the non-`$past` form is off by one.
4. **Include a mid-operation reset test.** The flow starts registers at zero
   (`+verilator+rand+reset+0`), so a design with a dead reset branch looks perfect at
   time 0. Reset while the design holds nonzero state and check *every* state element.
5. **Keep the evidence suite fast.** Per-mutant timeout is
   `min(300, max(30, 2·evidence_seconds + 15))`; a 45 s suite wastes 105 s on each of 40
   mutants and slows every iteration.

---

## Known unknowns (`TODO(SPEC)` index)

Search for `TODO(SPEC)` — every hit is a design decision that must come from the released
spec, not from a guess:

| File | Guess |
|---|---|
| `dma_model.py` | register offsets/stride, `CTRL_*`/`STATUS_*` bit positions, `SIZE` count encoding, status clear rule (W1C vs W0C), request level vs pulse, re-arbitration granularity, descriptor field layout and "more" bit, unmapped-address response, interrupt pending formula (transfer vs descriptor complete, shared vs separate) |
| `rv32_iss.py` | misaligned behaviour (fault/ignore/support), `mtval` on illegal instruction, read-only CSR write behaviour, `MPP` after MRET, which CSRs are implemented, interrupt priority/vectoring, MRET shadow rule |
| `apb4.py` / `axi4lite.py` | none — protocol behaviour is standard; only the port-name mapping changes |
| `sva/*.sv` | none — standard protocol rules |
| `rvcore/rv_stage_sva.sv` | the *names* of the stage taps, and whether the core flags illegal instructions at decode (`CHECK_ILLEGAL`), implements CR-D3 (`CR_D3`), and has a documented squash schedule (`HAZARD_MODE`) |
| `rvcore/stage_harness.sv` | scratch fixture only — its hazard/forwarding schedule is **our** choice, not the released core's. Never ship it, never treat it as a reference |

## Environment note

All simulations must run inside `proving-ground/flow:public` (host Verilator is 5.006 and too
old). Verified in the image on 2026-11-08: Verilator 5.048, cocotb 2.0.1, Yosys 0.68, SBY,
OpenROAD, `pgflow`. The host python (3.10.12) has no cocotb, which is why the ISS and DMA
model tests are pure-python.
