"""
AXI4-Lite master and slave(memory) BFMs for cocotb (design-agnostic).

Challenge-2's DUT exposes an AXI4-Lite **master**, so the slave memory model below is what
we place behind it. The master BFM is provided for VIP self-test and for designs that
expose an AXI4-Lite slave port.

Modelled rules:
  - independent AW / W / AR channels (AW and W may arrive in either order, and on
    different cycles),
  - VALID held stable until the corresponding READY,
  - B only after both AW and W are accepted (one response per AW),
  - R response holds RData/RResp until RREADY,
  - configurable per-channel READY/VALID delays, jitter and error responses
    (SLVERR=2, DECERR=3).

Drive/sample discipline (important)
-----------------------------------
The master drives VALID at a **falling** edge, observes READY with `ReadOnly()` in the
window that ends at the next rising edge, and keeps VALID asserted through the
completion rising edge before deasserting. The slave model mirrors this: it drives its
outputs at each falling edge from its Python state, samples handshakes with `ReadOnly()`
and updates state for the next cycle. Sampling post-edge values instead would miss
handshakes and cause spurious timeouts.

Usage (slave behind a DUT master):

    mem = AXI4LiteSlave(dut, dut.aclk, dut.aresetn,
                        names=dict(awvalid="m_awvalid", ...))
    mem.start()
    ... run DUT ...
    assert mem.peek_word(0x1000) == 0xDEADBEEF
    mem.log   # list of transactions: kind/addr/data/strb/resp
"""

from __future__ import annotations

import random
from collections import deque
from dataclasses import dataclass, field
from typing import Deque, Dict, List, Optional, Tuple

import cocotb
from cocotb.triggers import FallingEdge, ReadOnly, RisingEdge

DEFAULT_AXI_NAMES = {
    "awvalid": "awvalid", "awready": "awready", "awaddr": "awaddr", "awprot": "awprot",
    "wvalid": "wvalid", "wready": "wready", "wdata": "wdata", "wstrb": "wstrb",
    "bvalid": "bvalid", "bready": "bready", "bresp": "bresp",
    "arvalid": "arvalid", "arready": "arready", "araddr": "araddr", "arprot": "arprot",
    "rvalid": "rvalid", "rready": "rready", "rdata": "rdata", "rresp": "rresp",
}

RESP_OKAY = 0
RESP_SLVERR = 2
RESP_DECERR = 3


@dataclass
class AXI4LiteSlaveConfig:
    aw_delay: int = 0
    w_delay: int = 0
    ar_delay: int = 0
    b_delay: int = 0
    r_delay: int = 0
    jitter: int = 0
    seed: int = 0
    err_ranges: List[Tuple[int, int, int]] = field(default_factory=list)
    # RESP value held on rresp/bresp while the channel is idle (RESP_OKAY=0 is
    # the usual choice; a nonzero value emulates a slave that leaves an error
    # code latched between transfers - RRESP/BRESP are don't-care when VALID is
    # low, so compliant masters must not act on it; response-decode mutants
    # that ignore the VALID qualifier do).
    resp_when_idle: int = 0


