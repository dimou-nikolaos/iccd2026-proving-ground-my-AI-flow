"""Per-REQ functional evidence suite for dma_ctrl (spec rev 1.0 + CR-D2).

Every test docstring starts with its REQ tag(s). Tests are spec-driven; timing
inside the DUT (grant latency, burst scheduling beyond REQ bounds) is not
constrained by the spec, so tests assert on spec-visible ordering and content.

The four seeded-deviation probes (equal-priority rotation D1, LEN=0 D2, bresp
error D3, abort status D4) and the dack double-pulse probe (D5) are kept here
under spec-test names: they fail on baseline RTL and pass on fixed RTL.
"""

import cocotb
from cocotb.triggers import FallingEdge

from dma_tb import (ACTIVE, CH_CTRL, CH_DST, CH_LEN, CH_NEXT, CH_SRC, CH_STAT,
                    EN, DST_FIX, GCTRL, HW_REQ, ID, IRQ_ENABLE, IRQ_STATUS, LAST_GRANT,
                    LINK, SRC_FIX, DmaTB, ch, done_bit, err_bit, prio,
                    race_fire, race_start)

PRE = 3  # race_fire bus-freeze pre-delay (cycles), same in every race attempt


def wcount_since(tb, mark):
    return sum(1 for e in tb.mem.log[mark:] if e[0] == "W")


def raddrs(tb):
    return [e[1] for e in tb.mem.log if e[0] == "R"]


def waddrs(tb):
    return [e[1] for e in tb.mem.log if e[0] == "W"]


@cocotb.test()
async def test_req01_reset_values(dut):
    """REQ-01: every register field resets to 0 except ID and LAST_GRANT(=3)."""
    tb = DmaTB(dut)
    await tb.start()
    assert await tb.read(ID) == 0x444D0100
    assert await tb.read(GCTRL) == 0
    assert await tb.read(IRQ_STATUS) == 0
    assert await tb.read(IRQ_ENABLE) == 0
    assert await tb.read(ACTIVE) == 0
    assert await tb.read(LAST_GRANT) == 3
    for n in range(4):
        assert await tb.read(ch(n, CH_CTRL)) == 0
        assert await tb.read(ch(n, CH_SRC)) == 0
        assert await tb.read(ch(n, CH_DST)) == 0
        assert await tb.read(ch(n, CH_LEN)) == 0
        assert await tb.read(ch(n, CH_NEXT)) == 0
        assert await tb.read(ch(n, CH_STAT)) == 0
    assert int(dut.irq.value) == 0
    assert int(dut.dack.value) == 0
    tb.check_bus()


@cocotb.test()
async def test_req02_reset_after_config(dut):
    """REQ-02: reset any time; after: reset values, valids/dack/irq 0."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [1, 2, 3, 4])
    await tb.write(GCTRL, 1)
    await tb.write(IRQ_ENABLE, 0xFF)
    await tb.program(0, 0x1000, 0x2000, 4, EN | LINK, 0x3000)
    await tb.write(ch(1, CH_SRC), 0x9000)
    await tb.write(ch(1, CH_LEN), 7)
    await tb.write(ch(1, CH_CTRL), EN)
    await tb.wait_cycles(10)
    await tb.reset()
    assert await tb.read(ID) == 0x444D0100
    assert await tb.read(GCTRL) == 0
    assert await tb.read(IRQ_STATUS) == 0
    assert await tb.read(IRQ_ENABLE) == 0
    assert await tb.read(ACTIVE) == 0
    assert await tb.read(LAST_GRANT) == 3
    for n in range(4):
        assert await tb.read(ch(n, CH_LEN)) == 0
        assert await tb.read(ch(n, CH_CTRL)) == 0
        assert await tb.read(ch(n, CH_STAT)) == 0
    assert int(dut.irq.value) == 0
    for _ in range(5):
        await tb.wait_cycles(1)
        assert int(dut.dack.value) == 0
        assert int(dut.m_arvalid.value) == 0
        assert int(dut.m_awvalid.value) == 0
        assert int(dut.m_wvalid.value) == 0
    tb.check_bus()


@cocotb.test()
async def test_req10_en_start_clears_and_takes_bits(dut):
    """REQ-10: EN 0->1 starts a stopped channel, clears its ERR/DONE bits, and the
    other CH_CTRL bits take the value of the same write."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0xAA])
    tb.mem.read_err.add(0x1000)
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 1, EN)
    await tb.wait_idle()
    assert await tb.read(ch(0, CH_STAT)) == 0b010, "ERR set after failed word"
    assert (await tb.read(IRQ_STATUS)) & 0xFF == err_bit(0)
    tb.mem.read_err.clear()
    await tb.write(ch(0, CH_CTRL), EN | SRC_FIX)  # EN 0->1 plus SRC_FIX, same write
    assert await tb.read(ch(0, CH_CTRL)) == (EN | SRC_FIX), "other CTRL bits take same write"
    assert await tb.read(ch(0, CH_STAT)) & 0b110 == 0, "ERR/DONE cleared on start"
    assert (await tb.read(IRQ_STATUS)) & 0xFF == err_bit(0), "IRQ_STATUS sticky across restart"
    await tb.wait_idle()
    assert tb.mem.dump(0x2000, 1) == [0xAA]
    assert await tb.read(ch(0, CH_SRC)) == 0x1000, "SRC_FIX keeps source"
    assert await tb.read(ch(0, CH_STAT)) == 0b100
    tb.check_bus()


