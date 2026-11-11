# challenge-2 — REQ coverage matrix

One row per requirement. Exit gate: every REQ has >= 1 test and >= 1 assertion.
Status values: `todo`, `test`, `assert`, `done`, `n/a` (with reason).

Tests live in `challenge-2/tests/` (`test_dma_reqs.py`, `test_bus_protocol.py`,
`test_outstanding.py`, `test_cr_desc.py`; harness `dma_tb.py`). Suite result:
**53/53 PASS on fixed RTL (0.86 s sim); on baseline/rtl 16 FAIL** = every seeded
deviation D1–D5 + all CR-D2 tests (baseline lacks CR). Test names without file
prefix; `test_bus_protocol.` file holds the APB/AXI legality suite.

| REQ | Summary | RTL location | Test | Assertion | Status |
|---|---|---|---|---|---|
| REQ-01 | all fields reset 0 except ID, LAST_GRANT | all always_ff; engine last_q=3 | req01_reset_values; cr7 (DESC/DESC_IE reset) | a_rst_* reset-value checks; next-state def set | done |
| REQ-02 | reset anytime; after: reset vals, valids/dack/irq 0 | engine S_IDLE reset; chan/irq/gctrl resets | req02_reset_after_config; out01/02/03 (mid-transaction) | axi_sva a_req02_* (valids/dack zero first edge); a_rst_* | done |
| REQ-03 | pready=1 always | dma_apb.sv:35 | req03_pready_always_ready + mem.run global checker in all tests | a_req03_pready_const | done |
| REQ-04 | transfer only in access phase | dma_apb.sv:27 (`access`) | req04_setup_and_psel0_no_effect | a_req04_reg_wr | done |
| REQ-05 | decode paddr[11:2], [1:0] ignored | dma_apb.sv:28 | req05_paddr_low_bits_ignored | a_decode_* (apb golden decode) | done |
| REQ-06 | prdata valid in read access phase, else 0; undef bits 0 | dma_ctrl.sv:193–218 | req06_prdata_and_undef_bits | a_req06_prdata_zero; ctrl_sva read-mux defs | done |
| REQ-07 | unmapped → pslverr=1, no effect, read 0; 0x118/0x11C+n unmapped | dma_apb.sv:30–36 | req07_unmapped_accesses | a_req07_pslverr | done |
| REQ-08 | pstrb byte-lane writes incl. IRQ_STATUS, CH_CTRL EN bit | dma_apb.sv:38; chan wmask; irq wmask | req08_pstrb_byte_lanes; req33; cr4 | a_req08_wmask; a_en_wmask_def | done |
| REQ-09 | RO write: pslverr=0, no effect; undef bits ignore writes | ctrl mux (no wr path for ID/ACTIVE/GRANT/STAT) | req09_ro_write_and_undef_write; RO: no-write-path defs (ctrl_sva) | ctrl_sva write-strobe decode defs (RO words have no write path) | done |
| REQ-10 | EN 0→1 starts, clears ERR/DONE, other CTRL bits same write | dma_chan.sv:82–91, 107 | req10_en_start_clears_and_takes_bits | a_req10_restart | done |
| REQ-11 | while EN=1: SRC/DST/LEN/NEXT/CTRL[6:1] writes no effect | dma_chan.sv:107,119–147 (`!en_q` gates) | req11_locked_while_enabled | a_req11_cfg/src/dst/len/next | done |
| REQ-12 | word read at CH_SRC, write at CH_DST, wstrb=F, aligned | engine S_RD_ADDR/S_WR; araddr/awaddr :99,102 | req12_word_transfer_addresses; req12_wstrb_and_alignment | a_req12_ar_aligned/aw_aligned/wstrb; a_araddr_def/a_awaddr_def | done |
| REQ-13 | one outstanding; arvalid/awvalid never both 1; AW/W either order | engine states; S_WR :196–206 | req13_one_outstanding_log_order; req13_aw_w_either_order; req13_awvalid_arvalid_exclusive; out01/02 | a_req13_no_ar_aw; state defs | done |
| REQ-14 | valid+payload hold while ready=0 | engine outputs state-gated | req14_valid_hold_under_stalls; out03; _stable checker in all tests | a_req14_ar_hold/aw_hold/w_hold | done |
| REQ-15 | after successful word: LEN−1, src/dst +4 unless FIX | dma_chan.sv:119–141 (word_done) | req15_len_and_address_update (frozen-bus mid-read) | a_len_def/a_src_def/a_dst_def (next-state defs) | done |
| REQ-16 | burst ≤ 2^BURST words; keep engine unless desc ends/error/abort | engine words_q, burst_words :74; :212–220 | req16_burst_bounds (dack counts per burst); req25 | a_burst_def; a_burst_words_def | done |
| REQ-17 | descriptor ends when CH_LEN reaches 0 | engine :214 (`ch_len <= 16'd1`) | req17_descriptor_ends_at_len_zero; req12; state next defs (S_END on len<=1) | a_entry_end (S_END entered exactly at len==1 completion) | done |
| REQ-18 | LEN=0: ends at once at grant, NO data moved/access, LEN stays 0; HW_REQ+LEN=0 eligible w/o dreq | **D2 fixed** | req18_zero_length_descriptor (FAILS on baseline) | a_req18_len0_end; a_elig*_def | done |
| REQ-19 | word addresses wrap mod 2^32; LEN 0xFFFF = 65535 words | dma_chan 30-bit counters wrap | req19_address_wrap_and_max_len (wrap + LEN-0xFFFF abort arithmetic) | a_src_def/a_dst_def wrap defs | done |
| REQ-20 | end: LINK&NEXT≠0 → fetch; else EN clears + DONE set | engine S_END :131–139, :224–231 | req20_completion_variants; req21; cr1 (DONE only at chain end) | a_ev_done_def; state next defs | done |
| REQ-21 | fetch 4 words at CH_NEXT in order, holding engine | engine S_FD_ADDR/FD_DATA, m_araddr :99 | req21_fetch_address_order (exact log sequence) | a_araddr_def (fetch addr) | done |
| REQ-22 | fetched words replace SRC/DST/LEN/NEXT; [1:0] ignored | dma_chan load_* :121–147; engine :110–123 | req22_fetched_field_masks | a_load_*_def | done |
| REQ-23 | after fetch: release engine, compete again | engine S_RELEASE | req23_release_after_fetch (ch1 served between fetch and continuation); state defs (S_RELEASE) | a_entry_idle + S_RELEASE entry defs | done |
| REQ-24 | eligible+PRIO+rotating order after last granted; 0 after reset | engine :78–83; **D1 fixed** | req24_priority_wins; req24_rotation_after_reset; req24_rotation_alternates; req24_rotation_start_after_last_grant; req24_27_dreq_dack_handshake (3 rotation tests FAIL on baseline) | a_req24_grant_golden; a_req24_one_owner; a_elig*_def | done |
| REQ-25 | GEN=0 stops new grants; burst in progress finishes | eligible `gen` term | req25_gen_cleared_mid_burst | a_elig*_def (gen term) | done |
| REQ-26 | LAST_GRANT = most recent grant, 3 after reset | engine last_q :161,173 | req26_last_grant; req24_rotation_start_after_last_grant | a_last_grant_def; a_req26_last_after_rst/last_q_next | done |
| REQ-27 | dack[n] ONE pulse after a burst that moved ≥1 word ends | engine :136–144 → **D5 fixed** | req27_dack_single_pulse (3 chain bursts, width=1; FAILS on baseline); req16; handshake test | a_req27_dack_pulse; a_dack_def | done |
| REQ-28 | any non-OKAY rresp/bresp → stop, ERR, no DONE, chain ends | **D3 fixed (bresp)** | req28_read_resp_error_stops; req28_write_resp_error_stops; req28_fetch_error_stops; cr2 (bresp test FAILS on baseline) | a_ev_err_def; state defs (S_ERR) | done |
| REQ-29 | failed read: nothing written, SRC/DST/LEN keep values | engine S_RD_DATA → S_ERR ✓ | req28_read_resp_error_stops (register-value leg) | a_load_*_def + word gating defs | done |
| REQ-30 | failed write resp: SRC/DST/LEN keep values | **D3 fixed** | req28_write_resp_error_stops (register-value leg) | a_word_end_def; a_len_def/a_dst_def | done |
| REQ-31 | abort: last word finishes, no new word, EN clears, no DONE/ERR | chan abort_q; engine S_STOP → **D4 fixed** | req31_abort_midburst; req31_abort_stalled_midword (both FAIL on baseline); cr6 | a_req31_abort_clears_en/self_clear; a_abort_def | done |
| REQ-32 | status set in event cycle; CH_STAT DONE/ERR sticky until restart | dma_chan :93–99 | req32_status_sticky | a_req32_done_sticky/err_sticky/set_done/set_err | done |
| REQ-33 | IRQ_STATUS W1C with strobe | dma_irq.sv:20,31 | req33_w1c_bit_and_lane_granularity; cr4 | a_req33_w1c; a_clr_vec_def | done |
| REQ-34 | set beats clear same cycle | dma_irq.sv:31 | req34_set_beats_clear (bus-frozen cycle-precise race, write edge == event edge via mem.cycle stamps) | a_req34_set_wins | done |
| REQ-35 | irq = \ | (status&enable), function of regs only | dma_irq.sv:24 | a_req35_irq | done |
| REQ-36 | (CR-D2) DESC[n] per completed descriptor incl zero-length and chain-mid; same cycle as DONE if last; NOT on error/abort; W1C lane pstrb[1] | engine ev_desc; dma_irq set_vec; IRQ_MASK 0xFFF | cr1–cr7 (+ req08 IRQ_ENABLE lanes) — ALL 7 FAIL on baseline | a_req36_desc0..3; a_ev_desc_def | done(CR) |
| REQ-37 | sync design, rst_n async only, no latches, meets SDC | flow check (P stage) — no sim test | n/a |  | n/a (PPA stage enforces) |

## Notes
- Extract/refresh REQ list: `grep -ohE 'REQ-[0-9]+' challenge-2/spec.md | sort -uV`
- DEV rows = confirmed seeded deviations D1–D5, fixed in `rtl/`; the listed tests
  fail on `baseline/rtl` (16 baseline failures = D1×3, D2, D3, D4×2, D5, CR-D2×8).
- REQ-34 race harness: `race_start` freezes the bus with the B response pending;
  `race_fire` stamps (mem.cycle) the B accept, the irq rise (event edge) and the
  sampled APB write-access cycle; write update edge = stamp+1 (access phase ends
  at that edge) — race leg asserts stamp+1 == event edge, sanity leg clears.
- REQ-19 second leg runs LEN=0xFFFF with fixed addresses and aborts; asserts
  LEN == 65535 − moved-words exactly (full 65535-word completion kept out for
  mutation-timeout runtime budget).
- Assertions column is filled by the assertion-writer agent (separate pass).
