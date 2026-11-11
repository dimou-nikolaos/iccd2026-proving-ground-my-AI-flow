"""CR-D2 acceptance suite: DESC[n] event (IRQ_STATUS[11:8] W1C, IRQ_ENABLE[11:8]
RW). Each test docstring carries REQ-36 plus the spec REQs it exercises."""

import cocotb
from cocotb.triggers import FallingEdge

from dma_tb import (ACTIVE, CH_CTRL, CH_DST, CH_LEN, CH_NEXT, CH_SRC, CH_STAT,
                    DESC_BIT, EN, GCTRL, HW_REQ, IRQ_ENABLE, IRQ_STATUS, LINK,
                    DmaTB, ch, desc_bit, done_bit, err_bit, prio,
                    race_fire, race_start)

PRE = 3


@cocotb.test()
async def test_cr1_chain_sets_desc_each_done_once(dut):
    """REQ-36/REQ-20: a chain of two descriptors sets DESC[n] when each completes;
    DONE[n] is set only at the end of the second. Observed mid-chain (after the
    first descriptor's data write, before the chain ends): DESC set, DONE clear."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0xE1])
    tb.mem.load(0x1100, [0xE2])
    tb.mem.load(0x3000, [0x1100, 0x2100, 1, 0])
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 1, EN | LINK, 0x3000)
    # wait until the SECOND descriptor's fetch completed (its data read issued)
    while not any(e[0] == "R" and e[1] == 0x1100 for e in tb.mem.log):
        await FallingEdge(dut.clk)
    mid = await tb.read(IRQ_STATUS)
    assert mid & desc_bit(0), "DESC0 set at first descriptor completion"
    assert not (mid & done_bit(0)), "DONE must not be set mid-chain"
    await tb.wait_idle()
    end = await tb.read(IRQ_STATUS)
    assert end & desc_bit(0), "DESC0 still set"
    assert end & done_bit(0), "DONE set only at chain end"
    assert (end & 0xF00) == desc_bit(0), "only DESC0 among the DESC bits"
    tb.check_bus()


@cocotb.test()
async def test_cr2_fetch_fail_desc_plus_err(dut):
    """REQ-36/REQ-28: if the fetch of the next descriptor fails, DESC[n] is set
    for the descriptor that completed, ERR[n] is set, and there is no DONE[n]."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0xE1])
    tb.mem.read_err.add(0x3000)  # first descriptor-table word fails
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 1, EN | LINK, 0x3000)
    await tb.wait_idle()
    s = await tb.read(IRQ_STATUS)
    assert s & desc_bit(0), "DESC0 must be set for the completed descriptor"
    assert s & err_bit(0), "ERR0 set by fetch failure"
    assert not (s & done_bit(0)), "no DONE on fetch failure"
    assert await tb.read(ch(0, CH_STAT)) == 0b010
    tb.check_bus()


@cocotb.test()
async def test_cr3_zero_length_sets_desc(dut):
    """REQ-36/REQ-18: a zero-length descriptor sets DESC[n] (and DONE[n]) at
    grant; no bus access."""
    tb = DmaTB(dut)
    await tb.start()
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 0, EN)
    await tb.wait_idle()
    assert tb.mem.log == []
    s = await tb.read(IRQ_STATUS)
    assert s & desc_bit(0), "DESC0 set by zero-length completion"
    assert s & done_bit(0)
    tb.check_bus()


@cocotb.test()
async def test_cr4_desc_w1c_respects_lane1(dut):
    """REQ-36/REQ-33: DESC bits lie in byte lane 1: a W1C write of bit 8 with
    pstrb=0x1 (lane 0) does NOT clear DESC0; with pstrb=0x2 it does. DONE bits in
    lane 0 are unaffected by the lane-1 write."""
    tb = DmaTB(dut)
    await tb.start()
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 0, EN)  # zero-length: DESC0 + DONE0
    await tb.wait_idle()
    s = await tb.read(IRQ_STATUS)
    assert (s & 0xF00) == desc_bit(0) and (s & done_bit(0)), "DESC0 + DONE0 set"
    await tb.write(IRQ_STATUS, desc_bit(0) | done_bit(0), 0x1)  # wrong lane for DESC
    s = await tb.read(IRQ_STATUS)
    assert s & desc_bit(0), "lane-0 strobe must not clear DESC0"
    assert not (s & done_bit(0)), "lane-0 strobe with written bit0 clears DONE0"
    await tb.write(IRQ_STATUS, desc_bit(0), 0x2)  # right lane
    s = await tb.read(IRQ_STATUS)
    assert not (s & 0xF00), "lane-1 strobe clears DESC0"
    tb.check_bus()


