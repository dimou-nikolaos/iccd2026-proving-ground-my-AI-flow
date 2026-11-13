# challenge-3 — `rv_core` (five-stage RV32IM core)

Design knowledge doc. Update during Phase 1+. Signal names live HERE only.

## Release facts

- Released 2026-11-13. Archive `design-3.zip`; `challenge-3/` verified byte-identical to release.
- Top: `rv_core`, 10 files in `rtl/` (filelist order: rv_pkg, rv_regfile, rv_decode, rv_alu, rv_muldiv, rv_csr, rv_hazard, rv_fetch, rv_lsu, rv_core), 1203 lines.
- Clock 15.0 ns sky130hd, async active-low `rst_n`. Ports: imem (valid/ready/addr/rdata), dmem (valid/ready/write/addr/wstrb/wdata/rdata), irq_ext, irq_timer.
- Spec: REQ-01..17, 20..42 (no 18/19). CR-D3 adds REQ-20: SH1ADD/SH2ADD/SH3ADD (funct7=0010000, f3=010/100/110) + MIN/MINU/MAX/MAXU (funct7=0000101, f3=100/101/110/111), all opcode 0x33.
- Archetype: **pipelined core** (playbook §3).

## Baseline md5 (pristine, committed)

c4d1397a rv_alu, 68a57e89 rv_core, e4bc942e rv_csr, 1f342b10 rv_decode, 25796e52 rv_fetch, bc740fd8 rv_hazard, d73cd05c rv_lsu, 89e01cb7 rv_muldiv, d7fbac74 rv_pkg, 601a42c8 rv_regfile

## Hierarchy + signal map

- `rv_core.sv` holds all pipeline registers: id_* (from fetch), `ex_valid_q/ex_pc_q/ex_instr_q/ex_rs1_q/ex_rs2_q/ex_dec_q`, `mem_valid_q/mem_result_q/mem_store_q/mem_rd_q/mem_wb_en_q/mem_is_load_q/mem_is_store_q/mem_unsigned_q/mem_size_q`, `wb_valid_q/wb_en_q/wb_rd_q/wb_data_q`.
- Stall/flush: `mem_stall = dmem_valid && !dmem_ready`; `mdu_busy = ex_valid_q && ex_dec_q.is_mdu && !mdu_done`; `stall_front = mem_stall||mdu_busy||load_use`; `ex_fire = ex_valid_q && !mem_stall && !mdu_busy`. `redirect/redirect_pc` from trap/mret/flow_change.
- ID→EX: mem_stall||mdu_busy → operand-refresh branch (`ex_rs1_q <= fwd_rs1`); redirect||load_use → bubble; else advance.
- `decoded_t` struct packed (rv_pkg): rs1,rs2,rd,uses_rs1,uses_rs2,wb_en,imm,alu_op,src_a,src_b_imm,is_load,is_store,mem_size,mem_unsigned,is_branch,branch_f3,is_jal,is_jalr,csr_op,csr_uses_imm,csr_addr,is_mdu,mdu_op,is_ecall,is_ebreak,is_mret,illegal.
- Fetch: `pc_q`, `pend_pc_q/pend_q` (redirect during imem wait), `started_q`; imem_valid=started_q.
- Muldiv: FSM S_IDLE/S_RUN/S_DONE, `count_q=32`, 64-bit `acc_q`, shared shift-add/restore sequencer; start gated by `mdu_start = ex_valid_q && is_mdu && !mdu_active_q && !mem_stall`; `mdu_active_q` cleared on `mdu_done && !mem_stall`.
- CSR: `mie_q,mpie_q,meie_q,mtie_q,mtvec_q,mepc_q,mscratch_q,mcause_q,mtval_q,mcycle_q,minstret_q`. `irq_pending = mie_q && ((meie_q&&irq_ext)||(mtie_q&&irq_timer))`, ext priority. trap→mpie←mie,mie←0,mepc←trap_pc; mret→mie←mpie (NOTE: mpie←1 missing — see D2).
- Hazard: forwarding EX/MEM over MEM/WB, but **no x0 guard** (see D1); `load_use` interlock 1 bubble.
- LSU: byte-lane strobes/wdata, load extract+extend; `dmem_addr` word-aligned always.
- Regfile: `regs_q[1:31]`, x0 hard-zero + write-guard + write-through read.
- Public TB protocol: store to TOHOST=0xFFFF0000 halts; DEVICE_IRQ=0xFFFF0004 drives irq lines (bit0 ext, bit1 timer); SIG_BASE=0x00004000 dump region; `dump(regs)`/`halt()` pseudo-ops in rv_asm.py.

