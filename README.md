# Proving Ground ICCD 2026 — Workspace

This repo holds our agent tooling and process docs only:

- `docs/` — how we work: `joint-flow.md` (the operator↔agent cycle, read first), `flow.md` (4-phase method), `joint-strategy.md`, `competition-strategy.md`, `task-cards.md`, `running-the-flow.md`, `report-template.md`; plus `skills/`, `agents/`, `designs/`, `state/`, `sessions/`.
- `.opencode/skills/` + `.opencode/agents/` — the load-on-demand skills and subagents the orchestrator spawns.
- `runs/` — where the packaged submission and flow outputs land (gitignored).

The organizers' materials are **not** committed here — you copy them in when they release, exactly where the docs expect them:

## Copy these in, and where

When the scoring kit is released (before/at Nov 7):

| From organizers | Copy to | Why (what references it) |
|---|---|---|
| `SUBMISSION.md` | repo root (`./SUBMISSION.md`) | submission format + what the backend checks; referenced by `docs/flow.md` |
| `flow-image/` (its `README.md`, `SCORING.md`, `docker/Dockerfile.public`, scoring details) | repo root, as `./flow-image/` | referenced by `docs/flow.md` and `docs/skills/flow-commands.md` (C/V/P formulas, build args, SVA subset) |
| Prebuilt flow image tarball, e.g. `proving-ground-flow-public-*.tar.gz` | anywhere, then `docker load -i <tarball>` | all runs use `proving-ground/flow:public`; host Verilator is too old |

When each design is released (Nov 7 / 10 / 13), copy the whole design bundle to the repo root as `challenge-<N>/`, keeping its internal layout:

```
challenge-<N>/
  spec.md             requirements (REQ-xx) — the source of truth
  change_request.md   the CR to implement
  design.yaml         top module, clock period, flow config
  rtl/                problem code: baseline RTL + rtl/filelist.f  (do NOT change ports/filelist/module names)
  tb/public/          public cocotb testbench (does NOT catch the seeded deviations)
  flow/constraint.sdc clock constraint (do NOT change)
  README.md           design-specific notes (leave in place)
```

Do not rename inside the bundle. Do not delete `tb/public/`, `flow/constraint.sdc`, or `rtl/filelist.f` — the flow checks them. The agent creates the matching `docs/designs/challenge-<N>.md` and `docs/state/challenge-<N>-progress.md` on its own.

Sanity check the tools after copying (one Docker smoke test):

```bash
docker images proving-ground/flow:public
docker run --rm -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-<N>:/design --entrypoint bash proving-ground/flow:public \
  -c "cd /design/tb/public && make"
```

## Starting prompt (paste to the agent)

```
Work on challenge-<N> (dir: challenge-<N>/). It was just released.
Follow docs/joint-flow.md (the cycle) and docs/flow.md (the 4 phases).

Rules:
- Save a pristine copy of the released rtl/ before editing, so we can validate our tests fail on the baseline.
- All tools via Docker proving-ground/flow:public, 12 cores. Never run verilator/yosys/sby on the host.
- First deliverables: docs/designs/challenge-<N>.md and docs/state/challenge-<N>-progress.md.
- First drop: fix the seeded deviations + implement the CR + report.md + rtl only → package runs/sub.tar.gz, then STOP for me to upload.
- Keep docs/state/challenge-<N>-progress.md updated every step.
- Spawn subagents per docs/task-cards.md. Priority: C=1.0, then P>0.5, then the V ceiling.
```

Then: you upload `runs/sub.tar.gz`, paste the flow report back (C, V, P, failed hidden-test names, surviving mutants, PPA stages), and the agent iterates one change per submission (see `docs/joint-flow.md`).

## Score

`score = 100 × C × (0.5·V + 0.5·P)` — C hidden checks (server only), V mutants killed by your tests/assertions, P PPA on sky130hd vs baseline (0.5 = parity, timing-fail caps at 0.3). 30 submissions/design, 1 per 30 min.
