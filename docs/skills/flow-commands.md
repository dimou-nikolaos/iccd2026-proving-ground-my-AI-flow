# Flow Commands: Exact Commands, Flags, and Report Formats

Reference for `@flow-runner`. All commands needed to run public tests, submission tests, and the full scoring flow.

## Prerequisites

- **Docker**: for the flow image (contains Verilator 5.048, cocotb 2.0.1, Yosys, OpenROAD, SymbiYosys)
- **Local tools** (for quick iteration without Docker): Verilator 5.036+, Python 3.9+, cocotb 2.0.1

```bash
pip install "cocotb==2.0.1"  # for local test runs
```

## Flow Image

Load prebuilt or build from `flow-image/docker/Dockerfile.public`:
```bash
# Load prebuilt (1.8GB file):
docker load -i proving-ground-flow-public-0.1.0.tar.gz

# Build (30-60 minutes):
docker build -f flow-image/docker/Dockerfile.public --target public -t proving-ground/flow:public flow-image/
```

Image contains: Verilator 5.048, cocotb 2.0.1, Yosys 0.68 (with slang), SymbiYosys, yices, boolector, z3, OpenROAD, sky130hd PDK, `pgflow` runner.

## Run Public Tests (no Docker needed)

```bash
# Run all public tests on baseline RTL:
make -C challenge-1/tb/public

# Run on a different RTL tree (must have rtl/filelist.f):
make -C challenge-1/tb/public RTL_ROOT=path/to/rtl/tree

# Run single test:
COCOTB_TEST_FILTER=test_name make -C challenge-1/tb/public

# Results written to: tb/public/results.xml (JUnit XML format)
```

## Run Submission Tests (your own tests/ + assertions/)

```bash
# Clean artifacts, then run your tests WITH assertions against the fixed RTL:
rm -rf challenge-1/tests/sim_build challenge-1/tests/results.xml challenge-1/tests/__pycache__
make -C challenge-1/tests RTL_ROOT=$PWD/challenge-1 ASSERTIONS=1

# Tests only (no assertions):
make -C challenge-1/tests RTL_ROOT=$PWD/challenge-1 ASSERTIONS=0

# Run your tests against the BASELINE RTL (expect deviation tests to fail):
RTL_ROOT=/path/to/baseline make -C challenge-1/tests ASSERTIONS=0
```

## Full Flow Run (Docker)

### Package the submission
```bash
mkdir -p runs
tar czf runs/sub.tar.gz -C <submission-dir> rtl tests assertions report.md
```

`<submission-dir>` must contain: `rtl/`, `tests/`, `assertions/`, `report.md`. A single wrapping folder is also accepted. `trajectory/` (optional agent transcript) is accepted but ignored in scoring.

### Run: fast iteration (skip PPA)
```bash
docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-1:/design:ro \
  -v $PWD/runs:/runs \
  proving-ground/flow:public \
  run --design /design --submission /runs/sub.tar.gz --out /runs/out --jobs 12 --cache /runs/cache --skip ppa
```

### Run: full scoring (all stages)
```bash
docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-1:/design:ro \
  -v $PWD/runs:/runs \
  proving-ground/flow:public \
  run --design /design --submission /runs/sub.tar.gz --out /runs/out --jobs 12 --cache /runs/cache
```

### Pre-cache baseline PPA (once per design, speeds up full runs)
```bash
docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-1:/design:ro \
  -v $PWD/runs:/runs \
  proving-ground/flow:public \
  baseline --design /design --cache /runs/cache --jobs 12
```

### Useful flags
| Flag | Purpose |
|------|---------|
| `--skip ppa` | Skip synthesis + place-and-route (fast test/mutation iteration) |
| `--skip mutation` | Skip mutation analysis (just check evidence passes) |
| `--keep-work` | Keep `out/work/` for debugging build/test directories |
| `--jobs N` | Parallelism (default: all CPUs) |
| `--cache DIR` | Baseline cache location (reused across runs) |
| `--mode public` | Default. Skip hidden checks (server only) |

## Reading the Report