## Deviations found (Phase 1) — CONFIRMED by probe sweep 2026-11-13

| # | REQ | Location | Spec says | RTL does | Pattern | Probe evidence (baseline→fixed) |
|---|-----|----------|-----------|----------|---------|-------------------------------|
| D1 | REQ-11 | rv_hazard.sv:30,40 | x0 always reads 0, incl. right after rd=x0 (ALU/load/CSR/mul/JAL) | forward match matched on reg 0: rd=x0 producer in MEM/WB injected its value into an x0 operand in EX | drop_term (missing guard) | FAIL→PASS (obs x5=5/10, CSR/mul/load values; JAL-x0 producer sub-case did NOT corrupt — use others for assertions) |
| D2 | REQ-41 | rv_csr.sv:108 | MRET: MPIE←MIE then MPIE←1 | `mpie_q` never loaded 1 | missing latch | FAIL→PASS (mstatus 0x1800 vs 0x1880; MPP=3 base 0x1800) |
| D3 | REQ-25 | rv_lsu.sv:64 | LH at offset 2 extends from bit 31 | replicate bit `dmem_rdata[15]` both offsets | off-by-one bit-slice | FAIL→PASS (0x8000_0000@+2 → 0x00008000 vs 0xFFFF8000; 0x1234_8000@+2 → 0xFFFF1234 vs 0x00001234) |
| D4 | REQ-30 | rv_muldiv.sv:55 | div-by-0: REM/REMU return dividend | returned 0; dividend unstored | drop_term/missing | FAIL→PASS (div fixed 0xFFFFFFF9→0; fixed RTL returns raw dividend) |

D5 (mdu_active_q race) = **FALSE ALARM** — schedule analysis (mdu_start gated by !mem_stall; mdu_busy freezes front-end from EX-entry, MEM drains and stays empty during muldiv run) + 4-seed wait-state probes clean on BOTH trees. Keep wait-state stress in the test suite anyway.

Probe-per-REQ sweep (Gate PASSED): 44 probes, all PASS on fixed; 7 FAIL on baseline (D1-D4 + REQ-25 probe + 2 CR-absent probes). REQ-05/42 = structural → assigned to lint/synth stage in REQ matrix. Deviation count = 4 (in 3-6 band; random ISS differential in Phase 3 is the backstop for a 5th).

## CR-D3 implementation site

- `rv_decode.sv` OP_OP `unique case (f7)`: add `7'b0010000` (SH1ADD/2/3: f3 010/100/110, alu = (rs1<<k)+rs2) and `7'b0000101` (MIN/MINU/MAX/MAXU: f3 100..111); other f3 illegal. `rv_pkg.sv` alu_op_t: add ALU ops (new names, keep existing). `rv_alu.sv`: add arms — SHxADD `(a<<k)+b`, MIN/MAX reuse `lt_signed/lt_unsigned`. Results wrap mod 2^32, no traps. MISA unchanged.
- ALU enum is `logic [4:0]` with 10 used → 22 free slots; enum re-encode is fine (synthesis style may shift slightly).

## V ceiling

- TBD after first mutation run. Expect survivors in unused encodings (e.g., alu default arms, fence variants) — classify before chasing.

## PPA plan

See `challenge-3-ppa-plan.md`. Prior: 15 ns → likely area/power game (multiplier+divider+regfile+64-bit counters dominate). `mcycle_q/minstret_q` 64-bit always-on counters are large; spec pins them RW and counting but NOT exact values — audit freedom before touching.

## Assertions (Phase 3b) — mapped vs disabled

Files in `assertions/`: `rv_ref_pkg.sv` + `rv_stage_sva.sv` (vendored, UNMODIFIED), `rv_core_stage_bind.sv` (observer bind), `rv_top_if.sv`, `rv_core_defs.sv`, `rv_csr_defs.sv`, `rv_muldiv_defs.sv`.

