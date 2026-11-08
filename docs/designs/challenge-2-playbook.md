# Challenge-2 playbook (control + bus, expected Nov 10)

**Prep notes drafted Nov 8 from challenge-1 learnings, before design release.**

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

## PPA levers (predicted)
- **FSM encoding**: one-hot vs binary (challenge-1 showed one-hot LOST — likely also for ctrl designs since binary is smaller)
- **Bus mux trees**: address decoder fanout can dominate timing — check critical path early (use `critical-path.sh`)
- **Register file replication**: high-fanout control signals benefit from per-instance copies via wire attributes (challenge-1 showed `(* keep *)` wires get optimized away — NOT a reliable lever)
- **Operand isolation**: only multipliers/wide adders benefit; small ALU ops lose
- **Async → sync reset**: only pays on WIDE flop arrays (>64 bits)

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