@cocotb.test()
async def test_req11_locked_while_enabled(dut):
    """REQ-11: while EN=1, writes to CH_SRC/DST/LEN/NEXT and CH_CTRL[6:1] have no
    effect (tested with GEN=0 so the channel is enabled but not granted)."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0x77])
    await tb.program(0, 0x1000, 0x2000, 1, EN | prio(1))
    await tb.write(ch(0, CH_SRC), 0xDEAD0000)
    await tb.write(ch(0, CH_DST), 0xBEEF0000)
    await tb.write(ch(0, CH_LEN), 9)
    await tb.write(ch(0, CH_NEXT), 0x3000)
    await tb.write(ch(0, CH_CTRL), EN | LINK | HW_REQ | prio(2))
    assert await tb.read(ch(0, CH_SRC)) == 0x1000
    assert await tb.read(ch(0, CH_DST)) == 0x2000
    assert await tb.read(ch(0, CH_LEN)) == 1
    assert await tb.read(ch(0, CH_NEXT)) == 0
    assert await tb.read(ch(0, CH_CTRL)) == (EN | prio(1)), "CTRL[6:1] locked while EN=1"
    await tb.write(GCTRL, 1)
    await tb.wait_idle()
    assert tb.mem.dump(0x2000, 1) == [0x77], "ran with original values"
    assert tb.mem.reads() == [("R", 0x1000)], "no fetch (LINK never taken)"
    tb.check_bus()


@cocotb.test()
async def test_req12_word_transfer_addresses(dut):
    """REQ-12: reads at CH_SRC, writes at CH_DST, m_wstrb=4'hF, word aligned;
    REQ-17: descriptor ends when CH_LEN reaches 0."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0xA1, 0xA2])
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 2, EN)
    await tb.wait_idle()
    assert tb.mem.log == [("R", 0x1000), ("W", 0x2000, 0xA1, 0xF),
                          ("R", 0x1004), ("W", 0x2004, 0xA2, 0xF)], tb.mem.log
    tb.mem.check_aligned_strb()
    assert await tb.read(ACTIVE) == 0
    tb.check_bus()


@cocotb.test()
async def test_req13_one_outstanding_log_order(dut):
    """REQ-13: at most one access outstanding: no write starts before read data is
    accepted and no next read before the write response is accepted (log shows R,W
    strictly alternating); m_arvalid and m_awvalid never both 1 (checker);
    REQ-14: valid+payload hold while ready=0 (checker)."""
    tb = DmaTB(dut, stall=0.5)
    await tb.start()
    tb.mem.load(0x1000, list(range(1, 7)))
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 6, EN)
    await tb.wait_idle()
    kinds = [e[0] for e in tb.mem.log]
    assert kinds == ["R", "W"] * 6, kinds
    tb.mem.check_aligned_strb()
    tb.check_bus()


