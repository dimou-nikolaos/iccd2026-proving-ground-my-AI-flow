"""Public testbench for rv_core.

These tests pass on the released baseline. They are a starting point, not a
complete verification plan: passing them does not mean the design meets its
specification.
"""

import cocotb

from rv_asm import MCAUSE, MEPC, MISA, MSCRATCH, MTVEC, Asm
from rv_tb import run_program


def finish(a, regs):
    a.dump(regs)
    a.halt()


@cocotb.test()
async def test_first_fetch_is_zero(dut):
    """The first fetch after reset is at address 0."""
    a = Asm()
    a.halt()
    tb = await run_program(dut, a)
    assert tb.fetch_log[0] == 0


@cocotb.test()
async def test_arithmetic(dut):
    """Register and immediate arithmetic."""
    a = Asm()
    a.li(1, 1000)
    a.li(2, 234)
    a.add(3, 1, 2)
    a.sub(4, 1, 2)
    a.addi(5, 1, -1)
    a.xor(6, 1, 2)
    a.or_(7, 1, 2)
    a.and_(8, 1, 2)
    a.xori(9, 1, 0xF)
    a.ori(10, 1, 0xF)
    a.andi(11, 1, 0xF)
    a.lui(12, 0x12345)
    a.auipc(13, 0)
    finish(a, [3, 4, 5, 6, 7, 8, 9, 10, 11, 12])
    tb = await run_program(dut, a, regs=[3, 4, 5, 6, 7, 8, 9, 10, 11, 12])
    assert tb.values == [1234, 766, 999, 1000 ^ 234, 1000 | 234, 1000 & 234, 1000 ^ 15, 1000 | 15,
                         1000 & 15, 0x12345000]


@cocotb.test()
async def test_shifts_and_compares(dut):
    """Shifts and set-less-than on small positive values."""
    a = Asm()
    a.li(1, 0x10)
    a.li(2, 2)
    a.sll(3, 1, 2)
    a.srl(4, 1, 2)
    a.slli(5, 1, 3)
    a.srli(6, 1, 1)
    a.slt(7, 2, 1)
    a.sltu(8, 1, 2)
    a.slti(9, 2, 5)
    a.li(10, -16)
    a.srai(11, 10, 2)
    finish(a, [3, 4, 5, 6, 7, 8, 9, 11])
    tb = await run_program(dut, a, regs=[3, 4, 5, 6, 7, 8, 9, 11])
    assert tb.values == [0x40, 4, 0x80, 8, 1, 0, 1, 0xFFFFFFFC]


@cocotb.test()
async def test_branches_and_loop(dut):
    """A counting loop and branch conditions."""
    a = Asm()
    a.li(1, 0)
    a.li(2, 10)
    a.label("loop")
    a.addi(1, 1, 1)
    a.bne(1, 2, "loop")
    a.li(3, 0)
    a.beq(1, 2, "eq")
    a.addi(3, 3, 1)
    a.label("eq")
    a.blt(2, 1, "bad")
    a.bge(1, 2, "ok")
    a.label("bad")
    a.addi(3, 3, 2)
    a.label("ok")
    finish(a, [1, 3])
    tb = await run_program(dut, a, regs=[1, 3])
    assert tb.values == [10, 0]


@cocotb.test()
async def test_jumps(dut):
    """JAL and JALR with link values."""
    a = Asm()
    a.jal(1, "f")
    a.label("back")
    a.li(3, 7)
    a.jal(0, "end")
    a.label("f")
    a.addi(2, 1, 0)
    a.jalr(4, 1, 0)
    a.label("end")
    finish(a, [1, 2, 3, 4])
    tb = await run_program(dut, a, regs=[1, 2, 3])
    assert tb.values == [4, 4, 7]


@cocotb.test()
async def test_load_store(dut):
    """Word, byte and low halfword accesses."""
    a = Asm()
    a.li(1, 0x8000)
    a.lw(2, 1, 0)
    a.lb(3, 1, 3)
    a.lbu(4, 1, 3)
    a.lh(5, 1, 0)
    a.lhu(6, 1, 2)
    a.li(7, 0xABCD)
    a.sw(7, 1, 4)
    a.sb(7, 1, 9)
    a.sh(7, 1, 12)
    a.lw(8, 1, 4)
    a.lw(9, 1, 8)
    a.lw(10, 1, 12)
    finish(a, [2, 3, 4, 5, 6, 8, 9, 10])
    tb = await run_program(dut, a, regs=[2, 3, 4, 5, 6, 8, 9, 10], data={0x8000: [0x92341234]})
    assert tb.values == [0x92341234, 0xFFFFFF92, 0x92, 0x1234, 0x9234, 0xABCD, 0xCD00, 0xABCD]


