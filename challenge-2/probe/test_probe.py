"""Probe tests for the four suspected seeded deviations in the released baseline.

Each test encodes the SPEC-required behavior. On the BASELINE RTL all four must
FAIL (that is the proof of the deviation); on the fixed RTL all must PASS.
P1 D1 REQ-24  equal-PRIO rotation: first channel after LAST_GRANT wins
P2 D2 REQ-18  LEN=0 descriptor: no data moved, no bus access, LEN stays 0
P3 D3 REQ-28/30  failing write response stops the channel with ERR (no DONE)
P4 D4 REQ-31  abort sets neither DONE nor ERR
"""

import cocotb
from cocotb.triggers import FallingEdge

from dma_tb import (ACTIVE, CH_CTRL, CH_DST, CH_LEN, CH_STAT, EN, GCTRL, ID, IRQ_STATUS,
                    DmaTB, ch, done_bit, err_bit, prio)


@cocotb.test()
async def p1_equal_priority_rotation(dut):
    """REQ-24: equal PRIO -> the FIRST eligible in rotating order (after reset,
    search starts at channel 0) gets the engine. Both channels must be eligible
    BEFORE GEN goes up, so the very first grant is the rotating-order decision."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [1, 2, 3, 4])
    tb.mem.load(0x2000, [5, 6, 7, 8])
    # identical PRIO=1 for both channels; program first, enable last
    await tb.program(0, 0x1000, 0x8000, 4, EN | prio(1))
    await tb.program(1, 0x2000, 0x9000, 4, EN | prio(1))
    await tb.write(GCTRL, 1)
    while len(tb.mem.writes()) < 8:
        await FallingEdge(dut.clk)
    first_dst = tb.mem.writes()[0][1]
    assert first_dst == 0x8000, f"ch0 must win first grant after reset (start at 0); got 0x{first_dst:04X}"
    tb.check_bus()


@cocotb.test()
async def p2_zero_length_descriptor(dut):
    """REQ-18: LEN=0 ends at once on grant: no data access, no write, LEN stays 0,
    DONE set (normal completion, not an error)."""
    tb = DmaTB(dut)
    await tb.start()
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 0, EN)
    await tb.wait_idle()
    assert tb.mem.log == [], f"LEN=0 made bus accesses: {tb.mem.log[:4]}"
    assert await tb.read(ch(0, CH_LEN)) == 0, "CH_LEN must stay 0"
    assert (await tb.read(IRQ_STATUS)) & 0xFF == done_bit(0), "DONE must be set"
    assert await tb.read(ch(0, CH_STAT)) == 0b100


@cocotb.test()
async def p3_write_response_error(dut):
    """REQ-28/30: a non-OKAY m_bresp stops the channel: ERR set, DONE clear,
    SRC/DST/LEN keep the values of the failed word, chain stops."""
    tb = DmaTB(dut)
    await tb.start()
    await tb.write(GCTRL, 1)
    tb.mem.load(0x1000, [1, 2, 3])
    tb.mem.write_err.add(0x2000)  # first written word fails at the RESPONSE
    await tb.program(0, 0x1000, 0x2000, 3, EN)
    await tb.wait_idle()
    irq = (await tb.read(IRQ_STATUS)) & 0xFF
    assert irq == err_bit(0), f"expect ERR only, IRQ_STATUS[7:0]=0b{irq:08b}"
    assert await tb.read(ch(0, CH_DST)) == 0x2000, "CH_DST must keep the failed word's value"
    assert await tb.read(ch(0, CH_LEN)) == 3, "CH_LEN must keep the failed word's value"
    assert await tb.read(ch(0, CH_STAT)) == 0b010


@cocotb.test()
async def p4_abort_sets_nothing(dut):
    """REQ-31: EN 1->0 while the engine HOLDS the channel (mid-burst): the word in
    progress finishes, no further word starts, DONE and ERR NOT set."""
    tb = DmaTB(dut, stall=0.5)
    await tb.start()
    await tb.write(GCTRL, 1 | 2 << 1)  # BURST=2 -> 4 words per grant
    tb.mem.load(0x1000, [1, 2, 3, 4, 5, 6, 7, 8])
    await tb.program(0, 0x1000, 0x2000, 8, EN)
    while len(tb.mem.writes()) < 1:
        await FallingEdge(dut.clk)
    await tb.write(ch(0, CH_CTRL), 0)  # abort mid-burst (engine still owns ch0)
    await tb.wait_idle()
    n_after = len(tb.mem.writes())
    assert 1 <= n_after <= 2, f"words continued after abort: {n_after}"
    assert (await tb.read(IRQ_STATUS)) & 0xFF == 0, "abort must not set DONE or ERR"
    assert await tb.read(ch(0, CH_STAT)) == 0, "CH_STAT must be clean after abort"
    assert await tb.read(ch(0, CH_CTRL)) & EN == 0
    tb.check_bus()


@cocotb.test()
async def p5_hwreq_chain_dack_count(dut):
    """REQ-27: dack[n] pulses ONCE after each burst that moved >=1 word ends.
    A HW_REQ chain of 3 one-word descriptors (programmed + 2 fetched) is 3 bursts
    -> exactly 3 dack pulses. Baseline pulses at S_END *and* S_RELEASE for
    chain-continuing descriptors -> 5 pulses (D5)."""
    tb = DmaTB(dut)
    await tb.start()
    await tb.write(GCTRL, 1)
    tb.mem.load(0x1000, [0xA1])
    tb.mem.load(0x1100, [0xA2])
    tb.mem.load(0x1200, [0xA3])
    # descriptor tables: src, dst, len, next
    tb.mem.load(0x3000, [0x1100, 0x2100, 1, 0x3010])
    tb.mem.load(0x3010, [0x1200, 0x2200, 1, 0])
    await tb.program(0, 0x1000, 0x2000, 1, EN | 0x40 | 0x08, 0x3000)  # EN|LINK|HW_REQ
    dut.dreq.value = 1  # hold the request through the whole chain
    await tb.wait_idle()
    dut.dreq.value = 0
    n = tb.mem.dack_count[0]
    assert tb.mem.dump(0x2000, 1) == [0xA1] and tb.mem.dump(0x2100, 1) == [0xA2] \
        and tb.mem.dump(0x2200, 1) == [0xA3], "chain did not complete"
    assert n == 3, f"expected 3 dack pulses (one per burst), got {n}"
