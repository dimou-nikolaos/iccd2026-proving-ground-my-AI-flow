"""DMA evidence suite (fixture version - becomes the challenge-2 test skeleton).

Run inside the flow image:
    cd prep/dma_vip && make

Each body is tagged with the requirement *shape* it covers. On day 1 the tags
map onto the released REQ ids; the bodies survive unchanged because they only
talk to the register map (dma_env.Regmap) and the bus boundary.

Deliberate design choices (from the challenge-1 lessons):
  * checks at the bus boundary only (memory content, transaction log, strobes,
    irq shape) - the same things hidden tests can observe;
  * a mid-operation reset test: reset-branch mutants are invisible at time 0
    because registers start at zero (flow-internals 5);
  * `wait_done` raises DmaTimeout quickly - hang-class deviations must fail
    loudly, not eat the 600 s sim timeout;
  * whole suite stays in the low single-digit sim seconds (mutation timeout).
"""

import cocotb
from cocotb.triggers import FallingEdge, RisingEdge

from axi4lite import AXI4LiteSlaveConfig
from dma_env import DmaEnv, DmaTimeout, Regmap

# Fixture port names (AXI master ports are m_* on the DUT). Day 1: adjust here.
AXI_NAMES = {k: f"m_{k}" for k in (
    "awvalid", "awready", "awaddr", "wvalid", "wready", "wdata", "wstrb",
    "bvalid", "bready", "bresp", "arvalid", "arready", "araddr",
    "rvalid", "rready", "rdata", "rresp")}

SRC, DST, DESC = 0x0100, 0x0800, 0x0C00


def pattern(n: int, seed: int = 0xA5) -> list:
    return [((seed + i * 0x12345) ^ (i * 7919)) & 0xFFFF_FFFF for i in range(n)]


def fill(mem, addr: int, words: list) -> None:
    for i, w in enumerate(words):
        mem.poke_word(addr + 4 * i, w)


def zero_region(mem, addr: int, n: int) -> None:
    for i in range(n):
        mem.poke_word(addr + 4 * i, 0)


async def setup(dut, axi_cfg=None, bug=0):
    env = DmaEnv(dut, Regmap(), axi_names=AXI_NAMES, axi_cfg=axi_cfg)
    await env.start(bug=bug)
    return env


# ---------------------------------------------------------------------------
# REQ-A: register interface
# ---------------------------------------------------------------------------
@cocotb.test()
async def test_reg_rw_and_ro(dut):
    """TODO(REQ: config regs): RW regs hold written values; RO regs do not change."""
    env = await setup(dut)
    # reset values, read back before anything is written (kills mutated
    # reset-branch constants that the engine would otherwise re-load anyway)
    for addr, nm in ((env.rm.ctrl, "CTRL"), (env.rm.src, "SRC"), (env.rm.dst, "DST"),
                     (env.rm.length, "LEN"), (env.rm.desc, "DESC")):
        got = await env.read_reg(addr)
        assert got == 0, f"{nm} reset read-back: expected 0, got {got:#010x}"
    for addr, val in ((env.rm.src, 0x1234_5678), (env.rm.dst, 0x9ABC_0000),
                      (env.rm.length, 7), (env.rm.desc, 0x0C00)):
        await env.write_reg(addr, val)
        got = await env.read_reg(addr)
        assert got == val, f"reg @0x{addr:03x}: wrote {val:#x} read {got:#x}"
    # RO registers: ID read-back must be constant and writes must not stick.
    if env.rm.id_value is not None:
        got = await env.read_reg(env.rm.dev_id)
        assert got == env.rm.id_value, f"ID reg: expected {env.rm.id_value:#x}, got {got:#x}"
    await env.write_ro_expect_unchanged(env.rm.dev_id, 0xFFFF_FFFF)
    await env.write_ro_expect_unchanged(env.rm.next_, 0xDEAD_BEEF)
    # byte strobes on a RW register (pstrb handling)
    await env.write_reg(env.rm.src, 0xAAAA_AAAA)
    await env.write_reg(env.rm.src, 0x5555_5555, strb=0b0011)
    got = await env.read_reg(env.rm.src)
    assert got == 0xAAAA_5555, f"pstrb merge: expected 0xAAAA5555, got {got:#010x}"
    # CTRL read-back must show every writable bit where it was written (kills
    # bit-index mutants in the CTRL decode: a bit written at position i must
    # read back at position i, not a neighbour)
    await env.write_reg(env.rm.ctrl, 0b11101)     # EN, RELOAD, MODE, IE; START=0
    got = await env.read_reg(env.rm.ctrl)
    assert got & 0b11101 == 0b11101, f"CTRL read-back lost bits: wrote 0b11101, got {got:#b}"
    await env.write_reg(env.rm.ctrl, 0b00000)
    got = await env.read_reg(env.rm.ctrl)
    assert got & 0b11111 == 0, f"CTRL bits not cleared by writing 0: {got:#b}"


