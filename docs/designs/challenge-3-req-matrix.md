# challenge-3 — REQ coverage matrix

One row per requirement (REQ-01..17, REQ-20..42; 18/19 do not exist).
Exit gate: every REQ has >= 1 test and >= 1 assertion (REQ-05/42 = structural,
enforced by the flow build/P stages).
Status values: `todo`, `test`, `assert`, `done`, `n/a` (with reason).

Tests live in `challenge-3/tests/`: `test_rv_submission.py` (72 directed
cocotb tests) + `test_rv_random.py` (13 ISS-lockstep differential tests:
`test_rv_rand_n101/103/107/113/127` stall-free, `test_rv_rand_n201/211/223/227/233`
with 30% imem/dmem waits, `test_rv_rand_t301/302/303` trap-mode seeds — all
generated with `include_cr=True` or `trap_config`). Suite:
**85/85 PASS on fixed RTL (~3.5 s); 22 FAIL on `baseline/rtl`** = seeded
deviations D1–D4 (tests prefixed `test_dev_*`), REQ-25 second-leg, and all
CR-D3 tests (baseline lacks CR). Harnesses `rv_core_env.py`/`randprog.py`
vendored into `tests/` (flow archive strips `tb/`).

Assertions in `challenge-3/assertions/`: `rv_top_if.sv` (8 port-protocol props),
`rv_core_defs.sv` (27 signal-def/next-state pins bound into rv_core),
`rv_csr_defs.sv` (3), `rv_muldiv_defs.sv` (2), plus 7 active observer props from
vendored `rv_stage_sva.sv` (ID/EX contract; mapped via `rv_core_stage_bind.sv`;
mem_*/wb_*/if_* props disabled — real core carries no instr word past EX).
Total **47 props: 0 firings on fixed RTL; on baseline they fire and identify the
deviations**: D1→`A_FWD_RS1`, D2→`A_CSR_MPIE`, D3→`A_LOAD_DATA`,
D4→`A_MDU_REM0`, CR-not-implemented→`A_CRD3_LEGAL`.

