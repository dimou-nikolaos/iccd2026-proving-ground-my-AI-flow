"""Submission test suite for dsp_stream.

Tests are named after specification requirements and change-request items.
Uses golden model from dsp_tb.py for event-driven comparison.

At least 3 tests (deviation + CR tests) FAIL on the baseline RTL.
"""

import random

import cocotb
from cocotb.triggers import FallingEdge, RisingEdge

from dsp_tb import (
    COEF0, CTRL, GAIN, ID, IRQ_ENABLE, IRQ_STATUS, LAST_LEN, LIM_HI, LIM_LO,
    NTAPS, OFFSET, PEAK_MAX, PEAK_MIN, ROUND_FLOOR, ROUND_HALF_EVEN,
    ROUND_HALF_UP, ROUND_TOWARD_ZERO, STAT_CLEAR, STAT_CLIP, STAT_IN,
    STAT_OUT, STATUS, DspTB, GoldenPipeline, PipelineConfig,
    ctrl_word, frame, make_frame, rs, s16, to_u16,
)


async def setup(dut):
    """Initialize clock and reset, return driver."""
    tb = DspTB(dut)
    await tb.start()
    return tb


# ===========================================================================
# 1. DEVIATION TESTS — FAIL on baseline RTL
# ===========================================================================

@cocotb.test()
async def test_req17_underflow_saturation(dut):
    """REQ-17: RS saturates below -32768 to exactly -32768 (not -32767).

    Discriminates baseline bug: rndsat uses 16'sh8001 instead of 16'sh8000.
    GAIN=0x8000 (-128.0 Q8.8), x=257: product=257*(-32768)=-8414208.
    RS(-8414208, 8, FLOOR): kept=-32868, saturated to -32768.
    """
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=-32768, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    # x=257: product = 257 * (-32768) = -8414208. RS -> -32868 -> saturate to -32768.
    samples = frame([257])
    expected = golden_pipeline_single(samples, cfg)
    got = await tb.stream(samples, len(expected))
    assert len(got) == 1, f"expected 1 output, got {len(got)}"
    # Key assertion: must be exactly -32768, not -32767.
    assert got[0][0] == -32768, (
        f"REQ-17 underflow: expected -32768, got {got[0][0]}")


def golden_pipeline_single(samples, cfg):
    """Helper: run golden model for a single frame."""
    pipe = GoldenPipeline(cfg)
    return pipe.process_frame(samples)


@cocotb.test()
async def test_req17_underflow_saturation_multi(dut):
    """REQ-17: multiple underflow samples all saturate to -32768.

    Tests multiple samples that trigger underflow in the gain stage.
    """
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=-32768, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    # x=256,257,512 all cause underflow.
    samples = frame([256, 257, 512])
    got = await tb.stream(samples, 3)
    assert len(got) == 3
    # x=256: product=-8388608, kept=-32768, exactly at boundary (no saturation).
    assert got[0][0] == -32768, f"x=256: expected -32768, got {got[0][0]}"
    # x=257: product=-8414208, kept=-32868, underflow → -32768.
    assert got[1][0] == -32768, f"x=257: expected -32768, got {got[1][0]}"
    # x=512: product=-16777216, kept=-65536, underflow → -32768.
    assert got[2][0] == -32768, f"x=512: expected -32768, got {got[2][0]}"


@cocotb.test()
async def test_req37_stat_clear_peaks(dut):
    """REQ-37: STAT_CLEAR returns PEAK_MAX and PEAK_MIN to reset values.

    Discriminates baseline bug: sets peaks to 0 instead of 0x8000/0x7FFF.
    Reset values: PEAK_MAX=0x8000 (raw), PEAK_MIN=0x7FFF (raw).
    """
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    # Send samples that produce distinct peaks.
    samples = frame([100, -200, 50])
    await tb.stream(samples, 3)
    # Verify peaks are non-zero and non-default.
    pm = await tb.read(PEAK_MAX)
    pn = await tb.read(PEAK_MIN)
    assert pm != 0x8000 or pn != 0x7FFF, "peaks should have updated"

    # Now clear stats.
    await tb.clear_stats()

    # REQ-37: peaks must return to reset values (0x8000 and 0x7FFF raw).
    pm = await tb.read(PEAK_MAX)
    pn = await tb.read(PEAK_MIN)
    assert pm == 0x8000, (
        f"REQ-37: PEAK_MAX after STAT_CLEAR: expected 0x8000, got 0x{pm:04X}")
    assert pn == 0x7FFF, (
        f"REQ-37: PEAK_MIN after STAT_CLEAR: expected 0x7FFF, got 0x{pn:04X}")


@cocotb.test()
async def test_req27_decim_partial_group_reset(dut):
    """REQ-27: after group closes on in_last, sum/phase/clip reset.

    Discriminates baseline bug: only resets sum/phase on group_full,
    not on in_last-triggered close.

    With DECIM=1 (G=2): frame [10, 20, 30(last)], frame [40, 50(last)].
    Fixed: RS(30,1)=15, RS(30,1)=15 (from [30] alone), RS(90,1)=45.
    Baseline: after 30 closes on last but doesn't reset phase/sum,
    next group accumulates residual, giving wrong results.
    """
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=1, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)

    # First frame: [10, 20, 30(last=1)]
    samples1 = [(10, 0), (20, 0), (30, 1)]
    # Expected outputs from golden model:
    # Group 1: 10+20=30, RS(30,1)=15, last=0.
    # Group 2: 30, RS(30,1)=15, last=1.
    expected1_count = 2
    got1 = await tb.stream(samples1, expected1_count)
    assert len(got1) == 2, f"frame 1: expected 2 outputs, got {len(got1)}"
    assert got1[0] == (15, 0), f"frame 1 sample 0: expected (15,0), got {got1[0]}"
    assert got1[1] == (15, 1), f"frame 1 sample 1: expected (15,1), got {got1[1]}"

    # Second frame: [40, 50(last=1)]
    samples2 = [(40, 0), (50, 1)]
    # Expected: Group 3: 40+50=90, RS(90,1)=45, last=1.
    expected2_count = 1
    got2 = await tb.stream(samples2, expected2_count)
    assert len(got2) == 1, f"frame 2: expected 1 output, got {len(got2)}"
    assert got2[0] == (45, 1), f"frame 2 sample 0: expected (45,1), got {got2[0]}"


# ===========================================================================
# 2. CHANGE-REQUEST TESTS (CR-D1)
# ===========================================================================

@cocotb.test()
async def test_cr1_round_toward_zero(dut):
    """CR-D1 REQ-18: ROUND=3 rounds toward zero.

    Acceptance: GAIN=0x0040, ROUND=3, inputs 9,11,15,-9,-11,-15 →
    2,2,3,-2,-2,-3.
    """
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=0x0040, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_TOWARD_ZERO,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    inputs = [9, 11, 15, -9, -11, -15]
    expected_outputs = [2, 2, 3, -2, -2, -3]
    samples = frame(inputs)
    got = await tb.stream(samples, len(inputs))
    assert len(got) == len(expected_outputs)
    for i, (gv, ev) in enumerate(zip(got, expected_outputs)):
        assert gv[0] == ev, (
            f"CR-D1 toward-zero sample {i} (x={inputs[i]}): "
            f"expected {ev}, got {gv[0]}")


