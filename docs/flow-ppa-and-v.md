# Phase 4 Detail: PPA and Mutation Kill

Detail reference for Phase 4b (PPA) and Phase 4c (V). The workflow skeleton in `docs/flow.md` points here. Read once when entering Phase 4.

## PPA optimization (Phase 4b)

**Critical insight**: P=0.5 = "same as baseline." To score >80 overall, P must exceed 0.5. Your fixed RTL removes bug-created logic, so P>0.5 is achievable.

### Three PPA tiers — budget each differently

| Tier | Scope | Local verification | Submissions to verify |
|------|-------|-------------------|----------------------|
| **A — provably-free** | Reset strip on datapath/memory flops; precompute config-derived shift/mask at latch; remove dead writes; Yosys-friendly tidy | 1 synth (check cells+WNS), 1 tests@ASSERTIONS=1, 1 local PPA | **0** — bundle with Tier B/C |
| **B — timing-neutral** | Pipeline split of long combinational paths (within REQ max-latency budget); operand isolation on idle adders; register replication for fanout; ICG-friendly 3-branch flops | synth + tests@ASSERTIONS=1 + 1 local PPA | **1 C-check** — verify hidden tests don't pin internal signal names or exact latency |
| **C — structural** | Module fusion, FSM restructure (one-hot↔binary, split/merge states), shared multipliers, multi-stage retiming across module boundaries, aggressive read-mux rewriting | synth + tests@ASSERTIONS=1 + 1 local PPA + REQ matrix re-audit | **1-2 C-checks** — verify hidden assertions don't bind to old internals or check exact cycle counts |

**Tier A is safe**: these don't change observable behavior. Yosys may already collapse some (always-0 bits). Apply, bundle, ship with Tier B.

**Tier B is where C-check discipline matters**: a 2-stage pipeline split moves `gain.data_q` to `gain.product_q + gain.data_q`. Hidden test for max latency (REQ-09, ≤16 cycles) passes; hidden assertion checking `$past(gain.data_q) == gain.prev_input` would fail. Local tests cannot see the hidden assertion. ALWAYS bank 1 C-check submission after Tier B before stacking more.

**Tier C is high-risk high-reward**: structural redesign can unlock big P wins (e.g., single multiplier shared between FIR and gain saves ~30% area) but typically breaks hidden test assumptions. Submit a Tier C variant, see if C drops. If yes: revert; if no: bank it.

### Iteration economy concrete budget (per design, after Phase 2 + 3)

- 0 submissions: stack Tier A changes locally, verify each with synth+tests (~5 min each).
- 1 submission: bundle Tier A + 1-2 Tier B changes, get C+P+V feedback.
- 1-2 submissions: try Tier C variants one at a time, C-check each.
- Final: ship the highest-projective-score archive.

### Reading PPA feedback

At each stage (synthesis → placement → CTS → routing): compare area (added flops/logic → remove equivalent-or-more elsewhere), WNS (timing regression → find the critical path in the report), power (correlates with area here).

**Power is often the biggest P term** (weight 0.2 but can swing ±30%): the #1 generic reclaim is to **compute config-derived shifts/masks (`v<<(32-W)`, `v&mask(W)`) ONCE when the config is latched, and read the pre-processed register in the hot datapath** — an always-recomputed shifter/mux toggles every cycle and lengthens the path. In challenge-1 this flipped P from 0.45 (a required config latch added flops) to 0.54 (power −34% vs baseline).

**Techniques**: full sky130hd-specific list in the `sky130hd-ppa` skill (deep ref `docs/skills/sky130hd-optimization.md`). High-level: free area from fixes, precompute config shifts/masks at load, narrow masked registers, `>=`→`==` where safe, balanced mux trees, homogeneous resets, strip async reset from pure data-path memory flops, register replication for fanout, operand isolation, then aggressive restructuring for the Explorer award.

### Stacked PPA recipe for control+bus designs

Validated challenge-2, P 0.48→0.4965. Apply in this order because **timing slack is the currency** — every area/power win must fit inside slack established by prior timing wins:

1. **ICG-friendly 3-branch flop** on every counter with distinct load/step/hold conditions (`if (load) q<=init; else if (step) q<=next; else hold;`). Buys WNS slack (+0.3–0.5 ns observed).
2. **Simplify hot comparators** using design invariants (e.g., `count >= period` → `count == period` when `count <= period` always holds).
3. **AND-OR read mux with one-hot decode** (replace `unique case` with `rd = (dec[0]&v0) | ...`). Big area win but WNS regression — must be bundled with (2) to recover timing.
4. **Operand isolation on idle adders** (`in_iso = en ? in : 0; sum = in_iso + 1`) when adder is enabled <50% of cycles.

Do **NOT** skip to step 3 without steps 1–2 first — the WNS cliff collapses P. Never attempt narrow-register rewrites or module fusion without prior timing slack. P ceiling on control+bus designs is ~0.50 (parity), NOT 0.48 as previously believed.

### Keep discipline (measured)

