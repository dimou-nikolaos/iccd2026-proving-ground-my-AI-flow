# Joint Flow — Operator ↔ Agent

Single source of truth for how we work together during the competition window (Nov 7–16 2026). Both operator and agent reference this file. If we disagree on what to do, this file wins. Update this file when we change process.

## File conventions

| File | Owner | Purpose |
|------|-------|---------|
| `docs/joint-flow.md` | joint | This file — the process itself |
| `docs/joint-strategy.md` | joint | High-level strategy (priority order, score targets, heuristics) |
| `docs/designs/<name>.md` | agent | Design knowledge: hierarchy, signal map, deviations found, V ceiling |
| `docs/state/<name>-progress.md` | agent | Running score table, phase status, current best sub |
| `docs/sessions/<name>-session.md` | agent | Retrospective after design closed |
| `runs/sub.tar.gz` | agent | Current best submission (the one to upload) |
| `runs/sub-best.tar.gz` | agent | Highest-score submission so far (fallback) |
| `runs/sub-iter-<N>.tar.gz` | agent | Per-submission archive (for submission N you uploaded) |

## Directory gating (design-specific knowledge isolation)

**Rule**: when the working dir is `<X>/` (e.g., `challenge-1/`, `challenge-2/`), only load design-specific files for `<X>`:
- `docs/designs/<X>.md` — read at start, update during Phase 1+
- `docs/state/<X>-progress.md` — running log
- `docs/sessions/<X>-session.md` — only if a prior session for this design exists (resumption case)

**Never cross-load**. Don't read `docs/designs/<other>.md` when working on a different design. Patterns are extracted to general skills/agents where they apply to all designs. The design doc name matches the working dir name (`challenge-1/` → `docs/designs/challenge-1.md`).

**What's always loaded (general)**:
- `docs/joint-flow.md`, `docs/joint-strategy.md`
- `docs/competition-strategy.md`
- `.opencode/skills/*` and `.opencode/agents/*`
- `docs/skills/*` and `docs/agents/*` (methodology)
- Tech-specific (all designs use same PDK): `.opencode/skills/sky130hd-ppa/SKILL.md` for P iteration; deep ref `docs/skills/sky130hd-optimization.md`

**Why**: each design has its own signal names, deviation patterns, and V ceiling. Cross-contamination leads me to assume signal names or PPA opportunities that don't exist in the new design.

## The cycle (one round per design)

```
┌──────────────────────────────────────────────────────────────┐
│ T0  Operator: "go on <design-name>" (after copying dir)      │
└──────────────────────────────────────────────────────────────┘
                  ↓
┌──────────────────────────────────────────────────────────────┐
│ T1  Agent autonomous block (no operator input needed)        │
│     - Read spec.md, change_request.md, tb/public/, rtl/      │
│     - Phase 1: find 3+ deviations (HIGH confidence preferred)│
│     - Phase 2: fix RTL + implement CR + run public TB        │
│     - Write docs/designs/<name>.md and state/<name>-progress.md
│     - **FIRST DROP**: package runs/sub.tar.gz with rtl+report only
│     - Run --skip ppa flow locally to sanity-check            │
│     - STOP. Hand off to operator.                            │
└──────────────────────────────────────────────────────────────┘
                  ↓
┌──────────────────────────────────────────────────────────────┐
│ T2  Operator (parallel — happens while agent starts Phase 3) │
│     - Upload runs/sub.tar.gz to competition portal           │
│     - Read spec.md + change_request.md fully                 │
│     - Wait for C result (~15min)                             │
└──────────────────────────────────────────────────────────────┘
                  ↓
┌──────────────────────────────────────────────────────────────┐
│ T3  Agent autonomous block (continues during operator read)  │
│     - Phase 3: golden model + cocotb tests + SVA assertions  │
│     - Run flow with tests+assert, --skip ppa → measure V     │
│     - Run full flow → measure P                              │
│     - Update state file                                      │
└──────────────────────────────────────────────────────────────┘
                  ↓
┌──────────────────────────────────────────────────────────────┐
│ T4  Operator interrupt (after reading + C result in hand)    │
│     Reports: C value, failed hidden test names, PPA stages,  │
│              any spec ambiguities noticed                    │
│     Agent incorporates and resumes                           │
└──────────────────────────────────────────────────────────────┘
                  ↓
┌──────────────────────────────────────────────────────────────┐
│ T5  Agent iteration (one cycle per 30min submission window)  │
│     Priority: C=1.0 → P>0.5 → V ceiling                      │
│     Each cycle:                                              │
│       1. Apply ONE change (RTL fix, test, assertion, PPA opt)│
│       2. Test locally with --skip ppa (fast)                 │
│       3. If V/PPA improved locally → repackage sub.tar.gz    │
│       4. Signal "ready for submission N" to operator         │
│     Operator uploads, reports back, cycle repeats            │
└──────────────────────────────────────────────────────────────┘
                  ↓
                  (loop T4↔T5 until operator says design is done
                   or 30 submissions exhausted)
                  ↓
┌──────────────────────────────────────────────────────────────┐
│ T6  Agent autonomous block — DESIGN CLOSE (no approval needed)│
│     1. Write docs/sessions/<name>-session.md (template:       │
│        Outcome / Went well / Hard / Skill improvements /      │
│        Patterns discovered / Surviving mutants)               │
│     2. EXTRACT general patterns from session log into:        │
│        - .opencode/skills/<relevant-skill>/SKILL.md           │
│        - docs/agents/<agent>.md (methodology)                │
│        - docs/skills/<topic>.md (deep reference)             │
│     3. DO NOT put design-specific info in skills/agents.      │
│        Signal names, addresses, deviations → design.md only.  │
│     4. Keep new general patterns CONCISE (1-3 lines each).    │
│     5. Update `runs/sub-best.tar.gz` to final best.           │
│     6. Operator does not approve this step — agent does it    │
│        automatically before starting next design.             │
└──────────────────────────────────────────────────────────────┘
```