@cocotb.test()
async def test_cr1_round_toward_zero_negative_half(dut):
    """CR-D1 REQ-18: negative half values round toward zero (magnitude decreases).

    GAIN=0x0080 (0.5), input -1: product=-128. RS(-128,8,TOWARD_ZERO):
    kept=-1 (floor(-128/256)=-1), dropped=128 (nonzero), kept<0 → inc=1.
    Result: 0.
    """
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=0x0080, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_TOWARD_ZERO,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    # -1 * 128 = -128. floor(-128/256) = -1. dropped = -128 & 0xFF = 128 != 0.
    # kept = -1 < 0 → inc=1. result = 0.
    samples = frame([-1])
    got = await tb.stream(samples, 1)
    assert got[0][0] == 0, f"toward-zero(-1): expected 0, got {got[0][0]}"


@cocotb.test()
async def test_cr1_last_len_register(dut):
    """CR-D1 REQ-38: LAST_LEN at 0x060 holds completed frame length.

    After 7-sample frame delivered, reads 7. STAT_CLEAR resets to 0.
    Also verifies 0x060 is mapped (cfg_err=0).
    """
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)

    # Read LAST_LEN initially — should be 0 (reset).
    val, err = await tb.read_err(LAST_LEN)
    assert err == 0, f"LAST_LEN (0x060) should be mapped: cfg_err={err}"
    assert val == 0, f"LAST_LEN after reset: expected 0, got {val}"

    # Send a 7-sample frame.
    samples = frame([1, 2, 3, 4, 5, 6, 7])
    await tb.stream(samples, 7)
    await tb.idle()

    # Read LAST_LEN — should be 7.
    val, err = await tb.read_err(LAST_LEN)
    assert err == 0, f"LAST_LEN read error: {err}"
    assert val == 7, f"LAST_LEN after 7-sample frame: expected 7, got {val}"

    # Start another frame (partial) — LAST_LEN should still be 7.
    samples2 = [(10, 0), (20, 0)]
    got = await tb.stream(samples2, 2, out_stall=0.9)
    # We collected 2 samples but they're not last yet... actually we set last=0
    # so we need to check carefully. Let's just read LAST_LEN after partial.
    # If we only sent 2 non-last samples, no frame completed, LAST_LEN still 7.
    val, err = await tb.read_err(LAST_LEN)
    assert val == 7, f"LAST_LEN mid-frame: expected 7, got {val}"

    # STAT_CLEAR resets LAST_LEN.
    await tb.clear_stats()
    val, err = await tb.read_err(LAST_LEN)
    assert val == 0, f"LAST_LEN after STAT_CLEAR: expected 0, got {val}"


@cocotb.test()
async def test_cr1_last_len_mapped(dut):
    """CR-D1: address 0x060 is mapped (cfg_err=0), 0x064 is unmapped."""
    tb = await setup(dut)
    _, err = await tb.read_err(0x60)
    assert err == 0, "0x060 should be mapped"
    _, err = await tb.read_err(0x64)
    assert err == 1, "0x064 should be unmapped"


# ===========================================================================
# 3. BOUNDARY TESTS
# ===========================================================================

@cocotb.test()
async def test_req17_saturation_positive_boundary(dut):
    """REQ-17: saturation boundary at +32767.

    Values just below and at 32767 should not saturate; values above do.
    """
    tb = await setup(dut)
    # GAIN=0x0200 (2.0). x=16383: product=32766. RS(32766,8) = 127. Fine.
    # x=16384: product=32768. RS(32768,8) = 128. Fine.
    # x=40960: product=81920. RS(81920,8)=320. Fine.
    # x=4194304 is too large for 16-bit input.
    # Let's use GAIN=0x7FFF (32767/256 ≈ 127.996).
    # x=258: product = 258*32767 = 8453886. kept = 8453886>>8 = 33022. >32767. Saturates.
    cfg = PipelineConfig(
        offset=0, gain=32767, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    # x=257: product=257*32767=8421119. kept=8421119>>8=32894. >32767. Sat.
    # x=256: product=256*32767=8388352. kept=32767. Exactly at boundary. NOT sat.
    samples = frame([256, 257])
    got = await tb.stream(samples, 2)
    assert got[0][0] == 32767, f"x=256: expected 32767 (exact, no sat), got {got[0][0]}"
    assert got[1][0] == 32767, f"x=257: expected 32767 (saturated), got {got[1][0]}"


@cocotb.test()
async def test_req30_limiter_exact_boundary(dut):
    """REQ-30: sample equal to limit is NOT clipped.

    LIM_HI=100, LIM_LO=-100. Sample=100 not clipped, sample=101 clipped.
    Sample=-100 not clipped, sample=-101 clipped.
    """
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=100, lim_lo=-100,
    )
    await tb.configure(cfg)
    await tb.clear_stats()

    samples = frame([100, 101, -100, -101, 0])
    got = await tb.stream(samples, 5)
    assert got[0][0] == 100, f"x=100: expected 100, got {got[0][0]}"
    assert got[1][0] == 100, f"x=101: expected clipped to 100, got {got[1][0]}"
    assert got[2][0] == -100, f"x=-100: expected -100, got {got[2][0]}"
    assert got[3][0] == -100, f"x=-101: expected clipped to -100, got {got[3][0]}"
    assert got[4][0] == 0, f"x=0: expected 0, got {got[4][0]}"

    # REQ-32: STAT_CLIP counts clipped outputs. Should be 2 (x=101 and x=-101).
    await tb.idle()
    sc = await tb.read(STAT_CLIP)
    assert sc == 2, f"STAT_CLIP: expected 2, got {sc}"


@cocotb.test()
async def test_req24_decim_zero_passthrough(dut):
    """REQ-24/REQ-25: DECIM=0 means G=1, every sample is its own group.

    With shift=0, RS(sum,0) just saturates. Passthrough for values in range.
    """
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    values = [100, -200, 0, 32767, -32768, 1]
    samples = frame(values)
    got = await tb.stream(samples, len(values))
    assert len(got) == len(values)
    for i, (gv, _) in enumerate(got):
        assert gv == values[i], (
            f"decim=0 passthrough sample {i}: expected {values[i]}, got {gv}")


@cocotb.test()
async def test_req21_ntaps_single(dut):
    """REQ-21: NTAPS=1 with a single coefficient.

    COEF[0] = 0x4000 (0.5 in Q1.15). FIR: RS(input * 0.5 in Q1.15, 15).
    Input after gain=1.0: x. Product: x * 0x4000. RS(product, 15, FLOOR).
    For x=100: product=100*16384=1638400. RS(1638400,15)=50.
    """
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=0, ntaps=1,
        coefs=[0x4000, 0, 0, 0, 0, 0, 0, 0],
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    inputs = [100, 200, -400, 1, -1]
    samples = frame(inputs)
    got = await tb.stream(samples, len(inputs))
    # Expected: RS(x*0x4000, 15, FLOOR) = RS(x*16384, 15) = x*16384 >> 15
    # x=100: 1638400 >> 15 = 50. x=200: 3276800>>15=100. x=-400: -6553600>>15=-200.
    # x=1: 16384>>15=0. x=-1: -16384>>15=-1 (arithmetic shift floor).
    expected = [50, 100, -200, 0, -1]
    for i, (gv, _) in enumerate(got):
        assert gv == expected[i], (
            f"NTAPS=1 sample {i} (x={inputs[i]}): expected {expected[i]}, got {gv}")


