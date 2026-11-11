# challenge-2 formal findings

**Decision: SBY formal was SKIPPED for challenge-2** (recorded pre-release in
`docs/state/prep-plan.md` item 10, re-confirmed at Phase 1 with the design in hand).

Rationale (measured against c1 experience):
- The design's hard REQs (REQ-13/14 outstanding engine, REQ-19 descriptor fetch, REQ-24
  rotation) are SEQUENCE properties over a deep engine (multi-cycle AXI phases + 4 channel
  banks) — BMC depth needed ≫ what SBY on this image converges in budget; c1 showed formal
  value concentrates in purely combinational blocks (rounding/shift units).
- The only trivially-convergent units here (apb decode, irq set/clear) were already fully
  covered by SVA bound in simulation + neg fixtures (all fire), and probes confirmed D1–D5.
- Time budget went instead into the 52-round soak test with Python golden — which caught all
  5 deviations + CR on baseline (equal value, verified at flow-score level).

Cost check: 0 counterexamples needed — every deviation was caught by probe/soak before Phase 4.

For challenge-3 (pipelined core): revisit — per-stage combinational units (ALU decode,
bypass mux) are formal-friendly; stage boundary SVA + SBY on small units was pre-planned
(`docs/designs/challenge-3-playbook.md`).
