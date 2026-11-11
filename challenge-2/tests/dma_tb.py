"""Testbench helpers for dma_ctrl: clock, reset, APB4 driver, an AXI4-Lite memory
model with optional stalls and error injection, and peripheral request drivers.
Inputs change at the falling edge of clk; outputs are sampled shortly after it.

Extensions over the probe harness (names of existing helpers preserved):
  mem.block   : set of 'ar','aw','w','r','b' forcing that channel fully stalled
                (ready=0 / valid=0) until the test unblocks it; survives reset
  mem.keep_pend : if True, pending rq/bq/aw/w are NOT discarded at reset, so the
                slave delivers late m_rvalid/m_bvalid after the DUT was reset
  mem.aw_gap_after_ack : if set to N, m_awready is forced 0 for the N cycles after
                the first observed AW handshake. (The model already holds awready
                0 until the AW+W pair merges; this knob extends that gap PAST the
                merge, so awready stays 0 for the rest of the word's write.)
                Default None = old behavior.
  mem.w_gap_after_ack  : symmetric for m_wready: forced 0 for the N cycles after
                the first observed W handshake. Default None = old behavior.
  mem.cycle   : falling-edge cycle counter (shared clock base with the APB driver)
  mem.log     : W entries carry the strobe: ("W", addr, data, strb)
  mem.dack_count / mem.dack_width : dack pulse counting and max pulse width
  violations  : added checks — pready=0 (REQ-03), arvalid&awvalid both 1 (REQ-13),
                valid/payload change while stalled (REQ-14)
"""

import random

import cocotb
from cocotb.clock import Clock
from cocotb.triggers import FallingEdge, Timer

CLK_NS = 10

ID, GCTRL, IRQ_STATUS, IRQ_ENABLE, ACTIVE, LAST_GRANT = 0x000, 0x004, 0x008, 0x00C, 0x010, 0x014
CH_CTRL, CH_SRC, CH_DST, CH_LEN, CH_NEXT, CH_STAT = 0x00, 0x04, 0x08, 0x0C, 0x10, 0x14

EN, SRC_FIX, DST_FIX, HW_REQ, LINK = 1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 6
RESP_SLVERR = 2

DESC_BIT = 8  # IRQ_STATUS[11:8] DESC[n] (CR-D2)


def ch(n, reg):
    return 0x100 + 0x20 * n + reg


def prio(p):
    return (p & 3) << 4


def done_bit(n):
    return 1 << n


def err_bit(n):
    return 1 << (4 + n)


