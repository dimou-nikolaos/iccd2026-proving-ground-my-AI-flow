# challenge-1 PPA Experiments

## Current best: sub-15 (W1+W7 stacked) → P = 0.6063 (a=−0.0256, t=+0.2324, p=+0.6496)
- Baseline: area=66379 µm², WNS=2.70 ns, power=31.45 mW, achievable_period=12.30 ns
- sub-15:  area=68080 µm² (+2.6%), WNS=5.56 ns (+2.86), power=11.02 mW (−65%), achievable_period=9.44 ns
- Projected score at C=1.0: 79.06 at V=0.975 local (V will reroll on server)
- Changes from sub-13 (gain 3-stage):
  - **W1**: FIR 16×16 multiplier operand isolation — gate coef & delay_q inputs to 0 when state_q != S_MAC. P +0.0121, power −2.89 mW.
  - **W7**: Gain 17×33 multiplier operand isolation — gate centered_q & gain inputs to 0 when stage2 not firing. P +0.0024, power −2.65 mW.

## Previous best: sub-13 (gain 3-stage) → P = 0.5918 (a=−0.031, t=+0.253, p=+0.474)
- sub-13:  area=68403 µm² (+3.1%),  WNS=5.81 ns, power=16.56 mW, achievable_period=9.19 ns
- Change vs sub-12: dsp_gain 2-stage → 3-stage (split subtract from multiply, registered centered_q). Adds 17 flops.

## Previous best: sub-11/sub-12 → P = 0.5658 (a=-0.026, t=+0.169, p=+0.372)

Final routed numbers vs baseline:
- area: 68076 vs 66379 µm² (+2.6%)
- WNS: 4.78 vs 2.70 ns (better by +2.08ns)
- power: 19.76 vs 31.45 mW (-37%)

The +0.169 timing score comes from this large timing slack. **This slack is itself a P asset** — losing it costs more than area/power wins typically gain.

## Wildcards tried (Phase 4e)

### Batched runs (3 batches, 9 wildcards, 3h wall-clock)

Pattern: pre-build N variant dirs → tar → ONE bash invocation launches N Docker jobs in parallel with `--cpus $((12/N))`. Wall-clock: max(single) × 1.3-1.5x = much faster than serial N×6min.

| Batch | Variants | Cores each | Wall-clock | Notes |
|-------|----------|------------|------------|-------|
| A | W1, W2, W3, W4, W5 | 2 | 1135s (19 min) | 5 full-flow at --cpus 2, slower per-flow than solo but total wall-clock 2x better than serial |
| B | W6, W7 | 6 | 529s (9 min) | 2 full-flow at --cpus 6, near-solo speed per-flow |
| C | W8, W9 | 6 | 681s (11 min) | W8 rejected at public (assertion broke); W9 scored |

### Wildcard outcomes (vs sub-13 baseline at start of batches)

| W | Wildcard | P | ΔP | Outcome |
|---|----------|---|----|---------|
| W1 | FIR mult operand isolation | **0.6039** | **+0.0121** | **WIN — promoted to sub-14** |
| W2 | Gain mult+subtractor ISO | 0.5645 | −0.027 | Loss (reorder forward-decl fix made it build; P-negative anyway) |
| W3 | FIR acc adder input ISO | 0.5849 | −0.007 | Loss |
| W4 | FIR rndsat input ISO | 0.5663 | −0.025 | Loss (rndsat not on critical path; gating mux adds delay) |
| W5 | Carry-save FIR accumulator | 0.5163 | −0.075 | Loss (82 flops vs 40, big area penalty, no timing win) |
| W6 | Stats peak comparator ISO | 0.5887 | −0.015 | Loss |
| W7 | Gain mult ISO (on top of W1) | **0.6063** | **+0.0145** (stack) | **WIN — promoted to sub-15** |
| W8 | Decim sum_next ISO | — | — | REJECTED — assertions broke, abandoned |
| W9 | Gain rndsat input ISO | 0.6033 | −0.003 | Loss |

**Pattern from 9 experiments**: operand isolation only pays on multipliers (W1, W7). Other elements (small adders, comparators, combinational blocks like rndsat) lose P because the gating mux adds delay without enough toggle-current reduction.

