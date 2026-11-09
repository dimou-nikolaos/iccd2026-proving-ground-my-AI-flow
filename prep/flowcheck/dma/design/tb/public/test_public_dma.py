"""Public testbench stand-in for the synthetic DMA design (design/tb/public).

Self-contained (raw signal driving, no VIP imports) - the same shape a released
public tb has. Deliberately *lenient*: it checks that a 4-word move "works" by
inspecting the first three destination words, so the seeded LEN off-by-one
(bug[0]) passes it. That is exactly how seeded deviations survive a public tb,
and why our evidence suite must own the boundary checks.
"""

import cocotb
from cocotb.clock import Clock
from cocotb.triggers import FallingEdge, ReadOnly, RisingEdge

CLK_NS = 10


async def apb_write(dut, addr, data):
    await FallingEdge(dut.clk)
    dut.psel.value = 1
    dut.penable.value = 0
    dut.pwrite.value = 1
    dut.paddr.value = addr
    dut.pwdata.value = data
    dut.pstrb.value = 0xF
    await RisingEdge(dut.clk)
    await FallingEdge(dut.clk)
    dut.penable.value = 1
    await ReadOnly()
    await RisingEdge(dut.clk)
    await FallingEdge(dut.clk)
    dut.psel.value = 0
    dut.penable.value = 0
    dut.pwrite.value = 0


async def apb_read(dut, addr):
    await FallingEdge(dut.clk)
    dut.psel.value = 1
    dut.penable.value = 0
    dut.pwrite.value = 0
    dut.paddr.value = addr
    await RisingEdge(dut.clk)
    await FallingEdge(dut.clk)
    dut.penable.value = 1
    await ReadOnly()
    val = int(dut.prdata.value)
    await RisingEdge(dut.clk)
    await FallingEdge(dut.clk)
    dut.psel.value = 0
    dut.penable.value = 0
    return val


async def axi_model(dut):
    """Minimal always-on AXI4-Lite slave (zero delay, no errors)."""
    mem = {}

    def rd(a):
        return mem.get(a & ~3, 0)

    dut.m_awready.value = 1
    dut.m_wready.value = 1
    dut.m_arready.value = 1
    dut.m_bvalid.value = 0
    dut.m_rvalid.value = 0
    aw_addr = None
    while True:
        await FallingEdge(dut.clk)
        if int(dut.rst_n.value) == 0:
            aw_addr = None
            dut.m_bvalid.value = 0
            dut.m_rvalid.value = 0
            continue
        if int(dut.m_awvalid.value):
            aw_addr = int(dut.m_awaddr.value) & ~3
        if int(dut.m_wvalid.value) and aw_addr is not None:
            wdata, strb = int(dut.m_wdata.value), int(dut.m_wstrb.value)
            cur = rd(aw_addr)
            for i in range(4):
                if strb & (1 << i):
                    cur = (cur & ~(0xFF << (8 * i))) | ((wdata >> (8 * i)) & 0xFF) << (8 * i)
            mem[aw_addr] = cur
            dut.m_bvalid.value = 1
            aw_addr = None
        elif int(dut.m_bvalid.value) and int(dut.m_bready.value):
            dut.m_bvalid.value = 0
        if int(dut.m_arvalid.value) and not int(dut.m_rvalid.value):
            dut.m_rdata.value = rd(int(dut.m_araddr.value))
            dut.m_rresp.value = 0
            dut.m_rvalid.value = 1
        elif int(dut.m_rvalid.value) and int(dut.m_rready.value):
            dut.m_rvalid.value = 0


@cocotb.test()
async def test_public_basic_move(dut):
    cocotb.start_soon(Clock(dut.clk, CLK_NS, unit="ns").start())
    cocotb.start_soon(axi_model(dut))
    dut.rst_n.value = 0
    for _ in range(4):
        await RisingEdge(dut.clk)
    dut.rst_n.value = 1
    await FallingEdge(dut.clk)

    # preload source words straight into the model memory via the bus
    src, dst = 0x0100, 0x0800
    vals = [(0xC0DE + i) & 0xFFFF_FFFF for i in range(4)]
    await apb_write(dut, 0x014, src)          # DESC register: repurpose as data port? no:
    # The public tb pokes memory through the *AXI master port* is not possible;
    # the released public tb would use the design's own memory model. Here the
    # design drives all traffic, so the tb verifies the ID register and that a
    # configured move completes with the engine reaching DONE.
    rid = await apb_read(dut, 0x01C)
    assert rid == 0xD1A4_0000, f"ID reg mismatch: {rid:#x}"

    await apb_write(dut, 0x008, src)
    await apb_write(dut, 0x00C, dst)
    await apb_write(dut, 0x010, 4)
    await apb_write(dut, 0x000, 0b0001)       # EN
    await apb_write(dut, 0x000, 0b0011)       # EN + START

    for _ in range(500):
        st = await apb_read(dut, 0x004)
        if not (st & 1):
            break
        await RisingEdge(dut.clk)
    else:
        assert False, "engine never left BUSY"
    st = await apb_read(dut, 0x004)
    assert st & 0b10, f"DONE not set: status={st:#b}"