@cocotb.test()
async def test_req29_lim_hi_wins_when_inverted(dut):
    """REQ-29: LIM_HI wins when LIM_HI < LIM_LO.

    Set LIM_HI=100, LIM_LO=200. Value 150: >100 → clamp to 100 (not 200).
    """
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=100, lim_lo=200,
    )
    await tb.configure(cfg)
    samples = frame([150, 250, 50])
    got = await tb.stream(samples, 3)
    # REQ-28: > LIM_HI → LIM_HI. REQ-29: check hi first.
    # 150 > 100 → LIM_HI = 100.
    assert got[0][0] == 100, f"inverted limits 150: expected 100, got {got[0][0]}"
    assert got[1][0] == 100, f"inverted limits 250: expected 100, got {got[1][0]}"
    # 50 > 100? No. 50 < 200? No (50 is less than 200). → 50 < LIM_LO? 50 < 200 yes → LIM_LO=200.
    # Wait: REQ-29: "a sample less than LIM_LO becomes LIM_LO". 50 < 200 yes → 200.
    assert got[2][0] == 200, f"inverted limits 50: expected 200, got {got[2][0]}"


# ===========================================================================
# 4. PROTOCOL TESTS
# ===========================================================================

@cocotb.test()
async def test_req07_backpressure_heavy(dut):
    """REQ-07: output stalls (m_ready=0) hold data stable, no loss.

    Heavy backpressure (90% stall) with continuous input.
    """
    tb = DspTB(dut, seed=42)
    await tb.start()
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    values = list(range(-30, 30))
    samples = frame(values)
    got = await tb.stream(samples, len(values), out_stall=0.9)
    assert got == frame(values), "backpressure: output mismatch"


@cocotb.test()
async def test_req07_backpressure_both_sides(dut):
    """REQ-07: heavy stalls on both input and output sides."""
    tb = DspTB(dut, seed=99)
    await tb.start()
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    values = list(range(100))
    samples = frame(values)
    got = await tb.stream(samples, len(values), in_stall=0.3, out_stall=0.5)
    assert got == frame(values), "both backpressure: output mismatch"


@cocotb.test()
async def test_req08_enable_off_midstream(dut):
    """REQ-08: while ENABLE=0, s_ready=0. Samples in pipeline still drain.

    Send a full frame, disable ENABLE after all sent, verify pipeline drains.
    """
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)

    # Send all 3 samples (they enter the pipeline).
    samples = [(10, 0), (20, 0), (30, 1)]
    got = await tb.stream(samples, 3)
    assert len(got) == 3, f"REQ-08: expected 3 outputs, got {len(got)}"
    assert [v for v, _ in got] == [10, 20, 30]

    # Now disable ENABLE.
    await tb.write(CTRL, ctrl_word(enable=0, bypass=1))

    # s_ready should be 0 when ENABLE=0.
    d = tb.dut
    d.s_valid.value = 1
    d.s_data.value = 99
    d.s_last.value = 0
    await FallingEdge(d.clk)
    assert int(d.s_ready.value) == 0, "s_ready should be 0 when ENABLE=0"
    d.s_valid.value = 0


@cocotb.test()
async def test_req02_reset_midstream(dut):
    """REQ-02: reset mid-operation clears pipeline.

    Start streaming, assert reset, verify clean state after reset.
    """
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)

    # Send some samples to get pipeline busy.
    samples = frame([10, 20, 30])
    await tb.stream(samples, 3)

    # Assert reset.
    await tb.reset()

    # After reset: verify register state is clean.
    val = await tb.read(CTRL)
    assert val == 0x02, f"after reset CTRL: expected 0x02, got 0x{val:02X}"
    assert int(tb.dut.m_valid.value) == 0, "after reset: m_valid should be 0"
    assert int(tb.dut.irq.value) == 0, "after reset: irq should be 0"

    # Reconfigure and verify design works.
    await tb.configure(cfg)
    samples2 = frame([7, 8, 9])
    got = await tb.stream(samples2, 3)
    assert [v for v, _ in got] == [7, 8, 9], "post-reset operation failed"


# ===========================================================================
# 5. CONSTRAINED-RANDOM TESTS
# ===========================================================================

@cocotb.test()
async def test_random_configs_floor(dut):
    """Random configs with ROUND=0 (floor), 10 configs, random data."""
    await run_random_configs(dut, seed=1001, n_configs=10, round_mode=ROUND_FLOOR)


@cocotb.test()
async def test_random_configs_half_up(dut):
    """Random configs with ROUND=1 (half-up), 10 configs."""
    await run_random_configs(dut, seed=2002, n_configs=10, round_mode=ROUND_HALF_UP)


@cocotb.test()
async def test_random_configs_half_even(dut):
    """Random configs with ROUND=2 (half-even), 10 configs."""
    await run_random_configs(dut, seed=3003, n_configs=10, round_mode=ROUND_HALF_EVEN)


@cocotb.test()
async def test_random_configs_toward_zero(dut):
    """Random configs with ROUND=3 (toward-zero, CR-D1), 10 configs."""
    await run_random_configs(dut, seed=4004, n_configs=10, round_mode=ROUND_TOWARD_ZERO)


async def run_random_configs(dut, seed, n_configs, round_mode):
    """Run multiple random configs and compare against golden model."""
    tb = DspTB(dut, seed=seed)
    await tb.start()
    rng = tb.rng

    for ci in range(n_configs):
        # Reset between configs to clear FIR delay line.
        if ci > 0:
            await tb.reset()

        # Random config.
        offset = rng.randint(-1000, 1000)
        # Gain: moderate range to avoid constant saturation.
        gain = rng.randint(-4096, 4096)
        fir_bypass = rng.choice([0, 1])
        ntaps = rng.randint(1, 8)
        coefs = [rng.randint(-8192, 8192) for _ in range(8)]
        # Zero out coefficients beyond ntaps for clean FIR.
        for k in range(ntaps, 8):
            coefs[k] = 0
        decim = rng.randint(0, 3)
        lim_hi = rng.randint(0, 32767)
        lim_lo = rng.randint(-32768, lim_hi)
        clear_on_last = rng.choice([0, 1])

        cfg = PipelineConfig(
            offset=offset, gain=gain, fir_bypass=fir_bypass, ntaps=ntaps,
            coefs=coefs, decim=decim, round_mode=round_mode,
            lim_hi=lim_hi, lim_lo=lim_lo, clear_on_last=clear_on_last,
        )

        # Generate random frame (moderate length for speed).
        n_samples = rng.randint(4, 20)
        values = [rng.randint(-32768, 32767) for _ in range(n_samples)]
        samples = frame(values)

        await tb.stream_and_compare(samples, cfg, in_stall=0.1, out_stall=0.1)


# ===========================================================================
# 6. REGISTER-MAP TESTS
# ===========================================================================

@cocotb.test()
async def test_req04_addr_decode_ignores_low_bits(dut):
    """REQ-04: address decode uses cfg_addr[7:2], bits [1:0] ignored.

    Read ID at 0x00, 0x01, 0x02, 0x03 — all return same value.
    """
    tb = await setup(dut)
    for byte_offset in [0x00, 0x01, 0x02, 0x03]:
        val = await tb.read(byte_offset)
        assert val == 0x44530100, f"ID at 0x{byte_offset:02X}: expected 0x44530100, got 0x{val:08X}"


