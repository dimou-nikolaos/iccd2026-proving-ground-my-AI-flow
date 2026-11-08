# Lessons Learned

Measured pitfalls from challenge-1 and challenge-2, encoded as rules. **Read before starting Phase 1 of a new design.** Each lesson is numbered and referenced from `docs/flow.md` and the role docs.

## Phase 1 — analysis

**L1. A 4th seeded deviation can hide in plain sight.** challenge-1's REQ-16 (HALF_EVEN rounding) was missed by initial static analysis (3 found) and by formal+probe. It was only caught by writing one test per REQ from the spec text.
**Lesson**: at end of Phase 1, write one probe test per REQ (not per RTL line). If a probe disagrees with the golden model → deviation.

## Phase 3 — verify

**L2. Mutant-hunting line-by-line plateaus at V≈0.85-0.90.** V only jumped to 0.95+ after switching to REQ coverage matrix (filling REQ-01/03/09/10/11/20/22/24/28/31/34/35/36 gaps).
**Lesson**: REQ coverage first; mutant hunting for residue.

**L3. Interface assertions are cheap and high-value.** One AXI-stream `valid && !ready |=> valid && $stable(data)` pair on the top module kills drop_term mutants on handshake logic that signal-def assertions miss.
**Lesson**: write top-level interface assertion module before any per-signal assertion.

**L12. Phase 3 must cover every REQ up front, not iteratively.** Test and assertion writing is ~5 lines per REQ after the golden model exists. Treating this as "fill gaps after the first mutation run" wastes 2-3 iterations of cooldown waiting on work that was always going to be required. The mutation run is verification, not gap-finding.
**Lesson**: in Phase 3a and 3b, enumerate the REQ list mechanically and write ≥1 test + ≥1 assertion per REQ before the first flow run.

## Phase 4 — iterate (C)

**L4. Tier B PPA change (pipeline split) without C-check is a hidden risk.** challenge-1's gain stage 1→2 stage pipeline shift moved `gain.data_q` to `gain.product_q + gain.data_q`. Local tests pass. Hidden assertion could still fail C.
**Lesson**: Tier B/C MUST get a server C-check submission. Server C is the only ground truth.

## Phase 4 — iterate (P)

**L5. Any archive change rerolls the mutation seed.** V is deterministic per tar but unpredictable across tars. A "safe" P-neutral RTL change can lower V by rerolling to a worse seed.
**Lesson**: only promote archive changes that measurably improve P (or V). Otherwise revert and keep sub-best.

**L6. P is not additive across independent changes.** A FIFO reset-strip alone gave +0.034 but stacked with operand isolation dropped to +0.001 (timing regression cancelled power win).
**Lesson**: bundle Tier A, isolate Tier B/C.

**L9. Timing slack in P is itself a score asset, not just a margin to spend.** Empirical (challenge-1): t has ~4× the weight of a in P; a small timing regression of 1.5 ns costs ~ −0.13 P, while a 2% area reduction gains ~ +0.006 P. Two PPA attempts (input FIFO removal, decim+limiter fusion) both LOST P because they regressed WNS toward baseline.
**Lesson**: PPA changes that lengthen comb paths must recover timing somewhere else (pipeline split) before applying. Otherwise they're P-negative.

**L10. The first pipeline split is the biggest P win.** challenge-1 sub-2 → sub-4 went from P=0.505 to P=0.566 by splitting the longest combo path (gain stage) into 2 cycles. That unlocked +2 ns WNS which became +0.06 P and a "timing slack asset" of +0.17 t-score.
**Lesson**: identify the critical path early (run PPA, look at WNS) and split it before doing area/power optimization.

**L11. Synthesis counter-intuition: RTL-level "narrowing" can INCREASE area.** challenge-1 narrowed FIR accumulator from 40 to 35 bits (mathematically sufficient) and area INCREASED +656 µm² with worse timing AND power. Yosys+OpenROAD maps odd-width registers and shift/mask logic less efficiently on sky130 standard cells.
**Lesson**: spec-minimum widths aren't automatically optimal for PPA; always measure before/after. Don't optimize widths that don't correspond to clean cell boundaries (8/16/32/64).

**L14. Aggressive area-reduction topology changes can pass build+tests but FAIL at CTS.** challenge-1 parallel FIR (8 multipliers + adder tree, replacing iterative 1-multiplier FSM) built cleanly, passed 63/63 cocotb tests, but failed OpenROAD's CTS stage — 18748 cells (2.5× baseline 7312), the ORFS cts.tcl couldn't produce the metrics report.
**Lesson**: PPA experiments must run the FULL flow (synth → floorplan → place → CTS → route) before being counted as Explorer candidates. Synthesis-only or test-only validation is insufficient. A PPA-failing design = C=0 = rejected.