@cocotb.test()
async def test_req13_aw_w_either_order(dut):
    """REQ-13: AW and W may complete in either order: W accepted while AW is held
    off, and AW accepted while W is held off; the word still transfers correctly.
    REQ-14: the held-off valid+payload stay stable (checker)."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0xBEEF])
    tb.mem.block = {"aw"}
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 1, EN)
    await tb.wait_cycles(8)
    tb.mem.block = set()
    await tb.wait_idle()
    assert tb.mem.log == [("R", 0x1000), ("W", 0x2000, 0xBEEF, 0xF)], tb.mem.log
    await tb.reset()
    tb.mem.block = {"w"}
    tb.mem.load(0x1004, [0xF00D])
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1004, 0x2004, 1, EN)
    await tb.wait_cycles(8)
    tb.mem.block = set()
    await tb.wait_idle()
    assert ("W", 0x2004, 0xF00D, 0xF) in tb.mem.log, tb.mem.log
    tb.check_bus()


@cocotb.test()
async def test_req15_len_and_address_update(dut):
    """REQ-15: after a successful word CH_LEN decrements and CH_SRC/CH_DST advance
    by 4 unless SRC_FIX/DST_FIX. Registers read mid-transfer with the bus frozen
    so the next word cannot complete during the APB reads."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0xC1, 0xC2])
    await tb.write(GCTRL, 1)
    tb.mem.block = {"b"}  # word 1's B response withheld until word 1 is "successful"
    await tb.program(0, 0x1000, 0x4000, 2, EN | SRC_FIX)
    while not any(e[0] == "W" for e in tb.mem.log):
        await FallingEdge(dut.clk)
    tb.mem.block = {"ar", "aw", "w"}  # release B only; freeze word 2's address/data
    await tb.wait_cycles(3)  # word 1's B OKAY lands: LEN/addresses update
    assert await tb.read(ch(0, CH_LEN)) == 1, "LEN decremented after successful word"
    assert await tb.read(ch(0, CH_SRC)) == 0x1000, "SRC fixed"
    assert await tb.read(ch(0, CH_DST)) == 0x4004, "DST advanced by 4"
    tb.mem.block = set()
    await tb.wait_idle()
    assert tb.mem.dump(0x4000, 2) == [0xC1, 0xC1], "both words from same fixed source"
    assert await tb.read(ch(0, CH_DST)) == 0x4008
    tb.check_bus()


@cocotb.test()
async def test_req16_burst_bounds(dut):
    """REQ-16: a burst is at most 2^BURST word transfers: LEN=3 with BURST=0 is
    served as 3 one-word bursts; LEN=6 with BURST=2 as 4+2; LEN=3 with BURST=2 as
    one burst (burst counts via REQ-27 dack pulses)."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0xE0, 0xE1, 0xE2])
    tb.mem.load(0x1100, [0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5])
    tb.mem.load(0x1200, [0xE4, 0xE5, 0xE6])
    dut.dreq.value = 0b001
    await tb.program(0, 0x1000, 0x2000, 3, EN | HW_REQ)
    await tb.write(GCTRL, 1)  # BURST=0 -> at most 1 word per burst
    await tb.wait_idle()
    assert tb.mem.dack_count[0] == 3, f"BURST=0, LEN=3: {tb.mem.dack_count[0]} bursts"
    await tb.write(GCTRL, 1 | 2 << 1)  # BURST=2 -> at most 4 words per burst
    dut.dreq.value = 0b010
    await tb.program(1, 0x1100, 0x2100, 6, EN | HW_REQ)
    await tb.wait_idle()
    assert tb.mem.dack_count[1] == 2, f"BURST=2, LEN=6: {tb.mem.dack_count[1]} bursts (want 4+2)"
    await tb.program(2, 0x1200, 0x2200, 3, EN | HW_REQ)
    dut.dreq.value = 0b100
    await tb.wait_idle()
    assert tb.mem.dack_count[2] == 1, f"BURST=2, LEN=3: {tb.mem.dack_count[2]} bursts"
    assert tb.mem.dump(0x2000, 3) == [0xE0, 0xE1, 0xE2]
    assert tb.mem.dump(0x2100, 6) == [0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5]
    assert tb.mem.dump(0x2200, 3) == [0xE4, 0xE5, 0xE6]
    assert max(tb.mem.dack_width) == 1, f"dack pulse widths {tb.mem.dack_width}"
    dut.dreq.value = 0
    tb.check_bus()


@cocotb.test()
async def test_req17_descriptor_ends_at_len_zero(dut):
    """REQ-17: a descriptor ends when CH_LEN reaches 0: LEN=1 moves exactly one
    word then the channel completes (EN clears, DONE set)."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0x1234])
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 1, EN)
    await tb.wait_idle()
    assert len(tb.mem.log) == 2, tb.mem.log
    assert tb.mem.dump(0x2000, 1) == [0x1234]
    assert await tb.read(ch(0, CH_LEN)) == 0
    assert await tb.read(ACTIVE) == 0
    assert (await tb.read(IRQ_STATUS)) & 0xFF == done_bit(0)
    tb.check_bus()


