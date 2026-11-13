# challenge-2 session checkpoint (2026-11-12, pre-compaction)

## Resume recipe (compaction-safe)
1. `md5sum runs/sub-best.tar.gz` — must be `42dd5c5e2c4d0f9e46a188465dca7502` (= c2-sub-32, abar2 bank; CLEAN-REPACK tar of 2026-11-13, L62 — legit content byte-identical to the old `d3d0381f…` tar, junk stripped; re-scored end-to-end green in out-repack-c2-sub-32).
   If different, trust the tar and fix this file + progress file.
2. Read `docs/state/challenge-2-longhorizon-plan.md` NEXT STEPS **v8** (bank decision + submission order).
3. `docs/state/challenge-2-progress.md` → top activity log entry ("W8 DEMOTED …").
4. Pool truth table: `runs/c2-best.md` (gitignored; snapshot in `docs/designs/challenge-2-sub-catalog.md`).
5. Workspace `challenge-2/rtl/` = family A RTL (NOT the abar2 bank RTL!). The bank RTL lives in
   `runs/variants/abar2/*.sv` (rebuild recipe: progress log "W8 bank rebuild-verification" — same
   recipe works for abar2 pkg staging).

## State at end of session
- **Bank: c2-sub-32** (abar2 family; V=1.0, P=0.5184, proj 75.92@C=1.0). Spares: c2-sub-33/-34
  (V=1.0 abar2 rerolls). `runs/sub-best.tar.gz` = c2-sub-32.
- **W8 (P=0.5212, −9.2% area) DEMOTED** — L60 microarch-contract violation proven by unmodified
  goldens audit. W8/W9/W6/W10/R1/f1/k1 = Explorer/late-slot class. w10 (V=1.0, P=0.5165) scored.
- Pool: ~51 archives, ~20 distinct netlists. All local (server DOWN whole campaign; 30 slots unused).
- Golden-audit gate active: any family not green vs UNMODIFIED goldens = contract-change label.
- Phases 1–3 DONE (D1–D5 fixed, CR-D2 in, 58-test suite, 1264-line SVA, REQ matrix 100%).
- P campaign CLOSED (P≈0.52 route-side ceiling, L58). V ledger: abar2 3/4 clean seeds.
- Challenge-3 prep complete (ISS, assembler, randprog, stage SVA, flowcheck); only port wiring awaits release.

## Submission playbook when server opens (v8, LOCKED)
1. Slot 1: `runs/c2-sub-32.tar.gz` (safe; goldens-silent). If C=1.0 → bank ~75.9.
2. If C<1.0: failed-test names → diagnose; slot 2/3 = c2-sub-33/-34 (same netlist, V reroll) to
   separate V-noise from C-real; then bisect family (c2-sub-26 a2 family).
3. Slot 4+: `runs/c2-sub-w8.tar.gz` PROBE (distinct netlist, P +0.003, C-risk known).
   If C=1.0 → W8-line becomes bank candidate (re-promote via later slots). If C<1.0 → confirms
   hidden probes internal semantics; stick with abar2; document which tests failed (gold for c3).
4. Remaining slots: Explorer breadth (distinct netlists from runs/c2-best.md queue:
   w6, w9, w10, r1, w1, w2b, w3, e05/e06-era extras, keep0-2, arb2, a2/-26, a3/arb3/abar3,
   f1/k1 reroll spares). Goal: most distinct netlists with C>0.9.
5. Do NOT ship packs whose assertions/ differ from family-A goldens as FIRST probe of a class —
   that's fine for Explorer (they self-certify) but never for the bank.

## Compaction-survival plan
- This file + progress file + longhorizon plan v8 + sub-catalog = complete cold-start set
  (AGENTS.md resume recipe already routes to them; this checkpoint is the pinned-state snapshot).
- After compaction: (a) verify bank md5 (above), (b) `git log --oneline -5` on `challenge-2`
  branch — HEAD must be ≥ the "W8 DEMOTED" commit (b3bbe14+), (c) check server availability,
  (d) execute playbook step 1, (e) do NOT re-run closed P campaigns (L58 ceiling; ledger in ppa-plan).
- Operator owed a sync: W8 demotion rationale (L60), bank change w8→32, w10 scored, merge-to-main
  archive done (this commit). Operator pushes branches; never push yourself.
- If Nov 13 arrives before server opens: switch to challenge-3 bootstrap (`docs/design-init.md`
  checklist + `challenge-3-playbook.md` "what challenge-2 adds" section), keep c2 bank frozen.
