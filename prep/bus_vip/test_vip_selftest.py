"""Self-test for the prep bus VIPs against the loopback DUTs in dut_loopback.sv.

Run inside the flow image:
    cd prep/bus_vip && make

Checks:
  1. APB4 master BFM vs apb_regs_dut (write/read, strobes, wait states, PSLVERR).
  2. AXI4-Lite master BFM vs axi_ram_dut (handshake, WSTRB, read-back).
  3. AXI4-Lite slave BFM vs axi_seq_master (accepts AW/W in any order, B/R responses).
"""

import cocotb
from cocotb.clock import Clock
from cocotb.triggers import RisingEdge, Timer

from apb4 import APB4Master
from axi4lite import AXI4LiteMaster, AXI4LiteSlave, AXI4LiteSlaveConfig

CLK_NS = 10


async def _reset(dut, cycles: int = 4) -> None:
    dut.rst_n.value = 0
    for name in ("apb_psel", "apb_penable", "apb_pwrite", "apb_paddr", "apb_pwdata",
                 "apb_pstrb", "apb_pprot", "ram_awvalid", "ram_awaddr", "ram_awprot",
                 "ram_wvalid", "ram_wdata", "ram_wstrb", "ram_bready",
                 "ram_arvalid", "ram_araddr", "ram_arprot", "ram_rready"):
        getattr(dut, name).value = 0
    for _ in range(cycles):
        await RisingEdge(dut.clk)
    dut.rst_n.value = 1
    for _ in range(2):
        await RisingEdge(dut.clk)


def _apb(dut) -> APB4Master:
    return APB4Master(dut, dut.clk, dut.rst_n, names=dict(
        psel="apb_psel", penable="apb_penable", pwrite="apb_pwrite",
        paddr="apb_paddr", pwdata="apb_pwdata", pstrb="apb_pstrb", pprot="apb_pprot",
        pready="apb_pready", prdata="apb_prdata", pslverr="apb_pslverr"))


def _axi_master(dut) -> AXI4LiteMaster:
    return AXI4LiteMaster(dut, dut.clk, dut.rst_n, names=dict(
        awvalid="ram_awvalid", awready="ram_awready", awaddr="ram_awaddr",
        awprot="ram_awprot", wvalid="ram_wvalid", wready="ram_wready",
        wdata="ram_wdata", wstrb="ram_wstrb", bvalid="ram_bvalid", bready="ram_bready",
        bresp="ram_bresp", arvalid="ram_arvalid", arready="ram_arready",
        araddr="ram_araddr", arprot="ram_arprot", rvalid="ram_rvalid",
        rready="ram_rready", rdata="ram_rdata", rresp="ram_rresp"))


def _axi_slave(dut) -> AXI4LiteSlave:
    return AXI4LiteSlave(dut, dut.clk, dut.rst_n, mem_size=1024, names=dict(
        awvalid="seq_awvalid", awready="seq_awready", awaddr="seq_awaddr",
        wvalid="seq_wvalid", wready="seq_wready", wdata="seq_wdata", wstrb="seq_wstrb",
        bvalid="seq_bvalid", bready="seq_bready", bresp="seq_bresp",
        arvalid="seq_arvalid", arready="seq_arready", araddr="seq_araddr",
        rvalid="seq_rvalid", rready="seq_rready", rdata="seq_rdata", rresp="seq_rresp"),
        config=AXI4LiteSlaveConfig(aw_delay=2, w_delay=1, b_delay=2, ar_delay=1,
                                   r_delay=2, jitter=1, seed=3))