@cocotb.test()
async def test_req18_zero_length_descriptor(dut):
    """REQ-18: LEN=0 ends at once at grant: no data moved, NO bus access, CH_LEN
    stays 0, completes normally (DONE, not ERR). A channel with HW_REQ=1 and
    LEN=0 is eligible without dreq, and moves nothing, hence no dack (REQ-27).
    (probe p2 / deviation D2 evidence)"""
    tb = DmaTB(dut)
    await tb.start()
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 0, EN)
    await tb.wait_idle()
    assert tb.mem.log == [], f"LEN=0 made bus accesses: {tb.mem.log[:4]}"
    assert await tb.read(ch(0, CH_LEN)) == 0, "CH_LEN must stay 0"
    assert (await tb.read(IRQ_STATUS)) & 0xFF == done_bit(0), "DONE must be set"
    assert await tb.read(ch(0, CH_STAT)) == 0b100
    dut.dreq.value = 0
    await tb.program(1, 0x1000, 0x2000, 0, EN | HW_REQ)
    await tb.wait_idle()
    assert tb.mem.log == [], f"HW_REQ LEN=0 made bus accesses: {tb.mem.log[:4]}"
    assert (await tb.read(IRQ_STATUS)) & done_bit(1), "ch1 zero-length completed"
    assert sum(tb.mem.dack_count) == 0, "no dack without moved words"
    tb.check_bus()


@cocotb.test()
async def test_req19_address_wrap_and_max_len(dut):
    """REQ-19: word addresses wrap modulo 2^32 (reads cross 0xFFFFFFFF->0 and the
    words are delivered in order); CH_LEN of 0xFFFF is a 65535-word count:
    LEN decrements exactly once per moved word (checked via abort arithmetic on a
    fixed-address 0xFFFF transfer before the count could exhaust)."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0xFFFFFFF4, [0xD0, 0xD1, 0xD2])
    tb.mem.load(0x00000000, [0xD3])
    await tb.write(GCTRL, 1)
    await tb.program(0, 0xFFFFFFF4, 0x4000, 4, EN)
    await tb.wait_idle()
    assert raddrs(tb) == [0xFFFFFFF4, 0xFFFFFFF8, 0xFFFFFFFC, 0x0]
    assert tb.mem.dump(0x4000, 4) == [0xD0, 0xD1, 0xD2, 0xD3], "wrapped words delivered"
    mark = len(tb.mem.log)
    await tb.program(1, 0xFFFFFFF8, 0x5000, 0xFFFF, EN | SRC_FIX | DST_FIX)
    while wcount_since(tb, mark) < 20:
        await FallingEdge(dut.clk)
    await tb.write(ch(1, CH_CTRL), 0)  # abort the 65535-word descriptor
    await tb.wait_idle()
    moved = wcount_since(tb, mark)
    assert 20 <= moved < 0xFFFF, f"unexpected completion state, moved={moved}"
    assert await tb.read(ch(1, CH_LEN)) == 0xFFFF - moved, "LEN counts down one per moved word"
    tb.check_bus()


@cocotb.test()
async def test_req20_completion_variants(dut):
    """REQ-20: end with LINK=1 but CH_NEXT=0 -> completes (EN clear, DONE, no
    fetch). LINK=0 with CH_NEXT nonzero -> completes, no fetch."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0x01])
    tb.mem.load(0x1004, [0x02])
    tb.mem.load(0x3000, [0, 0, 0, 0])
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 1, EN | LINK, 0)   # LINK but NEXT=0
    await tb.program(1, 0x1004, 0x2100, 1, EN, 0x3000)     # NEXT set but LINK=0
    await tb.wait_idle()
    for a in raddrs(tb):
        assert a in (0x1000, 0x1004), f"unexpected fetch read at 0x{a:08X}"
    assert (await tb.read(IRQ_STATUS)) & 0xFF == done_bit(0) | done_bit(1)
    assert await tb.read(ch(0, CH_STAT)) == 0b100
    assert await tb.read(ch(1, CH_STAT)) == 0b100
    tb.check_bus()


@cocotb.test()
async def test_req21_fetch_address_order(dut):
    """REQ-21: descriptor fetch reads 4 consecutive words at CH_NEXT in order,
    incrementing addresses, while holding the engine (nothing interleaved)."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0x11])
    tb.mem.load(0x1100, [0x22])
    tb.mem.load(0x3000, [0x1100, 0x2100, 1, 0])
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 1, EN | LINK, 0x3000)
    await tb.wait_idle()
    assert tb.mem.log[:2] == [("R", 0x1000), ("W", 0x2000, 0x11, 0xF)], tb.mem.log[:2]
    assert tb.mem.log[2:6] == [("R", 0x3000), ("R", 0x3004), ("R", 0x3008), ("R", 0x300C)], \
        tb.mem.log[2:6]
    assert tb.mem.log[6:] == [("R", 0x1100), ("W", 0x2100, 0x22, 0xF)], tb.mem.log[6:]
    assert (await tb.read(IRQ_STATUS)) & 0xFF == done_bit(0)
    tb.check_bus()


@cocotb.test()
async def test_req22_fetched_field_masks(dut):
    """REQ-22: fetched words replace CH_SRC/DST/LEN/NEXT; fetched address bits
    [1:0] ignored; fetched LEN bits [31:16] ignored."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0xAA])
    tb.mem.load(0x1004, [0x55])
    tb.mem.load(0x3000, [0x1004 | 0b11, 0x2004 | 0b10, 0x00010001, 0x00000000])
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 1, EN | LINK, 0x3000)
    await tb.wait_idle()
    assert tb.mem.dump(0x2004, 1) == [0x55], "LEN[15:0]=1: one word fetched and moved"
    assert tb.mem.log[6] == ("R", 0x1004), \
        f"fetched SRC [1:0] ignored on load: {tb.mem.log[6]}"
    assert not any(e[0] == "W" and e[1] != 0x2004 for e in tb.mem.log[6:]), \
        "LEN[31:16] must be ignored (only 1 word moved by descriptor 2)"
    assert await tb.read(ch(0, CH_SRC)) == 0x1008, "SRC = fetched(masked)+4 after the moved word"
    assert await tb.read(ch(0, CH_DST)) == 0x2008, "DST = fetched(masked)+4 after the moved word"
    assert await tb.read(ch(0, CH_LEN)) == 0
    tb.check_bus()


