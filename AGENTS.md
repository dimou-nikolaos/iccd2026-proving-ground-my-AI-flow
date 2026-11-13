# Proving Ground ICCD 2026

SystemVerilog RTL design verification challenge. Each released design ships a seeded-deviation baseline RTL, spec, change request, and public cocotb testbench. See `README.md` for the operator quick-start (what to copy where, starting prompt).

## Resume recipe (compaction-safe cold start)

If you're a fresh agent (or returning after context compaction) and don't know where work left off:

0. If no design is released yet (pre-release prep period), read `docs/state/prep-plan.md` — it
   holds the prep roadmap for challenge-2/3, open checkboxes, and verified environment facts.
1. `ls docs/state/` — find the current design's progress file (e.g., `challenge-2-progress.md`).
   If a `*-longhorizon-plan.md` exists for the current design (server-down / long-window
   mode, e.g. challenge-2), read that FIRST — it carries the phase tracker and next action.
   If a `*-session-checkpoint-*.md` exists, read the newest right after — it pins the bank
   md5 and the locked submission playbook (compaction survival).
2. Read the file's **"Next action for operator"** section and the **score history** table.
3. `md5sum runs/sub-best.tar.gz` — confirm it matches the md5 in the state file. If they diverge, trust `runs/sub-best.tar.gz` and update state.
4. Read `docs/designs/<current>-req-matrix.md` (REQ coverage status) and `docs/designs/<current>-ppa-plan.md` (what PPA variants exist).
5. Now you can take an action.

If there is no state file for the current design, you're starting that design from scratch — read `docs/design-init.md` for the bootstrap checklist (copy baseline, init docs from templates, cache baseline PPA), then read the **"Start here"** list below.

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

**Logging protocol (non-negotiable): LOG-BEFORE-NEXT-ACTION — update `docs/state/<design>-progress.md` + commit after every meaningful step, and write a dated session-checkpoint before compaction. Full protocol: `docs/joint-flow.md` §Continuous logging.**

1. **Analyze** — find spec deviations via static analysis, formal, and probes
2. **Fix** — correct RTL deviations + implement change request
3. **Verify** — write golden model, cocotb tests, SVA assertions, AND complete `docs/designs/<name>-req-matrix.md` (every REQ has ≥1 test and ≥1 assertion). Exit gate: REQ matrix complete + ≥3 tests fail on baseline + 0 assertions fire on fixed RTL.
4. **Iterate** — Tier A (free) → Tier B (C-check 1 submission) → Tier C (C-check 1-2 submissions). See `docs/flow-ppa-and-v.md` for tier definitions and per-design submission budget. After Tier C is exhausted and C=1.0 is banked, **Phase 4e (wildcard exploration)** is available during cooldown time. See `docs/flow-wildcard.md`.

## Start here

- **Operator+Agent process (source of truth)**: read `docs/joint-flow.md` FIRST — defines cycle, handoffs, file conventions, communication
- **Competition strategy**: read `docs/competition-strategy.md` (scoring priorities, timing, PPA tier budget)
- **PPA freedom audit + tiers**: read `docs/ppa-freedom-strategy.md` before Phase 4b (which spec-silences can be exploited, tier classification, C-check rules)
- **Technical 4-phase workflow**: read `docs/flow.md` (skeleton + exit gates). For Phase 4 detail, see `docs/flow-ppa-and-v.md`. For prior pitfalls, see `docs/flow-lessons.md`.
- **Spawning agents**: read `docs/task-cards.md`
- **Design knowledge**: `docs/designs/` (see README for template; create per design — including the REQ matrix file). For upcoming designs, read pre-drafted playbooks: `docs/designs/challenge-2-playbook.md` (control+bus, predicted), `docs/designs/challenge-3-playbook.md` (pipelined core, predicted).
- **Running the flow** (Docker, prompts, iteration): read `docs/running-the-flow.md`
- **Session state** (where we left off): `docs/state/`

### What to read for what (phase routing)

| You are at | Read first | Then |
|---|---|---|
| Cold start / resume after compaction | `docs/state/<design>-progress.md` + `md5sum runs/sub-best.tar.gz` | `docs/joint-flow.md` (cycle + handoff) |
| Starting a new design | `docs/design-init.md` (bootstrap checklist) | `docs/designs/<design>-playbook.md` + prior challenge's `*-flow-log.md`, `*-tool-inventory.md`, `*-ppa-plan.md` |
| Phase 1 (analyze spec deviations) | `docs/flow.md` § Phase 1 | skill `spec-deviation-hunting`, `docs/skills/bug-hunting.md` |
| Phase 2 (fix RTL) | `docs/flow.md` § Phase 2 | task-card `rtl-fixer` |
| Phase 3 (write tests + assertions) | `docs/flow.md` § Phase 3 | skills `cocotb-test-writing`, `sva-assertion-writing`; deep refs `docs/skills/cocotb-patterns.md`, `docs/skills/sva-patterns.md` |
| Phase 3 formal SBY (combinational modules) | skill `formal-verification-flow` | `docs/flow-lessons.md` L13, L24-L26 (gotchas before you start) |
| Phase 4a (server C feedback / submission) | `docs/joint-flow.md` § cycle | `docs/competition-strategy.md` (sub budget) |
| Phase 4b (PPA Tier A/B/C) | `docs/flow-ppa-and-v.md` § PPA | skill `sky130hd-ppa`, `docs/ppa-freedom-strategy.md`, `docs/skills/sky130hd-optimization.md` |
| Phase 4c (mutation kill) | `docs/flow-ppa-and-v.md` § Mutation | skill `mutation-kill-tactics`, `docs/skills/mutation-strategies.md` |
| Phase 4e (wildcard exploration) | `docs/flow-wildcard.md` | `docs/flow-lessons.md` (P section, L17/L20/L27/L28 wildcard catalogs) |
| Running the scoring flow | skill `pg-flow-commands` | `scripts/pg-batch.sh` for parallel batches; `docs/skills/flow-internals.md` for what each stage checks and how to iterate V locally |
| PPA failure diagnosis | `docs/flow-lessons.md` L11/L14/L16/L21 (synthesis + counter-intuition) | `scripts/{submodule-synth,critical-path}.sh` |
| Rebuild any historical submission | `docs/designs/<design>-sub-catalog.md` (parent chain + V/P/status) | `docs/designs/<design>-sub-diffs.md` (per-sub diff vs parent) |
| Review our workflow for a new design | `docs/designs/<design>-flow-log.md` | `docs/designs/<design>-tool-inventory.md` |

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

Baseline RTL has **seeded deviations** the public testbench does not catch. Measured: **4–5 per design (expect 3–6; never stop at 3)**. Find them by comparing spec requirements against RTL behavior; the probe-per-REQ sweep (flow-lessons L1) is the completeness gate.
