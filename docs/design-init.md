# Design Initialization Checklist

Run this at the start of each new design (challenge-2, challenge-3). ~15 min, ensures all per-design artifacts exist with the right shape before spawning sub-agents.

## 0. Read prior challenge retro docs (5-10 min — don't skip)

For each completed challenge, scan the retro docs to pick up proven patterns and dead-ends:

- `docs/designs/<completed-design>-flow-log.md` — what worked chronologically; what we'd do differently
- `docs/designs/<completed-design>-tool-inventory.md` — which agents/skills/scripts to invoke per phase
- `docs/designs/<completed-design>-ppa-plan.md` — measured ΔP per wildcard class; PPA dead-ends not to repeat
- `docs/designs/<completed-design>-sub-catalog.md` — patterns of successful sub chains; how the winning stack was built up

For challenge-1 specifically, the highest-ROI findings to carry over:
- `(* fsm_encoding = "one_hot" *)` is a strong P lever on small FSMs (L31)
- `(* keep = "true" *)` cross-module stacks compound (L29)
- Pipeline stage splits at natural boundaries are the BIGGEST P lever (challenge-1 +0.060 single-shot)
- Spec-compliance probe tests before committing wildcards (L35)
- Critical-path mux restructure on Yosys-decoded readback is dead-end (L30)
- Operand ISO on adders/comparators loses (L17)

## 1. Copy baseline RTL into workspace

```bash
cp -r /path/to/released/challenge-N challenge-N
ls challenge-N/{rtl,tb,spec.md,change_request.md,design.yaml,flow/}  # verify present
mkdir -p challenge-N/baseline  # pristine mirror for PPA caching (see L14 in flow-lessons)
cp -r challenge-N/{rtl,tb,flow,design.yaml} challenge-N/baseline/
```

The `challenge-N/baseline/` mirror lets `pg-flow-commands` cache the baseline PPA without re-elaborating each time. Without it, every PPA run is fixed-vs-fixed (≈0.5, meaningless).

**Force-add the released + baseline content to git (L36):** `.gitignore` has the broad rule `challenge-[0-9]*/` which silently blocks everything inside the design dir. Remember to explicitly `git add -f`:

```bash
git add -f challenge-N/rtl/ challenge-N/spec.md challenge-N/change_request.md \
  challenge-N/design.yaml challenge-N/flow/constraint.sdc \
  challenge-N/tb/public/Makefile challenge-N/tb/public/dsp_tb.py challenge-N/tb/public/test_dsp_public.py \
  challenge-N/baseline/rtl/ challenge-N/baseline/flow/ \
  challenge-N/baseline/tb/public/Makefile challenge-N/baseline/tb/public/dsp_tb.py challenge-N/baseline/tb/public/test_dsp_public.py \
  challenge-N/baseline/design.yaml
git commit -m "Bootstrap challenge-N: released RTL + spec + CR + operator public testbench"
```

As you produce them later in the campaign, also force-add: `challenge-N/tests/`, `challenge-N/assertions/`, `challenge-N/formal/*.sby`, `challenge-N/formal/*_sva.sv`. Stay ignored: `sim_build*/`, `__pycache__/`, `results.xml`, `challenge-N/formal/<run>/` (SBY run artifacts, regenerable).

## 2. Initialize per-design docs from templates

```bash
# Design knowledge doc
cp docs/designs/TEMPLATE.md docs/designs/challenge-N.md
# (edit: top module, hierarchy, signal map, CR summary — populated as Phase 1 runs)

# REQ coverage matrix (start empty, fill in Phase 3)
grep -oE 'REQ-[0-9]+' challenge-N/spec.md | sort -u > /tmp/reqs.txt
wc -l /tmp/reqs.txt  # sanity: how many REQs
# Manually build docs/designs/challenge-N-req-matrix.md with one row per REQ

# PPA plan (start with baseline reference, fill as Phase 4 runs)
# Use challenge-1-ppa-plan.md as the shape reference
```

If `docs/designs/TEMPLATE.md` doesn't exist, copy `docs/designs/README.md`'s template section into the file.

## 3. Initialize state file

```bash
cp docs/state/TEMPLATE.md docs/state/challenge-N-progress.md
# Edit header: design name, phase status (all "pending"), "Next action for operator: copy dir + start Phase 1"
```

## 4. Run baseline PPA (cache it)

```bash
# This produces the baseline area/WNS/power that all subsequent P scores reference.
cd challenge-N
tar czf ../runs/challenge-N-baseline.tar.gz rtl design.yaml tb flow
cd ..

docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-N/baseline:/design:ro \
  -v $PWD/runs:/runs \
  proving-ground/flow:public \
  run --design /design --submission /runs/challenge-N-baseline.tar.gz \
      --out /runs/challenge-N-baseline-out --jobs 12 --cache /runs/challenge-N-cache
```

This is the **slow step (~6-10 min)**, but it populates `/runs/challenge-N-cache/baseline_ppa.json` so all subsequent PPA runs reuse it. Without this, every submission's P is meaningless.

## 5. Record baseline numbers in state and PPA plan

From `runs/challenge-N-baseline-out/stages/ppa.json`:
- Baseline area, WNS, power, cell count
- Write into `docs/state/challenge-N-progress.md` "PPA" section
- Write into `docs/designs/challenge-N-ppa-plan.md` "Baseline" section

## 6. Run probe-per-REQ sweep prep (Phase 1 closing)

After Phase 1a (spec-analyst output), the spec-analyst's "Step 6" output gives the REQ → implementing-line map. The orchestrator hands this to `@probe-creator` in **probe-per-REQ sweep mode** (see `docs/agents/probe-creator.md` end section).

## 7. Set "Next action for operator" in state file

Update `docs/state/challenge-N-progress.md`:
```
## Next action for operator
- Agent is starting Phase 1a (static analysis via @spec-analyst).
- After Phase 1a: orchestrator will spawn @probe-creator for probe-per-REQ sweep.
- After Phase 2 fix: orchestrator will submit RTL-only first drop for C check.
```

## Done. Phase 1 begins.

## Anti-patterns

- Skipping step 1's `baseline/` mirror → every PPA run is fixed-vs-fixed. Wastes 5+ min per submission.
- Skipping step 4 (baseline PPA cache) → first Tier A PPA run takes 10 min instead of 3 min, AND baseline numbers aren't recorded for comparison.
- Editing the per-design docs ad-hoc without the templates → cross-references break, future agent can't find sections.
- Not initializing the REQ matrix early → Phase 3 has no structure, agent invents format, future re-audit is impossible.
