"""Scratch negative-check test (NOT part of the submission).

Drives dma_ctrl with a heavily backpressured AXI slave so the REQ-14 valid/payload-hold
assertions are exercised non-vacuously: 0 fires on the fixed RTL, and the brk_arhold
variant (S_RD_ADDR advances without m_arready) must fire them.
"""

import cocotb

from dma_tb import DmaTB


@cocotb.test()
async def test_req14_long_stalls(dut):
    tb = DmaTB(dut, seed=11, stall=0.8)
    await tb.start()
    tb.mem.load(0x0000, [0x11223344 + i for i in range(8)])
    await tb.program(0, 0x0000, 0x0100, 8, ctrl=1)
    await tb.write(0x0004, 1)  # GEN = 1
    await tb.wait_idle()
    got = tb.mem.dump(0x0100, 8)
    assert got == [0x11223344 + i for i in range(8)], got
    tb.check_bus()
