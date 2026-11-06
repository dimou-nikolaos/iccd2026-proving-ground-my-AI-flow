# Task Card Protocol

Task cards are how the orchestrator (Build agent) communicates with subagents. Each subagent invocation includes a task card as the prompt string. The agent's base behavior is defined by its `.opencode/agents/<name>.md` (identity and constraints). The task card provides **per-invocation specifics**.

## Task Card Template

```
DESIGN: challenge-1
PHASE: <phase-name>
GOAL: <one-sentence objective>

INPUTS (read these before starting):
- <path to spec.md>
- <path to rtl/ directory>
- <docs/ paths the agent should read>
- <docs/designs/<name>.md> (if design-specific knowledge needed)
- <any output from previous phases>

DELIVERABLES:
- <exact files to create/modify>
- <quality gates: what must pass>

FOCUS:
- <key things the agent must prioritize>
- <known tricky areas>
- <what NOT to do>

EXIT CRITERIA:
- <condition for success>
- <maximum iterations before stopping>

ESCALATE (stop and report back) if:
- <specific conditions where agent should not proceed alone>

FEEDBACK FORMAT (return this structured block when done):
- STATUS: done | partial | blocked
- ARTIFACTS: <list of files created/modified>
- METRICS: <phase-specific numbers>
- NOTES: <observations, ambiguities found, suggestions for skill docs>
- UNRESOLVED: <items needing human attention>
```

## How to Construct Task Cards (orchestrator instructions)

### First invocation of a phase (full card)
Include everything. The agent is starting fresh and has no context beyond its own `.opencode/agents/` definition.

### Iteration/retry (delta card)
Only include what changed:
```
DESIGN: challenge-1
PHASE: iterate-mutation (round 2)
GOAL: Kill these 11 remaining mutants.

NEW INPUTS:
- Surviving mutants: rtl/<file>.sv:84 (constant), rtl/<file>.sv:23 (relational), ...

PREVIOUS OUTPUT: (unchanged — your tests/assertions from round 1 are still in place)

DELIVERABLES:
- Add targeted assertions or tests for the listed survivors only

EXIT: V >= 0.90 or no improvement after trying all 11.
```

### Parallel invocation (when possible)
Phase 3a (test-writer) and Phase 3b (assertion-writer) can run in parallel — they produce different files and don't conflict. Phase 1b and 1c can run in parallel.

## Feedback Block Reference

The orchestrator parses the feedback block to decide next steps. Standard fields:

| Field | Orchestrator uses it to |
|-------|------------------------|
| `STATUS` | Decide whether phase is done, retry, or escalate |
| `ARTIFACTS` | Track what's in the submission |
| `METRICS` | Quality gates (test count, deviation count, V score, etc.) |
| `NOTES` | Refine skill docs and task cards for future sessions |
| `UNRESOLVED` | Surface to user for human judgment |

## Per-Phase Metrics Reference

| Phase | Metrics the agent returns |
|-------|--------------------------|
| Analyze | `DEVIATIONS_FOUND: <count>`, `CONFIDENCE: <HIGH/MEDIUM per finding>`, `REQ_COVERAGE: <checked/total>` |
| Fix | `FILES_MODIFIED: <list>`, `LINT_CLEAN: yes/no`, `ELABORATION_CLEAN: yes/no`, `PUBLIC_TB: pass/fail` |
| Verify | `TESTS_TOTAL: <n>`, `TESTS_FAIL_ON_BASELINE: <n>`, `ASSERTION_MODULES: <n>`, `ASSERTION_PROPERTIES: <n>` |
| Iterate | `V_SCORE: <0-1>`, `SURVIVORS_REMAINING: <count>`, `KILLS_ADDED: <n>`, `ITERATIONS: <n>` |

## Example: Complete Task Card

```
DESIGN: challenge-1
PHASE: verify-functional
GOAL: Write the full cocotb test suite with Python reference model.

INPUTS (read these before starting):
- challenge-1/spec.md (the specification — model comes from THIS, not RTL)
- challenge-1/change_request.md (CR-1: <short feature description>)
- challenge-1/rtl/ (fixed RTL — for port names and internal signal names only)
- docs/agents/test-writer.md (your methodology)
- docs/skills/cocotb-patterns.md (model architecture patterns)
- docs/designs/<name>.md (design facts: hierarchy, signals, protocol)

DELIVERABLES:
- tests/test_submission.py (all cocotb test functions)
- tests/<design>_tb.py (reference model, driver class, helpers)
- Must pass on the fixed rtl/ (in challenge-1/rtl/)
- At least 3 tests must FAIL on baseline RTL (proving deviations detected)

FOCUS:
- Event-driven model (this is a datapath: output per transaction/frame, not cycle-by-cycle)
- Cover: all functional REQs, and the CR
- Boundary: max/overflow counter values, empty transfers, invalid-strobe cases, config scramble
- Randomized: 20+ configs per test, seeded RNG, backpressure injection
- Name tests after requirements: test_req05_<behavior>, test_cr1_<feature>

EXIT CRITERIA:
- All tests pass on fixed RTL
- At least 3 tests fail on baseline RTL
- Total runtime < 120s (for the mutation timeout formula to stay reasonable)

ESCALATE if:
- You disagree with a deviation fix in the RTL
- The spec is ambiguous for a modeled behavior (state the ambiguity)
- A test fails on fixed RTL that should pass (potential fix bug)

FEEDBACK FORMAT:
STATUS: done | partial | blocked
ARTIFACTS: tests/test_submission.py, tests/<design>_tb.py
METRICS: TESTS_TOTAL=9, TESTS_FAIL_ON_BASELINE=3, DEVIATIONS_DETECTED=[REQ-xx, REQ-yy, REQ-zz]
NOTES: <observations>
UNRESOLVED: <if any>
```

## Writing Task Cards: Principles

1. **Name exact file paths** — agents don't guess, they read what you tell them
2. **State quality gates as boolean conditions** — "test X fails on baseline" not "test X should detect bugs"
3. **Include the output of the previous phase** — findings table, mutant list, surviving files
4. **Specify what NOT to do** — "do not modify RTL", "do not use RTL internals in model"
5. **Keep feedback structured** — the orchestrator parses it programmatically to decide next steps
6. **Escalation conditions are specific** — not "if stuck" but "if after 3 attempts the same assertion still fires on fixed RTL"
