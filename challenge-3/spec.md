# rv_core: Five-Stage RV32IM Processor Core

Specification, revision 1.0. Design 3.

## 1. Overview

`rv_core` is a 32-bit RISC-V core with a five-stage pipeline: fetch, decode, execute, memory and writeback. It implements the RV32I base integer instruction set, the M extension for multiply and divide, the Zicsr instructions, and a machine-mode subset of the RISC-V privileged architecture. It has separate instruction and data memory interfaces and two interrupt inputs.

Where this document is silent on an instruction's result, the RISC-V Instruction Set Manual (unprivileged volume, version 20191213, and privileged volume, version 20211203) applies. Requirements are numbered `REQ-xx`. Every requirement is testable at the ports of `rv_core`, by running programs. Text marked "Note" is informative.

Conventions. "Cycle" means one period of `clk`. A memory access completes on a rising edge at which valid and ready are both 1.

## 2. Ports

| Port | Dir | Width | Description |
|---|---|---|---|
| `clk` | in | 1 | Clock. |
| `rst_n` | in | 1 | Asynchronous reset, active low. |
| `imem_valid` | out | 1 | Instruction fetch request. |
| `imem_ready` | in | 1 | Fetch completes in this cycle. |
| `imem_addr` | out | 32 | Fetch byte address, word aligned. |
| `imem_rdata` | in | 32 | Instruction word, valid when `imem_ready` = 1. |
| `dmem_valid` | out | 1 | Data access request. |
| `dmem_ready` | in | 1 | Data access completes in this cycle. |
| `dmem_write` | out | 1 | 1 for a store. |
| `dmem_addr` | out | 32 | Data byte address, word aligned. |
| `dmem_wstrb` | out | 4 | Byte write strobes. |
| `dmem_wdata` | out | 32 | Store data, placed in the addressed byte lanes. |
| `dmem_rdata` | in | 32 | Load word, valid when `dmem_ready` = 1. |
| `irq_ext` | in | 1 | External interrupt request, level sensitive. |
| `irq_timer` | in | 1 | Timer interrupt request, level sensitive. |

## 3. Reset

- **REQ-01** Reset may be asserted at any time. All general registers, CSRs and pipeline state take the values given in this document, and general registers are 0.
- **REQ-02** After reset, `dmem_valid` = 0. The first fetch after reset is from address 0.

## 4. Memory interfaces

- **REQ-03** While `imem_valid` = 1 and `imem_ready` = 0, `imem_valid` and `imem_addr` hold their values. The core may fetch a word and not use it, and may fetch the same word again. Instruction memory reads have no side effects.
- **REQ-04** While `dmem_valid` = 1 and `dmem_ready` = 0, `dmem_valid`, `dmem_write`, `dmem_addr` and `dmem_wstrb` hold their values, and for a store `dmem_wdata` also holds.
- **REQ-05** The outputs of both interfaces do not depend combinationally on `imem_ready`, `imem_rdata`, `dmem_ready` or `dmem_rdata`.
- **REQ-06** Each load or store instruction that executes makes exactly one data access, and each instruction that does not execute makes none. This includes instructions on a wrong path and instructions that trap.
- **REQ-07** For a load, `dmem_wstrb` = `4'b0000`. For a store, `dmem_wstrb` selects the addressed bytes: one bit for SB, the lower or upper two bits for SH, and all bits for SW. The data of an SB or SH appears in the addressed byte lanes.
- **REQ-08** `dmem_addr` and `imem_addr` have bits [1:0] = 0 whenever their valid is 1.

## 5. Instruction execution

- **REQ-09** The core executes all RV32I instructions except that ECALL, EBREAK, FENCE, CSR instructions, WFI and MRET behave as stated in this document. FENCE and FENCE.I run as no-ops. WFI runs as a no-op.
- **REQ-10** Instructions take effect in program order. Results do not depend on memory wait states, and results are the same with `imem_ready` and `dmem_ready` held at 1 and with any pattern of wait states.
- **REQ-11** Register x0 always reads as 0. This also holds for an instruction that reads x0 right after an instruction with rd = x0, including an ALU instruction, a load, a CSR instruction, a multiply and a JAL.
- **REQ-12** A value written to a register is available to the next instruction. The core stalls or forwards as needed, including a load followed by an instruction that uses the loaded value.
- **REQ-13** Instructions fetched after a taken branch or a jump have no effect.
- **REQ-14** JALR clears bit 0 of the computed target address.
- **REQ-15** Branch and jump targets with bit 1 set raise an instruction-address-misaligned exception (REQ-36) when the branch is taken or the jump runs.
- **REQ-16** Shift amounts use the low five bits of the register operand.
- **REQ-17** SLTIU compares against the sign-extended immediate as an unsigned value.

Note: throughput beyond these rules is not specified. An implementation may change the number of cycles of any instruction.

## 6. Loads and stores

