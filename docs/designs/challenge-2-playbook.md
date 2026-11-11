# Challenge-2 playbook — `dma_ctrl` (four-channel DMA controller)

**Updated 2026-11-08 from the organizers' public design description.**
All facts below are from the public blurb, not from the spec/release zip (not yet available).
On release, re-derive; treat this file as the prior that tells us what to prepare.

## See also: challenge-1 retro docs

Challenge-1 completed end-to-end (datapath, 138 submissions, 34h agent time). When working challenge-2, **read these before Phase 4**:

- `docs/designs/challenge-1-flow-log.md` — what we did, wave-by-wave
- `docs/designs/challenge-1-tool-inventory.md` — what tools/agents/skills each phase used
- `docs/designs/challenge-1-ppa-plan.md` — PPA wins/dead-ends with measured ΔP per wildcard class
- `docs/designs/challenge-1-sub-catalog.md` + `challenge-1-sub-diffs.md` — how to rebuild any historical variant; useful as pattern for batch-generated P variants

Below we update PPA predictions with challenge-1 measured results (correcting some Nov 8 guesses).

## Challenge-2 IN FIELD RESULT (campaign complete Nov 12 — playbook priors vs reality)

Challenge-2 ran end-to-end locally (server down; ~51 archives, bank c2-sub-32 abar2 V=1.0
P=0.5184, proj 75.9@C=1.0). Retro docs: `challenge-2-flow-log.md`,
`challenge-2-tool-inventory.md`, `challenge-2-ppa-plan.md`, `challenge-2-sub-catalog.md`.
What the priors above got right/wrong on THIS design:

- Right: L21 config-read-mux prior (paddr→prdata wall was global in c2 too); deviation hunt
  list (D1/D2/D4/D7/D14-class all present as D1–D5); protocol SVA + per-REQ probes plan.
- Wrong: **attr wildcards (fsm_encoding, keep) were INERT on c2 synth — byte-identical
  netlists (L54)**. c1's top levers did not transfer. Always prove netlist delta first.
- New winner class: **equivalence-site removal + observable encoder restructures** (abar2
  family = bank; removed case-default arms that generated unkillable constant mutants).
- New ceiling: final-route WNS pinned ~2.5 ns in EVERY family (L58) — P plateau ~0.52 is
  place/route bound; only AREA moves P. W8 shared writeback dp: −9.2% area, P=0.5212 best,
  but DEMOTED for microarch-contract violation (L60 — keep shared-dp idea, fix contract).
- dreq input pipelining: never (L57, spurious-burst class).

## Confirmed design shape (superseded the Nov-8 "expected shape" prior)

| Fact | Value |
|---|---|
| Top module | `dma_ctrl` |
| Size | ~780 lines of SystemVerilog |
| Clock | 10 ns on sky130hd (same budget as challenge-1 → PPA lessons transfer 1:1) |
| Function | Copies 32-bit words between memory locations for **4 channels** |
| Config bus | **APB4 slave** register interface (software programs it) |
| Data bus | **AXI4-Lite master**, **one shared engine** performs every transfer |
| Arbitration | Channels have **priorities** |
| Flow control | Channels can be **paced by peripheral request lines** |
| Chaining | Channels can follow **linked descriptors stored in memory** |
| Change request | **CR-D2: per-channel descriptor-complete interrupt** |

Implications:

- Two standard protocols, one on each side. Protocol conformance is testable against the
  public APB4/AXI4-Lite specs — no guessing needed. **Build the protocol assertions before
  the design exists** (see `prep/sva/`).
- "One shared engine + 4 channels + priorities + request pacing + descriptor chains" means the
  interesting behaviour is **scheduling and bookkeeping**, not datapath. Expect the seeded
  deviations to live in: arbitration, counters/address increments, descriptor sequencing,
  status/interrupt bits, and APB read/write corner cases.
- 10 ns with a wide register file behind APB: challenge-1's L21 (critical path = config read
  mux tree `cfg_addr → cfg_rdata`, not the datapath) is the prior to check first.