| REQ | Summary | RTL location | Test | Assertion | Status |
|---|---|---|---|---|---|
| REQ-01 | reset anytime; all regs/CSRs/pipeline = 0 | all always_ff (rv_fetch.sv:31–38, rv_csr.sv:97–101, rv_core pipeline regs) | req01_reset_zero; req01_reset_mid_operation; req01_reset_consecutive; req01_counters_reset_zero; req36_csr_reset_values | A_RST_DMEM_FIRST (first cycle post-reset); $past def identities hold across re-reset | done |
| REQ-02 | after reset dmem_valid=0; first fetch from 0 | rv_fetch.sv:25,36–38 (started_q, pc_q≤0) | req02_first_fetch | A_FIRST_FETCH_ZERO; A_RST_DMEM_FIRST | done |
| REQ-03 | imem_valid/addr hold while ready=0 | rv_fetch.sv:25–26 (imem_valid=started_q, addr=pc_q; pc advances only on ready :52) | req03_04_08_hold; rand n201–n233 (30% istalls) | A_IMEM_HOLD (rv_top_if) | done |
| REQ-04 | dmem fields (incl. wdata for store) hold while ready=0 | rv_lsu.sv:25–44 from frozen mem_*_q; rv_core.sv:79, rv_core_defs A_MEMV_NXT hold | req03_04_08_hold; req10_same_under_wait_states | A_DMEM_HOLD; A_DMEM_WDATA_HOLD; A_MEMV_NXT | done |
| REQ-05 | outputs not comb. dependent on ready/rdata | imem from pc_q/started_q regs; dmem_* from mem_*_q regs (rv_lsu.sv:25–44) | structural — flow build stage: verilator lint 0 warnings + yosys synthesis (no comb path from inputs to outputs) | same (lint/synth structural check) | n/a (structural) |
| REQ-06 | exactly one data access per executed load/store; none for trapped/wrong-path | rv_lsu.sv:25; rv_core.sv:335 (mem_valid_q ≤ ex_fire && !trap) | req06_access_count; req22_misaligned_load_store (no access leg); req13_wrong_path_no_effect | A_MEMV_NXT; A_TRAP (trap kills the access) | done |
| REQ-07 | load wstrb=0; store wstrb selects addressed bytes; SB/SH data in lanes | rv_lsu.sv:30–44 | req07_wstrb | A_LOAD_WSTRB (load side); store lanes cycle-precise in test (A_DMEM_HOLD stability) | done |
| REQ-08 | addr[1:0]=0 whenever valid | rv_lsu.sv:27; rv_fetch.sv:26 (pc always word-aligned) | req03_04_08_hold | A_IMEM_ALIGN; A_DMEM_ALIGN | done |
| REQ-09 | all RV32I; FENCE/FENCE.I/WFI = no-ops | rv_decode.sv:160–203 (fence/wfi → no-op fields); rv_alu/rv_core datapath | req09_fence_wfi_noop; rand n*/t* (full RV32I lockstep) | A_DEC_FIELDS; A_EX_ALU (observer ISA ref) | done |
| REQ-10 | program order; results wait-state independent | rv_core.sv:79–82 (mem_stall/mdu_busy/stall_front pipeline freeze) | req10_same_under_wait_states; rand group A vs group B (same seeds, 0% vs 30% waits) | A_IMEM_HOLD; A_DMEM_HOLD; A_ID_TO_EX | done |
| REQ-11 | x0 always reads 0 (incl. right after rd=x0 producer) | rv_regfile.sv (x0 guard + write-through); rv_hazard.sv:31–45 match; **D1 fixed** (rv_hazard.sv:30,40 missing guard) | dev_d1_x0_forward (FAILS on baseline); req11_x0_reads_zero | A_FWD_RS1/RS2 (defs include rs≠0 guard → catch D1) | done |
| REQ-12 | written value available to next instr (forward/stall, load-use) | rv_hazard.sv:30–49 (EX/MEM > MEM/WB forwarding, load_use interlock) | req12_forward_chain; req13_load_use_dependent_load; req13_load_then_dependent_store; req13_csr_consumer_load_interlock; req13_no_stall_on_unused_rs1_field; cr4_forwarding_matrix | A_FWD_RS1; A_FWD_RS2; A_LOAD_USE | done |
| REQ-13 | instrs fetched after taken branch/jump have no effect | rv_core redirect → rv_fetch.sv:44–53 (pend/redirect, squash); ID→EX bubble (rv_core.sv) | req13_wrong_path_no_effect; req30_flushed_csr_instr_no_write (flush suppresses CSR write) | A_FLOW_CHANGE; A_REDIRECT; A_ID_TO_EX (bubble on redirect) | done |
| REQ-14 | JALR clears bit 0 of target | rv_core.sv:195–196 (`& ~1`) | req14_jalr_clears_bit0; req15_jalr_nonzero_imm | A_EX_TARGET_JALR (observer jalr target ref) | done |
| REQ-15 | branch/jump target[1]=1 → cause 0 trap when taken | rv_core.sv:203,233–235 (target_misaligned → exc_cause 0, tval=target) | req15_branch_misaligned; req15_jal_misaligned | A_TARGET_MISALIGNED (def; observer target props masked for that cycle) | done |
| REQ-16 | shift amount = low 5 bits of register operand | rv_alu.sv:13 (`shamt = b[4:0]`) | req16_shamt_uses_low_five | A_EX_ALU (observer alu_ref applies mask to real operands) | done |
| REQ-17 | SLTIU: unsigned compare vs sign-extended imm | rv_alu.sv:23 (lt_unsigned) + src_b_imm in decode | req17_sltiu_unsigned_imm; req17_bge_signed_adjacent (signed-compare contrast) | A_EX_ALU | done |
| REQ-20 | (CR-D3) SH1ADD/SH2ADD/SH3ADD + MIN/MINU/MAX/MAXU, opcode 0110011; wrap; forward like ALU; other funct3 illegal | rv_decode.sv:140–153 (funct7 0010000/0000101 cases); rv_alu.sv:29–35 | cr1_shadd; cr2_minmax; cr3_reserved_f3_illegal; cr4_forwarding_matrix; cr5_rd_rs_aliasing; rand n* (include_cr=True) — all FAIL on baseline | A_CRD3_LEGAL (forward-only legality pin → catches CR absent) | done (CR) |
| REQ-21 | LB/LH sign-extend; LBU/LHU zero-extend | rv_lsu.sv:63–65 (`{24{!is_unsigned && byte_val[7]}}` etc.) | req21_load_extension | A_LOAD_DATA (ld_ref golden extend model) | done |
| REQ-22 | LW/SW need 4-align, LH/LHU/SH 2-align; trap with NO access | rv_core.sv:209–213 (ls_misaligned), 237–238 (cause 4/6); trap suppresses access (rv_core.sv:335) | req22_misaligned_load_store (both legs incl. no-access check) | A_LS_MIS (def pins size-dependent alignment test) | done |
| REQ-23 | LB/LBU offset k → byte k of word | rv_lsu.sv:54–57 | req23_lb_offsets | A_LOAD_DATA | done |
| REQ-24 | LHU@2 → bits [31:16] | rv_lsu.sv:50 (`offset[1] ? [31:16] : [15:0]`), 64 | req24_lhu_offset2 | A_LOAD_DATA | done |
| REQ-25 | LH@0 from bit 15; LH@2 from bit 31 | rv_lsu.sv:50,64; **D3 fixed** (was: always replicated bit 15) | req25_lh_offset0; dev_d3_lh_offset2 (FAILS on baseline) | A_LOAD_DATA (ld_half sign term from bit 31 when offset[1]) | done |
| REQ-26 | MUL/MULH/MULHSU/MULHU bit results; MULHSU rs1 signed/rs2 unsigned | rv_muldiv.sv:35–38 (a_signed/b_signed per op), 54 (mul_out negate) | req26_mul_variants; rand n*/t* (m_rate 0.15, sign corners) | A_EX_RESULT (mdu_result mux pin); A_MDU_START (start def); results ISS-verified | done |
| REQ-27 | mul/div may take many cycles; rest of pipeline waits | rv_core.sv:80–81 (mdu_busy in stall_front); rv_fetch stall input | req27_muldiv_waits_pipeline; dev_d5_muldiv_during_load_stall; req10_27_muldiv_load_mix | A_MDU_BUSY; A_STALL_FRONT; A_EX_FIRE (defs of stall terms) | done |
| REQ-28 | DIV/REM round toward zero; REM sign = dividend | rv_muldiv.sv:22,90–91 (magnitude + res_neg/rem_neg=a_neg), 54–56 | req28_div_rem_round_to_zero; rand t301–t303 (divide-corner seeds) | A_EX_RESULT (result mux); rounding semantics ISS-verified in lockstep | done |
| REQ-29 | DIV(-2^31,-1)=-2^31; REM=0 | rv_muldiv.sv:36–38,90 (res_neg=(a_neg^b_neg)&&b≠0 → overflow path) | req29_div_int_min; rand t* divide corners | A_EX_RESULT (result mux); value ISS-verified | done |
| REQ-30 | div0: DIV=-1, DIVU=all-ones, REM/REMU=dividend | rv_muldiv.sv:56 (`rem_out = (op_b_q==0) ? op_a_q : ...`); **D4 fixed** (rv_muldiv.sv:55) | dev_d4_div_by_zero (FAILS on baseline) | A_MDU_DIV0; A_MDU_REM0 (shadow dividend a_ref_q → catch D4) | done |
| REQ-31 | CSR ops read old→rd, update per Zicsr; rs1=x0 / imm=0 = pure read (RO readable) | rv_csr.sv:78–90 (is_write masking, RW/RS/RC wr_val); rv_decode.sv:33–35,183–185 | req31_csr_semantics; req31_csrrc_forms_write; req31_32_read_only_csrs; req33_mcycle_write_readback; req33_mcycleh_pure_read_no_write; req33_minstret_write_readback; req33_minstreth_pure_read_no_write | A_CSR_MEPC (do_write path in def); A_EX_RESULT (rd←csr_rdata pin) | done |
| REQ-32 | unknown CSR addr → illegal; write to RO (addr[11:10]=3) → illegal | rv_csr.sv:77–79 (`ro`, `illegal = op≠NONE && (!known \|\| ro&&is_write)`) | req31_32_read_only_csrs; req34_illegal_encodings; req33_noncsr_instr_csr_addr_no_write | A_TRAP (exc via csr illegal gates trap); A_CSR_MEPC (write term) | done |
| REQ-33 | MINSTRET counts non-trapping completes; CSR read excludes itself | rv_csr.sv:147 (`else if (retire) minstret_q++`), retire=ex_fire&&!trap (rv_core.sv:283) | req33_minstret; req33_counter_deltas; req33_minstret_across_trap; req33_mcycleh_pure_read_no_write (self-exclusion via read-only path) | A_TRAP (pins retire-suppression term that gates the count) | done |
| REQ-34 | all undefined encodings illegal ([1:0]≠11, reserved funct3/funct7) | rv_decode.sv:39 (default illegal), 64–163 (per-opcode f3/f7 checks) | req34_illegal_encodings; cr3_reserved_f3_illegal; req23_illegal_load_f3; req39_illegal_traps_promptly | A_TRAP (illegal→exc→trap def pin); A_DEC_FIELDS (field extraction); A_CRD3_LEGAL (CR legality; A_DEC_LEGAL disabled — CHECK_ILLEGAL=0) | done |
| REQ-35 | trap: MEPC=pc of trapped instr (interrupt: next pc), MPIE←MIE, MIE←0, fetch @ MTVEC; no other effect | rv_core.sv:244–258 (trap→redirect_pc=mtvec, trap_pc=ex_pc_q); rv_csr.sv:104–107 | req34_35_mtval_no_other_effect; req35_trap_cycle_csr_write_suppressed; req30_flushed_csr_instr_no_write; req35_36_40_ecall_mret | A_CSR_MEPC; A_CSR_MPIE; A_CSR_MIE; A_TRAP; A_REDIRECT_PC | done |
| REQ-36 | MCAUSE/MTVAL per event table (0/2/3/4/6/11, irq 0x80000007/0x8000000B) | rv_core.sv:223–238 (exc_cause/exc_val mux); rv_csr.sv:53 (irq_cause), 106–107 (mcause/mtval write) | req36_ebreak; req34_35_mtval_no_other_effect; req36_csr_reset_values; req37_irq_cause_timer_when_ext_idle | A_TRAP (event-enable pin for the table write); cause/val values pinned by tests | done |
| REQ-37 | interrupt taken when MIE && (MIE&MIP)≠0 before next instr; ext > timer priority | rv_csr.sv:52–53 (irq_pending, ext-first irq_cause); rv_core.sv:244 (trap on ex_fire) | req37_irq_external_priority; req37_irq_timer; req37_irq_ext_only_mie; req37_irq_cause_timer_when_ext_idle | A_TRAP; A_CSR_MIE (mie_q def gates irq_pending) | done |
| REQ-38 | level-sensitive lines; masked assertion records nothing | rv_csr.sv:52 (irq_pending comb. from pins; no latch) | req38_masked_line_not_recorded | A_TRAP (trap requires live irq_pending — no stored event) | done |
| REQ-39 | interrupt taken within bounded time while instrs run; WFI does not wait | rv_core.sv:244 (trap at every ex_fire) | req39_interrupt_taken_bounded; req39_illegal_traps_promptly (prompt-trap twin) | A_TRAP; A_CSR_MPIE (entry pin) | done |
| REQ-40 | MRET continues fetch at MEPC | rv_core.sv:250–258 (mret → redirect_pc=csr_mepc); rv_fetch.sv:44–53 (pend redirect) | req40_mret_fetch_at_mepc; req35_36_40_ecall_mret | A_REDIRECT_PC (mret branch of mux def); A_CSR_MEPC | done |
| REQ-41 | MRET: MPIE→MIE then MPIE←1 | rv_csr.sv:108–110 (`mie_q≤mpie_q; mpie_q≤1'b1`); **D2 fixed** (mpie←1 was missing) | dev_d2_mret_sets_mpie (FAILS on baseline); req35_36_40_ecall_mret | A_CSR_MPIE (forces 1 after mret → catch D2); A_CSR_MIE | done |
| REQ-42 | synchronous to clk, rst_n async only, no latches, meets SDC | flow P stage — no sim test | n/a | n/a (flow build + OpenROAD stages enforce: yosys synth latch check, routed WNS vs 15 ns constraint; local baseline flow clean) | n/a (structural) |