def desc_bit(n):
    return 1 << (8 + n)


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
        self.dack_width = [0, 0, 0, 0]
        self.rq = []
        self.aw = None
        self.w = None
        self.bq = []
        self.prev = {}
        self.violations = []
        self.block = set()
        self.keep_pend = False
        self.aw_gap_after_ack = None  # None = off; N = awready=0 for N cycles after AW ack
        self._aw_gap = 0
        self.w_gap_after_ack = None   # None = off; N = wready=0 for N cycles after W ack
        self._w_gap = 0
        self.cycle = 0
        self.irq_seen_at = None
        self.b_accept = None
        self.apb_wr_cycles = []
        self._dack_prev = 0
        self._dack_start = [0] * 4

    def load(self, base, words):
        for i, w in enumerate(words):
            self.mem[base + 4 * i] = w & 0xFFFFFFFF

    def dump(self, base, n):
        return [self.mem.get(base + 4 * i, 0) for i in range(n)]

    def writes(self):
        return [e for e in self.log if e[0] == "W"]

    def reads(self):
        return [e for e in self.log if e[0] == "R"]

    def check_aligned_strb(self):
        """REQ-12: all AXI addresses word aligned, all write strobes 4'hF."""
        for e in self.log:
            if e[0] == "R":
                assert e[1] % 4 == 0, f"unaligned read address 0x{e[1]:08X}"
            else:
                assert e[1] % 4 == 0, f"unaligned write address 0x{e[1]:08X}"
                assert e[3] == 0xF, f"m_wstrb={e[3]:X} != F at 0x{e[1]:08X}"

    def _ready(self):
        return 0 if self.rng.random() < self.stall else 1

    def _stable(self, name, valid, ready, payload):
        p = self.prev.get(name)
        if p is not None and (not valid or payload != p):
            self.violations.append(
                f"{name}: valid or payload changed while stalled (cycle {self.cycle}, "
                f"prev={p}, now={payload})")
        self.prev[name] = payload if (valid and not ready) else None

    async def run(self):
        d = self.dut
        while True:
            await FallingEdge(d.clk)
            self.cycle += 1
            rst_now = int(d.rst_n.value) == 0
            if rst_now:
                self.prev = {}
                if not self.keep_pend:
                    self.rq, self.bq, self.aw, self.w = [], [], None, None
            if not rst_now:
                if int(d.pready.value) != 1:
                    self.violations.append(f"pready=0 at cycle {self.cycle}")
            ar_blk = "ar" in self.block
            aw_blk = "aw" in self.block
            w_blk = "w" in self.block
            r_blk = "r" in self.block
            b_blk = "b" in self.block
            d.m_arready.value = 0 if ar_blk else self._ready()
            d.m_awready.value = 0 if (aw_blk or self.aw is not None or self._aw_gap > 0) else self._ready()
            if self._aw_gap > 0:
                self._aw_gap -= 1
            d.m_wready.value = 0 if (w_blk or self.w is not None or self._w_gap > 0) else self._ready()
            if self._w_gap > 0:
                self._w_gap -= 1
            rv = 1 if (self.rq and not r_blk and self._ready()) else 0
            d.m_rvalid.value = rv
            if rv:
                addr = self.rq[0]
                d.m_rdata.value = self.mem.get(addr, 0)
                d.m_rresp.value = RESP_SLVERR if addr in self.read_err else 0
            else:
                d.m_rresp.value = 0
            bv = 1 if (self.bq and not b_blk and self._ready()) else 0
            d.m_bvalid.value = bv
            d.m_bresp.value = (self.bq[0] if bv else 0)
            await Timer(1, unit="ns")
            ar_v, ar_r = int(d.m_arvalid.value), int(d.m_arready.value)
            aw_v, aw_r = int(d.m_awvalid.value), int(d.m_awready.value)
            w_v, w_r = int(d.m_wvalid.value), int(d.m_wready.value)
            if ar_v and aw_v:
                self.violations.append(f"m_arvalid and m_awvalid both 1 at cycle {self.cycle}")
            if not rst_now:
                self._stable("ar", ar_v, ar_r, int(d.m_araddr.value) if ar_v else None)
                self._stable("aw", aw_v, aw_r, int(d.m_awaddr.value) if aw_v else None)
                self._stable("w", w_v, w_r, (int(d.m_wdata.value), int(d.m_wstrb.value)) if w_v else None)
            if ar_v and ar_r:
                addr = int(d.m_araddr.value)
                self.rq.append(addr)
                self.log.append(("R", addr))
            if aw_v and aw_r:
                self.aw = int(d.m_awaddr.value)
                if self.aw_gap_after_ack is not None and self._aw_gap == 0:
                    self._aw_gap = self.aw_gap_after_ack
            if w_v and w_r:
                self.w = (int(d.m_wdata.value), int(d.m_wstrb.value))
                if self.w_gap_after_ack is not None and self._w_gap == 0:
                    self._w_gap = self.w_gap_after_ack
            if rv and int(d.m_rready.value):
                self.rq.pop(0)
            if bv and int(d.m_bready.value):
                self.bq.pop(0)
                self.b_accept = self.cycle
            if self.aw is not None and self.w is not None:
                addr, (data, strb) = self.aw, self.w
                self.aw = self.w = None
                self.log.append(("W", addr, data, strb))
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
                    if not self._dack_prev >> n & 1:
                        self._dack_start[n] = self.cycle
                    width = self.cycle - self._dack_start[n] + 1
                    if width > self.dack_width[n]:
                        self.dack_width[n] = width
            self._dack_prev = dack
            if self.irq_seen_at is None and int(d.irq.value):
                self.irq_seen_at = self.cycle
            if (int(d.psel.value) and int(d.penable.value) and int(d.pwrite.value)
                    and int(d.pready.value)):
                self.apb_wr_cycles.append(self.cycle)


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
        await Timer(2, unit="ns")  # drive after the model's edge sampling: no same-edge race
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

    async def wait_cycles(self, n):
        for _ in range(n):
            await FallingEdge(self.dut.clk)

    def check_bus(self):
        assert not self.mem.violations, self.mem.violations[:3]


async def race_start(tb, n, src, dst):
    """Arm one REQ-34-style race: reset, IRQ_ENABLE[done n] set, bus frozen with the
    'b' block, channel n programmed LEN=1 LINK=0 and started; returns once its AW+W
    are accepted so the OKAY B response is pending (channel parked in write-resp)."""
    await tb.reset()
    tb.mem.block = {"b"}
    await tb.write(IRQ_ENABLE, done_bit(n))
    tb.mem.load(src, [0x5A5A0000 | n])
    mark = len(tb.mem.log)
    await tb.write(GCTRL, 1)
    await tb.program(n, src, dst, 1, EN)
    while not any(e[0] == "W" for e in tb.mem.log[mark:]):
        await FallingEdge(tb.dut.clk)


async def race_fire(tb, pre, offset, value=None, strb=0xF):
    """Wait `pre` cycles, unfreeze the bus, wait `offset` more cycles, then issue
    an IRQ_STATUS write (if value given), then wait for irq to rise. The memory
    model records, on ONE shared timeline (mem.cycle): the cycle the B response
    was accepted, the first cycle irq reads 1, and every sampled APB write-access
    cycle. A write whose access cycle equals the irq-rise cycle is exactly
    same-cycle with the status-set event. Returns (readback, k_irq, j_write)."""
    m = tb.mem
    m.irq_seen_at, m.b_accept, m.apb_wr_cycles = None, None, []
    for _ in range(pre):
        await FallingEdge(tb.dut.clk)
    m.block = set()
    for _ in range(offset):
        await FallingEdge(tb.dut.clk)
    if value is not None:
        await tb.write(IRQ_STATUS, value, strb)
    while m.irq_seen_at is None:
        await FallingEdge(tb.dut.clk)
        assert m.cycle < 20000, "race event never arrived"
    rb = await tb.read(IRQ_STATUS)
    return rb, m.irq_seen_at, (m.apb_wr_cycles[-1] if m.apb_wr_cycles else None)


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