@cocotb.test()
async def test_cr5_desc_set_beats_clear(dut):
    """REQ-36/REQ-34: an event setting DESC0 in the same cycle as a W1C (lane 1)
    clears it leaves DESC0=1. Same race mechanism as test_req34, aimed at the
    DESC bit; the one-cycle-later leg must clear (sanity)."""
    tb = DmaTB(dut)
    await tb.start()
    await race_start(tb, 0, 0x1000, 0x2000)
    rb0, k0, _ = await race_fire(tb, PRE, 0)
    assert rb0 & desc_bit(0), "DESC0 set on completion (CR-D2)"
    ek = k0 - tb.mem.b_accept
    await race_start(tb, 1, 0x1010, 0x2010)
    rb1, k1, j1 = await race_fire(tb, PRE, 0, desc_bit(1), 0x2)
    assert k1 - tb.mem.b_accept == ek
    if j1 + 1 < k1:
        assert rb1 & desc_bit(1), "write before event: DESC stays set afterwards"
    else:
        assert j1 + 1 == k1 and rb1 & desc_bit(1), "zero-offset write landed on the event edge"
    delta = k1 - j1 - 1
    assert delta >= 0
    await race_start(tb, 2, 0x1020, 0x2020)
    rb2, k2, j2 = await race_fire(tb, PRE, delta, desc_bit(2), 0x2)
    assert j2 + 1 == k2, f"race mis-aimed: write edge {j2 + 1}, event edge {k2}"
    assert rb2 & desc_bit(2), "DESC set must beat same-cycle W1C"
    assert rb2 & done_bit(2), "DONE (unset by the lane-1 write) stays set"
    await race_start(tb, 3, 0x1030, 0x2030)
    rb3, k3, j3 = await race_fire(tb, PRE, delta + 1, desc_bit(3), 0x2)
    assert j3 + 1 == k3 + 1, f"sanity leg mis-aimed: write edge {j3 + 1}, event edge {k3}"
    assert not (rb3 & desc_bit(3)), "lane-1 W1C one cycle after the event must clear"
    assert rb3 & done_bit(3), "DONE untouched"
    tb.check_bus()


@cocotb.test()
async def test_cr6_desc_no_set_on_abort(dut):
    """REQ-36/REQ-31: DESC[n] is not set by a descriptor stopped by an abort:
    in a 2-descriptor chain aborted inside the second descriptor, DESC0 is set
    (first descriptor completed) but DESC must not be counted for the aborted
    one; no DONE/ERR."""
    tb = DmaTB(dut, stall=0.5)
    await tb.start()
    tb.mem.load(0x1000, [0xD1])
    tb.mem.load(0x1100, [0xD2])
    tb.mem.load(0x3000, [0x1100, 0x2100, 4, 0])  # 2nd descriptor: 4 words
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 1, EN | LINK, 0x3000)
    # wait for the second descriptor's FIRST write, then abort mid-burst
    while len(tb.mem.writes()) < 1:
        await FallingEdge(dut.clk)
    await tb.write(ch(0, CH_CTRL), 0)
    await tb.wait_idle()
    s = await tb.read(IRQ_STATUS)
    assert s & desc_bit(0), "DESC0 set for the completed first descriptor"
    assert not (s & done_bit(0)), "no DONE for an aborted chain"
    assert not (s & err_bit(0)), "no ERR for a clean abort"
    assert await tb.read(ch(0, CH_STAT)) == 0, "aborted channel: clean CH_STAT"
    tb.check_bus()


@cocotb.test()
async def test_cr7_irq_enable_desc_and_reset(dut):
    """REQ-36/REQ-35/REQ-01: IRQ_ENABLE[11:8] is RW (lane 1), DESC drives irq via
    (IRQ_STATUS AND IRQ_ENABLE); DESC and DESC_IE reset to 0."""
    tb = DmaTB(dut)
    await tb.start()
    assert await tb.read(IRQ_ENABLE) == 0
    assert await tb.read(IRQ_STATUS) == 0
    await tb.write(IRQ_ENABLE, 0xF00)
    assert await tb.read(IRQ_ENABLE) == 0xF00, "IRQ_ENABLE[11:8] RW"
    await tb.write(IRQ_ENABLE, 0)
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 0, EN)  # DESC0 + DONE0 set
    await tb.wait_idle()
    assert int(dut.irq.value) == 0, "irq 0 while DESC_IE/DONE_IE disabled"
    await tb.write(IRQ_ENABLE, 0x100)  # DESC_IE0 only
    assert int(dut.irq.value) == 1, "DESC0 AND DESC_IE0 -> irq 1"
    await tb.write(IRQ_STATUS, desc_bit(0), 0x2)
    assert int(dut.irq.value) == 0, "W1C on DESC drops irq"
    await tb.reset()
    assert await tb.read(IRQ_ENABLE) == 0, "DESC_IE resets to 0"
    assert await tb.read(IRQ_STATUS) == 0, "DESC resets to 0"
    assert int(dut.irq.value) == 0
    tb.check_bus()
