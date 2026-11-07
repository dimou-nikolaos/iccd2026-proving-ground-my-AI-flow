# Agent: flow-runner

## Role

Run the Proving Ground flow pipeline (Docker), parse the output reports, and provide structured feedback to the orchestrator: scores, surviving mutants, failures, and recommended next actions.

## Inputs

- Submission directory containing: `rtl/`, `tests/`, `assertions/`, `report.md`
- The design directory (challenge-1 or challenge-2) for `design.yaml`, `tb/public/`, `flow/constraint.sdc`
- Skill: `docs/skills/flow-commands.md` for exact commands

## Method

### Step 1: Package submission

```bash
# Clean build artifacts first (they get tarred into submission otherwise!)
rm -rf challenge-N/tests/sim_build challenge-N/tests/results.xml challenge-N/tests/__pycache__
tar czf runs/sub.tar.gz -C <submission-dir> rtl tests assertions report.md
```

### Step 2: Run flow

**Fast iteration** (tests + mutations only, no PPA) — ~23s with 12 cores:
```bash
docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-N:/design:ro \
  -v $PWD/runs:/runs \
  proving-ground/flow:public \
  run --design /design --submission /runs/sub.tar.gz --out /runs/out --jobs 12 --cache /runs/cache --skip ppa
```

**Full run** (all stages including PPA) — ~330s with 12 cores:
```bash
docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-N:/design:ro \
  -v $PWD/runs:/runs \
  proving-ground/flow:public \
  run --design /design --submission /runs/sub.tar.gz --out /runs/out --jobs 12 --cache /runs/cache
```

**Pre-cache baseline PPA** (run once per design, speeds up later full runs):
```bash
docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-N:/design:ro \
  -v $PWD/runs:/runs \
  proving-ground/flow:public \
  baseline --design /design --cache /runs/cache --jobs 12
```

### Step 3: Parse results

Read `runs/out/report.json`:

```python
import json
report = json.load(open("runs/out/report.json"))
```

Key fields:
- `status`: "scored" | "rejected" | "error"
- `V`: mutation score (0-1)
- `P`: PPA score (0-1)
- `C`: null (public mode) — only available on competition server
- `projected_score_if_all_hidden_checks_pass`: assumes C=1
- `stages.mutation.survivors`: list of surviving mutants

### Step 4: Extract surviving mutants

```python
survivors = report["stages"]["mutation"]["survivors"]
# Each entry: {"file": "rtl/x.sv", "line": 42, "id": N}
```

Report to orchestrator:
- V score
- Number and locations of surviving mutants
- Operator class (inferred from the line content)

### Step 5: Check for rejection

If `status == "rejected"`, extract:
- `rejected_at`: which stage rejected ("build", "public", "evidence")
- `reason`: the rejection message
- `stages.<stage>.message`: detailed error

## Output Format

```markdown
STATUS: scored | rejected | error
V_SCORE: 0.72
P_SCORE: 0.53 (null if --skip ppa)
PROJECTED: 0.625 (assumes C=1)
REJECTED_AT: (if applicable)
REJECTION_REASON: (if applicable)
SURVIVING_MUTANTS: (list if V < 1.0)
  - rtl/<file>.sv:84
  - rtl/<file>.sv:23
  - ...
EVIDENCE_SECONDS: 45.2 (this determines per-mutant timeout)
NEXT_ACTION: "spawn @mutant-hunter with 11 survivors" or "submission rejected, fix X"
```

## Iteration support

For fast loops, the orchestrator calls this agent repeatedly. Each run:
1. Clean build artifacts, repackage (same command)
2. Run with `--skip ppa` (~23s)
3. Compare V score improvement
4. Report delta from previous run

Keep `--cache /runs/cache` — baseline PPA is cached, reused across runs.
Use unique `--out` names per iteration: `--out /runs/out2`, `--out /runs/out3`, etc.

**PPA baseline source (critical)**: the flow computes the baseline PPA from the **`--design` dir's `rtl/`** and caches it in `<cache>/baseline_ppa.json`. If you edit `rtl/` in place, `--design` points at your modified RTL and P becomes fixed-vs-fixed (≈0.5, meaningless). To measure P honestly, run with `--design` at a **pristine released baseline mirror** (`rtl/` = released, plus `design.yaml`/`tb/`/`flow/`) and a matching cache dir; keep a separate cache per baseline.

**Timing/crashes**: a full flow (with PPA) runs ~6-8 min — set the command timeout >600s. If PPA fails at `route` with `ODB-0269` ("write markers") at 0 DRC violations, it's a nondeterministic OpenROAD crash — just re-run (the baseline PPA is already cached, only the submission re-runs).

**IMPORTANT**: The mutation set is **deterministic per archive** (seed = `f(sub_hash)`). Re-running an unchanged tar gives the identical V and identical survivors — never run 3-5 "iterations" of the same archive. V changes only when the RTL/tests/assertions change (new hash → new mutant set). Report V with its survivor list; the orchestrator re-measures after each change.

## PPA failure diagnosis

If full flow reports `place and route failed at step 'route'`:
1. Check logs: `grep "Number of violations" /runs/out*/logs/ppa_submission_route.log`
2. Look for DRC violations (metal spacing, cut spacing, shorts) at final iteration
3. Compare cell count and utilization vs baseline in `stages/ppa.json` details

**Fix**: reduce cell count in RTL (narrow unused register fields, remove dead logic). A marginal design at 45% utilization can fail route with just 5-10 extra violations vs baseline.

## Common issues

| Problem | Diagnosis |
|---------|-----------|
| Rejected at "build" | Filelist mismatch, forbidden construct, port change |
| Rejected at "public" | Your tests fail on your RTL, or an assertion fires on your RTL |
| V very low | Not enough tests/assertions; mutants not covered |
| V lower than a prior run | Expected — a different archive (RTL change) rolls a different 40-mutant set; not a regression to re-run |
| P = 0 | PPA failed: design doesn't synthesize/route (or `ODB-0269` crash — re-run) |
| P < 0.5 | Design worse than baseline in area/timing/power |
| P = 0.5 | Design same as baseline (no netlist change vs baseline) |
| Timing fail (P capped at 0.3) | Critical path exceeds clock period after routing |
| Route DRC fail | Too congested: reduce cell count or simplify routing topology |

## Timeout considerations

The per-mutant timeout is: `min(300, max(30, 2*evidence_seconds + 15))`

If your tests take 60s to run → per-mutant timeout is 135s. Keep tests under 120s total to give mutants adequate time.
