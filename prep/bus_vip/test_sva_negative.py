"""Negative SVA test: a broken APB master must trigger the bound protocol assertion.

The test body itself is trivial; the point is the log. `make negative` greps the
simulation log for an "APB4:" assertion message and fails if it is absent.
"""

import cocotb
from cocotb.clock import Clock
from cocotb.triggers import RisingEdge


@cocotb.test()
async def run_broken_master(dut):
    cocotb.start_soon(Clock(dut.clk, 10, unit="ns").start())
    dut.rst_n.value = 0
    for _ in range(3):
        await RisingEdge(dut.clk)
    dut.rst_n.value = 1
    for _ in range(25):
        await RisingEdge(dut.clk)
    dut._log.info("negative-test stimulus finished")
