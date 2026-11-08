---
name: sky130hd-ppa
description: Tech-specific PPA optimization for sky130hd + OpenROAD — register/reset restructuring, mux-tree balancing, fanout control, hold-time mitigation
---

Use when iterating on P (PPA score). All competition designs use sky130hd + ORFS.

## Step 0: spec-freedom cross-check (BEFORE applying checklist items)

Have an LLM (or expert human) read the spec and produce a list: **"What is NOT externally observable, or only constrained by a MINIMUM requirement?"** Typical categories:
- Buffer/FIFO depth minimums ("at least N") — implement exactly N.
- Throughput minimums (e.g., "1 sample per NTAPS*2+8 cycles") — leaves room for multicycle/iterative architectures.
- Latency maximums (e.g., "≤16 cycles in bypass") — leaves room for pipeline choice elsewhere.
- Operation uniqueness ("same RS at every stage") — opens sharing options.
- Behavior UNDEFINED (config changes during BUSY, concurrent W1C vs event beyond spec, etc.) — opens scheduling freedom.
- Internal accumulator widths — minimum-sufficient width per math, NOT power-of-2 by default.

Cross-check the resulting list against your current architecture. Most items will already be implemented. The remaining items are your Tier B/C PPA candidate list. Items on the list that have already been tried and P-negative in measurement are a strong signal the architecture is at a local optimum for that topology.

**Anti-pattern**: skipping this step and trying "obvious" area cuts that don't actually map to spec freedom. You waste submissions on changes that aren't actually allowed-or-safe.

**Challenge-1 result**: external analysis produced 13 freedoms; 12 already implemented in sub-11; 3 untried (input FIFO removal, RS sharing across stages, narrow FIR accumulator widths) — all 3 measured P-negative. Confirmed sub-11 is at local optimum.

## Top-10 checklist (apply ONE at a time, submit, measure P delta)

1. **Strip resets from datapath flops** — pipeline regs, captured data, buffers. Async-reset FF (`adfrq`) is larger than plain DFF; reset net adds routing congestion. Don't strip from FSM state, counters used in comparisons, anything observable in cycle-1 tests. **Confirmed (challenge-1):** stripping the result-FIFO data memory (208 flops) saved **614 synth cells** and all of area/timing/power beat baseline (P +0.027). **Anti-pattern (challenge-2):** stripping async reset from an 8-bit pcount_q in a control+bus design saved only 42 cells (8 reset muxes) but degraded WNS by 0.21ns and raised power +5%; net P −0.007. **Threshold:** reset-strip only pays when the flop array is ≥64 bits AND the register is read-after-write / gated by a valid flag so its reset value is never observed. Caveats: (a) it can *nudge timing the wrong way* — a second strip (32 flops) cut area again but lowered WNS, net P −0.007, so **isolate reset-strips (one per submission, measure ΔP); do not batch them together** — but *do* batch them with provably-free independent changes per `docs/competition-strategy.md` → "PPA iteration economy"; (b) if a **bound reset assertion** checks that register's reset value, remove that assertion too (it would self-fire on the new RTL — the orchestrator gate catches it).
2. **Homogeneous reset style per module** — don't mix async and sync reset. Yosys can't share logic across mixed styles → fragmented layout.
3. **Narrow masked registers** — `reg & 0x0F0F` masks 12 effective bits; declare as 12-bit. Fewer DFFs, less routing. **Caveat (challenge-2):** narrowing IRQ_STATUS/IRQ_ENABLE from 32→12 bits saved 43 cells but degraded WNS by 0.31ns (OpenROAD re-routing shifted the critical path). Net P −0.007. Only apply when the register is NOT on the timing-critical path, or when congestion is the bottleneck.
4. **Clean clock-enable for DFFRE inference** — single `if (en)` with `d_next` mux pre-computed outside. Nested `if-else` blocks enable inference, kills ICG (power).
5. **Replace `>=` with `==` where overshoot impossible** — `==` is 24 gates, `>=` is 48 gates at 16-bit. 2× cheaper. **Caveat 1 (challenge-2, prescaler):** NOT safe when config can change mid-operation — if `pcnt_q` was 5 and prescale changes from 10 to 3, `pcnt_q >= prescale` fires but `pcnt_q == prescale` does not; `>=` is required. **Caveat 2 (challenge-2, channel wrap):** `count_q >= period_act_q` IS equivalent to `==` (count resets at period_act_q, never exceeds), but replacing it degraded WNS 1.80→1.55 and saved only 58 cells net P +0.001 (wash). **Rule:** only apply when (a) overshoot is truly impossible AND (b) the comparator is NOT on the critical path.
6. **Manual balanced mux-tree** — never a 8-16-way `case` or if-else chain. Build via intermediate signals in a 2-stage or 3-stage binary tree. Critical path = log₂(N) muxes instead of N.
7. **Parenthesize arithmetic** — `Z = (A+B) + (C+D)` not `Z = A+B+C+D`. Forces balanced adder tree.
8. **Balance parallel combinational depths** — paths meeting at same register must have similar gate counts. OpenROAD pads the short path with dlybuf cells → area explosion.
9. **Manual register replication for high-fanout signals** — `gen_q`, `update_pulse`, etc. Use `logic [N-1:0] gen_q_repl; ... <= {N{gen_d}}` so each channel has its own driver register.
10. **Operand isolation on idle math blocks** — AND/mux that zeroes multiplier/adder inputs when output not read. Cuts dynamic power.
11. **Precompute config-derived shifts/masks at load, not in the datapath** — a `v << (32-width)` barrel shifter or `v & mask(width)` that is re-derived every cycle from a *constant-per-transaction* config is pure combinational logic toggling continuously → big dynamic power + a long path. Compute it once when the config is latched (store the already-shifted/masked value) and read the register in the hot path. Behavior-identical; huge power/timing win. (challenge-1: this moved P 0.45→0.54, power −34%.)