## Handoff protocol

### When agent finishes autonomous block (T1 or T3)
Agent writes to `docs/state/<name>-progress.md` under "Next action for operator":
- What's ready (sub.tar.gz path + iteration count)
- What scores I observed locally (V, P if available)
- Recommended next operator action (upload, interrupt, read X)

### When operator interrupts (T4 or after submission N result)
Operator pastes the flow's report into chat:
```
C = 0.95
V = 0.925
P = 0.5
Failed hidden tests: test_<name>_drain, test_<name>_width
Survivors: rtl/<file>.sv:84, rtl/<file>.sv:45, ...
PPA: synth area 28500, route WNS 1.2, ...
```

Or for spec feedback:
```
You missed REQ-NN: the spec says config can change between frames,
not just at frame start. The CR explicitly mentions this case.
```

Agent updates the state file's "Operator feedback received" log section.

### When operator says "design done"
Agent writes `docs/sessions/<name>-session.md` retrospective.

## Resumption protocol (if session restarts)

If a new agent session starts (context overflow, restart, etc.):
1. Agent reads `docs/state/<name>-progress.md` — this tells phase status, score history, last action
2. Agent reads `docs/joint-flow.md` — refreshes process
3. Agent reads `docs/joint-strategy.md` — refreshes strategy
4. Agent reads `docs/designs/<name>.md` — design knowledge (gated by current dir name)
5. General lessons already live in `.opencode/skills/` and `docs/agents/` (always loaded); also check `docs/sessions/*-session.md` for any prior design retrospectives
6. Continue from "Next action for operator" entry

## Continuous logging (write incrementally, don't wait for end)

Update `docs/state/<name>-progress.md` **after every meaningful step**, not just at phase boundaries. A "meaningful step" is:
- Phase transition (analysis → fix → verify → iterate)
- Each flow run (record the score row + 1-line note)
- Each new deviation found
- Each new PPA optimization attempted
- Each operator feedback received
- Each "next action for operator" change

This file is the agent's memory across session restarts. Treat it as the authoritative state. If I lose context, the next session reads this file and resumes seamlessly.

**Format**: append-only style. Don't rewrite history; add new sections. Keep entries 1-3 lines each.

**What NOT to log here**: design-specific implementation details (those go in `docs/designs/<name>.md`), general lessons (those go in the session log at the end).

## State file template (each design)

Use `docs/state/TEMPLATE.md` as the starting structure (copy it to `docs/state/<name>-progress.md` at Phase 1 start). It has: Phase status table, Score history, Deviations found, CR notes, V ceiling, PPA current + opts applied, **Activity log (append-only)**, Operator feedback log, **Next action for operator**, Best submission.

The two sections that must stay current for seamless session restart: **Activity log** and **Next action for operator**.

## Submission packaging rule

NEVER include build artifacts:
```bash
rm -rf <design>/tests/sim_build <design>/tests/results.xml <design>/tests/__pycache__
tar czf runs/sub-iter-N.tar.gz -C <design> rtl tests assertions report.md
```

## Sub-count budget rule

30 submissions per design. Reserve at least 5 for end-of-window safety.
Suggested allocation:
- Subs 1-3: Phase 1-2 (C feedback)
- Subs 4-10: V iteration
- Subs 11-20: PPA Phase B (safe opt)
- Subs 21-25: PPA Phase C (aggressive) + Explorer variants
- Subs 26-30: reserve / final tweaks

If we burn through subs fast, slow down — don't hit 30 with P still at 0.5.

## Communication conventions

- When I (agent) start an autonomous block, I say: **"Working — [next thing]"**
- When I finish a deliverable: **"DROP READY: runs/sub.tar.gz. [scores]. Awaiting upload."**
- When I need input: **"BLOCKED: <reason>. Need <specific thing>."**
- Operator to me: paste reports, "go", "stop", "design done", or feedback.

## Things we DON'T do

- Don't run verilator/yosys/sby on host (always Docker)
- Don't use `--cpus 4` (use 12 — see `running-the-flow.md`)
- Don't chase equivalent mutants (document V ceiling, stop)
- Don't include build artifacts in tar
- Don't add comments to RTL/tests/assertions (operator preference)
- Don't over-abstract. Smallest correct change wins for fixes; biggest area reduction wins for PPA.
- Don't ask permission before writing session log + extracting patterns at design close (T6). It's automatic.
- Don't cross-load design knowledge (read `docs/designs/<X>.md` only for the current design dir). Gate by dir name.
- Don't write design-specific signal names in skills/agents. Those go to `docs/designs/<name>.md`.
- Don't bloat skills/agents. General patterns only. Concise (1-3 lines each).

## Where knowledge lives (canonical)

| Type of knowledge | Location |
|-------------------|----------|
| Process / who-does-what | `docs/joint-flow.md` (this file) |
| High-level strategy | `docs/joint-strategy.md`, `docs/competition-strategy.md` |
| General patterns (cross-design) | `.opencode/skills/*/SKILL.md`, `docs/agents/*.md`, `docs/skills/*.md` |
| Design-specific knowledge (signals, addresses, deviations, V ceiling) | `docs/designs/<name>.md` ONLY |
| Running state (current iteration) | `docs/state/<name>-progress.md` |
| End-of-design retrospective | `docs/sessions/<name>-session.md` |
