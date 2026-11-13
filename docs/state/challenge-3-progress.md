# challenge-3 — progress (rv_core, RV32IM core)

### Phase status

| Phase | Status |
|-------|--------|
| 1 ANALYZE | done (probe-per-REQ gate passed; 4 deviations confirmed; D5 false alarm) |
| 2 FIX | done pending server C (lint/elab/public-TB green; drop 1 packaged) |
| 3 VERIFY | pending (ISS lockstep + tests/ + assertions/ + REQ matrix fill during cooldown) |
| 4 ITERATE | pending |

## Score history

| Sub | Time | C | V | P | Notes |
|-----|------|---|---|---|-------|
| (baseline ref) | 2026-11-13 | null | 0.0 | 0.5 | baseline-vs-baseline parity run; PPA banked to cache; final: area 136024.0 um2, WNS +4.691 ns, power 17.20 mW, ~12.1k cells synth |
| drop 1 (pending upload) | 2026-11-13 | ? | ~0 (no tests yet) | ? | rtl+report.md only: D1-D4 fixed + CR-D3 |

## Deviations found

CONFIRMED 4 (D1-D4) — table + probe evidence in `docs/designs/challenge-3.md`. D5 false alarm (schedule analysis + wait-state probes clean). Probe gate: 44 probes, 44/44 PASS fixed, 7 FAIL baseline. Expect 3-6; random ISS differential (Phase 3) is the 5th-deviation backstop.

## CR implementation notes

- CR-D3 = REQ-20: SH1ADD/SH2ADD/SH3ADD (f7=0010000) + MIN/MINU/MAX/MAXU (f7=0000101), opcode 0x33. Sites: rv_decode f7 case, rv_pkg alu_op_t (new enum members), rv_alu (new arms; MIN/MAX reuse lt_signed/lt_unsigned). Acceptance values in change_request.md §Acceptance.

## V ceiling

- TBD.

## PPA

- Baseline (CACHED, runs/challenge-3-cache): **area 62097.405 um2, WNS +2.743 ns** (@15 ns → slack +2.74, timing not binding), total power 9728.181 uW. Cells ~5880, depth 30 (yosys synth). P is an AREA/POWER game (L58-style decision: area-first batches, watch WNS cliff).
- Prior flow attempt died mid-route (OpenROAD nondeterministic ODB class); `baseline` subcommand run clean (~14 min).
- Optimizations applied: none yet.

## Environment (updated 2026-11-13)

- **Docker access**: plain `docker` in agent shells = permission denied on docker.sock (session doesn't carry the `docker` group even though the account is a member). WORKING FORMS: `sudo -n docker ...` (as prep/README says) and `scripts/pgdocker.sh ...` (wraps `sg docker -c`). Use one of these for EVERY docker call. Verified: Verilator 5.048, Yosys 0.68 in image.
- Flow rejects archives without `report.md` ("submission is missing: report.md") — even baseline PPA runs. Baseline tar now includes a stub report.md.

## Activity log (append-only, newest at top)

#### 2026-11-13 late — probe-per-REQ sweep gate PASSED
- Did: @probe-creator wrote challenge-3/probes/ (44 tests, 1 build). Ran vs baseline AND fixed. Orchestrator packaged drop 1 (challenge-3/report.md + rtl), local --skip ppa sanity next.
- Found: 44/44 PASS on fixed; 7 FAIL on baseline (D1-D4 + req25 probe + 2 CR probes). D5 CONFIRMED FALSE ALARM (muldiv-dense waits clean on baseline). No new deviations from 38 directed REQ probes. Baseline full flow completed (route 1529 s!): area 136024 um2, WNS +4.691, power 17.2 mW — corrected earlier stale numbers.
- Next: local --skip ppa on drop-1 archive → operator uploads; then Phase 3 (ISS lockstep harness + tests/ + assertions/ + REQ matrix fill) during cooldown.

#### 2026-11-13 late — Phase 2 first pass (rtl-fixer) landed
- Did: @rtl-fixer applied D1 (hazard x0 guards, 4 arms), D2 (mpie<=1 on mret), D3 (lh sign bit half[15]), D4 (op_a_q reg + div0 rem returns raw dividend) + CR-D3 (7 enum members APPENDED after ALU_AND, decode f7=0010000/0000101 arms, 7 alu arms). Orchestrator reviewed diff: exactly scoped, filelist byte-identical.
- Gates: verilator lint 0 warnings, yosys elab clean, public TB 12/12 PASS on fixed RTL, CR acceptance probes 7/7.
- Found: D5 RE-ANALYSIS says race UNREACHABLE by schedule: mdu_start gated by !mem_stall, mdu_busy=1 from the moment muldiv-in-EX is in EX (done=0), so MEM drains and stays empty during muldiv run → mdu_done && mem_stall never co-assert. Keeping D5 as NO-DEVIATION (false alarm) but EMPIRICAL check stays: ISS random-prog runs with random wait patterns must show no hang.
- Next: @probe-creator writes deviation tests D1-D4 + probe-per-REQ sweep (must FAIL on baseline, PASS on fixed); ISS lockstep harness adapt; then package drop 1 (rtl+report.md) after probes confirm D1-D4 fail on baseline.
- Baseline PPA cache: full flow died mid-route (ODB nondeterministic, logs complete to 50% DRC both nets); relaunched `baseline` subcommand (bg).

#### 2026-11-13 evening — Phase 1 boost-off
- Did: ISS kit self-test green (29/29, host python); rvcore SVA kit self-test green in Docker (TESTS=3 PASS=3, 0 fires; negative 9/9 fired). Kits trustworthy → release work = adapter work.
- Found: flow rejects archives missing report.md (also for baseline runs).
- Next: spawned @rtl-fixer (D1-D4 + CR-D3, parallel) and @spec-analyst (independent re-derivation + REQ→line map). Probe sweep (ISS lockstep + directed) after fixer lands, against BOTH baseline and fixed RTL.

#### 2026-11-13 evening — bootstrap

#### 2026-11-13 evening — bootstrap
- Did: verified challenge-3/ == design-3.zip (diff clean); created baseline/ mirror; git force-added released+baseline content (commit 1e3b1b4); read spec+CR+all RTL+public TB; wrote design doc.
- Found: D1..D5 deviation candidates (design doc table); docker group quirk (fix: sudo -n / pgdocker.sh); flow requires report.md in every archive.
- Next: launch baseline PPA cache run (relaunched with report.md stub); prep kit self-tests (rviss host, rvcore docker); spawn spec-analyst confirm + probe-per-REQ sweep; then Phase 2 fix + CR + first drop.

## Operator feedback log

- 2026-11-13: release day go instruction; docker sudo hint (confirmed working).

## Next action for operator

- **Upload: runs/sub.tar.gz (drop 1)** — rtl + report.md, D1-D4 fixed + CR-D3. Expect C feedback (all probes + public TB green locally; C is server-only truth). Local V will show ~0 (no tests in archive yet — intentional), P should be ≥0.5-ish from CR logic added vs baseline (unknown until server/local full flow).
- Then: paste flow report back (C, failed test names, P terms per stage). Agent meanwhile works Phase 3 (ISS lockstep, tests/, assertions/, REQ matrix).

## Best submission

- none banked yet (drop 1 awaiting C).
