"""Outstanding-transaction evidence: reset asserted in the middle of AXI
transactions (late responses must not be latched), GEN cleared mid-burst, and
abort arriving while the bus is stalled mid-word. Docstrings carry REQ tags."""

import cocotb
from cocotb.triggers import FallingEdge, Timer

from dma_tb import (ACTIVE, CH_CTRL, CH_DST, CH_LEN, CH_NEXT, CH_SRC, CH_STAT,
                    EN, GCTRL, ID, IRQ_ENABLE, IRQ_STATUS, LAST_GRANT,
                    DmaTB, ch, done_bit, err_bit)


async def check_dut_quiet(tb, cycles):
    """All DUT AXI/dack/irq outputs must stay 0 for `cycles` cycles."""
    d = tb.dut
    for _ in range(cycles):
        await FallingEdge(d.clk)
        await Timer(1, unit="ns")
        assert int(d.m_arvalid.value) == 0, "m_arvalid not 0 after reset"
        assert int(d.m_awvalid.value) == 0, "m_awvalid not 0 after reset"
        assert int(d.m_wvalid.value) == 0, "m_wvalid not 0 after reset"
        assert int(d.dack.value) == 0, "dack not 0 after reset"
        assert int(d.irq.value) == 0, "irq not 0 after reset"


async def check_reset_regs(tb):
    """REQ-02: all registers at reset values after the mid-transaction reset."""
    assert await tb.read(ID) == 0x444D0100
    assert await tb.read(GCTRL) == 0
    assert await tb.read(IRQ_STATUS) == 0
    assert await tb.read(IRQ_ENABLE) == 0
    assert await tb.read(ACTIVE) == 0
    assert await tb.read(LAST_GRANT) == 3
    for n in range(4):
        assert await tb.read(ch(n, CH_CTRL)) == 0
        assert await tb.read(ch(n, CH_LEN)) == 0
        assert await tb.read(ch(n, CH_STAT)) == 0


@cocotb.test()
async def test_out01_reset_after_ar_before_r(dut):
    """REQ-02/REQ-13: rst_n LOW after the AR handshake while the R response is
    still pending; late m_rvalid arriving after reset must not be latched: no
    write for that word is ever issued, DUT valids stay 0, registers hold reset
    values, irq/dack 0."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0xA0, 0xA1])
    tb.mem.block = {"r"}  # R response never given while frozen
    await tb.write(GCTRL, 1)
    await tb.write(IRQ_ENABLE, 0xFF)
    await tb.program(0, 0x1000, 0x2000, 2, EN)
    while len(tb.mem.reads()) < 1:  # AR accepted, response pending
        await FallingEdge(dut.clk)
    tb.mem.keep_pend = True  # slave keeps the pending read across the reset
    await tb.reset()
    tb.mem.block = set()  # unblock AFTER reset: the R response now arrives late
    await tb.wait_cycles(3)
    assert int(dut.m_rvalid.value) == 1, "slave should still deliver the late response"
    await Timer(1, unit="ns")
    assert int(dut.m_rready.value) == 0, "reset DUT must not accept the late response"
    mark = len(tb.mem.log)
    await check_dut_quiet(tb, 15)
    assert tb.mem.log[mark:] == [], f"post-reset bus activity: {tb.mem.log[mark:]}"
    assert tb.mem.dump(0x2000, 2) == [0, 0], "no memory writes after mid-read reset"
    await check_reset_regs(tb)
    tb.check_bus()


@cocotb.test()
async def test_out02_reset_after_aww_before_b(dut):
    """REQ-02/REQ-13: rst_n LOW after AW+W handshakes, before the B response; the
    late m_bvalid must not be latched: no next word starts, DUT quiet, registers
    at reset values. (The slave's write of the accepted word is a bus-side effect,
    not a DUT action.)"""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0xB0, 0xB1])
    tb.mem.block = {"b"}  # B response withheld
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 2, EN)
    while len(tb.mem.writes()) < 1:  # AW+W accepted, B pending
        await FallingEdge(dut.clk)
    mark = len(tb.mem.log)
    tb.mem.keep_pend = True
    await tb.reset()
    tb.mem.block = set()  # the B response now arrives late
    await tb.wait_cycles(3)
    assert int(dut.m_bvalid.value) == 1, "slave should still deliver the late response"
    await Timer(1, unit="ns")
    assert int(dut.m_bready.value) == 0, "reset DUT must not accept the late response"
    await check_dut_quiet(tb, 15)
    assert tb.mem.log[mark:] == [], f"post-reset bus activity: {tb.mem.log[mark:]}"
    assert tb.mem.dump(0x2004, 1) == [0], "second word must not be written"
    await check_reset_regs(tb)
    tb.check_bus()


@cocotb.test()
async def test_out03_reset_during_ar_stall_then_recover(dut):
    """REQ-02/REQ-14: rst_n LOW while m_arvalid is held with m_arready=0 (stalled
    mid-handshake): valid+payload held during the stall, then reset: valids drop,
    nothing is latched (no accesses at all), and the block works normally after."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0xC0])
    tb.mem.block = {"ar"}
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 1, EN)
    await tb.wait_cycles(12)  # arvalid+araddr held stable (checked by _stable)
    tb.check_bus()
    await tb.reset()
    tb.mem.block = set()
    await check_dut_quiet(tb, 15)
    assert tb.mem.log == [], "no accesses may complete from the aborted transaction"
    assert tb.mem.dump(0x2000, 1) == [0], "no memory write"
    await check_reset_regs(tb)
    # recovery: fresh transfer on the same interfaces
    tb.mem.load(0x1400, [0xC1])
    await tb.write(GCTRL, 1)
    await tb.program(1, 0x1400, 0x2400, 1, EN)
    await tb.wait_idle()
    assert tb.mem.dump(0x2400, 1) == [0xC1]
    tb.check_bus()


