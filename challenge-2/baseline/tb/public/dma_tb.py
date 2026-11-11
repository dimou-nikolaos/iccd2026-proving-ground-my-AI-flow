"""Testbench helpers for dma_ctrl: clock, reset, APB4 driver, an AXI4-Lite memory
model with optional stalls and error injection, and peripheral request drivers.
Inputs change at the falling edge of clk; outputs are sampled shortly after it."""

import random

import cocotb
from cocotb.clock import Clock
from cocotb.triggers import FallingEdge, Timer

CLK_NS = 10

ID, GCTRL, IRQ_STATUS, IRQ_ENABLE, ACTIVE, LAST_GRANT = 0x000, 0x004, 0x008, 0x00C, 0x010, 0x014
CH_CTRL, CH_SRC, CH_DST, CH_LEN, CH_NEXT, CH_STAT = 0x00, 0x04, 0x08, 0x0C, 0x10, 0x14

EN, SRC_FIX, DST_FIX, HW_REQ, LINK = 1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 6
RESP_SLVERR = 2


def ch(n, reg):
    return 0x100 + 0x20 * n + reg


def prio(p):
    return (p & 3) << 4


def done_bit(n):
    return 1 << n


def err_bit(n):
    return 1 << (4 + n)


class Memory:
    """AXI4-Lite memory, word addressed by byte address. `log` records accesses."""

    def __init__(self, dut, seed=1, stall=0.0):
        self.dut = dut
        self.rng = random.Random(seed)
        self.stall = stall
        self.mem = {}
        self.log = []
        self.read_err = set()
        self.write_err = set()
        self.dack_count = [0, 0, 0, 0]
        self.rq = []
        self.aw = None
        self.w = None
        self.bq = []
        self.prev = {}
        self.violations = []

    def load(self, base, words):
        for i, w in enumerate(words):
            self.mem[base + 4 * i] = w & 0xFFFFFFFF

    def dump(self, base, n):
        return [self.mem.get(base + 4 * i, 0) for i in range(n)]

    def writes(self):
        return [e for e in self.log if e[0] == "W"]

    def _ready(self):
        return 0 if self.rng.random() < self.stall else 1

    def _stable(self, name, valid, ready, payload):
        p = self.prev.get(name)
        if p is not None and (not valid or payload != p):
            self.violations.append(f"{name}: valid or payload changed while stalled")
        self.prev[name] = payload if (valid and not ready) else None

    async def run(self):
        d = self.dut
        while True:
            await FallingEdge(d.clk)
            if not int(d.rst_n.value):
                self.rq, self.bq, self.aw, self.w, self.prev = [], [], None, None, {}
            d.m_arready.value = self._ready()
            d.m_awready.value = self._ready() if self.aw is None else 0
            d.m_wready.value = self._ready() if self.w is None else 0
            rv = 1 if self.rq and self._ready() else 0
            d.m_rvalid.value = rv
            if rv:
                addr = self.rq[0]
                d.m_rdata.value = self.mem.get(addr, 0)
                d.m_rresp.value = RESP_SLVERR if addr in self.read_err else 0
            else:
                d.m_rresp.value = 0
            bv = 1 if self.bq and self._ready() else 0
            d.m_bvalid.value = bv
            d.m_bresp.value = (self.bq[0] if bv else 0)
            await Timer(1, unit="ns")
            ar_v, ar_r = int(d.m_arvalid.value), int(d.m_arready.value)
            aw_v, aw_r = int(d.m_awvalid.value), int(d.m_awready.value)
            w_v, w_r = int(d.m_wvalid.value), int(d.m_wready.value)
            self._stable("ar", ar_v, ar_r, int(d.m_araddr.value) if ar_v else None)
            self._stable("aw", aw_v, aw_r, int(d.m_awaddr.value) if aw_v else None)
            self._stable("w", w_v, w_r, (int(d.m_wdata.value), int(d.m_wstrb.value)) if w_v else None)
            if ar_v and ar_r:
                addr = int(d.m_araddr.value)
                self.rq.append(addr)
                self.log.append(("R", addr))
            if aw_v and aw_r:
                self.aw = int(d.m_awaddr.value)
            if w_v and w_r:
                self.w = (int(d.m_wdata.value), int(d.m_wstrb.value))
            if rv and int(d.m_rready.value):
                self.rq.pop(0)
            if bv and int(d.m_bready.value):
                self.bq.pop(0)
            if self.aw is not None and self.w is not None:
                addr, (data, strb) = self.aw, self.w
                self.aw = self.w = None
                self.log.append(("W", addr, data))
                old = self.mem.get(addr, 0)
                mask = sum(0xFF << (8 * i) for i in range(4) if strb >> i & 1)
                if addr in self.write_err:
                    self.bq.append(RESP_SLVERR)
                else:
                    self.mem[addr] = (old & ~mask) | (data & mask)
                    self.bq.append(0)
            dack = int(d.dack.value)
            for n in range(4):
                if dack >> n & 1:
                    self.dack_count[n] += 1


