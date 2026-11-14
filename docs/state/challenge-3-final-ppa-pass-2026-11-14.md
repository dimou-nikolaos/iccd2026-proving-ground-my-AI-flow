# challenge-3 — FINAL PPA PASS (operator notes v2 triage, 2026-11-14 ~15:30)

Operator provided a v2 ideas document (saved summary below). Mandate: final local pass, 6 hours
autonomous, all prior work stays (golden `runs/sub.tar.gz` drop7v3 `092ca181e03abce1647667c1895e5d32`,
V=1.0+P=0.484, untouched). This file = the plan; tick the tracker; commit after EVERY step
(LOG-BEFORE-NEXT-ACTION). Newest checkpoint file is authoritative on restart.

## Measured ground truths that CORRECT the notes (do not re-litigate)
1. P formula (measured via report details): **P = 0.5 + 0.5·(0.4a + 0.4t + 0.2p)**;
   a = −Δarea/136024 (area vs baseline), t from achievable-period/WNS (ours WNS 4.469, base 4.691),
   p vectorless power (v6 strip proved p can reach +0.17). All three matter; timing is NOT a
   mere WNS≥0 gate.
2. Baseline measured = 136024 µm² / WNS 4.691 / 17.202 mW. Our live = 144312 / 4.469 / 17.117.
3. 18 distinct netlists measured; ALL < 0.484. The WNS lottery is asymmetric and punishes any
   perturbation (strip family: power/area wins reproduced but WNS 2.3–3.6 every time).
   ⇒ Only changes with STRUCTURAL area savings AND off-cone or cone-neutral logic changes get tried;
   every candidate must plausibly not-lengthen the WB→fwd→branch/target→redirect→fetch cone.
4. The cone is NOT mux-depth limited (v7t experiment: speculative parallel adders LOST).
5. Cycle-count-changing ideas are KILLED BY OUR OWN SUITE (proxy for hidden): deterministic pins
   FIRST_MINSTRET=0, FIRST_MCYCLE=4, delta==4 (req33), delta==13, delta==5 (no-spurious-stall),
   delta==6 (store-base interlock). D2/D3 (2/3-stage), F1 (2×MDU), F3 (fetch-time JAL),
   F4 (branch prediction), A6 (started_q=const 1), B3 (regfile read in EX), E2 (single-port RF),
   E3 (serial shifter) all change retire timing/bubbles → would fail these pins → C=0 risk. NOT tried.
6. E5 (MCAUSE shrink) fails CSR write+read-back tests (ours and surely hidden). NOT tried.
7. Yosys attributes: instance-level = flow REJECT (wkh). fsm_encoding = no-op. Statement ORDER is
   invisible to synth (a2/b4 clones). Duplicate assigns on logic vars = synth FAIL. Appending to
   running bash queues does NOT execute — start fresh queue files. Never overwrite a tar while its
   flow run is in flight.

## §L dispute (hidden-check model)
Notes claim hidden binds use ports only. Our established rule (from release docs + prior challenges):
hidden tests may poke baseline internal names. Consequence: ideas that DELETE baseline internal
signals (op_a_q, ex_dec_q fields, pend_pc_q, id_pc_q, started_q) = Tier C: build+audit locally,
stage as C-unverified archives, upload ONLY after drop7v3 C=1.0 banked, one per sub, drop7v3 is the
fallback. Downside bounded; P upside real. Keep top ports + filelist byte-identical ALWAYS.

