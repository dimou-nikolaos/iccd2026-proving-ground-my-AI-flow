# Proving Ground Verification Workflow

This is the master workflow. Read it once at session start. It tells you what to do, in what order, and where to find the detailed instructions for each step.

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
- **Known bug patterns to scan for**:
  - Off-by-one at boundary comparisons
  - Missing register latch (config passes through combinational)
  - Missing reset branch in `always_ff`
  - Extra disjunct in boolean expression
  - Wrong operation ordering in combinational chain
  - Missing qualifier (strobe mask, enable gating)

### 1b. Formal-assisted (confirms HIGH + resolves MEDIUM)
- **Agent**: `@formal-writer`
- **Skill**: load `formal-verification-flow` for sby setup steps
- **Deep reference**: `docs/skills/formal-flow.md`
- **Method**: Write SVA properties derived from spec. Run SymbiYosys against baseline RTL. CEX = deviation confirmed.
- **Output**: per-REQ proof/CEX status

### 1c. Probe simulation (confirms remaining MEDIUM/LOW)
- **Agent**: `@probe-creator`
- **Method**: Write a minimal cocotb test (5-20 lines) that exercises the suspected deviation. Run against baseline. Fail = confirmed.
- **Output**: probe test code + pass/fail result

**Phase 1 exit criterion**: Confirmed deviation table (3 deviations per design).

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

### 3a. Tests
- **Agent**: `@test-writer`
- **Skill**: load `cocotb-test-writing` for model and driver templates
- **Deep reference**: `docs/skills/cocotb-patterns.md`
- **Design doc**: read `docs/designs/<name>.md`

**Golden model strategy**:
- **Control designs** (register interfaces, FSMs): **cycle-accurate** Python model. Runs in lockstep with DUT. Checks every cycle. (timer/register-map style: `TimerTB._cycle()`)
- **Datapath designs** (streaming, transformation): **event-driven** Python model. Computes expected output per transaction. Timing abstracted by handshake drivers. (streaming style: `DesignTB.send_beats()` + `wait_results()`)

**Test taxonomy** (write tests in each category):
1. **Deviation tests**: one per confirmed deviation. Must FAIL on baseline.
2. **Change-request tests**: exercise new feature with known acceptance values.
3. **Boundary tests**: saturation edges, empty/zero, boundary comparisons.
4. **Protocol tests**: handshake, backpressure, reset mid-operation.
5. **Constrained-random tests**: random configs within spec constraints, random data, multiple seeds.
6. **Register-map tests** (if applicable): full address space, PSLVERR, byte strobes, W1C.

**Naming**: `test_req<NN>_<description>` or `test_cr<N>_<description>`

### 3b. Assertions
- **Agent**: `@assertion-writer`
- **Skill**: load `sva-assertion-writing` for bind templates and assertion categories
- **Deep reference**: `docs/skills/sva-patterns.md`
- **Design doc**: read `docs/designs/<name>.md` for signal names and hierarchy

**Assertion strategy** (by priority for mutation kills):
1. **Signal-definition assertions** (highest kill value): `assert property (output == <spec formula>)`. Kills any mutation to the RHS expression. (These mirror the RTL exactly → cannot fire on correct RTL.)
2. **Next-state assertions** (kill off_by_one / relational mutations) — use the **one-cycle-back identity**, not `|=>` with mixed cycles. A reg sampled at posedge T was written at T-1 from signals sampled at T-1, so: `q == ($past(en) ? f($past(d)) : $past(q))` and `cfg_q.f == $past(cfg_f)`. The classic self-fire bug is pairing `$past(event)` with the **current** clear/config (mixed cycles) — it fires right after a clear or a config change. Always `$past()` the enable/clear/config too.
3. **Protocol invariants**: handshake rules, output stability, ready/valid mutual rules.
4. **Reset assertions** (no `disable iff`): checks registers are 0 after reset. Kills "missing reset" mutations.
5. **Boundary/range assertions**: value ranges, saturation behavior.

**Critical rule**: assertions must NEVER fire on the correct fixed RTL. If they fire on your own submission, the submission is REJECTED. Subagents routinely emit off-by-one next-state asserts — the **orchestrator must run the tests with `ASSERTIONS=1` on a fresh `sim_build` and confirm 0 fires before packaging**, even when the subagent reports success.

**Phase 3 exit criterion**: All tests pass on fixed RTL, at least 3 tests fail on baseline, no assertion fires on fixed RTL.

---

## Phase 4: Iterate (C → P → V)

**Goal**: Maximize score = 100 × C × (0.5V + 0.5P). Priority: **C=1.0 first, P second, V third.**