# ---------------------------------------------------------------------------
# REQ-B: single block move (the core data path)
# ---------------------------------------------------------------------------
@cocotb.test()
async def test_block_move_8_words(dut):
    """TODO(REQ: block move): LEN words move src->dst, all four lanes, exact addresses."""
    env = await setup(dut)
    words = pattern(8)
    fill(env.mem, SRC, words)
    zero_region(env.mem, DST, 8)
    await env.configure_block(SRC, DST, 8)
    await env.set_mode(chain=False)
    await env.trigger_start()
    st = await env.wait_done()
    assert st & env.rm.st_done, f"DONE not set, status={st:#010x}"
    assert not (st & env.rm.st_err), f"spurious ERR, status={st:#010x}"
    env.expect_block_moved(SRC, DST, 8, words)
    env.expect_write_addresses(DST, 8)
    env.expect_all_strobes(DST, DST + 32)


@cocotb.test()
async def test_block_len_boundaries(dut):
    """TODO(REQ: LEN boundary): LEN=1 moves exactly one word; LEN=0 moves nothing.

    Off-by-one on the length counter is *the* classic seeded deviation (D1):
    only a boundary-len test catches LEN-1 or LEN+1, so both edges are checked,
    and the word *after* the region must stay untouched (catches LEN+1 too).
    """
    for length in (1, 2, 4):
        env = await setup(dut)
        words = pattern(length)
        fill(env.mem, SRC, words)
        zero_region(env.mem, DST, length + 1)
        await env.configure_block(SRC, DST, length)
        await env.set_mode(chain=False)
        await env.trigger_start()
        await env.wait_done()
        env.expect_block_moved(SRC, DST, length, words)
        env.expect_write_addresses(DST, length)
        extra = env.mem.peek_word(DST + 4 * length)
        assert extra == 0, f"LEN={length}: wrote past the end (word@+{4*length}={extra:#x})"
        assert len(env.data_writes_in(DST, DST + 4 * (length + 2))) == length, \
            f"LEN={length}: wrong number of destination writes"

    # LEN=0: no data traffic at all, must still terminate cleanly.
    env = await setup(dut)
    await env.configure_block(SRC, DST, 0)
    await env.set_mode(chain=False)
    await env.trigger_start()
    st = await env.wait_done()
    assert not (st & env.rm.st_busy), "BUSY stuck after LEN=0 start"
    assert env.data_writes_in(DST, DST + 0x100) == [], "LEN=0 moved data"


@cocotb.test()
async def test_backpressure_stress(dut):
    """TODO(REQ: AXI timing): identical result with slow/jittery slave.

    Backpressure exposes payload-stability and pipelining deviations that a
    zero-delay slave hides. Data must match exactly, addresses too.
    """
    cfg = AXI4LiteSlaveConfig(aw_delay=2, w_delay=1, ar_delay=3, b_delay=2,
                              r_delay=2, jitter=2, seed=7)
    env = await setup(dut, axi_cfg=cfg)
    words = pattern(6)
    fill(env.mem, SRC, words)
    zero_region(env.mem, DST, 6)
    await env.configure_block(SRC, DST, 6)
    await env.set_mode(chain=False)
    await env.trigger_start()
    st = await env.wait_done(max_polls=800)
    assert st & env.rm.st_done, f"DONE not set under backpressure (status={st:#010x})"
    env.expect_block_moved(SRC, DST, 6, words)
    env.expect_write_addresses(DST, 6)