## Ranked candidate list (build order; all keep schedule + ports)
| ID | Idea (from notes) | Est. save | Cone risk | Tier | Verdict basis |
|---|---|---|---|---|---|
| s1 | **C4 JALR target via ALU adder**: `target = {alu_result[31:1],1'b0}` when is_jalr (JALR already executes ADD rs1+imm through u_alu; decode check FIRST: is dec.alu_op==ALU_ADD && src_a==RS1 && src_b_imm for JALR?) | ~660 µm² | NEUTRAL by construction (ALU already computes it; removal is pure de-dup) | B | P vs 0.484 |
| s2 | **B6 hold-enable deletion**: MEM-stage data fields + WB data fields load EVERY cycle (values provably stable during mem_stall because EX is frozen; WB consumers gated by wb_valid_q). Removes ~110 muxes | ~1.1–1.4 k | off-cone (mux removal on flop D-inputs; alu_result fanout reduced) | B (expr change, names kept) | P |
| ~~s3~~ REJECTED — C1-light premise FALSE: counterexample: two older producers of same rd before MDU instr; at start+2 the mdu_busy fwd-hold `ex_rs1_q <= fwd_rs1` recaptures the OLDER producer's value while MDU still needs the nearer one (op_a_q NOT dead). Only op_q removable (~20µm²) → skip |: drop op_a_q/op_b_q/op_q (operands+opcode frozen while mdu_busy — EX not re-latched; verify with SVA `mdu_busy |=> $stable(ex_rs*_q)` and ISS random MDU-dense seeds). Feed rv_muldiv from ex_rs*_q + ex_dec_q.mdu_op directly | ~1.7–2 k | off-cone (muldiv result has 30+ cycles slack) | C (deletes baseline regs) | P |
| s4 | **branch-eq via ALU SUB**: decode branches with alu_op=ALU_SUB; `branch_taken(eq) = (alu_result==32'd0)`; kills the separate 32-bit eq comparator (v1 already shares lt comparators; v10 misfired because op wasn't SUB — fix IS the decode change) | ~600–900 µm² | POSSIBLY negative (branch path now through SUB carry) — watch WNS | B | P; abandon if WNS drops |
| s5 | **A4 PC bits [31:2]**: pc_q/pend_pc_q/id_pc_q/ex_pc_q store [31:2] (bit0=0 by construction; bit1: target-misalign traps before fetch redirect can put bit1 into pc_q — verify via probe/SVA), narrow adders | ~0.5–0.8 k | low | B | P |
| s6 | **B5 pend_pc_q → id_instr_q**: while pend_q=1, id_valid=0 so id_instr_q/id_pc_q are dead; store target there, mux at id_pc output | ~0.9 k | low, off-cone | C (deletes baseline reg) | P |
| s7 | **B4 id_pc = pc_q−4 invariant** (delete id_pc_q, add 30-bit dec + mux) | ~0.6 k net | medium-low | C (deletes baseline reg) | P |
| s8 | **B1-alt valid bits**: regfile 31 valid-FFs + AND-gated read; CSR per-register written-bits folded into read-mux selects; data flops lose reset | ~4 k + reset-tree | med (read path +AND, ID path slack OK) | C | P |
| s9 | **B1 full reset sequencer** (counter walks regfile+CSR through write ports, imem_valid=0 until done, mcycle reset AFTER clearing so FIRST_MCYCLE=4 pin survives) — only if s8 wins | ~5.7 k | med-high (hidden reset-pulse timing) | C | P |
| s10 | **E1 lint probe ONLY** (package regfile row clock-gating idea, run flow `--skip ppa`, see if build stage rejects; cost ~1 min; do NOT invest further unless accepted + sim-able) | (10 k moonshot) | high | — | accept/reject gate |
| s11 | **B2 pipeline instr, re-decode in EX** (shrink ex_dec_q flop set ~75 flops; ex_instr_q already pipelined) | ~2.7 k | med (re-decode parallel, off fwd path; deletes struct fields) | C | P |

Explicitly NOT tried (evidence above): D2/D3/F1/F3/F4/A6/B3/E2/E3/E5/A1 (alt-bugfix form), C1-full
(MDU on ALU adder — adds operand muxes INTO the cone), C3 shifter merge (effort > payoff in time
left; cone-neutral but ABC often already merges right-shifters — revisit only if time remains),
C5/C6 micro-gains, B1 without prior s8 win.

## Protocol per candidate (L14 + our rules)
build tree /tmp/c3-ppa-sN → diff vs live rtl (MUST differ — lesson from q13 misfire) →
95/95 tests ASSERTIONS=1 + 0 fires (update OUR rv_core_defs pins if they pin an expression we
intentionally changed; never delete a pin, re-aim it; V-killing ability must be preserved) →
public TB 12/12 → repack runs/sub-c3-sN.tar.gz (md5 logged) → full PPA: pristine mount
/tmp/c3-des-pristine:ro + --cache /runs/challenge-3-cache + baseline side == 136024.0/4.69101
check → compare P vs 0.484 with per-term breakdown (which of a/t/p moved). Two parallel queues max
(--cpus 6 each, fresh script files, ~27 min/run).

## Consolidation plan (after individual P numbers, ~1h left)
- STACK all P-plausible winners (s1+s2+s3+... by file-disjointness) → s9x tree → audit → package
  drop-8 candidate. Pool: drop8a + 3 report-comment hedges → V rolls. If drop-8 V≥ drop7v3's known
  1.0 (can't exceed) AND P>0.484 → new golden pointer; else keep drop7v3 golden and keep drop8 as
  staged Tier-C explorer/uplift archive.
- Server-return playbook unchanged: sub1 = drop7v3 → C bank → then per cooldown: drop-8 (stacked
  P-uplift, C-check), then explorer singles.

## Tracker (tick + commit each row)
| s# | built | audit | PPA P | decision |
|---|---|---|---|---|
| s1 | DONE 18:13 95/95 (assert clean); JALR src_b_imm=1 + target={alu_result[31:1],1'b0} | DONE | qS running (archive 56efc312) | pending P |
| s2 | DONE; 95/95; needed re-aiming A_WBE/WBRD/WBDATA_NXT pins to unconditional-load defs (s2 tree only) | DONE | qS (6cf72e7b) | pending P; NOTE clean-build lesson: first clean-build ASSERTIONS=1 aborts sim on pin fire (cascades 93 fails) — check assertion msgs not counts |
| ~~s3~~ | rejected above | — | — | — |
| s4 | DONE 95/95; branch alu_op=SUB + eq/ne from alu_result | DONE | qS (1be2ed47) | pending P |
| s5 | ☐ | ☐ | ☐ | ☐ |
| s6 | ☐ | ☐ | ☐ | ☐ |
| s7 | ☐ | ☐ | ☐ | ☐ |
| s8 | ☐ | ☐ | ☐ | ☐ |
| s10 lint probe | DONE 18:18 | e1 coarse AND-gated regfile clock: 94/95 (1 reset-window fail) + **flow REJECTED at build** (derived clock disallowed) → E1 DEAD, both sim-unsafe and lint-illegal | — | closed |
| s11 | ☐ | ☐ | ☐ | ☐ |
| stack+pool+docs | ☐ | ☐ | ☐ | ☐ |

## Restart instructions (compaction)
1. `md5sum runs/sub.tar.gz` == 092ca181… (golden, do not touch) else git-log until pinned.
2. `ls docs/state/ | grep checkpoint` → newest 11-14 file, then THIS file's tracker.
3. `tail -5 runs/ppa-queue.log` for queue progress; P numbers in runs/out-c3-s*-ppa3/stages/ppa.json
   (verify baseline side 136024.0 first).
4. Resume first un-ticked tracker row above.
