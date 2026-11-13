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

## Deviations found (Phase 1)

| # | REQ | Location | Spec says | RTL does | Pattern | Conf. | Status |
|---|-----|----------|-----------|----------|---------|-------|--------|
| D1 | REQ-11 | rv_hazard.sv:30,40 | x0 always reads 0, incl. right after rd=x0 (ALU/load/CSR/mul/JAL) | forward match `mem_rd == ex_rs1` matches when both are 0: instruction with rd=x0 in MEM/WB forwards its result into an x0 operand of the EX instruction | drop_term (missing guard) | HIGH | probe pending |
| D2 | REQ-41 | rv_csr.sv:108-110 | MRET copies MPIE→MIE, then sets MPIE=1 | `mret` branch sets only `mie_q <= mpie_q`; `mpie_q` never loaded with 1 | missing latch / drop_term | HIGH | probe pending |
| D3 | REQ-25 | rv_lsu.sv:64 | LH at offset 2 returns bits[31:16] extended **from bit 31** | sign bit taken from `dmem_rdata[15]` for both half offsets | off-by-one in bit-slice | HIGH | probe pending |
| D4 | REQ-30 | rv_muldiv.sv:55 | div-by-0: REM/REMU return the dividend | `rem_out = (op_b_q==0) ? 32'd0 : ...` returns 0; dividend not even stored (no op_a_q) | drop_term / missing logic | HIGH | probe pending |
| D5 | REQ-10 | rv_core.sv:289-297 | results same with any wait-state pattern | if `mdu_done` lands while `mem_stall` (older load in MEM waiting dmem), clear `mdu_active_q && !mem_stall` is missed; mdu_active_q stuck → every later muldiv hangs (mdu_busy forever) | missing reset branch / gating | MEDIUM-HIGH | probe pending |

Notes: public TB `test_load_store` uses `lh(5,1,0)` (offset 0, passes with D3) but never `lh` at offset 2 — D3 hides in plain sight. `test_multiply_divide` uses ordinary divisors (no div-by-0, no INT_MIN/-1).

## CR-D3 implementation site

- `rv_decode.sv` OP_OP `unique case (f7)`: add `7'b0010000` (SH1ADD/2/3: f3 010/100/110, alu = (rs1<<k)+rs2) and `7'b0000101` (MIN/MINU/MAX/MAXU: f3 100..111); other f3 illegal. `rv_pkg.sv` alu_op_t: add ALU ops (new names, keep existing). `rv_alu.sv`: add arms — SHxADD `(a<<k)+b`, MIN/MAX reuse `lt_signed/lt_unsigned`. Results wrap mod 2^32, no traps. MISA unchanged.
- ALU enum is `logic [4:0]` with 10 used → 22 free slots; enum re-encode is fine (synthesis style may shift slightly).

## V ceiling

- TBD after first mutation run. Expect survivors in unused encodings (e.g., alu default arms, fence variants) — classify before chasing.

## PPA plan

See `challenge-3-ppa-plan.md`. Prior: 15 ns → likely area/power game (multiplier+divider+regfile+64-bit counters dominate). `mcycle_q/minstret_q` 64-bit always-on counters are large; spec pins them RW and counting but NOT exact values — audit freedom before touching.
