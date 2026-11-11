"""Testbench for dsp_stream submission tests.

Golden model from spec.md (REQ-01 through REQ-41 plus CR-D1 changes).
Driver extends the public TB for extended register map and richer stimulus.
"""

import random
from dataclasses import dataclass, field
from typing import List, Tuple

import cocotb
from cocotb.clock import Clock
from cocotb.triggers import FallingEdge, Timer

CLK_NS = 10

# Register offsets (including CR-D1 additions).
ID, CTRL, OFFSET, GAIN, LIM_HI, LIM_LO, NTAPS, STATUS = (
    0x00, 0x04, 0x08, 0x0C, 0x10, 0x14, 0x18, 0x1C)
COEF0 = 0x20
STAT_IN, STAT_OUT, STAT_CLIP, PEAK_MAX, PEAK_MIN, STAT_CLEAR, IRQ_STATUS, IRQ_ENABLE = (
    0x40, 0x44, 0x48, 0x4C, 0x50, 0x54, 0x58, 0x5C)
LAST_LEN = 0x60

# Rounding modes.
ROUND_FLOOR, ROUND_HALF_UP, ROUND_HALF_EVEN, ROUND_TOWARD_ZERO = 0, 1, 2, 3


def ctrl_word(enable=1, bypass=1, rnd=ROUND_FLOOR, decim=0, clear_on_last=0):
    return enable | (bypass << 1) | (rnd << 2) | (decim << 4) | (clear_on_last << 6)


def s16(v):
    """Interpret lower 16 bits as signed."""
    v &= 0xFFFF
    return v - 0x10000 if v & 0x8000 else v


def to_u16(v):
    """Convert signed value to unsigned 16-bit for register writes."""
    return v & 0xFFFF


# ---------------------------------------------------------------------------
# Golden model: RS (round + shift + saturate) per spec REQ-12..REQ-18 + CR-D1
# ---------------------------------------------------------------------------

def rs(value, shift, round_mode):
    """RS(v, n) per spec.

    Returns (result_16bit_signed, saturated).

    value is an arbitrary-precision integer (the product before shifting).
    shift is the number of bits to shift right (0..15).
    round_mode is 0..3.
    """
    if shift == 0:
        # REQ-15: RS with n=0 returns v unchanged except for saturation.
        rounded = value
    else:
        # Arithmetic shift right (floor division by 2^shift).
        kept = value >> shift  # Python >> is arithmetic (floor) for negative

        # Dropped bits (the fractional part bits).
        mask = (1 << shift) - 1
        dropped = value & mask  # Python & with positive mask gives two's-complement bits

        half = 1 << (shift - 1)
        has_frac = (dropped != 0)
        frac_ge_half = (dropped >= half)
        frac_gt_half = (dropped > half)

        if round_mode == ROUND_FLOOR:
            inc = 0
        elif round_mode == ROUND_HALF_UP:
            inc = 1 if frac_ge_half else 0
        elif round_mode == ROUND_HALF_EVEN:
            # REQ-16: f > 1/2 always rounds up; f == 1/2 rounds to even.
            inc = 1 if (frac_gt_half or (frac_ge_half and (kept & 1))) else 0
        elif round_mode == ROUND_TOWARD_ZERO:
            # REQ-18 (CR-D1): nonzero fraction → round toward zero.
            # Positive: truncate (inc=0). Negative with fraction: inc=1.
            if has_frac and kept < 0:
                inc = 1
            else:
                inc = 0
        else:
            inc = 0

        rounded = kept + inc

    # Saturate to signed 16-bit.
    if rounded > 32767:
        return 32767, True
    elif rounded < -32768:
        return -32768, True
    else:
        return rounded, False


# ---------------------------------------------------------------------------
# Golden model: full pipeline (stateful across frames for FIR delay line)
# ---------------------------------------------------------------------------

@dataclass
class PipelineConfig:
    offset: int = 0
    gain: int = 256  # 0x0100 = 1.0 in Q8.8
    fir_bypass: int = 1
    ntaps: int = 1
    coefs: List[int] = field(default_factory=lambda: [0] * 8)
    decim: int = 0
    round_mode: int = ROUND_FLOOR
    lim_hi: int = 32767
    lim_lo: int = -32768
    clear_on_last: int = 0


