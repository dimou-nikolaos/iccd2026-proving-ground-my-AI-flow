"""REQ-42 async-reset semantic test on FIR delay_q specifically.

REQ-42: rst_n must be an ASYNCHRONOUS reset. A short rst_n pulse MUST clear
all registers without requiring a clk edge.

Challenge-1 risk: sub-16 wildcard g2 changed FIR delay_q from async reset
(`always_ff @(posedge clk or negedge rst_n)`) to sync reset
(`always_ff @(posedge clk) ... if (!rst_n)`). This test detects that.

PASSES if RTL on FIR delay_q is async. FAILS if sync.
"""
import cocotb
from cocotb.triggers import RisingEdge, Timer, ReadOnly
from cocotb.clock import Clock


async def write_reg(dut, addr, data):
    await RisingEdge(dut.clk)
    dut.cfg_we.value = 1
    dut.cfg_addr.value = addr
    dut.cfg_wdata.value = data
    await RisingEdge(dut.clk)
    dut.cfg_we.value = 0


@cocotb.test()
async def test_async_reset_fir_delay(dut):
    """Drop rst_n mid-cycle; FIR delay_q must clear without clk edge."""
    cocotb.start_soon(Clock(dut.clk, 10, units="ns").start())
    dut.rst_n.value = 0
    dut.cfg_we.value = 0
    dut.cfg_addr.value = 0
    dut.cfg_wdata.value = 0
    dut.m_ready.value = 0
    dut.s_valid.value = 0
    dut.s_data.value = 0
    dut.s_last.value = 0
    for _ in range(3):
        await RisingEdge(dut.clk)
    dut.rst_n.value = 1
    for _ in range(3):
        await RisingEdge(dut.clk)

    # CTRL @ word 0x01 → addr 0x004. ENABLE=1, FIR_BYPASS=0, ROUND=0.
    await write_reg(dut, 0x004, 0x01)
    # NTAPS @ word 0x06 → addr 0x018. 1 tap → FIR is 1 cycle per sample.
    await write_reg(dut, 0x018, 0x01)
    for _ in range(3):
        await RisingEdge(dut.clk)

    # Stream 4 samples (gain 3-stage pipeline + FIR 1-cycle each ≈ 5-7 cycle latency).
    for sample in [0x1234, 0x2222, 0x3333, 0x4444]:
        await RisingEdge(dut.clk)
        dut.s_valid.value = 1
        dut.s_data.value = sample
        dut.s_last.value = 0
    await RisingEdge(dut.clk)
    dut.s_valid.value = 0
    # Wait for FIR to absorb samples into its delay line.
    for _ in range(15):
        await RisingEdge(dut.clk)
    await Timer(1, units="ns")
    await ReadOnly()

    dq0 = int(dut.u_fir.delay_q[0].value)
    dq1 = int(dut.u_fir.delay_q[1].value)
    dut._log.info(f"delay_q[0]={dq0:#x} delay_q[1]={dq1:#x}")
    assert (dq0 != 0) or (dq1 != 0), (
        f"delay_q stayed 0 after streaming FIR samples — setup issue (dq0={dq0:#x} dq1={dq1:#x})"
    )

    # Leave ReadOnly phase before driving rst_n.
    await Timer(1, units="ns")
    # Drop rst_n mid-cycle (no clk edge between now and check).
    dut.rst_n.value = 0
    await Timer(2, units="ns")
    await ReadOnly()

    dq0_after = int(dut.u_fir.delay_q[0].value)
    dut._log.info(f"delay_q[0] after mid-cycle rst_n=0 (no clk edge) = {dq0_after:#x}")
    assert dq0_after == 0, (
        f"REQ-42 VIOLATION: delay_q[0] = {dq0_after:#x} after mid-cycle rst_n=0 "
        f"(expected 0). RTL uses SYNC reset on FIR delay_q."
    )
