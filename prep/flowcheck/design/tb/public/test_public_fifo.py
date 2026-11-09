"""Public (informational) testbench for the synthetic flowcheck design."""

import cocotb
from cocotb.clock import Clock
from cocotb.triggers import FallingEdge, RisingEdge

CLK_NS = 10


async def _reset(dut, cycles=3):
    dut.rst_n.value = 0
    dut.wr_en.value = 0
    dut.rd_en.value = 0
    dut.wdata.value = 0
    for _ in range(cycles):
        await RisingEdge(dut.clk)
    dut.rst_n.value = 1
    await FallingEdge(dut.clk)


@cocotb.test()
async def test_public_basic(dut):
    cocotb.start_soon(Clock(dut.clk, CLK_NS, unit="ns").start())
    await _reset(dut)

    assert int(dut.empty.value) == 1
    for i in range(4):
        dut.wdata.value = 0x1000 + i
        dut.wr_en.value = 1
        await RisingEdge(dut.clk)
        dut.wr_en.value = 0
        await FallingEdge(dut.clk)

    assert int(dut.count.value) == 4, int(dut.count.value)
    for i in range(4):
        dut.rd_en.value = 1
        await RisingEdge(dut.clk)
        dut.rd_en.value = 0
        await FallingEdge(dut.clk)
    assert int(dut.empty.value) == 1
    assert int(dut.count.value) == 0
