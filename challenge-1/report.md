# Design 1 — dsp_stream

## Deviations Fixed

| REQ | File | Spec requirement | Baseline bug | Fix |
|-----|------|-----------------|--------------|-----|
| REQ-17 | dsp_rndsat.sv | Underflow saturates to -32768 | Output was -32767 (0x8001) | Changed constant to 0x8000 |
| REQ-37 | dsp_stats.sv | STAT_CLEAR restores PEAK_MAX/MIN to reset values | Set to 0 instead of 0x8000/0x7FFF | Use SAMPLE_MIN/SAMPLE_MAX |
| REQ-27 | dsp_decim.sv | After any group close (full or last), reset sum/phase/clip | Only reset on group_full, not on in_last close | Changed condition to close_group |

## Change Request CR-D1 Implemented

1. **ROUND=3 (toward zero)**: In dsp_rndsat, mode 3 now increments when fraction is nonzero and value is negative (toward zero).
2. **LAST_LEN at 0x060**: New RO register holding the output frame length of the most recently completed frame. Frame counter in dsp_stream.sv increments on m_* handshakes, latches on m_last handshake, resets on STAT_CLEAR.
3. **Mapped range extended**: cfg_err = 0 for offset 0x060. Offsets above 0x060 are unmapped.

## Verification

- Verilator lint: clean (0 warnings)
- Yosys elaboration: synthesizable, 0 problems
- Public testbench: 10/10 PASS