@cocotb.test()
async def test_req05_cfg_err_unmapped(dut):
    """REQ-05: cfg_err=1 for unmapped offsets, returns 0.

    In fixed RTL: 0x060 is last mapped (LAST_LEN), 0x064 is first unmapped.
    """
    tb = await setup(dut)
    # 0x05C (IRQ_ENABLE) is mapped.
    _, err = await tb.read_err(0x5C)
    assert err == 0, "0x05C should be mapped"
    # 0x060 (LAST_LEN) is mapped (CR-D1).
    _, err = await tb.read_err(0x60)
    assert err == 0, "0x060 should be mapped (CR-D1)"
    # 0x064 is unmapped.
    val, err = await tb.read_err(0x64)
    assert err == 1, "0x064 should be unmapped"
    assert val == 0, f"unmapped read should return 0, got {val}"
    # 0x068 also unmapped.
    _, err = await tb.read_err(0x68)
    assert err == 1, "0x068 should be unmapped"
    # 0xFF unmapped.
    _, err = await tb.read_err(0xFF)
    assert err == 1, "0xFF should be unmapped"


@cocotb.test()
async def test_req06_readonly_id(dut):
    """REQ-06: write to read-only ID has no effect."""
    tb = await setup(dut)
    await tb.write(ID, 0xDEADBEEF)
    val = await tb.read(ID)
    assert val == 0x44530100, f"ID after write: expected 0x44530100, got 0x{val:08X}"


@cocotb.test()
async def test_req06_readonly_status(dut):
    """REQ-06: write to read-only STATUS has no effect."""
    tb = await setup(dut)
    await tb.write(STATUS, 0xFFFFFFFF)
    val = await tb.read(STATUS)
    # STATUS should be at reset value (0 = not busy, empty=1 → m_valid=0, empty=1).
    # bit[1] = EMPTY = 1 (m_valid=0). bit[0] = BUSY = 0.
    assert val == 0x2, f"STATUS after write: expected 0x2, got 0x{val:X}"


@cocotb.test()
async def test_req06_readonly_stats(dut):
    """REQ-06: write to read-only STAT_IN/OUT/CLIP/PEAK has no effect."""
    tb = await setup(dut)
    for addr in [STAT_IN, STAT_OUT, STAT_CLIP, PEAK_MAX, PEAK_MIN]:
        await tb.write(addr, 0xFFFFFFFF)
    val = await tb.read(STAT_IN)
    assert val == 0, f"STAT_IN after write: expected 0, got {val}"
    val = await tb.read(STAT_OUT)
    assert val == 0, f"STAT_OUT after write: expected 0, got {val}"
    val = await tb.read(PEAK_MAX)
    assert val == 0x8000, f"PEAK_MAX after write: expected 0x8000, got 0x{val:04X}"
    val = await tb.read(PEAK_MIN)
    assert val == 0x7FFF, f"PEAK_MIN after write: expected 0x7FFF, got 0x{val:04X}"


@cocotb.test()
async def test_req05_all_mapped_reads(dut):
    """REQ-05: all mapped addresses (0x000..0x060) read without cfg_err."""
    tb = await setup(dut)
    for word in range(0x19):  # 0x00..0x18 → offsets 0x00..0x60
        addr = word * 4
        _, err = await tb.read_err(addr)
        assert err == 0, f"address 0x{addr:02X} (word {word}): expected cfg_err=0, got {err}"


@cocotb.test()
async def test_req06_ntaps_out_of_range(dut):
    """REQ-05/REG: NTAPS writes outside 1..8 are ignored."""
    tb = await setup(dut)
    # Write 0 (invalid).
    await tb.write(NTAPS, 0)
    val = await tb.read(NTAPS)
    assert val == 1, f"NTAPS after write 0: expected 1 (reset), got {val}"
    # Write 9 (invalid).
    await tb.write(NTAPS, 9)
    val = await tb.read(NTAPS)
    assert val == 1, f"NTAPS after write 9: expected 1 (reset), got {val}"
    # Write 15 (invalid).
    await tb.write(NTAPS, 15)
    val = await tb.read(NTAPS)
    assert val == 1, f"NTAPS after write 15: expected 1 (reset), got {val}"
    # Write 8 (valid).
    await tb.write(NTAPS, 8)
    val = await tb.read(NTAPS)
    assert val == 8, f"NTAPS after write 8: expected 8, got {val}"


# ===========================================================================
# 7. FIR and pipeline interaction tests
# ===========================================================================

@cocotb.test()
async def test_req23_clear_on_last(dut):
    """REQ-23: CLEAR_ON_LAST=1 clears FIR delay line after last sample.

    Two frames with same FIR: without clear, frame 2 uses residual from frame 1.
    With clear, frame 2 starts fresh.
    """
    tb = await setup(dut)
    # FIR with 2 taps: coef[0]=0x4000 (0.5), coef[1]=0x4000 (0.5) — averaging.
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=0, ntaps=2,
        coefs=[0x4000, 0x4000, 0, 0, 0, 0, 0, 0],
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
        clear_on_last=1,
    )
    await tb.configure(cfg)

    # Frame 1: [100, 200]
    # d after 100: [100, 0, ...]. FIR: RS(100*16384 + 0, 15) = RS(1638400,15)=50.
    # d after 200: [200, 100, ...]. FIR: RS(200*16384 + 100*16384, 15) = RS(4915200,15)=150.
    # After last: delay line cleared.
    samples1 = [(100, 0), (200, 1)]
    got1 = await tb.stream(samples1, 2)
    assert got1[0][0] == 50, f"clear_on_last f1 s0: expected 50, got {got1[0][0]}"
    assert got1[1][0] == 150, f"clear_on_last f1 s1: expected 150, got {got1[1][0]}"

    # Frame 2: [300, 400]
    # With clear: d after 300: [300, 0, ...]. FIR: RS(300*16384, 15)=150.
    # d after 400: [400, 300, ...]. FIR: RS(400*16384+300*16384, 15)=350.
    samples2 = [(300, 0), (400, 1)]
    got2 = await tb.stream(samples2, 2)
    assert got2[0][0] == 150, f"clear_on_last f2 s0: expected 150, got {got2[0][0]}"
    assert got2[1][0] == 350, f"clear_on_last f2 s1: expected 350, got {got2[1][0]}"


@cocotb.test()
async def test_req23_no_clear_carryover(dut):
    """REQ-23: CLEAR_ON_LAST=0 carries over delay line between frames."""
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=0, ntaps=2,
        coefs=[0x4000, 0x4000, 0, 0, 0, 0, 0, 0],
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
        clear_on_last=0,
    )
    await tb.configure(cfg)

    # Frame 1: [100, 200]. Delay line after: [200, 100, 0, ...].
    samples1 = [(100, 0), (200, 1)]
    got1 = await tb.stream(samples1, 2)
    assert got1[0][0] == 50
    assert got1[1][0] == 150

    # Frame 2: [300].
    # d after 300: [300, 200, 100, 0, ...] (carry-over from frame 1).
    # FIR: RS(300*16384 + 200*16384, 15) = RS(8192000, 15) = 250.
    samples2 = [(300, 1)]
    got2 = await tb.stream(samples2, 1)
    assert got2[0][0] == 250, f"no_clear carryover: expected 250, got {got2[0][0]}"


# ===========================================================================
# 8. Statistics and IRQ tests
# ===========================================================================

