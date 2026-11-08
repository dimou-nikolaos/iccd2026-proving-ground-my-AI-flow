# Flow documentation audit (Nov 8, 2026 — between Phase A formal hunt and Phase B wildcard batch)

Trigger: after ~16h of work on challenge-1, the doc tree has grown. An agent entering cold (or post-compaction) reads ~8-12 files before taking action. Goal of this audit: identify which docs are load-bearing for navigation, which are bloated, and what splits/cross-references would make per-phase entry cheap.

## Current doc tree (sizes)

| File | Lines | Role | Issue |
|------|------:|------|-------|
| `docs/state/challenge-1-exploration-plan.md` | 421 | 12h exploration plan, **DONE** | Stale; should archive |
| `docs/skills/cocotb-patterns.md` | 281 | Deep-ref patterns | OK |
| `docs/flow.md` | 263 | 4-phase skeleton | OK |
| `docs/skills/mutation-strategies.md` | 252 | Deep-ref mutation tactics | OK |
| `docs/skills/sva-patterns.md` | 232 | Deep-ref SVA patterns | OK |
| `docs/joint-flow.md` | 231 | Operator↔Agent process | OK |
| `docs/state/challenge-1-progress.md` | 209 | Session state | Long but live |
| `docs/skills/flow-commands.md` | 207 | Deep-ref commands | OK |
| `docs/skills/formal-flow.md` | 192 | Deep-ref SBY | OK |
| `docs/skills/sky130hd-optimization.md` | 190 | Deep-ref PPA | OK |
| `docs/task-cards.md` | 187 | Sub-agent prompts | OK |
| `docs/state/challenge-1-longhorizon-plan.md` | 179 | 10-day plan (live) | OK |
| `docs/flow-lessons.md` | 134 | 28 lessons | **Poorly sub-sectioned** |
| `docs/flow-ppa-and-v.md` | 128 | Phase 4 detail | OK |
| `.opencode/skills/*/SKILL.md` | 83-168 each | Operational skill files | OK |

## Findings (priority order)

### F1 — Stale plan files clutter state dir
`docs/state/challenge-1-exploration-plan.md` (421 lines) was the 12h exploration plan completed yesterday. It's marked DONE in its tracker table but still appears in `ls docs/state/`, mixing active and dead state. **Action**: move to `docs/archive/`.

### F2 — flow-lessons.md misplaced lessons
- L13 (SBY formal ROI) under "Tools" — OK
- L24/L25/L26 (SBY gotchas, bind-port-direction, sequential SBY fragility) under "Phase 4-P" — WRONG. They are Phase 3 / formal-flow tooling. A Phase 4 P-optimizer reading flow-lessons doesn't need them; a Phase 3 formal-writer would never find them.
- L22 (seed-luck, Explorer variants) under "Phase 4-P" — half wrong. Seed reroll is V-phase (mutate V), not P-phase.
- L23 (semantic-safety audit for Explorer variants) — same: V / Explorer topic.

**Action**: restructure flow-lessons.md with 7 fine-grained sub-sections:
- Phase 1 — analysis
- Phase 3 — verify (testing/assertions)
- Phase 3 — formal SBY tooling (L13, L24, L25, L26)
- Phase 4 — iterate C (correctness risk)
- Phase 4 — iterate P (PPA strategy + wildcards)
- Phase 4 — iterate V (mutation kill + seed hedging) (L22, L23 move here)
- Tooling environment / cross-cutting

### F3 — AGENTS.md "Start here" is flat list with no phase routing
A Phase 4 agent reading AGENTS.md sees 8 entries and must scan all to know which apply. Should be:

```
### What to read for what
| You are at | Read first | Then |
|---|---|---|
| Cold start / resume | docs/joint-flow.md, docs/state/<design>-progress.md | docs/flow.md |
| Phase 1 (analyze) | docs/flow.md § Phase 1 | .opencode/skills/spec-deviation-hunting/ |
| Phase 2 (fix) | docs/flow.md § Phase 2 | task-cards rtl-fixer |
| Phase 3 (verify) | docs/flow.md § Phase 3 | .opencode/skills/cocotb-test-writing/, .opencode/skills/sva-assertion-writing/ |
| Phase 4 PPA | docs/flow-ppa-and-v.md, docs/ppa-freedom-strategy.md | .opencode/skills/sky130hd-ppa/ |
| Phase 4 wildcards | docs/flow-wildcard.md | .opencode/skills/sky130hd-ppa/ |
| Phase 4 mutation kill | docs/flow-ppa-and-v.md § Mutation | .opencode/skills/mutation-kill-tactics/ |
```

### F4 — docs/skills/ vs .opencode/skills/ duplication is unclear
Five pairs exist:
| `docs/skills/<X>.md` (deep ref) | `.opencode/skills/<Y>/SKILL.md` (operational) |
|---|---|
| cocotb-patterns | cocotb-test-writing |
| flow-commands | pg-flow-commands |
| formal-flow | formal-verification-flow |
| mutation-strategies | mutation-kill-tactics |
| sky130hd-optimization | sky130hd-ppa |

Different names. Some cross-refs in docs use the deep-ref name, some use the skill name. Confusing.

**Action**: standardize. Deep-ref name = `<topic>-patterns.md` or `<topic>-reference.md`; skill name = short verb form. Already mostly true. Document the convention in docs/joint-flow.md "Where knowledge lives" section.

### F5 — Phase 4 split across 4 files (no single entry)
- `docs/flow.md` § Phase 4 — overview, Tier A/B/C definitions
- `docs/flow-ppa-and-v.md` — P iteration + V iteration detail
- `docs/flow-wildcard.md` — Phase 4e wildcards
- `docs/ppa-freedom-strategy.md` — spec-freedom audit