class GoldenPipeline:
    """Stateful golden model. Process frames and collect outputs."""

    def __init__(self, config: PipelineConfig):
        self.cfg = config
        self.delay_line = [0] * 8  # d[0] = newest
        self.group_sum = 0
        self.group_count = 0
        self.group_clip = False

    def process_frame(self, samples: List[Tuple[int, int]]) -> List[Tuple[int, int, bool]]:
        """Process a frame of (data, last) pairs.

        Returns list of (data, last, clipped) for each output sample.
        """
        cfg = self.cfg
        G = 1 << cfg.decim
        outputs = []

        for (x, last) in samples:
            # --- Gain stage: RS((x - offset) * gain, 8) ---
            product = (x - cfg.offset) * cfg.gain
            gain_val, gain_clip = rs(product, 8, cfg.round_mode)

            # --- FIR stage ---
            if cfg.fir_bypass:
                fir_val = gain_val
                fir_clip = gain_clip
            else:
                # Insert into delay line (d[0] = newest).
                self.delay_line = [gain_val] + self.delay_line[:-1]
                acc = sum(cfg.coefs[k] * self.delay_line[k]
                          for k in range(cfg.ntaps))
                fir_val, fir_sat = rs(acc, 15, cfg.round_mode)
                fir_clip = gain_clip or fir_sat

                # REQ-23: clear delay line after last sample leaves filter.
                if cfg.clear_on_last and last:
                    self.delay_line = [0] * 8

            # --- Decimator ---
            self.group_sum += fir_val
            self.group_count += 1
            self.group_clip = self.group_clip or fir_clip

            # Group closes on G-th sample or on last flag.
            if self.group_count >= G or last:
                dec_val, dec_sat = rs(self.group_sum, cfg.decim, cfg.round_mode)
                dec_clip = self.group_clip or dec_sat

                # REQ-27: reset group after close (any reason).
                self.group_sum = 0
                self.group_count = 0
                self.group_clip = False

                # --- Limiter (REQ-28..REQ-30) ---
                if dec_val > cfg.lim_hi:
                    lim_val = cfg.lim_hi
                    lim_clip = True
                elif dec_val < cfg.lim_lo:
                    lim_val = cfg.lim_lo
                    lim_clip = True
                else:
                    lim_val = dec_val
                    lim_clip = False

                out_clip = dec_clip or lim_clip
                outputs.append((lim_val, last, out_clip))

        return outputs


def golden_model(samples, config: PipelineConfig):
    """Convenience: run pipeline model for a single frame from clean state."""
    pipe = GoldenPipeline(config)
    return pipe.process_frame(samples)


# ---------------------------------------------------------------------------
# Configuration helpers
# ---------------------------------------------------------------------------

def write_config(tb, cfg: PipelineConfig):
    """Write all config registers. Must be called with BUSY=0."""
    tb.write(CTRL, ctrl_word(enable=1, bypass=cfg.fir_bypass, rnd=cfg.round_mode,
                              decim=cfg.decim, clear_on_last=cfg.clear_on_last))
    tb.write(OFFSET, to_u16(cfg.offset))
    tb.write(GAIN, to_u16(cfg.gain))
    tb.write(LIM_HI, to_u16(cfg.lim_hi))
    tb.write(LIM_LO, to_u16(cfg.lim_lo))
    tb.write(NTAPS, cfg.ntaps)
    for k in range(8):
        tb.write(COEF0 + 4 * k, to_u16(cfg.coefs[k]))


def frame(values):
    """A list of signed samples as (data, last) pairs, last set on the final one."""
    return [(v, 1 if i == len(values) - 1 else 0) for i, v in enumerate(values)]


def make_frame(values):
    """Alias for frame()."""
    return frame(values)


# ---------------------------------------------------------------------------
# Driver class
# ---------------------------------------------------------------------------

