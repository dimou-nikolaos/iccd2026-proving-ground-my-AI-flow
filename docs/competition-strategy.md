# Competition Strategy

## Scoring formula
```
score = 100 × C × (0.5V + 0.5P)
```
- C = hidden test pass rate (multiplier — 0 kills everything)
- V = mutation kill rate (40 mutants, participant's tests/assertions kill)
- P = PPA vs baseline (0.5 = same, >0.5 = better, 0.3 cap if timing fails)

**Baseline untouched ≈ 30.** To score >80: C=1.0, V≥0.95, P≥0.65.
**To score >90: C=1.0, V=0.95, P≥0.85** (need significant PPA improvement).
**P > 0.5 comes from**: removing logic (free from deviation fixes) + active RTL restructuring + library-aware coding.

## Priority order (per design)

1. **C = 1.0 first** (non-negotiable). Fix all deviations + CR correctly.
2. **P > 0.5 second** (PPA better than baseline). PPA is half the score.
3. **V maximize** (write comprehensive tests + assertions for maximum kill).

Rationale: C=0.9 × (0.5×0.95 + 0.5×0.5) = 65. C=1.0 × (0.5×0.8 + 0.5×0.7) = 75. C=1.0 × (0.5×1.0 + 0.5×0.8) = 90. PPA wins massively once C is solid.

## Feedback loop (each submission returns in 15 min)

| What's returned | How to use it |
|----------------|---------------|
| Hidden test names that FAILED | Diagnose which deviation/CR requirement is wrong. Fix RTL. |
| Surviving mutant locations | Write targeted assertions. |
| PPA at each stage (synth, place, CTS, route) | Identify which stage regressed; optimize. |

## PPA optimization strategy

**P = 0.5 means equal to baseline.** Above 0.5 requires: lower area, better timing, or lower power. You can apply ANY RTL restructuring — you are NOT limited to minimal fix+CR changes. As long as top interface matches and C=1.0.

**Canonical strategy list**: `.opencode/skills/sky130hd-ppa/SKILL.md` (concise 10-step checklist) and deep reference `docs/skills/sky130hd-optimization.md` (code examples, sky130 cell library, failure diagnosis). Both are always loaded for P iteration.

### Summary of the approach

- **Phase A (during fix)**: make each deviation fix in its cheapest form; look for free area wins (removing extra comparator terms, removing dead logic the bug created, reusing existing signals for the CR).
- **Phase B (C=1.0 confirmed)**: apply safe RTL-only restructurings from the skill checklist — narrow masked registers, homogenize reset style, `>=`→`==` where overshoot is impossible, balanced mux trees, clean enable patterns for ICG, register replication for high-fanout signals.
- **Phase C (aggressive / Explorer award)**: pipeline deep combinational, datapath reset stripping, operand isolation. Each distinct netlist with C>0.9 is an Explorer point.

### Why P>0.5 is achievable in competition

The flow baseline is the **original** released RTL (with the seeded deviations). Your fixed RTL is structurally different — removing an extra comparator term, narrowing masked registers, and restructuring to exploit the sky130hd library gives genuine area/timing/power deltas, so P>0.5 is achievable.

**Calibration caveat (measured):** a seeded bug can make the baseline *cheaper* than the correct design (e.g. a missing config latch = the baseline simply omits flops). A correct fix can therefore push P **below 0.5** at first — the fix is not optional, so reclaim it with restructuring, never by dropping the fix. **Power is usually the dominant P term** and responds most to removing always-toggling combinational logic (move config-derived shift/mask computation to load time). Observed on a datapath design: fix-only P=0.45 → 0.54 after moving per-frame shift/mask precompute into the config latch (power −34% vs baseline), with area at parity and timing better than baseline. Net: target P≈0.5–0.6 from safe restructurings; only aggressive Phase-C microarchitecture gets past ~0.65.

**Worked example (datapath, full loop, local):** 3 deviations + CR fixed; 13 golden-model tests + 9 bound SVA; three safe PPA steps (config precompute + two datapath reset-strips). End state **V≈1.0, P≈0.56, score ≈78 at C=1.0**. Take-aways: V is cheap to max on a datapath (signal-definition + next-state assertions kill nearly all mutants; only provably-equivalent survivors remain); P is the hard half and lands ~0.55 for safe work. Budget iterations accordingly: most of the score comes from C (free once deviations+CR are right) and a solid P; chasing P past ~0.6 or V past the equivalent ceiling is where effort stalls.

**Worked example (control+bus, full loop, local):** 3 deviations + CR (interrupt expansion) fixed; 47 golden-model tests + 9 bound SVA modules (120 properties); 14 total PPA experiments across two 10-drop cycles. End state **V=1.0 (lucky seed), P=0.4965 (best P), proj=74.58 at C=1.0** (best archive = drop 19: V=1.0 + P=0.4916). Take-aways:
1. **Control+bus designs under 3000 cells have a P ceiling near 0.50**, not 0.48. Achievable via stacked wins, but not beyond baseline (CR-mandated additions are spec-required area cost).
2. **Four PPA wins stacked to reach P=0.4965:** (a) ICG-friendly 3-branch flop restructure — biggest timing win (+0.033 timing component, WNS 1.80→2.16); (b) wrap `>=→==` when invariant holds — saves 58 cells; (c) AND-OR read mux with one-hot decode — saves 241 cells but needs timing slack; (d) operand isolation on idle adders — area+power win.
3. **Ten failed PPA attempts** narrowed the space: narrow IRQ, fuse modules, countdown prescaler, precompute flops, register replication, etc. See `docs/designs/challenge-2-ppa-plan.md` for full log.
4. **Timing slack is the currency.** Stack timing wins first (ICG restructure, shorter comparators), then spend slack on area/power wins. Never stack two area-only wins — the WNS cliff hits.
5. Strobe-gating assertions (3-path pattern) are the highest-yield kill pattern for APB/register designs — 40/40 on one seed.
6. **V is seed-sensitive.** 20 archives tested, only 3–4 drew V=1.0. **Any content change re-rolls the seed** (RTL, tests, assertions, comments). Pick the highest-projective-score archive; don't assume new content always improves V.

### Risk

Hidden regression tests and hidden formal properties may reference internal signals or exact cycle timing. Aggressive Phase C changes can drop C below 1.0. Mitigation: keep a C-critical change isolated so a C drop identifies it uniquely; if C drops, that behavior is hidden-checked — revert and binary-search the sub-list. The 30-submission budget is the search budget (see **PPA iteration economy** below: bundle the P-safe changes, isolate only the C-/timing-critical ones).

### PPA iteration economy (how many changes per submission)

Local runs are unbounded (only wall-clock); **server submissions are the scarce resource** (30/design, 30-min cooldown). So economize submissions differently locally vs on the server.

**Tier-based budget** (see `docs/flow-ppa-and-v.md` for tier definitions):

| Tier | Local verification per change | Server submissions |
|------|------------------------------|-------------------|
| A (provably-free) | synth + tests@ASSERTIONS=1 (~5 min each) | 0 — bundle into Tier B submission |
| B (timing-neutral) | synth + tests + 1 local PPA (~6 min) | **1 C-check after stacking A+B changes** |
| C (structural) | synth + tests + 1 local PPA + REQ re-audit | **1 C-check per Tier C variant** (one variant per submission) |

Typical total per design: 3-5 submissions for full PPA optimization (1 initial C-check after Phase 2 + 1 stack-A+B + 1-3 Tier C trials + 1 final).

- **P is not additive.** The PPA score combines area/timing/power with an achievable-period / timing-met gate. Two individually-safe edits can oppose each other (one shrinks area/power, the other lengthens the critical path) or compete for routing/cell rows and move the critical path. A bundle can tip WNS into a violation, which **caps P (~0.3)** and erases every gain at once. Measured: a FIFO datapath reset-strip gave +0.027, but a second strip bundled-or-not could mask it — measured separately it *hurt timing* (WNS 4.61→4.25, net −0.007). You only see that with single-variable measurement.
- **Bundle Tier A; isolate Tier B/C.** Tier A changes (provably-free / independent: dead-code & extra-term deletion, the CR mux, config precompute) cannot tip timing or C. Tier B changes (reset-strips, operand isolation, pipeline splits) MUST get a C-check submission before stacking further Tier B/C work — hidden tests can pin internal signal names or exact cycle counts. Tier C changes (deep restructuring, module fusion) MUST be one-per-submission because the failure mode is C regression (hidden assertion on internals).
- **Pre-screen before spending a submission:** in Docker, `yosys ... synth` for cell count + a WNS/timing estimate, and lint/elab + `tests@ASSERTIONS=1` for C risk. Batch only the pre-screen survivors into one submission. A PPA flow run is ~4–6 min locally; a server submission is 30 min + a budget slot — don't burn one per micro-change.
- **Locally** you may still isolate a technique once to *learn* its ΔP and update the skill docs (cheap, buys reusable rules); **on the server**, batch the Tier A survivors and keep the scarce one-at-a-time for Tier B/C verification.
- **Synthesis counter-intuition**: spec-minimum RTL widths aren't always synthesis-optimal. Yosys+OpenROAD on sky130hd maps odd-width registers (35, 40 → 35) and shift/mask logic worse than power-of-2 widths. Always measure before/after; don't "narrow to spec minimum" without PPA check. (challenge-1: FIR acc_q 40→35 increased area +656 µm² AND degraded timing AND degraded power.)
- **Aggressive topology restructures can pass build+tests then fail PPA at CTS.** Validate Explorer-variant candidates by running the FULL Docker flow (synth → floorplan → place → CTS → route) and confirming non-null P. A PPA-failing variant scores C=0 on the server = rejected = doesn't count for Explorer award. Don't sink time into variants without a green full-flow run.

## V seed uncertainty (per-design strategy)

Mutant seed is derived from submission tar content hash — **any tar change rerolls the seed**. Local V is a single sample from a high-variance distribution. Strategy:

| Don't | Do |
|-------|----|
| Tune tests to chase V=1.0 on the current seed | Cover all REQs with ≥1 test + ≥1 assertion (REQ matrix completeness) |
| Treat V=1.0 as "we're done" | Treat V as expected value over seeds; maximize expected coverage |
| Submit many near-identical variants | Submit one robust variant + a few genuinely-distinct Explorer variants |

A submission with more REQ coverage beats one with a luckier seed. Measure local V only as a sanity check (no unexpected survivors on common classes), not as a target.

## Timing strategy

| Phase | Timing |
|-------|--------|
| Design 1 (datapath) released Nov 7 09:00 | Day 1: analyze + fix + first submission. Day 2: PPA + V iteration. |
| Design 2 (control + bus) released Nov 10 | Day 4: analyze + fix. Day 5: PPA + V. |
| Design 3 (pipelined core) released Nov 13 | Day 7: analyze + fix. Day 8: PPA + V. |
| Window closes Nov 16 23:59 | Days 9-10: cross-design optimization. Re-submit designs 1-2 with late insights. |

Each design: ~3 days dedicated. Submit within 12h of release (baseline fix), then iterate PPA/V. On release, classify the archetype and load the matching section of `docs/skills/design-playbooks.md` (datapath / control+bus / pipelined core) — it sets the expected mutation classes, the highest-yield assertion/test tactics, and the PPA levers for that type.

## Submission rhythm

```
# After Phase 2 (fix): submit immediately. Check C.
# If C < 1.0: read failed test names, fix RTL, resubmit.
# If C = 1.0: focus on V (assertions) and P (PPA optimization).
# Each submission takes 30min cooldown + 15min flow = submit, work on improvements, submit again.
```

## Explorer award strategy

"Most distinct valid submissions (distinct in netlist, C > 0.9)."
After C=1.0 and good V/P: submit microarchitectural variants:
- Different FSM encoding
- Different pipeline depth
- Different resource sharing
- Different area/speed tradeoffs

Each must produce a distinct netlist (different gate count or connectivity).

**MUST validate locally with the full flow before considering a variant as Explorer-eligible.** Synthesis-only or test-only validation is insufficient — aggressive topologies can build, pass tests, then crash OpenROAD at CTS (measured: 8-multiplier parallel FIR added 2.5× cell count and failed at CTS, would score C=0 on server = rejected = not a "valid submission" for Explorer counting). Run the full Docker flow (`pg-flow-commands` skill) and confirm `status: scored` with non-null P. Save survivors in `runs/ppa-candidates/`.

**Realistic per-design count**: 3-5 distinct valid variants. Beyond that, diminishing returns vs. risk of accidentally submitting a low-C variant. Track in `docs/designs/<name>-ppa-plan.md`.

## Practical: always keep a valid submission ready

Never let your "best" submission get worse. Save `runs/sub.tar.gz` only when it scores well. Maintain `runs/sub-best.tar.gz` as a fallback.

## What NOT to do
- Don't skip C validation (C=0.5 kills score even with V=1.0, P=1.0)
- Don't let timing fail (P caps at 0.3 — always verify against constraint.sdc)
- Don't add area without removing elsewhere (keeps P at/below 0.5)
- Don't over-optimize V past the ceiling (equivalent mutants exist in every design)
- Don't make P-neutral RTL changes hoping for V gain — RTL changes re-roll the mutation seed; you're as likely to draw more equivalent mutants as fewer
- Don't attempt PPA micro-optimizations (<100 cell delta) on control+bus designs under 3000 cells *in isolation* — OpenROAD routing noise dominates. But STACKED structural wins (ICG restructure + wrap `==` + AND-OR mux + operand isolation) reach P=0.4965 on a 2652-cell control+bus design
- Don't stack two area-only wins without a timing win in between — the WNS cliff hits
- Don't fuse submodules to enable CSE — the unused source file's mutations become no-op equivalents (survivors → V drops)
- Don't precompute small expressions into flops (`x-1`, masks). Flops cost more than the saved adder unless the expression is multi-input or expensive
- Don't narrow wide registers for area savings (e.g., IRQ 32→12) without first gaining timing slack — WNS collapses and P drops
- Don't try to preserve a lucky V=1.0 seed by refusing PPA improvements. P and V both move; the highest-projective-score archive wins
- Don't spend more than 5–6 submissions on PPA for a control+bus design. Beyond that, we're at the ceiling. Save slots for C-confidence and the next design.
