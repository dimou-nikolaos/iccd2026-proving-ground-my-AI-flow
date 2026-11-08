# Running the Flow — Operational Guide

Companion to the `pg-flow-commands` skill (exact commands) and `docs/joint-flow.md` (the cycle). This file is the narrative/orientation only.

## Session start prompt

Paste this when you drop a new competition design at the repo root as `challenge-<N>/`:

```
Work on challenge-<N> (dir: challenge-<N>/). Follow docs/joint-flow.md and docs/flow.md through all 4 phases.
Rules:
- All tools via Docker proving-ground/flow:public, 12 cores. Never host verilator/yosys/sby.
- Create docs/designs/challenge-<N>.md and docs/state/challenge-<N>-progress.md as your first deliverables.
- First drop: fix deviations + implement CR + report.md + rtl only → package runs/sub.tar.gz, then STOP for upload.
- Spawn agents per docs/task-cards.md. Priority: C=1.0, then P>0.5, then V ceiling.
```

For a resumed session (after a restart):

```
Work on challenge-<N>. Read docs/state/challenge-<N>-progress.md first, then docs/joint-flow.md, and resume from the "Next action for operator" entry.
```

## Docker commands

Exact, copy-paste commands are in the `pg-flow-commands` skill and `docs/skills/flow-commands.md`. The short version:

```bash
mkdir -p runs
# fast (V only, ~23s): add --skip ppa
docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-<N>:/design:ro -v $PWD/runs:/runs \
  proving-ground/flow:public \
  run --design /design --submission /runs/sub.tar.gz --out /runs/out --jobs 12 --cache /runs/cache --skip ppa
```

Always clean artifacts before packaging:
```bash
rm -rf challenge-<N>/tests/sim_build challenge-<N>/tests/results.xml challenge-<N>/tests/__pycache__
```

## Self-validation (your evidence really catches the bugs)

Run YOUR tests against the BASELINE (pre-fix) RTL — the deviation tests must FAIL there and PASS on your fixed RTL. Keep a pristine copy of the original `rtl/` (before you edit) so you can point `RTL_ROOT`/`--design` at it. This is the strongest proof your tests/assertions are discriminating.

## PPA and cache invalidation (IMPORTANT)

The flow hashes the design RTL to key the baseline-PPA cache. Editing `rtl/` in Phase 2 changes the hash, so the cached baseline (from the original RTL) no longer applies.

**Competition note**: the scored baseline is the ORIGINAL released RTL (with the seeded deviations). Pre-cache it before you touch RTL:
```bash
docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-<N>:/design:ro -v $PWD/runs:/runs \
  proving-ground/flow:public \
  baseline --design /design --cache /runs/cache --jobs 12   # run on the UNMODIFIED rtl/ first
```
After Phase 2 edits, the first full run re-keys a new hash dir; copy `baseline_ppa.json` from the original-hash dir into the new one to reuse the cached baseline instead of recomputing. Until you do, use `--skip ppa` for fast V iteration.

## Time budget tracking (IMPORTANT for time-conpressed challenges)

Each Docker invocation has a characteristic duration. Future challenge runs may need to decide based on time left; keep this table current with measured values from each run.

| Task | Typical | Notes |
|------|---------|-------|
| Verilator lint (Docker) | 20-30 s | with `--lint-only` |
| Yosys elaborate only (Docker) | 20-40 s | `read_slang` + `hierarchy -check` + `proc` + `check` |
| Public testbench run | 20-40 s | 21 tests, Verilator build dominates |
| Submission tests @ ASSERTIONS=1 (fresh) | 60-120 s | 60+ tests, Verilator build dominates (~40-60s) |
| Flow run `--skip ppa` (~V only) | 23-30 s | 40 mutants in parallel, 12 cores |
| Flow run full (PPA included) | 6-8 min | baseline PPA cached, only submission PPA runs |
| Flow run full (first time, cache cold) | 12-15 min | baseline+submission PPA both run |
| Synthesis only (yosys synth) | 40-90 s | for cell-count check |
| Full flow with `ODB-0269` route crash (retry) | +6-8 min | nondeterministic; just re-run |
| Package tar.gz (clean + tar) | <5 s | local, no Docker |

**Per-agent wall-clock budgets** (measured challenge-1, 12-core machine):

