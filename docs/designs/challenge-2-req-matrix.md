# challenge-2 — REQ coverage matrix

One row per requirement. Exit gate: every REQ has >= 1 test and >= 1 assertion.
Status values: `todo`, `test`, `assert`, `done`, `n/a` (with reason).

| REQ | Summary | RTL location | Test | Assertion | Status |
|---|---|---|---|---|---|
| REQ-01 | all fields reset 0 except ID, LAST_GRANT | all always_ff; engine last_q=3 | | | todo |
| REQ-02 | reset anytime; after: reset vals, valids/dack/irq 0 | engine S_IDLE reset; chan/irq/gctrl resets | | | todo |
| REQ-03 | pready=1 always | dma_apb.sv:35 | | | todo |
| REQ-04 | transfer only in access phase | dma_apb.sv:27 (`access`) | | | todo |
| REQ-05 | decode paddr[11:2], [1:0] ignored | dma_apb.sv:28 | | | todo |
| REQ-06 | prdata valid in read access phase, else 0; undef bits 0 | dma_ctrl.sv:193–218 | | | todo |
| REQ-07 | unmapped → pslverr=1, no effect, read 0; 0x118/0x11C+n unmapped | dma_apb.sv:30–36 | | | todo |
| REQ-08 | pstrb byte-lane writes incl. IRQ_STATUS, CH_CTRL EN bit | dma_apb.sv:38; chan wmask; irq wmask | | | todo |
| REQ-09 | RO write: pslverr=0, no effect; undef bits ignore writes | ctrl mux (no wr path for ID/ACTIVE/GRANT/STAT) | | | todo |
| REQ-10 | EN 0→1 starts, clears ERR/DONE, other CTRL bits same write | dma_chan.sv:82–91, 107 | | | todo |
| REQ-11 | while EN=1: SRC/DST/LEN/NEXT/CTRL[6:1] writes no effect | dma_chan.sv:107,119–147 (`!en_q` gates) | | | todo |
| REQ-12 | word read at CH_SRC, write at CH_DST, wstrb=F, aligned | engine S_RD_ADDR/S_WR; araddr/awaddr :99,102 | | | todo |
| REQ-13 | one outstanding; arvalid/awvalid never both 1; AW/W either order | engine states; S_WR :196–206 | | | todo |
| REQ-14 | valid+payload hold while ready=0 | engine outputs state-gated | | | todo |
| REQ-15 | after successful word: LEN−1, src/dst +4 unless FIX | dma_chan.sv:119–141 (word_done) | | | todo |
| REQ-16 | burst ≤ 2^BURST words; keep engine unless desc ends/error/abort | engine words_q, burst_words :74; :212–220 | | | todo |
| REQ-17 | descriptor ends when CH_LEN reaches 0 | engine :214 (`ch_len <= 16'd1`) | | | todo |
| REQ-18 | LEN=0: ends at once at grant, NO data moved/access, LEN stays 0; HW_REQ+LEN=0 eligible w/o dreq | **engine: MISSING special case → D2**; eligible :81 OK | | | **DEV** |
| REQ-19 | word addresses wrap mod 2^32; LEN 0xFFFF = 65535 words | dma_chan 30-bit counters wrap | | | todo |
| REQ-20 | end: LINK&NEXT≠0 → fetch; else EN clears + DONE set (CH_STAT+IRQ) | engine S_END :131–139, :224–231 | | | todo |
| REQ-21 | fetch 4 words at CH_NEXT in order, incrementing, holding engine | engine S_FD_ADDR/FD_DATA, m_araddr :99 | | | todo |
| REQ-22 | fetched words replace SRC/DST/LEN[15:0]/NEXT; [1:0] ignored | dma_chan load_* :121–147; engine load_* :110–123 | | | todo |
| REQ-23 | after fetch: release engine, compete again | engine S_RELEASE | | | todo |
| REQ-24 | eligible = GEN&EN&!abort&(dreq if HW_REQ&LEN≠0); highest PRIO; first in rotating order among equal; start=last+1, 0 after reset | engine eligible :78–83; **dma_arb.sv:22 `>=` → D1**; start :13 | | | **DEV** |
| REQ-25 | GEN=0 stops new grants; burst in progress finishes | eligible `gen` term | | | todo |
| REQ-26 | LAST_GRANT = most recent grant, 3 after reset | engine last_q :161,173 | | | todo |
| REQ-27 | dack[n] ONE pulse after a burst that moved ≥1 word ends | engine :136–144 → **D5 double pulse (MED)** | | | **DEV?** |
| REQ-28 | any non-OKAY rresp/bresp → stop EN, ERR set, no DONE, chain ends | S_ERR via rresp OK; **bresp never checked → D3** | | | **DEV** |
| REQ-29 | failed read: nothing written, SRC/DST/LEN keep values | engine S_RD_DATA → S_ERR (word_done=0) ✓ | | | todo |
| REQ-30 | failed write resp: SRC/DST/LEN keep values | **missing → part of D3** | | | **DEV** |
| REQ-31 | abort: last word finishes, no new word, EN clears, NO DONE unless last word failed | chan abort_q :86; engine S_STOP :149 → **ev_done wrongly set → D4**; fetch-after-abort edge → D6 | | | **DEV** |
| REQ-32 | status set in event cycle; CH_STAT DONE/ERR sticky until restart | dma_chan :93–99 | | | todo |
| REQ-33 | IRQ_STATUS W1C with strobe | dma_irq.sv:20,31 | | | todo |
| REQ-34 | set beats clear same cycle | dma_irq.sv:31 (set after clear in next-state) | | | todo |
| REQ-35 | irq = \|(status&enable), function of regs only | dma_irq.sv:24 | | | todo |
| REQ-36 | (CR-D2) DESC[n] set per completed descriptor incl zero-length and chain-mid; same cycle as DONE if last; NOT on error/abort; W1C lane pstrb[1] | **to implement**: engine ev_desc at S_END; dma_irq set_vec + IRQ_MASK→0xFFF | | | CR |
| REQ-37 | sync design, rst_n async only, no latches, meets SDC | flow check (P stage) — no sim test | | | n/a (PPA stage enforces) |

## Notes
- Extract/refresh REQ list: `grep -ohE 'REQ-[0-9]+' challenge-2/spec.md | sort -uV`
- DEV rows = confirmed-pending-probe deviations D1–D4 (see `docs/state/challenge-2-longhorizon-plan.md`).
- D5 (dack double-pulse) decision pending a probe on baseline + spec re-read.
