"""Negative matrix: every seeded deviation in the fixture must be CAUGHT.

Run inside the flow image:
    cd prep/dma_vip && make negative

This is the same discipline as `make negative` in prep/bus_vip and
prep/rvcore, applied to the *test suite itself*: before trusting "all tests
pass" as evidence that the released design is clean, we prove that the suite
detects each deviation class the playbook predicts (D1 length off-by-one,
D7 chain-end, D8 descriptor race, D10 RO writes, plus status/irq/addressing
lifecycle bugs). A probe returns True when it *observes* the deviation; the
test fails if a seeded bug goes unnoticed.

One build runs the whole matrix: on the fixture `bug` is a plain input port.
On a real design you instead patch the RTL (or use mutant builds) - the probe
bodies below are what you reuse.
"""

import os

import cocotb

import test_dma as td
from axi4lite import AXI4LiteSlaveConfig
from dma_env import DmaEnv, DmaTimeout, Regmap

AXI_NAMES = td.AXI_NAMES
SRC, DST, DESC = td.SRC, td.DST, td.DESC

# Two-stage negative run (see Makefile): the bug5 case intentionally lets the
# bound dma_irq_sva fire, which aborts the sim - so stage 1 runs everything
# else with assertions bound (nothing may fire, all probes must catch), and
# stage 2 runs ONLY the irq case and requires the assertion in the log.
_ONLY = os.environ.get("DMA_NEG_ONLY")
_SKIP = os.environ.get("DMA_NEG_SKIP")


def _select(name: str) -> bool:
    if _ONLY:
        return name in _ONLY.split(",")
    if _SKIP:
        return name not in _SKIP.split(",")
    return True


def test(fn):
    """cocotb.test wrapper honouring the DMA_NEG_ONLY/SKIP selectors."""
    if not _select(fn.__name__):
        return fn
    return cocotb.test()(fn)


async def _env(dut, bug: int, axi_cfg=None) -> DmaEnv:
    env = DmaEnv(dut, Regmap(), axi_names=AXI_NAMES, axi_cfg=axi_cfg)
    await env.start(bug=bug)
    return env


async def _move_block(env, length: int, src=SRC, dst=DST, mode=False) -> None:
    words = td.pattern(length)
    td.fill(env.mem, src, words)
    td.zero_region(env.mem, dst, length + 2)
    await env.configure_block(src, dst, length)
    await env.set_mode(chain=mode)
    await env.trigger_start()
    await env.wait_done()


def _data_corrupted(env, length: int, src=SRC, dst=DST) -> bool:
    want = td.pattern(length)
    got = [env.mem.peek_word(dst + 4 * i) for i in range(length)]
    return got != want


# ---------------------------------------------------------------------------
# bug0: LEN off-by-one (playbook D1)
# ---------------------------------------------------------------------------
@test
async def neg_len_off_by_one(dut):
    """bug0 must show up as wrong destination content at a LEN boundary."""
    env = await _env(dut, bug=1 << 0)
    await _move_block(env, 4)
    assert _data_corrupted(env, 4), "LEN off-by-one NOT detected by the block test"


# ---------------------------------------------------------------------------
# bug1: read-only register accepts writes (D10)
# ---------------------------------------------------------------------------
@test
async def neg_ro_write_accepted(dut):
    env = await _env(dut, bug=1 << 1)
    detected = False
    try:
        await env.write_ro_expect_unchanged(env.rm.dev_id, 0xFFFF_FFFF)
    except AssertionError:
        detected = True
    assert detected, "RO write corruption NOT detected"


# ---------------------------------------------------------------------------
# bug2: chain end ignored - hang (D7)
# ---------------------------------------------------------------------------
@test
async def neg_chain_end_hang(dut):
    env = await _env(dut, bug=1 << 2)
    d0 = DESC
    await env.program_descriptor(d0, SRC, DST, 2, 0)     # single, chain ends here
    td.fill(env.mem, SRC, td.pattern(2))
    await env.write_reg(env.rm.desc, d0)
    await env.set_mode(chain=True)
    await env.trigger_start()
    hung = False
    try:
        await env.wait_done(max_polls=120)
    except DmaTimeout:
        hung = True
    assert hung, "chain-end hang NOT detected (transfer 'completed')"


