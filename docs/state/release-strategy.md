# Release strategy — submissions, time budget, Explorer award (written Nov 9, before C2 release)

Read together with `docs/competition-strategy.md` (scoring), `docs/flow-ppa-and-v.md`
(tiers), `docs/state/prep-plan.md` (prep assets). This file answers: **what we do minute-by-minute
at release, when the first push happens, and how we pursue Explorer without burning submissions.**

Key rules of the game (recap):
- 30 submissions/design, 1 per 30 min, ~15 min feedback. C = hidden-test pass rate,
  V = 40-mutant kill, P = PPA vs baseline. score = 100·C·(0.5V+0.5P).
- Explorer award: **most distinct netlists with C>0.9** (per design or total — assume per design).
- Any archive content change re-rolls the mutation seed (V is seed luck; L22).

---

## 1. Submission safety doctrine ("never burn")

Every submission must pass, *before packaging*, the **identical local pipeline**:
`run --design <released> --submission sub.tar.gz` (full, incl. PPA) → `status: scored`,
evidence green, no assertion fires. A locally-red or non-packaged archive is never submitted.
Rejections still consume slot + cooldown. We rehearsed this exact loop twice
(`prep/flowcheck/`, `prep/flowcheck/dma/`) — trust the rehearsal, not optimism.

Variant queues (color-code in the ppa/variant plan file per design):

| Queue | What | C-risk | When |
|---|---|---|---|
| **GREEN** | variants **yosys-proven equivalent** to the banked C=1.0 core (`equiv_make/equiv_simple -seq 5/equiv_induct -seq 5` — same commands the flow uses; see flow-internals §mutation) | ~0 (proof = identical observable behavior, incl. hidden latency assertions) | Explorer workhorse — unlimited |
| **AMBER** | Tier B restructures that preserve spec but may change microarchitectural timing (pipelining, precompute) | small, bounded; 1 C-check each | P improvement phase |
| **RED** | Tier C redesigns / wildcard experiments | real | only from ≥6 reserved slots, one at a time, only after core banked |

Netlist distinctness (Explorer counts only distinct netlists): before submitting a GREEN
variant, compute a post-synthesis fingerprint locally (`yosys synth -top <top>; stat;
write_verilog; sha256`). **Do not submit a variant whose fingerprint duplicates an already
submitted one** — yosys often collapses pure renames to identical netlists. A variant is
"distinct" only if cell counts/structure differ. Expect ~50% of cosmetic candidates to
collapse; generate 2× candidates as a result.

Each variant also gets a local PPA number before submission (≈3–4 min each; batch overnight
with `scripts/pg-batch.sh`). If P is materially below the banked core's P, submit it only if
the Explorer count justifies the small score risk (award counts C>0.9, not P).

Endgame insurance: the **last submission of each design = the highest-projected archive**
(covers both "final counts" and "best counts" scoring interpretations). Track a
`runs/best.md` table: archive → C, V, P, proj.

## 2. Day-0 timeline (per design; C2 = Nov 10, C3 = Nov 13, 09:00 HKT)

| Clock | Step | Local time cost |
|---|---|---|
| 09:00–09:30 | `scripts/design-init.sh`, unzip, md5, scaffold req-matrix from spec; **baseline full flow run in background** (caches baseline PPA, gives baseline WNS) | run 10–20 min bg |
| 09:30–12:30 | Phase 1: REQ extraction → `Regmap`/`AXI_NAMES` fill (`prep/dma_vip/tests/dma_env.py` TODOs) → public tests green (≤10 min) → deviation hunt: static spec-vs-RTL pass, L41 probe families on live design, SBY on config/register logic | hunt is the variable; stop when 2–4 deviations + CR read are nailed (L1: look for the 4th) |
| 12:30–14:00 | Phase 2: fixes (cheapest correct form) + CR-D2/D3; lint/elab/public green in Docker | ~90 min |
| 14:00–15:00 | Phase 3: vendor VIPs + `dma_env/test_dma` into `tests/`, re-tag REQs, bind SVA; local `--skip ppa` (~2 min) then full run (~5–15 min); `scripts/mutant-report.py` triage; package | ~60 min |
| **~15:00** | **SUBMISSION #1** (C2 target T+6h, C3 target T+7–8h — core is bigger) | — |

Afternoon: read C feedback → fix queue starts immediately; Tier A PPA stacking (free, local).

## 3. Submission budget per design (30 slots)

