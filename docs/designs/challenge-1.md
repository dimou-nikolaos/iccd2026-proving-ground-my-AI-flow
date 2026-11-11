# challenge-1: dsp_stream — Streaming Fixed-Point Sample Conditioner

## Companion documents (campaign retrospective)

- **`challenge-1-flow-log.md`** — narrative of the 4-phase workflow as executed
- **`challenge-1-tool-inventory.md`** — agents, skills, scripts, configs used per phase
- **`challenge-1-sub-catalog.md`** — every submission archive: parent chain, V/P, status
- **`challenge-1-sub-diffs.md`** — rebuild diff per submission vs its parent
- **`challenge-1-ppa-plan.md`** — PPA experiments, wins, dead-ends
- **`challenge-1-req-matrix.md`** — REQ ↔ test/assertion coverage
- **`challenge-1-formal-findings.md`** — SBY formal results

## Archetype: Datapath (streaming / transformation)

Pipeline: `s_* -> input FIFO -> gain -> FIR -> decimator -> limiter -> output FIFO -> m_*`

## Hierarchy

| File | Module | Role |
|------|--------|------|
| dsp_pkg.sv | package | Constants, register addresses |
| dsp_fifo.sv | dsp_fifo | Sync FIFO (parameterized width/depth) |
| dsp_rndsat.sv | dsp_rndsat | Arithmetic shift + round + saturate |
| dsp_regs.sv | dsp_regs | Register file (config + status + IRQ) |
| dsp_gain.sv | dsp_gain | RS((x - OFFSET) * GAIN, 8) |
| dsp_fir.sv | dsp_fir | FIR filter (FSM, shared multiplier, delay line) |
| dsp_decim.sv | dsp_decim | Boxcar decimator (group sum, shift by D) |
| dsp_limit.sv | dsp_limit | Clamp to [LIM_LO, LIM_HI] |
| dsp_stats.sv | dsp_stats | Counters (IN/OUT/CLIP) + peak hold |
| dsp_stream.sv | dsp_stream | Top-level wiring |

## Signal map (top level)

- Input handshake: s_valid/s_ready/s_data/s_last
- Output handshake: m_valid/m_ready/m_data/m_last
- Config: cfg_we/cfg_addr/cfg_wdata/cfg_rdata/cfg_err
- IRQ: irq (combinational from irq_stat_q & irq_en_q)

## Key parameters
- Clock: 15.0 ns period (generous timing)
- Input FIFO: DEPTH=2, WIDTH=17
- Output FIFO: DEPTH=8, WIDTH=17
- MAX_TAPS = 8
- FIR: NTAPS+3 cycles per sample (S_IDLE→S_MAC→S_ACC loop→S_DONE)
- Rounding: RS(value, shift) in dsp_rndsat — 3 instances (gain shift=8, FIR shift=15, decim shift=decim)

## Deviations found

| # | REQ | File:Line | Spec says | RTL does | Pattern | Confidence |
|---|-----|-----------|-----------|----------|---------|------------|
| 1 | REQ-17 | dsp_rndsat.sv:54 | underflow saturates to -32768 (0x8000) | saturates to -32767 (0x8001) | wrong constant | HIGH |
| 2 | REQ-37 | dsp_stats.sv:53-54 | STAT_CLEAR returns PEAK_MAX/MIN to reset values (0x8000/0x7FFF) | sets them to 0 | wrong reset value | HIGH |
| 3 | REQ-27 | dsp_decim.sv:62 | after group closes (any reason), reset sum/phase/clip for next group | only resets when group_full, not on in_last close | missing condition | HIGH |
| 4 | REQ-16 | dsp_rndsat.sv:36 | HALF_EVEN: f>1/2 always rounds up; f==1/2 rounds to even | `frac_ge_half && kept[0]` (ANDs parity with both cases) | missing disjunct | HIGH |

## CR-D1 implementation notes

1. **ROUND=3 (toward zero)**: In dsp_rndsat, for mode=3: `inc = (dropped != 0) && kept[IN_W-1]` (increment negative values with fraction toward zero). Remove `RND_RESERVED` from the default floor case.
2. **LAST_LEN at 0x060**: Add `A_LAST_LEN = 6'h18` to package, extend mapped to `word <= 6'h18`. Add frame counter in dsp_regs or dsp_stream. Count m_* handshakes per frame, update on frame_event.
3. **No port changes**, ID unchanged.

## V ceiling
- Local V samples across iterations: 0.85, 0.90, 0.925, 0.95, 0.975, 1.0 (seed-dependent)
- **Seed is content-hash of submission tar** — adding/removing any file rerolls the seed and changes which 40 mutants are drawn.
- sub-11 (61 tests) drew a lucky seed: V=1.0 local. sub-12 (63 tests, +REQ-12 +REQ-34) on its seed: V=0.975.
- sub-12 is the better submission because more REQ coverage → higher expected V over the unknown server seed, not lower.
- Common V-ceiling survivors: `dsp_decim.sv:57` always_ff `or negedge rst_n` drop_term — Verilator default-init equivalent. `dsp_stats.sv:42` 0xFFFF saturation requires 65K events (now testable via test_req34, but still may survive some seeds).
- **Don't chase V=1.0** — maximize REQ coverage; treat local V as a sample, not a target.

## PPA notes (final)
- **Baseline**: area=66379 µm², WNS=2.70 ns, power=31.45 mW, synth cells=5386
- **sub-11/sub-12 (best)**: area=68076 (+1697, −2.6%), WNS=4.78 (+2.08, +77% BETTER), power=19.76 mW (−37%). P=0.5658.
- **P score weights (inferred from measurements)**: t≈1.2, a≈0.27, p≈0.28. Timing slack dominates.
- **PPA wins applied** (cumulative):
  1. FIFO mem_q async-reset strip (Tier A, +0.034) — biggest single area win
  2. Gain 2-stage pipeline split (Tier B, +0.057) — biggest timing win, +3.63ns WNS
  3. FIR datapath reset strip (Tier A, +0.004)
- **PPA experiments tried and reverted (saved in runs/ppa-candidates/)**:
  - sub-fifo-rm (P=0.5612, Δ−0.005): input FIFO removal — WNS penalty
  - sub-acc-narrow (P=0.5518, Δ−0.014): FIR acc 40→35 bits — synthesis mapped worse
  - sub-fuse-lim (P=0.5424, Δ−0.023): decim rndsat + limiter fusion — WNS penalty
  - sub-fir-parallel (CTS fail): 8-mult parallel FIR — too many cells (18748 vs 7312)
- **Spec-freedom cross-check** (PDF analysis): 13 recommendations, 12 already in sub-11. The 3 unimplemented all turned P-negative. Sub-11 is at a local optimum.
- **Valid Explorer-award candidates**: 4 total (sub-best + 3 PPA variants with non-null P)
- **Don't try further PPA micro-optimizations** without spec-freedom justification + full-flow validation.
