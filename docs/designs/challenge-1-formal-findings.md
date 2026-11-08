# Formal Verification Attempt — Findings

## Tools available
- SBY 2 (SymbiYosys) at /usr/local/bin/sby
- yosys 0.68 with `read_slang` (SystemVerilog frontend)
- Engines: smtbmc yices (only one in image)

## RTL static analysis (sufficient for our needs)

### REQ-37 (STAT_CLEAR wins over concurrent event)
Location: `challenge-1/rtl/dsp_stats.sv:31-44` (counters), `:51-57` (peak hold)
Pattern: `if (clear) <reset> else if (events) <increment>`
→ clear takes priority. **Correct.**
Pinned by simulation-level next-state assertions in `challenge-1/assertions/dsp_stats_sva.sv` (a_inq_ns, a_outq_ns, a_clipq_ns, a_maxq_ns, a_minq_ns). All run every cycle of every test, never fire.

### REQ-40 (W1C loses to concurrent event for SAME bit)
Location: `challenge-1/rtl/dsp_regs.sv:121`
Pattern: `irq_stat_q <= (irq_stat_q & ~cfg_wdata[1:0]) | {ev_frame, ev_clip}`
The clear mask is applied first, then OR with event. **Event wins.**
Pinned by simulation assertion `a_irqstat_ns` in `dsp_regs_sva.sv`. Runs every cycle.

### REQ-13 (clip combine across decim group)
Location: `challenge-1/rtl/dsp_decim.sv` — `clip_or_q` accumulates clip across the group.
Pinned by simulation assertions and 2 cocotb tests (test_req13_clip_across_decim_group, test_req13_clip_from_gain).

### REQ-01/02 (reset clears all state)
All `always_ff @(posedge clk or negedge rst_n)` blocks have explicit reset assignments. Verified via static scan.

## Why SBY attempt failed
- Yosys `read_verilog` (slang-less) rejects modern SV features (unpacked-array ports, `default disable iff`).
- Yosys `read_slang` accepts RTL but rejects SVA `|=>` and `|->` patterns as "unsupported SVA feature" in this slang version.
- Workaround attempt: convert assertions to immediate-style `if (...) assert(...)` inside `always @(posedge clk)`. SBY found CEX at step 1 due to free initialization of helper regs (`anyinit_driver_*` overrides setundef -zero).
- Could continue with `chformal -anyinit -remove` and `chformal -init assume` to force proper initial state, but ROI is low given simulation assertions already cover the same ground.

## Decision
**Skip formal.** Static inspection + simulation next-state assertions already pin REQ-37/40/13/01/02. Hidden-test risk for these REQs is mitigated by:
- cocotb test_req37_full_stat_clear, test_req37_stat_clear_peaks
- cocotb test_req40_irq_w1c, test_req40_irq_w1c_preserves_other_bit
- cocotb test_req13_*
- cocotb test_req01_reset_register_values, test_req02_reset_midstream

## General lesson extracted
SBY setup cost (~1-2h for a non-trivial RTL) vs simulation next-state assertions (already in place from Phase 3b) gives very low ROI for per-cycle properties. Reserve formal for:
- Deep temporal (>20 cycle) properties
- Reachability questions
- Induction-style unbounded proofs
- CEX generation to disambiguate a MEDIUM-confidence deviation that no test reaches

For challenge-1, formal wasn't needed — Phase 3b simulation assertions cover the same ground.
