# Change Request CR-D3: Address-generation and min/max instructions

Applies to: `rv_core` specification revision 1.0. Design 3.

## Motivation

Compiled code spends many cycles on array indexing and on clamping values. This change adds seven standard instructions from the RISC-V Zba and Zbb extensions. They run in one execute cycle like other ALU instructions.

## Change

1. New requirement in section 5:
   - **REQ-20** The core executes the following R-type instructions with opcode `0110011`:

   | Instruction | funct7 | funct3 | Result in rd |
   |---|---|---|---|
   | SH1ADD | `0010000` | `010` | (rs1 << 1) + rs2 |
   | SH2ADD | `0010000` | `100` | (rs1 << 2) + rs2 |
   | SH3ADD | `0010000` | `110` | (rs1 << 3) + rs2 |
   | MIN | `0000101` | `100` | smaller of rs1 and rs2, signed |
   | MINU | `0000101` | `101` | smaller of rs1 and rs2, unsigned |
   | MAX | `0000101` | `110` | larger of rs1 and rs2, signed |
   | MAXU | `0000101` | `111` | larger of rs1 and rs2, unsigned |

2. Results wrap modulo 2^32. The operands and results take part in forwarding like those of any other ALU instruction (REQ-12).
3. Other values of funct3 with those funct7 values remain illegal (REQ-34).
4. MISA keeps the value `0x40001100`. No port changes.

## Acceptance

- With x1 = `0x80000003` and x2 = 100, SH1ADD, SH2ADD and SH3ADD give 106, 112 and 124.
- With x1 = `0xFFFFFFFF` and x2 = 1, MIN gives `0xFFFFFFFF`, MINU gives 1, MAX gives 1 and MAXU gives `0xFFFFFFFF`.
- funct7 = `0010000` with funct3 = `000` traps with cause 2.
