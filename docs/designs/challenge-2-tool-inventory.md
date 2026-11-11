# challenge-2 tool inventory

Common infrastructure is identical to challenge-1 — read `challenge-1-tool-inventory.md`
for the Docker invocation patterns, `pg-batch.sh`, `critical-path.sh`, `submodule-synth.sh`
usage (all reused here unchanged). This file records only challenge-2 deltas.

## Sub-agents used
rtl-fixer (×2: fixes + CR), spec-analyst, probe-creator, test-writer, assertion-writer,
mutant-hunter (survivor forensics → equiv-site classification), formal-writer (pre-release
stage-SVA drills only), ppa-optimizer (W-family builds), general (variant trees),
explore — same roster as c1; ppa-optimizer usage was heavier here (W6/W8/W9 campaign).

## Custom scripts (new in challenge-2, all in `scripts/`)
- `scripts/pg-batch.sh` — reused; NEVER launch via `(cmd &)` subshell (SIGHUP orphans +
  half-written `runs/out-*` poison retries; launch as plain background command).
- `scripts/mutant-report.py` — parse flow report survivors (operator-class buckets,
  equivalent-vs-real classification).
- `scripts/explorer-check.sh` — distinct-netlist counting across pool (P/area/WNS tuples
  from reports).
- `scripts/gen-sub-diffs-c2.py` — generates `challenge-2-sub-diffs.md` (family trees vs
  fixed RTL; baseline→fixed section included). Rerolls listed, not diffed.
- `scripts/gen-sub-catalog-c2.py` — generates `challenge-2-sub-catalog.md` (archives ↔
  report.json via submission_sha256; 44/51 matched).

## Test infrastructure (`challenge-2/tests/`, 7 modules, 58 tests)
- Golden model in Python mirrors APB/AXI/DMA semantics incl. outstanding engine; drivers
  parameterized by REQ tag.
- `test_soak.py`: 52 randomized rounds, golden-oracle checked — catches D1/D2/D5+CR on baseline.
- `test_rotation_ch2_mut.py` style rotation cases in `test_dma_reqs.py` (REQ-24 killer).
- `tests/Makefile`: `RTL_ROOT=<dir>` runs the suite on any variant tree
  (VERILOG_SOURCES from `<dir>/rtl/filelist.f`).
- `challenge-2/neg/` (NEVER SHIP): per-assertion negative fixtures; each must fire.

### Local tests+GOLDEN-assertions audit recipe (L60 — the bank gate)
```bash
VS=$( (sed 's|^rtl/|/design/runs/variants/<V>/|' runs/variants/<V>/filelist.f; \
      ls challenge-2/assertions/*.sv | sed 's|^challenge-2/|/design/challenge-2/|') | tr '\n' ' ')
docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp -v $PWD:/design \
  --entrypoint bash proving-ground/flow:public -c \
  "cd /design/challenge-2/tests && make SIM_BUILD=/design/runs/sb-<V> \
   RTL_ROOT=/design/runs/variants/<V> VERILOG_SOURCES='$VS'"
```
Any family needing MODIFIED goldens = contract-change → Explorer/late-slot only.

## Assertion infrastructure (`challenge-2/assertions/`, 5 bind files, 1264 lines)
- `dma_ctrl.sv` top binds: g_chan[i].u_chan (chan_irq_sva), engine, apb, axi, irq.
- Next-state goldens (`a_len_q_next`, `a_src_q_next`, …) model family-A microarch — they
  double as microarch-contract detectors (caught W8/W6; L60).
- Verilator caveats: 2-state (`=== 1'bx` never true, L59); bind-signal scope = module instance
  paths must match released hierarchy.

## Synthesis/PPA observations (tool-level)
- `(* keep *)`, `(* fsm_encoding = "one_hot" *)` attrs are INERT on this design's synth
  path (byte-identical netlists, L54) — always prove netlist delta (explorer-check) before
  rolls. (Contrast: c1 got real wins from these — design-dependent.)
- Final-route WNS pinned ~2.5–2.7 in every family (L58): OpenROAD/effort1 plateau; don't
  chase timing, chase area/cell-count (W8 shared-dp = the only real lever found, −9.2%).
- `runs/cache` (flow `--cache`) makes re-runs with unchanged RTL ~3s for PPA stage — pool
  rerolls are cheap.

## State-doc structure (per design, in `docs/state/`)
Same as c1: `<design>-progress.md` (phase table + newest-first logs + bank pointer),
`<design>-longhorizon-plan.md` (v-numbered NEXT STEPS), session checkpoint (this campaign:
`challenge-2-session-checkpoint-2026-11-12.md`). Archive pool truth: `runs/c2-best.md`
(gitignored working copy; snapshot appended to `challenge-2-sub-catalog.md`).

## Summary statistics
47 flow runs, ~51 archives, ~20 distinct netlists, 5 deviations fixed, V=1.0 bank,
P=0.5184 (baseline 0.50), local proj 75.92 @C=1.0.