@cocotb.test()
async def test_forwarding_and_load_use(dut):
    """Back-to-back dependencies, including a load followed by its use."""
    a = Asm()
    a.li(1, 5)
    a.add(2, 1, 1)
    a.add(3, 2, 1)
    a.add(4, 3, 2)
    a.li(5, 0x8000)
    a.lw(6, 5, 0)
    a.add(7, 6, 6)
    a.sw(7, 5, 4)
    a.lw(8, 5, 4)
    a.beq(8, 7, "ok")
    a.li(9, 1)
    a.label("ok")
    finish(a, [2, 3, 4, 7, 8])
    tb = await run_program(dut, a, regs=[2, 3, 4, 7, 8], data={0x8000: [21]})
    assert tb.values == [10, 15, 25, 42, 42]


@cocotb.test()
async def test_multiply_divide(dut):
    """Multiply and divide on ordinary operands."""
    a = Asm()
    a.li(1, 1234)
    a.li(2, 56)
    a.mul(3, 1, 2)
    a.div(4, 1, 2)
    a.rem(5, 1, 2)
    a.li(6, -1234)
    a.div(7, 6, 2)
    a.rem(8, 6, 2)
    a.mulhu(9, 1, 2)
    finish(a, [3, 4, 5, 7, 8, 9])
    tb = await run_program(dut, a, regs=[3, 4, 5, 7, 8, 9])
    assert tb.values == [69104, 22, 2, 0xFFFFFFEA, 0xFFFFFFFE, 0]


@cocotb.test()
async def test_csr_access(dut):
    """Read and write of machine CSRs."""
    a = Asm()
    a.li(1, 0x55AA)
    a.csrrw(2, MSCRATCH, 1)
    a.csrrs(3, MSCRATCH, 0)
    a.csrrs(4, MISA, 0)
    finish(a, [2, 3, 4])
    tb = await run_program(dut, a, regs=[2, 3, 4])
    assert tb.values == [0, 0x55AA, 0x40001100]


@cocotb.test()
async def test_ecall_trap(dut):
    """ECALL enters the trap handler and MRET returns after it."""
    a = Asm()
    a.la(1, "handler")
    a.csrrw(28, MTVEC, 1)
    a.li(2, 0)
    a.label("call")
    a.ecall()
    a.addi(2, 2, 1)
    finish(a, [2, 3, 4])
    a.label("handler")
    a.csrrs(3, MCAUSE, 0)
    a.csrrs(4, MEPC, 0)
    a.addi(4, 4, 4)
    a.csrrw(28, MEPC, 4)
    a.mret()
    tb = await run_program(dut, a, regs=[2, 3])
    assert tb.values == [1, 11]


@cocotb.test()
async def test_illegal_instruction(dut):
    """An illegal instruction traps with cause 2."""
    a = Asm()
    a.la(1, "handler")
    a.csrrw(28, MTVEC, 1)
    a.word(0xFFFFFFFF)
    a.label("handler")
    a.csrrs(3, MCAUSE, 0)
    finish(a, [3])
    tb = await run_program(dut, a, regs=[3])
    assert tb.values == [2]


@cocotb.test()
async def test_wait_states(dut):
    """The same dependencies with wait states on both memories."""
    a = Asm()
    a.li(1, 0x8000)
    a.lw(2, 1, 0)
    a.add(3, 2, 2)
    a.sw(3, 1, 4)
    a.lw(4, 1, 4)
    a.mul(5, 4, 4)
    a.addi(6, 5, 1)
    finish(a, [3, 4, 5, 6])
    tb = await run_program(dut, a, regs=[3, 4, 5, 6], data={0x8000: [11]}, istall=0.4, dstall=0.5)
    assert tb.values == [22, 22, 484, 485]
