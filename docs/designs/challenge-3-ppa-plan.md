# challenge-3 — PPA plan + family ledger

Baseline final (authoritative, cache design-3-4dd373a87a3a): area 136024.0 µm², WNS +4.691 ns,
power 17.2019 mW. T_base = 15 − 4.691 = 10.309 ns achievable period.

**P = 0.5 + 0.5·(0.4·a + 0.4·t + 0.2·p)**, a/t/p = relative deltas vs baseline (area, period,
power). Timing is 40 % of the delta even though slack is huge — WNS improvement pays.

Measurement setup: `/tmp/c3-des-pristine` mounted as `/design` (L64!), submission archive per
variant, `--cache /runs/challenge-3-cache` (baseline side cached, 867 s per run), `--keep-work`
for path analysis. Results under `runs/out-c3-<v>-ppa2/`.

## Ledger (one run per netlist; ORFS noise ±0.5 ns WNS / ±0.002 P, L63)

| variant | change | final area µm² | WNS | power mW | P | verdict |
|---|---|---|---|---|---|---|
| ref (drop-2 RTL) | none | 144220 | 3.495 | 17.489 | **0.4631** | CR cost: a −0.060, t −0.116, p −0.017 |
| v1 | rv_alu exports lt_s/lt_u; core branch arms reuse ALU comparators (kills 4 duplicate 32-bit compares incl. 2 signed-adder cones) | 144312 | 4.469 | 17.117 | **0.4840** | **WIN +0.021** (timing recovery) |
| v2 | rv_regfile per-register write-enable genvar loop (DFFEN) | 142861 | 3.830 | 17.685 | 0.4704 | +0.007 vs ref but < v1; area −1450 µm², timing/power worse — do NOT bank alone |
| v3 | rv_muldiv clock-enable `(state_q != S_IDLE \|\| start)` | — | — | — | — | running (q5) |
| v4 | v1 + fold SH1/2/3ADD into main adder via a_pre operand mux (drops 3 dedicated 32-bit adders) | — | — | — | — | running (q5) |

All variants passed the golden audit (full 85-test suite at branch time + ASSERTIONS=1, 0 fires)
before queueing.

## Critical-path findings (v1 tree, `6_final.odb`, openroad report_checks)

- Worst setup cone (re-run slack 7.35): `wb_rd_q/bit → forwarding compares → fwd muxes →
  branch/target cone → redirect → u_fetch.id_pc/id_instr D`. The WB→forward→redirect→fetch
  loop is the design's critical cone, not the ALU internal op-mux.
- v1 helps by removing the ALU op-mux + signed adders from that cone (branch_taken no longer
  depends on alu_result path).
- Reset-net recovery paths (rst_n → regfile RESET_B, slack 10.3) and WB junk paths are noise.
- re-run max-slack on 6_final.odb (7.35) ≠ flow finish WNS (4.469): flow's 6_report.json
  includes finish-stage changes (antenna fill etc.). Compare variants only via flow P numbers.

## Tier decisions

- Tier A (free, stack onto drop-3 RTL): v1 (confirmed). v4 pending (stacks on v1; ALU-only).
  v3 pending (muldiv power; independent cone).
- v2: bank only if it stacks net-positive with v1 (separate regfile run not scheduled yet;
  its area gain competes with its timing loss — regfile write-en changes regfile read mux
  loading, likely WNS variance. Low priority.)
- Not tried: fsm_encoding attr (L54 — flow ignores), operand ISO on muldiv (iterative
  sequencer = c1 loss class), 64→32-bit muldiv acc rewrite (deep surgery, high V risk —
  wildcard only).

## After banking: mutation consequence

Stacking RTL changes re-rolls the mutation seed → fresh survivor hunt on drop-3 (Phase 4c
loop, ~1 reroll per kill-test iteration; V-ceiling notes in progress.md).