| Agent task | Typical |
|-----------|---------|
| @spec-analyst (full pass, 30-40 REQs) | 15-25 min |
| @probe-creator (per-finding probe) | 10-15 min per finding |
| @probe-creator (per-REQ sweep, 30-50 REQs) | 45-90 min |
| @rtl-fixer (3-4 deviations + CR) | 20-40 min |
| @test-writer (golden model + 60 tests covering all REQs) | 60-120 min |
| @assertion-writer (interface + signal-def, all REQs) | 45-90 min |
| @flow-runner (one fast cycle) | 1-3 min |
| @ppa-optimizer (Tier A single change) | 15-25 min including measurement |
| @ppa-optimizer (Tier B single change) | 25-40 min including measurement |
| @ppa-optimizer (Tier C structural, e.g., parallel FIR) | 90-180 min including full-flow validation |
| @mutant-hunter (10 survivors, per-line analysis) | 20-40 min |

**Challenge-1 actuals** (measurements from this session — use to calibrate estimates for challenges 2/3):

| Activity | Wall-clock |
|----------|-----------|
| Phase 1 (analyze: static + probe sweep + fix 4 deviations) | ~2.5 h |
| Phase 2 (fix + CR implementation) | ~1.5 h |
| Phase 3 (tests + assertions + REQ matrix to 40/40) | ~3 h |
| Phase 4 iterate (5 PPA submissions + Tier A/B/C variants) | ~3 h |
| Documentation reflection + doc split | ~1.5 h |
| Phase 4e wildcards (2 attempts: 1 promote, 1 revert) | ~1.75 h |
| **Total agent wall-clock on challenge-1 to sub-13** | **~13.25 h** |

**Per-Docker-command measured (challenge-1, 12 cores)**:

| Command | Measured |
|---------|----------|
| Verilator lint+elab with assertions | ~1 s (cold build cache) |
| Cocotb 63-test suite (fresh sim_build) with ASSERTIONS=1 | 24 s |
| Flow `--skip ppa` | ~30 s |
| Flow full (PPA included, warm baseline cache) | 345-390 s |
| Flow full first time after RTL change | ~13 min (recomputes baseline too) |
| Flow `ODB-0269` crash recovery (retry) | +345 s (each retry = another full run) |

**Batched parallel runs (challenge-1 measured, 12 cores total)**:

| Pattern | Per-job | Total wall-clock | vs serial |
|---------|---------|------------------|-----------|
| 5 cocotb test runs in parallel (--cpus 2) | 27s total | 27s | vs 5 × 24 = 120s → **4.4× faster** |
| 2 cocotb test runs in parallel (--cpus 4) | 25s total | 25s | vs 2 × 24 = 48s → **1.9× faster** |
| 2 lint runs in parallel (--cpus 2) | 1s total | 1s | (too fast to measure) |
| 5 full-flow PPA runs in parallel (--cpus 2 --jobs 2) | ~1135s total | 19 min | vs 5 × 345 = 29 min → **1.5× faster** |
| 2 full-flow PPA runs in parallel (--cpus 6 --jobs 6) | ~529s total | 9 min | vs 2 × 345 = 12 min → **1.3× faster** |
| 2 full-flow PPA runs serial | 345s each | 12 min | reference |

**Why batching wins less than expected on full-flow**: mutation phase uses `--jobs` internally for parallel mutant verification. With --cpus 2, only 2 mutants at a time, so mutation stretches from 45s (solo) to 170-200s. PPA phase (OpenROAD detailed route) is mostly single-core and parallelizes well across Docker jobs. Net: 1.5× speedup for 5 parallel PPA runs vs 5 serial.

**When batching wins big**: cocotb tests (4.4× for 5 parallel) because Verilator build phase dominates and is per-test-suite.

**When batching wins modest**: full-flow PPA (1.3-1.5× for 5 parallel) because mutation has internal parallelism that gets throttled when --cpus is low.

**How to use this table**:
- When user asks "can we try X in the remaining time?", compare X's estimated wall-clock to time left.
- If time is tight, prefer Tier A + Tier B (fast, low-risk) over Tier C variants (slow, may CTS-fail).
- A full-flow PPA run is 6-8 min minimum. **You cannot iterate PPA more than ~5 times in 1 hour.** Plan experiment batches accordingly.
- Submissions are rate-limited to 1 per 30 min regardless of local speed. If you have >5 PPA candidates, you can only C-check ~5 per 2.5 hours of wall-clock.

**Update this table** after each challenge with actual measured values. Overconfidence on agent task time is the main scheduling error — measured challenge-1 ran ~1.5h slower than the initial estimate at each phase.