### 4a. Submit early, get C feedback

After Phase 2 fixes (even before tests/assertions are written), submit the RTL alone. The flow returns:
- **C**: pass/fail names of hidden regression tests and hidden properties
- **V**: with only public tests (baseline mutant kill rate)
- **P**: PPA vs baseline (area, timing, power at each stage)

**C is server-only.** The local (Docker) flow returns `C = null` — hidden checks live only on the server. Locally you can only measure V and P and reason about C from the spec. So: bank C early with an rtl+report drop, and treat the server report as the ONLY ground truth for C. Do not assume local green ⇒ C=1.0.

**If C < 1.0** (highest priority — multiplies everything):
- Read the FAILED TEST NAMES (names reveal which REQ/behavior is wrong)
- Diagnose: missed deviation? Wrong CR interpretation? Edge case?
- Fix RTL, resubmit (30min cooldown)
- Repeat until C=1.0

**If C = 1.0**: proceed to P and V optimization.

### 4b. PPA optimization (P ≥ 0.5 minimum, > 0.5 for high scores)
- **Agent**: `@ppa-optimizer` (runs after C=1.0 locked)
- **Skill**: load `sky130hd-ppa` for the ranked checklist
- **Deep reference**: `docs/skills/sky130hd-optimization.md`

**Critical insight**: P=0.5 = "same as baseline." To score >80 overall, P must exceed 0.5. Your fixed RTL removes bug-created logic, so P>0.5 is achievable.

**Read PPA feedback** at each stage (synthesis → placement → CTS → routing): compare area (added flops/logic → remove equivalent-or-more elsewhere), WNS (timing regression → find the critical path in the report), power (correlates with area here). **Power is often the biggest P term** (weight 0.2 but can swing ±30%): the #1 generic reclaim is to **compute config-derived shifts/masks (`v<<(32-W)`, `v&mask(W)`) ONCE when the config is latched, and read the pre-processed register in the hot datapath** — an always-recomputed shifter/mux toggles every cycle and lengthens the path. In challenge-1 this flipped P from 0.45 (a required config latch added flops) to 0.54 (power −34% vs baseline).

**Techniques**: full sky130hd-specific list in the `sky130hd-ppa` skill (deep ref `docs/skills/sky130hd-optimization.md`). High-level: free area from fixes, precompute config shifts/masks at load, narrow masked registers, `>=`→`==` where safe, balanced mux trees, homogeneous resets, strip async reset from pure data-path memory flops, register replication for fanout, operand isolation, then aggressive restructuring for the Explorer award. **Iteration economy** (`docs/competition-strategy.md` → "PPA iteration economy"): pre-screen with `synth` cells + WNS + `tests@ASSERTIONS=1`, then **bundle** provably-free/independent changes into one submission and **isolate only** the timing-`t`/C-critical ones (P is not additive; a bundle can tip WNS into a violation and cap P). Keep a change only if it measurably improves P while C stays 1.0.

**Stacked PPA recipe for control+bus designs** (validated challenge-2, P 0.48→0.4965): apply in this order because **timing slack is the currency** — every area/power win must fit inside slack established by prior timing wins:
1. **ICG-friendly 3-branch flop** on every counter with distinct load/step/hold conditions (`if (load) q<=init; else if (step) q<=next; else hold;`). Buys WNS slack (+0.3–0.5 ns observed).
2. **Simplify hot comparators** using design invariants (e.g., `count >= period` → `count == period` when `count <= period` always holds).
3. **AND-OR read mux with one-hot decode** (replace `unique case` with `rd = (dec[0]&v0) | ...`). Big area win but WNS regression — must be bundled with (2) to recover timing.
4. **Operand isolation on idle adders** (`in_iso = en ? in : 0; sum = in_iso + 1`) when adder is enabled <50% of cycles.

Do **NOT** skip to step 3 without steps 1–2 first — the WNS cliff collapses P. Never attempt narrow-register rewrites or module fusion without prior timing slack. P ceiling on control+bus designs is ~0.50 (parity), NOT 0.48 as previously believed.

