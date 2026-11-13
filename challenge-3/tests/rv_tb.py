"""Testbench helpers for rv_core: clock, reset and a shared memory with optional
wait states. Inputs change shortly after the falling edge of clk."""

import random

import cocotb
from cocotb.clock import Clock
from cocotb.triggers import FallingEdge, Timer

from rv_asm import DEVICE_IRQ, SIG_BASE, TOHOST

CLK_NS = 10


class RvTB:
    def __init__(self, dut, seed=1, istall=0.0, dstall=0.0):
        self.dut = dut
        self.rng = random.Random(seed)
        self.istall = istall
        self.dstall = dstall
        self.mem = {}
        self.done = False
        self.cycles = 0
        self.store_log = []
        self.load_log = []
        self.fetch_log = []
        self.violations = []
        self.prev_i = None
        self.prev_d = None

    def load(self, base, words):
        for i, w in enumerate(words):
            self.mem[(base + 4 * i) & 0xFFFFFFFC] = w & 0xFFFFFFFF

    def word(self, addr):
        return self.mem.get(addr & 0xFFFFFFFC, 0)

    def sig(self, n):
        return [self.word(SIG_BASE + 4 * i) for i in range(n)]

    async def start(self):
        d = self.dut
        d.rst_n.value = 0
        d.imem_ready.value = 0
        d.imem_rdata.value = 0
        d.dmem_ready.value = 0
        d.dmem_rdata.value = 0
        d.irq_ext.value = 0
        d.irq_timer.value = 0
        cocotb.start_soon(Clock(d.clk, CLK_NS, unit="ns").start(start_high=False))
        for _ in range(3):
            await FallingEdge(d.clk)
        d.rst_n.value = 1

    async def step(self):
        """One clock cycle of memory service."""
        d = self.dut
        await FallingEdge(d.clk)
        self.cycles += 1
        await Timer(1, unit="ns")
        iv = int(d.imem_valid.value)
        ready = 1 if iv and self.rng.random() >= self.istall else 0
        d.imem_ready.value = ready
        cur_i = int(d.imem_addr.value) if iv else None
        if self.prev_i is not None and cur_i != self.prev_i:
            self.violations.append("imem_addr changed while waiting")
        self.prev_i = cur_i if iv and not ready else None
        if iv:
            d.imem_rdata.value = self.word(cur_i)
            if ready:
                self.fetch_log.append(cur_i)
        dv = int(d.dmem_valid.value)
        dready = 1 if dv and self.rng.random() >= self.dstall else 0
        d.dmem_ready.value = dready
        if dv:
            addr = int(d.dmem_addr.value)
            we = int(d.dmem_write.value)
            strb = int(d.dmem_wstrb.value)
            data = int(d.dmem_wdata.value)
            cur_d = (addr, we, strb, data if we else 0)
            if self.prev_d is not None and cur_d != self.prev_d:
                self.violations.append("dmem request changed while waiting")
            self.prev_d = None if dready else cur_d
            d.dmem_rdata.value = self.word(addr)
            if dready:
                if we:
                    self.store_log.append((addr, strb, data))
                    if addr == TOHOST:
                        self.done = True
                    elif addr == DEVICE_IRQ:
                        d.irq_ext.value = data & 1
                        d.irq_timer.value = (data >> 1) & 1
                    else:
                        mask = sum(0xFF << (8 * i) for i in range(4) if strb >> i & 1)
                        self.mem[addr] = (self.word(addr) & ~mask) | (data & mask)
                else:
                    self.load_log.append(addr)
        else:
            self.prev_d = None

    async def run(self, timeout=20000):
        while not self.done:
            await self.step()
            assert self.cycles < timeout, "program did not finish"
        assert not self.violations, self.violations[:3]


async def run_program(dut, asm, regs=(), data=None, **kw):
    """Assemble, load and run a program; return the values of `regs` at the dump."""
    timeout = kw.pop("timeout", 20000)
    tb = RvTB(dut, **kw)
    tb.load(asm.base, asm.build())
    for base, words in (data or {}).items():
        tb.load(base, words)
    await tb.start()
    await tb.run(timeout)
    tb.values = tb.sig(len(regs))
    return tb