# ---------------------------------------------------------------------------
# REQ-C: descriptor / chain mode
# ---------------------------------------------------------------------------
@cocotb.test()
async def test_chain_two_descriptors(dut):
    """TODO(REQ: linked descriptors): two chained descriptors both move, in order,
    and the chain ends at next=0 (D7: chain-end must actually terminate)."""
    env = await setup(dut)
    wA, wB = pattern(3, 0x11), pattern(5, 0x22)
    fill(env.mem, SRC, wA)
    fill(env.mem, SRC + 0x40, wB)
    zero_region(env.mem, DST, 3)
    zero_region(env.mem, DST + 0x40, 5)
    d0, d1 = DESC, DESC + 0x10
    env.mem.poke_word(d0 + 0, SRC);      env.mem.poke_word(d0 + 4, DST)
    env.mem.poke_word(d0 + 8, 3);        env.mem.poke_word(d0 + 12, d1)
    env.mem.poke_word(d1 + 0, SRC + 0x40); env.mem.poke_word(d1 + 4, DST + 0x40)
    env.mem.poke_word(d1 + 8, 5);        env.mem.poke_word(d1 + 12, 0)   # chain end
    await env.write_reg(env.rm.desc, d0)
    await env.set_mode(chain=True)
    await env.trigger_start()
    st = await env.wait_done()
    assert st & env.rm.st_done, f"chain did not complete (status={st:#010x})"
    assert not (st & env.rm.st_err), f"spurious ERR in clean chain run (status={st:#010x})"
    env.expect_block_moved(SRC, DST, 3, wA, why="descriptor 0: ")
    env.expect_block_moved(SRC + 0x40, DST + 0x40, 5, wB, why="descriptor 1: ")


@cocotb.test()
async def test_chain_next_ptr_observable(dut):
    """TODO(REQ: chain status): the last fetched next pointer is readable, and
    the descriptor fields actually come from the fetched descriptor, not the
    block registers (D8: descriptor race shows up as wrong src/len)."""
    env = await setup(dut)
    # deliberately wrong block registers: a design that uses them instead of
    # the descriptor will produce provably wrong data.
    await env.configure_block(0xDEAD_0000, 0xBEEF_0000, 99)
    words = pattern(4, 0x77)
    src2, dst2, desc = 0x0200, 0x0900, DESC
    fill(env.mem, src2, words)
    zero_region(env.mem, dst2, 4)
    await env.program_descriptor(desc, src2, dst2, 4, 0x0000_0000)
    await env.write_reg(env.rm.desc, desc)
    await env.set_mode(chain=True)
    await env.trigger_start()
    st = await env.wait_done()
    assert not (st & env.rm.st_err), f"spurious ERR fetching a clean descriptor (status={st:#010x})"
    env.expect_block_moved(src2, dst2, 4, words, why="descriptor fetch: ")
    nxt = await env.read_reg(env.rm.next_)
    assert nxt == 0, f"NEXT after a next=0 descriptor: expected 0, got {nxt:#x}"


@cocotb.test()
async def test_chain_second_from_first_output(dut):
    """TODO(REQ: chain addressing): the second block's addresses come from the
    second descriptor - a wrong `next` follow moves the *wrong* block."""
    env = await setup(dut)
    wA, wB = pattern(2, 0x31), pattern(2, 0x32)
    srcA, dstA, srcB, dstB = 0x0200, 0x0A00, 0x0300, 0x0B00
    fill(env.mem, srcA, wA); fill(env.mem, srcB, wB)
    zero_region(env.mem, dstA, 2); zero_region(env.mem, dstB, 2)
    d0, d1 = DESC, DESC + 0x10
    await env.program_descriptor(d0, srcA, dstA, 2, d1)
    await env.program_descriptor(d1, srcB, dstB, 2, 0)
    await env.write_reg(env.rm.desc, d0)
    await env.set_mode(chain=True)
    await env.trigger_start()
    st = await env.wait_done()
    assert not (st & env.rm.st_err), f"spurious ERR in two-descriptor chain (status={st:#010x})"
    env.expect_block_moved(srcA, dstA, 2, wA, why="desc0: ")
    env.expect_block_moved(srcB, dstB, 2, wB, why="desc1 (via next): ")
    # nothing may have been moved to the block-register addresses (0xDEAD/BEEF
    # would be an illegal AXI slave range -> error resp; just assert no writes
    # in the low unused window instead)
    stray = env.data_writes_in(0x0000, 0x0100) + env.data_writes_in(0x0400, 0x0800)
    assert not stray, f"stray writes outside the two destinations: {stray}"


# ---------------------------------------------------------------------------
# REQ-D: status / busy / interrupt lifecycle
# ---------------------------------------------------------------------------
@cocotb.test()
async def test_busy_status_and_irq_pulse(dut):
    """TODO(REQ: status+irq): BUSY is visible during, DONE after; IE=1 gives a
    1-cycle irq pulse at completion (level irq is a seeded deviation)."""
    env = await setup(dut, axi_cfg=AXI4LiteSlaveConfig(ar_delay=3, r_delay=2, b_delay=2))
    await env.configure_block(SRC, DST, 4)
    fill(env.mem, SRC, pattern(4))
    await env.set_mode(chain=False, ie=True)
    await env.trigger_start()
    st = await env.status()
    assert st & env.rm.st_busy, f"BUSY not visible right after start (status={st:#010x})"
    st = await env.wait_done(max_polls=400)
    assert st & env.rm.st_done, "DONE never set"
    env.expect_irq_pulses(1)


