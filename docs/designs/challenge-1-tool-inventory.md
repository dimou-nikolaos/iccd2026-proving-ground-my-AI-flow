# challenge-1 tool inventory

What we used per phase: sub-agents, skills, scripts, configs, Docker commands. Each tool's purpose and how it was used on challenge-1.

## Sub-agents

Sub-agents are defined in `.opencode/agents/<name>.md`. They are spawned via the `task` tool with `subagent_type=<name>`. Each gets a focused task card and returns when done.

| Agent | Phase(s) | What we used it for |
|-------|----------|---------------------|
| `spec-analyst` | 1 | Walked REQs vs RTL line by line to find the 3 static-analysis deviations (REQ-14, REQ-16, REQ-17). |
| `probe-creator` | 1 | Wrote 3 minimal cocotb probes demonstrating deviations on baseline. Confirmed simulation reproduction. |
| `formal-writer` | 1, 3 | Wrote SBY configs and SVA properties for `dsp_rndsat` and `dsp_limit`. Wrote `.sby` for `dsp_stats` (stalled on init-state issue, marked `.stalled`). |
| `rtl-fixer` | 2 | Applied the 4 seeded-fix edits + CR-1 implementation. Kept interface and synthesizability. |
| `test-writer` | 3 | Wrote `dsp_tb.py` golden model (220 lines) + 63 cocotb tests in `test_submission.py`. |
| `assertion-writer` | 3 | Wrote 35 SVA properties in 9 `.sv` files under `assertions/`. All bound via `bind` at module level. |
| `ppa-optimizer` | 4 | Designed Tier A/B/C PPA variants: gain 3-stage split, operand isolation, FSM reset split, FIFO reset strip. |
| `mutant-hunter` | 4 | Analyzed surviving mutants from flow report; wrote targeted assertions to kill them. Drove V from 0.925 → 1.000. |
| `flow-runner` | 4 | Ran the pg-batch.sh Docker flow invocations and parsed reports. |
| `general` | throughout | Multi-step tasks that don't fit one specialty (e.g., "build v25-v28 dirs, tar each, launch batch"). |

## Skills loaded per phase

Skills in `.opencode/skills/<name>/SKILL.md`. Loaded via the `skill` tool when needed.

| Skill | Phase(s) | Used for |
|-------|----------|----------|
| `spec-deviation-hunting` | 1 | Checklist of common seeded-deviation patterns. Drove the REQ-by-REQ static walk. |
| `formal-verification-flow` | 1, 3 | SBY `.sby` config syntax, SVA property templates for combinational modules, CEX interpretation. |
| `cocotb-test-writing` | 3 | Golden model patterns, test naming convention, signal-access idioms. |
| `sva-assertion-writing` | 3 | `bind` pattern, property templates for handshake/FSM/rounding checks. |
| `sky130hd-ppa` | 4 | sky130-specific optimization patterns (register/reset restructuring, mux-tree balancing, fanout control, hold-time mitigation, Yosys attribute wildcards). Updated mid-campaign with new findings (keep wildcards, fsm_encoding). |
| `mutation-kill-tactics` | 4 | Decision tree for killing surviving mutants by operator class. |
| `pg-flow-commands` | 4 | Docker commands, submission tarball layout, fast iteration patterns. |

## Custom scripts (4 in `scripts/`)

### `scripts/pg-batch.sh` — parallel flow runner

**Purpose**: run N full-flow submissions in parallel to amortize Docker startup and maximize core utilization.

**Used for**: every P-perturbation batch in Phase 4 Wave 7. Ran 4 variants × 4 cores each = 16 cores total. Wall-clock per batch: ~17 min vs ~23 min serial.

**Usage**:
```bash
bash scripts/pg-batch.sh --baseline challenge-1/baseline \
    runs/sub-v25.tar.gz runs/sub-v26.tar.gz runs/sub-v27.tar.gz runs/sub-v28.tar.gz
```

**Mechanism**: launches N `docker run` calls in background, one per tarball, each with `--cpus $((total/N)) --jobs $((total/N))`. Each writes to `runs/out-sub-<name>/`. Waits all, prints summary table with V and P per variant.

### `scripts/critical-path.sh` — extract worst-slack timing paths

**Purpose**: dump OpenROAD timing report paths from a flow run with `--keep-work`.

