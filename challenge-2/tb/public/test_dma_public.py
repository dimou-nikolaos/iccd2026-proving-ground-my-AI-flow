"""Public testbench for dma_ctrl.

These tests pass on the released baseline. They are a starting point, not a
complete verification plan: passing them does not mean the design meets its
specification.
"""

import cocotb

from dma_tb import (ACTIVE, CH_CTRL, CH_DST, CH_LEN, CH_SRC, CH_STAT, DST_FIX, EN, GCTRL, HW_REQ, ID,
                    IRQ_ENABLE, IRQ_STATUS, LINK, SRC_FIX, DmaTB, ch, done_bit, err_bit, peripheral, prio)


async def setup(dut, stall=0.0, burst=0):
    tb = DmaTB(dut, stall=stall)
    await tb.start()
    await tb.write(GCTRL, 1 | burst << 1)
    return tb


@cocotb.test()
async def test_id_and_registers(dut):
    """Identifier and channel register read-back."""
    tb = DmaTB(dut)
    await tb.start()
    assert await tb.read(ID) == 0x444D0100
    assert tb.last_err == 0
    await tb.write(ch(1, CH_SRC), 0x1000)
    await tb.write(ch(1, CH_DST), 0x2000)
    await tb.write(ch(1, CH_LEN), 16)
    assert await tb.read(ch(1, CH_SRC)) == 0x1000
    assert await tb.read(ch(1, CH_DST)) == 0x2000
    assert await tb.read(ch(1, CH_LEN)) == 16
    assert await tb.read(ch(0, CH_SRC)) == 0
    await tb.read(0x0FC)
    assert tb.last_err == 1


@cocotb.test()
async def test_memory_copy(dut):
    """A plain copy with a done interrupt."""
    tb = await setup(dut)
    data = [0x11110000 + i for i in range(10)]
    tb.mem.load(0x1000, data)
    await tb.write(IRQ_ENABLE, done_bit(0))
    await tb.program(0, 0x1000, 0x2000, len(data), EN)
    await tb.wait_idle()
    assert tb.mem.dump(0x2000, len(data)) == data
    assert (await tb.read(IRQ_STATUS)) & 0xFF == done_bit(0)
    assert int(dut.irq.value) == 1
    assert await tb.read(ch(0, CH_LEN)) == 0
    assert await tb.read(ch(0, CH_STAT)) == 0b100
    await tb.write(IRQ_STATUS, done_bit(0))
    assert int(dut.irq.value) == 0
    tb.check_bus()


@cocotb.test()
async def test_copy_with_stalls_and_bursts(dut):
    """Bus stalls and a burst setting of four words."""
    tb = await setup(dut, stall=0.5, burst=2)
    data = [0xA5000000 + 3 * i for i in range(25)]
    tb.mem.load(0x4000, data)
    await tb.program(2, 0x4000, 0x5000, len(data), EN)
    await tb.wait_idle()
    assert tb.mem.dump(0x5000, len(data)) == data
    tb.check_bus()


@cocotb.test()
async def test_fixed_addresses(dut):
    """Fixed source and fixed destination modes."""
    tb = await setup(dut)
    tb.mem.load(0x100, [7])
    await tb.program(0, 0x100, 0x200, 3, EN | SRC_FIX)
    await tb.wait_idle()
    assert tb.mem.dump(0x200, 3) == [7, 7, 7]
    tb.mem.load(0x300, [1, 2, 3])
    await tb.program(0, 0x300, 0x400, 3, EN | DST_FIX)
    await tb.wait_idle()
    assert tb.mem.dump(0x400, 1) == [3]
    assert [e[1] for e in tb.mem.writes()[-3:]] == [0x400, 0x400, 0x400]


@cocotb.test()
async def test_priority(dut):
    """The higher priority channel runs first, whichever order they start in."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [1, 2, 3, 4])
    tb.mem.load(0x2000, [5, 6, 7, 8])
    await tb.program(0, 0x1000, 0x8000, 4, EN | prio(0))
    await tb.program(1, 0x2000, 0x9000, 4, EN | prio(3))
    await tb.write(GCTRL, 1)
    await tb.wait_idle()
    writes = [e[1] for e in tb.mem.writes()]
    assert writes == [0x9000, 0x9004, 0x9008, 0x900C, 0x8000, 0x8004, 0x8008, 0x800C]


@cocotb.test()
async def test_hardware_request(dut):
    """A channel with HW_REQ moves a burst for each request and acknowledges it."""
    tb = await setup(dut)
    data = [100, 200, 300]
    tb.mem.load(0x1000, data)
    await tb.program(3, 0x1000, 0x2000, 3, EN | HW_REQ)
    for _ in range(20):
        await tb.read(ACTIVE)
    assert tb.mem.writes() == []
    await peripheral(tb, 3, 3)
    await tb.wait_idle()
    assert tb.mem.dump(0x2000, 3) == data
    assert tb.mem.dack_count[3] == 3


@cocotb.test()
async def test_linked_descriptors(dut):
    """A chain of two descriptors after the programmed block."""
    tb = await setup(dut)
    tb.mem.load(0x1000, [1, 2])
    tb.mem.load(0x1100, [3, 4, 5])
    tb.mem.load(0x1200, [6])
    tb.mem.load(0x3000, [0x1100, 0x2100, 3, 0x3010])
    tb.mem.load(0x3010, [0x1200, 0x2200, 1, 0])
    await tb.write(IRQ_ENABLE, done_bit(1))
    await tb.program(1, 0x1000, 0x2000, 2, EN | LINK, 0x3000)
    await tb.wait_idle()
    assert tb.mem.dump(0x2000, 2) == [1, 2]
    assert tb.mem.dump(0x2100, 3) == [3, 4, 5]
    assert tb.mem.dump(0x2200, 1) == [6]
    assert (await tb.read(IRQ_STATUS)) & 0xFF == done_bit(1)


@cocotb.test()
async def test_read_error(dut):
    """A read error stops the channel and reports it."""
    tb = await setup(dut)
    tb.mem.load(0x1000, [1, 2, 3, 4])
    tb.mem.read_err.add(0x1008)
    await tb.program(0, 0x1000, 0x2000, 4, EN)
    await tb.wait_idle()
    assert tb.mem.dump(0x2000, 4) == [1, 2, 0, 0]
    assert (await tb.read(IRQ_STATUS)) & 0xFF == err_bit(0)
    assert await tb.read(ch(0, CH_STAT)) == 0b010
    assert await tb.read(ch(0, CH_CTRL)) & EN == 0
    assert await tb.read(ch(0, CH_DST)) == 0x2008
