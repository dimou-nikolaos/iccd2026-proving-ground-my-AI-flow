# Proving Ground ICCD 2026

SystemVerilog RTL design verification challenge. Each released design ships a seeded-deviation baseline RTL, spec, change request, and public cocotb testbench. See `README.md` for the operator quick-start (what to copy where, starting prompt).

## Critical constraints (violations = rejection)

- Do NOT change: top module name, port names/widths, `rtl/filelist.f`, `flow/constraint.sdc`
- RTL must be synthesizable: no `initial`, `#` delays, `fork`, `force`, DPI, classes, `$readmemh`, `$fopen`, `$system`, `$random`, `$urandom`, `$value$plusargs`
- Filelist must match released version exactly (same filenames, same order)
- Tests and assertions must pass on your own design within 10 minutes

## Tool environment (MANDATORY)

**All simulation, lint, synthesis, and formal tools MUST run inside the Docker image `proving-ground/flow:public`.** The host has Verilator 5.006 (too old for SVA features used by the flow). Never run verilator, yosys, sby, cocotb, or make (for sim targets) on the host. Always use:

```bash
docker run --rm -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/<design>:/design:ro --entrypoint bash proving-ground/flow:public \
  -c "<command>"
```

Use `:ro` for lint/elab/flow commands; omit `:ro` for sim builds that write `sim_build/`.

## The 4-Phase Workflow

1. **Analyze** — find spec deviations via static analysis, formal, and probes
2. **Fix** — correct RTL deviations + implement change request
3. **Verify** — write golden model, cocotb tests, and SVA assertions
4. **Iterate** — run flow, kill surviving mutants, optimize PPA

## Start here

- **Operator+Agent process (source of truth)**: read `docs/joint-flow.md` FIRST — defines cycle, handoffs, file conventions, communication
- **Competition strategy**: read `docs/competition-strategy.md` (scoring priorities, timing, PPA targets)
- **Technical 4-phase workflow**: read `docs/flow.md`
- **Spawning agents**: read `docs/task-cards.md`
- **Design knowledge**: `docs/designs/` (see README for template; create per design)
- **Running the flow** (Docker, prompts, iteration): read `docs/running-the-flow.md`
- **Session state** (where we left off): `docs/state/`

## Quick commands (all via Docker)

Use `--cpus 12 --jobs 12` for all flow runs (12 cores available on this machine). Never use fewer.
```bash
# Public tests:
docker run --rm -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-1:/design --entrypoint bash proving-ground/flow:public \
  -c "cd /design/tb/public && make"

# Submission tests with assertions:
docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-1:/design --entrypoint bash proving-ground/flow:public \
  -c "cd /design/tests && make RTL_ROOT=/design"

# Fast flow iteration (~23s):
docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-1:/design:ro -v $PWD/runs:/runs \
  proving-ground/flow:public \
  run --design /design --submission /runs/sub.tar.gz --out /runs/out --jobs 12 --cache /runs/cache --skip ppa
```

Run the full scoring flow via Docker — see `docs/skills/flow-commands.md` for exact docker command.

## Score formula

```
score = 100 × C × (0.5V + 0.5P)
```
- C = hidden test pass rate (server only)
- V = mutation kill rate (40 mutants from 6 operator classes)
- P = PPA vs baseline (0.5 = same; timing fail caps at 0.3)

**Target**: C=1.0, V≥0.90, P≥0.60 → score >80. Baseline untouched ≈ 30.

## Competition logistics

- **3 designs** released sequentially: datapath (Nov 7), control+bus (Nov 10), pipelined core (Nov 13)
- **All open until Nov 16 23:59** — work on any design any time
- **30 submissions per design**, 1 per 30 min, ~15 min turnaround
- **Feedback**: C (failed hidden test names), V (surviving mutant locations), P (PPA per stage)
- **Priority per design**: C=1.0 → P>0.5 → V ceiling
- **Explorer award**: most distinct netlist variants with C>0.9

## Key insight

Baseline RTL has **seeded deviations** the public testbench does not catch. Find them by comparing spec requirements against RTL behavior.