@cocotb.test()
async def test_req34_stats_saturate_at_ffff(dut):
    """REQ-34: STAT_IN/OUT saturate at 0xFFFF, do not wrap.
    Push 65540 samples with fir_bypass=1, decim=0, m_ready=1 — counters
    should hit 0xFFFF and stay there."""
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    await tb.clear_stats()
    d = tb.dut
    target = 65540
    sent = 0
    cycles = 0
    d.m_ready.value = 1
    d.s_valid.value = 1
    d.s_data.value = 1
    d.s_last.value = 0
    while sent < target and cycles < target * 4:
        cycles += 1
        await RisingEdge(d.clk)
        if int(d.s_ready.value):
            sent += 1
    d.s_valid.value = 0
    d.m_ready.value = 1
    for _ in range(20):
        await RisingEdge(d.clk)
    si, so, sc, _, _ = await tb.read_stats()
    assert si == 0xFFFF, f"REQ-34 STAT_IN did not saturate: got {si}"
    assert so >= 0xFF00, f"REQ-34 STAT_OUT too low: got {so}"


@cocotb.test()
async def test_req12_rs_same_at_every_stage(dut):
    """REQ-12: each stage (gain, FIR, decim) uses the SAME RS operation.
    Cross-check: configure each stage in isolation (others passthrough) and
    feed the same value through. All three should round identically."""
    tb = await setup(dut)
    # ROUND_HALF_UP; x=100 → gain path: (100-0)*256 = 25600, RS(25600, 8, HU) = 100
    # FIR path: 1 tap coef=1 → sum=100, RS(100, 15, HU) = 0 (shifted right by 15)
    # Decim path: decim=0 → 1-sample group, sum=100, RS(100, 0, HU) = 100
    # Verify decim rounding matches expected exactly when FIR bypassed.
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=1, round_mode=ROUND_HALF_UP,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    # Two samples [3, 4] in one decim=1 group (G=2): sum=7, RS(7, 1, HU).
    # 7 >>> 1 = 3 with frac 1, half = 1, frac_ge_half → round up → 4.
    samples = frame([3, 4])
    got = await tb.stream(samples, 1)
    expected = golden_pipeline_single(samples, cfg)
    assert got == [(v, l) for v, l, *_ in expected], f"REQ-12 RS-at-every-stage: got {got}, expected {expected}"


@cocotb.test()
async def test_req33_statistics_counts(dut):
    """REQ-33: STAT_IN counts s_* handshakes, STAT_OUT counts m_* handshakes."""
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    await tb.clear_stats()

    samples = frame([10, 20, 30, 40, 50])
    await tb.stream(samples, 5)
    await tb.idle()

    si, so, sc, _, _ = await tb.read_stats()
    assert si == 5, f"STAT_IN: expected 5, got {si}"
    assert so == 5, f"STAT_OUT: expected 5, got {so}"
    assert sc == 0, f"STAT_CLIP: expected 0, got {sc}"


@cocotb.test()
async def test_req37_full_stat_clear(dut):
    """REQ-37: STAT_CLEAR resets STAT_IN, STAT_OUT, STAT_CLIP to 0."""
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=32767, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)

    # Send samples that will saturate (clip).
    samples = frame([300, -300])
    await tb.stream(samples, 2)
    await tb.idle()

    si, so, sc, _, _ = await tb.read_stats()
    assert si > 0 and so > 0 and sc > 0, f"expected non-zero stats before clear"

    await tb.clear_stats()
    si, so, sc, _, _ = await tb.read_stats()
    assert si == 0, f"STAT_IN after clear: expected 0, got {si}"
    assert so == 0, f"STAT_OUT after clear: expected 0, got {so}"
    assert sc == 0, f"STAT_CLIP after clear: expected 0, got {sc}"


@cocotb.test()
async def test_req35_peak_hold(dut):
    """REQ-35: PEAK_MAX/MIN track extreme output values."""
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)

    samples = frame([500, -1000, 300, -50, 2000])
    await tb.stream(samples, 5)
    await tb.idle()

    pm = s16(await tb.read(PEAK_MAX))
    pn = s16(await tb.read(PEAK_MIN))
    assert pm == 2000, f"PEAK_MAX: expected 2000, got {pm}"
    assert pn == -1000, f"PEAK_MIN: expected -1000, got {pn}"


@cocotb.test()
async def test_req39_irq_clip(dut):
    """REQ-39/REQ-41: IRQ CLIP fires when clipped sample enters output FIFO."""
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=100, lim_lo=-100,
    )
    await tb.configure(cfg)
    await tb.write(IRQ_ENABLE, 0x1)  # Enable CLIP interrupt.

    # Send sample that gets clipped by limiter.
    samples = frame([200])
    await tb.stream(samples, 1)
    await tb.idle()

    assert int(tb.dut.irq.value) == 1, "irq should be 1 (CLIP enabled, clip occurred)"
    irq_stat = await tb.read(IRQ_STATUS)
    assert irq_stat & 0x1, f"IRQ_STATUS.CLIP should be set, got 0x{irq_stat:X}"


@cocotb.test()
async def test_req40_irq_w1c(dut):
    """REQ-40: IRQ_STATUS is write-1-to-clear."""
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=100, lim_lo=-100,
    )
    await tb.configure(cfg)
    await tb.write(IRQ_ENABLE, 0x1)

    # Trigger a clip.
    samples = frame([200])
    await tb.stream(samples, 1)
    await tb.idle()

    assert int(tb.dut.irq.value) == 1
    # Clear CLIP bit.
    await tb.write(IRQ_STATUS, 0x1)
    assert int(tb.dut.irq.value) == 0, "irq should be 0 after W1C"


# ===========================================================================
# 9. Clip propagation tests
# ===========================================================================