## Algorithmic patterns (Phase 4e wildcards, apply AFTER Tier A/B/C exhausted)

Structural moves plateau P at ~0.55-0.60 on datapath designs. To break 0.70 you typically need one of these. Apply only after spec-freedom cross-check (Step 0) confirms the spec doesn't pin the current algorithm.

### Arithmetic restructurings

**Aggressive pipelining to max latency budget** — the single biggest lever. Timing score has ~1.2 weight in P; each pipeline split buys +0.8-1.2 ns WNS. Challenge-1 REQ-09 gives 16-cycle latency budget; typical use is 8-10 cycles. **Slack is free pipeline stages**. Push WNS from 4.78 → 8-9 ns by splitting every combinational stage ≥3 LUT depth.

**CSD (Canonical Signed Digit) for constant coefficients** — replace `x * C` (compile-time C) with a shift-add tree using C digits (only {−1,0,+1}, average half the density of binary). Saves ~50% of add ops and reduces the multiplier footprint. Only applies when C is a literal, not a runtime config — check spec.

**Carry-save accumulation** — a running sum `acc <= acc + new_term` has the CPA (carry-propagate add) on the critical path every cycle. Replace with `(sum, carry)` pair: `sum_next = sum XOR carry XOR new_term; carry_next = majority(...)`. Collapse to a single value only when the accumulator is read. Cuts accumulator critical path by ~N-gate delay for N-term accumulation.

**Distributed arithmetic FIR** — for `y = Σ Cᵢxᵢ` with compile-time constants, precompute a ROM indexed by bits of x across taps; replace multipliers with shift+ROM lookups. Big area win when N taps × bit-width is small vs coefficient space.

**Transposed FIR form** — rearrange `y = Σ Cᵢx[n-i]` into `y[k] = Σ partial_sums` where each tap has its own accumulator. Shorter critical path (single multiply per stage), more registers, better for aggressive pipelining.

**Polyphase decomposition (FIR+decimator)** — for FIR followed by decimation-by-M, decompose the filter into M sub-filters operating on interleaved input. Only compute partial sums at decimation positions; skip (M-1)/M of the FIR work. Only applies if decimation factor is compile-time constant — skip if runtime config.

### Storage restructurings