### report.json structure
```json
{
  "status": "scored",
  "design_id": "challenge-1",
  "mode": "public",
  "V": 0.825,
  "P": 0.52,
  "C": null,
  "projected_score_if_all_hidden_checks_pass": 0.67,
  "seconds": {"build": 12.5, "public": 45.2, "mutation": 120.3, "ppa": 300.0},
  "stages": {
    "build": {"status": "pass"},
    "public": {"status": "pass", "details": {"evidence_seconds": 45.2}},
    "mutation": {"status": "pass", "details": {"V": 0.825, "killed": 33, "counted": 40, "survivors": [...]}},
    "ppa": {"status": "pass", "details": {"score": {"P": 0.52, ...}, "final": {...}, "baseline": {...}}}
  }
}
```

### Surviving mutants format
```json
"survivors": [
  {"file": "rtl/<file>.sv", "line": 84, "id": 7},
  {"file": "rtl/<file2>.sv", "line": 23, "id": 12}
]
```

### PPA details (in report.md, human-readable)
Table showing area, cell count, utilization, slack, power, wirelength at each stage: synthesis → floorplan → placement → CTS → global route → detailed route → final. For both your design and baseline.

### report.md (human-readable)
```
runs/out/report.md  # human-readable
runs/out/report.json  # machine-readable (same content)
runs/out/logs/  # build, lint, testbench, mutation, place-and-route logs
```

## Build Stage Checks (what causes rejection)

1. Archive must contain `rtl/filelist.f` matching the released filelist exactly (comments use `#` or `//`, both handled)
2. No clock constraint change (any `.sdc` in submission must match released)
3. No forbidden constructs in RTL: `initial`, `final`, delays (`#`), `fork`, `wait`, `force`, `release`, classes, DPI, program blocks
4. No forbidden system calls: `$readmemh`, `$fopen`, `$system`, `$random`, `$urandom`, `$value$plusargs`, etc.
5. Verilator lint must pass (`--lint-only`)
6. Yosys elaboration must pass (`read_slang` + `hierarchy -check` + `proc` + `check`)
7. Top-level ports must match baseline exactly (direction + width of every port)

## Public/Evidence Stage Checks (what causes rejection)

1. RTL + assertion files compile together with `verilator --assert`
2. Public testbench runs (informational — failure is OK if CR changed behavior)
3. **Your tests must all pass AND no assertion may fire on your own design**
4. Your evidence runtime determines per-mutant timeout

## Assertion File Handling

- Files in `assertions/` with suffix `.sv` or `.v` are compiled (NOT `.svh`)
- Built with: `verilator --assert` + design's `build_args` (default: `-Wno-fatal --x-assign unique --x-initial unique`)
- Simulation uses `+verilator+rand+reset+0` (registers start at 0) and `+verilator+seed+<derived_from_submission_hash>`
- Assertion failure detection: regex matches "Assertion failed", "%Error:.*assert", or "[N] %Error" in simulation log

## Per-Mutant Timeout Formula

```
per_mutant_timeout = min(300, max(30, 2 * evidence_seconds + 15))
```

If your tests take 45s to run → timeout = 105s per mutant. Tests too long → mutants get less time → timeouts kill them trivially but reduce your ability to detect subtle mutations.

## Score Formula (local, public mode)

```
projected_score = 100 × 1.0 × (0.5 × V + 0.5 × P)
```
(Local mode assumes C=1 since hidden checks aren't run)

## Yosys + Verilator Quick Checks (local, no Docker)

```bash
# Verilator lint:
verilator --lint-only -Wno-fatal -Wno-DECLFILENAME -Wno-UNUSEDSIGNAL \
  --top-module <top> $(cat rtl/filelist.f)

# Yosys elaboration:
yosys -q -p "read_slang --top <top> $(cat rtl/filelist.f); hierarchy -check -top <top>; proc; check"

# Build assertions with design:
verilator --assert -Wno-fatal --x-assign unique --x-initial unique \
  --top-module <top> $(cat rtl/filelist.f) assertions/*.sv --cc <top> --exe
```