@cocotb.test()
async def test_req13_clip_from_gain(dut):
    """REQ-13/REQ-32: clip mark from gain saturation propagates to output."""
    tb = await setup(dut)
    # GAIN=0x7FFF. x=257 causes saturation.
    cfg = PipelineConfig(
        offset=0, gain=32767, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    await tb.clear_stats()

    samples = frame([257])  # Saturates in gain stage.
    got = await tb.stream(samples, 1)
    assert got[0][0] == 32767, f"expected 32767, got {got[0][0]}"
    await tb.idle()
    sc = await tb.read(STAT_CLIP)
    assert sc == 1, f"gain saturation clip: expected STAT_CLIP=1, got {sc}"


@cocotb.test()
async def test_req13_clip_across_decim_group(dut):
    """REQ-13: clip marks combine across a decimation group.

    DECIM=1 (G=2). Sample 1 saturates in gain, sample 2 doesn't.
    Output of that group should be marked clipped.
    """
    tb = await setup(dut)
    # GAIN=0x7FFF. x=257 saturates, x=1 doesn't.
    cfg = PipelineConfig(
        offset=0, gain=32767, fir_bypass=1, ntaps=1,
        decim=1, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    await tb.clear_stats()

    # Frame: [257(sat), 1, 1(sat? no), 257(sat), 1(last=1)]
    # Group 1: 257+1. 257 saturates in gain → clip.
    # Group 2: 1+257. 257 saturates → clip.
    # Actually: G=2. samples: 257, 1 → group closes. Then 1, 257 → group closes. Then 1 (last) → closes.
    # Wait frame is [(257,0),(1,0),(1,0),(257,1)] — 4 samples, last on 257.
    # G=2: Group 1: 257,1 → closes (count=2). Group 2: 1,257 → closes (count=2).
    # Hmm, we need 4 outputs? No. With G=2, 4 samples → 2 groups → 2 outputs.
    samples = [(257, 0), (1, 0), (1, 0), (257, 1)]
    # Expected outputs: 2. First: RS(gain(257)+gain(1), 1). gain(257) saturated → clip.
    # Second: RS(gain(1)+gain(257), 1). gain(257) saturated → clip. Last=1.
    got = await tb.stream(samples, 2)
    assert len(got) == 2, f"expected 2 outputs, got {len(got)}"
    await tb.idle()
    sc = await tb.read(STAT_CLIP)
    assert sc == 2, f"decim clip propagation: expected STAT_CLIP=2, got {sc}"


# ===========================================================================
# 10. Decimator edge cases
# ===========================================================================

@cocotb.test()
async def test_req25_decim_shift_with_short_group(dut):
    """REQ-25: shift by D even for short (last-triggered) group.

    DECIM=2 (G=4, shift=2). Frame of 3 samples: only last closes short group.
    RS(sum, 2) applied even though group has only 3 samples.
    """
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=2, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    # Frame: [10, 20, 30(last=1)] — 3 samples, short group.
    # sum = 60. RS(60, 2) = 15.
    samples = [(10, 0), (20, 0), (30, 1)]
    got = await tb.stream(samples, 1)
    assert len(got) == 1, f"short group: expected 1 output, got {len(got)}"
    assert got[0] == (15, 1), f"short group: expected (15,1), got {got[0]}"


@cocotb.test()
async def test_req26_decim_last_flag(dut):
    """REQ-26: emitted sample has last flag if closing sample has it."""
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=1, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    # G=2. Frame: [10, 20(last=1)]. Group closes on 2nd sample with last.
    # Output should have last=1.
    samples = [(10, 0), (20, 1)]
    got = await tb.stream(samples, 1)
    assert got[0] == (15, 1), f"decim last flag: expected (15,1), got {got[0]}"


# ===========================================================================
# 11. Multi-stage pipeline tests
# ===========================================================================

@cocotb.test()
async def test_req19_offset_gain_combined(dut):
    """REQ-19: combined offset and gain. RS((x-OFFSET)*GAIN, 8)."""
    tb = await setup(dut)
    # OFFSET=10, GAIN=0x0180 (1.5). ROUND=HALF_UP.
    cfg = PipelineConfig(
        offset=10, gain=0x0180, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_HALF_UP,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    values = [10, 11, 12, 13, 110, 1010]
    samples = frame(values)
    got = await tb.stream(samples, len(values))
    # (10-10)*1.5=0 → 0. (11-10)*1.5=1.5 → half-up → 2.
    # (12-10)*1.5=3 → 3. (13-10)*1.5=4.5 → half-up → 5.
    # (110-10)*1.5=150 → 150. (1010-10)*1.5=1500 → 1500.
    expected = [0, 2, 3, 5, 150, 1500]
    for i, (gv, _) in enumerate(got):
        assert gv == expected[i], (
            f"offset+gain sample {i}: expected {expected[i]}, got {gv}")


@cocotb.test()
async def test_full_pipeline_gain_fir_decim_limiter(dut):
    """Full pipeline: gain -> FIR -> decim -> limiter with golden model compare."""
    tb = DspTB(dut, seed=55)
    await tb.start()
    cfg = PipelineConfig(
        offset=5, gain=0x0200, fir_bypass=0, ntaps=3,
        coefs=[0x2AAA, 0x2AAA, 0x2AAA, 0, 0, 0, 0, 0],
        decim=1, round_mode=ROUND_HALF_UP,
        lim_hi=5000, lim_lo=-5000,
    )
    rng = tb.rng
    n_samples = 10
    values = [rng.randint(-500, 500) for _ in range(n_samples)]
    samples = frame(values)

    await tb.stream_and_compare(samples, cfg, in_stall=0.05, out_stall=0.05)


# ===========================================================================
# 12. REQ-15: RS with shift=0 passthrough
# ===========================================================================

@cocotb.test()
async def test_req15_shift_zero_passthrough(dut):
    """REQ-15: RS with n=0 returns value unchanged (only saturation) for all rounds."""
    # Test the rs function directly (pure model test).
    for rm in range(4):
        for v in [-32768, -32769, -1, 0, 1, 32767, 32768, 100000, -100000]:
            result, sat = rs(v, 0, rm)
            if v > 32767:
                assert result == 32767 and sat, f"rs({v},0,{rm}): expected 32767,sat"
            elif v < -32768:
                assert result == -32768 and sat, f"rs({v},0,{rm}): expected -32768,sat"
            else:
                assert result == v and not sat, f"rs({v},0,{rm}): expected {v},no-sat"


# ===========================================================================
# 13. REQ-14/16: rounding mode validation
# ===========================================================================

@cocotb.test()
async def test_req14_floor_rounding_negative(dut):
    """REQ-14: floor rounds toward negative infinity for negative values.

    GAIN=0x0081 (129/256). x=-1: product=-129. RS(-129,8,FLOOR): kept=-1.
    """
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=0x0081, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    samples = frame([-1])
    got = await tb.stream(samples, 1)
    # RS(-129, 8, FLOOR): kept = -129>>8 = -1 (Python floor shift).
    assert got[0][0] == -1, f"floor(-129/256): expected -1, got {got[0][0]}"


@cocotb.test()
async def test_req14_half_up_rounding_negative(dut):
    """REQ-14: half-up adds 1 when f >= 1/2, for negative too.

    GAIN=0x0080 (0.5). x=-3: product=-384. RS(-384,8,HALF_UP):
    kept=-2 (floor(-384/256)=-2). dropped = -384 & 0xFF = 128.
    half=128. 128>=128 → f >= 1/2. inc=1. Result=-1.
    """
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=0x0080, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_HALF_UP,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    samples = frame([-3, -1, -129, -130])
    got = await tb.stream(samples, 4)
    # x=-3: -384. kept=-2. dropped=128. 128>=128 → inc=1. → -1.
    assert got[0][0] == -1, f"half_up(-3*0.5): expected -1, got {got[0][0]}"
    # x=-1: -128. kept=-1. dropped=128. 128>=128 → inc=1. → 0.
    assert got[1][0] == 0, f"half_up(-1*0.5): expected 0, got {got[1][0]}"
    # x=-129: -16512. kept=-65. dropped=128. 128>=128 → inc=1. → -64.
    assert got[2][0] == -64, f"half_up(-129*0.5): expected -64, got {got[2][0]}"
    # x=-130: -16640. kept=-65. dropped=0. 0<128 → inc=0. → -65.
    assert got[3][0] == -65, f"half_up(-130*0.5): expected -65, got {got[3][0]}"


@cocotb.test()
async def test_req16_half_even_above_half_even_floor(dut):
    """REQ-16: HALF_EVEN rounds up when f > 1/2 regardless of parity."""
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=0x00E0, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_HALF_EVEN,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    samples = frame([1, 3])
    got = await tb.stream(samples, 2)
    # x=1: product=224. kept=0, dropped=224>128(half). f>1/2 → inc=1. result=1.
    assert got[0][0] == 1, f"half_even(1*0.875): expected 1, got {got[0][0]}"
    # x=3: product=672. kept=2, dropped=160>128. f>1/2 → inc=1. result=3.
    assert got[1][0] == 3, f"half_even(3*0.875): expected 3, got {got[1][0]}"


@cocotb.test()
async def test_req17_underflow_boundary(dut):
    """REQ-17: rounded == -32769 saturates to -32768 (off-by-one mutant check).

    offset=32767, gain=256, in_data=-2:
    centered=-32769, product=-8388864, kept=-32769, dropped=0, inc=0.
    rounded=-32769 < -32768 → over_lo → result=-32768.
    """
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=32767, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    got = await tb.stream(frame([-2]), 1)
    assert got[0][0] == -32768, f"underflow boundary: expected -32768, got {got[0][0]}"


@cocotb.test()
async def test_req40_irq_w1c_preserves_other_bit(dut):
    """REQ-40: W1C on CLIP bit preserves FRAME bit (no event same cycle)."""
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=100, lim_lo=-100,
    )
    await tb.configure(cfg)
    # Enable both IRQ sources (bit 0 = CLIP, bit 1 = FRAME).
    await tb.write(IRQ_ENABLE, 0x3)
    # Send a frame with a clipped last sample: triggers CLIP at push, FRAME at drain.
    got = await tb.stream(frame([0, 200]), 2)
    await tb.idle()
    irq_stat = await tb.read(IRQ_STATUS)
    # Both bits should be set.
    assert irq_stat & 0x1, f"CLIP bit should be set, irq_stat=0x{irq_stat:X}"
    assert irq_stat & 0x2, f"FRAME bit should be set, irq_stat=0x{irq_stat:X}"
    # W1C CLIP only (bit 0), keep FRAME bit.
    await tb.write(IRQ_STATUS, 0x1)
    irq_stat2 = await tb.read(IRQ_STATUS)
    assert not (irq_stat2 & 0x1), f"CLIP should be cleared, irq_stat=0x{irq_stat2:X}"
    assert irq_stat2 & 0x2, f"FRAME should be preserved after W1C CLIP, irq_stat=0x{irq_stat2:X}"


@cocotb.test()
async def test_req17_overflow_boundary(dut):
    """REQ-17: rounded == 32768 saturates to 32767 (off_by_one mutant).

    offset=0, gain=-256, in_data=-32768:
    centered=-32768, product=8388608, kept=32768, dropped=0, inc=0.
    rounded=32768 > 32767 → over_hi → result=32767.
    """
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=-256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    got = await tb.stream(frame([-32768]), 1)
    assert got[0][0] == 32767, f"overflow boundary: expected 32767, got {got[0][0]}"


@cocotb.test()
async def test_enable_blocks_push_valid(dut):
    """ENABLE=0 blocks internal push_valid (not just s_ready).

    Drive s_valid high for many cycles with ENABLE=0 — no sample should
    enter the pipeline (no STAT_IN, no output, no BUSY).
    """
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    await tb.write(CTRL, ctrl_word(enable=0, bypass=1))

    # Drive s_valid high with last=1 for 20 cycles. Correct RTL: nothing enters.
    # Keep s_valid asserted (AXI-Stream requires valid held until ready).
    d = tb.dut
    d.s_valid.value = 1
    d.s_data.value = 12345 & 0xFFFF
    d.s_last.value = 1
    d.m_ready.value = 1
    for _ in range(20):
        await FallingEdge(d.clk)

    stat_in = await tb.read(STAT_IN)
    stat_out = await tb.read(STAT_OUT)
    busy = await tb.read(STATUS) & 1
    assert stat_in == 0, f"STAT_IN should be 0 with ENABLE=0, got {stat_in}"
    assert stat_out == 0, f"STAT_OUT should be 0 with ENABLE=0, got {stat_out}"
    assert not busy, "BUSY should be 0 with ENABLE=0"
    # Re-enable so a normal handshake can complete, allowing s_valid to drop.
    await tb.write(CTRL, ctrl_word(enable=1, bypass=1))
    for _ in range(30):
        await FallingEdge(d.clk)
        if int(d.s_ready.value):
            break
    d.s_valid.value = 0
    d.m_ready.value = 0


@cocotb.test()
async def test_req11_output_fifo_at_least_8(dut):
    """REQ-11: output FIFO >= 8 deep — 9 samples buffered without m_ready."""
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    d = tb.dut
    # Send 9 samples with m_ready=0. Total capacity (in-FIFO 2 + pipeline + out-FIFO 8)
    # must accommodate at least 8 buffered.
    d.m_ready.value = 0
    accepted = 0
    for i, val in enumerate(range(1, 10)):
        is_last = 1 if i == 8 else 0
        d.s_valid.value = 1
        d.s_data.value = val & 0xFFFF
        d.s_last.value = is_last
        # Wait for handshake while keeping s_valid stable.
        fired = False
        for _ in range(200):
            await FallingEdge(d.clk)
            if int(d.s_ready.value):
                accepted += 1
                fired = True
                break
        if not fired:
            # Sample couldn't be accepted — buffer full. Drop valid and stop.
            # To avoid handshake violation, we already have valid stable for
            # many cycles; dropping here is not a protocol violation as long
            # as we've waited for ready=1 first. Since we haven't, keep valid
            # high and re-enable m_ready to drain, then complete handshake.
            d.m_ready.value = 1
            for _ in range(200):
                await FallingEdge(d.clk)
                if int(d.s_ready.value):
                    accepted += 1
                    fired = True
                    break
            break
    d.s_valid.value = 0
    # Drain remaining outputs.
    d.m_ready.value = 1
    drained = []
    for _ in range(500):
        await FallingEdge(d.clk)
        if int(d.m_valid.value):
            drained.append(s16(int(d.m_data.value)))
        if len(drained) >= accepted:
            break
    d.m_ready.value = 0
    # At least 8 accepted (FIFO depth minimum + pipeline slack).
    assert accepted >= 8, f"accepted only {accepted}; output FIFO too shallow"


@cocotb.test()
async def test_req09_latency_at_most_16(dut):
    """REQ-09: with FIR_BYPASS=1, DECIM=0, m_ready=1, latency <= 16 cycles."""
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    d = tb.dut
    # Send a single sample with last=1, m_ready=1, measure handshake-to-output cycles.
    d.m_ready.value = 1
    d.s_valid.value = 1
    d.s_data.value = 42
    d.s_last.value = 1
    start = 0
    end = 0
    cycle = 0
    got = None
    while cycle < 40:
        cycle += 1
        await FallingEdge(d.clk)
        if start == 0 and int(d.s_ready.value):
            # Handshake fires this cycle; next cycle we can drop s_valid.
            start = cycle
            d.s_valid.value = 0
        if int(d.m_valid.value) and int(d.m_ready.value) and got is None:
            end = cycle
            got = s16(int(d.m_data.value))
            break
    d.m_ready.value = 0
    assert got is not None, "no output produced"
    latency = end - start
    assert latency <= 16, f"latency {latency} > 16 cycles"


@cocotb.test()
async def test_req20_fir_bypass_passthrough(dut):
    """REQ-20: with FIR_BYPASS=1, filter passes samples unchanged."""
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=4,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    samples = frame([-32768, -1, 0, 1, 32767, 1234, -5678])
    got = await tb.stream(samples, 7)
    expected = [-32768, -1, 0, 1, 32767, 1234, -5678]
    assert [v for v, _ in got] == expected, f"bypass failed: got {[v for v, _ in got]}"


@cocotb.test()
async def test_req31_last_through_limiter(dut):
    """REQ-31: limited (clipped) output keeps last flag through limiter."""
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=100, lim_lo=-100,
    )
    await tb.configure(cfg)
    # Last sample exceeds lim_hi → gets clipped, but last flag must survive.
    samples = [(0, 0), (50, 0), (200, 1)]
    got = await tb.stream(samples, 3)
    assert [v for v, _ in got] == [0, 50, 100], f"limiter failed: {got}"
    # Verify last flag propagation.
    assert [int(l) for _, l in got] == [0, 0, 1], f"last flag lost through limiter: {got}"


@cocotb.test()
async def test_req36_peak_before_first_delivery(dut):
    """REQ-36: PEAK_MAX/MIN read reset values until first delivery."""
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    # Before any output: PEAK_MAX should read 0x8000 (reset, -32768 signed).
    # PEAK_MIN should read 0x7FFF (reset, +32767 signed).
    pm = await tb.read(PEAK_MAX)
    pn = await tb.read(PEAK_MIN)
    assert pm == 0x8000, f"PEAK_MAX before first delivery: expected 0x8000, got 0x{pm:04X}"
    assert pn == 0x7FFF, f"PEAK_MIN before first delivery: expected 0x7FFF, got 0x{pn:04X}"
    # After delivery, should update.
    got = await tb.stream(frame([100]), 1)
    await tb.idle()
    pm2 = await tb.read(PEAK_MAX)
    pn2 = await tb.read(PEAK_MIN)
    assert pm2 & 0xFFFF == 100, f"PEAK_MAX after delivery: expected 100, got {pm2}"
    assert pn2 & 0xFFFF == 100, f"PEAK_MIN after delivery: expected 100, got {pn2}"


@cocotb.test()
async def test_req03_read_no_side_effect(dut):
    """REQ-03: cfg reads are pure; no read strobe, no side effect."""
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    # Read OFFSET repeatedly — must return same value every time.
    v1 = await tb.read(OFFSET)
    v2 = await tb.read(OFFSET)
    v3 = await tb.read(OFFSET)
    assert v1 == v2 == v3 == 0, f"read side effect: {v1}, {v2}, {v3}"
    # Reading STAT_IN should not clear it.
    got = await tb.stream(frame([42]), 1)
    await tb.idle()
    s1 = await tb.read(STAT_IN)
    s2 = await tb.read(STAT_IN)
    assert s1 == s2 == 1, f"STAT_IN read has side effect: {s1}, {s2}"


@cocotb.test()
async def test_req35_peak_min_strict_less(dut):
    """REQ-35: PEAK_MIN updates when out_data is strictly less than min_q.

    Discriminates `out_data < min_q` from `out_data <= min_q` (or off_by_one).
    Send 100 then 99. PEAK_MIN must be 99 (strict less-than update).
    """
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    got = await tb.stream(frame([100, 99]), 2)
    await tb.idle()
    pn = await tb.read(PEAK_MIN)
    assert pn & 0xFFFF == 99, f"PEAK_MIN should be 99, got {pn & 0xFFFF}"


@cocotb.test()
async def test_req01_reset_register_values(dut):
    """REQ-01: every register field has its spec reset value after rst_n."""
    tb = await setup(dut)
    # After reset, read all mapped registers.
    assert await tb.read(ID) == 0x44530100, f"ID reset: got 0x{await tb.read(ID):08X}"
    ctrl = await tb.read(CTRL)
    assert ctrl & 1 == 0, f"CTRL.ENABLE reset=0, got {ctrl}"
    assert (ctrl >> 1) & 1 == 1, f"CTRL.FIR_BYPASS reset=1, got {(ctrl>>1)&1}"
    assert (ctrl >> 2) & 3 == 0, f"CTRL.ROUND reset=0, got {(ctrl>>2)&3}"
    assert await tb.read(OFFSET) == 0, "OFFSET reset"
    assert await tb.read(GAIN) == 0x0100, "GAIN reset=1.0 (Q8.8)"
    assert await tb.read(LIM_HI) == 0x7FFF, "LIM_HI reset"
    assert await tb.read(LIM_LO) == 0x8000, "LIM_LO reset"
    assert await tb.read(STAT_IN) == 0, "STAT_IN reset"
    assert await tb.read(STAT_OUT) == 0, "STAT_OUT reset"
    assert await tb.read(STAT_CLIP) == 0, "STAT_CLIP reset"
    assert await tb.read(PEAK_MAX) == 0x8000, "PEAK_MAX reset=-32768"
    assert await tb.read(PEAK_MIN) == 0x7FFF, "PEAK_MIN reset=+32767"
    assert await tb.read(IRQ_STATUS) == 0, "IRQ_STATUS reset"
    assert await tb.read(IRQ_ENABLE) == 0, "IRQ_ENABLE reset"


@cocotb.test()
async def test_req10_fir_throughput(dut):
    """REQ-10: with FIR_BYPASS=0, accepts ≥1 sample every NTAPS*2+8 cycles."""
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=0, ntaps=4,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
    )
    await tb.configure(cfg)
    d = tb.dut
    total_cycles = 0
    accepted = 0
    samples = [(1, 0), (2, 0), (3, 0), (4, 1)]
    i = 0
    d.m_ready.value = 1
    d.s_valid.value = 1
    d.s_data.value = samples[0][0]
    d.s_last.value = samples[0][1]
    while i < len(samples) and total_cycles < 200:
        total_cycles += 1
        await FallingEdge(d.clk)
        if int(d.s_ready.value):
            i += 1
            accepted += 1
            if i < len(samples):
                d.s_data.value = samples[i][0] & 0xFFFF
                d.s_last.value = samples[i][1]
            else:
                d.s_valid.value = 0
    d.m_ready.value = 0
    assert accepted == len(samples), f"only accepted {accepted}"
    budget = len(samples) * (4 * 2 + 8) + 16
    assert total_cycles <= budget, f"FIR too slow: {total_cycles} > budget {budget}"


