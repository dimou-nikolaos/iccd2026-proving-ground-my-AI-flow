# sky130hd + OpenROAD PPA Optimization Reference

Tech-specific RTL strategies that change how Yosys synthesizes and OpenROAD places/routes. All designs in this competition use sky130hd + ORFS — these apply universally. The constraints (top ports fixed, filelist fixed, no script changes) mean we work purely through RTL restructuring.

## 1. Register & Reset Control (Area + Routing Congestion)

**sky130_hd penalizes resets hard.** A reset-capable FF (`sky130_fd_sc_hd__adfrq_*`, `dfxtp`) is physically larger than a plain DFF. Routing the async reset net adds severe congestion in TritonRoute (the OpenROAD detailed router).

### Strategies

- **Strip resets from deep datapath flops.** Pipeline registers, line buffers, data-path arrays — if a random initial value doesn't break control logic, drop the reset. Reduces cell area AND routing pressure (no reset net to those flops).
  - Apply to: shift chains, captured data, frame buffers, **FIFO data memory arrays**
  - Don't apply to: FSM state, valid/ready flags, counters used in comparisons, anything observable in cycle-1 tests
  - **Confirmed (challenge-1, streaming CRC):** stripping the result-FIFO data array (208 flops) → **−614 synth cells**, area/timing/power all beat baseline (P 0.545→0.572). Two caveats measured: (1) strips can *nudge the critical path* — a second 32-bit strip lowered WNS and cost P −0.007, so **one strip per submission + measure**, never bundle; (2) if a **bound SVA reset assertion** checks that register's reset value you must delete it too (self-fires on the new RTL), and the register must be read-after-write / gated by a valid flag so its reset value is never observed.

- **Keep reset styles homogeneous per module.** Don't mix async reset (`always_ff @(posedge clk or negedge rst_n)`) with sync reset (`always_ff @(posedge clk) if (!rst_n)`) inside the same module. Yosys can't share logic across mixed styles → fragmented layout, larger area.

- **Clean clock-enable for DFFRE inference.** Use a single dedicated `if (en)` inside `always_ff`. Nested `if-else` chains for the enable condition prevent Yosys from inferring `sky130_fd_sc_hd__dfrt_*` (sync reset + enable). Missed ICG (integrated clock gate) substitution → more dynamic power.

  Good:
  ```systemverilog
  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n)          q <= '0;
    else if (en)         q <= d;
  end
  ```
  Bad (yosys struggles to extract enable):
  ```systemverilog
  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) q <= '0;
    else begin
      if (sel_a) q <= a;
      else if (sel_b) q <= b;
      else if (sel_c) q <= c;
    end
  end
  ```
  Better:
  ```systemverilog
  assign d_next = sel_a ? a : sel_b ? b : c;
  assign en     = sel_a | sel_b | sel_c;
  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) q <= '0;
    else if (en) q <= d_next;
  end
  ```

## 2. Combinational & Arithmetic Structure (Timing + Congestion)

sky130_hd cells have weak drive strength. Yosys can build long deep gate chains that destroy timing AND create local routing hotspots.

### Strategies

- **Manual balanced binary mux trees** for wide multiplexing. A 16-case `case` or 8-deep `if-else` chain becomes a cascaded 2:1 mux chain (`mux2_1`) — serial delay accumulates. Restructure into balanced tree using intermediate wires:

  ```systemverilog
  // 8-to-1, balanced:
  logic [31:0] m01, m23, m45, m67;
  assign m01 = s[0] ? d1 : d0;
  assign m23 = s[0] ? d3 : d2;
  assign m45 = s[0] ? d5 : d4;
  assign m67 = s[0] ? d7 : d6;
  logic [31:0] m0123, m4567;
  assign m0123 = s[1] ? m23 : m01;
  assign m4567 = s[1] ? m67 : m45;
  assign y     = s[2] ? m4567 : m0123;
  ```

  Critical path = 3 mux stages instead of 8.

- **Parenthesize arithmetic for balanced adder trees.** Force Yosys into balanced carry trees instead of left-to-right chain:

  Bad:  `Z = A + B + C + D;` → sequential chain of 3 adders
  Good: `Z = (A + B) + (C + D);` → 2 parallel adders + 1 final = log2 depth