### Wildcard: gain 2→3 stage pipeline (sub-13, PROMOTED, earlier)
Hypothesis: split the gain combinational path (subtract + 33×33 multiply + rndsat) into 3 registered stages to maximize WNS margin.
Time: ~50 min wall-clock (impl 15 min + 2 full flow runs due to one ODB-0269 crash).
Result: **PROMOTED to sub-13 / sub-best.**
Measurements: P=0.5918 (was 0.5658, +0.026). WNS=5.81 (was 4.78). Area +3.1%, power −47% vs baseline. Achievable period 9.19 ns vs baseline 12.30 ns.
Verdict for future challenges: **split long combinational datapaths into as many registered stages as the REQ latency budget allows.** Even small RTL edits (+17 flops) can unlock big timing-score wins. Always check REQ latency budget first; if slack ≥1 cycle, split.

### Wildcard: FIR rndsat output pipeline (REVERTED)
Hypothesis: register FIR output (rndsat result) via new FSM state S_OUT to split the rndsat combo path from downstream.
Time: ~30 min (impl 15 min + 1 full flow run).
Result: **REVERTED.** P=0.5773 (on top of gain3 sub-best, P −0.0145). WNS +0.30 ns (small win), area +1002 µm² (penalty), power +31% (big penalty from extra flop toggling and FSM decode).
Verdict for future challenges: **only pipeline stages that ARE on the critical path.** FIR's rndsat is not the critical path after gain3 split (WNS already +3 ns slack). Adding a flop where no timing benefit exists = pure area/power penalty. Check the critical path in the PPA report before splitting.

## PPA experiments run locally (not submitted)

| Candidate | Tier | Change | P | ΔP | Verdict |
|-----------|------|--------|---|-----|---------|
| sub-11 | (best) | gain 2-stage pipeline + FIFO reset strips + FIR split FSM | 0.5658 | — | KEEP |
| sub-fifo-rm | B | remove input FIFO (DEPTH=2 → 0) | 0.5612 | -0.005 | REVERT |
| sub-fuse-lim | C | fuse decim rndsat + limiter (BOUNDED param) | 0.5424 | -0.023 | REVERT |
| sub-acc-narrow | A | FIR acc_q 40 → 35 bits | 0.5518 | -0.014 | REVERT — area INCREASED, synthesis mapped 35-bit worse |
| sub-fir-parallel | C | parallel FIR: 8 multipliers + tree, 1-cycle latency | (CTS FAIL) | n/a | REVERT — 18748 cells (2.5x baseline), CTS crashed (ORFS could not produce report file). Synthesis OK, public tests OK (63/63), but fails at clock tree synthesis |

All archives saved in `runs/ppa-candidates/`.

## Cross-check against "Specification Design Freedoms" analysis

External analysis (ChatGPT) of spec freedom mapped onto our architecture:

| Analysis recommendation | We have? | Note |
|-------------------------|----------|------|
| Output FIFO exactly 8 entries | ✓ | DEPTH=8 in sub-11 |
| Output FIFO 136 bits (no clip stored) | ✓ | WIDTH=17 (data + last) |
| Eliminate or minimize input FIFO | partial | DEPTH=2. Tried removal — lost P |
| Iterative FIR with one MAC | ✓ | sub-11 FIR is single MAC |
| FIR delay line as shift registers (not RAM) | ✓ | unpacked array, Yosys maps to flops |
| FIR accumulator width minimum sufficient (35 bits) | ✗ | we use 40. Tried 35 — area INCREASED |
| Decimator: 1 accumulator + counter (no FIFO) | ✓ | sum_q + phase_q |
| Decimator accumulator 19 bits | ✓ | |
| Offset/gain: 17-bit sub, 17×16 multiplier | ✓ | |
| Share RS unit across stages | ✗ | 3 instances. Tried fusion of decim+lim, lost P |
| Bypass path independent of iterative FIR | ✓ | FIR `if (bypass && state_q==S_IDLE)` combinational |
| STAT counters 16-bit saturating | ✓ | |
| IRQ 2-bit AND, 2 FFs each | ✓ | |
| Register decode once | ✓ | dsp_regs is_coef / word decode |

