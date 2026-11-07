---
name: pg-flow-commands
description: Exact commands for running the Proving Ground flow — packaging, running Docker, iterating fast, reading reports
---

## What I do

Provide the exact copy-paste commands for every step of the Proving Ground flow pipeline.

## Quick reference

### Public tests (local, no Docker)
```bash
make -C challenge-N/tb/public
COCOTB_TEST_FILTER=test_name make -C challenge-N/tb/public
make -C challenge-N/tb/public RTL_ROOT=path/to/rtl/tree
```

### Your submission tests (with assertions)
```bash
make -C challenge-N/tests RTL_ROOT=$PWD/challenge-N ASSERTIONS=1   # with assertions
make -C challenge-N/tests RTL_ROOT=$PWD/challenge-N ASSERTIONS=0   # tests only
# Always clean build artifacts first:
rm -rf challenge-N/tests/sim_build challenge-N/tests/results.xml challenge-N/tests/__pycache__
```

### Package submission
```bash
tar czf runs/sub.tar.gz -C <dir> rtl tests assertions report.md
```

### Run flow — fast iteration (skip PPA) — ~23s
```bash
docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-N:/design:ro \
  -v $PWD/runs:/runs \
  proving-ground/flow:public \
  run --design /design --submission /runs/sub.tar.gz --out /runs/out --jobs 12 --cache /runs/cache --skip ppa
```

### Run flow — full scoring — ~330s
```bash
docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-N:/design:ro \
  -v $PWD/runs:/runs \
  proving-ground/flow:public \
  run --design /design --submission /runs/sub.tar.gz --out /runs/out --jobs 12 --cache /runs/cache
```

### Cache baseline PPA (run once)
```bash
docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-N:/design:ro \
  -v $PWD/runs:/runs \
  proving-ground/flow:public \
  baseline --design /design --cache /runs/cache --jobs 12
```

### Skip options
- `--skip ppa` — skip synthesis+PnR (fast test/mutation loop)
- `--skip mutation` — skip mutation analysis (just verify evidence passes)

## Reading results
- `runs/out/report.json` — machine-readable
- `runs/out/report.md` — human-readable
- Surviving mutants: `report.stages.mutation.details.survivors` → `[{file, line, id}]`
- Per-mutant timeout: `min(300, max(30, 2*evidence_seconds + 15))`

## Gotchas (learned)
- **PPA baseline source**: the flow computes the baseline PPA from the `--design` dir's `rtl/` (cached in `<cache>/baseline_ppa.json`). If you edit `rtl/` in place, `--design` must point at a PRISTINE mirror (released `rtl/` + `design.yaml` + `tb/` + `flow/`) or P is fixed-vs-fixed (≈0.5, meaningless). Use a separate cache dir per baseline.
- **Full flow (with ppa) takes ~420s** — set the shell/docker timeout >600s; `--skip ppa` is ~30-90s.
- **`ODB-0269` route crash** at detailed route ("unable to open ... to write markers") can happen even at 0 DRC violations (OpenROAD bug, nondeterministic). Just re-run; the baseline PPA is cached so only the submission re-runs.
- **Mutation seed is deterministic** per submission archive (`seed=f(sub_hash)`): same tar -> identical 40 mutants + survivors. No averaging. The mutant SET changes when the RTL changes (hash), so write behavior-based evidence, not line-targeted.
- **Never build-test on host** Verilator/cocotb; and `rm -rf sim_build` before switching `RTL_ROOT` (Verilator reuses the cached `Vtop`).
- **Validate evidence with `ASSERTIONS=1` (fresh sim_build) before packaging** — an assertion firing on your own design REJECTS the whole submission.

## For deeper context
Read `docs/skills/flow-commands.md` (12KB, full details on build checks, rejection reasons, PPA details).
