"""Interface-legality suite: APB4 completer rules, AXI4-Lite master rules, and
the dreq/dack peripheral handshake. Each test docstring carries its REQ tag(s)."""

import cocotb
from cocotb.triggers import FallingEdge, Timer

from dma_tb import (ACTIVE, CH_CTRL, CH_DST, CH_LEN, CH_NEXT, CH_SRC, CH_STAT,
                    DESC_BIT, EN, GCTRL, HW_REQ, ID, IRQ_ENABLE, IRQ_STATUS, LAST_GRANT,
                    SRC_FIX, DmaTB, ch, done_bit, err_bit, peripheral, prio)

MAPPED = [ID, GCTRL, IRQ_STATUS, IRQ_ENABLE, ACTIVE, LAST_GRANT] + \
         [ch(n, r) for n in range(4)
          for r in (CH_CTRL, CH_SRC, CH_DST, CH_LEN, CH_NEXT, CH_STAT)]


async def scan_pready(tb, cycles):
    """REQ-03: pready is 1 in every cycle (mem.run also flags any 0 globally)."""
    for _ in range(cycles):
        await FallingEdge(tb.dut.clk)
        assert int(tb.dut.pready.value) == 1


@cocotb.test()
async def test_req03_pready_always_ready(dut):
    """REQ-03: zero-wait-state completer: pready=1 in every cycle, idle and during
    traffic."""
    tb = DmaTB(dut)
    await tb.start()
    await scan_pready(tb, 10)
    tb.mem.load(0x1000, [1, 2])
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 2, EN)
    await scan_pready(tb, 20)
    await tb.wait_idle()
    tb.check_bus()


@cocotb.test()
async def test_req04_setup_and_psel0_no_effect(dut):
    """REQ-04: transfer occurs only in the access phase: a long-held setup phase
    (penable=0) has no effect, and psel=0 cycles with active pwrite/paddr/pwdata
    have no effect."""
    tb = DmaTB(dut)
    await tb.start()
    d = dut
    # held setup: EN=1 to ch0 CH_CTRL, penable never asserted
    d.psel.value = 1
    d.penable.value = 0
    d.pwrite.value = 1
    d.paddr.value = ch(0, CH_CTRL)
    d.pwdata.value = EN
    d.pstrb.value = 0xF
    await scan_pready(tb, 6)
    d.psel.value = 0
    d.pwdata.value = 0
    await tb.wait_cycles(2)
    assert await tb.read(ch(0, CH_CTRL)) == 0, "held setup phase must not write"
    assert await tb.read(ACTIVE) == 0, "held setup phase must not enable"
    # psel=0 cycles with write-looking inputs
    d.pwrite.value = 1
    d.paddr.value = ch(0, CH_LEN)
    d.pwdata.value = 0xBAD
    d.pstrb.value = 0xF
    await tb.wait_cycles(4)
    d.pwrite.value = 0
    assert await tb.read(ch(0, CH_LEN)) == 0, "psel=0 cycles must not write"
    tb.check_bus()


@cocotb.test()
async def test_req05_paddr_low_bits_ignored(dut):
    """REQ-05: decode uses paddr[11:2]; paddr[1:0] ignored (reads and writes;
    unmapped-ness also ignores them)."""
    tb = DmaTB(dut)
    await tb.start()
    for off in (0x000, 0x001, 0x002, 0x003):
        assert await tb.read(off) == 0x444D0100, f"ID read at 0x{off:03X}"
    await tb.write(ch(1, CH_LEN) + 1, 0x5678)
    await tb.write(ch(1, CH_LEN) + 2, 0x0000, 0x0)  # same register, strb=0
    assert await tb.read(ch(1, CH_LEN)) == 0x5678
    assert await tb.read(ch(1, CH_LEN) + 3) == 0x5678
    for off in (0x018, 0x019, 0x01A, 0x01B):
        assert await tb.read(off) == 0 and tb.last_err == 1, f"0x{off:03X} must stay unmapped"
    tb.check_bus()


