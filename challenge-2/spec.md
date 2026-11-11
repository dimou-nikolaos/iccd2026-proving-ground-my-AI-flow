# dma_ctrl: Four-Channel DMA Controller

Specification, revision 1.0. Design 2.

## 1. Overview

`dma_ctrl` copies 32-bit words between memory locations for up to four channels. Software configures the unit through an AMBA APB4 completer. One shared engine performs all transfers through an AXI4-Lite master. Channels can be paced by peripheral request lines and can follow linked descriptors stored in memory.

Requirements are numbered `REQ-xx`. Every requirement is testable at the ports of `dma_ctrl`. Text marked "Note" is informative.

Conventions. "Cycle" means one period of `clk`. A register write takes effect on the rising edge that ends its APB access phase. A handshake on an AXI channel occurs on a rising edge at which valid and ready are both 1.

## 2. Ports

| Port | Dir | Width | Description |
|---|---|---|---|
| `clk` | in | 1 | Clock. |
| `rst_n` | in | 1 | Asynchronous reset, active low. |
| `psel`, `penable`, `pwrite` | in | 1 | APB control. |
| `paddr` | in | 12 | APB byte address. |
| `pwdata` | in | 32 | APB write data. |
| `pstrb` | in | 4 | APB write byte strobes. |
| `prdata` | out | 32 | APB read data. |
| `pready` | out | 1 | APB ready. |
| `pslverr` | out | 1 | APB error response. |
| `m_awvalid`, `m_awready`, `m_awaddr[31:0]` | out, in, out | | AXI write address. |
| `m_wvalid`, `m_wready`, `m_wdata[31:0]`, `m_wstrb[3:0]` | out, in, out, out | | AXI write data. |
| `m_bvalid`, `m_bready`, `m_bresp[1:0]` | in, out, in | | AXI write response. |
| `m_arvalid`, `m_arready`, `m_araddr[31:0]` | out, in, out | | AXI read address. |
| `m_rvalid`, `m_rready`, `m_rdata[31:0]`, `m_rresp[1:0]` | in, out, in, in | | AXI read data. |
| `dreq` | in | 4 | Peripheral request, one bit per channel. |
| `dack` | out | 4 | Peripheral acknowledge, one bit per channel. |
| `irq` | out | 1 | Interrupt request, active high. |

## 3. Reset

- **REQ-01** Every register field resets to 0, except the read-only identifier and LAST_GRANT (REQ-26).
- **REQ-02** Reset may be asserted at any time, including during a transfer. After reset, every register reads its reset value, all AXI valid outputs are 0, `dack` = 0 and `irq` = 0.

## 4. APB interface

- **REQ-03** The unit is an APB4 completer with zero wait states: `pready` is 1 in every cycle.
- **REQ-04** A transfer is performed in its access phase (`psel` = 1 and `penable` = 1). Setup-phase cycles and cycles with `psel` = 0 have no effect.
- **REQ-05** Address decode uses `paddr[11:2]`; `paddr[1:0]` are ignored.
- **REQ-06** In the access phase of a read of a mapped register, `prdata` carries the register value. Bits not defined for a register read as 0. Outside the access phase of a read, `prdata` = 0.
- **REQ-07** An access to an unmapped offset completes with `pslverr` = 1. An unmapped write has no effect, and an unmapped read returns 0. Accesses to mapped offsets complete with `pslverr` = 0.
- **REQ-08** A write updates only the bits in byte lanes whose `pstrb` bit is 1, for every writable register, including IRQ_STATUS and the EN bit of CH_CTRL.
- **REQ-09** A write to a read-only register completes with `pslverr` = 0 and has no effect. Undefined bits of writable registers ignore writes.

## 5. Register map

| Offset | Name | Access | Fields |
|---|---|---|---|
| 0x000 | ID | RO | Reads `0x444D0100`. |
| 0x004 | GCTRL | RW | [0] GEN global enable. [2:1] BURST: burst length is 2^BURST words. |
| 0x008 | IRQ_STATUS | W1C | [3:0] DONE[n]. [7:4] ERR[n]. |
| 0x00C | IRQ_ENABLE | RW | [7:0], matching IRQ_STATUS. |
| 0x010 | ACTIVE | RO | [3:0] EN of channel n. |
| 0x014 | LAST_GRANT | RO | [1:0] channel granted most recently (REQ-26). |
| 0x100 + 0x20n | CH_CTRL(n) | RW | [0] EN. [1] SRC_FIX. [2] DST_FIX. [3] HW_REQ. [5:4] PRIO. [6] LINK. |
| 0x104 + 0x20n | CH_SRC(n) | RW | [31:2] source word address. [1:0] read as 0. |
| 0x108 + 0x20n | CH_DST(n) | RW | [31:2] destination word address. [1:0] read as 0. |
| 0x10C + 0x20n | CH_LEN(n) | RW | [15:0] words left to move. |
| 0x110 + 0x20n | CH_NEXT(n) | RW | [31:2] address of the next descriptor. [1:0] read as 0. |
| 0x114 + 0x20n | CH_STAT(n) | RO | [0] ACTIVE, equal to EN. [1] ERR sticky. [2] DONE sticky. |

