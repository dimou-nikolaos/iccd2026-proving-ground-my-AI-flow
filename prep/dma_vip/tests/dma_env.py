"""Design-agnostic DMA test environment (challenge-2 prep, item E).

One cocotb env object wires together everything a DMA test needs:

  - `APB4Master` (prep/bus_vip) on the config port,
  - `AXI4LiteSlave` memory model (prep/bus_vip) behind the data master,
    with poke/peek, transaction log, backpressure and error injection,
  - a register-map dataclass (`Regmap`) so the *same* test bodies run against
    the fixture today and the released design on day 1 - only the map changes,
  - an irq monitor that records pulse widths,
  - helpers: `reset()`, `write_reg/read_reg`, `start_block()`, `wait_done()`.

Release-day adoption (the whole point of item E):
  1. copy `test_dma.py` bodies into `<design>/tests/`, keep this file beside them
     (the flow puts `tests/` on pythonpath, helpers import normally);
  2. fill the `TODO(SPEC)` fields of `Regmap` from the released register table;
  3. remap `AXI_NAMES`/`APB_NAMES` if the released port names differ;
  4. the per-test `TODO(REQ)` tags name which requirement each body covers -
     re-tag them to the released REQ ids in the req-matrix.

The env never assumes internal signals: everything is checked at the bus
boundary (memory content + transaction log + APB-visible status), which is all
the hidden tests can see either.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import List, Optional, Tuple

import cocotb
from cocotb.clock import Clock
from cocotb.triggers import FallingEdge, ReadOnly, RisingEdge

from apb4 import APB4Master
from axi4lite import AXI4LiteSlave, AXI4LiteSlaveConfig

CLK_NS = 10


# ---------------------------------------------------------------------------
# Register map abstraction - fill from the released spec on day 1
# ---------------------------------------------------------------------------
@dataclass
class Regmap:
    """Address/encoding of the config space. Fixture values shown; TODO(SPEC)."""

    ctrl: int = 0x000      # TODO(SPEC): names/offsets from the released map
    status: int = 0x004
    src: int = 0x008
    dst: int = 0x00C
    length: int = 0x010    # TODO(SPEC): words or bytes? (the D1-class question)
    desc: int = 0x014      # descriptor pointer (chain mode)
    next_: int = 0x018     # RO: last fetched next pointer
    dev_id: int = 0x01C    # RO ID register

    # bit positions inside ctrl/status
    en_bit: int = 0
    start_bit: int = 1
    reload_bit: int = 2    # TODO(SPEC): does RELOAD exist at all?
    mode_bit: int = 3      # 1 = descriptor/chain mode
    ie_bit: int = 4
    busy_bit: int = 0
    done_bit: int = 1
    err_bit: int = 2

    id_value: Optional[int] = 0xD1A4_0000   # TODO(SPEC): expected ID, or None

    # word index of src/dst/len/next inside a descriptor
    desc_fields: Tuple[int, int, int, int] = (0, 1, 2, 3)

    @property
    def ctrl_en(self) -> int:
        return 1 << self.en_bit

    @property
    def ctrl_start(self) -> int:
        return 1 << self.start_bit

    @property
    def ctrl_mode(self) -> int:
        return 1 << self.mode_bit

    @property
    def ctrl_ie(self) -> int:
        return 1 << self.ie_bit

    @property
    def st_busy(self) -> int:
        return 1 << self.busy_bit

    @property
    def st_done(self) -> int:
        return 1 << self.done_bit

    @property
    def st_err(self) -> int:
        return 1 << self.err_bit


# ---------------------------------------------------------------------------
# Environment
# ---------------------------------------------------------------------------
class DmaTimeout(AssertionError):
    """Raised by wait_done when BUSY never clears (hang-class deviations)."""


class DmaEnv:
    def __init__(self, dut, rm: Optional[Regmap] = None, mem_size: int = 1 << 16,
                 axi_cfg: Optional[AXI4LiteSlaveConfig] = None,
                 apb_names: Optional[dict] = None,
                 axi_names: Optional[dict] = None,
                 irq_name: str = "irq_done", bug_name: Optional[str] = "bug"):
        self.dut = dut
        self.rm = rm or Regmap()
        self.clk = dut.clk
        self.rst_n = dut.rst_n
        self.apb = APB4Master(dut, self.clk, names=apb_names)
        self.mem = AXI4LiteSlave(dut, self.clk, rst_n=self.rst_n,
                                 names=axi_names, mem_size=mem_size,
                                 config=axi_cfg)
        self.irq = getattr(dut, irq_name, None)
        self.bug = getattr(dut, bug_name, None) if bug_name else None
        self.irq_widths: List[int] = []
        self._clock_task = None
        self._irq_task = None

    # -- lifecycle ---------------------------------------------------------
    async def start(self, bug: int = 0, rst_cycles: int = 4) -> None:
        """Clock + reset. `bug` only exists on the fixture; None-port safe."""
        if self._clock_task is None:
            self._clock_task = cocotb.start_soon(Clock(self.clk, CLK_NS, unit="ns").start())
        self.mem.start()
        if self.bug is not None:
            self.bug.value = bug
        await self.reset(rst_cycles)
        if self.irq is not None:
            self._irq_task = cocotb.start_soon(self._irq_monitor())

    async def reset(self, cycles: int = 4) -> None:
        """Full reset, mid-test included (reset-branch mutants need that)."""
        self.rst_n.value = 0
        self.apb.idle()
        for _ in range(cycles):
            await RisingEdge(self.clk)
        await FallingEdge(self.clk)
        self.rst_n.value = 1
        await FallingEdge(self.clk)
        self.irq_widths.clear()

    async def _irq_monitor(self) -> None:
        """Record the width (in cycles) of every irq pulse.

        Samples at falling edges: combinational outputs are settled in that
        window and the DUT re-samples at the next rising edge.
        """
        high = 0
        while True:
            await FallingEdge(self.clk)
            try:
                v = int(self.irq.value)
            except ValueError:            # X during/just after reset
                if high:
                    self.irq_widths.append(high)
                    high = 0
                continue
            if v and not high:
                high = 1
            elif v:
                high += 1
            elif high:
                self.irq_widths.append(high)
                high = 0

    # -- registers ---------------------------------------------------------
    async def write_reg(self, addr: int, value: int, strb: int = 0xF) -> None:
        await self.apb.write_expect_ok(addr, value, strb)

    async def read_reg(self, addr: int) -> int:
        return await self.apb.read_expect_ok(addr)

    async def status(self) -> int:
        return await self.read_reg(self.rm.status)

    async def write_ro_expect_unchanged(self, addr: int, value: int) -> None:
        """Write a read-only register; it must not corrupt anything visible."""
        before = await self.read_reg(addr)
        await self.apb.write(addr, value)
        after = await self.read_reg(addr)
        assert after == before, \
            f"RO write @0x{addr:03x} changed read-back: {before:#010x} -> {after:#010x}"

    # -- programming -------------------------------------------------------
    async def configure_block(self, src: int, dst: int, words: int) -> None:
        await self.write_reg(self.rm.src, src)
        await self.write_reg(self.rm.dst, dst)
        await self.write_reg(self.rm.length, words)

    async def program_descriptor(self, desc_addr: int, src: int, dst: int,
                                 length: int, next_ptr: int) -> None:
        i_src, i_dst, i_len, i_next = self.rm.desc_fields
        words = [0] * 4
        words[i_src] = src
        words[i_dst] = dst
        words[i_len] = length
        words[i_next] = next_ptr
        for i, w in enumerate(words):
            self.mem.poke_word(desc_addr + 4 * i, w)

    async def set_mode(self, chain: bool, ie: bool = False) -> None:
        """Enable + select plain/descriptor mode + interrupt enable."""
        val = self.rm.ctrl_en | (self.rm.ctrl_mode if chain else 0) | \
            (self.rm.ctrl_ie if ie else 0)
        await self.write_reg(self.rm.ctrl, val)

    async def trigger_start(self) -> None:
        """Pulse START while keeping EN/MODE/IE as configured."""
        cur = await self.read_reg(self.rm.ctrl)
        keep = cur & (self.rm.ctrl_en | self.rm.ctrl_mode | self.rm.ctrl_ie)
        await self.write_reg(self.rm.ctrl, keep | self.rm.ctrl_start)

    # -- completion --------------------------------------------------------
    async def wait_done(self, max_polls: int = 200) -> int:
        """Poll STATUS until BUSY clears, then let outstanding AXI traffic drain.

        Raises DmaTimeout if BUSY is still set after `max_polls` accesses:
        hang-class deviations (chain-end ignored, BUSY stuck) must fail *loudly*
        and *quickly* rather than eat the whole sim timeout.

        The drain (`mem.wait_idle` + settle cycles) matters: BUSY can clear a
        couple of cycles before the last B-response write is *applied* inside
        the memory model - reading memory before the drain is an observation
        race, not a DUT bug.
        """
        st = 0
        for _ in range(max_polls):
            st = await self.status()
            if not (st & self.rm.st_busy):
                await self.mem.wait_idle(timeout=200)
                for _ in range(3):
                    await RisingEdge(self.clk)
                return st
            await RisingEdge(self.clk)
        raise DmaTimeout(f"BUSY still set after {max_polls} polls, last status={st:#010x}")

    # -- golden checks at the bus boundary ---------------------------------
    def expect_block_moved(self, src: int, dst: int, words: int, pattern: List[int],
                           why: str = "") -> None:
        got = [self.mem.peek_word(dst + 4 * i) for i in range(words)]
        assert got == pattern[:words], (
            f"{why}dst 0x{dst:04x} mismatch after moving {words} word(s) from "
            f"0x{src:04x}:\n  want {[f'{w:#010x}' for w in pattern[:words]]}"
            f"\n  got  {[f'{w:#010x}' for w in got]}")

    def data_writes_in(self, lo: int, hi: int) -> List[dict]:
        return [t for t in self.mem.log
                if t["kind"] == "write" and lo <= t["addr"] <= hi]

    def expect_write_addresses(self, base: int, words: int, stride: int = 4,
                               why: str = "") -> None:
        """Every expected destination word must have been written at its own
        address - catches wrong-stride and merged-write deviations even when
        the final memory content happens to look right."""
        addrs = sorted({t["addr"] for t in self.data_writes_in(base, base + stride * words)})
        want = [base + stride * i for i in range(words)]
        assert addrs == want, (
            f"{why}write addresses to 0x{base:04x}: want {want}, got {addrs}")

    def expect_all_strobes(self, lo: int, hi: int, why: str = "") -> None:
        """A full-word DMA write must strobe all four byte lanes."""
        for t in self.data_writes_in(lo, hi):
            assert t["strb"] == 0xF, \
                f"{why}write @0x{t['addr']:04x} strobe {t['strb']:b}, expected 0b1111"

    def expect_irq_pulses(self, count: int = 1, why: str = "") -> None:
        widths = list(self.irq_widths)
        assert len(widths) == count, \
            f"{why}expected {count} irq pulse(s), saw {len(widths)}: widths={widths}"
        for w in widths:
            assert w == 1, f"{why}irq pulse width {w} cycles, expected a 1-cycle pulse"
