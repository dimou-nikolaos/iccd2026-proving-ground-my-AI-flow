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
| v1 | rv_alu exports lt_s/lt_u; core branch arms reuse ALU comparators (kills 4 duplicate 32-bit compares incl. 2 signed-adder cones) | 144312 | 4.469 | 17.117 | **0.4840** | **WIN +0.021** (timing recovery); reproduced byte-identical in ppa2b re-run |
| v2 | rv_regfile per-register write-enable genvar loop (DFFEN) | 142861 | 3.830 | 17.685 | 0.4704 | +0.007 vs ref but < v1; do NOT bank |
| v3 | rv_muldiv clock-enable `(state_q != S_IDLE \|\| start)` | 142349 | 2.981 | 18.935 | 0.4474 | **LOSS** — power WORSE (ICG + wire detour), WNS −1.5. Discard |
| v4 | v1 + fold SH1/2/3ADD into main adder via a_pre operand mux | 140226 | 2.807 | 17.001 | 0.4584 | area −4000 µm² but +1 mux depth in the critical WB→redirect cone → WNS −1.7. Discard |
| v5 | v1 + v2 stack | 144739 | 3.220 | 16.993 | 0.4599 | **LOSS** stacked — regfile en even INCREASED area over v1 here; discard |
| lot1 | text probe: dup `assign shamt` in rv_alu | 145864 | 3.049 | 18.226 | 0.4477 | LOSS (V=0.95) |
| lot2 | text probe: decode branch-illegal expr `!f3[2]&&f3[1]` | 142814 | 3.760 | 17.471 | 0.4704 | LOSS (area −1.5k but timing −0.7) |
| lot3 | text probe: `ex_fire = valid && !(stall||busy)` | 145090 | 2.857 | 18.022 | 0.4463 | LOSS |

| lot4 | text probe: dup `assign alu_b` | — | — | — | FAIL | yosys SYNTH FAILURE: duplicate continuous assign to `logic` var passes Verilator sim but is illegal in synth. Never dup-assign. |
| lot5 | text probe: dup `assign ex_fire` | 143857 | 3.295 | 17.681 | 0.4586 | LOSS (V=0.925) (yosys accepted THIS one — not a reliable guard) |
| v6 | reset-strip all datapath flops | 142239 | 3.125 | 14.286 | 0.4774 | net LOSS vs 0.484 but AREA −2073 + POWER −17% (!); WNS hit −1.34. Isolating off-cone subset as v6a. V=0.95. |

**Lottery conclusion (2026-11-14 10:07):** 3/3 RTL-text draws LOST (P 0.446–0.470 vs banked 0.484);
the current v1 text is a good anchor — do NOT text-lottery core control/cone signals. The knob is
asymmetric: cone text variations mostly hurt (WNS 2.86–3.76 vs 4.47). lot4/lot5 (operand/fire dup
probes) remain as data points only.

**Key dynamic: this design is timing-dominated** — the WB→forward-compare→fwd-mux→branch/
target→redirect→fetch cone sets WNS; ANY depth added inside ALU-operand/forward path costs
~0.03–0.06 P (40 % weight). Area wins that don't touch that cone are worth ~0.01–0.03.
ORFS reproduction: same submission archive + same design tree reproduced identical
area/WNS/power/score across two separate full runs (out-c3-v1-ppa2 vs ppa2b) — and again on drop5a
full run (144312 / 4.46935 / 17.117 bit-identical). ORFS is DETERMINISTIC per RTL text; the earlier
0.4625 sample (drop3r4) came from a *cleaned rv_alu source text* (single lt_s assigns) that
synthesized a worse netlist. Lesson: P differences between logically-identical RTL texts are REAL;
freeze the measured v1 text byte-identically (L63 refined).

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