n = 0 to 3. All other offsets in 0x000 to 0xFFC are unmapped (REQ-07), including 0x118 + 0x20n and 0x11C + 0x20n.

## 6. Channels and transfers

- **REQ-10** Writing EN = 1 to a stopped channel starts it and clears its CH_STAT ERR and DONE bits. The other CH_CTRL bits take the value of the same write.
- **REQ-11** While EN = 1, writes to CH_SRC, CH_DST, CH_LEN, CH_NEXT and to CH_CTRL bits [6:1] have no effect.
- **REQ-12** A word transfer reads one word from the source with `m_araddr` = CH_SRC, then writes it to the destination with `m_awaddr` = CH_DST and `m_wstrb` = `4'hF`. Addresses are always word aligned.
- **REQ-13** The engine has at most one access outstanding: it does not start a write until the read data has been accepted, and does not start the next read until the write response has been accepted. `m_arvalid` and `m_awvalid` are never both 1. AW and W may complete in either order.
- **REQ-14** While an AXI valid output is 1 and its ready input is 0, that valid and its payload hold their values.
- **REQ-15** After a successful word transfer, CH_LEN decrements, CH_SRC advances by 4 unless SRC_FIX = 1, and CH_DST advances by 4 unless DST_FIX = 1.
- **REQ-16** A burst is up to 2^BURST word transfers. A channel keeps the engine for a burst, unless its descriptor ends, an error occurs or it is aborted first.
- **REQ-17** A descriptor ends when CH_LEN reaches 0.
- **REQ-18** A descriptor with LEN = 0 ends at once when its channel is granted. It moves no data and makes no bus access for data, and CH_LEN stays 0. A channel with HW_REQ = 1 and LEN = 0 is eligible without a request.
- **REQ-19** Word addresses wrap modulo 2^32. A CH_LEN of 0xFFFF moves 65535 words.
- **REQ-20** When a descriptor ends with LINK = 1 and CH_NEXT nonzero, the channel fetches the next descriptor (section 7). Otherwise the channel completes: EN clears, CH_STAT.DONE and IRQ_STATUS.DONE[n] are set.

## 7. Linked descriptors

- **REQ-21** A descriptor is four consecutive words at CH_NEXT: source address, destination address, length in bits [15:0], and next descriptor address. The channel reads them in that order with incrementing addresses, while it holds the engine.
- **REQ-22** The fetched words replace CH_SRC, CH_DST, CH_LEN and CH_NEXT. Bits [1:0] of the fetched addresses are ignored and bits [31:16] of the length are ignored.
- **REQ-23** After a fetch, the channel releases the engine and competes again.

## 8. Arbitration

- **REQ-24** The engine serves one channel at a time. A channel is eligible when GEN = 1, EN = 1 and it is not being aborted, and, if HW_REQ = 1 and CH_LEN is nonzero, `dreq[n]` = 1. Among eligible channels the highest PRIO wins. Among eligible channels of equal PRIO, the first one found in rotating order wins, starting at the channel after the one granted most recently. After reset the search starts at channel 0.
- **REQ-25** Clearing GEN stops new grants. A burst in progress finishes.
- **REQ-26** LAST_GRANT reads the channel granted most recently, and reads 3 after reset.
- **REQ-27** For a channel with HW_REQ = 1, `dack[n]` pulses for one cycle after a burst that moved at least one word ends. The peripheral is expected to hold `dreq[n]` until it sees `dack[n]`.

## 9. Errors, abort and interrupts

- **REQ-28** Any `m_rresp` or `m_bresp` other than `2'b00` stops the channel in that transfer. EN clears, CH_STAT.ERR and IRQ_STATUS.ERR[n] are set, and DONE is not set. The channel makes no further bus access, and a linked chain ends. This also applies to a descriptor fetch.
- **REQ-29** On a failed word read, nothing is written for that word and CH_SRC, CH_DST and CH_LEN keep their values for that word.
- **REQ-30** On a failed write response, CH_SRC, CH_DST and CH_LEN keep their values for that word.
- **REQ-31** Writing EN = 0 to a running channel aborts it. The word transfer in progress finishes, no further word starts, EN clears, and CH_STAT.DONE, IRQ_STATUS.DONE[n] and the ERR bits are not set unless that last word itself fails.
- **REQ-32** Status bits are set in the cycle the event occurs, and CH_STAT.DONE and CH_STAT.ERR stay set until the channel is started again.
- **REQ-33** IRQ_STATUS is write-1-to-clear: a write clears each status bit for which the written bit is 1 and the byte lane strobe is 1.
- **REQ-34** If an event sets a status bit in the same cycle as a write clears it, the bit is 1 after that cycle.
- **REQ-35** `irq` = 1 if and only if (IRQ_STATUS AND IRQ_ENABLE) is nonzero. `irq` is a function of the register state only.

## 10. Timing and implementation constraints

- **REQ-37** The design is synchronous to `clk`, uses only `rst_n` as an asynchronous reset, has no latches, and meets the released clock constraint (`flow/constraint.sdc`).

## 11. Compatibility notes

- The module name, port names and port widths in section 2 are fixed.
- The filelist `rtl/filelist.f` lists the source files in compile order, with paths relative to the design root.
- Timing of individual accesses beyond the requirements above is not specified, so an implementation may add or remove cycles of latency.