class AXI4LiteSlave:
    """AXI4-Lite slave memory model with configurable backpressure / errors."""

    def __init__(self, dut, clk, rst_n=None, names: Optional[Dict[str, str]] = None,
                 mem_size: int = 1 << 16, config: Optional[AXI4LiteSlaveConfig] = None):
        self.dut = dut
        self.clk = clk
        self.rst_n = rst_n
        self.cfg = config or AXI4LiteSlaveConfig()
        self.rng = random.Random(self.cfg.seed)
        self.mem = bytearray(mem_size)
        self.log: List[dict] = []
        self.io: Dict[str, object] = {}
        for logical, default in DEFAULT_AXI_NAMES.items():
            actual = (names or {}).get(logical, default)
            if actual is not None and hasattr(dut, actual):
                self.io[logical] = getattr(dut, actual)
        for required in ("awvalid", "awready", "awaddr", "wvalid", "wready", "wdata",
                         "bvalid", "bready", "arvalid", "arready", "araddr",
                         "rvalid", "rready", "rdata"):
            if required not in self.io:
                raise AttributeError(f"AXI4LiteSlave: DUT is missing signal '{required}'")
        self._task = None
        self._init_state()
        self._drive()

    # -- helpers ---------------------------------------------------------
    def _init_state(self) -> None:
        self.aw_q: Deque[int] = deque()
        self.w_q: Deque[Tuple[int, int]] = deque()
        self.ar_q: Deque[int] = deque()
        self.b_q: Deque[int] = deque()                  # write responses
        self.r_q: Deque[Tuple[int, int]] = deque()      # (data, resp)
        self.aw_t = self.cfg.aw_delay
        self.w_t = self.cfg.w_delay
        self.ar_t = self.cfg.ar_delay
        self.b_t = self.cfg.b_delay
        self.r_t = self.cfg.r_delay

    def start(self) -> None:
        self._task = cocotb.start_soon(self._run())

    def _jitter(self) -> int:
        return self.rng.randint(0, self.cfg.jitter) if self.cfg.jitter else 0

    # -- memory access ---------------------------------------------------
    def resp_for(self, addr: int) -> int:
        for lo, hi, resp in self.cfg.err_ranges:
            if lo <= addr <= hi:
                return resp
        return RESP_OKAY

    def _read_word(self, addr: int) -> int:
        addr &= ~0x3
        if addr + 4 > len(self.mem):
            return 0
        return int.from_bytes(self.mem[addr:addr + 4], "little")

    def _write_word(self, addr: int, data: int, strb: int) -> int:
        addr &= ~0x3
        for i in range(4):
            if strb & (1 << i):
                if addr + i < len(self.mem):
                    self.mem[addr + i] = (data >> (8 * i)) & 0xFF
        return self.resp_for(addr)

    def poke_word(self, addr: int, value: int) -> None:
        self.mem[addr:addr + 4] = (value & 0xFFFF_FFFF).to_bytes(4, "little")

    def peek_word(self, addr: int) -> int:
        return self._read_word(addr)

    def poke_bytes(self, addr: int, data: bytes) -> None:
        self.mem[addr:addr + len(data)] = data

    # -- main loop -------------------------------------------------------
    def _drive(self) -> None:
        """Drive outputs for the cycle that ends at the next rising edge."""
        def put(name: str, value: int) -> None:
            if name in self.io:
                self.io[name].value = value

        put("awready", 1 if self.aw_t == 0 else 0)
        put("wready", 1 if self.w_t == 0 else 0)
        put("arready", 1 if self.ar_t == 0 else 0)
        put("bvalid", 1 if (self.b_q and self.b_t == 0) else 0)
        put("bresp", self.b_q[0] if self.b_q else self.cfg.resp_when_idle)
        put("rvalid", 1 if (self.r_q and self.r_t == 0) else 0)
        if self.r_q:
            put("rdata", self.r_q[0][0])
            put("rresp", self.r_q[0][1])
        else:
            put("rdata", 0)
            put("rresp", self.cfg.resp_when_idle)

    async def _run(self) -> None:
        while True:
            await FallingEdge(self.clk)

            if self.rst_n is not None and not int(self.rst_n.value):
                self._init_state()
                self._drive()
                continue

            self._drive()
            await ReadOnly()

            awv = int(self.io["awvalid"].value)
            wv = int(self.io["wvalid"].value)
            arv = int(self.io["arvalid"].value)
            bv = int(self.io["bvalid"].value)
            rv = int(self.io["rvalid"].value)
            brdy = int(self.io["bready"].value)
            rrdy = int(self.io["rready"].value)

            aw_hs = awv and int(self.io["awready"].value)
            w_hs = wv and int(self.io["wready"].value)
            ar_hs = arv and int(self.io["arready"].value)
            b_hs = bv and brdy
            r_hs = rv and rrdy

            awaddr = int(self.io["awaddr"].value)
            wdata = int(self.io["wdata"].value)
            wstrb = int(self.io["wstrb"].value) if "wstrb" in self.io else 0xF
            araddr = int(self.io["araddr"].value)

            # ---- timers: one cycle elapses ----
            if self.aw_t > 0:
                self.aw_t -= 1
            if self.w_t > 0:
                self.w_t -= 1
            if self.ar_t > 0:
                self.ar_t -= 1
            if self.b_t > 0:
                self.b_t -= 1
            if self.r_t > 0:
                self.r_t -= 1

            # ---- captures and responses ----
            if aw_hs:
                self.aw_q.append(awaddr)
                self.aw_t = self.cfg.aw_delay + self._jitter()
            if w_hs:
                self.w_q.append((wdata, wstrb))
                self.w_t = self.cfg.w_delay + self._jitter()
            if ar_hs:
                self.ar_q.append(araddr)
                self.ar_t = self.cfg.ar_delay + self._jitter()
            if b_hs:
                if self.b_q:
                    self.b_q.popleft()
                self.b_t = self.cfg.b_delay + self._jitter()
            if r_hs:
                if self.r_q:
                    self.r_q.popleft()
                self.r_t = self.cfg.r_delay + self._jitter()

            # ---- pair AW with W, then apply the write (B is ordered behind B) ----
            while self.aw_q and self.w_q:
                addr = self.aw_q.popleft()
                data, strb = self.w_q.popleft()
                resp = self._write_word(addr, data, strb)
                was_empty = not self.b_q
                self.b_q.append(resp)
                if was_empty:
                    self.b_t = self.cfg.b_delay + self._jitter()
                self.log.append(dict(kind="write", addr=addr & ~0x3, data=data,
                                     strb=strb, resp=resp))

            # ---- serve reads ----
            while self.ar_q:
                addr = self.ar_q.popleft()
                data = self._read_word(addr)
                resp = self.resp_for(addr)
                was_empty = not self.r_q
                self.r_q.append((data, resp))
                if was_empty:
                    self.r_t = self.cfg.r_delay + self._jitter()
                self.log.append(dict(kind="read", addr=addr & ~0x3, data=data, resp=resp))

    # -- waiting helpers for tests --------------------------------------
    async def wait_idle(self, timeout: int = 1000) -> None:
        for _ in range(timeout):
            await RisingEdge(self.clk)
            if (not self.aw_q and not self.w_q and not self.ar_q and
                    not self.b_q and not self.r_q):
                return
        raise TimeoutError("AXI4LiteSlave did not go idle")


