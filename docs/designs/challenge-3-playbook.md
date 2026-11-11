# Challenge-3 playbook (pipelined core, expected Nov 13)

**Prep notes drafted Nov 8 from challenge-1 learnings.**

## See also: challenge-1 retro docs

Challenge-1 completed end-to-end. When working challenge-3, **read before Phase 4**:

- `docs/designs/challenge-1-flow-log.md` — wave-by-wave narrative of how we iterated
- `docs/designs/challenge-1-tool-inventory.md` — agents/skills/scripts per phase
- `docs/designs/challenge-1-ppa-plan.md` — PPA wins/losses with measured ΔP
- `docs/designs/challenge-1-sub-catalog.md` + `challenge-1-sub-diffs.md` — rebuild recipes

Challenge-3 is **pipeline-heavy**, which is challenge-1's strongest area (the gain 2→3 stage split was the single biggest P win, +0.060 P). Apply that lesson aggressively here.

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

## PPA levers (CORRECTED with challenge-1 measured data)

| Pattern | Verdict (from challenge-1) | Measured evidence |
|---------|----------------------------|-------------------|
| **Pipeline stage splits at natural boundaries** | **STRONG WIN** | challenge-1 gain 2→3 stage split: **+0.060 P single-shot**. Biggest single P lever in campaign. |
| **Operand isolation on multiplier** | WIN on multipliers only | FIR mult ISO +0.0121, gain mult ISO +0.0145 (stack) |
| **Operand isolation on adder/comparator** | LOSS | FIR adder ISO −0.007, decim sum ISO −0.015, rndsat ISO −0.025 |
| **`(* fsm_encoding = "one_hot" *)` on small FSMs** | STRONG WIN (high hit rate) | FIR 4-state FSM: +0.0049 single-shot (L31) |
| **`(* keep = "true" *)` on wires (cross-module stacks)** | WIN, hit rate ~1/8 per wire | cumulative +0.012 across 4 wires in different modules (L29) |
| **Forwarding mux restructure** | **LIKELY LOSS** — see L30 | challenge-1 cfg_addr→cfg_rdata read mux restructure ALL LOST P by −0.01 to −0.03. Yosys's default lowering already optimal. |
| Register file RAM inference | Not tested in challenge-1 | Try on challenge-3 if reg-file present. |
| Per-stage always_ff consolidation | Likely neutral (Yosys opt_dff already does it) | Skip unless profile shows specific issue. |
| Async → sync reset wildcards | LOSS unless spec allows; creates C-risk if spec mandates async-only | L32 — challenge-1 lost 4h undoing this. |

### New for challenge-3 (predicted but unverified)
- Multi-port register file RAM inference: try `(* ram_style = "block" *)` or yosys `memory_libmap` directives.
- Branch predictor FSM (if present) — apply fsm_encoding one_hot.
- Forwarding MUX operand isolation — gate to 0 when no-forward active. Challenge-1 pattern suggests this MIGHT lose (comparators lose from gating mux delay), but pipeline forwarding mux trees are bigger than challenge-1's small comparators. Worth 1 experiment.

### Challenge-3 specific PPA opportunity
Pipeline cores have MORE natural split points than datapath. Each stage boundary is a split candidate. Challenge-1 found +0.060 from ONE split. **Challenge-3 should have 4-6 split candidates**. Try aggressively:
1. Identify WNS path with `scripts/critical-path.sh` after baseline flow.
2. If path crosses multiple stages, try splitting at each stage boundary (one variant per boundary, batch via `scripts/pg-batch.sh`).
3. If path stays within one stage, look at the intra-stage combinational block for ISO / fsm_encoding opportunities.

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
