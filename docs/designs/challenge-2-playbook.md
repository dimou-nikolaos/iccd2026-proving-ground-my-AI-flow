# Challenge-2 playbook (control + bus, expected Nov 10)

**Prep notes drafted Nov 8 from challenge-1 learnings, before design release.**

## See also: challenge-1 retro docs

Challenge-1 completed end-to-end (datapath, 138 submissions, 34h agent time). When working challenge-2, **read these before Phase 4**:

- `docs/designs/challenge-1-flow-log.md` — what we did, wave-by-wave
- `docs/designs/challenge-1-tool-inventory.md` — what tools/agents/skills each phase used
- `docs/designs/challenge-1-ppa-plan.md` — PPA wins/dead-ends with measured ΔP per wildcard class
- `docs/designs/challenge-1-sub-catalog.md` + `challenge-1-sub-diffs.md` — how to rebuild any historical variant; useful as pattern for batch-generated P variants

Below we update PPA predictions with challenge-1 measured results (correcting some Nov 8 guesses).

## Expected design shape
- FSM-driven state machine
- Bus interface (likely AXI4-Lite or APB-style with handshake)
- Peripherals: registers, memory map, IRQ
- Multiple internal sub-blocks

## Likely spec patterns (predicted REQ categories)
- **Reset**: register reset values, midstream reset, sync vs async
- **Bus protocol**: setup/hold of strobes, write/read handshake, error response on bad address
- **State machine**: legal transitions, error states, mid-transaction reset recovery
- **Memory**: write/read timing, address decode, partial write (byte enable)
- **IRQ**: edge vs level, write-1-clear semantics, masking
- **Clock domain**: likely single clock but possible CDC scenarios

## Common seeded deviation patterns to hunt
1. **Off-by-one** in state machine transitions or counters
2. **Wrong reset value** on individual fields
3. **Missing edge case** (e.g., simultaneous set+clear, clear priority)
4. **Wrong comparison** (`>` vs `>=`, signed vs unsigned)
5. **Missing condition** in next-state logic
6. **Wrong width** in bus response (16-bit vs 32-bit reads)
7. **Bus protocol violation**: not holding data when ready=0, or accepting when ready=0
8. **Async reset asserted mid-bus-transaction** — must clear properly
9. **Write-1-clear semantics**: same-cycle event vs write — wrong priority
10. **Missing IRQ set on edge**, only on level

## PPA levers (CORRECTED with challenge-1 measured data)
- **FSM encoding wildcards — STRONG**: `(* fsm_encoding = "one_hot" *)` on small FSMs (2-4 states) is one of the highest hit-rate P levers. Challenge-1 measured +0.0049 P single-shot on FIR 4-state FSM (L31). Hit rate ~1/2. Cost: 1-2 extra flops. **Try this FIRST** on every FSM in the design.
- **Wire `(* keep = "true" *)` — RELIABLE**: counter-intuitive P wildcard. Challenge-1 measured +0.0042 (rndsat.kept) +0.0025 (fir.acc_q) +0.0045 (stats counters) +0.0013 (fir.idx_q) = ~+0.012 cumulative. Hit rate ~1/8 per wire. **Stack across modules** (L29: cross-module keeps compound; same-module stacks plateau). Try on each module independently.
- **Bus mux trees**: address decoder fanout can dominate timing — check critical path early (use `scripts/critical-path.sh`). Challenge-1 cfg_addr→cfg_rdata read mux was on critical path; **Yosys's default `unique case` lowering was already optimal** (L30). Don't waste time restructuring the mux tree — try Yosys attribute wildcards first.
- **Operand isolation**: only multipliers/wide adders benefit; small ALU ops lose (L17). Challenge-1 measured: FIR mult ISO +0.012, gain mult ISO +0.015; FIR adder ISO −0.007, rndsat ISO −0.025.
- **Async → sync reset**: only pays on WIDE flop arrays (>64 bits, L20). **But creates REQ-42-class C-risk if spec mandates async-only reset** (L32). Challenge-1 lost 4h undoing this. Check spec text before applying.

### New PPA dead-ends discovered in challenge-1 (don't waste time)
- Critical-path mux-tree restructure (parallel_case, AND-OR tree, keep on read word) — all LOST (L30)
- Operand isolation on adders/comparators (already covered above)
- Fusing rndsat + limiter (variable comparator wider, slower)
- Narrow accumulator bits (sky130 standard-cell mapping often WORSE for non-power-of-2 widths)
- Same-module stacked keeps (plateau after 2 in same module — go cross-module)

## Predicted Tier A/B/C sequence

| Tier | Action | When |
|------|--------|------|
| A | Baseline tests + assertions to lock in C=1.0 | Day 1 |
| B | Spec gap tests for hidden-test robustness | Day 1-2 |
| B | Critical path extraction + datapath pipeline splits if path is in datapath | Day 2 |
| B | Operand isolation on multipliers/wide adders ONLY | Day 2 |
| C | Aggressive: bus mux tree restructure, parallel FSM state decode | Day 3+ |
| 4e | Cosmetic variants for Explorer (8+ per design minimum) | Always |

## Tooling reuse (from challenge-1)
- `scripts/pg-batch.sh` — parallel full-flow runner
- `scripts/synth-filter.sh --top <TOP>` — pre-filter obvious area regressions
- `scripts/submodule-synth.sh` — fast per-module area comparison
- `scripts/critical-path.sh` — openroad path report from --keep-work runs

All validated on challenge-1. Reuse as-is for challenge-2.

## Specific to-control-design experiments worth trying on day 1

1. **Critical path extraction immediately** after first flow run — don't assume datapath is critical; in control-heavy designs it's usually bus mux/decoder
2. **Bus handshake assertions** with `bind` to catch valid/ready protocol deviations
3. **Formal SBY on FSM** — small FSMs verify cleanly; find illegal transitions formally
4. **Mutation analysis** — control designs may have specific mutant patterns (state-transition mutations especially)

## What NOT to repeat from challenge-1
- Don't blindly add pipeline registers without checking critical path (cost 0.015 P on FIR S_OUT)
- Don't try operand isolation on small ALU blocks (lost 4/6 times in challenge-1)
- Don't add `(* keep *)` wire replications — get optimized away
- Don't extend sync-reset to narrow flop groups (loses on most)