@cocotb.test()
async def test_req23_release_after_fetch(dut):
    """REQ-23: after a fetch the channel releases the engine and competes again:
    an equal-PRIO channel granted between the fetch and the chain continuation
    proves release + rotating re-arbitration (REQ-24)."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0x01])
    tb.mem.load(0x1100, [0x02])
    tb.mem.load(0x1200, [0x03])
    tb.mem.load(0x3000, [0x1100, 0x2100, 1, 0])
    await tb.program(0, 0x1000, 0x2000, 1, EN | LINK, 0x3000)
    await tb.program(1, 0x1200, 0x9000, 1, EN)  # same PRIO as ch0
    await tb.write(GCTRL, 1)
    await tb.wait_idle()
    ra, wa = raddrs(tb), waddrs(tb)
    f0 = ra.index(0x3000)
    assert ra[f0:f0 + 4] == [0x3000, 0x3004, 0x3008, 0x300C]
    assert wa.index(0x9000) < wa.index(0x2100), \
        f"ch1 not granted between fetch and chain continuation: {[hex(a) for a in wa]}"
    assert tb.mem.dump(0x9000, 1) == [0x03]
    tb.check_bus()


@cocotb.test()
async def test_req24_priority_wins(dut):
    """REQ-24: among eligible channels the highest PRIO wins: ch1 PRIO=1 served
    before ch0 PRIO=0 although ch0 became eligible first."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0x10])
    tb.mem.load(0x1004, [0x11])
    await tb.program(0, 0x1000, 0x8000, 1, EN | prio(0))
    await tb.program(1, 0x1004, 0x9000, 1, EN | prio(1))
    await tb.write(GCTRL, 1)
    await tb.wait_idle()
    assert waddrs(tb) == [0x9000, 0x8000], f"PRIO order wrong: {waddrs(tb)}"
    tb.check_bus()


@cocotb.test()
async def test_req24_rotation_after_reset(dut):
    """REQ-24: equal PRIO -> the FIRST eligible in rotating order wins; after
    reset the search starts at channel 0. (probe p1 / deviation D1 evidence)"""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [1, 2, 3, 4])
    tb.mem.load(0x2000, [5, 6, 7, 8])
    await tb.program(0, 0x1000, 0x8000, 4, EN | prio(1))
    await tb.program(1, 0x2000, 0x9000, 4, EN | prio(1))
    await tb.write(GCTRL, 1)
    while len(tb.mem.writes()) < 8:
        await FallingEdge(dut.clk)
    first_dst = tb.mem.writes()[0][1]
    assert first_dst == 0x8000, f"ch0 must win first grant after reset; got 0x{first_dst:04X}"
    tb.check_bus()