class DspTB:
    """Extended testbench driver for dsp_stream."""

    def __init__(self, dut, seed=1):
        self.dut = dut
        self.rng = random.Random(seed)
        self.last_err = 0

    async def start(self):
        d = self.dut
        d.rst_n.value = 0
        d.s_valid.value = 0
        d.s_data.value = 0
        d.s_last.value = 0
        d.m_ready.value = 0
        d.cfg_we.value = 0
        d.cfg_addr.value = 0
        d.cfg_wdata.value = 0
        cocotb.start_soon(Clock(d.clk, CLK_NS, unit="ns").start(start_high=False))
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

    async def write(self, addr, value):
        d = self.dut
        d.cfg_we.value = 1
        d.cfg_addr.value = addr
        d.cfg_wdata.value = value & 0xFFFFFFFF
        await FallingEdge(d.clk)
        d.cfg_we.value = 0

    async def read(self, addr):
        d = self.dut
        d.cfg_we.value = 0
        d.cfg_addr.value = addr
        await Timer(1, unit="ns")
        val = int(d.cfg_rdata.value)
        self.last_err = int(d.cfg_err.value)
        await FallingEdge(d.clk)
        return val

    async def read_err(self, addr):
        """Read and return (value, err)."""
        val = await self.read(addr)
        return val, self.last_err

    async def stream(self, samples, expect_count, in_stall=0.0, out_stall=0.0,
                     timeout=50000):
        """Send (data, last) pairs and collect expect_count (data, last) outputs.

        Uses handshake with optional random stalls.
        Returns list of (data_signed, last) tuples.
        """
        d = self.dut
        rng = self.rng
        out = []
        i = 0
        cycles = 0
        pending = False
        while i < len(samples) or len(out) < expect_count:
            cycles += 1
            assert cycles < timeout, (
                f"stream timeout: sent {i}/{len(samples)}, "
                f"got {len(out)}/{expect_count}")
            if not pending:
                if i < len(samples) and rng.random() >= in_stall:
                    data, last = samples[i]
                    d.s_valid.value = 1
                    d.s_data.value = data & 0xFFFF
                    d.s_last.value = last
                    pending = True
                else:
                    d.s_valid.value = 0
            d.m_ready.value = 1 if rng.random() >= out_stall else 0
            await Timer(1, unit="ns")
            if pending and int(d.s_ready.value):
                pending = False
                i += 1
                sent = True
            else:
                sent = False
            if int(d.m_ready.value) and int(d.m_valid.value):
                out.append((s16(int(d.m_data.value)), int(d.m_last.value)))
            await FallingEdge(d.clk)
            if sent:
                d.s_valid.value = 0
        d.s_valid.value = 0
        d.m_ready.value = 0
        return out

    async def idle(self, cycles=5000):
        """Wait until STATUS.BUSY is 0."""
        for _ in range(cycles):
            if not (await self.read(STATUS)) & 1:
                return
        raise AssertionError("STATUS.BUSY did not clear")

    async def configure(self, cfg: PipelineConfig):
        """Write all configuration registers (assumes BUSY=0)."""
        await self.idle()
        await self.write(CTRL, ctrl_word(enable=1, bypass=cfg.fir_bypass,
                                         rnd=cfg.round_mode, decim=cfg.decim,
                                         clear_on_last=cfg.clear_on_last))
        await self.write(OFFSET, to_u16(cfg.offset))
        await self.write(GAIN, to_u16(cfg.gain))
        await self.write(LIM_HI, to_u16(cfg.lim_hi))
        await self.write(LIM_LO, to_u16(cfg.lim_lo))
        await self.write(NTAPS, cfg.ntaps)
        for k in range(8):
            await self.write(COEF0 + 4 * k, to_u16(cfg.coefs[k]))

    async def stream_and_compare(self, samples, cfg: PipelineConfig,
                                  in_stall=0.0, out_stall=0.0,
                                  check_clip=False):
        """Configure, stream, and compare against golden model.

        samples: list of (data, last) pairs (the input frame).
        Returns the list of expected output tuples for further verification.
        """
        await self.configure(cfg)
        pipe = GoldenPipeline(cfg)
        expected = pipe.process_frame(samples)
        got = await self.stream(samples, len(expected),
                                in_stall=in_stall, out_stall=out_stall)
        assert len(got) == len(expected), (
            f"output count: got {len(got)}, expected {len(expected)}")
        for idx, ((gv, gl), (ev, el, _ec)) in enumerate(zip(got, expected)):
            if gv != ev:
                raise AssertionError(
                    f"sample {idx}: got data={gv}, expected {ev}\n"
                    f"  cfg: offset={cfg.offset}, gain={cfg.gain}, "
                    f"bypass={cfg.fir_bypass}, ntaps={cfg.ntaps}, "
                    f"coefs={cfg.coefs[:cfg.ntaps]}, decim={cfg.decim}, "
                    f"round={cfg.round_mode}, lim_hi={cfg.lim_hi}, "
                    f"lim_lo={cfg.lim_lo}, col={cfg.clear_on_last}\n"
                    f"  first samples: {samples[:4]}\n"
                    f"  expected all: {[e[0] for e in expected]}\n"
                    f"  got all: {[g[0] for g in got]}")
            assert gl == el, (
                f"sample {idx}: got last={gl}, expected {el}")
        return expected

    async def clear_stats(self):
        """Write STAT_CLEAR with bit 0 = 1."""
        await self.write(STAT_CLEAR, 1)

    async def read_stats(self):
        """Read and return (stat_in, stat_out, stat_clip, peak_max, peak_min)."""
        si = await self.read(STAT_IN)
        so = await self.read(STAT_OUT)
        sc = await self.read(STAT_CLIP)
        pm = s16(await self.read(PEAK_MAX))
        pn = s16(await self.read(PEAK_MIN))
        return si, so, sc, pm, pn
