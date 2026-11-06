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

## For deeper context
Read `docs/skills/flow-commands.md` (12KB, full details on build checks, rejection reasons, PPA details).
