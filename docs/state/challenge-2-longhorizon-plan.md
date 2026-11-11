# Challenge-2 long-horizon plan — server-DOWN mode (created Nov 11 ~05:00)

**Server status**: DOWN. No C feedback available. We work exactly like challenge-1's
12h/10-day solo pushes: build candidate archives, score them through the LOCAL full
pipeline, keep `runs/sub-best.tar.gz`, and be ready to submit the moment the server
returns (30 slots, 1/30min — all unused slots remain available until Nov 16 23:59 HKT).

## Resume recipe (fresh agent after compaction)

1. Read **Next action** at the bottom of this file, then the phase tracker.
2. `md5sum runs/sub-best.tar.gz` (if present) and compare with
   `docs/state/challenge-2-progress.md` → Best submission. Trust the tar.
3. `ls runs/` — archive naming: `c2-sub-<n>.tar.gz` (numbered like challenge-1),
   `runs/c2-out-<n>/` their local flow outputs. `runs/c2-best.md` = score table.
4. Read `docs/designs/challenge-2.md` + `challenge-2-req-matrix.md` for design facts.
5. All commands: Docker `proving-ground/flow:public`, `sudo -n docker`, `--cpus 12 --jobs 12`.
   Fast iterate: `run ... --skip ppa` (~1-2 min on this design); full: ~5-10 min.

## Current state snapshot (Nov 11, update on every phase change)

- Workspace: `challenge-2/` (released bundle, 7 RTL files, 777 lines, top `dma_ctrl`,
  4-channel + shared engine + APB4/AXI4-Lite, 36 REQs + CR-D2). `challenge-2/baseline/` pristine.
- Public tb: 8/8 PASS on baseline in 0.14 s — very lenient (no equal-prio arbitration,
  no write-response errors, no LEN=0, no abort, no W1C races).
- Baseline PPA cache run: `runs/challenge-2-baseline-out/` (in flight at plan time;
  if absent, rerun per `design-init.sh` output; check
  `runs/challenge-2-baseline-out/stages/ppa.json` for area/WNS/power).
- Docs: progress file, req-matrix (36 REQ rows), this plan.
- Prep kits ready to vendor: `prep/bus_vip/`, `prep/dma_vip/tests/*` (skeleton bodies +
  Regmap), `prep/sva/`, `scripts/{mutant-report,pg-batch,explorer-check,explorer-variants}.sh|py`.
- Lessons L1–L44 apply (esp. L41 probe families, L42/L43 dead-code & VIP-knob V tax,
  L31–L35 wildcard/keep/fsm/probe results from C1).

## Deviations found (static pass, Nov 11 — pending probe confirmation)

| ID | Location | Spec | RTL does | Pattern | Conf. |
|----|----------|------|----------|---------|-------|
| D1 | `dma_arb.sv:22` `prio[cand] >= best_prio` | REQ-24: FIRST eligible in rotating order wins among equal PRIO | `>=` lets LATER equal-prio channel overwrite → last wins | comparison flip | HIGH |
| D2 | `dma_engine.sv` S_IDLE grant: no `len==0` case | REQ-18: LEN=0 ends at once when granted, moves NO data, makes NO data bus access, CH_LEN stays 0 | performs a full read+write of one word; LEN wraps 0→0xFFFF | missing case | HIGH |
| D3 | `dma_engine.sv:207-223` `m_bresp` never inspected (input unused) | REQ-28/29/30: any non-OKAY `m_bresp` stops channel, sets ERR, no DONE; CH_SRC/DST/LEN keep values | every bresp treated OKAY: word counted (`word_done` pulses at :93), addresses advance, LEN decrements, chain continues | unused input | HIGH |
| D4 | `dma_engine.sv:149-152` S_STOP asserts `ev_done` | REQ-31: abort → DONE and ERR NOT set (unless last word itself failed) | abort path sets CH_STAT.DONE + IRQ_STATUS.DONE | wrong event set | HIGH |
| D5 | `dma_engine.sv:136-138` dack at S_END **and** :140-144 at S_RELEASE | REQ-27: one dack pulse after a burst that moved ≥1 word ends | two pulses per chain-descriptor end for HW_REQ chains (S_END + S_RELEASE) | extra pulse | MED — decide after probe |
| D6 | abort not checked outside S_WR_RESP: a channel aborted at descriptor-end still fetches 4 descriptor words, EN clears only after S_RELEASE | REQ-31 "no further word starts" (descriptor fetch = bus access?) | fetch completes, then kill | edge case | LOW-MED — likely not seeded |

**Fix sketches** (Phase 2, cheapest correct form):
- D1: `>=` → `>`.
- D2: in S_IDLE grant: `else if (ch_len[grant_idx] == 16'd0) state_q <= S_END;` (moved_q
  stays 0 → no dack for zero-length bursts, consistent REQ-27; CH_LEN untouched).