# ---------------------------------------------------------------------------
# bug3: DONE never set / BUSY stuck
# ---------------------------------------------------------------------------
@test
async def neg_done_never_set(dut):
    env = await _env(dut, bug=1 << 3)
    # BUSY never clears, so wait_done itself is the hang probe: accept the
    # timeout, then confirm the *status signature* (BUSY stuck, DONE never set).
    hung = False
    try:
        await _move_block(env, 2)
    except DmaTimeout:
        hung = True
    st = await env.status()
    detected = hung and bool(st & env.rm.st_busy) and not (st & env.rm.st_done)
    assert detected, f"missing DONE / stuck BUSY NOT detected (hung={hung}, st={st:#010x})"


# ---------------------------------------------------------------------------
# bug4: descriptor race - chain follows a not-yet-fetched `next` (D8)
# ---------------------------------------------------------------------------
@test
async def neg_descriptor_race(dut):
    """With the race, the chain follows next=0 (stale) instead of the programmed
    pointer: the second descriptor's block never moves."""
    env = await _env(dut, bug=1 << 4)
    wA, wB = td.pattern(2, 0x31), td.pattern(2, 0x32)
    srcA, dstA, srcB, dstB = 0x0200, 0x0A00, 0x0300, 0x0B00
    td.fill(env.mem, srcA, wA)
    td.fill(env.mem, srcB, wB)
    td.zero_region(env.mem, dstA, 2)
    td.zero_region(env.mem, dstB, 2)
    d0, d1 = DESC, DESC + 0x10
    await env.program_descriptor(d0, srcA, dstA, 2, d1)
    await env.program_descriptor(d1, srcB, dstB, 2, 0)
    await env.write_reg(env.rm.desc, d0)
    await env.set_mode(chain=True)
    await env.trigger_start()
    await env.wait_done()
    second_moved = [env.mem.peek_word(dstB + 4 * i) for i in range(2)] == wB
    assert not second_moved, "descriptor race NOT detected (chain worked anyway)"


# ---------------------------------------------------------------------------
# bug5: level irq instead of a pulse
# ---------------------------------------------------------------------------
@test
async def neg_irq_level(dut):
    env = await _env(dut, bug=1 << 5)
    # IE must be ON for the irq to be observable at all.
    words = td.pattern(2)
    td.fill(env.mem, SRC, words)
    await env.configure_block(SRC, DST, 2)
    await env.set_mode(chain=False, ie=True)
    await env.trigger_start()
    await env.wait_done()
    # a pulse is over by now; a *level* is still high (nothing here writes
    # STATUS, so DONE stays set and the level - if any - is still asserted).
    for _ in range(6):
        await cocotb.triggers.RisingEdge(dut.clk)
    await cocotb.triggers.ReadOnly()
    level_high = int(env.irq.value) == 1
    widths = list(env.irq_widths)
    assert level_high or any(w > 1 for w in widths), \
        f"level irq NOT detected (widths={widths}, level={level_high})"


# ---------------------------------------------------------------------------
# bug6: narrowed write strobes
# ---------------------------------------------------------------------------
@test
async def neg_narrow_strobes(dut):
    env = await _env(dut, bug=1 << 6)
    await _move_block(env, 4)
    detected = False
    try:
        env.expect_all_strobes(DST, DST + 16)
    except AssertionError:
        detected = True
    assert detected, "narrowed WSTRB NOT detected"


# ---------------------------------------------------------------------------
# bug7: wrong address stride
# ---------------------------------------------------------------------------
@test
async def neg_wrong_stride(dut):
    env = await _env(dut, bug=1 << 7)
    await _move_block(env, 4)
    detected = False
    try:
        env.expect_write_addresses(DST, 4)
    except AssertionError:
        detected = True
    # the data itself is wrong too, since reads stride as well
    assert detected or _data_corrupted(env, 4), "wrong stride NOT detected"