## Notes
- Extract/refresh REQ list: `grep -ohE 'REQ-[0-9]+' challenge-3/spec.md challenge-3/change_request.md | sort -uV` (18/19 absent by spec design; REQ-20 only in change_request.md CR-D3 §1).
- DEV rows = confirmed seeded deviations D1–D4 (details in `docs/designs/challenge-3.md`); the listed `test_dev_*` tests fail on `baseline/rtl`. 22 baseline failures = D1–D4 legs + REQ-25 leg + CR tests (cr1–cr5) + REQ-20-adjacent random seeds. D5 was a false alarm; `test_dev_d5_muldiv_during_load_stall` kept as wait-state stress (passes on both trees).
- Observer (`rv_stage_sva`, vendored, bound via `rv_core_stage_bind.sv`, params IALIGN32=1/CHECK_ILLEGAL=0/HAZARD_MODE=0/CR_D3=1): active props are A_DEC_FIELDS, A_DEC_IMM, A_EX_ALU, A_EX_BRANCH, A_EX_TARGET_BRANCH, A_EX_TARGET_JALR, A_ID_PC_ALIGN, A_ID_TO_EX; mem/wb/if-side props mapped to constants (no instr word past EX in the real core) — re-implemented by rv_core_defs next-state pins (A_MEMV_NXT/A_WBV_NXT/A_WBE_NXT/A_WBRD_NXT/A_WBDATA_NXT/A_MEM_FINAL).
- REQ-26/28/29 functional value semantics (M-extension bit results, rounding, overflow) are enforced end-to-end by the ISS lockstep random suite (rviss golden model); in-RTL assertion coverage there is the mdu result-mux/start pins (A_EX_RESULT/A_MDU_START) — a mul/div datapath pin would be nice-to-have but none exists (M units masked out of observer A_EX_ALU).
- REQ-05 and REQ-42 rows cite the flow build/P stages, not tests: verilator lint 0 warnings + yosys synthesis (both run every flow invocation); outputs are register-driven, so no combinational dependence on `*_ready`/`*_rdata` by construction.
- Assertion validation evidence (Docker, flow image): fixed RTL `make ASSERTIONS=1` → all tests PASS, 0 firings; baseline → `$stop` at first firing; isolated runs prove each deviation fires its mapped prop (see header).