**L15. External spec-freedom analysis is a high-ROI input to PPA.** Have an LLM (or a human expert) read the spec and enumerate "what's NOT externally observable or only constrained by a minimum requirement." Cross-check the resulting list against our current architecture. challenge-1: external analysis identified 13 implementation freedoms; we already implemented 12; the 3 we tried (input FIFO elimination, RS sharing, narrower widths) were the ones the analysis flagged AND all turned P-negative in experiments.
**Lesson**: do the spec-freedom cross-check before sinking time into Tier C variants. Confirms local optimum or surfaces untried levers.

**L16. Only pipeline stages that ARE on the critical path.** challenge-1's FIR rndsat output pipeline (Phase 4e wildcard, S_OUT state adding 1 registered cycle after FIR rndsat) produced P −0.0145 on top of an already-good sub-best (gain 3-stage had raised WNS to 5.81ns, moving the critical path away from FIR rndsat). Extra flops without a timing benefit = pure area+power penalty (+1002 µm², +31% power).
**Lesson**: check the PPA report's WNS and, if possible, the critical path (via OpenROAD timing report if accessible) BEFORE adding pipeline registers. On already-pipelined stages, more pipeline = P-negative.

**L17. Operand isolation pays ONLY on multipliers (in this design).** challenge-1 ran 6 operand-isolation wildcards: gate inputs to 0 when not firing. Results:
- **FIR 16×16 mult ISO**: P +0.0121 (power −2.89mW, timing +0.20ns WNS — multiplier IS on critical path AND high-toggle)
- **Gain 17×33 mult ISO**: P +0.0024 (power −2.65mW, timing −0.45ns — power win, small timing cost from gating mux)
- **Stats comparator ISO**: P −0.0152 (16-bit comparator toggling is too small to pay for gating logic)
- **FIR accumulator adder ISO**: P −0.0069 (small adder, no win)
- **FIR rndsat input ISO**: P −0.0255 (40-bit rndsat combo isn't on critical path; gating mux added delay)
- **Gain rndsat input ISO**: P −0.0030 (same pattern as FIR rndsat)
- **Decim sum_next adder ISO**: broke assertions (lost)
- **Gain subtractor ISO** (part of W2b): P −0.027 (17-bit subtractor + mult combo gating added delay)

**Lesson**: for each operand-isolation candidate, the gated element must be (a) high-toggle (multipliers, big adders), AND (b) not the gating condition's critical path. Multipliers in a non-tight timing path are the surest win; smaller elements or tight-path elements lose.

**L18. Stacking two P-positive wildcards stacks sub-linearly.** W1 alone: P=0.6039 (+0.012 over sub-13). W7 (gain mult ISO) alone on top of sub-13 would be +0.005; on top of W1 it's only +0.002. Total sub-15 = +0.0145 vs sub-13. Power savings DO stack (16.56 → 13.67 → 11.02 mW). Timing wins DO NOT stack cleanly (W1 added +0.20ns WNS, W7 subtracted −0.45ns). Net P still positive.
**Lesson**: stack wildcards with different levers (power + power + timing) — they reinforce. Stacking same-lever wildcards may under-add.

**L19. Verilator lint accepts forward-referenced continuous assigns; Yosys does not.** challenge-1 wildcard W2 (gain mult ISO) had:
```
logic s1_fire, s2_fire;
assign s1_fire = stage1_free && in_valid;   // references stage1_free
...
logic stage1_free;                            // declared later
assign stage1_free = ...;
```
Verilator lint PASSED (SystemVerilog allows textual forward-reference in continuous assigns). Yosys via slang REJECTED at build stage. Wasted a full-flow cycle.
**Lesson**: declare signals before their assign RHS references them, in source order. Verilator-only checks may not catch Yosys incompatibility. Add a quick `yosys -p "read_slang ...; hierarchy -top ..." | grep -i error` check before packaging if RTL has non-trivial signal ordering.

**L20. Sync reset on wide flop arrays saves area; on narrow flops it hurts.** challenge-1 sub-16 won P +0.0013 by switching `dsp_fir.delay_q` (8 × 16 = 128 flops) from async to sync reset. Extending same change to:
- FIR state_q (2 bits): P −0.016 (sync cells bigger than async)
- gain stage regs (17-34 bits each): P −0.023
- stats counters (16 bits × 3): P −0.014
- decim phase/sum/clip (4-20 bits): P −0.005
**Lesson**: in sky130hd, async-reset DFF cells (`dfrtp_1` etc.) are similar size to sync-reset DFF cells for narrow registers, but Yosys often picks larger sync-reset cells to handle the synchronous reset multiplexer. Only wide arrays amortize the saving. Try sync-reset only on flop groups >64 bits in one always_ff.

**L21. OpenROAD `report_checks` doesn't match flow's reported WNS by 0.5-1ns.** challenge-1 flow reports WNS 5.56 ns for sub-15; my OpenROAD `read_db 6_final.odb + read_sdc 6_final.sdc + report_checks -path_delay max` reports worst slack 6.49 ns. The flow likely uses post-route `report_worst_slack` directly with additional SDC pessimism (clock network + setup/hold derates). Use OpenROAD report as **directional guide only** (which paths are critical) — trust flow's reported WNS for actual numbers.
**Recipe**: `--keep-work` flag on `pgflow run`, then OpenROAD command above.
**Lesson**: the critical path is in the **register read mux tree** (`cfg_addr → cfg_rdata`), not the datapath. Pipeline splits and operand isolation on datapath don't shorten the true critical path; they help via power savings and secondary optimization effects.

**L22. V score is seed-luck dependent. Many distinct netlists are seed hedging.** Generated 8 cosmetic RTL variants of sub-15 (signal rename, block reorder, constant literal style, unused marker blocks). All pass 63/63 cocotb tests. Flow mutation V varied: 3 at V=1.0, 5 at V=0.975. Same RTL semantics, different RTL hash → different mutation seed → different V.
**Lesson**: for Explorer AND for V maximization at submission, prepare ≥8 cosmetic variants. Submit each; server V feedback tells you which seed-lucky RTL hash to keep as final sub-best. Same RTL semantics → same C. Different hash → different V.

**L23. Be careful which "cosmetic" variants you keep as Explorer candidates — some have subtle SEMANTIC changes that look cosmetic.** Challenge-1 Explorer variant e13 rewrote dsp_limit's `clipped = over || under` to `clipped = over != under`. With LIM_HI < LIM_LO (inverted window), `over` and `under` can both be 1; original `||` returns clipped=1, modified `!=` returns clipped=0. Public tests still pass (test_req29_lim_hi_wins_when_inverted doesn't check clipped flag on inverted window), but a hidden test might. Risk not worth the Explorer benefit.
**Lesson**: when generating Explorer variants, audit each for SEMANTIC change. Allowed: comments, named blocks, signal renames, `unique case` → `case`, literal format (0x0000 vs 'd0), assign reorder (continuous assigns are unordered), extra unused params. NOT allowed: any change to boolean expressions that affect output for some input combination, removing or adding conditions.

## Phase 4 — iterate (V)

**L7. Some mutants are functionally equivalent and unkillable.** DECIM's rndsat `sat` flag (challenge-1) cannot saturate for any legal input because max sum = 8 × 32767 = 262136, max rounded = 32767 (no overflow). A mutant dropping the `sat` term is provably equivalent.
**Lesson**: when V plateaus, mathematically check equivalence before adding more tests. Document as V ceiling.

**L8. Verilator default-init hides missing-reset mutants.** A `drop_term` mutant removing `!rst_n` from `always_ff @(posedge clk or negedge rst_n)` looks equivalent because Verilator's `--x-initial` initializes to 0 (the reset value).
**Lesson**: this is a V ceiling item — don't chase.

## Tools

**L13. SBY formal has poor ROI vs simulation next-state assertions in our flow image.** Flow Docker has SBY + slang, but slang's SVA support is limited (`|=>`, `|->` rejected; `@(*)` rejected) and async-reset FFs use `$anyinit` so helper-reg patterns produce false-positive CEXs at step 1. Setting up a clean SBY run costs 1-2 hours; the same property is pinned "for free" by a one-cycle-back identity assertion inside `always @(posedge clk)` that runs at every cycle of every cocotb test.
**Lesson**: default to simulation next-state assertions; reserve formal for deep temporal (>20 cycles), reachability, or unbounded induction properties.

## How to use this file

When you hit one of these situations mid-task, jump here for the prior decision. New lessons: append at the bottom of the appropriate section; don't renumber existing lessons (cross-refs from other docs use L-numbers).