## PPA levers (CORRECTED with challenge-1 measured data)
- **FSM encoding wildcards — STRONG**: `(* fsm_encoding = "one_hot" *)` on small FSMs (2-4 states) is one of the highest hit-rate P levers. Challenge-1 measured +0.0049 P single-shot on FIR 4-state FSM (L31). Hit rate ~1/2. Cost: 1-2 extra flops. **Try this FIRST** on every FSM in the design.
- **Wire `(* keep = "true" *)` — RELIABLE**: counter-intuitive P wildcard. Challenge-1 measured +0.0042 (rndsat.kept) +0.0025 (fir.acc_q) +0.0045 (stats counters) +0.0013 (fir.idx_q) = ~+0.012 cumulative. Hit rate ~1/8 per wire. **Stack across modules** (L29: cross-module keeps compound; same-module stacks plateau). Try on each module independently.
- **Bus mux trees**: address decoder fanout can dominate timing — check critical path early (use `scripts/critical-path.sh`). Challenge-1 cfg_addr→cfg_rdata read mux was on critical path; **Yosys's default `unique case` lowering was already optimal** (L30). Don't waste time restructuring the mux tree — try Yosys attribute wildcards first.
- **Operand isolation**: only multipliers/wide adders benefit; small ALU ops lose (L17). Challenge-1 measured: FIR mult ISO +0.012, gain mult ISO +0.015; FIR adder ISO −0.007, rndsat ISO −0.025.
- **Async → sync reset**: only pays on WIDE flop arrays (>64 bits, L20). **But creates REQ-42-class C-risk if spec mandates async-only reset** (L32). Challenge-1 lost 4h undoing this. Check spec text before applying.

### New PPA dead-ends discovered in challenge-1 (don't waste time)
- Critical-path mux-tree restructure (parallel_case, AND-OR tree, keep on read word) — all LOST (L30)
- Operand isolation on adders/comparators (already covered above)
- Fusing rndsat + limiter (variable comparator wider, slower)
- Narrow accumulator bits (sky130 standard-cell mapping often WORSE for non-power-of-2 widths)
- Same-module stacked keeps (plateau after 2 in same module — go cross-module)

## Predicted REQ categories (map to spec on release)

1. **APB4 slave**: 2-phase access (SETUP/ACCESS), PREADY wait states, PSLVERR conditions,
   byte strobes, read-only vs read-write registers, unmapped-address behavior, `paddr[1:0]`.
2. **Register map**: per-channel SRC/DST/SIZE/CONTROL/STATUS, global/priority registers,
   descriptor pointer, interrupt status/enable per channel.
3. **Channel engine**: address increments, word count, transfer start/stop, BUSY semantics,
   reads-before-writes ordering, 32-bit word alignment.
4. **Arbitration**: priority order, re-arbitration granularity, tie-breaks, starvation.
5. **Request pacing**: request input per channel (level), start/continue conditions, what
   happens when the request drops mid-transfer.
6. **Linked descriptors**: descriptor fetch (AXI read), next-pointer follow, chain-end,
   what status/IRQ is raised at which point.
7. **Interrupts**: per-channel complete/pending/error, mask/enable, W1C vs W0C semantics,
   same-cycle set/clear priority, top-level `irq` = OR of enabled statuses.
8. **Reset**: reset values of all registers, mid-transfer reset recovery (bus idle, no spurious
   AXI transactions), reset during APB access.
9. **CR-D2**: per-channel descriptor-complete interrupt (see below).

## Deviation catalog (hunt list)

Ordered by probability × detection cost. Each entry gets a probe test (L1: one probe per REQ)
and, where possible, a definition assertion (see assertion plan).

| # | Suspect | Why it is a classic | Detect with |
|---|---|---|---|
| D1 | Word-count off-by-one (N vs N−1 transfers; SIZE register encoded as count−1) | Boundary bug in every DMA ever written | Test with SIZE=1,2,3; count AXI write beats |
| D2 | Address increment: last word from `src+N` vs `src+N−1` | Same | Directed + random transfers, compare slave memory |
| D3 | BUSY/STATUS clear timing: cleared on last R beat vs B response vs final writeback | Affects readback and IRQ timing | Read STATUS every cycle during a transfer |
| D4 | Priority order/tie-break wrong (e.g., channel 3 wins ties, or rotating when spec says fixed) | Easy inversion in a comparator | Simultaneous requests on all 4 channels |
| D5 | Re-arbitration granularity (per word vs per burst vs per descriptor) | Spec defines it; easy to get wrong | Two channels active, measure interleaving |
| D6 | Request-line sampled at start only vs per word; mid-transfer deassert ignored | Common simplification | Toggle request mid-transfer |
| D7 | Descriptor chain: next-pointer ignored / always linear / chain-end flag not honoured | Descriptor bugs are the most common DMA bug | 3-descriptor chain with a jump back |
| D8 | Descriptor fetch order: engine starts data transfer before descriptor fully latched | Race | Backpressure the descriptor fetch (AXI RDY delays) |
| D9 | APB acceptance: samples during SETUP (PSEL&&!PENABLE) or missing PREADY wait | Bus protocol bug | BFM with wait states; assert on internal `accept` if visible |
| D10 | APB write to read-only register accepted (should be ignored or PSLVERR) | Spec-defined | Write RO, read back, check PSLVERR |
| D11 | PSLVERR decode range: last valid offset vs first invalid offset | Off-by-one in decode | Boundary address tests (see cocotb-patterns §8) |
| D12 | AXI4-Lite valid not held when `ready=0` (AW/W/AR/R/B paths) | L3 pattern, confirmed relevant | Protocol SVA + slow slave model |
| D13 | AXI write data/strb captured on the wrong beat; W before AW or vice versa | AXI-Lite allows either order | Slave model randomizes channel order |
| D14 | IRQ W1C race: same-cycle set + clear resolves to wrong value | L: pattern 9 | Assert then immediately clear, check status |
| D15 | IRQ = status (unmasked) instead of `status & enable` | Trivial wiring bug | Enable/disable sweep |
| D16 | Config written while BUSY: silently applied / corrupts transfer / correctly ignored | Spec-defined | Write SRC mid-transfer, check next transfer |
| D17 | Reset mid-transfer leaves AXI VALID asserted / stale interrupt | Reset recovery | Assert reset during active transfer |
| D18 | 32-bit `wstrb` handling on APB (partial writes to SIZE/SRC) | Strobes | Partial writes, read back |

