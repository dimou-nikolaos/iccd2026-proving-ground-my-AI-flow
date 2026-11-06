# Joint Strategy — Operator + Agent

**See also**: `docs/joint-flow.md` — operational cycle (who does what, when, handoffs, file conventions). This file is the high-level strategy; joint-flow is the process.

## Division of labor

| Operator (human) | Agent (me) |
|-----------------|------------|
| Reads spec.md, change_request.md fully | Starts static analysis / find deviations |
| Validates my first drop for correctness | Runs flow, iterates V/PPA |
| Reviews RTL changes for semantic sanity | Writes tests, assertions, optimizes |
| Decides PPA aggressiveness (risk appetite) | Presents PPA optimization options with C-risk notes |
| Decides when to stop per design (diminishing returns) | Continues until interrupted |

## Cycle

```
1. Operator: "design is challenge-N, go" → copy dir, I start immediately
2. Agent: Phases 1→2→3 in parallel (analyze → fix → test+assert → first submission)
3. Agent: ships first drop (RTL fix + CR + report.md + tests + assertions + sub.tar.gz)
4. Operator: interrupts agent, reads spec/CR, reviews drop, gives feedback:
   - "This deviation is correct" / "You missed something in spec"
   - "The CR has an additional requirement you missed: ..."
   - "I saw the hidden tests failed: [names]"
5. Agent: incorporates feedback, resubmits, continues V/PPA iteration
6. Repeat 4-5 each 30min submission window
```

## Priority (per design)

1. **C = 1.0** — non-negotiable. Fix + CR. Submit RTL alone for fastest C feedback (~15s flow).
2. **P > 0.5** — PPA optimization (free from fix, then active restructuring).
3. **V → ceiling** — tests + assertions, accept equivalent mutants.

## PPA strategy (aggressive, with C-risk management)

**Phase A (free)**: Fix removes logic, CR adds minimal (reuse existing signals). Submit.
**Phase B (safe)**: `>=` → `==` where counter can't overshoot. Narrow masked registers. `case`/`unique case` over if-else. Shared subexpression extraction. Each one is a separate submission.
**Phase C (risky)**: Restructure modules, change mux topology, re-architect internal signals. Risk: hidden formal on internals. Detect via C drop → binary search.

Each C≥0.9 submission = Explorer award point. Use all 30 submissions.

## Score targets

| Design | Target C | Target V | Target P | Target Score |
|--------|----------|----------|----------|--------------|
| D1 (datapath) | 1.0 | 0.95 | 0.65+ | 80+ |
| D2 (control) | 1.0 | 0.95 | 0.60+ | 77+ |
| D3 (pipelined) | 1.0 | 0.90 | 0.70+ | 80+ |
| **Total** | | | | **237+** |

Certificates require >150 total. 1st place likely >250.

## Time budget (per design, 3 days)

| Day | Focus | Submissions |
|-----|-------|-------------|
| Day 1 (release) | Analyze + fix + CR + first submit (RTL only for C) | 2-3 |
| Day 2 | V iteration (fast flow), PPA phase B | 5-10 |
| Day 3 | PPA phase C (aggressive), Explorer variants | 10-15 |

## Key heuristics

- **Use 12 cores**: `--cpus 12 --jobs 12` always
- **Fast flow first**: `--skip ppa` for V iteration (~23s). Only full flow for PPA check (~330s).
- **Clean before packaging**: `rm -rf tests/sim_build tests/results.xml tests/__pycache__`
- **HIGH confidence = skip formal/probe**: go straight to fix
- **`>=` → `==` comparator**: saves 50% per comparator (48→24 gates at 16-bit)
- **12-bit IRQ / narrow masked regs**: saves 2.5% cells, helps routing converge
- **Strobe discrimination tests**: write with WRONG lane → verify unchanged. Kills strobe mutants.
- **Address boundary tests**: read first-invalid unmapped → PSLVERR=1. Kills decode off-by-one.
- **Decode assertions**: `wr_reg[n] == (acc_wr && sel && creg == K)` pins every write-decode line
- **V ceiling**: 0.95-1.0 typical. Run 3-5 seeds to confirm before chasing.

## What I do autonomously (between operator interrupts)

- Iterate V (write assertions, run fast flow, check survivors, add tests)
- Try PPA Phase B optimizations one at a time (each as separate tar, each needs new submission)
- Update docs/designs/<name>.md with findings and V ceiling
- Maintain runs/sub-best.tar.gz as fallback

## What operator must do (I can't)

- Upload sub.tar.gz to competition portal
- Report back: C value, hidden test names that failed, P per stage, survivors list
- Read spec.md for ambiguities I might misinterpret
- Decide if PPA change is worth risk
- Stop me when V is at ceiling (I might chase equivalent mutants)
