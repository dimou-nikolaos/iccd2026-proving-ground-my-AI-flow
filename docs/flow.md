# Proving Ground Verification Workflow

This is the master workflow. Read it once at session start. It tells you what to do, in what order, and where to find the detailed instructions for each step.

**Companion docs** (read on demand, not upfront):
- `docs/flow-lessons.md` — 15 measured pitfalls, organized by phase. Read before starting Phase 1 of a new design.
- `docs/flow-ppa-and-v.md` — Phase 4b (PPA tiers) and Phase 4c (mutation kill) details. Read when entering Phase 4.
- `docs/joint-flow.md` — operator+agent cycle and handoffs.
- `docs/competition-strategy.md` — scoring priorities, timing, PPA tier budget.

## Overview

```
Phase 1: ANALYZE  →  Phase 2: FIX  →  Phase 3: VERIFY  →  Phase 4: ITERATE
   (find bugs)       (correct RTL)     (build evidence)     (kill mutants, PPA)
```

Each phase produces artifacts consumed by the next. Phases 1-2 are design-specific (you analyze a particular RTL). Phases 3-4 are verification-specific (you write tests and assertions, then iterate against the flow).

**State tracking**: After each phase completes, write progress to `docs/state/<design>-progress.md`. A fresh session reads this to know where to pick up.

## Phase 1: Analyze (find deviations)

**Goal**: Identify all deviations between spec.md and baseline RTL. A deviation is a place where RTL behavior contradicts a REQ-xx requirement.

**Step 0 — classify the archetype**: datapath / control+bus / pipelined core. Load `docs/skills/design-playbooks.md` and pull the matching section into every later task card (it routes which mutation classes, assertion patterns, test strategy, and PPA levers to prioritize per type).

**Three complementary methods**, run in this order:

### 1a. Static analysis
- **Agent**: `@spec-analyst`
- **Skill**: load `spec-deviation-hunting` for the detection checklist
- **Deep reference**: `docs/skills/bug-hunting.md`
- **Method**: Read spec, read RTL. For each REQ, locate the implementing code and check the expression against the spec text. Flag suspicious expressions.
- **Output**: findings table with confidence (HIGH/MEDIUM/LOW)
- **Known bug patterns**: off-by-one at boundary comparisons; missing register latch; missing reset branch; extra disjunct; wrong combinational order; missing qualifier.

### 1b. Formal-assisted (confirms HIGH + resolves MEDIUM)
- **Agent**: `@formal-writer`
- **Skill**: load `formal-verification-flow` for sby setup steps
- **Deep reference**: `docs/skills/formal-flow.md`
- **Method**: Write SVA properties derived from spec. Run SymbiYosys against baseline RTL. CEX = deviation confirmed.
- **ROI caveat (L13)**: in our flow image, SBY formal is often not worth the setup cost vs simulation next-state assertions. Reserve for deep temporal/reachability/unbounded induction.

### 1c. Probe simulation (confirms remaining MEDIUM/LOW)
- **Agent**: `@probe-creator`
- **Method**: Write a minimal cocotb test (5-20 lines) that exercises the suspected deviation. Run against baseline. Fail = confirmed.
- **Output**: probe test code + pass/fail result

**Phase 1 exit criterion**: Confirmed deviation table (3 deviations per design). After Phase 1, run probe-per-REQ sweep — see L1.

---

## Phase 2: Fix RTL

**Goal**: Correct all confirmed deviations and implement the change request — with PPA in mind.

- **Agent**: `@rtl-fixer`
- **Deep reference**: `docs/agents/rtl-fixer.md` (the fix methodology)
- **Design doc**: read `docs/designs/<name>.md` for hierarchy and signal names

**PPA-aware fix principles** (apply DURING implementation, not after):
- Off-by-one fixes are FREE (change literal). Apply immediately.
- Reordering fixes (e.g., reflect-XOR) often REDUCE logic. Favor them.
- Config latch fixes: minimize area. Options ranked:
  1. Pass config through existing pipeline (no new flops)
  2. Latch only width-relevant bits (mask to max needed width, not full 32)
  3. Full latch (last resort — costs area, hurts P)
