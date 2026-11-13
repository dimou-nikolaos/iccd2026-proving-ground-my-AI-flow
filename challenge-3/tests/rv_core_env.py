"""rv_core test environment: RvTB + wb_* commit-stream recorder + ISS glue.

The DUT-side write stream is sampled from the rv_core pipeline registers
wb_valid_q / wb_en_q / wb_rd_q / wb_data_q (visible with --public-flat-rw).
Each retiring register write asserts wb_valid&&wb_en for exactly one cycle,
so one sample per falling edge captures every regfile write exactly once.
Inputs are driven after the falling edge; wb_* is sampled at each falling
edge before driving.

The golden model is rv32_iss.RV32ISS configured per spec.md (see SPEC_CFG);
mcycle/mcycleh read values are never compared (implementation-defined cycles).
"""

from dataclasses import replace
import re

import cocotb
from cocotb.clock import Clock
from cocotb.triggers import FallingEdge, Timer

from asm import assemble
from diff import Ev, Step, first_divergence, instr_class
from randprog import GenConfig, generate, trap_config
from rv32_iss import RV32ISS, RV32Config
from rv_tb import CLK_NS, RvTB
from rv_asm import DEVICE_IRQ, SIG_BASE, TOHOST

MASK32 = 0xFFFFFFFF

SPEC_CFG = RV32Config(misaligned="fault", mtval_illegal="instr",
                      ro_csr_write="illegal", mpp_on_mret=3, zba_zbb=True,
                      mtvec_vectored=False, counter_csrs=True,
                      mret_irq_shadow=0)

UNPINNED_CSRS = (0xB00, 0xB80)          # mcycle / mcycleh: values not comparable
MIX_WAIT_PATTERNS = ((0.0, 0.0, 17), (0.3, 0.3, 23))

DUMP_REGS = [r for r in range(1, 32) if r != 2]

_CLOCK_TASKS = {}


class Env(RvTB):
    """RvTB with wb-write recording, always-on REQ-07/08 checks, and a
    one-clock-per-test guarantee so several programs can share one dut."""

    def __init__(self, dut, seed=1, istall=0.0, dstall=0.0):
        super().__init__(dut, seed=seed, istall=istall, dstall=dstall)
        self.writes = []

    async def start(self):
        d = self.dut
        task = _CLOCK_TASKS.get(id(d))
        if task is None or task.done():
            task = cocotb.start_soon(Clock(d.clk, CLK_NS, unit="ns").start(start_high=False))
            _CLOCK_TASKS[id(d)] = task
        d.rst_n.value = 0
        d.imem_ready.value = 0
        d.imem_rdata.value = 0
        d.dmem_ready.value = 0
        d.dmem_rdata.value = 0
        d.irq_ext.value = 0
        d.irq_timer.value = 0
        for _ in range(3):
            await FallingEdge(d.clk)
        d.rst_n.value = 1

    def _sample_wb(self):
        d = self.dut
        v = d.wb_valid_q.value
        e = d.wb_en_q.value
        if v.is_resolvable and e.is_resolvable and int(v) and int(e):
            rd = int(d.wb_rd_q.value)
            if rd != 0:
                self.writes.append((rd, int(d.wb_data_q.value)))

    async def step(self):
        d = self.dut
        await FallingEdge(d.clk)
        self._sample_wb()
        self.cycles += 1
        await Timer(1, unit="ns")
        iv = int(d.imem_valid.value)
        if iv:
            assert int(d.imem_addr.value) & 3 == 0, "REQ-08: imem_addr not word aligned"
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
            assert addr & 3 == 0, "REQ-08: dmem_addr not word aligned"
            if not we:
                assert strb == 0, "REQ-07: load with dmem_wstrb != 0"
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

    async def reset_pulse(self, cycles=3):
        """Assert rst_n low for `cycles` clock cycles, inputs quiesced."""
        d = self.dut
        d.rst_n.value = 0
        self.prev_i = None
        self.prev_d = None
        for _ in range(cycles):
            await FallingEdge(d.clk)
            d.imem_ready.value = 0
            d.dmem_ready.value = 0
            d.imem_rdata.value = 0
            d.dmem_rdata.value = 0
        await FallingEdge(d.clk)
        d.rst_n.value = 1
        self.prev_i = None
        self.prev_d = None

    async def run_checked(self, timeout=12000):
        while not self.done:
            await self.step()
            assert self.cycles < timeout, "program did not finish in %d cycles" % timeout
        assert not self.violations, self.violations[:3]


