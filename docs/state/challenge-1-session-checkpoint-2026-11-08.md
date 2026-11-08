# Challenge-1 session checkpoint (2026-11-08, pre-compaction)

## Resume recipe
1. `md5sum runs/sub-best.tar.gz` — must be `8afb1c1c4425245c5aececdae2e465dd` (= sub-e22). If different, trust tar.gz and update progress.md.
2. Workspace `challenge-1/rtl/` matches sub-e22 RTL.
3. Read "In progress" table below for active task status.
4. Continue per "Next action" at bottom.

## State at this moment
- **sub-best: sub-e22** (P=0.6143, V=1.0 local, C unknown). md5 above. Projected at C=1: 80.72.
- sub-e22 = sub-v28 + comment marker at top of each .sv file (Explorer variant for V reroll).
- sub-v28 = sub-v20 + `(* keep = "true" *)` on dsp_fir.acc_q.
- sub-v20 = sub-16 + `(* keep *)` on dsp_rndsat.kept.
- Total sub archives: 75+ (`runs/sub-*.tar.gz`)
- **V=1.0 lucky pool: 16 variants** (e3, e6, e7, e11, e12, e15, e16, v18, v20, v22, e22, e26, e28, e29, e31, e33)
- Server: DOWN — cannot submit.
- Lessons: L1-L30 (L29 added: cross-module keep stacks compound; L30: critical-path restructures on cfg_addr→cfg_rdata all LOST).
- Doc audit F1-F9 done. F9 work committed as `e20ae30`.
- Archive: `docs/archive/challenge-1-exploration-plan-2026-11-07.md` (now gitignored).

## In progress: post-audit challenge-1 time-spend plan

| Wave | Action | Variants | Status |
|------|--------|---------|--------|
| **A1** | `(* keep *)` on 4 wires (gain, decim, stream, fir) | v25-v28 | DONE — **v28 won P=0.6143** (stack on v20) |
| **A2** | Stack 4 more keeps on v28 base (gain.product_q/centered_q, stream.gain_data, fir.prod_q) | v29-v32 | DONE — all plateau at P=0.6143 or regress. L29. |
| **B1** | Critical-path restructure on dsp_regs (parallel_case, one-hot AND-OR, keep on word) | v33-v36 | DONE — ALL LOST P (0.59-0.60). L30. |
| **C1** | Explorer variants on v28 base (5: comment, marker-alt, literal, begin/end, named labels) | e21-e25 | DONE — **e22 V=1.0 lucky P=0.6143 → promoted to sub-best (80.72 projected)** |
| **C2** | Explorer variants on v20 base (5) | e26-e30 | DONE — e26, e28, e29 V=1.0 (P=0.6118). Pool grew. |
| **C3** | Explorer variants on e22 base (4) | e31-e34 | DONE — e31, e33 V=1.0 P=0.6143 (same as sub-best). Pool grew to 16. |
| **D1** | Formal SBY A4 (regs, next-state pattern) | formal/ | PENDING |

**Sub-best promotion rule**: any variant with V=1.0 AND P >= current best P AND md5 distinct from existing sub-best → promote.

**Time estimate remaining**: C3 ~17 min, then D1 ~1-2h (low priority). Then idle until server returns.

## What NOT to redo
- Re-archiving the 12h exploration plan (already in `docs/archive/`)
- Re-restructuring flow-lessons.md (done; INDEX at top)
- Re-adding phase-routing table to AGENTS.md (done)
- F9 restructure of docs/agents/ (done, committed in e20ae30)
- Re-running pg-batch on v17-v24 (results known: only v20 won)
- Re-running Phase A3 (dsp_stats SBY — STALLED, do not retry)

## Next action
**Wait for C3 batch (pid 2013133, e31-e34) to complete (~10 more min from launch).** Read results, promote if any have V=1.0 AND P=0.6143.

Then **optional Batch D** (formal SBY A4 on dsp_regs next-state pattern):
- Existing working example: `challenge-1/formal/regs_priority.sby` + `regs_priority_sva.sv` (next-state pattern proven).
- Extend: write `challenge-1/formal/dsp_regs.sby` + `dsp_regs_sva.sv` proving REQ-22/23/24/25/26 (write decode for CTRL, OFFSET, GAIN, LIM_HI, LIM_LO, NTAPS, IRQ_EN) and REQ-35/36 (irq W1C semantics).
- This is mostly verification value, low direct score impact.

If D done and time remains: try more PPA wildcards beyond `(* keep *)`:
- `(* fsm_encoding = "one_hot" *)` on FIR state_q (currently 2-bit state)
- `(* use_dsp48 = 0 *)` (probably no-op for sky130)
- Explicit Yosys techmap directives (advanced, risky)

If server comes back UP: immediately submit sub-e22 (current sub-best, projected 80.72). Wait for C result. If C<1.0, diagnose failed hidden test names.

## Compaction notes
If only this file survives, follow "Next action" above. The full F9 work is done and committed (e20ae30), so don't re-do it. The plan for A/B/C/D is the active work.