@cocotb.test()
async def test_req06_prdata_and_undef_bits(dut):
    """REQ-06: prdata carries the register value only in the read access phase;
    0 outside it (setup cycles, idle, and a write access phase); undefined bits
    read 0 (CH_SRC/CH_DST/NEXT [1:0], CH_LEN [31:16])."""
    tb = DmaTB(dut)
    await tb.start()
    d = dut
    await tb.write(ch(0, CH_SRC), 0xFFFFFFFF)
    await tb.write(ch(0, CH_DST), 0xFFFFFFFF)
    await tb.write(ch(0, CH_LEN), 0xFFFFFFFF)
    await tb.write(ch(0, CH_NEXT), 0xFFFFFFFF)
    assert await tb.read(ch(0, CH_SRC)) == 0xFFFFFFFC, "CH_SRC [1:0] read 0"
    assert await tb.read(ch(0, CH_DST)) == 0xFFFFFFFC, "CH_DST [1:0] read 0"
    assert await tb.read(ch(0, CH_LEN)) == 0x0000FFFF, "CH_LEN [31:16] read 0"
    assert await tb.read(ch(0, CH_NEXT)) == 0xFFFFFFFC, "CH_NEXT [1:0] read 0"
    assert await tb.read(ID) == 0x444D0100
    # prdata=0 during the setup cycle of a read
    d.psel.value = 1
    d.penable.value = 0
    d.pwrite.value = 0
    d.paddr.value = ID
    await FallingEdge(d.clk)
    await Timer(1, unit="ns")
    assert int(d.prdata.value) == 0, "prdata must be 0 during setup phase"
    d.penable.value = 1
    await Timer(1, unit="ns")
    assert int(d.prdata.value) == 0x444D0100, "prdata valid in read access phase"
    await FallingEdge(d.clk)
    d.psel.value = 0
    d.penable.value = 0
    await Timer(1, unit="ns")
    assert int(d.prdata.value) == 0, "prdata must be 0 after the transaction"
    # prdata=0 during a WRITE access phase
    d.psel.value = 1
    d.pwrite.value = 1
    d.paddr.value = ch(1, CH_LEN)
    d.pwdata.value = 7
    d.pstrb.value = 0xF
    await FallingEdge(d.clk)
    d.penable.value = 1
    await Timer(1, unit="ns")
    assert int(d.prdata.value) == 0, "prdata must be 0 during a write access phase"
    await FallingEdge(d.clk)
    d.psel.value = 0
    d.penable.value = 0
    d.pwrite.value = 0
    assert await tb.read(ch(1, CH_LEN)) == 7
    tb.check_bus()


@cocotb.test()
async def test_req07_unmapped_accesses(dut):
    """REQ-07: unmapped offsets complete with pslverr=1, reads give 0, writes have
    no effect; 0x118+0x20n and 0x11C+0x20n are unmapped; every mapped offset
    completes with pslverr=0."""
    tb = DmaTB(dut)
    await tb.start()
    # program ch0 so a mis-decode of an unmapped write would be visible
    await tb.program(0, 0x1000, 0x2000, 3, EN | prio(1))
    before = [await tb.read(a) for a in MAPPED]
    unmapped = [0x018, 0x01C, 0x020, 0x0FC, 0x118, 0x11C, 0x138, 0x13C,
                0x158, 0x15C, 0x178, 0x17C, 0x180, 0x1FC, 0x400, 0xFFC]
    for a in unmapped:
        assert await tb.read(a) == 0, f"unmapped read 0x{a:03X} must return 0"
        assert tb.last_err == 1, f"unmapped read 0x{a:03X} must set pslverr"
    for a in unmapped:
        await tb.write(a, 0xDEADBEEF)
        assert tb.last_err == 1, f"unmapped write 0x{a:03X} must set pslverr"
    after = [await tb.read(a) for a in MAPPED]
    assert before == after, "unmapped writes must have no effect on mapped registers"
    for a in MAPPED:
        await tb.read(a)
        assert tb.last_err == 0, f"mapped read 0x{a:03X} must have pslverr=0"
    await tb.write(IRQ_ENABLE, 0)
    assert tb.last_err == 0, "mapped write must have pslverr=0"
    tb.check_bus()