- CR implementation: share datapath. New width = mux entry, not new stage.
- After each fix: check if any intermediate register/logic became dead. Remove it.

**After-fix checks** (must all pass — Docker only, never host tools; exact commands in `pg-flow-commands` skill / `docs/running-the-flow.md`):
1. Filelist unchanged (`diff` vs released `rtl/filelist.f`)
2. Verilator lint — no new warnings/errors
3. Yosys elaboration — synthesizable (`read_slang` → `hierarchy -check` → `proc` → `check`)
4. Public testbench — passes (except tests the CR intentionally changes)

**Phase 2 exit criterion**: Clean lint + elaboration + public TB passes (except tests that exercise the old buggy behavior, which the CR intentionally changes).

---

## Phase 3: Verify (build evidence)

**Goal**: Write cocotb tests and SVA assertions that (a) prove the fixes work, (b) prove the CR is implemented, (c) will kill mutants in Phase 4.

**Cover-everything-up-front principle (L12)**: enumerate REQs and write ≥1 test + ≥1 assertion per REQ in the first pass. ~5 lines per REQ is essentially free; iterating to "fill gaps after the first mutation run" wastes 2-3 cooldown cycles.

### 3a. Tests
- **Agent**: `@test-writer`
- **Skill**: load `cocotb-test-writing` for model and driver templates
- **Deep reference**: `docs/skills/cocotb-patterns.md`
- **Design doc**: read `docs/designs/<name>.md`

**Golden model strategy**:
- **Control designs** (register interfaces, FSMs): **cycle-accurate** Python model. Runs in lockstep with DUT. Checks every cycle.
- **Datapath designs** (streaming, transformation): **event-driven** Python model. Computes expected output per transaction. Timing abstracted by handshake drivers.

**Test taxonomy** (write tests in each category):
1. **Deviation tests**: one per confirmed deviation. Must FAIL on baseline.
2. **Change-request tests**: exercise new feature with known acceptance values.
3. **Boundary tests**: saturation edges, empty/zero, boundary comparisons.
4. **Protocol tests**: handshake, backpressure, reset mid-operation.
5. **Constrained-random tests**: random configs within spec constraints, random data, multiple seeds.
6. **Register-map tests** (if applicable): full address space, PSLVERR, byte strobes, W1C.

**Naming**: `test_req<NN>_<description>` or `test_cr<N>_<description>`

**REQ coverage matrix (MANDATORY)**: build `docs/designs/<name>-req-matrix.md` with one row per REQ (enumerate from spec — `grep -oE 'REQ-[0-9]+' spec.md | sort -u`), columns: REQ id, test name(s) that cover it, assertion id(s) that cover it, status. Every REQ must have ≥1 test and ≥1 assertion. If a REQ is untestable (e.g., synthesis-only like REQ-42 "no latches"), write the row and justify.

### 3b. Assertions
- **Agent**: `@assertion-writer`
- **Skill**: load `sva-assertion-writing` for bind templates and assertion categories
- **Deep reference**: `docs/skills/sva-patterns.md`
- **Design doc**: read `docs/designs/<name>.md` for signal names and hierarchy

**Assertion strategy** (by priority for mutation kills):
1. **Signal-definition assertions** (highest kill value): `assert property (output == <spec formula>)`. Kills any mutation to the RHS expression.
2. **Next-state assertions** — use the **one-cycle-back identity**, not `|=>` with mixed cycles: `q == ($past(en) ? f($past(d)) : $past(q))`. Always `$past()` the enable/clear/config too — pairing `$past(event)` with current clear/config is the classic self-fire bug.
3. **Protocol invariants**: handshake rules, output stability, ready/valid mutual rules.
4. **Reset assertions** (no `disable iff`): checks registers are 0 after reset.
5. **Boundary/range assertions**: value ranges, saturation behavior.

**Critical rule**: assertions must NEVER fire on the correct fixed RTL. If they fire on your own submission, the submission is REJECTED. The **orchestrator must run tests with `ASSERTIONS=1` on a fresh `sim_build` and confirm 0 fires before packaging**, even when the subagent reports success.