**Memory inference for delay lines / FIFOs** — `reg [W-1:0] mem [0:D-1]` with simple read/write ports synthesizes to an SRAM macro on sky130hd (sram macros exist in the tech lib). A 128-bit FIR tap delay as flops costs ~500 µm²; as SRAM costs ~150 µm². Caveat: async read ports don't infer; must use sync read (registered output).

**Multi-bit flop packing** — adjacent flops with identical clock/enable get bundled into 2x/4x wide standard cells by Yosys `opt_dff`. This only triggers when the RTL structure is clean (single `always_ff` block with parallel assignments, not split across conditions). If you've split an `always_ff` for enable-condition clarity, you've blocked packing. Consolidate.

### Yosys attributes (RTL hints that survive synthesis)

- `(* fsm_encoding = "one_hot" *)` on FSM state reg — forces one-hot encoding even if `Yosys` default would pick binary. **Strong P wildcard on small FSMs (4-8 states)**: challenge-1 measured +0.0049 P in one shot on FIR 4-state FSM. Higher hit rate (~1/2) than `(* keep *)` (~1/8). Cost: 1-2 extra flops. Risk: NONE (attribute-only). Try on every detected FSM. Other encodings (`"gray"`, `"binary"`, `"sequential"`) tend to tie at the Yosys-default optimum.
- `(* keep = "true" *)` — prevents Yosys from constant-prop / merging / eliminating a signal. **Counter-intuitive use as P wildcard**: forcing a wire to survive often IMPROVES P (timing) via different downstream mux tree routing. Challenge-1: keep on dsp_rndsat.kept gained +0.0042 P; keep on dsp_fir.acc_q added another +0.0015 (different module — stacks). See § `(* keep *)` wildcards below.
- `(* dont_touch = "true" *)` — prevents cell replacement / merging on this instance. (Challenge-1: stacks with `keep` on same wire tend to tie or regress; redundant.)
- `(* maxfreq = N *)` — timing hint for ABC. Rarely effective; OpenROAD's STA dominates.

### Clock/power

**Explicit ICG cell instantiation** — the flow's Yosys script may or may not infer integrated clock gates from `if (en)` patterns depending on version. Instantiating `sky130hd_dfm` (flop with enable) directly forces ICG. Big power win on rarely-enabled datapaths.

**Operand isolation on idle blocks** — multiply/add inputs gated to zero during hold cycles: `mult_in_a = en ? a_in : 16'b0`. Prevents datapath toggling. Power −5 to −15% on sparse-activity blocks. **challenge-1 measured (9 ISO variants)**: wins ONLY on multipliers (P +0.012 FIR 16×16 mult ISO, P +0.002 gain 17×33 mult ISO). Lost on stats comparator (−0.015), FIR acc adder (−0.007), FIR rndsat input (−0.025), gain rndsat input (−0.003), decim sum adder (assertion broke), gain subtractor+mult combined (−0.027). **Rule**: ISO pays only when (a) gated element is a multiplier or high-toggle wide adder, AND (b) the gating AND/mux isn't itself on the critical path.

## Order to apply (safe → risky)

| Order | Action | C risk | P gain |
|-------|--------|--------|--------|
| 1 | Free from fix (remove extra comparator) | none | high |
| 2 | Narrow masked registers | none | medium |
| 3 | Homogenize resets per module | none | small |
| 4 | Replace `>=` with `==` (overshoot impossible) | low | medium |
| 5 | Clean enable pattern for ICG | none | small (power) |
| 6 | Balanced mux tree | none | medium (timing) |
| 7 | Arithmetic parenthesization | none | small |
| 8 | Strip datapath resets | low-medium | medium |
| 9 | Register replication for fanout | none | medium |
| 10 | Operand isolation | low | small |
| 11 | Pipeline deep combinational | HIGH (cycle tests) | high (timing only) |

## `(* keep *)` wildcards (low-risk PPA perturbation)

`(* keep = "true" *)` on an internal wire prevents Yosys's constant-prop, structural hashing, and merging from eliminating it. This usually costs area but can WIN P via different downstream mux-tree routing. Pure attribute — no semantic change, no test impact, no C risk.