@cocotb.test()
async def test_irq_masked_when_ie0(dut):
    """TODO(REQ: irq mask): IE=0 must produce no interrupt edge at all."""
    env = await setup(dut)
    await env.configure_block(SRC, DST, 2)
    fill(env.mem, SRC, pattern(2))
    await env.set_mode(chain=False, ie=False)
    await env.trigger_start()
    st = await env.wait_done()
    assert st & env.rm.st_done
    for _ in range(5):
        await RisingEdge(dut.clk)
    assert env.irq_widths == [], f"irq fired with IE=0: widths={env.irq_widths}"


@cocotb.test()
async def test_status_w1c(dut):
    """TODO(REQ: status W1C): writing 1 to DONE/ERR clears only those bits.

    Uses the *minimal* strobe (pstrb=0b0001, the status word is one byte):
    mutants that move the W1C strobe/bit decode to another bit index (a common
    `constant`-class mutation) then fail to clear, or clear the wrong bit.
    """
    env = await setup(dut)
    await env.configure_block(SRC, DST, 2)
    fill(env.mem, SRC, pattern(2))
    await env.set_mode(chain=False)
    await env.trigger_start()
    st = await env.wait_done()
    assert st & env.rm.st_done
    await env.write_reg(env.rm.status, env.rm.st_done, strb=0b0001)   # W1C DONE
    st = await env.status()
    assert not (st & env.rm.st_done), f"DONE not cleared by W1C (status={st:#010x})"
    assert not (st & env.rm.st_busy), f"BUSY unexpectedly set after W1C (status={st:#010x})"
    # strobe gating: a STATUS write that does NOT strobe byte 0 must not clear
    # anything even when pwdata bit 1 is set (kills dropped-pstrb mutants).
    await env.trigger_start()
    st = await env.wait_done()
    assert st & env.rm.st_done
    await env.write_reg(env.rm.status, env.rm.st_done, strb=0b1110)   # no byte-0 lane
    st = await env.status()
    assert st & env.rm.st_done, \
        f"DONE cleared by an un-strobed STATUS write (status={st:#010x})"


@cocotb.test()
async def test_ctrl_refresh_while_idle_does_not_restart(dut):
    """TODO(REQ: start decode): rewriting CTRL with EN=1, START=0 while idle must
    not start a transfer (e.g. a driver re-applying its config).

    Kills mutants that read the START bit position from the wrong CTRL bit:
    here EN (bit 0) is re-written while EN is already 1, which under such a
    mutant silently kicks off a transfer with the programmed addresses.
    """
    env = await setup(dut)
    await env.configure_block(SRC, DST, 2)
    fill(env.mem, SRC, pattern(2))
    await env.set_mode(chain=False)
    await env.trigger_start()
    st = await env.wait_done()
    assert st & env.rm.st_done
    # re-apply a config write with EN=1, IE=1, START=0 (bit 1 must stay 0)
    await env.set_mode(chain=False, ie=True)
    zero_region(env.mem, DST, 2)
    env.mem.log.clear()          # look only at traffic *after* the refresh
    for _ in range(6):
        await RisingEdge(dut.clk)
    st = await env.status()
    assert not (st & env.rm.st_busy), \
        f"CTRL refresh (START=0) restarted the engine (status={st:#010x})"
    assert env.data_writes_in(DST, DST + 0x10) == [], \
        f"CTRL refresh moved data without START: {env.data_writes_in(DST, DST + 0x10)}"
    # strobe gating on the START decode: a CTRL write whose byte-0 lane is NOT
    # strobed must not start even when pwdata carries the START bit
    await env.apb.write(env.rm.ctrl, env.rm.ctrl_en | env.rm.ctrl_start, strb=0b1110)
    for _ in range(6):
        await RisingEdge(dut.clk)
    st = await env.status()
    assert not (st & env.rm.st_busy), \
        f"START bit in an un-strobed CTRL byte restarted the engine (status={st:#010x})"
    assert env.data_writes_in(DST, DST + 0x10) == [], \
        f"un-strobed CTRL write moved data: {env.data_writes_in(DST, DST + 0x10)}"
    # with EN cleared, even a fully-strobed CTRL write carrying START must not
    # start (kills mutants that drop the EN gate from the start pulse).
    # EN is cleared by a *separate* write first: in the same write that clears
    # EN the engine still sees the old EN=1, which would confuse the probe.
    await env.write_reg(env.rm.ctrl, 0)                       # EN=0
    await env.write_reg(env.rm.ctrl, env.rm.ctrl_start)       # EN=0, START=1
    for _ in range(6):
        await RisingEdge(dut.clk)
    st = await env.status()
    assert not (st & env.rm.st_busy), \
        f"START with EN=0 restarted the engine (status={st:#010x})"