**Conclusion**: sub-11 already implements ~12/13 of the analysis's recommendations. The remaining items (input FIFO minimization, RS sharing, narrow accumulator widths) all turned P-negative in experiments. sub-11 is consistent with the spec-freedom analysis's recommended area-oriented architecture.

## Why these lost

**sub-fifo-rm**: area -1407 µm² (a improved +0.022), power -5.65mW (p improved +0.179), but **WNS regressed from 4.78 → 3.13** (t dropped -0.134). The s_ready → gain_ready combinational path (now goes through 2-stage gain handshake logic) ate timing slack.

**sub-fuse-lim**: area ~unchanged, but **WNS regressed 4.78 → 3.44** (t dropped -0.109). The fused rndsat compares against variable lim_hi/lo instead of constants 32767/32768, making the magnitude comparator significantly slower.

## What this tells us about the score formula

P is roughly `0.5 + Σ w_i * signed_i` where w_t is weighted more than the area savings suggest. Timing slack is precious — every ns of slack counts as ~0.04 in P.

Empirically (from data points):
- a delta: -0.026 to -0.004 (Δ+0.022) gives roughly ΔP ≈ +0.006 (weight ~0.27)
- t delta: +0.169 to +0.035 (Δ-0.134) gives roughly ΔP ≈ -0.16 (weight ~1.2)
- p delta: +0.372 to +0.551 (Δ+0.179) gives roughly ΔP ≈ +0.05 (weight ~0.28)

**Timing dominates**. To improve P, preserve timing slack.

## Not yet attempted (with reasoning)

**Shared multiplier (gain × FIR)**: biggest potential area win (~1500 µm², ~+0.02 P). But adds 17x17 (vs 16x16) multiplier, longer comb, and arbitration logic — high probability of timing regression that cancels area win. Skip.

**Tier A precompute group_max**: tiny (~60 µm² savings). Probably P-neutral. Skip.

**Parallel FIR (8 multipliers, 1-cycle)**: huge area penalty. Skip for P; could be an Explorer variant.

**Input FIFO DEPTH=1**: breaks dsp_fifo pointer math (zero-width AW). Would need FIFO module rewrite. Not worth it given fifo-rm already lost on timing.

## Where sub-11 came from (already applied)

| Sub | P | Change | Notes |
|-----|---|--------|-------|
| 0 | 0.4712 | baseline + 4 seeded fixes + CR | starting point |
| 2 | 0.5052 | + FIFO mem_q reset strip | Tier A, +0.034 |
| 3 | 0.5056 | + FIR FSM reset split | Tier A, +0.000 (neutral) |
| 4 | **0.5658** | + gain 2-stage pipeline (Tier B) | +0.060 timing — biggest win |
| 5-11 | 0.5658 | tests/assertions added (P neutral) | RTL unchanged |

The gain pipeline split is the single biggest P win — it lengthens a path across 2 cycles, freeing WNS by 2ns. This made the whole slack-based timing asset possible.

## Strategic conclusion

sub-11 is at a **local optimum** for our current topology. Further P improvement requires either:
1. Major topology restructure (shared multiplier, parallel FIR) — high C-risk
2. Aggressive post-route tricks (not RTL-controllable)

The score bottleneck is C, not P. Stop PPA iteration; lock C first when server returns.

## Submission order plan (when server returns)

1. **sub-0** first to lock C baseline (just RTL fixes + CR, no tests/asserts).
2. **sub-11** with tests/asserts — measure V and P against baseline. Expect C=1.0, V=1.0, P=0.5658.
3. **ppa-candidates/sub-fifo-rm** as 1-submission C-check + Explorer variant. Likely C=1.0 (topology is observable-equivalent). Slightly worse P but interesting distinct netlist.
4. **ppa-candidates/sub-fuse-lim** as 1-submission C-check. Higher C-risk (changes rndsat module signature, decim interface). Likely C=1.0 unless operator pinned internals.
5. Only if C=1.0 verified for sub-11: try shared-multiplier Tier C for another P push.