**Challenge-1 measured** (22 keep variants tried across 4 batches):
- v20: single keep on `dsp_rndsat.kept` → P +0.0042 vs sub-16
- v28: stack on rndsat + single keep on `dsp_fir.acc_q` (DIFFERENT module) → P +0.0025 more
- Same-module stacks (v21-v24 on rndsat internals): ALL regressed or plateaued
- Cross-module stacks (v25, v27): tied or marginal; v26 (decim) regressed

**Rules** (from L27, L28, L29):
1. Try 1 keep per variant on each candidate wire in a wide batch (~16 signals = 16 variants, ~25 min wall in 4-parallel pg-batch mode). Hit rate ~1/8 wins, +0.001 to +0.005 P each win.
2. **Cross-module stacks COMPOUND** (rndsat + fir wins → both stacks add). **Same-module stacks PLATEAU** (rndsat + rndsat loses).
3. After 2 winning cross-module keeps, plateau usually reached. Don't try 3+ stacks without measurement.

## What Yosys does for free — don't fight it

- CSE, constant folding, arithmetic resource sharing
- Cell sizing (drive strength, buffers)
- Tie cell insertion for constants
- Multi-bit DFF packing
- **Redundant AND-mask elimination** — `reg & 0x0FFF` where upper bits are provably zero (constant-0 in set_vec, or reset guarantees) is already collapsed by yosys's opt_expr pass. Removing the mask manually changes zero cells (challenge-2: removing `& IRQ_BITS` on status_q yielded +2 cells net due to routing variance).

Don't pre-optimize for these. Write RTL that EXPOSES structure (clean enables, clean resets, balanced trees, homogeneous reset) so yosys CAN use them.

## Control+bus design PPA ceiling

**Challenge-2 finding (pwm_timer, 4-channel APB completer, 2600–2900 cells):** P ceiling ≈ **0.50** (parity with baseline). Achieved via three *stacked* wins: ICG-friendly flop restructure (timing win) + wrap `>=→==` (shorter critical path) + AND-OR read mux with one-hot decode (area win) + operand isolation on idle adder (area+power win). Best P = 0.4965, area component POSITIVE (+0.018).

**Why not P > 0.50:** CR-2 mandates 4× 16-bit comparators + 8 IRQ flops (spec-required area). Baseline had unoptimized decode/mux that we clawed back to parity; the CR additions are the net extra.

**When PPA optimization IS effective:**
- Large designs (>4000 cells): enough routing slack for area savings to convert to P gains.
- Designs with clear datapath hot spots (barrel shifters, multipliers, FIFO arrays ≥64 bits).
- Precompute-at-load patterns (challenge-1: moved P 0.45→0.54, power −34%).
- Small control+bus designs when the baseline RTL has *unoptimized* decode/mux that we can replace (challenge-2: AND-OR read mux saved 241 cells).

**When PPA optimization is NOT effective:**
- Narrow-register rewrites (IRQ 32→12): save cells but WNS collapses (OpenROAD route noise dominates).
- Module fusion (inline submodule): no CSE win, hurts V (mutations on now-unused module file become no-op equivalents → survivors).
- Precompute small expressions into flops (cmp_m1_q): 16 flops cost more than the 16-bit adder saved.
- Spec-freedom counter reversals (countdown prescaler): may save comparator but need shrink-guards (OR conditions = more cells than the original).
- **Critical-path restructure when Yosys is already optimal (challenge-1, L30):** if the OpenROAD critical path is a `unique case` mux tree on cfg_addr → cfg_rdata, hand-restructuring to one-hot decode + AND-OR tree (or adding `(* parallel_case *)`) makes P WORSE, not better. Challenge-1: 4 restructures all regressed P (−0.011 to −0.024). The reported "critical path" is NOT automatically a recoverable asset; Yosys's default may already be the best lowering for the chosen cells. Always measure one variant of a restructure before committing to a series.

## Proven PPA wins (control+bus, challenge-2)