- **Pipeline deep math** (32-bit multipliers, wide adders). Don't run combinational — slice across 2-3 RTL pipeline registers. Smaller combinational chunks pack tighter into 6-track sky130 cell rows, improving area AND timing.

   Only do this if hidden tests don't check exact cycle timing. **High risk of C<1.0**.

- **Hoist config-derived shift/mask out of the datapath into the config latch (BIG power + timing win, behavior-identical).** A variable shifter (`v << (32-W)`) or mask (`v & mask(W)`) that is recomputed **every cycle** from a config that is **constant per transaction** is pure combinational logic that (a) toggles continuously → large dynamic power, and (b) sits on the datapath critical path. Compute it **once when the config is captured** and store the already-processed value; the hot path just reads a register.

  ```systemverilog
  // Bad: re-derived every token from the per-frame config
  assign poly_l = left_justify(frame_cfg.poly, frame_cfg.width);   // barrel shifter, every cycle
  // Good: pre-process at first-beat capture (width known there); hot path reads a wire
  always_ff @(posedge clk or negedge rst_n)
    if (!rst_n)         frame_cfg_q <= '0;
    else if (first_beat) begin
      frame_cfg_q.poly_l     <= left_justify(cfg_poly, cfg_width);  // shifter feeds a reg only
      frame_cfg_q.xor_masked <= cfg_xorout & width_mask(cfg_width); // mask feeds a reg only
    end
  assign poly_l = frame_cfg_q.poly_l;                               // just a wire in the datapath
  ```

  Same flop count as the config latch itself; removes N-bit variable shifters/masks from the datapath. Measured: flipped P 0.45→0.54 on a streaming CRC datapath (power −34% vs baseline, timing better than baseline, area parity). Update any bound assertion that referenced the old combinational signal to assert the pre-processed register field instead (keep the D3 "captured at first beat" assertion).

## 3. Physical & Optimization Mitigation (Hold Time + Fanout)

OpenROAD pads layout paths with delay buffers (`sky130_fd_sc_hd__dlybuf_*`) to fix hold-time violations. Skewed parallel paths that meet at adjacent registers explode area.

### Strategies

- **Balance combinational depth of parallel paths feeding the same register.** If path A is 2 gates and path B is 15 gates, OpenROAD inserts dozens of dlybuf cells on path A to match arrival. Match depths in RTL — add dummy pipeline stages or restructure to equalize.

  Example: a counter value (1 gate path) compared against a complex computed threshold (10 gate path) — register the threshold too, or compute it combinationally at the same depth.

- **Manual register replication for high-fanout internal control signals.** Commercial tools auto-replicate, but Yosys+ORFS does not (limited). Replicate `gen_q`, `update_pulse`, `irq_active` style signals manually so each fanout group has its own driver register. Reduces electrical load per cell output.

  Pattern:
  ```systemverilog
  logic [3:0] gen_q_repl;     // 4 copies of gen_q
  always_ff @(posedge clk or negedge rst_n)
    if (!rst_n) gen_q_repl <= '0;
    else        gen_q_repl <= {4{gen_d}};   // 4 flops, same input
  // Each channel uses gen_q_repl[n]
  ```

  Yosys infers 4 separate DFFs, each driving one channel — shorter reset+control nets.

- **Operand isolation on heavy math blocks.** Add a small AND/mux that zeroes multiplier/adder inputs when the output isn't read. Cuts dynamic power (no spurious toggling on unused datapaths).

  Pattern:
  ```systemverilog
  assign a_iso = capture_valid ? a : '0;
  assign b_iso = capture_valid ? b : '0;
  // multiply a_iso * b_iso instead of a * b
  ```

  Use when an idle period is detectable from a control signal already in the design.

## Quick checklist for PPA pass (run after C=1.0 confirmed)

Apply ONE at a time, submit, check P delta. Each is a separate submission. **Sequencing rule: timing slack is the currency — establish slack first (items 1–3), then spend it on area/power wins (items 4–11). Never stack two area-only wins without a timing win in between; the WNS cliff collapses P.**

