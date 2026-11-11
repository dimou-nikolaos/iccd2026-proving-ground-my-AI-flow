# challenge-2 — progress (started 2026-11-11T04:42Z)

## challenge-2 — Phase 1 (analyze) — in_progress

### Resume recipe
1. Read `docs/state/challenge-2-longhorizon-plan.md` (Next action + phase tracker) — server is DOWN, local-iteration mode.
2. `md5sum runs/sub-best.tar.gz` and compare with **Best submission**.
3. Read `docs/designs/challenge-2.md` and `docs/designs/challenge-2-req-matrix.md`.

### Phase status
| Phase | Status |
|-------|--------|
| 1 ANALYZE | done — D1–D5 CONFIRMED by probes (5/5 fail on baseline, 5/5 pass on fixed) |
| 2 FIX | done — D1–D5 fixed + CR-D2 implemented; public tb 8/8 on fixed tree |
| 3 VERIFY | done — 58-test suite v4 (fails 16+ on baseline; soak test catches D1/D2/D5+CR on baseline), 1264-line SVA (0 fires), REQ matrix 100%, neg fixtures all fire |
| 4 ITERATE | in_progress — pool building (V locked for family A; PPA families scoring; server down) |

### Score history (LOCAL — server down; C unknown, proj assumes C=1.0)
| Sub | Time | C | V | P | proj | Notes |
|-----|------|---|---|---|------|-------|
| c2-sub-1 | 11-11 ~09:00 | ? | 1.0 | 0.487 | 74.35 | family A; WNS 1.478 |
| c2-sub-9 lzero | ~10:30 | ? | 1.0 | 0.470 | 73.5 | power +15% — rejected family |
| c2-sub-10 arbpar | ~10:30 | ? | 0.975 | 0.4962 | 74.8 | equiv-mutant trap — superseded |
| c2-sub-11 both1 | ~10:30 | ? | 0.95 | 0.4952 | 74.8 | superseded by abar2 |
| c2-sub-18/19/20 | ~11:00 | ? | 1.0 | 0.486/0.486/0.490 | 74.3–74.5 | keep-reg ×A: ≈no-op (+0.003 max) |
| c2-sub-21 arbbar | ~11:00 | ? | 0.975 | 0.5089 | 75.9 | P best (case-swap encoder equiv site → superseded by arb2) |
| c2-sub-22 abar | ~11:00 | ? | 0.95 | 0.4959 | — | superseded by abar2 |
| c2-sub-23/24/25 | ~12:00 | ? | queued | queued | — | a2 / arb2 / abar2 (waveC, expected P 0.49–0.52, V pool after) |

Full pool table with md5s: **runs/c2-best.md** (single source of truth for archives).

### Deviations found
| REQ | Location | Spec says | RTL does | Pattern | Confidence |
|-----|----------|-----------|----------|---------|------------|
| REQ-24 | dma_arb.sv:22 | first in rotating order wins among equal PRIO | `>=` → last wins | comparison | HIGH |
| REQ-18 | dma_engine.sv S_IDLE/grant | LEN=0 ends at once, no data moved/access, LEN stays 0 | full word moved, LEN wraps to 0xFFFF | missing case | HIGH |
| REQ-28/30 | dma_engine.sv S_WR_RESP (:207) | non-OKAY m_bresp → stop+ERR, hold addresses | m_bresp never inspected, word counted | unused input | HIGH |
| REQ-31 | dma_engine.sv S_STOP (:149) | abort: DONE not set | ev_done asserted at S_STOP | wrong event | HIGH |
| REQ-27 | dma_engine.sv S_END dack (:136) | one dack after burst end | two pulses per HW_REQ chain-descriptor end | extra pulse | MED |
| REQ-31 | abort vs in-flight fetch | no further bus access after abort | 4-word fetch completes, then EN clears | edge | LOW-MED |

Full fix sketches + phase tracker + protocol: **`docs/state/challenge-2-longhorizon-plan.md`**
(server-DOWN mode — read it before anything else).

### CR implementation notes
- CR-D2 DESC[11:8]: engine `ev_desc` output pulses at EVERY S_END entry (descriptor
  completion incl. zero-length via D2 fix; never on error/abort paths); wired through
  dma_ctrl to dma_irq `set_vec = {20'h0, ev_desc, ev_err, ev_done}`; IRQ_MASK → 0xFFF.
- Fetch-fail acceptance case falls out free: DESC was set at the completing descriptor's
  S_END cycle; S_ERR then sets ERR without DONE.
- W1C/enable lane rules come free via existing wmask path (byte lane 1 = pstrb[1]).