Reasonable split, but no single doc says "here's how to navigate Phase 4". Add a "Phase 4 entry map" at top of `docs/flow.md` § Phase 4, OR add to AGENTS.md's new "What to read for what" table.

### F6 — longhorizon-plan.md is the new session-state pattern (good — generalize)
The new `docs/state/challenge-1-longhorizon-plan.md` has the right structure for cold-resume:
1. Resume recipe at top (md5 check, tracker table pointer)
2. Tracker table of phases with status + findings
3. Phase-by-phase sections with details
4. "Next action for the agent reading this" at bottom
5. Append-only activity log

**Action**: update `docs/state/TEMPLATE.md` to include this pattern. Future designs get this template on bootstrap.

### F7 — challenge-1-progress.md is too long for a status file (209 lines)
Mixes: score history, current state, activity log (verbose), decision history. The score history table is the load-bearing part. Activity log entries should be summarized or moved to longhorizon-plan.md.

**Action**: defer this — file is still scannable, and the resume recipe tells future agents to skip to specific sections.

### F8 — sub-archives in `runs/` (63 tars, ~50 MB) not in doc audit but worth flagging
63 archives is healthy but the doc tree doesn't say which are scored vs which are scratch. The progress.md score table maps sub-X → score. **Action**: add a `runs/INDEX.md` with one line per archive: `sub-16 — baseline for sub-20 stack, P=0.6076, V=0.975`. Helps navigation. Low priority.

## Action plan (executed in this session)

1. ✅ Move `docs/state/challenge-1-exploration-plan.md` → `docs/archive/challenge-1-exploration-plan-2026-11-07.md`
2. ✅ Restructure `docs/flow-lessons.md` with finer sections (F2)
3. ✅ Add "What to read for what" routing table to `AGENTS.md` (F3)
4. ✅ Update `docs/state/TEMPLATE.md` with longhorizon-plan pattern (F6)
5. (Defer) `runs/INDEX.md` (F8)

## What NOT to split

- `docs/skills/cocotb-patterns.md` (281 lines): single coherent topic; splitting fragments knowledge.
- `docs/joint-flow.md` (231 lines): all sections reference each other (cycle + handoff + state template).
- `.opencode/skills/*/SKILL.md`: each is already scoped to one task type.

## F9 (added 14:30) — `docs/agents/` size issue + safe scalable fix

**Problem**: `docs/agents/<role>.md` files (9 of them, 1491 lines total) are loaded into every sub-agent invocation. Each invocation pays the methodology-file cost. assertion-writer 345, test-writer 266, others 70-160. Across many invocations and growing with challenge-2/3 lessons, the always-loaded methodology corpus balloons.

**Quantified**: only 2 of 9 files have heavy embedded pattern libraries (~238 lines of pattern content in assertion-writer + test-writer). The remaining 7 files are mostly role+workflow+rules and don't bloat.

**Safe scalable solution** (3 steps, ~30 min execution):

1. **Move pattern blocks from agent.md to existing deep-ref files** (`docs/skills/<topic>-patterns.md`). For assertion-writer.md: categories 6-10 (Persistence, Decode, Strobe, Mux, Interface — ~100 lines, NOT already in sva-patterns.md) get merged into `docs/skills/sva-patterns.md § Assertion Categories`. Replace in agent.md with one-line pointer: "Categories 1-10 with code examples: `docs/skills/sva-patterns.md` § Assertion Categories (read before writing)." For test-writer.md: move Test Taxonomy detail (~98 lines) into `docs/skills/cocotb-patterns.md`.
2. **Add INDEX at top of each agent.md** (like flow-lessons.md now has). Cheap, helps navigation even for the short files.
3. **Convention going forward**: when a new pattern/lesson applies to a specific role, append it to the **deep ref** (e.g., sva-patterns.md), not the agent.md. Agent.md only grows for workflow or role-definition changes. This caps agent.md growth at the role layer.

**Net result**: assertion-writer 345→~210, test-writer 266→~180, others unchanged. ~25% reduction in always-loaded methodology corpus. Pattern library becomes the single source of truth (currently duplicated across agent.md and skills/).

**Why safe**: content is moved, not deleted. Sub-agent already loads deep-ref via Read tool per existing convention (the `## Inputs` section of every agent.md lists it). No behavior change to sub-agents; only less duplication.

**Why scalable**: future pattern lessons (from challenge-2/3) land in the existing deep-ref files where they're discoverable to ALL role agents (e.g., a new SVA pattern helps assertion-writer AND mutant-hunter AND rtl-fixer). Agent.md per role stays small.

**Why not all agent files**: 7 of 9 are already lean role+workflow files (70-160 lines, no embedded patterns). They don't need splitting; just need an INDEX for navigation.

**Execution cost**: 30-40 min. Do before challenge-2 (Nov 10) so the new design's agent invocations benefit.

## Reflect-on-lessons (this turn)

What we learned about the FLOW itself during Phase A + first Phase B waves:

- **L24/L25/L26 are formal-flow tooling lessons** that took ~1h of debugging to extract. They save future-me 1h+ per attempt. They were appended under Phase 4-P because I was running wildcards when I added them — wrong location. Reorganize.

- **`(* keep *)` is a new wildcard class** with surprising behavior (can help P by routing effect, but degrades when stacked). Worth cataloging in sky130hd-ppa skill.

- **Sequential SBY verification is high-friction in our flow image** (3rd time hitting this; L13 said it, A3 confirmed it). We should DEFAULT to "no SBY for sequential modules" unless there's a clear unbounded-invariant need. The skill `formal-verification-flow` should say this more explicitly.

- **Plan files survive compaction well when they follow the resume-recipe + tracker-table pattern**. Generalize the template.