@cocotb.test()
async def test_apb_master(dut):
    cocotb.start_soon(Clock(dut.clk, CLK_NS, unit="ns").start())
    await _reset(dut)
    apb = _apb(dut)

    await apb.write_expect_ok(0x00, 0xDEADBEEF)
    assert await apb.read_expect_ok(0x00) == 0xDEADBEEF

    # partial write: only lanes 0-1 update
    assert await apb.write(0x00, 0x00001234, strb=0x3) == 0
    assert await apb.read_expect_ok(0x00) == 0xDEAD1234

    await apb.write_expect_ok(0x04, 0x0BADF00D)
    assert await apb.read_expect_ok(0x04) == 0x0BADF00D

    # unmapped address -> PSLVERR, zero data
    data, err = await apb.read(0x20)
    assert err == 1 and data == 0, f"unmapped read: data={data:#x} err={err}"
    assert await apb.write(0x20, 0x1234) == 1

    # address 0x10 is the first unmapped word
    _, err = await apb.read(0x10)
    assert err == 1


@cocotb.test()
async def test_axi_master_against_ram(dut):
    cocotb.start_soon(Clock(dut.clk, CLK_NS, unit="ns").start())
    await _reset(dut)
    axi = _axi_master(dut)

    assert await axi.write(0x40, 0x12345678) == 0
    data, resp = await axi.read(0x40)
    assert (data, resp) == (0x12345678, 0), f"read-back {data:#x} resp={resp}"

    # WSTRB partial write: clear bytes 0 and 2
    assert await axi.write(0x40, 0x00000000, strb=0b0101) == 0
    data, _ = await axi.read(0x40)
    assert data == 0x12005600, f"partial write result {data:#x}"

    # sweep
    for i in range(16):
        addr = 0x100 + 4 * i
        value = (0xA5A50000 | i) & 0xFFFF_FFFF
        assert await axi.write(addr, value) == 0
    for i in range(16):
        addr = 0x100 + 4 * i
        data, _ = await axi.read(addr)
        assert data == (0xA5A50000 | i), f"@{addr:#x}: {data:#x}"


@cocotb.test()
async def test_axi_slave_against_seq_master(dut):
    cocotb.start_soon(Clock(dut.clk, CLK_NS, unit="ns").start())
    slave = _axi_slave(dut)
    await _reset(dut)
    slave.start()

    for _ in range(4000):
        await RisingEdge(dut.clk)
        if int(dut.seq_done.value):
            break
    else:
        raise TimeoutError("axi_seq_master did not finish")

    # let the last write land
    for _ in range(4):
        await RisingEdge(dut.clk)

    assert slave.peek_word(0x20) == 0xA5A51234, hex(slave.peek_word(0x20))
    assert slave.peek_word(0x30) == 0xFF00FF00, hex(slave.peek_word(0x30))
    assert int(dut.seq_rd0.value) == 0xA5A51234
    assert int(dut.seq_rd1.value) == 0xFF00FF00

    writes = [t for t in slave.log if t["kind"] == "write"]
    reads = [t for t in slave.log if t["kind"] == "read"]
    assert len(writes) == 3, f"{len(writes)} writes"
    assert len(reads) == 2, f"{len(reads)} reads"
    assert all(t["resp"] == 0 for t in slave.log)


@cocotb.test()
async def test_axi_slave_error_injection(dut):
    cocotb.start_soon(Clock(dut.clk, CLK_NS, unit="ns").start())
    cfg = AXI4LiteSlaveConfig(err_ranges=[(0x80, 0x8F, 3)])   # DECERR
    slave = AXI4LiteSlave(dut, dut.clk, dut.rst_n, mem_size=1024, names=dict(
        awvalid="seq_awvalid", awready="seq_awready", awaddr="seq_awaddr",
        wvalid="seq_wvalid", wready="seq_wready", wdata="seq_wdata", wstrb="seq_wstrb",
        bvalid="seq_bvalid", bready="seq_bready", bresp="seq_bresp",
        arvalid="seq_arvalid", arready="seq_arready", araddr="seq_araddr",
        rvalid="seq_rvalid", rready="seq_rready", rdata="seq_rdata", rresp="seq_rresp"),
        config=cfg)
    await _reset(dut)
    slave.start()
    await slave.wait_idle(4000)
    assert slave.resp_for(0x84) == 3
    assert slave.resp_for(0x40) == 0