### Activity log (newest first)
#### 2026-11-11 ~12:00 / round 2 — V hardening + arbiter restructures + soak
- Did: V reroll round 1 (5 seeds) -> found real gaps -> suite v2 (aw/w done-gaps,
  penable-without-psel, 4-way rotation); PPA round 1 (lzero/arbpar/both1/keep×3);
  rotation mutant forensics (equivalence sweep in Python, L52/L53); new families
  a2/arb2/abar2 (default-arm equiv removal + observable barrel encoder + phase-3b
  kill scenario); soak test (52 rounds, golden oracle) — catches D1/D2/D5+CR on
  baseline (D3/D4 covered by directed req28/req31 tests). Suite now v4: 58/58 fixed,
  0 assertion fires; neg fixtures g1–g6+wdone all fail correctly.
- Ops: pg-batch DOCKER/LOGDIR/--extra/per-sub-cache fixes; critical-path.sh DOCKER;
  explorer-variants keep-comma bug fixed. Sizing: full-flow ≤3×5cpu, skip-ppa 5×3.
- Found: baseline RTL itself breeds equivalent mutants (unreachable prdata default
  arms — L52); mis-grant mutants self-heal — need active wrong target or per-grant
  LAST_GRANT check (L53).
- Next: collect waveC -> reroll V pools for a2/arb2/abar2 -> pick winner family ->
  freedom pass (latency-free restructures) -> explorer GREEN queue.
#### 2026-11-11 ~05:30 / Phase 1 confirmed + Phase 2 fixes
- Did: probes `runs/c2-probe/` (p1..p5) — ALL 5 FAIL on baseline (D1 grant order, D2 LEN=0
  bus access+wrap, D3 bresp error treated as success (DONE set!), D4 abort DONE set,
  D5 five dack pulses instead of 3 on HW_REQ chain). Applied fixes in `challenge-2/rtl/`:
  dma_arb `>`; engine len0 grant→S_END; S_WR_RESP checks m_bresp→S_ERR + word_end gated
  on OKAY; S_STOP no ev_done; S_END dack only when not chain-continuing; CR-D2 ev_desc
  chain (engine/ctrl/irq/pkg).
- Found: baseline self-compare flow needs `report.md` stub in archive (re-tar + rerun);
  probes need careful stimulus: p1 must program BEFORE GEN; p4 must abort while engine
  OWNS the channel (burst>1 words); p5 must hold dreq through the chain.
- Next: Phase 3 evidence suite in `challenge-2/tests/` (+ vendored VIPs) and `assertions/`;
  then local full-flow scoring `runs/c2-sub-1`.

### Next action for operator
- Nothing blocking. Agent executing plan v3 NEXT STEPS (critical path -> V-lock
  reroll batch -> defensive PPA batches -> freedom pass -> explorer). Pool table:
  runs/c2-best.md. Operator task: watch for submission server availability.

### Best submission
- c2-sub-1 (family A, md5 c81fe01b41880bf0ca506b0608172185) — proj 74.35 — LIKELY
  superseded by a2/arb2/abar2 (waveC) and their V rerolls; nothing banked (server down).

### V ceiling
- Observed: family A **V=1.0 on 6 distinct seeds** (sub-1 + sub-12..16). arb2/abar2 pool pending.
- Equivalent (unkillable) mutants: A had 2 (prdata default arms) → REMOVED in a2/arb2/abar2;
  arbpar case-rotate family had ≥3 (avoided); barrel encoder keeps 1 fallback site
  (harmless: reachable only when grant_valid=0).

### PPA
- Baseline (cached, `runs/challenge-2-baseline-out`): area **45710.1 um2**, final WNS **+2.367 ns**
  (achievable period 7.63 ns → real timing headroom), power **6.63 mW**, 6066 cells,
  hold WNS 0.47 (no hold fix needed), route WL 192k um. PPA stage 592 s (route 503 s).
- Critical path (sub-1, 6_finish.rpt): dreq[0] → u_engine.state_q[6]/[8] (slack 1.48) via
  arbiter serial compare + ch_len[grant_idx] 16-bit mux+compare. Second group paddr→prdata 3.45.
- Applied optimizations:
  | Sub | Change | C | V | P | delta-P |
  |-----|--------|---|---|---|---------|
  | sub-9 lzero | ch_len zero/le1 flags | ? | 1.0 | 0.470 | −0.017 (power) |
  | sub-10/21 arbpar/arbbar | parallel arbiter (max-tree+rotate) | ? | — | 0.496→0.509 | +0.009→+0.022 |
  | sub-11/22 both1/abar | lzero+arbiter | ? | — | 0.495 | +0.008 |
  | sub-18..20 keep cfg/en/len | single keep-reg | ? | 1.0 | 0.486..0.490 | ≤+0.003 |
  | waveC a2/arb2/abar2 | arm-fix + observable encoder | ? | queued | queued | expect ≥+0.02 |

### Operator feedback log
-
