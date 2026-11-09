"""Participant evidence suite for the synthetic flowcheck design.

Sampling discipline (same lesson as the bus VIP BFMs):
  * drive on falling edges,
  * read combinational flags with ReadOnly() while the request is asserted,
  * read registered state at the *next* falling edge (the rising edge in
    between is where the DUT commits).

Kept short (a few hundred ns) so the per-mutant timeout stays at its 30 s floor.
"""

import cocotb
from cocotb.clock import Clock
from cocotb.triggers import FallingEdge, RisingEdge, ReadOnly

CLK_NS = 10
DEPTH = 8


async def _reset(dut, cycles=3):
    dut.rst_n.value = 0
    dut.wr_en.value = 0
    dut.rd_en.value = 0
    dut.wdata.value = 0
    for _ in range(cycles):
        await RisingEdge(dut.clk)
    dut.rst_n.value = 1
    await FallingEdge(dut.clk)


async def _write(dut, value):
    """Assert one write; return (overflow, full) as seen during the request."""
    await FallingEdge(dut.clk)
    dut.wdata.value = value
    dut.wr_en.value = 1
    await ReadOnly()
    flags = (int(dut.overflow.value), int(dut.full.value))
    await RisingEdge(dut.clk)          # DUT commits here
    dut.wr_en.value = 0
    await FallingEdge(dut.clk)         # state settled; safe to sample
    return flags


async def _read(dut):
    """Assert one read; return (rdata, underflow, empty)."""
    await FallingEdge(dut.clk)
    dut.rd_en.value = 1
    await ReadOnly()
    flags = (int(dut.underflow.value), int(dut.empty.value))
    await RisingEdge(dut.clk)          # DUT commits here
    dut.rd_en.value = 0
    await FallingEdge(dut.clk)
    return int(dut.rdata.value), flags[0], flags[1]


@cocotb.test()
async def test_fill_drain_order(dut):
    cocotb.start_soon(Clock(dut.clk, CLK_NS, unit="ns").start())
    await _reset(dut)
    assert int(dut.count.value) == 0
    assert int(dut.empty.value) == 1
    assert int(dut.full.value) == 0

    for i in range(DEPTH):
        ovf, full = await _write(dut, 0xA000 + i)
        assert ovf == 0 and full == 0, (i, ovf, full)
        assert int(dut.count.value) == i + 1, (i, int(dut.count.value))

    assert int(dut.full.value) == 1
    assert int(dut.empty.value) == 0

    # writing while full must flag overflow and not change the count
    ovf, full = await _write(dut, 0xDEAD)
    assert (ovf, full) == (1, 1), (ovf, full)
    assert int(dut.count.value) == DEPTH

    for i in range(DEPTH):
        got, unf, empty = await _read(dut)
        assert unf == 0 and empty == 0, (i, unf, empty)
        assert got == 0xA000 + i, (i, hex(got))

    assert int(dut.empty.value) == 1
    assert int(dut.full.value) == 0

    # reading while empty must flag underflow and keep the count at zero
    _, unf, empty = await _read(dut)
    assert (unf, empty) == (1, 1), (unf, empty)
    assert int(dut.count.value) == 0


@cocotb.test()
async def test_wraparound(dut):
    cocotb.start_soon(Clock(dut.clk, CLK_NS, unit="ns").start())
    await _reset(dut)
    # three laps of five elements: pointers wrap past DEPTH twice
    for lap in range(3):
        for i in range(5):
            await _write(dut, 0x5000 + lap * 16 + i)
        for i in range(5):
            got, _, _ = await _read(dut)
            assert got == 0x5000 + lap * 16 + i, (lap, i, hex(got))
        assert int(dut.empty.value) == 1
        assert int(dut.count.value) == 0


@cocotb.test()
async def test_reset_midrun(dut):
    """Reset in the middle of activity must restore every state element.

    The flow starts registers at zero (`+verilator+rand+reset+0`), so a design
    whose reset branch is dead looks correct at time 0. Removing the reset
    branch (a `drop_term` mutant) is only visible with a mid-run reset.
    """
    cocotb.start_soon(Clock(dut.clk, CLK_NS, unit="ns").start())
    await _reset(dut)
    for i in range(5):
        await _write(dut, 0x7000 + i)
    assert int(dut.count.value) == 5
    assert int(dut.total_written.value) == 5

    dut.rst_n.value = 0
    await RisingEdge(dut.clk)
    await FallingEdge(dut.clk)
    assert int(dut.count.value) == 0
    assert int(dut.empty.value) == 1
    assert int(dut.full.value) == 0
    assert int(dut.total_written.value) == 0
    assert int(dut.rdata.value) == 0, hex(int(dut.rdata.value))

    dut.rst_n.value = 1
    await FallingEdge(dut.clk)
    for i in range(3):
        await _write(dut, 0x8000 + i)
    for i in range(3):
        got, _, _ = await _read(dut)
        assert got == 0x8000 + i, hex(got)


@cocotb.test()
async def test_write_counter(dut):
    cocotb.start_soon(Clock(dut.clk, CLK_NS, unit="ns").start())
    await _reset(dut)
    assert int(dut.total_written.value) == 0
    for i in range(DEPTH + 3):
        await _write(dut, i)
    # only DEPTH writes were accepted
    assert int(dut.total_written.value) == DEPTH, int(dut.total_written.value)
    assert int(dut.count.value) == DEPTH
