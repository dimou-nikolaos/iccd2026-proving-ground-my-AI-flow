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
tar czf runs/sub-<N>.tar.gz -C challenge-N rtl tests assertions report.md
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

## Batched concurrent runs (the wall-clock saver)

**Serialization reality**: each bash call my agent makes is one stall. Running N Docker jobs in N separate bash calls = N stalls = N × single-job-wall-clock. Batch them into ONE bash call with `parallel` (or `xargs -P`) so the agent stalls ONCE and gets all results at once. Wall-clock drops from N×T to ~T (with some contention overhead).

 ### Submodule synth pre-filter (fast, cheap, area-only) — **use `scripts/submodule-synth.sh`**

For PPA wildcards that target **area** (operand isolation, memory inference, flop packing): run yosys generic synth on the modified submodule alone, compare cell count vs baseline. ~1s per submodule (parallel-friendly).

```bash
bash scripts/submodule-synth.sh <design_root> <module_name>
# Example:
bash scripts/submodule-synth.sh /tmp/opencode/sr-test/sub15 dsp_gain
# Output: integer cell count (yosys generic, pre-sky130-map)
```

**challenge-1 calibration** (verified 1a): Yosys cell delta correctly flags LARGE area regressions (>+100 cells → likely P-negative). For SMALL deltas (+30 to +100 cells), uncorrelated with P direction. Use as RED FLAG filter only.

Caveats: Yosys `stat` gives PRE-PLACE generic gate count, not sky130-mapped area. Real area is ~30-50× larger (sky130 cells have specific µm² each). Only RELATIVE delta is meaningful. NO timing or power.

### Whole-design synth pre-filter — **use `scripts/synth-filter.sh`**

```bash
bash scripts/synth-filter.sh --top <top_module> runs/sub-a.tar.gz runs/sub-b.tar.gz ...
```

Runs yosys on each tar.gz whole-design in parallel (--cpus 1 each). Outputs cell count per variant. ~5s per variant in parallel (7s wall for 10 variants). Same red-flag rule: only delta >+100 cells is reliable.

### Critical path extraction — **use `scripts/critical-path.sh`**

```bash
# Step 1: run flow with --keep-work flag to retain .odb files
docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-1/baseline:/design:ro -v $PWD/runs:/runs \
  proving-ground/flow:public \
  run --design /design --submission /runs/sub-X.tar.gz --out /runs/out-X --jobs 12 --cache /runs/cache --keep-work

# Step 2: extract worst-slack paths
bash scripts/critical-path.sh runs/out-X
```

**Note**: openroad reports WNS in 6_final.odb differs by ~1ns from flow's reported WNS (flow includes additional SDC pessimism). Use openroad report for DIRECTION (which nets/paths are critical), not absolute slack numbers.

### Batched whole-design full-flow (Tier B/C wildcards validation) — **use `scripts/pg-batch.sh`**

```bash
bash scripts/pg-batch.sh --baseline challenge-1/baseline runs/sub-a.tar.gz runs/sub-b.tar.gz ...
```

Defaults: cpus-per = floor(nproc/N), jobs-per = cpus-per. Override with `--cpus-per N --jobs-per N`.

**Manual pattern (if you want to script it yourself)**:

Build N variant tar.gz files upfront, launch in ONE bash call. **NOTE: GNU `parallel` is NOT installed on this machine.** Use bash `&` + `wait` instead:

```bash
# Pre-build all variants (each in /tmp/opencode/wild-variants/w<N>/rtl, tests, assertions, report.md)
for v in w1 w2 w3 w4 w5; do
  tar czf runs/sub-$v.tar.gz -C /tmp/opencode/wild-variants/$v rtl tests assertions report.md
done

# Launch all in parallel (5 variants × 2 cores each = 10 cores, 2 spare)
for w in w1 w2 w3 w4 w5; do
  ( docker run --rm --cpus 2 -u $(id -u):$(id -g) -e HOME=/tmp \
      -v $PWD/challenge-1/baseline:/design:ro -v $PWD/runs:/runs \
      proving-ground/flow:public \
      run --design /design --submission /runs/sub-$w.tar.gz --out /runs/out-$w --jobs 2 --cache /runs/cache \
      > /tmp/opencode/flow_$w.log 2>&1 ) &
done
wait
```

**Batching parameter rule** (validated challenge-1 1c):
- **Match `--jobs == --cpus`** per container. Over-allocating jobs (jobs > cpus) HURTS.
- 2-3 variants: `--cpus 6 --jobs 6` saves ~30% wall vs serial
- 5 variants: `--cpus 2 --jobs 2` saves ~35% wall vs serial
- 8 variants: `--cpus 1 --jobs 2` saves ~75% wall for cocotb, ~70% for --skip ppa
- >5 variants: split into multiple waves of 5

**Cache sharing**: safe. Baseline PPA is read-only after first compute. Each variant's submission PPA writes to its own `--out` dir. No cross-variant race.

**Failure modes in a batch**:
- One variant crashes ODB-0269 at route → its report has `status=scored`, `P=null`. Others complete normally. Re-run that variant alone.
- One variant CTS-crashes (lesson L14, design too dense) → same pattern. Abandon that variant, others unaffected.
- All variants finish within ~2× single-run time when 5× --cpus 2, i.e., ~10-12 min instead of 5 × 6 = 30 min serial.

**When NOT to batch**:
- Long interactive exploration where each result informs the next.
- Mutant hunting (already internally parallelized by the flow).
- Any operation that writes to shared mutable state (baseline PPA write, cache recompute).

## Gotchas (learned)
- **PPA baseline source**: the flow computes the baseline PPA from the `--design` dir's `rtl/` (cached in `<cache>/baseline_ppa.json`). If you edit `rtl/` in place, `--design` must point at a PRISTINE mirror (released `rtl/` + `design.yaml` + `tb/` + `flow/`) or P is fixed-vs-fixed (≈0.5, meaningless). Use a separate cache dir per baseline.
- **Full flow (with ppa) takes ~420s** — set the shell/docker timeout >600s; `--skip ppa` is ~30-90s.
- **`ODB-0269` route crash** at detailed route ("unable to open ... to write markers") can happen even at 0 DRC violations (OpenROAD bug, nondeterministic). Just re-run; the baseline PPA is cached so only the submission re-runs.
- **Mutation seed is deterministic** per submission archive (`seed=f(sub_hash)`): same tar -> identical 40 mutants + survivors. No averaging. The mutant SET changes when the RTL changes (hash), so write behavior-based evidence, not line-targeted.
- **Never build-test on host** Verilator/cocotb; and `rm -rf sim_build` before switching `RTL_ROOT` (Verilator reuses the cached `Vtop`).
- **Validate evidence with `ASSERTIONS=1` (fresh sim_build) before packaging** — an assertion firing on your own design REJECTS the whole submission.

## For deeper context
Read `docs/skills/flow-commands.md` (12KB, full details on build checks, rejection reasons, PPA details).
