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
| 3 VERIFY | in_progress — evidence suite (tests/ + assertions/) next |
| 4 ITERATE | pending (local-iteration mode, server down) |

### Score history
| Sub | Time | C | V | P | Notes |
|-----|------|---|---|---|-------|
| — | — | — | — | — | baseline not measured yet |

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
- Phase 3 underway (agent building evidence suite). Review deviation list + fixes
  (`git diff` in challenge-2/rtl) if desired; D5 fix is the one judgment call.

### Best submission
- (none yet — local scoring pending)

### V ceiling
- Observed:
- Equivalent (unkillable) mutants:

### PPA
- Baseline: (fill from `runs/challenge-2-baseline-out/stages/ppa.json`)
- Applied optimizations:
  | Sub | Change | C | V | P | delta-P |
  |-----|--------|---|---|---|---------|


### Operator feedback log
-