**Do not** skip the boring ones: D1/D2/D9/D12/D14 are the most likely seeds and the cheapest
to test.

## CR-D2 analysis (prior)

"Per-channel descriptor-complete interrupt" implies a new per-channel event alongside the
existing transfer/complete interrupt. The spec-critical questions on release:

1. **Granularity**: does a 3-descriptor chain raise 1 descriptor-complete or 3? (Name says
   *descriptor*-complete → most likely: once per descriptor, or once when the *last*
   descriptor of the chain completes. Read the CR text literally.)
2. **Register placement**: new bit(s) in the existing per-channel STATUS, or a new
   global register? Reuse of the existing W1C/mask machinery is expected.
3. **Enable/mask**: is the new interrupt separately masked, or gated by the existing
   enable bit? Does it share the per-channel IRQ output or add a new output? (Top-level port
   list cannot change — CRs must fit existing ports; verify against spec.)
4. **Interaction**: if a descriptor completes with an error, is the bit set? Is BUSY cleared
   at the same time?
5. **W1C priority**: set vs software clear in the same cycle — spec will state one; test both.

Implementation plan once spec is known: add the event at the descriptor FSM's terminal state,
add status bit + enable bit + W1C handling, assert `irq_bit == status_dc & enable_dc` (or the
spec's formula), and give it a dedicated assertions file (new signals don't exist on baseline).

## Assertion plan

1. **Protocol (mandatory, top-level)** — bind `prep/sva/apb4_protocol_sva.sv` and
   `prep/sva/axi4lite_protocol_sva.sv` to `dma_ctrl`. These check setup/access legality,
   valid-stability, response stability, and (optionally) bounded liveness. They kill the
   handshake mutant class (D9, D12, D13) that signal-level assertions miss.
2. **Register decode definitions** — one assertion per decoded register (cocotb-patterns §7):
   `a_wr_src_def: assert property (wr_src == (acc_wr && sel_ch && creg==...));`
   Kills constant/operator mutations in the APB decode.
3. **W1C / status persistence** — `wr_w1c && !lanes |=> status & $past(status) == $past(status)`,
   plus set-side and clear-side assertions (sva-patterns §6).
4. **IRQ definition** — `irq == |(status & enable)` (exact spec formula).
5. **Engine invariants** — word counter monotonic until done, address = base + 4·count,
   `busy |-> axi activity`, no new AR/AW after chain end until re-arm.
6. **Reset module** (no `disable iff`) — all registers to spec reset values; AXI VALIDs low
   after reset.

## Verification infrastructure (built in `prep/`, reuse on day 1)

| Artifact | Use |
|---|---|
| `prep/bus_vip/apb4_master.py` | Drive register reads/writes, insert wait states, sample PSLVERR |
| `prep/bus_vip/axi4lite_slave.py` | Memory model behind the DMA master; latency/error injection; transaction log to verify addresses/data/ordering |
| `prep/sva/apb4_protocol_sva.sv` | APB4 bind assertions |
| `prep/sva/axi4lite_protocol_sva.sv` | AXI4-Lite bind assertions |
| `prep/dma_model.py` | Golden engine skeleton (channels/arbiter/descriptor chain); fill register map on release |