class AXI4LiteMaster:
    """AXI4-Lite master BFM (for VIP self-test and AXI-slave DUTs)."""

    def __init__(self, dut, clk, rst_n=None, names: Optional[Dict[str, str]] = None):
        self.dut = dut
        self.clk = clk
        self.rst_n = rst_n
        self.io: Dict[str, object] = {}
        for logical, default in DEFAULT_AXI_NAMES.items():
            actual = (names or {}).get(logical, default)
            if actual is not None and hasattr(dut, actual):
                self.io[logical] = getattr(dut, actual)
        self.idle()

    def _put(self, name: str, value: int) -> None:
        if name in self.io:
            self.io[name].value = value

    def idle(self) -> None:
        for name in ("awvalid", "wvalid", "arvalid", "bready", "rready"):
            self._put(name, 0)
        self._put("awaddr", 0)
        self._put("wdata", 0)
        self._put("wstrb", 0)
        self._put("araddr", 0)

    async def _wait_ready(self, valid: str, ready: str, timeout: int, what: str) -> None:
        for _ in range(timeout):
            await ReadOnly()
            if int(self.io[ready].value):
                await RisingEdge(self.clk)      # completion edge, VALID still held
                await FallingEdge(self.clk)
                self._put(valid, 0)
                return
            await RisingEdge(self.clk)          # still waiting: keep VALID asserted
            await FallingEdge(self.clk)
        self.idle()
        raise TimeoutError(f"AXI4LiteMaster: {what} timeout")

    # -- write channel ---------------------------------------------------
    async def _send_aw(self, addr: int, timeout: int) -> None:
        await FallingEdge(self.clk)
        self._put("awaddr", addr)
        self._put("awvalid", 1)
        await self._wait_ready("awvalid", "awready", timeout, "AW")

    async def _send_w(self, data: int, strb: int, timeout: int) -> None:
        await FallingEdge(self.clk)
        self._put("wdata", data)
        self._put("wstrb", strb)
        self._put("wvalid", 1)
        await self._wait_ready("wvalid", "wready", timeout, "W")

    async def _wait_response(self, valid: str, ready: str, payload: Tuple[str, ...],
                             timeout: int, what: str) -> Tuple[int, ...]:
        await FallingEdge(self.clk)
        self._put(ready, 1)
        for _ in range(timeout):
            await ReadOnly()
            if int(self.io[valid].value):
                values = tuple(int(self.io[p].value) if p in self.io else 0
                               for p in payload)
                await RisingEdge(self.clk)      # response accepted
                await FallingEdge(self.clk)
                self._put(ready, 0)
                return values
            await RisingEdge(self.clk)
            await FallingEdge(self.clk)
        self.idle()
        raise TimeoutError(f"AXI4LiteMaster: {what} timeout")

    async def write(self, addr: int, data: int, strb: int = 0xF, timeout: int = 500) -> int:
        aw = cocotb.start_soon(self._send_aw(addr, timeout))
        w = cocotb.start_soon(self._send_w(data, strb, timeout))
        await aw
        await w
        (resp,) = await self._wait_response("bvalid", "bready", ("bresp",), timeout, "B")
        return resp

    async def read(self, addr: int, timeout: int = 500) -> Tuple[int, int]:
        await FallingEdge(self.clk)
        self._put("araddr", addr)
        self._put("arvalid", 1)
        await self._wait_ready("arvalid", "arready", timeout, "AR")
        data, resp = await self._wait_response("rvalid", "rready", ("rdata", "rresp"),
                                               timeout, "R")
        return data, resp
