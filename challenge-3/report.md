# Submission report: design-3 (rv_core)

## Deviations found

| # | Requirement | Location in the baseline | Symptom | Found by |
|---|---|---|---|---|
| 1 | REQ-11 (x0 always reads 0) | `rtl/rv_hazard.sv`, forwarding match `mem_rd == ex_rs1` / `wb_rd == ex_rs1` | Missing x0 guard: an instruction whose rd = x0 (ALU, load, CSR, multiply, JAL) sits in MEM/WB while the EX instruction reads x0 — the forwarder matches on register number 0 and injects the producer's result into an x0 read. Observed: `addi x0,x1,5; addi x5,x0,0` gave x5 = 5 (and 10, or the CSR/mul/load value), instead of 0. | `test_dev_d1_x0_forward` (fails on baseline) |
| 2 | REQ-41 (MRET sets MPIE to 1) | `rtl/rv_csr.sv`, `else if (mret)` branch | The branch copies MPIE to MIE but never sets MPIE to 1. After MRET, mstatus.MPIE stays at its old value; a subsequent trap loses the interrupt-enable state it should have pushed (observed mstatus 0x00001800 instead of 0x00001880 after MRET). | `test_dev_d2_mret_sets_mpie` (fails on baseline) |
| 3 | REQ-25 (LH at offset 2 extends from bit 31) | `rtl/rv_lsu.sv`, halfword load extension | Sign-replicate bit is `dmem_rdata[15]` for both halfword offsets. LH at byte offset 2 extends from bit 15 instead of bit 31: load of 0x8000_0000 at +2 returned 0x0000_8000 instead of 0xFFFF_8000; load of 0x1234_8000 at +2 returned 0xFFFF_1234 instead of 0x0000_1234. | `test_dev_d3_lh_offset2`, `test_req25_lh_offset0` (fail on baseline) |
| 4 | REQ-30 (REM/REMU with divisor 0 return the dividend) | `rtl/rv_muldiv.sv`, `rem_out` | `rem_out` forced the zero-divisor case to 0; the dividend was not even stored in the unit. REM/REMU by zero returned 0 instead of the dividend (observed 0 for dividend 0xFFFFFFF9; DIV/DIVU by zero were already correct). | `test_dev_d4_div_by_zero` (fails on baseline) |

The four deviations were localized statically (spec-vs-RTL expression audit) and each is
confirmed by a directed probe that fails on the released baseline and passes on the fixed RTL.

## Changes made

1. `rv_hazard.sv`: added `ex_rsN != 5'd0` guards to the four forwarding match arms (D1).
2. `rv_csr.sv`: added `mpie_q <= 1'b1;` to the `mret` branch (D2).
3. `rv_lsu.sv`: halfword sign extension replicates `half[15]` instead of `dmem_rdata[15]` (D3).
4. `rv_muldiv.sv`: added `op_a_q` (loaded with the raw dividend `a` on start); `rem_out` returns `op_a_q` when the divisor is zero (D4).
5. Change request CR-D3 (REQ-20): `rv_pkg.sv` — appended `ALU_SH1ADD, ALU_SH2ADD, ALU_SH3ADD, ALU_MIN, ALU_MINU, ALU_MAX, ALU_MAXU` to `alu_op_t` (existing member encodings unchanged); `rv_decode.sv` — new funct7 arms `0010000` (SH1ADD/SH2ADD/SH3ADD, f3=010/100/110) and `0000101` (MIN/MINU/MAX/MAXU, f3=100/101/110/111), other funct3 stay illegal; `rv_alu.sv` — seven new result arms: SHxADD = `(a<<k)+b` (wrap), MIN/MAX select on the existing `lt_signed`/`lt_unsigned` comparators. Results wrap modulo 2^32 and take part in forwarding like any ALU instruction; MISA unchanged.

Ports, the filelist and the clock constraint are unchanged.

## How it was verified

- `probes/test_probes.py` (submitted separately with the next drop as `tests/`): 44 directed cocotb tests on the released harness — one per externally testable REQ (REQ-01..04, 06..17, 20..41), 4 deviation tests, 3 CR acceptance tests. On the fixed RTL: 44/44 pass. On the released baseline: 7 fail (the 4 deviation tests, both CR tests, and the REQ-25 offset probe).
- CR-D3 acceptance from `change_request.md`: SH1ADD/SH2ADD/SH3ADD of 0x80000003 and 100 → 106/112/124; MIN/MINU/MAX/MAXU of 0xFFFFFFFF and 1 → 0xFFFFFFFF/1/1/0xFFFFFFFF; funct7=0010000 with funct3=000 traps with cause 2. All pass.
- Wait-state robustness (REQ-10): muldiv-dense programs run at istall/dstall=0.3 over 4 seeds, no hang on baseline or fixed RTL; the shared harness checks REQ-03/04 request-hold rules and REQ-07/08 port rules on every run.
- Verilator lint (-Wall) and Yosys elaboration are clean on the fixed RTL; the released public testbench passes 12/12.

Not covered by these tests: REQ-05 (combinational independence from ready) and REQ-42 (synchronous/no-latch/SDC compliance) are structural properties checked by the flow's lint/synthesis stages, not at ports; MCYCLE/MCYCLEH exact values and counter wrap are not pinned by the spec and only ordering is checked; rare interrupt-vs-precise-exception priority combinations beyond external-over-timer follow the spec's stated rule and are covered by the timer/external/priority probes.