Adoption steps on release day: copy `prep/bus_vip/*.py` into the released tb tree, rename
signal handles to the real top ports, then run the VIP self-test pattern against `dma_ctrl`.

## PPA plan (10 ns, sky130hd)

Prior from challenge-1: timing dominates P (~4× weight of area; L9). Baseline WNS will tell us
if there is a real timing problem at all.

| Step | Action | Tier | Note |
|---|---|---|---|
| A | Keep baseline RTL + fix deviations + CR; bank C=1.0 | A | Never delay this |
| B | Critical-path extraction (OpenROAD `report_checks` via `--keep-work`, `scripts/critical-path.sh`) | — | Expect the APB config-read mux tree (L21) |
| B | Read-decode restructure **only if measured** | B/C | L30: manual mux restructures lost on ch1; measure before |
| B | `(* keep *)` wildcard on one wire per module (L27–L29) | B | Cheap, attribute-only; one per module, stop stacking same-module |
| B | Sync-reset only on wide flop groups >64 bits (L20) | B | Narrow groups lose |
| C | Arbiter/priority restructure, descriptor-prefetch pipelining | C | Must C-check (L4) |
| — | Operand isolation | unlikely | No multipliers; skip unless an adder/comparator is both high-toggle and off the critical path |

Spec-freedom candidates to audit on release (see `docs/ppa-freedom-strategy.md`): FIFO depth,
descriptor prefetch depth, internal arbitration implementation, status sampling latency — any
"at least" or "may" wording is a lever; anything the spec pins cycle-accurately is frozen.

## Mutation notes (DMA-specific mutant classes)

- **State-transition mutants** on the channel/descriptor FSM: kill with (a) next-state
  assertions if internal state names are stable, (b) end-to-end transfer tests that check
  every word, (c) IRQ timing tests.
- **Counter mutants** (`+1`→`+2`, `<`→`<=`): kill with SIZE=1/2/3 and non-power-of-two
  transfer lengths.
- **Decode mutants**: kill with the per-register decode assertions + boundary addresses.
- **Handshake mutants**: kill with the protocol SVA (they need no stimulus design).
- Expect V≈0.9 achievable with REQ matrix + protocol SVA; residual will be equivalent mutants
  (e.g., reserved bits) — document as V ceiling.

## Day-1 checklist (Nov 10)

1. [ ] Unzip, `md5sum` baseline, `docs/design-init.md` bootstrap, create
   `docs/designs/challenge-2.md` + `challenge-2-req-matrix.md` + `challenge-2-progress.md`.
2. [ ] Baseline flow run → note C/V/P and WNS.
3. [ ] Wire `prep/bus_vip` + `prep/sva` into the tb; run public tests.
4. [ ] Copy `prep/dma_vip/tests/{dma_env.py,test_dma.py}` into `<design>/tests/`, **vendor**
   `apb4.py`/`axi4lite.py` alongside them (only `tests/` is on pythonpath), fill `Regmap`
   + `AXI_NAMES` from the released map, re-tag `TODO(REQ)` names into the req-matrix, and
   bind the protocol + irq SVA (`prep/dma_vip/tests/dma_bind.sv` is the shape).
   Make sure the L41 probe families all exist for the real map: reset read-back of every
   RW reg; CTRL bit read-back; STATUS read with don't-care PWDATA; PWRITE without PSEL;
   error injection on R **and** B channels + descriptor reads, with ERR==0 on clean runs
   (both modes) and ERR W1C at minimal strobe; chain LEN=0 descriptor; idle-latched
   RRESP/BRESP (`resp_when_idle`); mid-operation reset; busy-guard against START re-issue.
5. [ ] Phase 1: spec-analyst pass with the deviation catalog; probe per REQ (L1).
6. [ ] Fix deviations; implement CR-D2; per-REQ test+assertion matrix complete (L12).
7. [ ] First submission; then Tier B PPA while waiting on cooldown. After the first V
   report, run `scripts/mutant-report.py` and kill by class (L41/L42): classify survivors
   as dead/equivalent before adding tests; extend VIP knobs before adding per-test hacks (L43).

## What NOT to repeat from challenge-1

- Don't add pipeline registers without a measured critical path (L16).
- Don't operand-isolate small ALUs/comparators (L17).
- Don't rely on `(* keep *)` stacking (L28); one per module, measure.
- Don't hand-restructure a mux tree that Yosys already lowers well (L30).
- Don't skip the server C-check for any Tier B/C change (L4).
- Don't chase V past the equivalent-mutant ceiling (L7, L8).
