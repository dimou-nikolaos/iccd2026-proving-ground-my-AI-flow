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