**Top-level interface assertion module (MANDATORY)**: write one assertion module bound to the TOP module that checks every externally visible interface protocol. For AXI-style streams: `valid && !ready |=> valid && $stable(data) && $stable(last)` on every valid/ready pair. See L3.

**Submodule interface assertions (defensive, REQ-bound)**: for each submodule, if a REQ constrains its interface, bind a small assertion module that checks the interface contract — not the internals. These survive refactors; signal-def assertions on old internals do not. Prefer interface assertions when planning Tier B/C PPA work.

**Phase 3 exit criterion**:
- All tests pass on fixed RTL with `ASSERTIONS=1`
- At least 3 tests fail on baseline RTL
- No assertion fires on fixed RTL
- **REQ coverage matrix is complete** — every REQ has ≥1 test and ≥1 assertion (or justified untestable)
- Top-level interface assertion module exists and binds to the top

---

## Phase 4: Iterate (C → P → V)

**Goal**: Maximize score = 100 × C × (0.5V + 0.5P). Priority: **C=1.0 first, P second, V third.**

For Phase 4b PPA tiers, Phase 4b' aggressive redesigns, and Phase 4c mutation-kill details, read **`docs/flow-ppa-and-v.md`**.

### 4a. Submit early, get C feedback

After Phase 2 fixes (even before tests/assertions are written), submit the RTL alone. The flow returns:
- **C**: pass/fail names of hidden regression tests and hidden properties
- **V**: with only public tests (baseline mutant kill rate)
- **P**: PPA vs baseline (area, timing, power at each stage)

**C is server-only.** The local (Docker) flow returns `C = null`. Bank C early with an rtl+report drop, and treat the server report as the ONLY ground truth for C. Do not assume local green ⇒ C=1.0.

**If C < 1.0** (highest priority): read FAILED TEST NAMES → diagnose missed deviation / wrong CR interpretation / edge case → fix RTL → resubmit.

**If C = 1.0**: proceed to P and V optimization.

### 4b. PPA optimization

Run after C=1.0 locked. See `docs/flow-ppa-and-v.md` for tier definitions, iteration economy budget, stacked recipes per design archetype, and the spec-freedom improvisation protocol.

**Quick rules**:
- Tier A: provably-free, bundle, no submission cost.
- Tier B: timing-neutral, **1 C-check submission before stacking more**.
- Tier C: structural, one per submission, validate by **full PPA flow** (L14).
- **Timing slack is the dominant P contributor** (challenge-1: t≈1.2, a≈0.27, p≈0.28). Protect WNS aggressively (L9, L10).

### 4c. Mutation kill

REQ coverage first (Phase 3a + 3b naturally kill 30-38 of 40). For the residue, see `docs/flow-ppa-and-v.md` for the per-line method and V-ceiling expectation. **V seed rerolls on every archive change** (L5) — don't chase a specific seed's V=1.0; maximize REQ coverage as the proxy for expected V.

### 4d. Rapid iteration loop

With 30min cooldown + 15min flow time, use the wait productively:

```
Submit → while waiting 30min:
  - Work on PPA optimization (next RTL variant)
  - Write more assertions (next V improvement)
  - Analyze PPA report from previous submission
→ Submit next variant (improved P or V)
```

**Always keep `runs/sub-best.tar.gz`** — never overwrite a good submission with a worse one.

**Phase 4 exit criterion**: C = 1.0, P ≥ 0.50, V at design ceiling.

### 4e. Wildcard exploration (optional, after Phase 4 exit criteria met)

Triggered when C=1.0 is banked AND Tier A/B/C are exhausted AND cooldown time is available. Uses local time only; each survivor costs 1 C-check submission. **See `docs/flow-wildcard.md` for the full protocol.**

**Quick summary**: brainstorm 3-5 aggressive moves (algorithmic — CSD multipliers, carry-save, distributed arithmetic, polyphase, transposed FIR — or aggressive pipelining to max REQ-latency budget, or memory inference, or ICG cell insertion). Try each locally with full PPA flow validation (L14). Bank survivors. Record every attempt (win or lose) in `<design>-ppa-plan.md` "Wildcards tried" section so the next design inherits the learning.

