# Session State Template

Copy this to `docs/state/<name>-progress.md` at Phase 1 start. A fresh session reads it to resume without re-analyzing completed phases. **Update after every meaningful step** (phase change, flow run, deviation found, PPA opt, operator feedback). Append-only in the Activity log — don't rewrite history.

The two sections that must always be current for seamless restart: **Next action for operator** and **Activity log**.

```markdown
## <name> — Phase <N> (<phase-name>) — <status>

### Phase status
| Phase | Status |
|-------|--------|
| 1 ANALYZE | pending / in_progress / done |
| 2 FIX | … |
| 3 VERIFY | … |
| 4 ITERATE | … |

### Score history
| Sub | Time | C | V | P | Notes |
|-----|------|---|---|---|-------|
| 1 |  |  |  |  | RTL only, no tests |

### Deviations found
| REQ | Location | Spec says | RTL does | Pattern | Confidence |
|-----|----------|-----------|----------|---------|------------|

### CR implementation notes
-

### V ceiling
- Observed across N runs: [values]
- Equivalent (unkillable) mutants: file:line + reason

### PPA
- Synthesis area / Final area / WNS / power:
- vs baseline: Δarea, ΔWNS, Δpower
- Optimizations applied (each was a submission):
  | Sub | Change | C | V | P | ΔP |

### Activity log (append-only, newest at top)
#### sub-N / <timestamp>
- Did:
- Found:
- Next:

### Operator feedback log
- [date/time]:

### Next action for operator
- Upload: runs/sub.tar.gz (sub N) — expected C/V/P
- Then: paste flow report back

### Best submission
- runs/sub-best.tar.gz from sub N (C=x V=y P=z)
```

## Status values
- `done` — all phases complete, final submission packaged
- `in_progress` — currently working on the marked phase
- `blocked` — needs human input (spec ambiguity, conflicting requirements, C<1.0 diagnosis)

## Long-horizon plan file (create when time-per-design is large)

When a design has more time than the workflow (e.g., 10-day window, server down for several days), keep a SEPARATE `<design>-longhorizon-plan.md` alongside `<design>-progress.md`. This pattern proved compaction-safe in challenge-1.

Structure (in order):

1. **Resume recipe** at top (md5 check on sub-best, where to read tracker, what to do)
2. **Current state snapshot** (md5 of sub-best, total subs on disk, V=1.0 lucky pool, lessons count)
3. **Phase status tracker table** — rows = phases (A, B, C...), columns = goal, time budget, status, findings. Update each phase completion.
4. **Per-phase sections** with concrete sub-tasks, decision rationale, and "what we'd do differently next time"
5. **Time/when table** mapping calendar slots to planned activity
6. **"Next action for the agent reading this"** — single concrete next step, not a list
7. **Append-only activity log** at bottom

Key invariants:
- Tracker table is the single source of "are we done with phase X"
- "Next action" is short enough that a fresh agent can execute it without reading the whole file
- Findings column summarizes lessons (with L-numbers) so future sessions inherit the why

Don't conflate this with `<design>-progress.md`. The progress file is the LIVE running record (one cycle per server submission, score history table). The plan file is the STRATEGIC multi-day view. They coexist; cross-reference sub-best md5 between them.

## Session checkpoint file (write BEFORE compaction or end-of-day)

When context is getting long, compaction is signalled, or the operator ends the session: write `docs/state/<name>-session-checkpoint-<date>.md` BEFORE continuing. Contents (see `challenge-1-session-checkpoint-2026-11-08.md` and `challenge-2-session-checkpoint-2026-11-12.md` for the proven shape): pinned bank archive + md5, what stack of changes that archive carries, state-at-end bullets, batches/waves done (small table), C-risk surface, submission playbook (locked), and a compaction-survival checklist. The checkpoint pins NUMBERS (md5s, scores) so a post-compaction agent can detect drift.