**Observer (`rv_stage_sva`, params IALIGN32=1, CHECK_ILLEGAL=0, HAZARD_MODE=0, CR_D3=1):**
- Mapped & active: `A_DEC_FIELDS`, `A_DEC_IMM`, `A_EX_ALU`, `A_EX_BRANCH`, `A_EX_TARGET_BRANCH`, `A_EX_TARGET_JALR`, `A_ID_PC_ALIGN`. Taps: id_* / id_dec fields; ex_* = ex_pc_q/ex_instr_q/**alu_a**/**alu_b**/alu_result/branch_taken/target (operands the ALU actually used).
- DISABLED (mapped to constant-0 → vacuous): all `if_*` (A_IF_PC_FLOW, A_IF_PC_ALIGN), all `mem_*` (A_MEM_ADDR/SIZE/EXCLUSIVE/WE_IS_STORE/RE_IS_LOAD/LOAD_DATA), all `wb_*` (A_WB_RD/WE/DATA_LUI), `redirect_*`/`flush_*`. Reason: the real core carries **no instruction word into MEM/WB** (only decoded fields), so the observer's word-decode contract is un-mappable there; imem_addr uses a 2-phase pending-redirect (pend_q) so A_IF_PC_FLOW's single-cycle-redirect model would self-fire. Their coverage is re-implemented at MEM/WB by `rv_core_defs` (load_data def, next-state pins).
- `ex_valid` masked in the bind: `ex_valid_q && !ex_dec_q.is_mdu && !((is_branch||is_jal) && target[1])`. (1) M-ext: decode leaves alu_op=ADD so alu_result=a+b ≠ M alu_ref → A_EX_ALU would self-fire on legal mul/div; masked out (M covered by muldiv pins + cocotb ISS). (2) branch/JAL with target[1]=1 (REQ-15 misaligned) — observer's A_EX_TARGET_BRANCH pins target[1:0]==00 which is NOT spec-guaranteed (bit1 is the legal misaligned-exception value); masked only for that cycle so value equality still checked on aligned targets.
- `CHECK_ILLEGAL=0`: observer legal_ref disagrees with the real decoder on edge encodings (e.g. OP-IMM slli f7=0100000), so A_DEC_LEGAL/A_CRD3_LEGAL risk self-fire on illegal-instruction tests. Replaced by the safe forward-only `A_CRD3_LEGAL` in rv_core_defs.

**rv_core_defs (bind rv_core, 27 props):** signal-def pins (mem_stall/mdu_busy/stall_front/ex_fire/mdu_start/trap/mret/redirect mux/redirect_pc/ex_link/flow_change/target_misaligned/alu_a/alu_b/ex_result mux/mem_final/ls_misaligned), forwarding defs `A_FWD_RS1/RS2` (WITH x0 guard → **catches D1**), `A_LOAD_USE`, load-data def `A_LOAD_DATA` (spec sign/zero-extend → **catches D3**), next-state identities (mem_valid_q/wb_valid_q/wb_en/wb_rd/wb_data via $past), `A_CRD3_LEGAL` (CR encodings not-illegal → **catches CR-not-implemented**).
**rv_csr_defs (bind rv_csr, 3):** mie_q/mpie_q/mepc_q next-state identities; MPIE forces 1 after mret → **catches D2**.
**rv_muldiv_defs (bind rv_muldiv, 2):** div0→all-ones, rem0→shadow dividend `a_ref_q` → **catches D4**.
**rv_top_if (bind rv_core ports, 8):** REQ-03 imem hold, REQ-04 dmem hold+wdata, REQ-07 load wstrb=0, REQ-08 addr[1:0]=0, REQ-02 dmem_valid=0 first cycle post-reset + first fetch addr 0.

**Validation (Docker, flow image):** fixed RTL `make ASSERTIONS=1` → **64/64 PASS, 0 assertion firings, 19.5 s wall** (< 45 s). Baseline `make RTL_ROOT=../baseline ASSERTIONS=1` → assertion `$stop` aborts sim; isolated runs prove firing: D1→A_FWD_RS1, D2→A_CSR_MPIE, D3→A_LOAD_DATA, D4→A_MDU_REM0, CR→A_CRD3_LEGAL.