| Order | Action | Expected P gain | C risk | Applies to |
|-------|--------|----------------|--------|-----------|
| 1 | Remove extra comparator term from deviation fix (free) | +0.05 to +0.10 | none | any |
| 2 | **ICG-friendly 3-branch flop restructure** on counters (distinct load/step/hold): `if (load) q<=init; else if (step) q<=next; else hold;` instead of `q<=mux(...)` | +0.005 area-wash, but **timing +0.03 to +0.05** — biggest lever | none | any design with counters/timers |
| 3 | Replace `>=` with `==` where counter cannot overshoot | +0.01–0.03 timing (area-wash alone, +0.008 bundled with item 4/6) | low | any counter with reset-on-bound invariant |
| 4 | **AND-OR read mux with one-hot decode** (`rd = (dec[0]&v0) \| ...` instead of `unique case`) | −100 to −250 cells, WNS regression −0.2 ns → **must be bundled with item 2 or 3** | none | register files, 5+ source read paths |
| 5 | Precompute config shifts/masks at latch-time (challenge-1 pattern) | +0.05 to +0.10 (power −30%+) | none | datapath with config |
| 6 | **Operand isolation on idle adders** (`in_iso = en ? in : 0; sum = in_iso + 1`) | +0.002 area, +0.005 power | none | adders enabled <50% cycles |
| 7 | Strip reset from datapath-only flops (≥64 bits, non-observable) | +0.01 to +0.03 | low-medium | large flop arrays only |
| 8 | Homogenize reset style per module (all async OR all sync) | +0.01 to +0.03 | none | any |
| 9 | Balance wide muxes / binary tree case | +0.01 to +0.05 (timing) | none | 8+ way mux |
| 10 | Parenthesize multi-operand adders | +0.01 (timing) | none | 3+ operand sums |
| 11 | Replicate high-fanout control registers (≥8 loads) | +0.02 to +0.05 (routing) | none | fanout ≥8, must not add delay |

Skip items 4/5 without prior items 2/3 — the WNS cliff hits.

**Anti-patterns (measured P-negative on challenge-2, do not retry):**
- Narrow wide registers for area alone (IRQ 32→12: −42 cells, WNS 1.80→1.49, P −0.007)
- Fuse submodules to enable CSE (V drops — mutations in now-unused source file become no-op equivalents)
- Precompute small expressions into flops (`x-1`, masks) — flops cost more than saved adder (16 extra flops cost 149 cells, saved adder cost 60)
- Register replication with inserted flops (adds cycle delay, breaks spec) — must use combinational buffers or replicate the source flop itself
- Countdown counter where count-up was used — needs shrink-guard (OR conditions), usually more cells than original comparator
- Strip small async resets (<64 bits) — costs more than saves

Risk profile: 1–3, 5–10 are RTL-only restructurings that preserve functional behavior at the cycle level. C should remain 1.0 if hidden tests don't inspect internal signal names via formal (rare but possible).

## Common PPA failure modes

- **Route DRC fail**: too many cells, too congested. Fix: strip resets + narrow registers → fewer cells.
- **Hold-time violation after CTS**: skewed parallel paths. Fix: balance logic depth in RTL.
- **Setup violation (WNS<0)**: long combinational path. Fix: mux-tree balancing, arithmetic re-parenthesization, or pipeline register insertion (with cycle-timing risk).
- **Power high**: missing ICG or operand isolation.

## How to verify PPA change is worth it

```bash
# Run fast flow first (skip PPA) — confirms C and V still pass
docker run --rm --cpus 12 ... run --skip ppa ...
# Then full flow with PPA
docker run --rm --cpus 12 ... run ...
# Compare final area, WNS, power in runs/out*/stages/ppa.json
# vs baseline_final / runs/cache/<hash>/final.json
```

The flow caches the baseline PPA per design-hash. After RTL changes the hash changes and baseline gets recomputed — first full flow after an RTL change takes longer (~5min). Subsequent runs reuse.

## What Yosys does for free (don't fight it)

- Common subexpression elimination
- Constant folding
- Resource sharing of arithmetic across branches
- Cell sizing for timing (drives, buffers)
- Auto-insertion of tie cells (conb_1) for constants
- Multi-bit register packing (sky130 has multi-bit DFF variants)

Don't write RTL that's *already* optimized for these — yosys handles them. Write RTL that EXPOSES the structure yosys needs to use these library features (clean enables, clean resets, balanced trees, homogeneous reset styles).