**Keep discipline (measured):** any RTL edit changes the archive hash and re-rolls the mutation seed, so a **P-neutral change is pure downside** — it can only move V (often down when the new seed draws an equivalent mutant you can't kill), never up. Yosys also often already collapses "obvious" savings (unused always-0 bits), so a tidy-up edit can be exactly P-neutral. **Only promote a change that measurably improves P (area/power/timing); otherwise revert so the working tree matches sub-best.**

Run full flow (with PPA) via `pg-flow-commands` skill — the baseline PPA is cached **by the `--design` dir's RTL** in `<cache>/baseline_ppa.json`. If you edit `rtl/` in place you MUST point `--design` at a **pristine baseline mirror** (released `rtl/` + `design.yaml` + `tb/` + `flow/`) with a matching cache dir, or P is fixed-vs-fixed (≈0.5, meaningless). First full run after an RTL change recomputes only the submission PPA (~longer, >2 min — set a large command timeout); the baseline PPA is reused. The OpenROAD detailed router can nondeterministically crash at 0 DRC (`ODB-0269`) — just re-run.

### 4c. Mutation kill (V maximize)
- **Agent**: `@flow-runner` (fast run with `--skip ppa`)
- **Skill**: load `mutation-kill-tactics` for per-operator kill recipes
- **Deep reference**: `docs/skills/mutation-strategies.md`
- **Method**: For each surviving mutant at file:line:
  1. Read the RTL at that line
  2. Determine what mutation was applied (which operator class)
  3. Check: semantically equivalent? (see `mutation-kill-tactics` table). If YES: skip.
  4. Write targeted assertion or test that kills it
  5. Re-run flow (fast, `--skip ppa`)
- **Repeat** until V reaches ceiling (design V ceiling documented in `docs/designs/<name>.md`)

**V is archive-specific and deterministic.** The mutation seed is `f(submission_hash)` — the SAME tar yields the SAME mutant set + survivors every run (no averaging). But **any archive change re-rolls the seed** (RTL, tests, assertions, even comments) → a DIFFERENT 40-mutant set. So (a) never target specific line numbers (they shift); write behavior-based evidence; (b) **re-measure V after every archive change** (a PPA refactor can move or drop mutants). Two common **unkillable (equivalent)** mutant classes — document as V ceiling, don't chase: (i) a write to a register whose value is overwritten before any read (dead write); (ii) an operand guarded by a design invariant that makes the mutated sub-expression unreachable (e.g. `x && !full` where the producer never asserts `x` when `full`).

**Practical V ceiling across 20 challenge-2 archives**: V=0.975 typical, V=1.0 on ~3–4 lucky draws (15–20% hit rate), V=0.925–0.95 on unlucky draws. Best strategy: **after each PPA improvement, submit once and observe the seed**. Keep the highest-projective-score archive. Do NOT assume that more archive changes always improve V.

**Phase 4 exit criterion**: C = 1.0, P ≥ 0.50, V at design ceiling.

### 4d. Rapid iteration loop (competition mode)

With 30min cooldown + 15min flow time, use the wait productively:

```
Submit → while waiting 30min:
  - Work on PPA optimization (next RTL variant)
  - Write more assertions (next V improvement)
  - Analyze PPA report from previous submission
→ Submit next variant (improved P or V)
```

**Always keep `runs/sub-best.tar.gz`** — never overwrite a good submission with a worse one.

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

Quick reference:

| Agent | When to spawn | Primary docs to include in task card |
|-------|---------------|--------------------------------------|
| `@spec-analyst` | Phase 1a | `docs/agents/spec-analyst.md`, `docs/skills/bug-hunting.md`, `docs/designs/<name>.md` |
| `@formal-writer` | Phase 1b | `docs/agents/formal-writer.md`, `docs/skills/formal-flow.md`, `docs/designs/<name>.md` |
| `@probe-creator` | Phase 1c | `docs/agents/probe-creator.md`, `docs/designs/<name>.md` |
| `@rtl-fixer` | Phase 2 | `docs/agents/rtl-fixer.md`, `docs/designs/<name>.md` |
| `@test-writer` | Phase 3a | `docs/agents/test-writer.md`, `docs/skills/cocotb-patterns.md`, `docs/designs/<name>.md` |
| `@assertion-writer` | Phase 3b | `docs/agents/assertion-writer.md`, `docs/skills/sva-patterns.md`, `docs/designs/<name>.md` |
| `@flow-runner` | Phase 4a, 4b, 4c | `docs/agents/flow-runner.md`, `docs/skills/flow-commands.md` |
| `@ppa-optimizer` | Phase 4b (after C=1.0) | `docs/agents/ppa-optimizer.md`, `docs/skills/sky130hd-optimization.md`, `docs/designs/<name>.md` |
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

## References to existing repo docs

- `SUBMISSION.md`: submission rules, what backend checks, scored-window limits
- `flow-image/SCORING.md`: exact formulas for C, V, P
- `flow-image/README.md`: flow image contents, assertion SVA subset table, build args
- `challenge-N/spec.md`: the specification (source of truth for REQ-xx)
- `challenge-N/change_request.md`: the required change
- `docs/report-template.md`: report.md format template