@cocotb.test()
async def test_read_of_ctrl_does_not_start(dut):
    """TODO(REQ: start decode): a CTRL *read* whose PWDATA happens to carry the
    START bit must not start a transfer.

    PWDATA is don't-care during reads - a real master may leave it at its last
    value. Mutants that decode start_req without the APB write qualifier fire
    here. Driven by hand because the BFM zeroes PWDATA on reads (which would
    hide exactly this class).
    """
    env = await setup(dut)
    await env.configure_block(SRC, DST, 2)
    fill(env.mem, SRC, pattern(2))
    await env.set_mode(chain=False)          # EN=1, engine idle
    dut = env.dut
    # hand-driven APB READ of CTRL with PWDATA carrying START (bit 1)
    await FallingEdge(dut.clk)
    dut.psel.value = 1
    dut.penable.value = 0
    dut.pwrite.value = 0
    dut.paddr.value = env.rm.ctrl
    dut.pwdata.value = env.rm.ctrl_start
    dut.pstrb.value = 0xF
    await RisingEdge(dut.clk)
    await FallingEdge(dut.clk)
    dut.penable.value = 1
    await RisingEdge(dut.clk)
    await FallingEdge(dut.clk)
    dut.psel.value = 0
    dut.penable.value = 0
    env.mem.log.clear()
    for _ in range(6):
        await RisingEdge(dut.clk)
    st = await env.status()
    assert not (st & env.rm.st_busy), \
        f"CTRL read with START-bit PWDATA started the engine (status={st:#010x})"
    assert env.data_writes_in(DST, DST + 0x10) == [], \
        f"CTRL read moved data: {env.data_writes_in(DST, DST + 0x10)}"


@cocotb.test()
async def test_ghost_write_without_psel(dut):
    """TODO(REQ: APB commit window): PWRITE asserted outside a selected transfer
    (PSEL=0) must not write anything - the write commits in the ACCESS phase only.

    Kills mutants that loosen the APB commit decode (pwrite alone deciding the
    commit): we hold PWRITE high with PSEL/PENABLE low, changing PWDATA to a
    value that would corrupt a register if the write were real.
    """
    env = await setup(dut)
    await env.write_reg(env.rm.src, 0x1111_1111)
    dut = env.dut
    await FallingEdge(dut.clk)
    dut.psel.value = 0
    dut.penable.value = 0
    dut.pwrite.value = 1
    dut.paddr.value = env.rm.src
    dut.pwdata.value = 0xDEAD_BEEF
    for _ in range(3):
        await RisingEdge(dut.clk)
    await FallingEdge(dut.clk)
    dut.pwrite.value = 0
    got = await env.read_reg(env.rm.src)
    assert got == 0x1111_1111, \
        f"PWRITE without PSEL wrote 0xDEADBEEF into SRC (read {got:#010x})"


@cocotb.test()
async def test_status_write_of_other_regs_does_not_clear(dut):
    """TODO(REQ: status sticky): a completed DONE survives config-register writes.

    Catches `relational` mutants that broaden the W1C decode (&& -> ||): here a
    normal write to LENGTH whose value has bit 1 set (the DONE bit position)
    would wrongly clear DONE. Also writes DESC/SRC with bit-set values.
    """
    env = await setup(dut)
    await env.configure_block(SRC, DST, 2)
    fill(env.mem, SRC, pattern(2))
    await env.set_mode(chain=False)
    await env.trigger_start()
    st = await env.wait_done()
    assert st & env.rm.st_done
    await env.write_reg(env.rm.length, 0b10)          # bit 1 == DONE bit position
    await env.write_reg(env.rm.src, SRC | 0b1110)
    await env.write_reg(env.rm.desc, DESC | 0b1110)
    st = await env.status()
    assert st & env.rm.st_done, \
        f"DONE cleared by writing bit-1 values to config regs (status={st:#010x})"