@cocotb.test()
async def test_req08_pstrb_byte_lanes(dut):
    """REQ-08: writes update only strobed byte lanes, for CH_SRC/CH_DST/CH_LEN/
    CH_NEXT/CH_CTRL (EN bit via lane 0), GCTRL and IRQ_ENABLE; strb=0 changes
    nothing."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0x33])
    # byte-lane writes on CH_SRC (channel 1, not started)
    await tb.write(ch(1, CH_SRC), 0xAABBCCDD, 0x4)  # lane 2 -> bits [23:16]
    assert await tb.read(ch(1, CH_SRC)) == 0x00BB0000, "lane-2-only write"
    await tb.write(ch(1, CH_SRC), 0x11223344, 0xA)  # lanes 1,3
    assert await tb.read(ch(1, CH_SRC)) == 0x11BB3300, "lanes 1+3 merge with lane 2"
    await tb.write(ch(0, CH_LEN), 0xFFFFFFFF, 0x0)
    assert await tb.read(ch(0, CH_LEN)) == 0, "strb=0 write must do nothing"
    await tb.write(ch(2, CH_DST), 0x12345678, 0xF)
    assert await tb.read(ch(2, CH_DST)) == 0x12345678
    # CH_CTRL EN bit via lane 0 only: starts the channel, PRIO from the same write
    await tb.program(0, 0x1000, 0x2000, 1, 0)
    await tb.write(GCTRL, 1)
    await tb.write(ch(0, CH_CTRL), EN | prio(1), 0x1)
    await tb.wait_idle()
    assert tb.mem.dump(0x2000, 1) == [0x33], "EN write via lane 0 started the transfer"
    assert await tb.read(ch(0, CH_CTRL)) == prio(1), "PRIO from lane-0 write kept"
    # GCTRL: GEN via lane 0
    await tb.write(GCTRL, 0, 0x1)
    assert await tb.read(GCTRL) == 0
    # IRQ_ENABLE byte lanes (lane 1 covers CR-D2 bits [11:8])
    await tb.write(IRQ_ENABLE, 0x000000F0, 0x1)
    assert await tb.read(IRQ_ENABLE) == 0xF0
    await tb.write(IRQ_ENABLE, 0xFF00, 0x2)
    assert await tb.read(IRQ_ENABLE) == 0xF0 | 0xF00, "IRQ_ENABLE lane behavior"
    tb.check_bus()


@cocotb.test()
async def test_req09_ro_write_and_undef_write(dut):
    """REQ-09: writes to read-only registers (ID, ACTIVE, LAST_GRANT, CH_STAT)
    complete with pslverr=0 and have no effect; writes to undefined bits of
    writable registers are ignored (CH_LEN[31:16], CH_CTRL[31:7])."""
    tb = DmaTB(dut)
    await tb.start()
    for a in (ID, ACTIVE, LAST_GRANT, ch(0, CH_STAT), ch(3, CH_STAT)):
        await tb.write(a, 0xFFFFFFFF)
        assert tb.last_err == 0, f"RO write 0x{a:03X} must complete with pslverr=0"
    assert await tb.read(ID) == 0x444D0100
    assert await tb.read(LAST_GRANT) == 3
    assert await tb.read(ACTIVE) == 0
    await tb.write(ch(0, CH_LEN), 0xFFFFFFFF)
    assert await tb.read(ch(0, CH_LEN)) == 0xFFFF, "undefined LEN bits ignore writes"
    await tb.write(ch(0, CH_CTRL), 0xFFFFFF80)  # ones in [31:7], zeros in [6:0]
    assert await tb.read(ch(0, CH_CTRL)) == 0, "undefined CTRL bits ignore writes"
    tb.check_bus()


@cocotb.test()
async def test_req14_valid_hold_under_stalls(dut):
    """REQ-14: while an AXI valid output is 1 and its ready is 0, valid and
    payload hold: done with heavy random stalls and a hard freeze of the read
    address channel mid-transaction."""
    tb = DmaTB(dut, stall=0.7)
    await tb.start()
    tb.mem.load(0x1000, [1, 2])
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 2, EN)
    # freeze the AR channel for a while mid-transaction
    await tb.wait_cycles(4)
    tb.mem.block = {"ar"}
    await tb.wait_cycles(30)
    tb.mem.block = set()
    await tb.wait_idle()
    assert tb.mem.dump(0x2000, 2) == [1, 2]
    tb.check_bus()


@cocotb.test()
async def test_req12_wstrb_and_alignment(dut):
    """REQ-12: m_wstrb=4'hF for every write and every AXI address word aligned
    (checked over a full multi-word transfer with stalls)."""
    tb = DmaTB(dut, stall=0.4)
    await tb.start()
    tb.mem.load(0x8000, [0x11111111, 0x22222222, 0x33333333])
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x8000, 0x9000, 3, EN)
    await tb.wait_idle()
    tb.mem.check_aligned_strb()
    assert all(e[3] == 0xF for e in tb.mem.writes())
    assert tb.mem.dump(0x9000, 3) == [0x11111111, 0x22222222, 0x33333333]
    tb.check_bus()


@cocotb.test()
async def test_req24_27_dreq_dack_handshake(dut):
    """REQ-24/REQ-27: HW_REQ pacing: nothing happens while dreq=0; the peripheral
    holds dreq[n] until dack[n] pulses one cycle (peripheral helper), once per
    burst; dack only pulses for bursts that moved >=1 word."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0x91, 0x92, 0x93])
    await tb.program(0, 0x1000, 0x2000, 3, EN | HW_REQ)
    await tb.write(GCTRL, 1)  # BURST=0; dreq still 0
    await tb.wait_cycles(30)
    assert tb.mem.log == [], "HW_REQ channel must not run without dreq"
    assert int(dut.dack.value) == 0
    tb.mem.block = set()
    await peripheral(tb, 0, 3)  # hold dreq until each dack
    await tb.wait_idle()
    assert tb.mem.dump(0x2000, 3) == [0x91, 0x92, 0x93]
    assert tb.mem.dack_count[0] == 3
    assert tb.mem.dack_width[0] == 1
    tb.check_bus()


@cocotb.test()
async def test_req13_awvalid_arvalid_exclusive(dut):
    """REQ-13: m_arvalid and m_awvalid are never both 1: checked continuously
    while a stalled transfer crawls through the engine (checker inside mem.run)."""
    tb = DmaTB(dut, stall=0.6)
    await tb.start()
    tb.mem.load(0x1000, list(range(5)))
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 5, EN)
    await tb.wait_idle()
    assert len(tb.mem.violations) == 0, tb.mem.violations[:3]
    tb.check_bus()