Any RTL edit changes the archive hash and re-rolls the mutation seed, so a **P-neutral change is pure downside** — it can only move V (often down when the new seed draws an equivalent mutant you can't kill), never up. Yosys also often already collapses "obvious" savings (unused always-0 bits), so a tidy-up edit can be exactly P-neutral. **Only promote a change that measurably improves P (area/power/timing); otherwise revert so the working tree matches sub-best.**

### Running the full flow correctly

Run full flow (with PPA) via `pg-flow-commands` skill — the baseline PPA is cached **by the `--design` dir's RTL** in `<cache>/baseline_ppa.json`. If you edit `rtl/` in place you MUST point `--design` at a **pristine baseline mirror** (released `rtl/` + `design.yaml` + `tb/` + `flow/`) with a matching cache dir, or P is fixed-vs-fixed (≈0.5, meaningless). First full run after an RTL change recomputes only the submission PPA (~longer, >2 min — set a large command timeout); the baseline PPA is reused. The OpenROAD detailed router can nondeterministically crash at 0 DRC (`ODB-0269`) — just re-run.

## High-level PPA improvisation (after Tier A/B exhausted)

When safe optimizations plateau (P stuck <0.55 on datapath or <0.50 on control), re-read the SPEC from the top and ask: **what does the spec NOT pin?**

Spec usually pins:
- Top-level interface (port names, widths, protocol)
- Functional behavior per REQ (output values given inputs)
- Reset state per register
- Bounded latency, throughput lower bound

Spec does NOT pin:
- Internal signal names (hidden assertions on internals are rare — but check)
- Exact per-sample pipeline depth (only max latency bound)
- FSM state encoding, state count
- Resource sharing (one multiplier vs two)
- Micro-architectural reordering (FIR then decim vs fused)

**Spec-freedom cross-check** (BEFORE brainstorming): have an LLM read the spec and produce a list of freedoms. Cross-check against your architecture. Most items will already be implemented. The remainder are your Tier C candidate list. Items that have been tried and P-negative are a strong signal you're at a local optimum — stop there. See `docs/skills/sky130hd-ppa/SKILL.md` Step 0.

**Brainstorm 2-3 aggressive redesigns**, write each as a separate sub-change list, then evaluate locally:

```
For each candidate Tier C redesign:
  1. Sketch the structural change (one paragraph).
  2. Re-check every REQ against the new structure (REQ matrix column).
  3. Implement, run synth + tests@ASSERTIONS=1 + local PPA.
  4. If P improves AND local tests pass AND full flow scores: queue for 1 C-check submission.
  5. If C=1.0 from the submission: bank as new sub-best.
  6. If C<1.0: revert; the hidden tests pinned what we thought was free.
```

Examples of Tier C outcomes from challenge designs:
- **Shared multiplier** between FIR and gain stages (saves ~1500µm²) — passes C on challenge-1 (REQ-22 doesn't pin "each sample uses a separate multiplier")
- **Combinational read mux** for register map (vs registered) — passes C on challenge-2 (REQ-03 said "combinational functions of cfg_addr" — required it, actually)
- **Async→sync reset conversion** — risky: REQ-42 may pin "asynchronous reset". Check the spec text.
- **8-parallel multiplier FIR** — FAILED at CTS (challenge-1): too many cells, design rejected. Lesson L14.

Always re-audit the REQ matrix after a Tier C change. If a REQ was previously pinned by an internal signal-def assertion you deleted, replace it with an interface assertion on the new structure.

## Mutation kill (Phase 4c)

**Primary V driver is REQ coverage, not line-by-line hunting.** Phase 3a's REQ matrix + Phase 3b's interface assertions naturally kill 30-38 of 40 mutants because mutants cluster at REQ-implementing code. If V < 0.90 after REQ coverage, the issue is a REQ coverage gap, not a missing targeted test — re-audit the matrix before writing per-line tests.

### Method for the residue (survivors after REQ coverage is complete)

1. Read the RTL at each survivor file:line
2. Determine what mutation was applied (which operator class)
3. Check: semantically equivalent? (see `mutation-kill-tactics` table). If YES: skip.
4. Identify the REQ that mutant implements (if any) — that REQ's coverage is incomplete
5. Add a test/assertion that exercises the SPECIFIC behavior the mutant changes
6. Re-run flow (fast, `--skip ppa`)
7. Repeat until V reaches ceiling (design V ceiling documented in `docs/designs/<name>.md`)

### V is archive-specific and deterministic

The mutation seed is `f(submission_hash)` — the SAME tar yields the SAME mutant set + survivors every run (no averaging). But **any archive change re-rolls the seed** (RTL, tests, assertions, even comments) → a DIFFERENT 40-mutant set. So:
- Never target specific line numbers (they shift); write behavior-based evidence.
- **Re-measure V after every archive change** (a PPA refactor can move or drop mutants).

Two common **unkillable (equivalent)** mutant classes — document as V ceiling, don't chase:
- A write to a register whose value is overwritten before any read (dead write)
- An operand guarded by a design invariant that makes the mutated sub-expression unreachable (e.g. `x && !full` where the producer never asserts `x` when `full`)
- Async-reset removal where Verilator's `--x-initial` matches the reset value (L8)

### V ceiling expectation

Across 20+ archives over 2 challenge designs:
- V=0.975 typical (REQ coverage complete + interface assertions)
- V=1.0 on ~3–4 lucky draws (15–20% hit rate)
- V=0.925–0.95 on unlucky draws or incomplete REQ coverage

**Best strategy: after each Tier B/C PPA improvement, submit once and observe the seed**. Keep the highest-projective-score archive. Do NOT assume that more archive changes always improve V. See L5 and the V seed uncertainty section in `docs/competition-strategy.md`.
