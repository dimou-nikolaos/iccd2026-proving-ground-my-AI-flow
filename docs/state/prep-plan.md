# Pre-release preparation plan — Challenge-2 (dma_ctrl) and Challenge-3 (rv_core)

**Status: PREP COMPLETE — waiting for the Challenge-2 release (Nov 10, 09:00 HKT).**
**Last updated 2026-11-08 (Sun).**
This file is the compaction-safe source of truth for *preparation work* done before the
Challenge-2 and Challenge-3 design zips are released. Per-design progress files
(`challenge-2-progress.md`, `challenge-3-progress.md`) will be created at release time and
take over; this file then becomes the prep log.

**Release-day plan (submission budget, Day-0 timeline, Explorer safety doctrine):
`docs/state/release-strategy.md` — read it before the first push.**

---

## 0a. BATCH B/C/D/E — COMPLETE (all green, committed `0f9bd60` + `a5e6c20`, not pushed)

Started 2026-11-08, operator away for hours, **work autonomously, do NOT `git push`**
(push happens later, by operator request). Four items, in build order:

| ID | Deliverable | Where | Status |
|---|---|---|---|
| **B** | Program shrinker: delta-debug a failing DUT-vs-ISS program to a minimal repro | `prep/rviss/{shrink,diff,fake_cores}.py` + 3 test files | **DONE** (13+12+3 tests OK) |
| **C** | Trap / CSR / misalign / illegal-encoding / divide / call-return program mode | `prep/rviss/randprog.py` (`trap_config()` + new fields) | **DONE** (29 ISS + 5 randprog tests OK) |
| **E** | DMA cocotb test skeleton driven by a register-map dict, per-REQ bodies, `TODO(SPEC)` constants | `prep/dma_vip/` (rtl fixture + tests env + SVA + negative matrix) | **DONE** (`make` 13/13 in 0.22 s sim; `make negative` 8/8 deviations caught + irq SVA fires) |
| **D** | DMA-shaped synthetic design (APB4 + AXI4-Lite) run through the **real** pgflow scoring pipeline with our bus VIP + bound protocol SVA + one seeded deviation | `prep/flowcheck/dma/` | **DONE** (`--skip ppa`: public+evidence pass, V 0.675→0.825 by adding the decode/don't-care/error probes below; full run incl. PPA: P=0.528, total 234 s) |

Rules learned earlier that still apply: host python is pure-python only (no cocotb) → B/C/E-model
tests run on host; anything with cocotb/Verilator runs in the flow image via `sudo -n docker`.
Keep every self-test in the low single-digit seconds — **exception**: the host-side shrink
suite (`test_shrink`) is ~75 s, and it is not part of the 10-minute submission evidence budget.

### B/C evidence (all 2026-11-08, host python, in `prep/rviss`)

| Command | Result |
|---|---|
| `python3 -m unittest test_iss test_randprog test_diff` | `Ran 41 tests OK` (0.5 s) |
| `python3 -m unittest test_detect_matrix` | `Ran 3 tests OK` (8 s) + prints the deviation triage map |
| `python3 -m unittest test_shrink` | `Ran 13 tests OK` (76 s) |

New assets:

- `diff.py` — normalized commit stream (`Step`/`Ev`), coarse `instr_class`,
  `first_divergence` (trace vs trace) and **`compare_run`** (lockstep with early exit at
  the divergence and at the final self-loop → ~1 ms per oracle call). A divergence carries
  a **structural signature** `(kind, instr_class, effect_kind)` that survives shrinking
  (values change, PCs move) — that is what keeps a minimal repro about the same bug.
- `fake_cores.py` — 20 deliberately wrong cores: 8 policy variants (`RV32Config` knobs) +
  12 seeded architectural deviations (lb zero-extended, sra logical, div-by-zero,
  INT_MIN/-1, mulhsu signedness, signed compares done unsigned, jal drops link, sb/sh
  clobbers the word, mepc off-by-one, ebreak-as-nop, sh3add wrong shift, min/max swapped).
  `NOT_REACHABLE` records the two classes the *serial* method cannot reach (vectored mtvec
  needs irq driving; mcycle/minstret are not comparable ISS-vs-RTL).
- `test_detect_matrix.py` — proves every reachable catalogue entry is *seen* by at least one
  of 4 program profiles (plain / cr / mext / corners) and prints the triage map: the negative
  test of the ISS-side method (analogue of `make negative` for SVA).
- `shrink.py` — n-wise ddmin + two-line removals + literal minimization + register
  canonicalization (deliberate aliasing) + spelling cleanup, oracle memoization, call budget,
  and `make_keep()` signature pinning. On 250-285-line random programs: CR-D3-missing →
  `max t0, t1, t2`; div-by-zero → `div t0, t1, t2`; misaligned store → `sw t0, 1(t1)`;
  ebreak → `ebreak`; sign-extension → `lb t0, 0(t1)` (0.1-3 s each).
- `randprog.trap_config()` — corner profile: emitted machine-mode trap handler (logs
  mcause/mepc/mtval into a data-memory window, bails after `max_traps`), misaligned and
  sub-word accesses, guaranteed-illegal encodings, ebreak, CSR corners (non-existent CSR,
  read-only writes), divide corners (`x0` divisor, INT_MIN/-1) and a jal/jalr call-return
  block. The handler uses only sp/gp/tp so the random body cannot clobber it.

### B/C findings (day-1 relevant)

1. **asm.py trap, found and fixed**: `li rd, label` with a *forward* reference silently
   encoded 0 (labels are resolved during expansion, before pass 2). It now raises.
   Consequence: generated programs bake numeric addresses for `mtvec`, not labels.
2. A naive "number" regex rewrites digits **inside register names** (`t0` → `0`). Any tool
   that edits assembly needs `(?<![A-Za-z_0-9'])...(?![A-Za-z_0-9'])` lookaround.
3. Immediate fields are narrower than the literals: a bigger number can assemble to the
   *same* instruction, so a shrinker that only asks "does it still fail?" drifts to absurd
   values (`0x7fffffff` in a 12-bit immediate). Guard: reject a literal change whose
   assembled words are unchanged, unless it also shortens the value.
4. ddmin granularity: chunk size = `len//n`, so **increase** n to make chunks smaller.
   Getting it inverted silently under-shrinks (dead code survives).
5. Pin the reference signature from the **original** program (probe it once before the first
   candidate), otherwise the shrink can converge onto a *different* latent deviation that
   only appeared once the noise was removed.
6. Reaching a deviation is seed-dependent (sign-extension needs a negative byte in the
   window). Day-1: run several profiles/seeds per deviation; tests accept a candidate list.
7. Shrinking against the **RTL** later is feasible but ~1 s per oracle call with cocotb;
   the fast motion is to shrink *inside one simulation* (re-load imem through the harness
   load port, ~ms per candidate). Write that template once the real core's ports exist.

### E/D evidence (2026-11-08, flow image)

| Command | Result |
|---|---|
| `cd prep/dma_vip && make` | `TESTS=24 PASS=24 FAIL=0` (0.4 s wall) — clean fixture, assertions bound and silent |
| `cd prep/dma_vip && make negative` | 7/7 bug bits caught by probes (stage 1) + `dma_irq_sva` fires on the level-irq bug (stage 2) |
| `cd prep/bus_vip && make` | `TESTS=4 PASS=4` (after the VIP `resp_when_idle` addition) |
| `cd prep/flowcheck/dma && ./run.sh --skip ppa` | scored, evidence pass, **V=0.825** (was 0.675 with the first skeleton) |
| `cd prep/flowcheck/dma && ./run.sh` | scored: **V=0.825, P=0.528** (submission re-ties `bug`→0 vs baseline), 234 s |

New assets:

- `prep/dma_vip/rtl/dma_ctrl.sv|dma_top.sv` — synthetic APB4+AXI4-Lite linked-descriptor
  DMA with an 8-bit `bug` deviation selector (bit0 LEN±1, bit1 RO-writable, bit2 chain-end
  ignored, bit3 DONE-never/BUSY-stuck, bit4 descriptor race, bit5 level irq, bit6 narrow
  strobes, bit7 wrong stride). Lint-clean (`-Wall`) and Yosys-check-clean.
- `prep/dma_vip/tests/dma_env.py` — `Regmap` dataclass (all `TODO(SPEC)` fields in one
  place) + env wiring APB4Master + AXI4LiteSlave(mem) + irq-width monitor + boundary
  checkers (`expect_block_moved`, `expect_write_addresses`, `expect_all_strobes`,
  `expect_irq_pulses`) + fast-fail `wait_done` (DmaTimeout) with bus drain.
- `prep/dma_vip/tests/test_dma.py` — 24 per-REQ bodies (regs/boundaries/backpressure/
  chain/status/irq/W1C/error-injection/reset), designed as the day-1 skeleton: bodies
  unchanged, only `Regmap` + `AXI_NAMES` get filled from the released spec.
- `prep/dma_vip/tests/test_dma_negative.py` — per-`bug`-bit detection probes with
  `DMA_NEG_ONLY/SKIP` selection (the level-irq probe intentionally aborts the sim via
  the bound assertion; Makefile splits the two stages).
- `prep/dma_vip/tests/dma_assertions.sv|dma_bind.sv` — irq-pulse + FSM-state/AXI-issue
  legality + APB/AXI protocol observers, bound with explicit maps (L25).
- `prep/flowcheck/dma/` — pgflow rehearsal: design/ (baseline w/ seeded `bug=8'h01` +
  lenient public tb that does NOT catch it), sub/ (fixed RTL + evidence + assertions).
  `run.sh` does not re-copy rtl (submission differs from baseline by the fix).
- `prep/bus_vip/axi4lite.py` gained `resp_when_idle` (latched error lane) config knob.

### E/D findings (day-1 relevant)

1. Yosys `check` (flow's equiv/slang path) flags multi-block sequential writers that
   Verilator accepts → one owner per register (L39). Fixture rewritten accordingly.
2. Ungated `PREADY=1` trips our own APB SVA; slave responses must be selection-qualified
   (L40). `wait_done` must drain the bus before memory checks (L39a).
3. The first-pass skeleton killed only 0.675 of 40; the kill-rate jumped by systematically
   adding the L41 probe families (reset read-back, CTRL bit read-back, don't-care-lane
   probes, error injection in R **and** B channels, chain LEN=0 descriptor, ERR W1C
   minimal strobe). Expect the same +0.15..0.25 on the real design for the same probes.
4. Fixture V ceiling ≈0.85: 6-8/40 mutants land on dead/constant-tied sites (the `bug`
   port itself!) and are unkillable (L42). The real design has no such hooks — do not
   extrapolate 0.825 to challenge-2; treat it as a lower bound of what the *suite shape*
   achieves, and re-check each survivor class on the real RTL.
5. `mem.log` (AXI VIP) accumulates across the whole test — `clear()` it before probing
   "no traffic after X" (self-inflicted false failure).
6. pgflow details confirmed on a bus design: `tb/public` needs no Makefile (flow
   discovers `test_*.py`); participant tests run with only `tests/` on pythonpath →
   **vendor** `apb4.py`/`axi4lite.py`/`dma_env.py` into `tests/`; assertion `.sv` files
   anywhere under `assertions/` are compiled+bound; a bus-ported top (25 IO) passes
   OpenROAD PPA fine with the tiny-design sdc.

## 0. Situation

- Challenge-1 is complete (C=1.0 banked per prior state; artifacts are gitignored and not in
  this checkout). All reusable knowledge is consolidated in `docs/flow-lessons.md` (L1–L43).
- Challenge-2 (`dma_ctrl`) releases **Nov 10, 2026, 09:00 HKT**; challenge-3 (`rv_core`)
  releases **Nov 13, 2026, 09:00 HKT**; both close **Nov 16, 23:59 HKT**.
- The design zips are **not downloadable yet** (Drive folders render empty / JS-only). We
  therefore prepare against the official public descriptions + standard protocol/ISA specs.
  **If the zips appear before release, abandon prediction work and start Phase-1 analysis.**
- Submission budget: 30/design, 1 per 30 min. Cooldown is not the binding constraint;
  analysis + verification time is.

## 1. Environment facts (verified 2026-11-08)

| Item | Fact |
|---|---|
| Flow image | `proving-ground/flow:public` present (4.29 GB) |
| Docker access | Account `ndimou1` is **not** in the `docker` group; `sudo -n docker ...` works. Use `sudo -n docker` for all flow/sim commands. |
| Toolchain in image | Verilator 5.048, cocotb 2.0.1, Yosys 0.68, SBY, OpenROAD, `pgflow` |
| Host python | 3.10.12, **no cocotb** → host python is only for pure-Python unit tests (ISS, assembler) |
| Cores | 16 (use `--cpus 12 --jobs 12` for flow runs per AGENTS.md) |

Canonical docker invocation (note `sudo -n`):

```bash
sudo -n docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/<design>:/design --entrypoint bash proving-ground/flow:public \
  -c "cd /design/tb/public && make"
```

For `prep/` self-tests, mount the prep tree (the SVA observers live one directory up from
the bus VIP, so mount `prep/` and work in `/prep/bus_vip`):

```bash
cd prep/bus_vip
sudo -n docker run --rm -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/..:/prep -w /prep/bus_vip --entrypoint bash proving-ground/flow:public -c "make"
```

### 1.1 Self-test evidence (all run 2026-11-08)

| Suite | Command | Result |
|---|---|---|
| Bus VIP + bound protocol SVA | `make` in `prep/bus_vip` (flow image) | `TESTS=4 PASS=4 FAIL=0` |
| SVA liveness (negative) | `make negative` in `prep/bus_vip` | assertion fires: `APB4: ACCESS continued after PREADY without a new SETUP` |
| ISS + assembler | `python3 -m unittest test_iss -v` (host, in `prep/rviss`) | 25 tests OK |
| Random program generator | `python3 -m unittest test_randprog -v` (host) | 4 tests OK (incl. 1000-instruction program) |
| DMA golden model | `python3 -m unittest test_dma_model -v` (host, in `prep`) | 13 tests OK |
| **RV32 stage-contract SVA + retire lockstep** | `make` in `prep/rvcore` (flow image) | `TESTS=3 PASS=3 FAIL=0`, no assertion fired, 0.74 s wall |
| RV32 observer liveness (negative) | `make negative` in `prep/rvcore` | 9/9 injected deviations trip an assertion |
| **Flow dry run, no PPA** | `prep/flowcheck/run.sh --skip ppa` | `status=scored`, `V=0.975`, evidence 0.5 s, mutation 11 s |
| **Flow dry run, full** | `prep/flowcheck/run.sh` | build+public+mutation+OpenROAD PPA on a real ORFS run |

The dry run rehearses the release-day pipeline on a synthetic design (`prep/flowcheck/`):
archive layout, `tests/` discovery, assertion compilation with `--assert`, mutant schemata,
the per-mutant timeout formula and the ORFS path. It found three issues that would have
cost release-day hours:

1. Evidence tests sampled registered state right after `RisingEdge` (pre-edge value) — off
   by one; fixed with the falling-edge discipline (§4 discipline in `prep/README.md`).
2. Two SVA properties were off by one because SVA samples **preponed** values:
   `count == $past(count) + wr_fire - rd_fire` must use `$past(wr_fire)`/`$past(rd_fire)`,
   and `$stable(rdata)` compares against the previous cycle, so it pairs with
   `$past(rd_fire)`.
3. A synthetic SDC using `remove_from_collection` aborted OpenROAD synthesis (OpenSTA does
   not implement that command). Harmless for the competition (the SDC is fixed), but it
   documented the real PPA toolchain.

The self-tests earned their keep immediately: they exposed **two real BFM bugs** that would
have produced false "DUT deviation" reports on release day.

1. The BFMs sampled handshake signals **post-edge** (`await RisingEdge; await ReadOnly`).
   AXI READY deasserts in the same edge as the handshake, so the master BFM missed every
   handshake and timed out; the APB BFM observed `PREADY` one cycle early and deasserted
   `PSEL` **before** the slave committed the write (data silently lost, looked like a DUT
   bug). Fixed by driving at falling edges, sampling with `ReadOnly()` in the window that
   ends at the next rising edge, and holding the request through the completion edge.
   This discipline is documented in both BFM docstrings — do not "simplify" it back.
2. The generated random programs (1000 instructions) ran past the `0x1000` scratch-data
   region; the ISS took `mcause=2` (illegal instruction) and the test looked like an ISS
   bug. Fixed by moving `data_base` to `0x8000`.

## 2. Confirmed design facts (from organizers' public descriptions)

### Challenge-2 — `dma_ctrl` (~780 lines, 10 ns sky130hd)

- Copies 32-bit words between memory locations for **4 channels**.
- Software programs it through an **APB4 register interface** (slave).
- **One shared engine** performs every transfer through an **AXI4-Lite master**.
- Channels have **priorities**, can be **paced by peripheral request lines**, and can follow
  **linked descriptors stored in memory**.
- **CR-D2: per-channel descriptor-complete interrupt.**

### Challenge-3 — `rv_core` (~1,200 lines, 15 ns sky130hd)

- **5-stage** pipeline: fetch / decode / execute / memory / writeback.
- **RV32I** base + **M** extension + **Zicsr** + **machine-mode traps and interrupts**.
- **Separate instruction and data memory interfaces**.
- Public testbench includes a **small Python assembler**.
- **CR-D3: add seven address-generation and min/max instructions from Zba and Zbb.**
  Verified against the ratified bit-manipulation spec, these are exactly:

  | Instr | opcode | funct7 | funct3 | Semantics (RV32) |
  |---|---|---|---|---|
  | SH1ADD | 0x33 | 0010000 | 010 | rd = (rs1<<1) + rs2 |
  | SH2ADD | 0x33 | 0010000 | 100 | rd = (rs1<<2) + rs2 |
  | SH3ADD | 0x33 | 0010000 | 110 | rd = (rs1<<3) + rs2 |
  | MIN    | 0x33 | 0000101 | 100 | signed min |
  | MINU   | 0x33 | 0000101 | 101 | unsigned min |
  | MAX    | 0x33 | 0000101 | 110 | signed max |
  | MAXU   | 0x33 | 0000101 | 111 | unsigned max |

  All arithmetic R-type, results mod 2^32, no exceptions.

> These are **descriptions, not the spec**. On release, the spec text is the only authority.
> Treat the above as priors that tell us what to prepare; re-derive everything from the zip.

## 3. Strategy in one paragraph

Both designs are **bus/protocol-heavy**, and both have a large share of their verification
surface in behavior that is hard to poke pin-by-pin (APB/AXI handshakes; traps/interrupts).
The leverage is therefore in **pre-built verification infrastructure**, not in guessing RTL
lines: (a) protocol BFMs and SVA templates for APB4 + AXI4-Lite, (b) a golden-cycle model
skeleton for a DMA engine, (c) a Python RV32IM+Zicsr ISS + assembler + constrained-random
program generator for lockstep comparison against the core. Deviation hunting then starts on
day 1 from a working harness; the generic deviation catalogs in the two playbooks route the
static analysis. PPA: same 10 ns budget as challenge-1 on ch2 (lessons transfer 1:1); ch3 has
15 ns (likely positive slack → area/power game, and cycle-count changes are probably
architecturally observable → Tier C only with C-check).

## 4. Work breakdown (status)

Legend: `[ ]` todo, `[~]` in progress, `[x]` done, `[-]` dropped/deferred.

### Track P — documentation

- [x] **P-1** Rewrite `docs/designs/challenge-2-playbook.md` with confirmed facts, DMA-specific
  deviation catalog, CR-D2 analysis, prep-artifact map. (2026-11-08)
- [x] **P-2** Rewrite `docs/designs/challenge-3-playbook.md` with confirmed facts, RV-specific
  deviation catalog, CR-D3 encodings/test matrix, PPA reframing for 15 ns. (2026-11-08)
- [x] **P-3** This plan file; link it from `AGENTS.md` resume recipe.
- [ ] **P-4** Fetch official reference PDFs into `docs/ref/` (RV32 unprivileged, privileged,
  bit-manipulation Zba/Zbb) for offline cross-checking at ch3 time.

### Track V — bus verification IP (challenge-2 leverage)

- [x] **V-1** `prep/bus_vip/apb4.py` — cocotb APB4 master BFM (+ a slave model for
  master-DUT testing). SETUP/ACCESS, wait states, error sampling, byte strobes.
- [x] **V-2** `prep/bus_vip/axi4lite.py` — AXI4-Lite master BFM + slave memory model
  (independent AW/W/AR channels, per-channel READY delay + jitter, SLVERR/DECERR injection,
  transaction log). Master BFM tracks AW and W independently and drives `bready`/`rready`
  only while waiting for a response.
- [x] **V-3** `prep/bus_vip/dut_loopback.sv` + `test_vip_selftest.py` — loopback DUTs +
  4 cocotb tests, all green in the flow image.
- [x] **V-4** `prep/sva/apb4_protocol_sva.sv` (7 assertions) and
  `prep/sva/axi4lite_protocol_sva.sv` (10 assertions) — bind-style, all-input ports (L25),
  `default disable iff (!rst_n)`, explicit `@(posedge clk)` (no clocking block).
- [x] **V-5** Assertions verified live in Verilator 5.048 with `--assert`: all 7 APB and 10
  AXI checks appear in the generated C++, they do not fire on the compliant BFMs, and
  `make negative` (broken APB master) makes `APB4: ACCESS continued after PREADY without a
  new SETUP` fire. This proves a green run is meaningful.
- [x] **V-6** `prep/dma_model.py` — golden transaction-level DMA engine (4 channels, priority
  arbiter, per-word re-arbitration + request pacing, descriptor chain, W1C/IRQ, CR-D2
  descriptor-complete) + `prep/test_dma_model.py` (13 tests, host python, all green).
  Register map / control bits are placeholder knobs marked `TODO(SPEC)`.

### Track R — RISC-V verification infrastructure (challenge-3 leverage)

- [x] **R-1** `prep/rviss/rv32_iss.py` — RV32IM + Zicsr + M-mode trap/interrupt ISS with
  configurable policies (`RV32Config`: misaligned fault/ignore/support, `mtval` on illegal,
  RO-CSR write, MPP on MRET, `zba_zbb` on/off, vectored mtvec).
- [x] **R-2** `prep/rviss/asm.py` — two-pass assembler/encoder (RV32I + M + Zicsr + SYSTEM +
  Zba/Zbb CR instructions, labels, `.org`/`.word`/`.align`, CSR pseudo-ops `csrr/csrw/csrs/
  csrc/csrrwi/...`, standard pseudo-ops).
- [x] **R-3** `prep/rviss/randprog.py` — constrained-random program generator
  (hazard-dense dst reuse, ALU/shift/M/load-store/branch mixes, optional CR-D3 ops,
  deterministic per seed) + `test_randprog.py` (4 tests incl. a 1000-instruction program).
- [x] **R-4** `prep/rviss/test_iss.py` — 25 unit tests: assembler round-trips, ALU/shift
  masking, load sign extension, branch signedness, JAL/JALR link+bit0, `x0`, M-extension
  corners (div-by-0, `INT_MIN/-1`, `MULHSU`), CSR `rs1=x0` no-write, RO-CSR trap,
  misaligned policies, ECALL/illegal trap entry + MRET, interrupt taken and shadowed,
  CR-D3 semantics and the "traps as illegal when disabled" case.
- [~] **R-5** Cocotb adapter for the core (imem/dmem memory model + lockstep compare) —
  **partially built, deliberately not finished**: `prep/rvcore/` now contains the
  retire-stream lockstep driver (`test_stage.py`: fetch → imem load port, per-retire
  compare against `rv32_iss`, final register + memory image compare, NOP-fill + dmem zero)
  and a self-testing synthetic 5-stage core (`stage_harness.sv`). What still needs the
  released top-level port list is the *memory/tap wiring* of the real core, not the method.
- [x] **R-6** `prep/rvcore/rv_ref_pkg.sv` — pure ISA reference functions used by both the
  SVA observer and (independently re-implemented) harness: ALU incl. M + CR-D3, all five
  immediate formats, branch compare, load extension, legality, `writes_rd`, access size.
- [x] **R-7** `prep/rvcore/rv_stage_sva.sv` — 20 concurrent assertions over a canonical
  pipeline tap (IF/ID/EX/MEM/WB), all-`input` and bind-safe, parameterized
  (`IALIGN32`, `CHECK_ILLEGAL`, `HAZARD_MODE`, `CR_D3`). Verified live: nine injected
  deviations (one per assertion family) each trip an assertion; the clean build fires none.
  Release-day work is a 20-line bind map, see `prep/rvcore/README.md`.

### Track X — flow rehearsal and release-day tooling

- [x] **X-1** D2 runbook (§7 below; adoption steps also in `prep/README.md`).
- [x] **X-2** D3 runbook (§7; ISS-first strategy in `docs/designs/challenge-3-playbook.md`).
- [x] **X-3** `prep/flowcheck/` — synthetic design + submission that runs the real pipeline
  end to end. Found the test-sampling bug, the two SVA preponed-sampling bugs and the
  OpenSTA SDC limitation (§1.1).
- [x] **X-4** `docs/skills/flow-internals.md` — verified `design.yaml` schema, archive
  rules, build reject list, mutation operator table + selection algorithm, evidence timing
  rules, PPA score math (weights 0.4/0.4/0.2, `T = period − WNS`, timing cap 0.3).
- [x] **X-5** `scripts/mutant-report.py` — turns `stages/mutation.json` into a survivor
  table with exact `original -> mutated` expressions and per-class kill stats. This makes
  V a **local** iteration loop (no server submission needed).
- [x] **X-6** `scripts/design-init.sh` — one-command release-day bootstrap (copy bundle,
  verify layout, baseline mirror, design doc, REQ matrix pre-filled with every REQ id,
  state file, printed next commands). Tested end to end in a scratch tree (idempotent).

### Track X — release-day runbooks

- [x] **X-1** D2 runbook (§7 below; adoption steps also in `prep/README.md`).
- [x] **X-2** D3 runbook (§7; ISS-first strategy in `docs/designs/challenge-3-playbook.md`).

## 5. Decision log

| Date | Decision | Rationale |
|---|---|---|
| 2026-11-08 | Build verification IP before designs exist | Protocol semantics are public standards; BFMs/SVA transfer unchanged. Removes ~1 day from each design's critical path. |
| 2026-11-08 | ISS before core release | Differential random-program testing is the highest-yield deviation finder for a CPU (L1 generalized). Base ISA/M/CSR-instruction semantics are standard; trap/CSR-map details are configurable. |
| 2026-11-08 | Use `sudo -n docker` for all flow commands | Account lacks docker group; sudo works non-interactively. |
| 2026-11-08 | Do **not** assume challenge-1 artifacts are available | `challenge-1/`, state files, reports are gitignored; rely on `docs/flow-lessons.md`. |
| 2026-11-08 | BFMs drive on falling edges, sample with `ReadOnly()` mid-window, hold through the completion edge | Post-edge sampling is off by one cycle: it misses AXI handshakes and completes APB before the write commits. Found by the self-test, see §1.1. |
| 2026-11-08 | DMA golden model derives `int_status` from per-channel event bits | One source of truth; avoids a pending-register model that disagrees with `STATUS` readback. The real relationship is a `TODO(SPEC)`. |
| 2026-11-08 | SVA observers verified with a deliberate-violation test | Verilator silently drops assertions without `--assert`; a green run must be proven meaningful once. |
| 2026-11-08 | Treat V as a **local** iteration loop, not a server round-trip | `stages/mutation.json` contains all 40 mutants with the exact mutated expression and kill reason; `scripts/mutant-report.py` renders them. Fix the class, re-run, repeat. |
| 2026-11-08 | Every test suite includes a mid-operation reset test | The flow starts registers at zero (`+verilator+rand+reset+0`), so a design with a dead reset branch is indistinguishable at time 0. |
| 2026-11-08 | Evidence suite budget: a few seconds | `per_mutant_timeout = min(300, max(30, 2·evidence_seconds + 15))`; long evidence directly slows mutation and reduces usable per-mutant time. |
| 2026-11-08 | Synthetic flow rehearsal lives in `prep/flowcheck/`, never in `challenge-N/` | Keeps filelist/port checks and caches clean; the rehearsal is disposable. |
| 2026-11-08 | Assert **stage contracts from the ISA**, attach with `bind` through a release-day signal map; keep the observer's port list canonical | The contract (given the instruction word in a stage, every datapath value has one legal value) is ISA-determined and writable pre-release; only the *signal names* are unknown. 20 assertions + 9-deviation proof built and verified now. |
| 2026-11-08 | Pair stage SVA with a retire-stream lockstep against `prep/rviss` | Neither layer alone kills silent mutants: a stage check cannot see a dropped/duplicated/speculatively-retired instruction; a retire check cannot see a wrong value that is overwritten before commit. |
| 2026-11-08 | Do **not** build a cycle-accurate model of the unreleased core | It would encode a guessed stall/flush/forward schedule as the golden reference and generate false deviations. Lockstep pins the architectural stream; tap-level timing assertions get added once the released spec/RTL documents the schedule. |
| 2026-11-08 | NOP-fill imem and zero dmem before every core run | A pipelined core fetches wrong-path words; X there makes assertions fire for unrelated reasons, and stale dmem across programs makes a correct DUT look deviant (L38). |

## 6. Risk register

| # | Risk | Impact | Mitigation |
|---|---|---|---|
| R1 | Design zips appear earlier than announced | Wasted prediction effort | Check links first; if live, switch to real analysis immediately |
| R2 | Ch3 trap/CSR semantics differ from standard priv spec | Wrong golden model → false deviations | ISS policies configurable; spec text overrides on release; one test per REQ |
| R3 | 15 ns ch3 makes timing slack large → PPA levers differ from ch1 | Misallocated PPA time | Measure first (WNS + area breakdown); assume area/power game; do not add pipelines reflexively (L16) |
| R4 | Cycle-count changes on ch3 are externally observable | C regression on Tier B/C | Treat cycle-accurate behavior as frozen unless spec silent; C-check every such change (L4) |
| R5 | Docker/group friction burns release-morning time | Lost hours | `sudo -n docker` validated today; keep canonical command in §1 |
| R6 | APB4/AXI4-Lite BFM bugs masquerade as DUT bugs | False deviation reports | V-3 self-test against a loopback DUT before use |
| R7 | Hidden tests expect assembly programs the public assembler can't emit | Coverage gap | Own assembler (R-2) covers full ISA incl. CR instructions |
| R8 | An assertion is off by one and fires on the correct RTL | Evidence rejection, wasted submission | Preponed-sampling rules validated in `prep/flowcheck`; every new assertion checked with a deliberate-violation or mutant run before packing |
| R9 | Evidence suite too slow | Small per-mutant timeout, slow mutation stage | Measure `evidence_seconds`; keep the suite in the low single-digit seconds |
| R10 | Unkillable constant/equivalence mutants cap V | ~0.025 V lost | Accept the ceiling; do not add 65k-cycle tests to chase one mutant |

## 7. Release-day runbook (D2, Nov 10)

1. `scripts/design-init.sh 2 <released-bundle-dir>` — copies the bundle, verifies the
   layout, builds the pristine `baseline/` mirror, and creates the design doc, the REQ
   matrix (pre-filled with every REQ id) and the progress file.
2. Public test smoke run (command printed by the script), then cache the baseline PPA.
3. Copy `prep/bus_vip/*` into `challenge-2/tb/`, wire signal names (top port names are known
   only after unzip). Run V-3 self-test semantics against the real top module's APB/AXI ports.
4. Phase 1: spec-analyst against `spec.md`; use playbook deviation catalog as a checklist.
5. Phase 3: one test + one assertion per REQ (L12), plus `docs/designs/challenge-2-req-matrix.md`.
6. Implement CR-D2; separate assertions file for new signals.
7. First evidence run with `--skip ppa` + `scripts/mutant-report.py`; close the V gaps you can
   see (empty `reason` = survivor), then submit.

D3 runbook is the same shape, with the ISS replacing the DMA model and the random-program
diff as the primary Phase-1 probe.

## 8. Change log

| Date | Change |
|---|---|
| 2026-11-08 | Created. Playbooks rewritten (P-1/P-2). Docker/toolchain verified. Bus VIP, SVA templates, RV ISS started. |
| 2026-11-08 | Read the installed `pgflow` package and ran a full pipeline rehearsal (`prep/flowcheck/`). Added `docs/skills/flow-internals.md`, `scripts/mutant-report.py`, `scripts/design-init.sh`, and offline RISC-V reference specs (`docs/ref/`). Found and fixed two SVA preponed-sampling bugs and one test-sampling bug. |
| 2026-11-08 | Built `prep/rvcore/`: ISA reference package, 20-assertion per-stage SVA observer over a canonical pipeline tap, synthetic 5-stage harness with 9 injectable deviations, retire-stream lockstep suite vs the ISS, operand-divergence diagnostic. `make` = 3/3 pass with no assertion; `make negative` = 9/9 caught. New lessons L36–L38 (stale declaration initializers, `?:` signedness collapse, uninitialized memories). |