class ISSCore(RV32ISS):
    """ISS with sparse memory and the TB device protocol (TOHOST halt store,
    DEVICE_IRQ store driving the interrupt lines)."""

    def __init__(self, zba_zbb=True):
        super().__init__(mem_size=4096, config=replace(SPEC_CFG, zba_zbb=zba_zbb))
        self.smem = {}

    def read_mem(self, addr, size):
        return sum(self.smem.get((addr + i) & MASK32, 0) << (8 * i) for i in range(size))

    def write_mem(self, addr, value, size, check=True):
        addr &= MASK32
        if any(((addr + i) & ~3) == TOHOST for i in range(size)):
            self.halted = True
            return
        if any(((addr + i) & ~3) == DEVICE_IRQ for i in range(size)):
            self.set_external_irq(11, bool(value & 1))
            self.set_external_irq(7, bool((value >> 1) & 1))
            return
        for i in range(size):
            self.smem[(addr + i) & MASK32] = (value >> (8 * i)) & 0xFF

    def fetch32(self, addr):
        return self.read_mem(addr, 4)


def run_iss(iss, words, max_steps=200000):
    for a, w in words:
        iss.load_words(a & ~3, [w])
    steps = 0
    while not iss.halted and steps < max_steps:
        iss.step()
        steps += 1
    if not iss.halted:
        raise AssertionError("ISS did not halt within %d steps" % max_steps)
    return iss


def epilogue_text(sig=SIG_BASE):
    """Dump x1, x3..x31 via sp to the SIG window, then halt at TOHOST."""
    lines = ["        li   sp, %d" % sig]
    off = 0
    for r in range(1, 32):
        if r == 2:
            continue
        lines.append("        sw   x%d, %d(sp)" % (r, off))
        off += 4
    lines.append("        li   sp, %d" % TOHOST)
    lines.append("        sw   x0, 0(sp)")
    lines.append("haltlp:")
    lines.append("        j    haltlp")
    return "\n".join(lines)


def rand_source(cfg):
    src = generate(cfg)
    # mcycle/minstret values are implementation-defined cycles: a program that
    # READS them into a GPR makes later dependent results incomparable between
    # ISS and RTL. Redirect those CSR names to misa (pinned value per spec;
    # still RO, so the write-illegal policy class is preserved).
    src = re.sub(r"\bmcycleh?\b|\bminstreth?\b", "misa", src)
    if "\n        j done\n" not in src:
        raise ValueError("generator self-loop not found in generated program")
    return src.replace("\n        j done\n", "\n" + epilogue_text() + "\n", 1)


def write_stream(iss):
    out = []
    for c in iss.trace:
        if c.rd:
            unpinned = ((c.instr & 0x7F) == 0x73
                        and ((c.instr >> 20) & 0xFFF) in UNPINNED_CSRS)
            out.append((c, unpinned))
    return out


def commit_compare(env, iss, label):
    exp = write_stream(iss)
    got = env.writes
    ref_steps, dut_steps = [], []
    n = max(len(exp), len(got))
    for i in range(n):
        c, unp = exp[i] if i < len(exp) else (None, False)
        g = got[i] if i < len(got) else None
        pc = c.pc if c else 0
        instr = c.instr if c else None
        ev_ref = (Ev("rd", c.rd, 0 if unp else c.wdata),) if c and c.rd else ()
        ev_dut = (Ev("rd", g[0], 0 if unp else g[1]),) if g else ()
        ref_steps.append(Step(pc, instr, ev_ref))
        dut_steps.append(Step(pc, None, ev_dut))
    d = first_divergence(ref_steps, dut_steps)
    if d is not None:
        ctx = []
        for j in range(max(0, d.index - 2), min(len(exp), d.index + 3)):
            c, _ = exp[j]
            ctx.append("  #%d %06x %08x %s" % (j, c.pc, c.instr, instr_class(c.instr)))
        raise AssertionError(
            "%s: ISS-vs-RTL commit stream mismatch (%d ISS writes, %d RTL writes)\n%s\n"
            "context:\n%s" % (label, len(exp), len(got), d.brief(), "\n".join(ctx)))


def final_compare(env, iss, label):
    vals = env.sig(len(DUMP_REGS))
    for i, r in enumerate(DUMP_REGS):
        if vals[i] != iss.regs[r]:
            raise AssertionError("%s: final x%d: RTL %08x != ISS %08x"
                                 % (label, r, vals[i], iss.regs[r]))
    keys = set(env.mem)
    for baddr in iss.smem:
        keys.add(baddr & ~3)
    for a in sorted(keys):
        iw = 0
        for k in range(4):
            iw |= iss.smem.get(a + k, 0) << (8 * k)
        if env.mem.get(a, 0) != iw:
            raise AssertionError("%s: mem[%08x]: RTL %08x != ISS %08x"
                                 % (label, a, env.mem.get(a, 0), iw))


async def run_lockstep(dut, cfg, istall, dstall, tb_seed, max_cycles=60000):
    src = rand_source(cfg)
    prog = assemble(src, base=0)
    words = [(a & ~3, w) for a, w in prog.words]
    iss = run_iss(ISSCore(), words)
    env = Env(dut, seed=tb_seed, istall=istall, dstall=dstall)
    for a, w in words:
        env.mem[a] = w
    await env.start()
    await env.run_checked(timeout=max_cycles)
    label = "seed=%d waits=(%.1f,%.1f)" % (cfg.seed, istall, dstall)
    commit_compare(env, iss, label)
    final_compare(env, iss, label)
    return env
