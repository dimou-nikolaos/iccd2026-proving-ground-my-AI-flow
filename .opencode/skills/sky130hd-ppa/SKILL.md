---
name: sky130hd-ppa
description: Tech-specific PPA optimization for sky130hd + OpenROAD — register/reset restructuring, mux-tree balancing, fanout control, hold-time mitigation
---

Use when iterating on P (PPA score). All competition designs use sky130hd + ORFS.

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

## Proven PPA wins (control+bus, challenge-2)

| Technique | RTL pattern | P impact | When to apply |
|-----------|-------------|----------|---------------|
| **ICG-friendly 3-branch flop** | `if (load) q <= init; else if (step) q <= next; else hold;` instead of `q <= mux(load, step, init, next, q)` | Timing +0.03 to +0.04, area wash | Any counter/register with distinct load and step conditions |
| **`>=` → `==` on wrap-style comparisons** | `count >= period` → `count == period` when invariant `count <= period` always holds | Timing −0.01 alone, +0.008 bundled | Any counter that resets on reaching bound |
| **AND-OR read mux** | `one_hot_decode` + `assign rd = (bitsel[0] & val0) \| (bitsel[1] & val1) \| ...` instead of `unique case` | Area +0.03, WNS −0.2 (must be bundled) | Register file read paths with 6–10 sources |
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

## For deeper context
Read `docs/skills/sky130hd-optimization.md` (full patterns, code examples, sky130 cell library details, baseline behavior of ORFS stages).