| Technique | RTL pattern | P impact | When to apply |
|-----------|-------------|----------|---------------|
| **ICG-friendly 3-branch flop** | `if (load) q <= init; else if (step) q <= next; else hold;` instead of `q <= mux(load, step, init, next, q)` | Timing +0.03 to +0.04, area wash | Any counter/register with distinct load and step conditions |
| **`>=` → `==` on wrap-style comparisons** | `count >= period` → `count == period` when invariant `count <= period` always holds | Timing −0.01 alone, +0.008 bundled | Any counter that resets on reaching bound |
| **AND-OR read mux** | `one_hot_decode` + `assign rd = (bitsel[0] & val0) \| (bitsel[1] & val1) \| ...` instead of `unique case` | Area +0.03, WNS −0.2 (must be bundled) | Register file read paths with 6–10 sources. **Caveat (challenge-1, L30):** for some designs Yosys's default lowering of `unique case` is ALREADY optimal; manual AND-OR rewrite LOSES P (−0.019 to −0.024). Challenge-2 pwm_timer AND-OR was a win (−241 cells, P +0.03); challenge-1 dsp_regs AND-OR was a loss. **Measure first; don't trust "the critical path is the mux tree therefore restructure it."** |
| **Operand isolation on idle adders** | `in_iso = en ? in : 0; sum = in_iso + 1` (adder input frozen when disabled) | Area +0.002, power +0.005 | Any adder enabled <50% of cycles |
| **Wrap shortening + mux restructure bundle** | Combined: wrap `==` shortens hot path, AND-OR mux saves area | Net +0.008 (bundled); alone each loses | Only apply together |

**Sequencing rule:** ICG-style flop restructure FIRST. It buys timing slack. Then spend slack on area/power wins (mux restructure, operand isolation, narrow regs). Attempting area wins without prior timing slack regresses WNS and P collapses.

**Timing slack is the currency.** Every area/power win must fit inside the WNS slack established by prior timing improvements. Never stack two area-only wins without a timing win in between.

## Decision heuristic

- If the design has a hot counter/timer/register with a load/step/hold pattern, apply ICG restructure (free timing win).
- If WNS slack > 1.5 ns after that, apply one area/power win (AND-OR mux, operand isolation).
- If still WNS slack, stack one more timing win (wrap `==`, remove redundant terms).
- Never strip small async resets (<64 bits) — costs more than saves.
- Never fuse modules — V drops and no P win.

## Verification flow

```bash
# 1. Fast flow (skip PPA) — confirm C, V unchanged
docker run --rm --cpus 12 ... run --skip ppa
# 2. Full flow (with PPA) — measure P delta
docker run --rm --cpus 12 ... run
# 3. Read runs/out/stages/ppa.json — compare final area/WNS/power vs baseline
# 4. If P improved AND C unchanged → keep, iterate next optimization
#    If C dropped → revert, that signal/behavior is in hidden tests
```

## PPA failure diagnosis

| Failure | Cause | Fix |
|---------|-------|-----|
| Route DRC fail at step 'route' | Too congested | Strip resets, narrow registers, replicate fanout |
| Hold-time violation post-CTS | Skewed parallel paths | Balance combinational depths |
| Setup WNS<0 | Long comb path | Mux balancing, parenthesization, register pipeline |
| Power high (no timing issue) | Missing ICG / operand isolation | Clean enable pattern, mask inputs |
| **CTS step crashes (no WNS report)** | **Cell count too high (>2× baseline) or extreme congestion** | **Abandon variant — too aggressive.** Reduce parallelism (e.g., 8-mult FIR → 4-mult), split into pipeline stages, or revert. Challenge-1: 8-mult parallel FIR (18748 cells vs baseline 7312) crashed at CTS reporting stage; ORFS couldn't write metrics file. Server would reject (C=0). |

A CTS-stage crash means the design is functionally valid but physically infeasible at the configured clock period and utilization. The flow returns `status: rejected` with P=null. **Do not submit such variants to the server** — they will be rejected (C=0) and don't count for the Explorer award.

## For deeper context
Read `docs/skills/sky130hd-optimization.md` (full patterns, code examples, sky130 cell library details, baseline behavior of ORFS stages).