| Slots | Purpose | Days |
|---|---|---|
| 1 | core fix bundle (deviations+CR+evidence+SVA) | Day 0 |
| +0–3 | C fixes from hidden-test names (C=1.0 gate — nothing else matters until banked) | Day 0–1 |
| +2–3 | PPA: 1 bundle of Tier A+B, then 1 Tier C C-check (P target 0.50–0.56; control+bus ceiling ≈0.50 per C1 calibration — note C1 was datapath and actually reached P=0.6146 there, so re-calibrate on C2's real baseline before capping ambition) | Day 1–2 |
| +1–2 | V targeted-kill round from server surviving-mutant locations (strobe-gating SVA pattern first — it hit 40/40 on one seed in C1); classify survivors killable/dead/equivalent (L7/L8/L42) before writing more tests. C1 evidence: **REQ-matrix completion is the primary V driver** — after filling 5 REQ gaps, V=1.0 was reached on the next seed | Day 2 |
| **14–16** | **Explorer GREEN queue**: equivalent-proven, fingerprint-distinct, locally scored variants — one per 30-min slot; each also re-rolls V (seed hedging per L22) | Day 2–5 |
| ≥6 | reserve: late C surprises, best-V archive re-submits, final best submission | Day 4–6 |

C3 (released Day 3): same budget shape; Explorer phase shifts to Day 4–6. Interleave:
C2 Explorer drops ride the 30-min cooldown while C3 core phases run — submissions cost
~1 min packaging + waiting, so parallel tracks don't conflict. Total Explorer across both
designs: realistically **24–30 distinct netlists** if GREEN generation keeps up.

## 4. GREEN variant generator (build the evening of Day 1)

Gate script already built and tested: **`scripts/explorer-check.sh <core> <variant>
[hashfile]`** — yosys equivalence with the *exact* flow recipe (read_slang, async2sync,
equiv_simple/induct -seq 5) + post-synth netlist fingerprint duplicate check. Verified on
`prep/flowcheck/variants/`: rename variant → PROVEN+distinct (2nd submission of it →
DUPLICATE caught); `>=`→`==` (semantically equal here!) → PROVEN; observable change →
2 unproven $equiv cells → rejected.

Candidate generation (write `scripts/explorer-variants.sh` once core RTL is banked —
inputs: banked core dir):
1. Apply mechanical edit classes (L23 audit list): **internal-signal** renames (NOT ports:
   ports are matched by name in instantiations and our bind maps), literal formats,
   always-block reordering, statement order within `always_comb`, extra intermediate wires,
   `case`↔`unique case`, parameterized width-1 constant re-expression, small
   re-decompositions yosys may not collapse (explicit mux select inversion + buffered tree
   with `keep` on one level).
2. Per candidate: `explorer-check.sh` (equiv + fingerprint, seconds) → PPA if distinct
   (batch overnight with `scripts/pg-batch.sh`). Green list → tar.
3. Submit from green list at every cooldown; keep 2-deep queue minimum (if the queue
   empties, hold the slot — an unused slot is not burned; a bad submission is).

RED/wildcard ideas (docs/flow-wildcard.md P-catalog) are only worth submitting once
GREEN supply is exhausted, since a C-drop both burns a slot and can't count for Explorer.

Note (C1 evidence): comment-only edits re-roll the V seed but produce an **identical
netlist** — they are seed-reroll archives, not Explorer variants. `explorer-check.sh`
fingerprint dedup catches exactly this; count them toward V-hedging, not the Explorer tally.

## 6. Challenge-1 reality check (measured calibration, Nov 7–8)

C1 (dsp_stream, datapath, 17 RTL files, ~5.4k cells) went baseline→**projected 80.73 in
~36 h**, so the Day-0 plan's pace is validated. Measured facts to carry into C2/C3:

- **Score ceiling**: final P=0.6146, V=1.0 (seed-lucky), C=1.0 banked. P stack: input-FIFO
  reset strip +0.034, pipeline-split-on-critical-path +0.057, FIR reset strip +0.004,
  multiplier operand isolation +0.014 (only multipliers win, L17), `fsm_encoding one_hot`
  on small FSMs, `keep` on 5 chosen registers. Power −66%, WNS 2.70→5.53 ns.
- **Critical path was the register-read mux** (`cfg_addr→cfg_rdata`), NOT the datapath
  (L21). DMA analogue: `psel/addr→prdata` decode path — attack it first for timing; the
  ch1 timing term was worth t=+0.23 of P.
- **V robustness beats V luck**: server seed ≠ local seed (tar-hash derived). C1 promoted
  the robust 0.975 archive over the seed-lucky 1.0 one mid-campaign, then banked a
  seed-lucky V=1.0 async-compliant archive as final when four equivalents tied on P.
  Same doctrine here: kill *classes*, and at endgame pick the lucky-V archive only among
  fingerprint-distinct equivalents with equal P.
- **Formal SBY was skipped on C1**: slang frontend rejected `|=>`/`|->` in SBY harnesses;
  per-cycle next-state sim assertions (run every cycle of every test) were judged
  sufficient. Precedent to reuse if C2 register logic formal gets fiddly — see
  `docs/designs/challenge-1-formal-findings.md` on the challenge-1 branch.
- **Server flakiness is real** (server was down at C1 session end; last C unverified).
  Bank C=1.0 by Day 0–1 and keep `runs/best.md` + banked archive md5 so endgame needs
  only re-submits, not diagnosis.
- **Costs measured**: full local flow on C1 RTL ≈ 6 min (synth 53 s + OpenROAD ~345 s,
  occasional ODB-0269 crash → just re-run); variant batches 5×`--cpus 2` = 19 min wall;
  cocotb gets 6.9× speedup at `--cpus 1` per job. Budget against these, not guesses.
- **Explorer tally on C1**: 21+ distinct netlists existed on disk by session end; our
  14–16/design target is achievable but needs the GREEN generator running Day 1 evening.
- C1 also confirmed Tier C restructuring risk is real: FIR parallel-topology rewrite
  passed tests but CTS-failed (L14) — Tier C only via RED queue with full-flow gate.

## 5. What NOT to do

- Never submit before the local full pipeline is green (rehearsal proved we can check).
- Never bundle two unproven Tier B/C changes in one archive (C diagnosis impossible).
- Don't burn Day 0–1 slots on P/V tinkering before C=1.0 is banked.
- Don't chase V past the equivalent-survivor ceiling (declare it in the progress file).
- Don't submit a fingerprint-duplicate "cosmetic" variant (yosys collapse = not distinct).
- Don't leave the final slot to an unproven experiment — final slot = best archive.
