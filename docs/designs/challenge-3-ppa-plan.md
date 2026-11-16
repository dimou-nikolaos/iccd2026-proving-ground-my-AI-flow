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

| v7t | JALR speculative target adders (tgt_ex/wb/mem_q + sel1 mux) | 145939 | 3.538 | 17.978 | 0.4585 | **LOSS** (V=0.975): +3 adders area, WNS no better — redirect cone is NOT mux-depth-limited. Tier-C speculation ABANDONED. |

| v6b | v6a + MEM/WB data regs strip | 144323 | 3.775 | 17.754 | 0.4668 | LOSS (V=0.90). |
| v6a-rerun | muldiv-only strip (clean rerun) | 145192 | 2.341 | 18.852 | 0.4313 | LOSS — WORSE than full-strip v6 (!): reset-strip placement effects are pure lottery noise, NOT monotone. RESET-STRIP LINE CLOSED (all variants < 0.484). |

### Wildcards tried (flow-wildcard.md catalog closure, 2026-11-14)
Formal 4e trigger (C=1.0 banked) NOT met (server down); catalog swept opportunistically anyway:
- Aggressive pipelining / arithmetic (CSD, carry-save, DA, transposed) / storage (SRAM inference) —
  ANALYZED INAPPLICABLE: no constant coefficients; regfile is 32x32 (macro latency breaks the
  0-cycle-forwarding microarch pinned by hidden mcycle-delta tests); any pipeline-depth change
  breaks ISS lockstep + delta pins → C=0 by construction. Not worth measuring.
- Yosys attribute draws (local, zero-risk, queued q14): `woh` fsm_encoding=one_hot on muldiv FSM
  (412d2517), `wauto` fsm_encoding=auto (8ca7c583), `wkh` keep_hierarchy on u_alu instance (0fb6f249).
  MEASURED (q14, 12:47): woh P=0.484 and wauto P=0.484 with area/WNS/power IDENTICAL to the banked
  netlist (144312/4.469/17.117) => fsm_encoding is a NO-OP through this flow's synth for this FSM
  (netlist unchanged — not even a lottery draw). wkh (keep_hierarchy on u_alu) = flow REJECTED at
  build ("non-synthesizable or disallowed constructs") => instantiation-level attributes are NOT
  allowed by the flow lint whitelist. WILDCARD PHASE CLOSED: no remaining untried knob.
  LESSON (new): the flow rejects unknown RTL attributes on instances; only module/signal-level
  fsm_encoding survives lint, and it maps nothing useful here.
- ICG/clock-en: tried as v3 (0.4474 LOSS). Operand-isolation: skipped (muldiv regs already quiet
  between ops; div duty tiny => upside <1% power).

**PPA EXPLORATION CLOSED (2026-11-14 ~12:00):** 10 RTL variants + 5 text draws measured; winner remains
v1 retexture at **P=0.484** (theoretical ceiling ~0.496). Do not open new PPA fronts without a cone-aware
mechanism argument; the WB→fwd→branch→redirect→fetch cone dominates and every perturbation costs more
than it gains.

| v13 | C-hedge: rv_alu interface = released baseline, comparators in core | 144296 | 3.340 | 17.254 | 0.4613 | LOSS as P play (comparator placement/timing lottery); VALUE = C-hedge archive (c8b50c69), staged for use only if server C<1.0 implicates ALU-interface hidden checks |
| a1 | strip via structural split (muldiv ctrl/datapath blocks; WB split) | 144358 | 3.297 | 17.342 | 0.4599 | LOSS |
| a2 | v6 full strip + stmt reorders | 142239 | 3.125 | 14.286 | 0.4774 | = v6 NETLIST (stmt order invisible to synth!). Repowers proof: p −16% reproducible. Also a seed hedge (V=0.95) |
| a3 | v6 minus fetch strip | 142250 | 2.948 | 16.014 | 0.4639 | LOSS |
| a4 | v6 + rd-addr strips | 142104 | 2.620 | 14.425 | 0.4670 | LOSS (best power −16% yet WNS 2.62) |
| a5 | v6 + ex_dec_q reset strip | 142353 | 3.555 | 16.200 | 0.4745 | LOSS (V=1.0 roll though) |
| b1 | branch case-arm reorder | 144572 | 2.754 | 18.030 | 0.4450 | LOSS; distinct netlist → Explorer entry |
| b2 | hazard compare-arg reorder | 143204 | 3.022 | 17.137 | 0.4574 | LOSS; distinct → Explorer entry |
| b3 | target if-invert | 144632 | 3.133 | 17.374 | 0.4561 | LOSS; distinct → Explorer entry |
| b4 | decode field-order reorder | 144312 | 4.469 | 17.117 | 0.4840 | = drop-7 NETLIST (pure reorder invisible); useful seed hedge (V=0.95) |

**Statistical closure (2026-11-14 15:15):** 18 distinct measured netlists total; **0 ever reached ≥0.484**.
Reset-strip power/area mechanism is REAL and reproducible (a2/a4: power −16..17%, area −2100) but every
placement landing pays ≥1.3 ns WNS in the cone-dominated map — the mechanism can NEVER pay without a
timing win that never appeared in any draw (best strip-family WNS 3.56 vs 4.47 needed). Statement-order
edits are invisible to synth (a2/b4 = clone netlists). P=0.484 FINAL; further draws EV≈0.
### FINAL PASS ledger (operator notes v2 triage, 2026-11-14 18:00–20:10) — full detail in
`docs/state/challenge-3-final-ppa-pass-2026-11-14.md`

| variant | change | final area µm² | WNS | power mW | P | verdict |
|---|---|---|---|---|---|---|
| s1 | JALR target via ALU adder (src_b_imm=1, target={alu_result[31:1],1'b0}) | 143453 | 2.722 | 17.618 | 0.4485 | LOSS (area GOOD −859, WNS draw) |
| s2 | hold-enable strip MEM/WB/fetch data flops (pins re-aimed) | 145113 | 2.703 | **13.870** | 0.4674 | LOSS; **p=+0.194 best power ever**; V=0.95 |
| s4 | branch eq/ne via ALU SUB | 144390 | 1.937 | 17.479 | 0.4326 | DEAD (lengthens cone, as predicted) |
| s8 | valid-bits regfile (B1-alt) | **138660** | 3.217 | 21.009 | 0.4454 | best area −5.4k but **power backfire** (reset-less X-toggle) → B1-full DEAD |
| s12 | s2+s8 stack | 138754 | 3.229 | 15.567 | **0.4771** | **best non-golden ever**; a+t+p all improved, WNS gap only; V=0.925 |
| s12b | same netlist re-pack | 138754 | 3.229 | 15.567 | 0.4771 | P identical → PPA deterministic per netlist (seed rolls V only); V=0.975 |
| s13 | s12+s1 stack | 138938 | 2.663 | 15.465 | 0.4665 | LOSS; ABC ate the JALR dedup area |
| e1 | regfile AND-gated clock (E1 probe) | — | — | — | — | flow **REJECT at build** (derived clock) + 1 sim fail → E1 DEAD |

**Final statistical closure (20:10): 24 distinct netlists drawn, 0 ≥ 0.484. P=0.484 FINAL (draws closed).**

Explorer pool now: drop7v3(main), drop2, drop3r4, v6, v6a, v6b, v7t, v11, v13, lot1, lot2, lot3, lot5,
a1, a3, a4, a5, b1, b2, b3, s1, s2, s4, s8, s12, s12b, s13 = **26 candidate netlists** (submit during
cooldowns after C bank; budget 30). Tier-C (s-family, v11, v13) = C-unverified: after main C banks only.

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
