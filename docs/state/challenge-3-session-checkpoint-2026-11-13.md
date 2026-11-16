# challenge-3 session checkpoint — 2026-11-13 (pre-drop-1)

## Pinned bank / archive identity

- runs/sub.tar.gz = DROP 1 (rtl/ fixed + report.md only), md5 `a35e0a5b13ead94b2cfae825c01c3e5e` — NOT banked (no runs/sub-best.tar.gz yet; C unknown until server).
- Baseline PPA banked in cache: runs/challenge-3-cache/design-3-4dd373a87a3a/baseline_ppa.json — final area 136024.0 um2, WNS +4.691 ns, power 17.20 mW; baseline-vs-baseline run gave P=0.5 parity (cache healthy).
- Pristine baseline mirror: challenge-3/baseline/ (rtl md5s listed in docs/designs/challenge-3.md).

## State snapshot

- Phase 1 DONE: 4 seeded deviations confirmed (D1 hazard x0-forwarding REQ-11; D2 mret MPIE REQ-41; D3 lh@+2 sign ext REQ-25; D4 rem-div0 REQ-30). D5 = FALSE ALARM (mdu race unreachable by schedule + probes clean).
- Phase 2 fixes + CR-D3 applied in challenge-3/rtl/ (commit 176f8e0; filelist byte-identical; lint/elab clean; public TB 12/12; probes 44/44 fixed, 7 fail baseline).
- challenge-3/probes/ = 44 directed tests (Phase 1 gate). These become the seed of tests/ in Phase 3.
- Local drop-1 flow (--skip ppa): status scored, no reject. C is SERVER-ONLY.

## Submission playbook (locked)

1. Drop 1 = C check (rtl+report). Budget: subs 1-3 for C.
2. Drop 2 = full evidence: tests/ (ISS lockstep w/ prep/rviss + probes corpus) + assertions/ (prep/rvcore rv_stage_sva bind map + interface asserts) → V measurement + golden-assert bank gate (L60).
3. Then PPA: 15 ns design, WNS +4.69 → area/power game. Prior levers: see design ppa-plan (to write). NEVER stack two area-only wins (L4). Attr wildcards: prove netlist delta first (L54).
4. V seed rerolls on any archive byte change (L5): keep 3-4 archives per netlist.

## Resume recipe

1. md5sum runs/sub.tar.gz vs a35e0a5b13ead94b2cfae825c01c3e5e (drop 1).
2. Read docs/state/challenge-3-progress.md: Next action + score history.
3. If operator pasted server report → update score history, act on C/failed-test-names.
4. Else continue Phase 3: (a) adapt prep/rviss ISS config (misalign policy=fault, mtval=instr on illegal, RO write=trap, CSR table per spec §8, ext-irq priority, no MRET shadow per REQ-37); (b) cocotb lockstep harness from prep/rvcore/test_stage.py wired to rv_tb protocol; (c) assertions/ bind map for rv_stage_sva (HAZARD_MODE=0 first, verify 0 fires on baseline+fixed); (d) docs/designs/challenge-3-req-matrix.md (REQ-05/42 → lint/synth rows).

## Environment

- Docker: `sudo -n docker` or `$PWD/scripts/pgdocker.sh` (plain docker = permission denied in agent shells). 12 cpus, --jobs 12.
- Flow rejects archives without report.md (even baseline runs).
- Full PPA flow on this design: route ~25 min (big design) — budget run times accordingly; prefer --skip ppa for iteration.

## Compaction-survival checklist

- [ ] progress file current (Activity log top entry = what changed)
- [ ] sub.tar.gz md5 above re-verified on resume
- [ ] once first C result in: score history row + operator feedback log updated, sub-best banked if C>0 with md5