- **REQ-21** LB and LH sign-extend, and LBU and LHU zero-extend.
- **REQ-22** LW and SW require an address aligned to 4, and LH, LHU and SH require an address aligned to 2. A violation raises an exception (REQ-36) with no data access.
- **REQ-23** LB and LBU at offset k return byte k of the word.
- **REQ-24** LHU at offset 2 returns bits [31:16] of the word.
- **REQ-25** LH at offset 0 returns bits [15:0] of the word, extended from bit 15. LH at offset 2 returns bits [31:16] of the word, extended from bit 31.

## 7. Multiply and divide

- **REQ-26** MUL, MULH, MULHSU and MULHU return the bits defined by the M extension. MULHSU treats rs1 as signed and rs2 as unsigned.
- **REQ-27** Multiply and divide may take many cycles. The rest of the pipeline waits.
- **REQ-28** DIV and REM round toward zero. The sign of a nonzero REM result equals the sign of the dividend.
- **REQ-29** DIV of -2^31 by -1 returns -2^31, and REM of the same operands returns 0.
- **REQ-30** With a divisor of 0: DIV returns -1, DIVU returns 2^32 - 1, and REM and REMU return the dividend.

## 8. Control and status registers

The following CSRs exist. All are 32 bits. Reset values are 0 unless stated.

| Address | Name | Access | Notes |
|---|---|---|---|
| 0xF11, 0xF12, 0xF13, 0xF14 | MVENDORID, MARCHID, MIMPID, MHARTID | RO | Read as 0. |
| 0x300 | MSTATUS | RW | [3] MIE, [7] MPIE. [12:11] MPP reads as 3 and ignores writes. Other bits read as 0. |
| 0x301 | MISA | RW | Reads `0x40001100`. Writes are ignored. |
| 0x304 | MIE | RW | [7] MTIE, [11] MEIE. Other bits read as 0. |
| 0x305 | MTVEC | RW | [31:2] BASE. [1:0] read as 0: direct mode only. |
| 0x340 | MSCRATCH | RW | |
| 0x341 | MEPC | RW | [31:2]. [1:0] read as 0. |
| 0x342 | MCAUSE | RW | |
| 0x343 | MTVAL | RW | |
| 0x344 | MIP | RW | [7] MTIP, equal to `irq_timer`. [11] MEIP, equal to `irq_ext`. Writes are ignored. |
| 0xB00, 0xB80 | MCYCLE, MCYCLEH | RW | 64-bit cycle counter. |
| 0xB02, 0xB82 | MINSTRET, MINSTRETH | RW | 64-bit retired instruction counter. |

- **REQ-31** CSRRW, CSRRS, CSRRC and their immediate forms read the old value into rd and update the CSR as the Zicsr extension defines. CSRRS and CSRRC with rs1 = x0, or a zero immediate, do not write and so can read a read-only CSR.
- **REQ-32** Access to an address not in the table raises an illegal-instruction exception. So does a write to a read-only CSR (address bits [11:10] = 3).
- **REQ-33** MINSTRET counts instructions that complete without a trap. A CSR read of MINSTRET does not include the reading instruction.

## 9. Traps

- **REQ-34** Every encoding not defined by this document raises an illegal-instruction exception, including encodings with bits [1:0] other than 3, and reserved values of funct3 and funct7.
- **REQ-35** On a trap, MEPC is written with the address of the instruction that trapped or, for an interrupt, the address of the next instruction that has not run. MPIE takes the value of MIE, MIE clears, and fetch continues at MTVEC. The trapping instruction has no other effect.
- **REQ-36** MCAUSE and MTVAL are written as follows:

| Event | MCAUSE | MTVAL |
|---|---|---|
| Instruction address misaligned | 0 | target address |
| Illegal instruction | 2 | instruction word |
| EBREAK | 3 | address of the EBREAK |
| Load address misaligned | 4 | load address |
| Store address misaligned | 6 | store address |
| ECALL | 11 | 0 |
| Timer interrupt | `0x80000007` | 0 |
| External interrupt | `0x8000000B` | 0 |

- **REQ-37** An interrupt is taken when MSTATUS.MIE = 1 and (MIE AND MIP) is nonzero, before the next instruction takes effect. Instructions before that point complete. External has priority over timer.
- **REQ-38** Interrupt lines are level sensitive. A line must stay asserted until it is taken. The core records nothing when a line is asserted while masked.
- **REQ-39** An interrupt is taken within a bounded time while instructions execute. WFI does not wait for it.
- **REQ-40** MRET continues fetch at MEPC.
- **REQ-41** MRET copies MPIE to MIE, then sets MPIE to 1.

## 10. Timing and implementation constraints

- **REQ-42** The design is synchronous to `clk`, uses only `rst_n` as an asynchronous reset, has no latches, and meets the released clock constraint (`flow/constraint.sdc`).

## 11. Compatibility notes

- The module name, port names and port widths in section 2 are fixed.
- The filelist `rtl/filelist.f` lists the source files in compile order, with paths relative to the design root.