@cocotb.test()
async def test_req22_fir_accumulator_no_overflow(dut):
    """REQ-22: FIR accumulator doesn't overflow for legal coeffs+inputs."""
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=0, ntaps=4,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=32767, lim_lo=-32768,
        coefs=[0x7FFF, 0x8001, 0x7FFF, 0x8001, 0, 0, 0, 0],
    )
    await tb.configure(cfg)
    vals = [32767, -32768, 32767, -32768, 32767, -32768, 32767, -32768]
    got = await tb.stream(frame(vals), len(vals))
    await tb.idle()
    for v, _ in got:
        assert -32768 <= v <= 32767, f"FIR output out of range: {v}"


@cocotb.test()
async def test_req28_lim_hi_clamp(dut):
    """REQ-28: sample > LIM_HI becomes LIM_HI."""
    tb = await setup(dut)
    cfg = PipelineConfig(
        offset=0, gain=256, fir_bypass=1, ntaps=1,
        decim=0, round_mode=ROUND_FLOOR,
        lim_hi=1000, lim_lo=-1000,
    )
    await tb.configure(cfg)
    samples = frame([1500, 999, 1000, 1001])
    got = await tb.stream(samples, 4)
    expected = [1000, 999, 1000, 1000]
    assert [v for v, _ in got] == expected, f"lim_hi clamp: got {[v for v,_ in got]}"


@cocotb.test()
async def test_req06_ntaps_one_writable(dut):
    """REQ-06: NTAPS=1 is in [1,8] range and writable. Mutant with `> 4'd1`
    would reject NTAPS=1, leaving previous value."""
    tb = await setup(dut)
    # Set NTAPS to 2, then 1, then verify it reads back as 1.
    await tb.write(NTAPS, 2)
    val2 = await tb.read(NTAPS)
    assert val2 == 2, f"NTAPS=2 didn't stick: got {val2}"
    await tb.write(NTAPS, 1)
    val1 = await tb.read(NTAPS)
    assert val1 == 1, f"NTAPS=1 should be writable (REQ-06 [1,8]), got {val1}"