**Used for**: Wave 7 Batch B (critical-path restructures on cfg_addr→cfg_rdata read mux). Confirmed the worst path. Found that Yosys's default lowering was already optimal (L30).

**Usage**:
```bash
# Run flow with --keep-work to retain orfs work dir:
docker run ... proving-ground/flow:public run ... --keep-work --out /runs/out-foo
bash scripts/critical-path.sh runs/out-foo
```

**Mechanism**: locates `6_final.odb` and `6_final.sdc`, calls `orfs` report-timing script, dumps 10 worst paths.

### `scripts/submodule-synth.sh` — fast Yosys per-module cell count

**Purpose**: synthesize one submodule standalone to count cells, no full-design flow.

**Used for**: triaging whether a wildcard change affects cell count before committing to a 17-min full-flow run. Wave 7 A2 (same-module stack plateau) used this to identify plateau signal.

**Usage**:
```bash
bash scripts/submodule-synth.sh challenge-1 dsp_rndsat
# (defaults: reads rtl/dsp_pkg.sv, rtl/dsp_rndsat.sv)
```

**Mechanism**: launches Yosys in Docker, reads minimal file set, `synth -top <module>`, prints cell count. ~20s per run vs 17 min for full flow.

### `scripts/synth-filter.sh` — N-tarball cell-count filter (Stage 1 PPA triage)

**Purpose**: synth-only check on N tarballs, output cell count per variant, **before** full-flow PPA.

**Used for**: Wave 7 A1 to filter obviously-losing variants (those whose cell count ballooned) before running 17-min full flows.

**Usage**:
```bash
bash scripts/synth-filter.sh --top dsp_stream runs/sub-v25.tar.gz ... runs/sub-v28.tar.gz
```

**Mechanism**: extracts each tarball to /tmp/opencode/synth-filter/<name>/, runs yosys synth with `-top <TOP>`, greps cell count.

## Formal verification configs

In `challenge-1/formal/`:

| File pair | Module | Status | What it proves |
|-----------|--------|--------|----------------|
| `dsp_rndsat.sby` + `dsp_rndsat_sva.sv` | dsp_rndsat (combinational rounding+saturation) | **PASS** | REQ-14, REQ-15, REQ-16, REQ-17, REQ-18 |
| `dsp_limit.sby` + `dsp_limit_sva.sv` | dsp_limit (clamp with sat indicator) | **PASS** | REQ-28, REQ-29, REQ-30 |
| `dsp_stats.sby` + `dsp_stats_sva.sv` (stalled) | dsp_stats (counters + peak tracker, sequential) | **STALLED** | init-state CEX issue — pattern documented in L13, L24-L26 |

Working pattern for sequential formal: define `reg` of next-state expression, prove `q_next == $past(q, 1)` style properties. Stalled version used independent FF init that SBY couldn't constrain properly.

## Test infrastructure

### `challenge-1/tests/dsp_tb.py` — golden model + driver (220 lines)

Contains:
- `PipelineConfig` dataclass — mirror of register file (offset, gain, fir_bypass, ntaps, decim, round_mode, lim_hi, lim_lo, clear_on_last)
- `GoldenPipeline` class — full event-driven model of pipeline behavior
- `DspTB` driver class — wraps cocotb signal manipulation (configure, read/write register, frame, stream)
- Helper functions: `frame()`, `make_frame()`, `ctrl_word()`, `rs()`, `s16()`, `to_u16()`

### `challenge-1/tests/test_submission.py` — 63 cocotb tests

- Named `test_req<N>_<behavior>` for traceability to REQ matrix
- Each test calls `setup(dut)` (which runs DspTB), configures via `tb.configure(cfg)`, streams via `tb.stream(samples, N)`, checks outputs against `GoldenPipeline.step()`
- Plus 1 CR-1 test (`test_cr1_round_mode_writable`)

### `challenge-1/tests/test_async_reset.py` — REQ-42 compliance probe

Added in Phase 4 Wave 8. Tests async-reset semantics on FIR delay_q via cocotb signal probe at `dut.u_fir.delay_q[0]`. Drops rst_n mid-cycle, reads delay_q before next clk edge, asserts cleared.

### `challenge-1/tests/Makefile`