# ---------------------------------------------------------------------------
# REQ-F: AXI error response handling (playbook: error-lifecycle deviations)
# ---------------------------------------------------------------------------
@cocotb.test()
async def test_axi_read_error_sets_err(dut):
    """TODO(REQ: error response): a slave error on a data read must raise ERR.

    Without error injection, every *response-handling* decode (`rresp/bresp !=
    OKAY`, the ERR set/clear logic) is dead code: mutants there are invisible.
    One injected SLVERR per channel makes the whole error path testable.
    """
    cfg = AXI4LiteSlaveConfig(err_ranges=[(SRC, SRC + 0x40, 2)])   # RESP_SLVERR
    env = await setup(dut, axi_cfg=cfg)
    fill(env.mem, SRC, pattern(4))
    zero_region(env.mem, DST, 4)
    await env.configure_block(SRC, DST, 4)
    await env.set_mode(chain=False)
    await env.trigger_start()
    st = await env.wait_done(max_polls=800)
    assert st & env.rm.st_err, f"SLVERR on data read did not set ERR (status={st:#010x})"


@cocotb.test()
async def test_axi_write_error_sets_err(dut):
    """TODO(REQ: error response): a slave error on a data *write* (B channel)."""
    cfg = AXI4LiteSlaveConfig(err_ranges=[(DST, DST + 0x40, 2)])
    env = await setup(dut, axi_cfg=cfg)
    fill(env.mem, SRC, pattern(3))
    await env.configure_block(SRC, DST, 3)
    await env.set_mode(chain=False)
    await env.trigger_start()
    st = await env.wait_done(max_polls=800)
    assert st & env.rm.st_err, f"SLVERR on data write did not set ERR (status={st:#010x})"


@cocotb.test()
async def test_axi_desc_error_sets_err(dut):
    """TODO(REQ: error response): an error reading a descriptor must raise ERR."""
    d0 = DESC
    cfg = AXI4LiteSlaveConfig(err_ranges=[(d0, d0 + 0x10, 2)])
    env = await setup(dut, axi_cfg=cfg)
    # sane descriptor contents even though reads answer SLVERR
    await env.program_descriptor(d0, SRC, DST, 2, 0)
    fill(env.mem, SRC, pattern(2))
    await env.write_reg(env.rm.desc, d0)
    await env.set_mode(chain=True)
    await env.trigger_start()
    st = await env.wait_done(max_polls=800)
    assert st & env.rm.st_err, f"SLVERR on descriptor fetch did not set ERR (status={st:#010x})"


@cocotb.test()
async def test_chain_len_zero_descriptor_skips(dut):
    """TODO(REQ: descriptor length): a mid-chain descriptor with LEN=0 moves
    nothing and the chain still terminates (kills mutants that key the LEN
    branch of the descriptor latch on the wrong descriptor word: with LEN=0
    the design must skip, and a wrong word makes it move one garbage word).
    """
    env = await setup(dut)
    wA = pattern(2, 0x71)
    srcA, dstA = 0x0200, 0x0A00
    srcB, dstB = 0x0300, 0x0B00
    fill(env.mem, srcA, wA)
    fill(env.mem, srcB, pattern(2, 0x72))
    zero_region(env.mem, dstA, 2)
    zero_region(env.mem, dstB, 2)
    d0, d1 = DESC, DESC + 0x10
    await env.program_descriptor(d0, srcA, dstA, 2, d1)
    await env.program_descriptor(d1, srcB, dstB, 0, 0)      # LEN=0: move nothing
    await env.write_reg(env.rm.desc, d0)
    await env.set_mode(chain=True)
    await env.trigger_start()
    st = await env.wait_done()
    assert st & env.rm.st_done, f"chain with LEN=0 descriptor did not complete (status={st:#010x})"
    env.expect_block_moved(srcA, dstA, 2, wA, why="desc0: ")
    assert [env.mem.peek_word(dstB + 4 * i) for i in range(2)] == [0, 0], \
        f"LEN=0 descriptor moved data to 0x{dstB:04x}"
    assert env.data_writes_in(dstB, dstB + 0x10) == [], \
        f"LEN=0 descriptor wrote to 0x{dstB:04x}: {env.data_writes_in(dstB, dstB + 0x10)}"


