"""
APB4 master and slave BFMs for cocotb (design-agnostic).

APB4 rules modelled:
  - SETUP cycle: PSEL=1, PENABLE=0, control/address/data stable.
  - ACCESS cycle: PSEL=1, PENABLE=1, held until PREADY=1.
  - PSLVERR sampled only with PREADY=1 in the access phase.
  - PENABLE deasserts after the transfer.
  - Address/data/control remain stable from SETUP through ACCESS completion.

Drive/sample discipline (important)
-----------------------------------
Signals are driven at a **falling** clock edge and are held constant across the next
rising edge, where the DUT samples them. Handshake outputs (PREADY) are observed with
`ReadOnly()` immediately after the falling edge, i.e. in the window that ends at the
next rising edge. When PREADY is observed high, the BFM keeps PSEL/PENABLE asserted
through the completion rising edge and only then returns to idle. Sampling post-edge
values instead would be off by one cycle and would deassert PSEL before the DUT
commits the transfer.

All signal names are mapped through `names` so the same BFM works on any DUT with only a
name mapping, e.g.:

    apb = APB4Master(dut, dut.pclk, dut.presetn,
                     names=dict(psel="psel", penable="penable", ...))
    data, err = await apb.read(0x04)
"""

from __future__ import annotations

from typing import Dict, Optional, Tuple

from cocotb.triggers import FallingEdge, ReadOnly, RisingEdge

DEFAULT_APB_NAMES = {
    "psel": "psel",
    "penable": "penable",
    "pwrite": "pwrite",
    "paddr": "paddr",
    "pwdata": "pwdata",
    "pstrb": "pstrb",
    "pprot": "pprot",
    "pready": "pready",
    "prdata": "prdata",
    "pslverr": "pslverr",
}


class APB4Master:
    """APB4 master BFM."""

    def __init__(self, dut, clk, rst_n=None, names: Optional[Dict[str, str]] = None):
        self.dut = dut
        self.clk = clk
        self.rst_n = rst_n
        self.io = {}
        for logical, default in DEFAULT_APB_NAMES.items():
            actual = (names or {}).get(logical, default)
            if actual is not None and hasattr(dut, actual):
                self.io[logical] = getattr(dut, actual)
        for required in ("psel", "penable", "pwrite", "paddr", "pwdata", "pready", "prdata"):
            if required not in self.io:
                raise AttributeError(f"APB4Master: DUT is missing signal '{required}'")
        self.idle()

    # -- low level -------------------------------------------------------
    def _set(self, name: str, value: int) -> None:
        if name in self.io:
            self.io[name].value = value

    def idle(self) -> None:
        self._set("psel", 0)
        self._set("penable", 0)
        self._set("pwrite", 0)
        self._set("paddr", 0)
        self._set("pwdata", 0)
        self._set("pstrb", 0)
        self._set("pprot", 0)

    # -- transfers -------------------------------------------------------
    async def write(self, addr: int, data: int, strb: int = 0xF, prot: int = 0,
                    timeout: int = 200) -> int:
        """Perform an APB4 write. Returns PSLVERR (0/1)."""
        _, err = await self._transfer(addr, True, data, strb, prot, timeout)
        return err

    async def read(self, addr: int, prot: int = 0,
                   timeout: int = 200) -> Tuple[int, int]:
        """Perform an APB4 read. Returns (PRDATA, PSLVERR)."""
        return await self._transfer(addr, False, 0, 0xF, prot, timeout)

    async def _transfer(self, addr: int, write: bool, data: int, strb: int, prot: int,
                        timeout: int) -> Tuple[int, int]:
        # ---- SETUP cycle ----
        await FallingEdge(self.clk)
        self._set("psel", 1)
        self._set("penable", 0)
        self._set("pwrite", int(write))
        self._set("paddr", addr)
        self._set("pwdata", data)
        self._set("pstrb", strb)
        self._set("pprot", prot)
        await RisingEdge(self.clk)          # DUT samples SETUP
        await FallingEdge(self.clk)         # start of the first ACCESS cycle

        # ---- ACCESS cycle(s) ----
        self._set("penable", 1)
        for _ in range(timeout):
            await ReadOnly()                # settle; PREADY is valid for this cycle
            if int(self.io["pready"].value):
                prdata = int(self.io["prdata"].value)
                err = int(self.io["pslverr"].value) if "pslverr" in self.io else 0
                await RisingEdge(self.clk)  # completion edge (PSEL/PENABLE still held)
                await FallingEdge(self.clk)
                self.idle()
                return prdata, err
            await RisingEdge(self.clk)      # wait state: hold everything
            await FallingEdge(self.clk)
        self.idle()
        raise TimeoutError(f"APB4 transfer timed out: addr=0x{addr:08x} write={write}")

    # -- register helpers ------------------------------------------------
    async def write_expect_ok(self, addr: int, data: int, strb: int = 0xF) -> None:
        err = await self.write(addr, data, strb)
        assert err == 0, f"unexpected PSLVERR writing 0x{addr:08x}"

    async def read_expect_ok(self, addr: int) -> int:
        data, err = await self.read(addr)
        assert err == 0, f"unexpected PSLVERR reading 0x{addr:08x}"
        return data


class APB4Slave:
    """Minimal APB4 slave model (register file) for testing APB masters.

    Not used by challenge-2 (the DUT is the APB slave there), but useful for unit-testing
    the master BFM and for designs that expose an APB master.
    """

    def __init__(self, dut, clk, rst_n=None, names: Optional[Dict[str, str]] = None,
                 read_fn=None, write_fn=None, ready_delay: int = 0):
        self.dut = dut
        self.clk = clk
        self.rst_n = rst_n
        self.io = {}
        for logical, default in DEFAULT_APB_NAMES.items():
            actual = (names or {}).get(logical, default)
            if actual is not None and hasattr(dut, actual):
                self.io[logical] = getattr(dut, actual)
        self.ready_delay = ready_delay
        self.read_fn = read_fn or (lambda addr, strb: 0)
        self.write_fn = write_fn or (lambda addr, data, strb: None)
        self._run_task = None

    def start(self) -> None:
        import cocotb
        self._run_task = cocotb.start_soon(self._run())

    async def _run(self) -> None:
        wait = 0
        while True:
            await FallingEdge(self.clk)
            if self.rst_n is not None and not int(self.rst_n.value):
                wait = self.ready_delay
                self.io["pready"].value = 0
                if "pslverr" in self.io:
                    self.io["pslverr"].value = 0
                continue
            psel = int(self.io["psel"].value)
            penable = int(self.io["penable"].value)
            if not psel:
                wait = self.ready_delay
                self.io["pready"].value = 0
                if "pslverr" in self.io:
                    self.io["pslverr"].value = 0
            elif not penable:
                self.io["pready"].value = 0
                wait = self.ready_delay
            elif wait > 0:
                wait -= 1
                self.io["pready"].value = 0
            else:
                addr = int(self.io["paddr"].value)
                strb = int(self.io["pstrb"].value) if "pstrb" in self.io else 0xF
                if int(self.io["pwrite"].value):
                    self.write_fn(addr, int(self.io["pwdata"].value), strb)
                else:
                    self.io["prdata"].value = self.read_fn(addr, strb)
                self.io["pready"].value = 1