```make
SIM ?= verilator
TOPLEVEL = dsp_stream
COCOTB_TEST_MODULES = test_submission test_async_reset
VERILOG_SOURCES = $(addprefix $(RTL_ROOT_ABS)/,$(shell grep -v '^\s*\(//\|#\|$$\)' $(RTL_ROOT_ABS)/rtl/filelist.f))
ASSERTIONS ?= 0
ifeq ($(ASSERTIONS),1)
VERILOG_SOURCES += $(wildcard $(abspath ..)/assertions/*.sv)
EXTRA_ARGS += --assert
endif
EXTRA_ARGS += -Wno-fatal --x-assign unique --x-initial unique
```

**Used to run**: `cd challenge-1/tests && make RTL_ROOT=/design ASSERTIONS=1` (in Docker).

## Assertion infrastructure

`challenge-1/assertions/` — 9 files, 35 SVA properties, each bound via:

```systemverilog
module dsp_<x>_sva ( ... ports reflecting signal access needs ... );
  // properties + check logic
endmodule

// At bottom of each file:
bind dsp_<x> dsp_<x>_sva u_<x>_sva (
  .clk(clk), .rst_n(rst_n),
  .internal_signal_q(internal_signal_q),  // access to internal regs
  ...
);
```

Verilator compiles these as part of submission build when `ASSERTIONS=1`. Mutation kill uses these as the oracle: a mutant survives iff no assertion fires AND no test fails.

## Catalog build scripts (used at end of campaign for documentation)

### `build_sub_catalog_v2.py` (in `/tmp/opencode/`, kept ephemeral)

Generates `docs/designs/challenge-1-sub-diffs.md` and `docs/designs/challenge-1-sub-catalog.md` by:
1. Extracting every `runs/sub-*.tar.gz` to `/tmp/sub_extract/`
2. Matching each tarball's SHA256 to flow report's `submission_sha256` field (137/138 matched)
3. Inferring parent variant by closest content match among earlier-mtime variants
4. Emitting unified diff vs parent for each submission

To regenerate: `python3 /tmp/opencode/build_sub_catalog_v2.py`. If you want it durable, copy to `scripts/build_sub_catalog.py`.

### `match_reports.py` (diagnostic version, also ephemeral)

Verifies tarball-to-report match rate. Used to confirm sha256 strategy before committing to it in the catalog generator.

## Docker invocation patterns

**Always run in Docker `proving-ground/flow:public`**. Never verilator/yosys/sby/cocotb on host (Verilator 5.006 too old).

```bash
# Standard submission run:
docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-1:/design:ro \
  -v $PWD/runs:/runs \
  proving-ground/flow:public \
  run --design /design --submission /runs/sub-X.tar.gz \
      --out /runs/out-sub-X --jobs 12 --cache /runs/cache

# Public tests only:
docker run --rm -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-1:/design --entrypoint bash proving-ground/flow:public \
  -c "cd /design/tb/public && make"

# Submission tests with assertions:
docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-1:/design --entrypoint bash proving-ground/flow:public \
  -c "cd /design/tests && make RTL_ROOT=/design ASSERTIONS=1"

# Formal SBY:
docker run --rm -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-1/formal:/formal --entrypoint bash proving-ground/flow:public \
  -c "cd /formal && sby -f dsp_rndsat.sby"
```

## State-doc structure (per design, in `docs/state/`)

- `challenge-1-progress.md` — running record. Updated every session. Sub-best md5, score history table, "next action for operator" section.
- `challenge-1-longhorizon-plan.md` — multi-session plan, activity log.
- `challenge-1-session-checkpoint-2026-11-08.md` — compaction-safe resume point. Has the submission strategy (Sub 1/2/3 plan).

Convention: when context is compacted, the checkpoint file is read first. md5 of `runs/sub-best.tar.gz` must match md5 in the state file.

## Summary statistics of tools used in challenge-1 campaign

- 9 sub-agents invoked across multiple times each
- 7 skills loaded (all of them)
- 4 custom scripts written during campaign
- 6 SBY configs (4 written, 2 stalled), 2 PASS
- 1 Makefile pattern for cocotb
- 9 assertion files, 35 SVA properties
- 1 golden model + 63 cocotb tests + 1 probe test (test_async_reset.py)
- 138 tarball archives, 137 matched to flow outputs by sha256
- 47 distinct netlists