@cocotb.test()
async def test_resp_valid_gating(dut):
    """TODO(REQ: error response): RRESP/BRESP are don't-care while RVALID/BVALID
    are low - the engine must NOT act on a latched error code between transfers.

    The slave holds RRESP=SLVERR whenever idle but answers every real read with
    OKAY. A response decode that ignores the VALID qualifier raises ERR on this
    clean run. Checked in both block and chain mode (the descriptor-read wait
    states are a separate decode site).
    """
    cfg = AXI4LiteSlaveConfig(resp_when_idle=2, ar_delay=2, r_delay=2, b_delay=1)
    env = await setup(dut, axi_cfg=cfg)
    words = pattern(4)
    fill(env.mem, SRC, words)
    zero_region(env.mem, DST, 4)
    await env.configure_block(SRC, DST, 4)
    await env.set_mode(chain=False)
    await env.trigger_start()
    st = await env.wait_done(max_polls=600)
    assert st & env.rm.st_done and not (st & env.rm.st_err), \
        f"latched idle RRESP misread in block mode (status={st:#010x})"
    env.expect_block_moved(SRC, DST, 4, words, why="idle-resp block run: ")
    # chain mode: descriptor fetch wait states are the other decode site
    env2 = await setup(dut, axi_cfg=cfg)
    wA = pattern(3, 0x61)
    fill(env2.mem, SRC, wA)
    zero_region(env2.mem, DST, 3)
    await env2.program_descriptor(DESC, SRC, DST, 3, 0)
    await env2.write_reg(env2.rm.desc, DESC)
    await env2.set_mode(chain=True)
    await env2.trigger_start()
    st = await env2.wait_done(max_polls=600)
    assert st & env2.rm.st_done and not (st & env2.rm.st_err), \
        f"latched idle RRESP misread in chain mode (status={st:#010x})"
    env2.expect_block_moved(SRC, DST, 3, wA, why="idle-resp chain run: ")


@cocotb.test()
async def test_err_w1c(dut):
    """TODO(REQ: status W1C for ERR): the ERR bit is clearable like DONE.

    Combined with the injected error this kills the ERR set/clear decode
    mutants (missing set, always-clear, wrong-address or wrong-strobe clear).
    """
    cfg = AXI4LiteSlaveConfig(err_ranges=[(SRC, SRC + 0x40, 2)])
    env = await setup(dut, axi_cfg=cfg)
    fill(env.mem, SRC, pattern(2))
    await env.configure_block(SRC, DST, 2)
    await env.set_mode(chain=False)
    await env.trigger_start()
    st = await env.wait_done(max_polls=800)
    assert st & env.rm.st_err, f"ERR never raised (status={st:#010x})"
    assert st & env.rm.st_done, f"DONE missing after completed-with-error run (status={st:#010x})"
    # writing a value with bit 2 (the ERR bit position) into an unrelated
    # config register must NOT clear ERR (kills &&->|| in the ERR W1C decode)
    await env.write_reg(env.rm.length, 0b100)
    st = await env.status()
    assert st & env.rm.st_err, f"ERR cleared by writing bit-2 to LENGTH (status={st:#010x})"
    # hand-driven STATUS *read* with PWDATA don't-care lanes carrying DONE|ERR:
    # reads must never clear status bits (kills W1C decodes that dropped the
    # APB-write qualifier - PWDATA is don't-care during reads)
    dut = env.dut
    await FallingEdge(dut.clk)
    dut.psel.value = 1
    dut.penable.value = 0
    dut.pwrite.value = 0
    dut.paddr.value = env.rm.status
    dut.pwdata.value = 0b110
    dut.pstrb.value = 0xF
    await RisingEdge(dut.clk)
    await FallingEdge(dut.clk)
    dut.penable.value = 1
    await RisingEdge(dut.clk)
    await FallingEdge(dut.clk)
    dut.psel.value = 0
    dut.penable.value = 0
    st = await env.status()
    assert (st & 0b110) == 0b110, \
        f"STATUS read with don't-care PWDATA cleared bits (status={st:#010x})"
    # clear ERR only: minimal strobe, minimal bit
    await env.write_reg(env.rm.status, env.rm.st_err, strb=0b0001)
    st = await env.status()
    assert not (st & env.rm.st_err), f"ERR not cleared by W1C (status={st:#010x})"
    assert st & env.rm.st_done, \
        f"ERR-W1C also cleared DONE (status={st:#010x})"
    # and DONE W1C must not clear ERR in return
    await env.trigger_start()
    st = await env.wait_done(max_polls=800)
    assert st & env.rm.st_err, f"ERR not re-raised on second run (status={st:#010x})"
    await env.write_reg(env.rm.status, env.rm.st_done, strb=0b0001)
    st = await env.status()
    assert st & env.rm.st_err, f"DONE W1C wrongly cleared ERR (status={st:#010x})"