@cocotb.test()
async def test_req24_rotation_alternates(dut):
    """REQ-24: the rotating search starts at the channel after the one granted
    most recently: two equal-PRIO eligible channels with one word per burst are
    served alternately. Also REQ-16 (BURST=0 -> 1 word per burst)."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0xA0, 0xA1, 0xA2])
    tb.mem.load(0x2000, [0xB0, 0xB1, 0xB2])
    await tb.program(0, 0x1000, 0x8000, 3, EN | prio(0))
    await tb.program(1, 0x2000, 0x9000, 3, EN | prio(0))
    await tb.write(GCTRL, 1)  # BURST=0
    await tb.wait_idle()
    want = [0x8000, 0x9000, 0x8004, 0x9004, 0x8008, 0x9008]
    assert waddrs(tb) == want, f"rotation must alternate: got {[hex(a) for a in waddrs(tb)]}"
    tb.check_bus()


@cocotb.test()
async def test_req24_rotation_start_after_last_grant(dut):
    """REQ-24/REQ-26: the rotating search starts at the channel AFTER the one
    granted most recently: after ch3 is granted, equal-PRIO ch0 and ch3 compete
    -> ch0 must win (order 0,1,2,3), NOT ch3 (searching FROM the last granted
    channel would pick ch3)."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x3000, [0x30])
    tb.mem.load(0x1000, [0x10])
    await tb.write(GCTRL, 1)
    await tb.program(3, 0x3000, 0xD000, 1, EN | prio(1))
    await tb.wait_idle()
    assert await tb.read(LAST_GRANT) == 3
    await tb.write(GCTRL, 0)  # program while ineligible (no grants)
    await tb.program(0, 0x1000, 0x8000, 1, EN | prio(1))
    await tb.program(3, 0x3000, 0xD000, 1, EN | prio(1))
    await tb.write(GCTRL, 1)
    await tb.wait_idle()
    wa = waddrs(tb)
    assert wa[1] == 0x8000, \
        f"after granting ch3 the next grant must be ch0: {[hex(a) for a in wa]}"
    tb.check_bus()


@cocotb.test()
async def test_req26_last_grant(dut):
    """REQ-26: LAST_GRANT reads the channel granted most recently; 3 after reset."""
    tb = DmaTB(dut)
    await tb.start()
    assert await tb.read(LAST_GRANT) == 3
    tb.mem.load(0x1000, [1])
    tb.mem.load(0x3000, [3])
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x8000, 1, EN)
    await tb.wait_idle()
    assert await tb.read(LAST_GRANT) == 0
    await tb.program(2, 0x3000, 0xA000, 1, EN)
    await tb.wait_idle()
    assert await tb.read(LAST_GRANT) == 2
    tb.check_bus()


@cocotb.test()
async def test_req27_dack_single_pulse(dut):
    """REQ-27: dack[n] pulses for ONE cycle after each burst that moved >=1 word
    ends: a HW_REQ chain of 3 one-word descriptors = 3 bursts -> exactly 3
    pulses, each one cycle wide. (probe p5 / deviation D5 evidence)"""
    tb = DmaTB(dut)
    await tb.start()
    await tb.write(GCTRL, 1)
    tb.mem.load(0x1000, [0xA1])
    tb.mem.load(0x1100, [0xA2])
    tb.mem.load(0x1200, [0xA3])
    tb.mem.load(0x3000, [0x1100, 0x2100, 1, 0x3010])
    tb.mem.load(0x3010, [0x1200, 0x2200, 1, 0])
    await tb.program(0, 0x1000, 0x2000, 1, EN | LINK | HW_REQ, 0x3000)
    dut.dreq.value = 1  # hold the request through the whole chain
    await tb.wait_idle()
    dut.dreq.value = 0
    n = tb.mem.dack_count[0]
    assert tb.mem.dump(0x2000, 1) == [0xA1] and tb.mem.dump(0x2100, 1) == [0xA2] \
        and tb.mem.dump(0x2200, 1) == [0xA3], "chain did not complete"
    assert n == 3, f"expected 3 dack pulses (one per burst), got {n}"
    assert tb.mem.dack_width[0] == 1, f"dack must be one cycle wide, got {tb.mem.dack_width[0]}"
    assert sum(tb.mem.dack_count[1:]) == 0
    tb.check_bus()


