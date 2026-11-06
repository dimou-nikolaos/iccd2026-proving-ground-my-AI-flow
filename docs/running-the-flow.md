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