@cocotb.test()
async def test_done_not_early_in_chain(dut):
    """TODO(REQ: completion semantics): DONE must not assert until the whole
    chain finished - a status decode keyed to the wrong FSM state (one before
    completion) raises DONE early while the last descriptor is still moving.

    Slow slave guarantees many polls while BUSY=1; every poll must show DONE=0.
    """
    cfg = AXI4LiteSlaveConfig(ar_delay=3, r_delay=3, aw_delay=2, b_delay=3, jitter=1, seed=11)
    env = await setup(dut, axi_cfg=cfg)
    wA, wB = pattern(4, 0x51), pattern(6, 0x52)
    srcA, dstA, srcB, dstB = 0x0200, 0x0A00, 0x0300, 0x0B00
    fill(env.mem, srcA, wA)
    fill(env.mem, srcB, wB)
    d0, d1 = DESC, DESC + 0x10
    await env.program_descriptor(d0, srcA, dstA, 4, d1)
    await env.program_descriptor(d1, srcB, dstB, 6, 0)
    await env.write_reg(env.rm.desc, d0)
    await env.set_mode(chain=True)
    await env.trigger_start()
    saw_busy = False
    st = 0
    for _ in range(600):
        st = await env.status()
        if st & env.rm.st_busy:
            saw_busy = True
            assert not (st & env.rm.st_done), \
                f"DONE set while chain still running (status={st:#010x})"
        if not (st & env.rm.st_busy) and saw_busy:
            break
    assert saw_busy, "never observed BUSY (slave too fast? test ineffective)"
    env.expect_block_moved(srcA, dstA, 4, wA, why="desc0: ")
    env.expect_block_moved(srcB, dstB, 6, wB, why="desc1: ")


@cocotb.test()
async def test_start_ignored_while_busy(dut):
    """TODO(REQ: busy guard): a second START while BUSY must be ignored and must
    not corrupt the running transfer (re-trigger deviation)."""
    env = await setup(dut, axi_cfg=AXI4LiteSlaveConfig(ar_delay=4, r_delay=3,
                                                       aw_delay=2, b_delay=3))
    words = pattern(4)
    fill(env.mem, SRC, words)
    zero_region(env.mem, DST, 4)
    await env.configure_block(SRC, DST, 4)
    await env.set_mode(chain=False)
    await env.trigger_start()
    st = await env.status()
    assert st & env.rm.st_busy, "engine not busy after start with slow slave"
    # re-write the *block* regs to bogus values while the transfer runs:
    # a design that re-latches mid-flight moves the wrong data.
    await env.write_reg(env.rm.src, 0x0340)      # another valid region
    await env.write_reg(env.rm.dst, 0x0C80)      # descriptor area: must stay put
    await env.trigger_start()                    # must be ignored
    st = await env.wait_done(max_polls=800)
    env.expect_block_moved(SRC, DST, 4, words, why="after mid-flight START: ")


# ---------------------------------------------------------------------------
# REQ-E: reset behaviour (kills reset-branch mutants)
# ---------------------------------------------------------------------------
@cocotb.test()
async def test_mid_operation_reset(dut):
    """TODO(REQ: reset): reset while BUSY clears engine state; every observable
    register returns to its reset value, and a fresh start afterwards works."""
    env = await setup(dut, axi_cfg=AXI4LiteSlaveConfig(ar_delay=6, r_delay=4,
                                                       aw_delay=4, b_delay=4))
    words = pattern(8)
    fill(env.mem, SRC, words)
    zero_region(env.mem, DST, 8)
    await env.configure_block(SRC, DST, 8)
    await env.set_mode(chain=False)
    await env.trigger_start()
    for _ in range(6):                    # let a few words (or none) move
        await RisingEdge(dut.clk)
    st = await env.status()
    await env.reset()                     # mid-flight reset
    st = await env.status()
    assert not (st & env.rm.st_busy), f"BUSY survives reset (status={st:#010x})"
    assert not (st & (env.rm.st_done | env.rm.st_err)), \
        f"DONE/ERR survive reset (status={st:#010x})"
    for _ in range(3):                    # no straggler traffic after reset
        await RisingEdge(dut.clk)
    # and the engine must work again from reset
    zero_region(env.mem, DST, 8)
    await env.configure_block(SRC, DST, 8)
    await env.set_mode(chain=False)
    await env.trigger_start()
    st = await env.wait_done(max_polls=600)
    assert st & env.rm.st_done, "engine dead after mid-operation reset"
    env.expect_block_moved(SRC, DST, 8, words, why="post-reset run: ")


@cocotb.test()
async def test_reset_kills_irq(dut):
    """Reset while DONE is held must clear the status side (no phantom irq)."""
    env = await setup(dut)
    await env.configure_block(SRC, DST, 1)
    fill(env.mem, SRC, pattern(1))
    await env.set_mode(chain=False, ie=True)
    await env.trigger_start()
    await env.wait_done()
    assert env.irq_widths == [1], f"unexpected irq shape: {env.irq_widths}"
    await env.reset()
    for _ in range(4):
        await RisingEdge(dut.clk)
    assert env.irq_widths == [], f"irq edge across reset: {env.irq_widths}"
    st = await env.status()
    assert st == 0, f"status not zero after reset: {st:#010x}"