Typical P gains available in Phase 4e that Tier A/B/C cannot reach: +0.05 to +0.15 on a well-constrained design. Enough to push P from 0.55 → 0.70 and score from ~77 to >82.

---

## Iteration and Looping

Phase 4 is iterative. The orchestrator loops:
```
run flow → parse survivors → spawn @mutant-hunter (with survivor list) →
→ check results → repeat
```

Each iteration's task card includes ONLY the new survivor list (not full context — the agent already wrote evidence in the previous round and can see it).

**Escalate to user when**:
- V plateaus after 3 iterations without improvement
- A surviving mutant appears semantically equivalent (mutation should have been filtered)
- PPA optimization conflicts with mutation kill coverage
- Spec ambiguity found that affects test expectations

---

## Agent Spawning

See `docs/task-cards.md` for the task card protocol. The orchestrator (Build agent) constructs task cards and invokes subagents via the Task tool.

| Agent | When to spawn | Primary docs to include in task card |
|-------|---------------|--------------------------------------|
| `@spec-analyst` | Phase 1a | `docs/agents/spec-analyst.md`, `docs/skills/bug-hunting.md`, `docs/designs/<name>.md` |
| `@formal-writer` | Phase 1b | `docs/agents/formal-writer.md`, `docs/skills/formal-flow.md`, `docs/designs/<name>.md` |
| `@probe-creator` | Phase 1c | `docs/agents/probe-creator.md`, `docs/designs/<name>.md` |
| `@rtl-fixer` | Phase 2 | `docs/agents/rtl-fixer.md`, `docs/designs/<name>.md` |
| `@test-writer` | Phase 3a | `docs/agents/test-writer.md`, `docs/skills/cocotb-patterns.md`, `docs/designs/<name>.md` |
| `@assertion-writer` | Phase 3b | `docs/agents/assertion-writer.md`, `docs/skills/sva-patterns.md`, `docs/designs/<name>.md` |
| `@flow-runner` | Phase 4a, 4b, 4c | `docs/agents/flow-runner.md`, `docs/skills/flow-commands.md` |
| `@ppa-optimizer` | Phase 4b (after C=1.0) | `docs/agents/ppa-optimizer.md`, `docs/skills/sky130hd-optimization.md`, `docs/designs/<name>.md`, `docs/flow-ppa-and-v.md` |
| `@mutant-hunter` | Phase 4c (loop) | `docs/agents/mutant-hunter.md`, `docs/skills/mutation-strategies.md`, `docs/designs/<name>.md` |

---

## Submission Package Format

Final submission must be a `.tar.gz`:
```
rtl/          fixed RTL (same filelist, same top-level ports)
tests/        cocotb tests: test_*.py + helper modules
assertions/   SVA files with bind statements (*.sv or *.v, NOT *.svh)
report.md     one page: deviations, changes, verification (see docs/report-template.md)
```

Package command:
```bash
tar czf runs/<handle>-<design>-<n>.tar.gz -C <submission-dir> rtl tests assertions report.md
```

---

## References

- `docs/design-init.md` — bootstrap checklist for a new design (steps 1-7 before Phase 1)
- `docs/flow-lessons.md` — 15 measured pitfalls
- `docs/flow-ppa-and-v.md` — Phase 4b PPA + Phase 4c V detail
- `docs/joint-flow.md` — operator+agent cycle
- `docs/competition-strategy.md` — scoring, timing, tier budgets
- `docs/ppa-freedom-strategy.md` — PPA freedom audit
- `docs/running-the-flow.md` — Docker commands
- `docs/task-cards.md` — agent spawning protocol
- `docs/designs/` — per-design docs (design, REQ matrix, PPA plan)
- `docs/state/` — per-design progress (where we left off)
- `SUBMISSION.md`, `flow-image/SCORING.md`, `flow-image/README.md` — competition rules
- `challenge-N/spec.md`, `challenge-N/change_request.md` — the design source of truth
