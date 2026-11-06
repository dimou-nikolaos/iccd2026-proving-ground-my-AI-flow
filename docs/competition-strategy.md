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

### Risk

Hidden regression tests and hidden formal properties may reference internal signals or exact cycle timing. Aggressive Phase C changes can drop C below 1.0. Mitigation: submit one change at a time; if C drops, that specific behavior is hidden-checked — revert and binary-search the sub-list. The 30-submission budget is the search budget.

## Timing strategy

| Phase | Timing |
|-------|--------|
| Design 1 (datapath) released Nov 7 09:00 | Day 1: analyze + fix + first submission. Day 2: PPA + V iteration. |
| Design 2 (control + bus) released Nov 10 | Day 4: analyze + fix. Day 5: PPA + V. |
| Design 3 (pipelined core) released Nov 13 | Day 7: analyze + fix. Day 8: PPA + V. |
| Window closes Nov 16 23:59 | Days 9-10: cross-design optimization. Re-submit designs 1-2 with late insights. |

Each design: ~3 days dedicated. Submit within 12h of release (baseline fix), then iterate PPA/V.

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

## Practical: always keep a valid submission ready

Never let your "best" submission get worse. Save `runs/sub.tar.gz` only when it scores well. Maintain `runs/sub-best.tar.gz` as a fallback.

## What NOT to do
- Don't skip C validation (C=0.5 kills score even with V=1.0, P=1.0)
- Don't let timing fail (P caps at 0.3 — always verify against constraint.sdc)
- Don't add area without removing elsewhere (keeps P at/below 0.5)
- Don't over-optimize V past the ceiling (equivalent mutants exist in every design)