class DmaTB:
    def __init__(self, dut, seed=1, stall=0.0):
        self.dut = dut
        self.mem = Memory(dut, seed, stall)
        self.last_err = 0

    async def start(self):
        d = self.dut
        d.rst_n.value = 0
        for name in ("psel", "penable", "pwrite", "paddr", "pwdata", "pstrb", "m_awready", "m_wready",
                     "m_bvalid", "m_bresp", "m_arready", "m_rvalid", "m_rdata", "m_rresp", "dreq"):
            getattr(d, name).value = 0
        cocotb.start_soon(Clock(d.clk, CLK_NS, unit="ns").start(start_high=False))
        cocotb.start_soon(self.mem.run())
        for _ in range(3):
            await FallingEdge(d.clk)
        d.rst_n.value = 1
        await FallingEdge(d.clk)

    async def reset(self):
        d = self.dut
        d.rst_n.value = 0
        await FallingEdge(d.clk)
        await FallingEdge(d.clk)
        d.rst_n.value = 1
        await FallingEdge(d.clk)

    async def write(self, addr, value, strb=0xF):
        d = self.dut
        d.psel.value = 1
        d.penable.value = 0
        d.pwrite.value = 1
        d.paddr.value = addr
        d.pwdata.value = value & 0xFFFFFFFF
        d.pstrb.value = strb
        await FallingEdge(d.clk)
        d.penable.value = 1
        await Timer(1, unit="ns")
        self.last_err = int(d.pslverr.value)
        await FallingEdge(d.clk)
        d.psel.value = 0
        d.penable.value = 0
        d.pwrite.value = 0

    async def read(self, addr):
        d = self.dut
        d.psel.value = 1
        d.penable.value = 0
        d.pwrite.value = 0
        d.paddr.value = addr
        await FallingEdge(d.clk)
        d.penable.value = 1
        await Timer(1, unit="ns")
        val = int(d.prdata.value)
        self.last_err = int(d.pslverr.value)
        await FallingEdge(d.clk)
        d.psel.value = 0
        d.penable.value = 0
        return val

    async def program(self, n, src, dst, length, ctrl=0, nxt=0):
        await self.write(ch(n, CH_SRC), src)
        await self.write(ch(n, CH_DST), dst)
        await self.write(ch(n, CH_LEN), length)
        await self.write(ch(n, CH_NEXT), nxt)
        await self.write(ch(n, CH_CTRL), ctrl)

    async def wait_idle(self, cycles=20000):
        for _ in range(cycles):
            if await self.read(ACTIVE) == 0:
                return
        raise AssertionError("channels still active")

    def check_bus(self):
        assert not self.mem.violations, self.mem.violations[:3]


async def peripheral(tb, n, count):
    """Hold dreq[n] until dack[n] is seen, `count` times."""
    d = tb.dut
    for _ in range(count):
        start = tb.mem.dack_count[n]
        d.dreq.value = int(d.dreq.value) | 1 << n
        while tb.mem.dack_count[n] == start:
            await FallingEdge(d.clk)
        d.dreq.value = int(d.dreq.value) & ~(1 << n)
        await FallingEdge(d.clk)
        await FallingEdge(d.clk)
