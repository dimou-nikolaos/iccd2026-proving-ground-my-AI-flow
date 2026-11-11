// engine_sva.sv — transfer-engine invariants of dma_engine
// (REQ-12/13/14 bus legality, REQ-16 burst bookkeeping, REQ-18 zero-length descriptor,
//  REQ-20/24/26/27 arbitration + completion events, REQ-28/30/31 error and abort paths).
//
// Bound to dma_engine with `(.*)`: every observer port is `input` and its name matches a
// signal in dma_engine's scope (ports or internal state) — flow-lessons L25.
//
// Two flavours:
//  * signal-definition checks: the combinational outputs of the engine are compared with
//    the spec-derived function of the state and inputs (kills any mutation of those
//    expressions in rtl/dma_engine.sv);
//  * FSM transition/entry checks: the state history must match the spec's transfer
//    sequence, including the exact entry conditions of S_END / S_RELEASE / S_STOP / S_ERR.
//
// Next-state style checks all use `$past()` on *every* input and carry a `$past(rst_n)`
// guard so they cannot fire on the first edge after an asynchronous reset.

/* verilator lint_off SYNCASYNCNET */

module engine_sva (
  input logic        clk,
  input logic        rst_n,
  // global config
  input logic        gen,
  input logic [1:0]  burst,
  // channel view
  input logic [3:0]  ch_active,
  input logic [3:0]  ch_abort,
  input logic [3:0]  ch_hw_req,
  input logic [3:0]  ch_link,
  input logic [1:0]  ch_prio [4],
  input logic [31:0] ch_src [4],
  input logic [31:0] ch_dst [4],
  input logic [15:0] ch_len [4],
  input logic [31:0] ch_next [4],
  input logic [3:0]  dreq,
  input logic [3:0]  dack,
  // channel updates
  input logic [3:0]  owned,
  input logic [3:0]  word_done,
  input logic [3:0]  load_src,
  input logic [3:0]  load_dst,
  input logic [3:0]  load_len,
  input logic [3:0]  load_next,
  input logic [31:0] load_data,
  input logic [3:0]  stop,
  input logic [3:0]  ev_done,
  input logic [3:0]  ev_err,
  input logic [3:0]  ev_desc,
  input logic [1:0]  last_grant,
  // AXI4-Lite master
  input logic        m_awvalid,
  input logic        m_awready,
  input logic [31:0] m_awaddr,
  input logic        m_wvalid,
  input logic        m_wready,
  input logic [31:0] m_wdata,
  input logic [3:0]  m_wstrb,
  input logic        m_bvalid,
  input logic        m_bready,
  input logic [1:0]  m_bresp,
  input logic        m_arvalid,
  input logic        m_arready,
  input logic [31:0] m_araddr,
  input logic        m_rvalid,
  input logic        m_rready,
  input logic [31:0] m_rdata,
  input logic [1:0]  m_rresp,
  // internal state (bind into dma_engine)
  input logic [3:0]  state_q,
  input logic [1:0]  own_q,
  input logic [1:0]  last_q,
  input logic [3:0]  words_q,
  input logic [1:0]  fd_idx_q,
  input logic [31:0] data_q,
  input logic        aw_done_q,
  input logic        w_done_q,
  input logic        moved_q,
  input logic [3:0]  eligible,
  input logic        grant_valid,
  input logic [1:0]  grant_idx,
  input logic [3:0]  own_mask,
  input logic        word_end,
  input logic [3:0]  burst_words
);

  default clocking cb @(posedge clk); endclocking
  default disable iff (!rst_n);

  localparam logic [3:0] S_IDLE = 4'd0, S_RD_ADDR = 4'd1, S_RD_DATA = 4'd2,
                         S_WR = 4'd3, S_WR_RESP = 4'd4, S_END = 4'd5,
                         S_FD_ADDR = 4'd6, S_FD_DATA = 4'd7, S_RELEASE = 4'd8,
                         S_ERR = 4'd9, S_STOP = 4'd10;

  logic rd_ok;   // a successful read handshake is happening this cycle
  logic wr_ok;   // a successful write-response handshake this cycle
  assign rd_ok = m_rvalid && (m_rresp == 2'b00);
  assign wr_ok = m_bvalid && (m_bresp == 2'b00);
  // the descriptor currently served continues as a chain (REQ-20)
  logic chain;
  assign chain = ch_link[own_q] && (ch_next[own_q] != 32'd0);

  // ---------------------------------------------------------------------------
  // State encoding legality
  // ---------------------------------------------------------------------------
  a_state_legal: assert property (state_q <= 4'd10)
    else $error("engine: state_q=%0d is not a legal encoding", state_q);

  // ---------------------------------------------------------------------------
  // Signal definitions: ownership and arbiter view (REQ-24)
  // ---------------------------------------------------------------------------
  a_own_mask_def: assert property (own_mask == (4'b0001 << own_q))
    else $error("engine: own_mask=%b != 1<<own_q(%0d)", own_mask, own_q);
  a_owned_def: assert property (owned == ((state_q == S_IDLE) ? 4'b0000 : own_mask))
    else $error("REQ-24: owned=%b while state=%0d own_mask=%b", owned, state_q, own_mask);
  a_burst_words_def: assert property (burst_words == (4'd1 << burst))
    else $error("REQ-16: burst_words=%b != 2^BURST(%0d)", burst_words, burst);
  a_last_grant_def: assert property (last_grant == last_q)
    else $error("REQ-26: LAST_GRANT reads %0d but last_q=%0d", last_grant, last_q);

  // REQ-24 eligibility: GEN and EN set, not being aborted, and (if HW_REQ) dreq or LEN==0.
  a_elig0_def: assert property (
      eligible[0] == (gen && ch_active[0] && !ch_abort[0] &&
                      (!ch_hw_req[0] || dreq[0] || (ch_len[0] == 16'd0))))
    else $error("REQ-24: eligible[0] wrong");
  a_elig1_def: assert property (
      eligible[1] == (gen && ch_active[1] && !ch_abort[1] &&
                      (!ch_hw_req[1] || dreq[1] || (ch_len[1] == 16'd0))))
    else $error("REQ-24: eligible[1] wrong");
  a_elig2_def: assert property (
      eligible[2] == (gen && ch_active[2] && !ch_abort[2] &&
                      (!ch_hw_req[2] || dreq[2] || (ch_len[2] == 16'd0))))
    else $error("REQ-24: eligible[2] wrong");
  a_elig3_def: assert property (
      eligible[3] == (gen && ch_active[3] && !ch_abort[3] &&
                      (!ch_hw_req[3] || dreq[3] || (ch_len[3] == 16'd0))))
    else $error("REQ-24: eligible[3] wrong");

  // REQ-24 golden selection: highest PRIO wins, ties go to the first channel found in
  // rotating order starting after the channel granted most recently.
  logic        gv_ref;
  logic [1:0]  gi_ref;
  logic [2:0]  bp_ref;
  always_comb begin
    logic [1:0] cand;
    logic [1:0] start_ref;
    gv_ref = 1'b0;
    gi_ref = 2'd0;
    bp_ref = 3'd0;
    start_ref = last_q + 2'd1;
    for (int i = 0; i < 4; i++) begin
      cand = start_ref + 2'(i);
      if (eligible[cand] && (!gv_ref || ({1'b0, ch_prio[cand]} > bp_ref))) begin
        gv_ref = 1'b1;
        gi_ref = cand;
        bp_ref = {1'b0, ch_prio[cand]};
      end
    end
  end
  a_req24_grant_golden: assert property (
      (grant_valid == gv_ref) && (!grant_valid || (grant_idx == gi_ref)))
    else $error("REQ-24: grant (%b,%0d) != rotating-priority model (%b,%0d)",
                grant_valid, grant_idx, gv_ref, gi_ref);

  // REQ-24: the engine serves one channel at a time.
  a_req24_one_owner: assert property ($onehot0(owned) && $onehot0(word_done) &&
                                     $onehot0(stop) && $onehot0(ev_done) &&
                                     $onehot0(ev_err) && $onehot0(ev_desc) &&
                                     $onehot0(dack))
    else $error("REQ-24: multiple channels served/signalled at once");

  // ---------------------------------------------------------------------------
  // Signal definitions: AXI master outputs as a function of the state (REQ-12/13)
  // ---------------------------------------------------------------------------
  a_arvalid_def: assert property (
      m_arvalid == ((state_q == S_RD_ADDR) || (state_q == S_FD_ADDR)))
    else $error("engine: m_arvalid=%b inconsistent with state=%0d", m_arvalid, state_q);
  a_rready_def: assert property (
      m_rready == ((state_q == S_RD_DATA) || (state_q == S_FD_DATA)))
    else $error("engine: m_rready=%b inconsistent with state=%0d", m_rready, state_q);
  a_awvalid_def: assert property (
      m_awvalid == ((state_q == S_WR) && !aw_done_q))
    else $error("REQ-13/29: m_awvalid=%b inconsistent with state=%0d aw_done_q=%b",
                m_awvalid, state_q, aw_done_q);
  a_wvalid_def: assert property (
      m_wvalid == ((state_q == S_WR) && !w_done_q))
    else $error("REQ-13: m_wvalid=%b inconsistent with state=%0d w_done_q=%b",
                m_wvalid, state_q, w_done_q);
  a_bready_def: assert property (m_bready == (state_q == S_WR_RESP))
    else $error("REQ-13: m_bready=%b while state=%0d", m_bready, state_q);
  a_wstrb_def: assert property (m_wstrb == 4'hF)
    else $error("REQ-12: m_wstrb=%b is not 1111", m_wstrb);
  a_wdata_def: assert property (m_wdata == data_q)
    else $error("REQ-12: m_wdata=%h does not hold the word read from the source (%h)",
                m_wdata, data_q);
  a_load_data_def: assert property (load_data == m_rdata)
    else $error("REQ-22: load_data=%h != m_rdata=%h", load_data, m_rdata);
  // REQ-12/REQ-21: read address is CH_SRC, or CH_NEXT plus the descriptor word offset.
  a_araddr_def: assert property (
      m_araddr == ((state_q == S_FD_ADDR)
                   ? (ch_next[own_q] + {28'd0, fd_idx_q, 2'b00})
                   : ch_src[own_q]))
    else $error("REQ-12/21: m_araddr=%h wrong (state=%0d fd_idx=%0d src=%h next=%h)",
                m_araddr, state_q, fd_idx_q, ch_src[own_q], ch_next[own_q]);
  a_awaddr_def: assert property (m_awaddr == ch_dst[own_q])
    else $error("REQ-12: m_awaddr=%h != CH_DST(%h) of owner %0d", m_awaddr, ch_dst[own_q], own_q);

  // REQ-21/22: one descriptor word per read, in order SRC, DST, LEN, NEXT.
  a_load_src_def: assert property (
      load_src == (((state_q == S_FD_DATA) && rd_ok && (fd_idx_q == 2'd0)) ? own_mask : 4'b0000))
    else $error("REQ-21: load_src=%b (state=%0d rvalid=%b fd_idx=%0d)",
                load_src, state_q, m_rvalid, fd_idx_q);
  a_load_dst_def: assert property (
      load_dst == (((state_q == S_FD_DATA) && rd_ok && (fd_idx_q == 2'd1)) ? own_mask : 4'b0000))
    else $error("REQ-21: load_dst=%b (state=%0d rvalid=%b fd_idx=%0d)",
                load_dst, state_q, m_rvalid, fd_idx_q);
  a_load_len_def: assert property (
      load_len == (((state_q == S_FD_DATA) && rd_ok && (fd_idx_q == 2'd2)) ? own_mask : 4'b0000))
    else $error("REQ-21: load_len=%b (state=%0d rvalid=%b fd_idx=%0d)",
                load_len, state_q, m_rvalid, fd_idx_q);
  a_load_next_def: assert property (
      load_next == (((state_q == S_FD_DATA) && rd_ok && (fd_idx_q == 2'd3)) ? own_mask : 4'b0000))
    else $error("REQ-21: load_next=%b (state=%0d rvalid=%b fd_idx=%0d)",
                load_next, state_q, m_rvalid, fd_idx_q);

  // REQ-15/17: a word completes exactly on an OKAY write response, for the owner only.
  a_word_end_def: assert property (
      word_end == ((state_q == S_WR_RESP) && wr_ok))
    else $error("REQ-15/28: word_end=%b but state=%0d bvalid=%b bresp=%b",
                word_end, state_q, m_bvalid, m_bresp);
  a_word_done_def: assert property (word_done == (word_end ? own_mask : 4'b0000))
    else $error("REQ-15: word_done=%b while word_end=%b own_mask=%b",
                word_done, word_end, own_mask);

  // REQ-20/28/31/36 + CR-D2: the completion, error and descriptor events.
  a_ev_desc_def: assert property (ev_desc == ((state_q == S_END) ? own_mask : 4'b0000))
    else $error("REQ-36: ev_desc=%b while state=%0d", ev_desc, state_q);
  a_ev_done_def: assert property (
      ev_done == (((state_q == S_END) && !chain) ? own_mask : 4'b0000))
    else $error("REQ-20/31: ev_done=%b while state=%0d chain=%b", ev_done, state_q, chain);
  a_ev_err_def: assert property (ev_err == ((state_q == S_ERR) ? own_mask : 4'b0000))
    else $error("REQ-28: ev_err=%b while state=%0d", ev_err, state_q);
  a_stop_def: assert property (
      stop == ((((state_q == S_END) && !chain) || (state_q == S_ERR) || (state_q == S_STOP))
               ? own_mask : 4'b0000))
    else $error("REQ-20/28/31: stop=%b while state=%0d chain=%b", stop, state_q, chain);
  // REQ-27: dack pulses once, when a burst that moved data ends (chain ends release instead).
  a_dack_def: assert property (
      dack == (((((state_q == S_END) && !chain) || (state_q == S_RELEASE))
                && ch_hw_req[own_q] && moved_q) ? own_mask : 4'b0000))
    else $error("REQ-27: dack=%b while state=%0d chain=%b hw_req=%b moved=%b",
                dack, state_q, chain, ch_hw_req[own_q], moved_q);
  a_req27_dack_pulse: assert property ((dack != 4'b0000) |=> (dack == 4'b0000))
    else $error("REQ-27: dack=%b held for more than one cycle", $past(dack));

  // ---------------------------------------------------------------------------
  // FSM entry legality (REQ-13/16/17/18/20/21/23/28/31): whenever the engine *changes*
  // state, the condition of that change must be the one the spec's transfer sequence
  // requires. Only changes are constrained, so cycles spent waiting for a bus handshake
  // (state held) are unconstrained by construction.
  // ---------------------------------------------------------------------------
  a_entry_idle: assert property (
      (state_q == S_IDLE && $past(state_q) != S_IDLE) |->
      (($past(state_q) == S_END && !$past(chain)) || $past(state_q) == S_RELEASE ||
       $past(state_q) == S_ERR || $past(state_q) == S_STOP))
    else $error("engine: entered S_IDLE from state %0d", $past(state_q));

  // REQ-18/16/21/23: entry into S_RD_ADDR — on grant of a nonzero-length descriptor, after
  // a completed word with words left in the burst, or to fetch the next descriptor word.
  a_entry_rd_addr: assert property (
      (state_q == S_RD_ADDR && $past(state_q) != S_RD_ADDR) |->
      (($past(state_q) == S_IDLE && $past(grant_valid) && $past(ch_len[grant_idx]) != 16'd0) ||
       ($past(state_q) == S_WR_RESP && $past(wr_ok) && !$past(ch_abort[own_q]) &&
        $past(ch_len[own_q]) > 16'd1 && $past(words_q) != 4'd1) ||
       ($past(state_q) == S_FD_DATA && $past(rd_ok) && $past(fd_idx_q) != 2'd3)))
    else $error("REQ-18/16/21: illegal entry into S_RD_ADDR from state %0d", $past(state_q));

  // REQ-13: the read address is accepted before the engine waits for read data.
  a_entry_rd_data: assert property (
      (state_q == S_RD_DATA && $past(state_q) != S_RD_DATA) |->
      ($past(state_q) == S_RD_ADDR && $past(m_arready)))
    else $error("REQ-13: entered S_RD_DATA from state %0d without an accepted AR",
                $past(state_q));

  // REQ-13: a write starts only after the read data has been accepted (OKAY response).
  a_entry_wr: assert property (
      (state_q == S_WR && $past(state_q) != S_WR) |->
      ($past(state_q) == S_RD_DATA && $past(m_rvalid) && $past(m_rresp) == 2'b00))
    else $error("REQ-13: entered S_WR from state %0d (a read must complete first)",
                $past(state_q));

  // REQ-13: the write response is awaited only after AW and W have both been accepted.
  a_entry_wr_resp: assert property (
      (state_q == S_WR_RESP && $past(state_q) != S_WR_RESP) |->
      ($past(state_q) == S_WR && ($past(aw_done_q) || $past(m_awready)) &&
       ($past(w_done_q) || $past(m_wready))))
    else $error("REQ-13: entered S_WR_RESP from state %0d before AW/W completed",
                $past(state_q));

  // REQ-17/18/20: entry into S_END — a zero-length descriptor at grant, or a word that
  // brought CH_LEN to 0.
  a_entry_end: assert property (
      (state_q == S_END && $past(state_q) != S_END) |->
      (($past(state_q) == S_IDLE && $past(grant_valid) && $past(ch_len[grant_idx]) == 16'd0) ||
       ($past(state_q) == S_WR_RESP && $past(wr_ok) && !$past(ch_abort[own_q]) &&
        $past(ch_len[own_q]) <= 16'd1)))
    else $error("REQ-17/18: illegal entry into S_END from state %0d (len=%h)",
                $past(state_q), $past(ch_len[own_q]));

  // REQ-21: the descriptor fetch walks one word per accepted read.
  a_entry_fd_addr: assert property (
      (state_q == S_FD_ADDR && $past(state_q) != S_FD_ADDR) |->
      (($past(state_q) == S_END && $past(chain)) ||
       ($past(state_q) == S_FD_DATA && $past(rd_ok) && $past(fd_idx_q) != 2'd3)))
    else $error("REQ-20/21: illegal entry into S_FD_ADDR from state %0d (chain=%b)",
                $past(state_q), $past(chain));
  a_entry_fd_data: assert property (
      (state_q == S_FD_DATA && $past(state_q) != S_FD_DATA) |->
      ($past(state_q) == S_FD_ADDR && $past(m_arready)))
    else $error("REQ-21: entered S_FD_DATA from state %0d without an accepted AR",
                $past(state_q));

  // REQ-16/23: entry into S_RELEASE — burst word budget used up, or a descriptor fetched.
  a_entry_release: assert property (
      (state_q == S_RELEASE && $past(state_q) != S_RELEASE) |->
      (($past(state_q) == S_WR_RESP && $past(wr_ok) && !$past(ch_abort[own_q]) &&
        $past(ch_len[own_q]) > 16'd1 && $past(words_q) == 4'd1) ||
       ($past(state_q) == S_FD_DATA && $past(rd_ok) && $past(fd_idx_q) == 2'd3)))
    else $error("REQ-16/23: illegal entry into S_RELEASE from state %0d (words=%0d len=%h)",
                $past(state_q), $past(words_q), $past(ch_len[own_q]));

  // REQ-31: entry into S_STOP only from a completed word of an aborted channel.
  a_entry_stop: assert property (
      (state_q == S_STOP && $past(state_q) != S_STOP) |->
      ($past(state_q) == S_WR_RESP && $past(wr_ok) && $past(ch_abort[own_q])))
    else $error("REQ-31: illegal entry into S_STOP from state %0d (abort=%b)",
                $past(state_q), $past(ch_abort[own_q]));

  // REQ-28: entry into S_ERR only on a non-OKAY read response or write response.
  a_entry_err: assert property (
      (state_q == S_ERR && $past(state_q) != S_ERR) |->
      ((($past(state_q) == S_RD_DATA) || ($past(state_q) == S_FD_DATA)) && $past(m_rvalid) &&
        $past(m_rresp) != 2'b00) ||
       ($past(state_q) == S_WR_RESP && $past(m_bvalid) && $past(m_bresp) != 2'b00))
    else $error("REQ-28: illegal entry into S_ERR from state %0d (rresp=%b bresp=%b)",
                $past(state_q), $past(m_rresp), $past(m_bresp));

  // REQ-18: a zero-length descriptor ends at once on grant, without any data access.
  a_req18_len0_end: assert property (
      ($past(state_q) == S_IDLE && $past(grant_valid) && $past(ch_len[grant_idx]) == 16'd0)
      |-> (state_q == S_END))
    else $error("REQ-18: grant of a LEN=0 descriptor did not go straight to S_END");

  // ---------------------------------------------------------------------------
  // Next-state bookkeeping identities (REQ-15/16/24/26)
  // ---------------------------------------------------------------------------
  // REQ-24/26: the owner and LAST_GRANT change only at a grant from S_IDLE.
  a_own_q_next: assert property ($past(rst_n) && ($past(state_q) == S_IDLE) |->
      own_q == ($past(grant_valid) ? $past(grant_idx) : $past(own_q)))
    else $error("REQ-24: own_q=%0d inconsistent (prev state S_IDLE, grant=%b, prev own=%0d)",
                own_q, $past(grant_valid), $past(own_q));
  a_req26_last_q_next: assert property ($past(rst_n) && ($past(state_q) == S_IDLE) |->
      last_q == ($past(grant_valid) ? $past(grant_idx) : $past(last_q)))
    else $error("REQ-26: last_q=%0d inconsistent (grant=%b, grant_idx=%0d, prev=%0d)",
                last_q, $past(grant_valid), $past(grant_idx), $past(last_q));
  // while a channel is served, ownership does not change (REQ-24)
  a_own_hold: assert property ($past(rst_n) && ($past(state_q) != S_IDLE) |->
      own_q == $past(own_q))
    else $error("REQ-24: owner changed from %0d to %0d mid-service", $past(own_q), own_q);

  // REQ-16: the burst counter is loaded at grant and decrements per completed word.
  a_words_q_next: assert property ($past(rst_n) |->
      words_q == (($past(state_q) == S_IDLE && $past(grant_valid)) ? $past(burst_words)
                  : ($past(word_end) ? ($past(words_q) - 4'd1) : $past(words_q))))
    else $error("REQ-16: words_q=%0d inconsistent (prev=%0d word_end=%b burst_words=%0d)",
                words_q, $past(words_q), $past(word_end), $past(burst_words));
  // REQ-27: "moved at least one word" is set by a completed word and cleared at grant.
  a_moved_q_next: assert property ($past(rst_n) |->
      moved_q == (($past(state_q) == S_IDLE && $past(grant_valid)) ? 1'b0
                  : ($past(word_end) ? 1'b1 : $past(moved_q))))
    else $error("REQ-27: moved_q=%b inconsistent (prev=%b word_end=%b)",
                moved_q, $past(moved_q), $past(word_end));

  // REQ-13: the AW/W handshakes of one word are tracked until both complete.
  a_aw_done_next: assert property ($past(rst_n) |->
      aw_done_q == (($past(state_q) == S_RD_DATA && $past(rd_ok)) ? 1'b0
                    : (($past(state_q) == S_WR && $past(m_awvalid) && $past(m_awready))
                       ? 1'b1 : $past(aw_done_q))))
    else $error("REQ-13: aw_done_q=%b inconsistent", aw_done_q);
  a_w_done_next: assert property ($past(rst_n) |->
      w_done_q == (($past(state_q) == S_RD_DATA && $past(rd_ok)) ? 1'b0
                   : (($past(state_q) == S_WR && $past(m_wvalid) && $past(m_wready))
                      ? 1'b1 : $past(w_done_q))))
    else $error("REQ-13: w_done_q=%b inconsistent", w_done_q);
  // REQ-13: the word written is the one that was read.
  a_data_q_next: assert property ($past(rst_n) |->
      data_q == (($past(state_q) == S_RD_DATA && $past(rd_ok)) ? $past(m_rdata)
                 : $past(data_q)))
    else $error("REQ-13: data_q=%h does not hold the accepted read word %h",
                data_q, $past(m_rdata));
  // REQ-21/23: descriptor fetch index walks 0..3 and restarts per fetch.
  a_fd_idx_next: assert property ($past(rst_n) |->
      fd_idx_q == (($past(state_q) == S_END && $past(chain)) ? 2'd0
                   : (($past(state_q) == S_FD_DATA && $past(rd_ok) &&
                       $past(fd_idx_q) != 2'd3) ? ($past(fd_idx_q) + 2'd1)
                                                : $past(fd_idx_q))))
    else $error("REQ-21: fd_idx_q=%0d inconsistent (prev %0d, chain=%b)",
                fd_idx_q, $past(fd_idx_q), $past(chain));

endmodule

// Reset state of the engine (REQ-01/REQ-02/REQ-26: LAST_GRANT resets to 3).
// No `default disable iff` — these must be live while rst_n is low.
module engine_reset_sva (
  input logic        clk,
  input logic        rst_n,
  input logic [3:0]  state_q,
  input logic [1:0]  own_q,
  input logic [1:0]  last_q,
  input logic [3:0]  words_q,
  input logic [1:0]  fd_idx_q,
  input logic [31:0] data_q,
  input logic        aw_done_q,
  input logic        w_done_q,
  input logic        moved_q
);

  a_rst_state: assert property (@(posedge clk) !rst_n |-> state_q == 4'd0)
    else $error("REQ-02: engine state is not S_IDLE during reset (%0d)", state_q);
  a_rst_own: assert property (@(posedge clk) !rst_n |-> own_q == 2'd0)
    else $error("REQ-02: own_q != 0 during reset");
  a_rst_last: assert property (@(posedge clk) (!rst_n && $past(!rst_n)) |-> last_q == 2'd3)
    else $error("REQ-26: last_q != 3 after a reset clock edge (got %0d)", last_q);
  a_rst_words: assert property (@(posedge clk) !rst_n |-> words_q == 4'd0)
    else $error("REQ-16: words_q != 0 during reset");
  a_rst_fdidx: assert property (@(posedge clk) !rst_n |-> fd_idx_q == 2'd0)
    else $error("REQ-21: fd_idx_q != 0 during reset");
  a_rst_data: assert property (@(posedge clk) !rst_n |-> data_q == 32'd0)
    else $error("REQ-02: data_q != 0 during reset");
  a_rst_awdone: assert property (@(posedge clk) !rst_n |-> !aw_done_q)
    else $error("REQ-13: aw_done_q != 0 during reset");
  a_rst_wdone: assert property (@(posedge clk) !rst_n |-> !w_done_q)
    else $error("REQ-13: w_done_q != 0 during reset");
  a_rst_moved: assert property (@(posedge clk) !rst_n |-> !moved_q)
    else $error("REQ-27: moved_q != 0 during reset");
  // REQ-26: once reset is released, LAST_GRANT reads 3 (its reset value).
  a_req26_last_after_rst: assert property (@(posedge clk)
      (rst_n && $past(!rst_n)) |-> last_q == 2'd3)
    else $error("REQ-26: LAST_GRANT does not read 3 after reset release (%0d)", last_q);
endmodule

/* verilator lint_on SYNCASYNCNET */

bind dma_engine engine_sva u_engine_sva (.*);
bind dma_engine engine_reset_sva u_engine_reset_sva (.*);