@cocotb.test()
async def test_req25_gen_cleared_mid_burst(dut):
    """REQ-25: clearing GEN stops new grants, but a burst in progress finishes:
    with BURST=2 and LEN=6, GEN=0 after the first word still finishes the current
    4-word burst, then nothing more runs until GEN goes back to 1."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, list(range(6)))
    await tb.write(GCTRL, 1 | 2 << 1)  # GEN=1, BURST=2 (4 words per burst)
    await tb.program(0, 0x1000, 0x2000, 6, EN)
    while len(tb.mem.writes()) < 1:
        await FallingEdge(dut.clk)
    await tb.write(GCTRL, 2 << 1)  # GEN=0 mid-burst (BURST preserved)
    await tb.wait_cycles(80)
    n = len(tb.mem.writes())
    assert n == 4, f"burst in progress must finish (4 words), got {n}"
    await tb.wait_cycles(60)
    assert len(tb.mem.writes()) == n, f"new grant despite GEN=0: {len(tb.mem.writes())} > {n}"
    assert await tb.read(ACTIVE) == 1, "channel must still be enabled"
    assert await tb.read(ch(0, CH_LEN)) == 6 - n
    await tb.write(GCTRL, 1 | 2 << 1)  # resume
    await tb.wait_idle()
    assert tb.mem.dump(0x2000, 6) == list(range(6))
    assert (await tb.read(IRQ_STATUS)) & 0xFF == done_bit(0)
    tb.check_bus()


@cocotb.test()
async def test_req31_abort_stalled_midword(dut):
    """REQ-31: abort (EN 1->0) arriving while the bus is stalled mid-word (AW/W
    handshake pending): the word in progress finishes, no further word starts,
    and DONE/ERR are not set."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0xD0, 0xD1, 0xD2])
    tb.mem.block = {"aw", "w"}  # stall the write phase of word 1
    await tb.write(GCTRL, 1 | 1 << 1)  # BURST=1 -> 2 words per burst
    await tb.program(0, 0x1000, 0x2000, 3, EN)
    while len(tb.mem.reads()) < 1:
        await FallingEdge(dut.clk)
    await tb.wait_cycles(6)  # engine parked mid-word, awvalid/wvalid held
    tb.check_bus()
    await tb.write(ch(0, CH_CTRL), 0)  # abort while stalled
    await tb.wait_cycles(2)
    tb.mem.block = set()  # let the in-progress word finish
    await tb.wait_idle()
    assert tb.mem.log == [("R", 0x1000), ("W", 0x2000, 0xD0, 0xF)], \
        f"abort must finish exactly the in-progress word: {tb.mem.log}"
    assert (await tb.read(IRQ_STATUS)) & 0xFF == 0, "abort sets no DONE/ERR"
    assert await tb.read(ch(0, CH_STAT)) == 0
    assert await tb.read(ch(0, CH_CTRL)) & EN == 0
    tb.check_bus()