- D3: S_WR_RESP: on `m_bresp != RESP_OKAY` → S_ERR; `word_end`/`moved_q` gated on OKAY.
- D4: S_STOP: `stop` only, drop `ev_done` (last-word-fail case already routes via S_ERR).
- D5: S_END dack only in the non-continuing branch (else S_RELEASE pulses).
- CR-D2: engine `ev_desc` output = own_mask at every S_END entry (covers programmed block,
  zero-length (with D2 fix), chain-mid and chain-last; fetch-fail leaves DESC set from the
  S_END cycle + ERR from S_ERR, DONE never — matches acceptance bullets); `dma_irq` extends
  set_vec to `{20'h0, ev_desc, ev_err, ev_done}`, `dma_pkg::IRQ_MASK` → `32'h0000_0FFF`.

## Phase tracker

| Phase | Goal | Budget | Status | Findings |
|-------|------|--------|--------|----------|
| 0 bootstrap | design-init, public smoke, baseline PPA cache | 30 min | **done except PPA cache in flight** | 36 REQs; public 8/8; tar bug fixed |
| 1 ANALYZE | confirm D1–D4 (+decide D5/D6) with live probes on baseline RTL | 1–2 h | **static pass done, probes pending** | table above; L1: 4 seeded ≈ matches "few" in README |
| 2 FIX | apply D1–D4 + CR-D2 (+D5 if decided); lint/elab/public green | 1 h | pending | |
| 3 VERIFY | vendor VIPs + env; per-REQ test suite (36+REQ-36); SVA binds; req-matrix complete; ≥3 tests fail on baseline; 0 fires on fixed | 4–6 h | pending | reuse `prep/dma_vip/tests/` bodies |
| 4a local score | full local flow → V, P; iterate V kill rounds via `scripts/mutant-report.py` | 2–4 h | pending | |
| 4b PPA | Tier A (reset-strip, keep, fsm_encoding) → Tier B (measured) per ppa doc; batch via pg-batch | 4–8 h | pending | baseline PPA numbers needed first |
| 4c Explorer | `explorer-variants.sh` GREEN queue + fingerprint dedup | continuous | pending | L44 class order |
| 5 SUBMIT | the moment server is up: submit sub-best immediately, then follow C feedback per budget in `docs/state/release-strategy.md` §3 | — | BLOCKED on server | |

## Local iteration protocol (server-down substitute for submissions)

- Archive per iteration: `runs/c2-sub-<n>.tar.gz` = `rtl/ tests/ assertions/ report.md`
  (flow layout — see `docs/skills/flow-internals.md` §1; `tests/` = vendored VIPs + our
  tests on pythonpath).
- Score locally with the same pipeline: `run --design challenge-2/baseline
  --submission ... --cache /runs/challenge-2-cache` (full = C-proxy via our own tests +
  V + real PPA). `status: scored` + numbers into `runs/c2-best.md`.
- **C-risk while server is down** is handled by: req-matrix completeness (every REQ ≥1
  test + ≥1 assertion), probe tests per deviation (must FAIL on baseline, PASS on fixed),
  formal SBY on the small modules (dma_apb/dma_arb/dma_irq — combinational/tiny state;
  C1 precedent says sim-assertions acceptable if SBY slang frontend fights us), and
  reading the spec twice for the 4th+ deviation (L1).
- V: expect fixture-style ceiling ~0.85-1.0; kill by class (L41/L42); keep a V=1.0 pool
  for rerolls (L22).
- NEVER submit before local full pipeline = scored (server-up doctrine unchanged).

## When the server comes up

1. Submit the banked sub-best archive immediately (slot 1) — record md5 in progress file.
2. Expect C feedback names → fix queue. Budget: 1 core + ≤3 C-fixes + 2–3 PPA + 1–2 V +
   14–16 Explorer + ≥6 reserve (release-strategy §3).
3. Explorer GREEN queue runs during every cooldown slot from `runs/explorer-green.txt`.

## Activity log (append-only, newest at top)

### Nov 11 ~05:00 / bootstrap + static Phase-1 pass
- Did: design-init.sh (36 REQs, baseline mirror, doc skeletons); public tests 8/8 PASS
  0.14 s; relaunched baseline PPA cache flow (first attempt died: tar was never created —
  harness background-arg bug, watch for that); read spec/CR/all RTL; deviations D1–D6 table.
- Found: public tb blind spots exactly at D1–D4 (no equal-prio, no write_err, no LEN=0,
  no abort) — consistent with 4 seeded deviations there. CR-D2 DESC event maps to every
  S_END entry after D2 fix; acceptance bullet 3 (fetch-fail sets DESC+ERR) falls out free.
- Next: confirm D1–D4 with quick targeted probe tests run against BASELINE (expect fail),
  then Phase 2 fixes.

## Next action (single step)

Write `tests_probe/test_d_probe.py` (4 tiny probes: equal-prio grant order, LEN=0
grant, bresp SLVERR, abort DONE check) and run them against the BASELINE RTL via
RTL_ROOT-style override — confirm all 4 FAIL on baseline (proves deviations + seeds the
evidence suite). Then mark Phase 1 done and start Phase 2 fixes.