@cocotb.test()
async def test_req28_read_resp_error_stops(dut):
    """REQ-28: a non-OKAY m_rresp mid-transfer stops the channel: EN clears, ERR
    set (CH_STAT + IRQ_STATUS), DONE NOT set, no further bus access. REQ-29: the
    failed word is not written; CH_SRC/CH_DST/CH_LEN keep its values."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [1, 2, 3])
    tb.mem.read_err.add(0x1004)  # second word read fails
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 3, EN)
    await tb.wait_idle()
    irq = (await tb.read(IRQ_STATUS)) & 0xFF
    assert irq == err_bit(0), f"ERR only expected, got 0b{irq:08b}"
    assert tb.mem.log == [("R", 0x1000), ("W", 0x2000, 1, 0xF), ("R", 0x1004)], tb.mem.log
    assert await tb.read(ch(0, CH_LEN)) == 2, "LEN keeps the failed word's value"
    assert await tb.read(ch(0, CH_SRC)) == 0x1004
    assert await tb.read(ch(0, CH_DST)) == 0x2004
    assert await tb.read(ch(0, CH_STAT)) == 0b010
    assert await tb.read(ACTIVE) == 0
    tb.check_bus()


@cocotb.test()
async def test_req28_write_resp_error_stops(dut):
    """REQ-28/REQ-30: a non-OKAY m_bresp stops the channel: ERR set, DONE clear,
    CH_DST/CH_LEN keep the failed word's values, no further bus access.
    (probe p3 / deviation D3 evidence)"""
    tb = DmaTB(dut)
    await tb.start()
    await tb.write(GCTRL, 1)
    tb.mem.load(0x1000, [1, 2, 3])
    tb.mem.write_err.add(0x2000)  # first written word fails at the RESPONSE
    await tb.program(0, 0x1000, 0x2000, 3, EN)
    await tb.wait_idle()
    irq = (await tb.read(IRQ_STATUS)) & 0xFF
    assert irq == err_bit(0), f"expect ERR only, IRQ_STATUS[7:0]=0b{irq:08b}"
    assert await tb.read(ch(0, CH_DST)) == 0x2000, "CH_DST keeps the failed word's value"
    assert await tb.read(ch(0, CH_LEN)) == 3, "CH_LEN keeps the failed word's value"
    assert await tb.read(ch(0, CH_STAT)) == 0b010
    assert tb.mem.log == [("R", 0x1000), ("W", 0x2000, 1, 0xF)], tb.mem.log
    tb.check_bus()


@cocotb.test()
async def test_req28_fetch_error_stops(dut):
    """REQ-28: the error rule also applies to a descriptor fetch: a failing fetch
    read sets ERR (not DONE), clears EN and ends the chain; the completed
    descriptor's data word was moved."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0x91])
    tb.mem.read_err.add(0x3004)  # fetch of descriptor word 2 (LEN) fails
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 1, EN | LINK, 0x3000)
    await tb.wait_idle()
    irq = (await tb.read(IRQ_STATUS)) & 0xFF
    assert irq == err_bit(0), f"fetch fail must set ERR only, got 0b{irq:08b}"
    assert await tb.read(ch(0, CH_STAT)) == 0b010
    assert await tb.read(ACTIVE) == 0
    assert raddrs(tb) == [0x1000, 0x3000, 0x3004], raddrs(tb)
    tb.check_bus()


@cocotb.test()
async def test_req31_abort_midburst(dut):
    """REQ-31: EN 1->0 while the engine HOLDS the channel (mid-burst): the word in
    progress finishes, no further word starts, DONE and ERR NOT set.
    (probe p4 / deviation D4 evidence)"""
    tb = DmaTB(dut, stall=0.5)
    await tb.start()
    await tb.write(GCTRL, 1 | 2 << 1)  # BURST=2 -> 4 words per burst
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
async def test_req32_status_sticky(dut):
    """REQ-32: CH_STAT.DONE/ERR stay set until the channel is started again; a
    W1C on IRQ_STATUS does not clear CH_STAT."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0x51])
    tb.mem.load(0x1004, [0x52])
    tb.mem.read_err.add(0x1004)
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 1, EN)
    await tb.program(1, 0x1004, 0x2100, 1, EN)
    await tb.wait_idle()
    assert await tb.read(ch(0, CH_STAT)) == 0b100
    assert await tb.read(ch(1, CH_STAT)) == 0b010
    await tb.write(IRQ_STATUS, 0xFF, 0x1)  # W1C both events in IRQ_STATUS
    assert (await tb.read(IRQ_STATUS)) & 0xFF == 0
    assert await tb.read(ch(0, CH_STAT)) == 0b100, "DONE sticky in CH_STAT"
    assert await tb.read(ch(1, CH_STAT)) == 0b010, "ERR sticky in CH_STAT"
    await tb.write(ch(0, CH_CTRL), EN)  # restart clears sticky status
    assert await tb.read(ch(0, CH_STAT)) & 0b110 == 0
    await tb.wait_idle()
    tb.check_bus()


@cocotb.test()
async def test_req33_w1c_bit_and_lane_granularity(dut):
    """REQ-33: IRQ_STATUS W1C clears only bits whose written bit AND byte-lane
    strobe are 1: written bits 0 and 4 with strb=0 -> no clear; strb=1 with value
    0x01 clears bit 0 only; then value 0x10 clears bit 4."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0x61])
    tb.mem.load(0x1004, [0x62])
    tb.mem.read_err.add(0x1004)
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 1, EN)
    await tb.program(1, 0x1004, 0x2100, 1, EN)
    await tb.wait_idle()
    assert (await tb.read(IRQ_STATUS)) & 0xFF == done_bit(0) | err_bit(1)
    await tb.write(IRQ_STATUS, done_bit(0) | err_bit(1), 0x0)  # bits set, no strobe
    assert (await tb.read(IRQ_STATUS)) & 0xFF == done_bit(0) | err_bit(1), "strb=0 must not clear"
    await tb.write(IRQ_STATUS, done_bit(0), 0x1)
    assert (await tb.read(IRQ_STATUS)) & 0xFF == err_bit(1), "bit 0 cleared, bit 5 kept"
    await tb.write(IRQ_STATUS, err_bit(1), 0x1)
    assert (await tb.read(IRQ_STATUS)) & 0xFF == 0
    tb.check_bus()


