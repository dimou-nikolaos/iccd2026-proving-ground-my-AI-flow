# Challenge-3 playbook (pipelined core, expected Nov 13)

**Prep notes drafted Nov 8 from challenge-1 learnings.**

## Expected design shape
- Multi-stage pipeline (4-6 stages: fetch/decode/execute/mem/wb)
- Hazards: data (forwarding), control (branch flush), structural
- Possibly interrupts/exceptions
- Register file, ALU, memory interface

## Likely spec patterns
- **Pipeline stages**: each stage's combinational function
- **Forwarding**: source of forwarded value when multiple hazards possible
- **Stalls**: which cycles insert, propagation backwards
- **Branches**: predict/flush semantics, when flush propagates
- **Pipeline emptying**: reset behavior mid-pipeline
- **Pipeline bubbles**: when invalid stages occur, behavior of downstream stages
- **Interrupts**: timing of save-state, vector fetch

## Common seeded deviation patterns to hunt
1. **Wrong forwarding source** (e.g., EX/MEM vs MEM/WB priority)
2. **Missing stall** for one hazard type
3. **Wrong flush width** in branch flush (one stage vs two)
4. **Reset not clearing all stages** — partial reset
5. **Wrong exception vector** or wrong save-state contents
6. **Pipeline bubble causing data corruption** downstream
7. **Wrong comparison for branch** (signed vs unsigned)
8. **Missing valid-pipeline** signal propagation (e.g., a stage that shouldn't accept when bubble)
9. **Interrupt latency** off by 1
10. **NOP handling** (zero instruction) — wrong zero value or wrong sign extension

## PPA levers (predicted)

| Pattern | Likely wins | Likely loses |
|---------|-------------|--------------|
| Deeper pipeline splits | YES — pipeline cores have natural split points | If new reg overhead > timing saving |
| Operand isolation on ALU mult | YES (if present) | No-op if single-cycle ALU |
| Forwarding mux restructure | Maybe — multiple parallel mux trees possible | If Yosys already optimal |
| Register file RAM inference | YES — multi-port register files can use SRAM macros | Yosys may not infer well for multi-port |
| Per-stage always_ff consolidation | Marginal (Yosys already does opt_dff) | If consolidation breaks enable inference |

## Predicted Tier A/B/C sequence

| Tier | Action | When |
|------|--------|------|
| A | Baseline tests + assertions to lock C=1.0 | Day 1 |
| A | REQ matrix completion (every REQ has test + assertion) | Day 1 |
| B | Critical path extraction — pipeline cores often have forwarding mux chains as critical path | Day 2 |
| B | Stage-internal pipeline splits at natural boundaries (between stage regs) | Day 2 |
| B | Operand isolation on multiplier if present | Day 2 |
| C | Multi-port regfile RAM inference attempt | Day 3+ |
| 4e | Cosmetic variants for Explorer | Always |

## Specific to pipeline designs

1. **Each stage boundary is a natural pipeline split point** — already pipelined in spec, but adding INTERNAL splits (split one combinational stage into two pipelined sub-stages) is the main PPA lever
2. **Forwarding network is the typical critical path** — extracting critical path will tell you
3. **Stall signals fan out across many regs** — duplication may help
4. **Pipeline valid bits** — typically a chain, low fanout

## Tooling reuse
All scripts from challenge-1 work directly:
- `scripts/pg-batch.sh`
- `scripts/synth-filter.sh --top <CORE>`
- `scripts/submodule-synth.sh <root> <ALU_or_stage>`
- `scripts/critical-path.sh`

## What NOT to repeat from challenge-1
- Same list as challenge-2 playbook (see above)
- Additionally: don't try one-hot FSM encoding on control FSMs unless FSM is on critical path
