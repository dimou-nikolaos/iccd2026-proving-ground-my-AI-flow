# challenge-3 — progress (rv_core, RV32IM core)

## Phase status

| Phase | Status |
|-------|--------|
| 1 ANALYZE | in_progress (static analysis done by orchestrator; spec-analyst + probe-per-REQ sweep running) |
| 2 FIX | pending |
| 3 VERIFY | pending |
| 4 ITERATE | pending |

## Score history

| Sub | Time | C | V | P | Notes |
|-----|------|---|---|---|-------|
| (baseline run) | 2026-11-13 | null | null | pending | baseline PPA cache run in flight (challenge-3-baseline.tar.gz) |

## Deviations found

See table in `docs/designs/challenge-3.md` (D1..D5, static analysis). D1-D4 HIGH, D5 MEDIUM-HIGH. Probe sweep will confirm + may find more (expect 3-6 total, never stop at 3).

## CR implementation notes

- CR-D3 = REQ-20: SH1ADD/SH2ADD/SH3ADD (f7=0010000) + MIN/MINU/MAX/MAXU (f7=0000101), opcode 0x33. Sites: rv_decode f7 case, rv_pkg alu_op_t (new enum members), rv_alu (new arms; MIN/MAX reuse lt_signed/lt_unsigned). Acceptance values in change_request.md §Acceptance.

## V ceiling

- TBD.

## PPA

- Baseline run in flight: `runs/challenge-3-baseline.tar.gz` (rtl+design.yaml+tb+flow+report.md — NOTE: flow now REQUIRES report.md even for baseline run) → `runs/challenge-3-baseline-out`, cache `runs/challenge-3-cache`.
- Numbers pending; expect area/power-dominated game at 15 ns.

## Environment (updated 2026-11-13)

- **Docker access**: plain `docker` in agent shells = permission denied on docker.sock (session doesn't carry the `docker` group even though the account is a member). WORKING FORMS: `sudo -n docker ...` (as prep/README says) and `scripts/pgdocker.sh ...` (wraps `sg docker -c`). Use one of these for EVERY docker call. Verified: Verilator 5.048, Yosys 0.68 in image.
- Flow rejects archives without `report.md` ("submission is missing: report.md") — even baseline PPA runs. Baseline tar now includes a stub report.md.

## Activity log (append-only, newest at top)

#### 2026-11-13 evening — bootstrap
- Did: verified challenge-3/ == design-3.zip (diff clean); created baseline/ mirror; git force-added released+baseline content (commit 1e3b1b4); read spec+CR+all RTL+public TB; wrote design doc.
- Found: D1..D5 deviation candidates (design doc table); docker group quirk (fix: sudo -n / pgdocker.sh); flow requires report.md in every archive.
- Next: launch baseline PPA cache run (relaunched with report.md stub); prep kit self-tests (rviss host, rvcore docker); spawn spec-analyst confirm + probe-per-REQ sweep; then Phase 2 fix + CR + first drop.

## Operator feedback log

- 2026-11-13: release day go instruction; docker sudo hint (confirmed working).

## Next action for operator

- Nothing yet. Agent is working Phase 1. First drop (runs/sub.tar.gz, rtl+report) expected within ~2 h — will ping for upload.

## Best submission

- none yet.
