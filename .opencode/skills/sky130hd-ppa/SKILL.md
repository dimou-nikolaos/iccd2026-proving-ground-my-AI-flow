---
name: sky130hd-ppa
description: Tech-specific PPA optimization for sky130hd + OpenROAD — register/reset restructuring, mux-tree balancing, fanout control, hold-time mitigation
---

Use when iterating on P (PPA score). All competition designs use sky130hd + ORFS.

## Top-10 checklist (apply ONE at a time, submit, measure P delta)

1. **Strip resets from datapath flops** — pipeline regs, captured data, buffers. Async-reset FF (`adfrq`) is larger than plain DFF; reset net adds routing congestion. Don't strip from FSM state, counters used in comparisons, anything observable in cycle-1 tests.
2. **Homogeneous reset style per module** — don't mix async and sync reset. Yosys can't share logic across mixed styles → fragmented layout.
3. **Narrow masked registers** — `reg & 0x0F0F` masks 12 effective bits; declare as 12-bit. Fewer DFFs, less routing.
4. **Clean clock-enable for DFFRE inference** — single `if (en)` with `d_next` mux pre-computed outside. Nested `if-else` blocks enable inference, kills ICG (power).
5. **Replace `>=` with `==` where overshoot impossible** — `==` is 24 gates, `>=` is 48 gates at 16-bit. 2× cheaper.
6. **Manual balanced mux-tree** — never a 8-16-way `case` or if-else chain. Build via intermediate signals in a 2-stage or 3-stage binary tree. Critical path = log₂(N) muxes instead of N.
7. **Parenthesize arithmetic** — `Z = (A+B) + (C+D)` not `Z = A+B+C+D`. Forces balanced adder tree.
8. **Balance parallel combinational depths** — paths meeting at same register must have similar gate counts. OpenROAD pads the short path with dlybuf cells → area explosion.
9. **Manual register replication for high-fanout signals** — `gen_q`, `update_pulse`, etc. Use `logic [N-1:0] gen_q_repl; ... <= {N{gen_d}}` so each channel has its own driver register.
10. **Operand isolation on idle math blocks** — AND/mux that zeroes multiplier/adder inputs when output not read. Cuts dynamic power.

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

Don't pre-optimize for these. Write RTL that EXPOSES structure (clean enables, clean resets, balanced trees, homogeneous reset) so yosys CAN use them.

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
