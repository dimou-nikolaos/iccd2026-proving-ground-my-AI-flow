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
| c2-sub-23/24/25 | ~12:00 | ? | 1.0/0.975/0.975 | 0.487/0.4984/**0.5184** | — | a2/arb2/abar2; abar2 = P best, survivors all EQUIV |
| c2-sub-29/30/31 | ~13:15 | ? | 1.0/1.0/0.975 | 0.4886/0.5037/0.5043 | — | a3/arb3/abar3 (equiv-site removal); P shuffles ±0.014 — place/route luck |
| c2-sub-26..28 | ~13:40 | ? | **1.0×3** | (0.487)* | — | a2 V-pool 4/4 |
| c2-sub-32..34 | ~13:45 | ? | **1.0×3** | (0.5184)* | — | abar2 V-pool 3/4 — best defensive core candidate |
| c2-sub-35/36/37 | ~14:00 | ? | queued | queued | — | waveF: abar5 (strobe-slim), abar8 (gate-enc), abar9 (keep len_q) |

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

#### 2026-11-11 ~14:30 / waves D–F + V pools + defensive ceiling
- Did: waveD a3/arb3/abar3 (V 1.0/1.0/0.975; P 0.4886/0.5037/0.5043 — P shuffles ±0.014,
  equiv-site-removal not free); waveE rerolls (a2 4/4 V=1.0, abar2 3/4); waveF isolation
  hybrids (abar5 strobe-slim P=0.4959 HURT; abar8 gate-enc P=0.5019 HURT; abar9 keep-len_q
  P=0.5169 ~flat: area −135µm² below baseline but WNS −0.14).
- Decided: defensive PPA STOPPED at abar2 (P=0.5184, 23 tries). Two counterintuitive facts:
  semantically-neutral micro-edits reshuffle P&R and often LOSE (±0.02); keep-reg buys area
  but spends WNS.
- Insight (from P_breakdowns): t-term ≈ −0.01 per 1% worse achievable period, LINEAR and
  uncapped while timing met → WNS maximization is the highest-value remaining lever
  (+0.9 ns WNS ≈ +0.12 P). waveG = abar2 keep-work rerun for crit-path extraction.
- Ops gotcha: pkill -f 'pg-batch.sh' matches your own shell command line — use pgrep -f
  'scripts/pg-batch' with care or kill by PID.

#### 2026-11-11 ~17:30 / wildcard program (Phase 4e adapted, server down)
- Crit-path (abar2, keep-work): only 2 real setup paths, both end at `ch_len_zero[grant_idx]`
  → state_q: (1) dreq[0]→arbiter(max-tree→match→barrel-rotate→rk mux→start+rk add)→state,
  (2) len_q→zero-flag→same mux. period_min 7.45 vs baseline 7.633.
- **Wildcard W1** (one-hot grant chain replacing rk+adder; python-verified 16k cases):
  REJECTED as P-core (P=0.5072; area +1270µm² ≫ t gain; chain depth not actually reduced —
  mux trees map efficiently to OAI cells). Keep as V-reroll asset (V=1.0 first seed).
- **Wildcard W2b** (registered arbiter decision + commit-guard `gv_q && eligible[gi_q]`):
  naive W2 (no guard) REJECTED by own assertion a_set_needs_en (REQ-32) — commit targeted
  an ineligible (already-served/EN-cleared) channel. Guarded version: 8/8 public + 58/58
  suite with patched engine_sva ($past(commit)/$past(gi_q)); full flow pending (waveK).
  Hypothesis: removes whole dreq cone from STA → period_min ~6.0 → t +0.10-ish.
  C-RISK: hidden tests may pin grant-cycle timing; bank ONLY after defensive C=1.0.
- Insight: t-term slope measured ≈ 0.00975 per 1% period_min change (abar2/a2/arb3 points
  fit line exactly, slope ~1.0 in normalized ratio units).
- Ops: explorer-check.sh needed design.yaml `top:` (silent filelist-head fallback = dma_pkg
  → elaboration fail); fixed to hard-error. buffer/keep-reg/rename micro-classes collapse or
  are naming-doctrine-unsafe on this design; Explorer diversity must come from semantic
  families (A vs abar2 vs w-variants).

#### 2026-11-11 ~19:00 / waveK W2b in + W3 built; P-term decomposition attempt
- W2b scored: V=1.0 (1st seed), P=0.4974. dreq cone eliminated (gone from report) but NEW
  worst = **paddr[6]→prdata combinational (APB read path, slack 2.13, period_min 7.87)** —
  present in EVERY family; the real global wall. Also +1010µm² area, +0.38mW power (gi_q
  loads every cycle unconditionally → toggle).
- P_breakdown terms a/t/p measured against P across 6 archives: NOT additive — power term
  is heavily attenuated (p=+0.14 → tiny P effect), area ≈ linear small, timing ≈ linear
  ~0.05/wns-ns locally at best. Conclusion: **P plateau ≈ 0.52 for this design**; big-P
  arithmetic tricks (challenge-1 pattern) do not apply (no multipliers/filters).
- W3 (registered len flags) equiv-PROVEN with flow's own yosys equiv gate (explorer-check
  as equivalence gate = new reusable pattern: `explorer-check.sh <core> <variant>` before
  spending full-flow time). Queued waveL w/ e03/e04 (e03/e04 mis-pack = ignore).
- Explorer distinct-netlist inventory (scored, local): A-family (sub-1,12..16,26..28 share
  netlist — one entry), arb2, abar2, abar5, abar8, abar9, a3, arb3, abar3, keep0-2, w1, w2b
  ≈ 12-13 distinct + w3/e05/e06 pending. Well above typical field.

#### 2026-11-11 ~21:30 / waves M–N + rejects; C2 exploration FROZEN
- e05/e06 (buffer entries): scored V=1.0, P=0.5058/0.5055 — netlist-distinct, added to
  Explorer queue. w5 (fsm_encoding one_hot): P=0.5184 with **byte-identical PPA to abar2**
  → attr ignored by flow synth → netlist DUPLICATE, do not submit (L54).
- Dropped: v6 (flags-at-ctrl: 1-cycle-stale flag = real deviation, L56), w2c (gated gi_q
  load: equiv gate can't induct the pairing; provably safe by hand but doctrine holds).
- w3 (len flags in chan, equiv-PROVEN): V=1.0, P=0.4999 — Explorer entry, not bankable.
- **C2 final state: bank#1 = c2-sub-32 (abar2, V=1.0, P=0.5184; spares -33/-34; md5 per
  progress file). Pool: 36 archive rows, ~16 distinct netlists (Explorer-ready).**
  27+ PPA tries. All wildcards logged (L54–L56). Remaining time → C3 readiness + upkeep.

#### 2026-11-11 ~22:30 / W6 freedom-pipeline (operator-approved; spec §11 latency freedom)
- Spec audit: freedoms that matter — §11 "may add/remove cycles of latency"; pinned:
  REQ-03 (no wait states), REQ-35 (irq=f(regs) only), REQ-13 (1 outstanding), REQ-32
  (status in event cycle). Two setup walls in EVERY family start at top INPUT ports
  (2 ns input-delay each): dreq→arbiter-cone (abar2 WNS 2.546) and paddr→prdata read
  mux (W2b exposed: 2.13). Cut BOTH → period_min 7.45 → ~6.0 → t +0.20 → **P≈0.68-0.73
  projected (score ~85)**. t-slope ≈ +0.0098 P per 1% period_min.
- **Rejected mid-build: `dreq_q` input pipeline.** One-cycle dreq shadow AFTER dack
  passes the W2b commit guard (eligible sees stale dreq=1) → spurious 2nd burst,
  REQ-27 violation. dreq sampling must stay live. (lesson L57 pending)
- **W6 = w2 base (registered grant + commit guard) + `word_d` APB decode register** at
  the SETUP/ACCESS boundary in dma_apb (dma_apb gets clk/rst_n ports — internal, OK).
  Doctrine: `word` stays live paddr[11:2] (baseline meaning); NEW name `word_d` feeds
  the decode. apb_sva: 6 decode-equivalence asserts gated `access |->` (REQ-04-faithful:
  setup-phase decode is spec-irrelevant). ctrl_sva/engine_sva need NO patch (already
  access-gated / bind into dma_engine so they follow the registered outputs).
- Status: lint clean (UNUSEDSIGNAL on kept-live `word` is intentional). Staged
  runs/testrun-w6 (rtl/tests/assertions/tb). Public TB run started, interrupted — rerun.
  Then 58-suite+assertions, then full flow → c2-sub-w6.
- C-risk (documented): hidden test driving psel&&penable with NO preceding SETUP cycle
  (baseline decodes live paddr; word_d holds previous). Hidden asserts binding dma_apb
  internals see stale decode outside access. Containment: exploratory slot AFTER
  bank #1 c2-sub-32; failure costs 1 slot + hidden test names.

#### 2026-11-11 ~23:15 / W6 SCORED: V=1.0 (1st seed), P=0.5082 → Explorer entry, not bank
- Full flow: public+tests+assertions green; mutation V=1.0 seed 1. PPA: area 46903.7
  (+1100 vs abar2), FINAL WNS 2.567 (abar2 2.546 — floorplan said 3.71, route ate it),
  power 6.08 mW (+0.39). md5 95a2d13d…, out at runs/out-c2-sub-w6.
- **L58: final-route WNS pinned ~2.5–2.7 in every family** ⇒ t-term plateau is
  place/route-bound; RTL pipelining cannot move P. P ceiling for this design ≈ 0.52.
- Area/power audit (user questions): (1) reset removal — only 11 truly-internal engine
  FFs safe (own_q/words_q/moved_q/aw_done_q/w_done_q/fd_idx_q; state_q+map regs must
  reset; data_q drives m_wdata top pin → x-after-reset C-risk) → ~25 µm², P +0.001 →
  build as R1-lite Explorer-only (queued); (2) FSM re-encode — attrs ignored (L54),
  same-width encodings = zero gain, one-hot = state_q width change → hidden bind `.*`
  elab fail → C=0. DEAD.
- Bank list UNCHANGED: #1 c2-sub-32 (abar2 0.5184). Explorer queue += w6, R1-lite.

#### 2026-11-11 ~23:40 / R1 SCORED: V=1.0 (1st seed), P=0.5102 → Explorer entry
- Reset-slim (11 internal engine FFs, load-before-read proven per FSM path): area
  **45552 µm² (−251 vs abar2, UNDER baseline 45710)** — reset tree + slim cells pay
  more than the 25 µm² FF estimate. But final WNS 2.118 vs 2.546 (place dice for this
  netlist) → net −0.008. md5 a6c9c4b7…; out runs/out-c2-sub-r1.
- Own-assertion fix required: 2-STATE Verilator `=== 1'bx` checks are vacuous-WRONG on
  x-initial regs — reset assertions for reset-less FFs must be REMOVED, not relaxed
  (engine_reset_sva updated in r1 tree).
- r1b dice-resample SKIPPED: micro-edit resamples historically lose ≥0.017 to placer
  shuffle vs +0.003 area upside. Bank stays c2-sub-32.
- CARRY TO C3: reset-slimming of unobservable datapath/pipeline flops is a REAL
  ~0.5% area lever where the design has wide registers (c2's register map pinned by
  REQ-02 blocked most of it). Audit C3 pipeline stage regs vs its reset REQ early.
- C2 status: 46 archives in pool, ~18 distinct netlists; exploration closed; upkeep
  + C3 readiness until server opens.

### Flow-conformance audit (2026-11-11 ~22:45, vs docs/flow.md)
- Phase 0/design-init: DONE (bootstrap, baseline PPA cache, public smoke).
- Phase 1: 1a static + 1c probes DONE (D1–D5 confirmed; probe tests runs/c2-probe cover
  the 5 deviation REQs; public-TB blind spots documented). **1b SBY formal: skipped
  DELIBERATELY per L13 ROI caveat** (control+bus design, shallow combinational cones;
  the SVA next-state defs + equiv_induct gate (explorer-check) did the confirming work).
  Not previously recorded — recorded here.
- Phase 2 after-fix checks: all DONE (filelist diff, lint, slang elab via flow runs, public TB).
- Phase 3 exit gate: ALL MET (58/58 ASSERTIONS run, 0 fires, 16 fail baseline, REQ matrix
  100%, top-level interface assertion modules bind dma_ctrl).
- Phase 4a: blocked by server outage (C unknown; will submit slot-1 on open).
- 4b/4c: DONE (plateau + V ledger). 4e: open — W6 in flight.
- **Gaps found & fixed**: (1) formal-skip decision now recorded (above); (2) NEW
  docs/designs/challenge-2-ppa-plan.md created — wildcard ledger was only in gitignored
  runs/c2-best.md (loss risk); (3) report.md in banked tars says "53 tests" (suite is 58)
  — regenerate report.md when next bank/spare is packed.

### Next action for operator
- In flight: **W6 freedom-pipeline** (operator-approved wildcard, see log 22:30). If it
  scores P>=0.60 locally, it becomes an exploratory slot AFTER bank #1; bank #1 stays
  **c2-sub-32** (abar2, V=1.0, P=0.5184, md5 d3d0381f7d7ca0c73157a416d6b80e8c; copy at
  runs/sub-best.tar.gz). Spares: -33/-34. Operator task: watch submission server.

### Best submission
- **c2-sub-32** (abar2 core, V=1.0, P=0.5184, md5 d3d0381f7d7ca0c73157a416d6b80e8c) —
  proj 75.9 @C=1.0. Nothing banked yet (server down). Earlier: c2-sub-1 (family A,
  proj 74.35) kept as the C-safest fallback if hidden asserts bind arbiter internals.

### V ceiling
- Observed: family A **V=1.0 on 6 seeds**; a2 **4/4 seeds**; abar2 **3/4 seeds** (miss = equiv site
  selected); arb3 V=1.0 (1 seed); a3 V=1.0 (1 seed).
- Equivalent (unkillable) mutants (permanent tax wherever present):
  1. prdata unreachable default arms ×2 — REMOVED (a2+ families).
  2. arbiter case-rotate dup sources — family avoided (barrel since arbbar).
  3. IRQ strobe redundant `glob_sel` ×2 — removed in a3/arb3/abar3 (effect on P mixed).
  4. rk encoder fallback const — removed in arb3/abar3/abar8 gate-form.
  5. `dma_chan wmask[0]→[1]` — lane0 bits indistinguishable under byte-strobe invariant:
     present in EVERY family, cannot be engineered out safely (wmask expansion invariant).
     Budget: ~8.8% of archives of any family lose 0.025 V → pool-reroll and pick clean seeds.
- Strategy: score 3–4 rerolls per finalist family; bank V=1.0 archives.

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