@cocotb.test()
async def test_req34_set_beats_clear(dut):
    """REQ-34: if an event sets a status bit in the SAME cycle as a W1C write
    clears it, the bit is 1 after that cycle. The bus is frozen with the OKAY B
    response pending; unfreezing lets the completion event land at a fixed cycle;
    the W1C write is aimed so its APB access phase sits on exactly that cycle
    (mem.cycle stamps the irq rise and every sampled write access on one shared
    timeline). A write one cycle later is the sanity leg: it must clear."""
    tb = DmaTB(dut)
    await tb.start()
    # pure event timing (ch0): where does the DONE event land relative to B accept?
    # Stamp conventions (all in mem.run cycle ids): the APB write access phase is
    # observed in cycle j, its update edge is the cycle ENDING edge (j+1); an irq
    # rise observed in cycle k was set by the cycle STARTING edge (k). A true
    # same-cycle race therefore has j + 1 == k.
    await race_start(tb, 0, 0x1000, 0x2000)
    rb0, k0, _ = await race_fire(tb, PRE, 0)
    assert rb0 & done_bit(0), "pure run: DONE set and readable"
    ek = k0 - tb.mem.b_accept
    assert 0 < ek < 10, f"unexpected event delay after B accept: {ek}"
    # calibrate write-access cycle vs offset (ch1)
    await race_start(tb, 1, 0x1010, 0x2010)
    rb1, k1, j1 = await race_fire(tb, PRE, 0, done_bit(1), 0x1)
    assert k1 - tb.mem.b_accept == ek, f"event delay not reproducible: {k1 - tb.mem.b_accept} vs {ek}"
    if j1 + 1 < k1:
        assert rb1 & done_bit(1), "write strictly before event: DONE set afterwards"
    else:
        assert j1 + 1 == k1 and rb1 & done_bit(1), "zero-offset write landed on the event edge"
    delta = k1 - j1 - 1
    assert delta >= 0, f"event precedes the earliest possible write (k1={k1}, j1={j1})"
    # race leg (ch2): write update edge == event edge
    await race_start(tb, 2, 0x1020, 0x2020)
    rb2, k2, j2 = await race_fire(tb, PRE, delta, done_bit(2), 0x1)
    assert j2 + 1 == k2, f"race mis-aimed: write edge {j2 + 1}, event edge {k2}"
    assert rb2 & done_bit(2), "REQ-34: set must beat clear in the same cycle"
    # sanity leg (ch3): write update edge == event edge + 1 -> clears
    await race_start(tb, 3, 0x1030, 0x2030)
    rb3, k3, j3 = await race_fire(tb, PRE, delta + 1, done_bit(3), 0x1)
    assert j3 + 1 == k3 + 1, f"sanity leg mis-aimed: write edge {j3 + 1}, event edge {k3}"
    assert not (rb3 & done_bit(3)), "W1C one cycle after the event must clear"
    tb.check_bus()


@cocotb.test()
async def test_req35_irq_iff_status_and_enable(dut):
    """REQ-35: irq = 1 iff (IRQ_STATUS AND IRQ_ENABLE) != 0, a function of
    register state only: DONE set with IRQ_ENABLE=0 -> irq 0; enabling a set bit
    -> 1; enabling an unset bit -> 0; W1C -> 0."""
    tb = DmaTB(dut)
    await tb.start()
    tb.mem.load(0x1000, [0x71])
    await tb.write(GCTRL, 1)
    await tb.program(0, 0x1000, 0x2000, 1, EN)
    await tb.wait_idle()
    assert await tb.read(IRQ_STATUS) & done_bit(0)
    assert await tb.read(IRQ_ENABLE) == 0
    assert int(dut.irq.value) == 0, "irq must be 0 with IRQ_ENABLE=0"
    await tb.write(IRQ_ENABLE, done_bit(1))
    assert int(dut.irq.value) == 0, "enable for a bit that is not set -> irq 0"
    await tb.write(IRQ_ENABLE, done_bit(0))
    assert int(dut.irq.value) == 1, "status AND enable -> irq 1"
    await tb.write(IRQ_STATUS, done_bit(0), 0x1)
    assert int(dut.irq.value) == 0, "W1C drops irq"
    await tb.write(IRQ_ENABLE, err_bit(0))
    assert int(dut.irq.value) == 0
    tb.check_bus()
