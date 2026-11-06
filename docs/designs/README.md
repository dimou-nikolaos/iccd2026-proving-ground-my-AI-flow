# Design Knowledge Documents

These documents capture per-design facts: module hierarchy, signal names for assertion targets, protocol summary, and confirmed deviations.

## Purpose

The workflow, agents, and skills are **design-agnostic**. When a new design appears (competition reveal), you create a design doc for it as the FIRST analysis step. Then task cards reference `docs/designs/<name>.md` and agents know where to look.

## Lifecycle

1. **Competition designs**: create a new doc here as soon as the design is revealed (Phase 1 start). The spec-analyst produces the hierarchy/signal table as a side-effect of Phase 1.

## Template

For a new design, capture:
- Top module name, clock, reset, period
- Module hierarchy (tree)
- Key internal signal names per submodule (for assertion bind targets)
- Protocol summary (handshake, register map, data flow)
- Change request summary
- Confirmed deviations (populated after Phase 1 completes)

## When NOT to read these

If you're working on a design that has no doc here, do Phase 1a yourself (or spawn @spec-analyst) and produce the doc as your first deliverable. Do NOT assume another design's structure carries over.

## Naming + dir-name gating (CRITICAL — prevents cross-contamination)

When the working dir is `<X>/` (e.g., `challenge-1/`, `challenge-2/`), the matching design doc is `docs/designs/<X>.md` (or `<X-short-slug>.md` if a name was assigned).

**Rule**: while working on `<X>/`, READ only `docs/designs/<X>*.md`. Do NOT read other designs' docs. Each design has different signal names, address maps, and V ceilings — assuming patterns from one design transfers to another causes errors.

When a new competition design is named (e.g., `challenge-1` with top module `fifo_ctrl`), name the design doc to match: `docs/designs/challenge-1.md` or `docs/designs/challenge-1-fifo-ctrl.md`. Use the working dir name as the gate key.

General lessons extracted to `.opencode/skills/` and `docs/agents/` always apply — those are NOT gated.
