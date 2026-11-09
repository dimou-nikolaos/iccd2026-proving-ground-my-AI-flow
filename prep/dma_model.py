"""
Golden transaction-level model of a 4-channel DMA controller (challenge-2 prep).

What this is
------------
A behavioural reference for `dma_ctrl`: APB4 register semantics on the control side and
AXI4-Lite word transactions on the data side. It is NOT cycle-accurate — it models the
*architectural* effects (memory contents, register readback, interrupt state, arbitration
order, descriptor chaining) that seeded deviations are visible in.

`prep/dma_model.py` is written before the spec exists, so every design decision that the
spec could pin down is expressed as a configurable knob:

    RegMap        - register offsets, per-channel stride, bit positions
    ModelConfig   - count encoding, re-arbitration granularity, descriptor format,
                    latency, W1C-vs-W0C, reset values

Every place where the real spec must supply the answer is marked `TODO(SPEC)`.

Usage
-----
    m = DMAModel()
    m.poke(0x1000, program_image)          # source data / descriptors
    m.apb_write(m.map.off("src", 0), 0x1000)
    m.apb_write(m.map.off("dst", 0), 0x2000)
    m.apb_write(m.map.off("size", 0), 0x10)
    m.apb_write(m.map.off("ctrl", 0), CTRL_START)
    m.run_until_idle()
    assert m.peek(0x2000) == m.peek(0x1000)
    assert m.irq == 0b0001

Run the self-tests on the host (no cocotb needed):
    python3 prep/test_dma_model.py
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple

MASK32 = 0xFFFF_FFFF


# ---------------------------------------------------------------------------
# Register map (PLACEHOLDER — replace with the released spec)
# ---------------------------------------------------------------------------
# TODO(SPEC): all offsets, the per-channel stride, which registers are per-channel vs
# global, and the exact bit positions below must come from the spec. The model only
# requires that the map answers "given a register name and a channel, what byte offset?"
# and "what bits live in it?".

CTRL_START = 1 << 0          # TODO(SPEC): start/arm bit
CTRL_IRQ_EN = 1 << 1         # per-channel interrupt enable (if per-channel)
CTRL_DESC_EN = 1 << 2        # use linked descriptors
CTRL_ABORT = 1 << 3
CTRL_PACED = 1 << 4         # transfer is paced by the peripheral request line

STATUS_BUSY = 1 << 0
STATUS_DONE = 1 << 1
STATUS_DESC_DONE = 1 << 2    # CR-D2 candidate location
STATUS_ERR = 1 << 3


@dataclass
class RegMap:
    """Byte offsets. `per_channel` means "offset within a channel block"."""

    ch_base: int = 0x10
    ch_stride: int = 0x20
    n_ch: int = 4

    # name -> (offset, per_channel)
    layout: Dict[str, Tuple[int, bool]] = field(default_factory=lambda: {
        # global block (placeholders)
        "gctrl":      (0x000, False),
        "int_status": (0x004, False),
        "int_enable": (0x008, False),
        "priority":   (0x00C, False),
        # per-channel block (placeholders; stride 0x20)
        "src":        (0x000, True),
        "dst":        (0x004, True),
        "size":       (0x008, True),
        "status":     (0x00C, True),
        "ctrl":       (0x010, True),
        "desc_ptr":   (0x014, True),
    })

    def off(self, name: str, ch: int = 0) -> int:
        base, per_ch = self.layout[name]
        if per_ch:
            return self.ch_base + self.ch_stride * ch + base
        return base

    def decode(self, offset: int) -> Optional[Tuple[str, int]]:
        """Inverse of `off`: (name, channel) or None if unmapped."""
        for name, (base, per_ch) in self.layout.items():
            if not per_ch:
                if offset == base:
                    return name, 0
            else:
                if self.ch_base <= offset < self.ch_base + self.ch_stride * self.n_ch:
                    rel = offset - self.ch_base
                    ch, within = divmod(rel, self.ch_stride)
                    if ch < self.n_ch and within == base:
                        return name, ch
        return None


@dataclass
class ModelConfig:
    # TODO(SPEC): does SIZE hold the word count, or count-1? Classic DMA specs often use
    # "N words" but hardware often stores N-1. Default: plain count.
    size_is_count_minus_one: bool = False
    # TODO(SPEC): does the engine re-arbitrate between words (per-word granularity),
    # between descriptors, or only when a channel finishes? Default: per word.
    arb_granularity: str = "word"        # "word" | "descriptor" | "channel"
    # TODO(SPEC): status clear is normally W1C; some designs use write-0-to-clear.
    status_clear: str = "w1c"            # "w1c" | "w0c"
    # TODO(SPEC): is a request line level-sensitive (transfer continues while high) or
    # edge/pulse-triggered (one request = one word)? Default: level, sampled per word.
    request_mode: str = "level"          # "level" | "pulse"
    # TODO(SPEC): AXI response errors - do they abort the channel?
    abort_on_error: bool = True
    # Transaction latency in engine cycles (lets tests model backpressure).
    read_latency: int = 1
    write_latency: int = 1
    # Descriptor format: byte offsets of the fields inside a descriptor, plus the
    # bit that says "another descriptor follows".
    desc_src: int = 0x0
    desc_dst: int = 0x4
    desc_size: int = 0x8
    desc_next: int = 0xC
    desc_flags: int = 0x10
    desc_flags_more: int = 1 << 0
    desc_stride: int = 0x20


# ---------------------------------------------------------------------------
# Channel state
# ---------------------------------------------------------------------------
@dataclass
class Channel:
    idx: int = 0
    src: int = 0
    dst: int = 0
    remaining: int = 0
    busy: bool = False
    done: bool = False
    desc_done: bool = False
    err: bool = False
    irq_en: bool = False
    desc_en: bool = False
    desc_ptr: int = 0
    paced: bool = False            # gated by the peripheral request line
    req: bool = False              # peripheral request line / latched token
    words_done: int = 0

    def reset(self) -> None:
        self.src = self.dst = self.remaining = 0
        self.busy = self.done = self.desc_done = self.err = False
        self.irq_en = self.desc_en = False
        self.desc_ptr = 0
        self.paced = False
        self.words_done = 0


class DMAModel:
    """4-channel DMA engine reference model (single shared AXI4-Lite engine)."""

    def __init__(self, regmap: Optional[RegMap] = None, cfg: Optional[ModelConfig] = None,
                 mem_size: int = 1 << 16):
        self.map = regmap or RegMap()
        self.cfg = cfg or ModelConfig()
        self.mem = bytearray(mem_size)
        self.ch = [Channel(idx=i) for i in range(self.map.n_ch)]
        self.global_ctrl = 0
        self._int_status_shadow = 0      # unused; see the int_status property
        self.int_enable = 0
        self.priority = list(range(self.map.n_ch))   # index 0 == highest priority
        self.txns: List[dict] = []
        self.cycle = 0

        # Engine FSM
        self._state = "IDLE"        # IDLE | WAIT_R | WAIT_B
        self._active: Optional[int] = None
        self._timer = 0
        self._rdata = 0
        self._active_desc_epoch = None

    # -- memory ----------------------------------------------------------
    def read32(self, addr: int) -> int:
        addr &= MASK32
        if addr + 4 > len(self.mem):
            return 0
        return int.from_bytes(self.mem[addr:addr + 4], "little")

    def write32(self, addr: int, value: int) -> None:
        addr &= MASK32
        if addr + 4 <= len(self.mem):
            self.mem[addr:addr + 4] = (value & MASK32).to_bytes(4, "little")

    def poke(self, addr: int, data: bytes) -> None:
        self.mem[addr:addr + len(data)] = data

    def peek(self, addr: int) -> int:
        return self.read32(addr)

    # -- interrupt -------------------------------------------------------
    @property
    def int_status(self) -> int:
        """Per-channel pending-interrupt bitmask.

        Derived from the per-channel event bits so there is exactly one source of truth.
        TODO(SPEC): whether transfer-complete and descriptor-complete share one pending
        bit, or live in separate registers/bits.
        """
        v = 0
        for c in self.ch:
            if c.done or c.desc_done:
                v |= 1 << c.idx
        return v

    @property
    def irq(self) -> int:
        """Bitmask of asserted per-channel interrupt lines (pending AND enable).

        TODO(SPEC): the exact formula (`pending & enable`, or a sticky register with a
        different clear rule) is a documented seeded-deviation site.
        """
        return self.int_status & self.int_enable

    # -- register interface ---------------------------------------------
    def apb_write(self, offset: int, value: int) -> Tuple[bool, bool]:
        """Returns (accepted, slverr)."""
        value &= MASK32
        dec = self.map.decode(offset)
        if dec is None:
            return False, True                    # TODO(SPEC): unmapped -> PSLVERR?
        name, ch = dec

        if name == "gctrl":
            self.global_ctrl = value
            return True, False
        if name == "int_status":
            # A single pending register per channel: W1C clears the underlying event.
            for c in self.ch:
                bit = 1 << c.idx
                if self.cfg.status_clear == "w1c":
                    if value & bit:
                        c.done = False
                        c.desc_done = False
                else:
                    c.done = bool(value & bit)
                    c.desc_done = False
            return True, False
        if name == "int_enable":
            self.int_enable = value
            return True, False
        if name == "priority":
            self.priority = self._decode_priority(value)
            return True, False

        c = self.ch[ch]
        if name == "src":
            c.src = value
        elif name == "dst":
            c.dst = value
        elif name == "size":
            c.remaining = self._size_to_count(value)
        elif name == "desc_ptr":
            c.desc_ptr = value
        elif name == "ctrl":
            c.irq_en = bool(value & CTRL_IRQ_EN)
            c.desc_en = bool(value & CTRL_DESC_EN)
            c.paced = bool(value & CTRL_PACED)
            if value & CTRL_ABORT:
                c.busy = False
            if value & CTRL_START:
                self._start(ch)
        elif name == "status":
            if self.cfg.status_clear == "w1c":
                c.done &= not (value & STATUS_DONE)
                c.desc_done &= not (value & STATUS_DESC_DONE)
                c.err &= not (value & STATUS_ERR)
            else:
                c.done = bool(value & STATUS_DONE)
                c.desc_done = bool(value & STATUS_DESC_DONE)
                c.err = bool(value & STATUS_ERR)
        return True, False

    def apb_read(self, offset: int) -> Tuple[int, bool]:
        dec = self.map.decode(offset)
        if dec is None:
            return 0, True
        name, ch = dec
        if name == "gctrl":
            return self.global_ctrl, False
        if name == "int_status":
            return self.int_status, False
        if name == "int_enable":
            return self.int_enable, False
        if name == "priority":
            return self._encode_priority(), False
        c = self.ch[ch]
        if name == "src":
            return c.src, False
        if name == "dst":
            return c.dst, False
        if name == "size":
            return self._count_to_size(c.remaining), False
        if name == "desc_ptr":
            return c.desc_ptr, False
        if name == "status":
            v = 0
            if c.busy:
                v |= STATUS_BUSY
            if c.done:
                v |= STATUS_DONE
            if c.desc_done:
                v |= STATUS_DESC_DONE
            if c.err:
                v |= STATUS_ERR
            return v, False
        return 0, True

    # -- encoding helpers ------------------------------------------------
    def _size_to_count(self, size: int) -> int:
        return (size & MASK32) + (1 if self.cfg.size_is_count_minus_one else 0)

    def _count_to_size(self, count: int) -> int:
        return count - (1 if self.cfg.size_is_count_minus_one else 0)

    def _decode_priority(self, value: int) -> List[int]:
        # Placeholder: 2 bits per channel, lower field = higher priority.
        order = []
        for slot in range(self.map.n_ch):
            order.append((value >> (2 * slot)) & 0x3)
        # TODO(SPEC): fixed priority vs programmable; tie-break direction.
        return order if sorted(order) == list(range(self.map.n_ch)) else list(range(self.map.n_ch))

    def _encode_priority(self) -> int:
        v = 0
        for slot, ch in enumerate(self.priority):
            v |= (ch & 0x3) << (2 * slot)
        return v

    # -- engine ----------------------------------------------------------
    def _start(self, ch: int) -> None:
        c = self.ch[ch]
        c.busy = True
        c.done = False
        c.err = False
        c.words_done = 0
        if c.remaining <= 0:
            c.remaining = self._size_to_count(0)
        if c.desc_en and c.desc_ptr:
            self._load_descriptor(ch, c.desc_ptr)

    def _load_descriptor(self, ch: int, addr: int) -> None:
        c = self.ch[ch]
        cfg = self.cfg
        c.src = self.read32(addr + cfg.desc_src)
        c.dst = self.read32(addr + cfg.desc_dst)
        c.remaining = self._size_to_count(self.read32(addr + cfg.desc_size))
        c.desc_ptr = addr
        self._active_desc_epoch = (ch, addr)

    def _desc_has_next(self) -> bool:
        if self._active_desc_epoch is None:
            return False
        ch, addr = self._active_desc_epoch
        flags = self.read32(addr + self.cfg.desc_flags)
        return bool(flags & self.cfg.desc_flags_more)

    def _desc_next_addr(self) -> Optional[int]:
        if not self._desc_has_next():
            return None
        ch, addr = self._active_desc_epoch
        nxt = self.read32(addr + self.cfg.desc_next)
        return nxt if nxt else None

    def _runnable(self, ch: int) -> bool:
        c = self.ch[ch]
        if not c.busy or c.remaining <= 0:
            return False
        # Pacing: only a paced channel is gated by the request line. A paced channel is
        # re-checked at every arbitration point, so a mid-transfer deassert stalls it.
        if c.paced and not c.req:
            return False
        return True

    def _arbitrate(self) -> Optional[int]:
        for ch in self.priority:
            if 0 <= ch < len(self.ch) and self._runnable(ch):
                return ch
        return None

    def tick(self) -> List[dict]:
        """Advance the engine by one cycle. Returns the AXI transactions it issued."""
        issued: List[dict] = []
        self.cycle += 1

        if self._state == "WAIT_R":
            self._timer -= 1
            if self._timer <= 0:
                self._rdata = self.read32(self.ch[self._active].src)
                c = self.ch[self._active]
                t = dict(kind="read", ch=self._active, addr=c.src, data=self._rdata)
                self.txns.append(t)
                issued.append(t)
                self._state = "WAIT_B"
                self._timer = self.cfg.write_latency
            return issued

        if self._state == "WAIT_B":
            self._timer -= 1
            if self._timer <= 0:
                c = self.ch[self._active]
                self.write32(c.dst, self._rdata)
                t = dict(kind="write", ch=self._active, addr=c.dst, data=self._rdata)
                self.txns.append(t)
                issued.append(t)
                c.src = (c.src + 4) & MASK32
                c.dst = (c.dst + 4) & MASK32
                c.remaining -= 1
                c.words_done += 1
                self._state = "IDLE"
                if c.remaining <= 0:
                    self._finish_channel(self._active)
                elif self.cfg.arb_granularity != "word":
                    self._active = self._active  # keep the channel (burst granularity)
                else:
                    self._active = None
            return issued

        # IDLE: start a new word
        if self.cfg.arb_granularity != "word" and self._active is not None:
            ch = self._active if self._runnable(self._active) else None
        else:
            ch = self._arbitrate()
        if ch is None:
            return issued

        c = self.ch[ch]
        if self.cfg.request_mode == "pulse" and c.req:
            c.req = False
        self._active = ch
        t = dict(kind="ar", ch=ch, addr=c.src)
        self.txns.append(t)
        issued.append(t)
        self._state = "WAIT_R"
        self._timer = self.cfg.read_latency
        return issued

    def _finish_channel(self, ch: int) -> None:
        c = self.ch[ch]
        c.done = True
        nxt = self._desc_next_addr() if c.desc_en else None
        if nxt is not None:
            # CR-D2: descriptor-complete event. The spec must say whether it fires per
            # descriptor (here) or only for the last descriptor of a chain.
            c.desc_done = True
            self._load_descriptor(ch, nxt)
            c.busy = True
            c.done = False
        else:
            c.busy = False
            c.desc_done = True if c.desc_en else c.desc_done
        self._active = None
        self._active_desc_epoch = None if nxt is None else self._active_desc_epoch

    def run_until_idle(self, max_cycles: int = 100_000) -> None:
        for _ in range(max_cycles):
            busy = any(c.busy for c in self.ch) or self._state != "IDLE"
            if not busy:
                return
            self.tick()
        raise TimeoutError("DMA model did not go idle")

    # -- test helpers ----------------------------------------------------
    def set_request(self, ch: int, level: bool) -> None:
        self.ch[ch].req = level

    def set_prio(self, order: List[int]) -> None:
        self.priority = list(order)
