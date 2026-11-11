// chan_irq_sva.sv — per-channel register contract (dma_chan) and the interrupt
// registers (dma_irq): REQ-08 strobed writes, REQ-10/11 enable and write-scope rules,
// REQ-15 length bookkeeping, REQ-31 abort, REQ-32 sticky status, REQ-33/34/36 the
// W1C semantics of IRQ_STATUS including the CR-D2 DESC[11:8] bits, and REQ-35 irq.
//
// Bound with `(.*)` to dma_chan (all four generate instances) and to dma_irq.
// Every observer port is `input` and named exactly like the target signal (L25).

/* verilator lint_off SYNCASYNCNET */

module chan_sva (
  input logic        clk,
  input logic        rst_n,
  // register writes
  input logic        wr_ctrl,
  input logic        wr_src,
  input logic        wr_dst,
  input logic        wr_len,
  input logic        wr_next,
  input logic [31:0] wdata,
  input logic [31:0] wmask,
  // engine updates
  input logic        owned,
  input logic        word_done,
  input logic        load_src,
  input logic        load_dst,
  input logic        load_len,
  input logic        load_next,
  input logic [31:0] load_data,
  input logic        stop,
  input logic        set_err,
  input logic        set_done,
  // outputs
  input logic        active,
  input logic        abort,
  input logic        hw_req,
  input logic [1:0]  prio,
  input logic        link,
  input logic [31:0] src,
  input logic [31:0] dst,
  input logic [15:0] len,
  input logic [31:0] next,
  input logic [31:0] ctrl_rd,
  input logic [31:0] stat_rd,
  // internal state
  input logic        en_q,
  input logic        abort_q,
  input logic        err_q,
  input logic        done_q,
  input logic [5:0]  cfg_q,
  input logic [29:0] src_q,
  input logic [29:0] dst_q,
  input logic [15:0] len_q,
  input logic [29:0] next_q,
  input logic [31:0] ctrl_wr,
  input logic [31:0] src_m,
  input logic [31:0] dst_m,
  input logic [31:0] next_m,
  input logic        en_wr_visible
);

  default clocking cb @(posedge clk); endclocking
  default disable iff (!rst_n);

  // a write of EN=1 to a stopped channel restarts it (REQ-10)
  logic restart;
  assign restart = en_wr_visible && wdata[0] && !en_q;

  // ---------------------------------------------------------------------------
  // Output definitions (CH_CTRL / CH_STAT read-back, REQ-06/09 field placement)
  // ---------------------------------------------------------------------------
  a_active_def: assert property (active == en_q)
    else $error("REQ-20: ACTIVE output != EN");
  a_abort_def: assert property (abort == abort_q)
    else $error("REQ-31: abort output != abort_q");
  a_cfg_bits_def: assert property (
      hw_req == cfg_q[2] && prio == cfg_q[4:3] && link == cfg_q[5])
    else $error("REQ-06: CH_CTRL fields read wrong (hw_req=%b prio=%0d link=%b cfg=%b)",
                hw_req, prio, link, cfg_q);
  a_ctrl_rd_def: assert property (ctrl_rd == {25'h0, cfg_q, en_q})
    else $error("REQ-06: CH_CTRL read-back %h wrong (cfg=%b en=%b)", ctrl_rd, cfg_q, en_q);
  a_stat_rd_def: assert property (stat_rd == {29'h0, done_q, err_q, en_q})
    else $error("REQ-06: CH_STAT read-back %h wrong (done=%b err=%b en=%b)",
                stat_rd, done_q, err_q, en_q);

  // REQ-12/21: word-aligned address outputs; REQ-15 the length register.
  a_src_def: assert property (src == {src_q, 2'b00})
    else $error("REQ-12: CH_SRC read-back %h != {src_q,2'b00}", src);
  a_dst_def: assert property (dst == {dst_q, 2'b00})
    else $error("REQ-12: CH_DST read-back %h != {dst_q,2'b00}", dst);
  a_next_def: assert property (next == {next_q, 2'b00})
    else $error("REQ-21: CH_NEXT read-back %h != {next_q,2'b00}", next);
  a_len_def: assert property (len == len_q)
    else $error("REQ-15: CH_LEN read-back %h != len_q %h", len, len_q);

  // REQ-08: a write updates only the strobed byte lanes (masked write merge).
  a_ctrl_wr_def: assert property (ctrl_wr == ((ctrl_rd & ~wmask) | (wdata & wmask)))
    else $error("REQ-08: CH_CTRL merge %h wrong for wmask=%h wdata=%h", ctrl_wr, wmask, wdata);
  a_src_m_def: assert property (src_m == (({src_q, 2'b00} & ~wmask) | (wdata & wmask)))
    else $error("REQ-08: CH_SRC merge %h wrong for wmask=%h", src_m, wmask);
  a_dst_m_def: assert property (dst_m == (({dst_q, 2'b00} & ~wmask) | (wdata & wmask)))
    else $error("REQ-08: CH_DST merge %h wrong for wmask=%h", dst_m, wmask);
  a_next_m_def: assert property (next_m == (({next_q, 2'b00} & ~wmask) | (wdata & wmask)))
    else $error("REQ-08: CH_NEXT merge %h wrong for wmask=%h", next_m, wmask);
  // REQ-08: the EN bit is written through the byte lane 0 strobe only.
  a_en_wmask_def: assert property (en_wr_visible == (wr_ctrl && wmask[0]))
    else $error("REQ-08: EN write visibility %b wrong (wr_ctrl=%b wmask[0]=%b)",
                en_wr_visible, wr_ctrl, wmask[0]);

  // ---------------------------------------------------------------------------
  // Next-state identities (the always_ff bodies of dma_chan)
  // ---------------------------------------------------------------------------
  a_en_q_next: assert property ($past(rst_n) |->
      en_q == ($past(stop) ? 1'b0
               : $past(abort_q && !owned) ? 1'b0
               : $past(restart) ? 1'b1 : $past(en_q)))
    else $error("REQ-10/31: en_q=%b inconsistent (stop=%b abort_q=%b owned=%b restart=%b prev=%b)",
                en_q, $past(stop), $past(abort_q), $past(owned), $past(restart), $past(en_q));
  a_abort_q_next: assert property ($past(rst_n) |->
      abort_q == ($past(stop || (abort_q && !owned)) ? 1'b0
                  : $past(en_wr_visible && !wdata[0] && en_q) ? 1'b1 : $past(abort_q)))
    else $error("REQ-31: abort_q=%b inconsistent", abort_q);
  a_err_q_next: assert property ($past(rst_n) |->
      err_q == ($past(restart) ? 1'b0 : ($past(set_err) ? 1'b1 : $past(err_q))))
    else $error("REQ-10/28/32: err_q=%b inconsistent (restart=%b set_err=%b prev=%b)",
                err_q, $past(restart), $past(set_err), $past(err_q));
  a_done_q_next: assert property ($past(rst_n) |->
      done_q == ($past(restart) ? 1'b0 : ($past(set_done) ? 1'b1 : $past(done_q))))
    else $error("REQ-10/20/32: done_q=%b inconsistent (restart=%b set_done=%b prev=%b)",
                done_q, $past(restart), $past(set_done), $past(done_q));
  a_cfg_q_next: assert property ($past(rst_n) |->
      cfg_q == ($past(wr_ctrl && !en_q) ? $past(ctrl_wr[6:1]) : $past(cfg_q)))
    else $error("REQ-10/11: cfg_q=%b inconsistent (wr_ctrl=%b prev en_q=%b ctrl_wr=%h)",
                cfg_q, $past(wr_ctrl), $past(en_q), $past(ctrl_wr));
  a_src_q_next: assert property ($past(rst_n) |->
      src_q == ($past(wr_src && !en_q) ? $past(src_m[31:2])
               : $past(load_src) ? $past(load_data[31:2])
               : $past(word_done && !cfg_q[0]) ? ($past(src_q) + 30'd1) : $past(src_q)))
    else $error("REQ-15: src_q=%h inconsistent (wr_src=%b en_q=%b load_src=%b word_done=%b src_fix=%b)",
                src_q, $past(wr_src), $past(en_q), $past(load_src), $past(word_done), $past(cfg_q[0]));
  a_dst_q_next: assert property ($past(rst_n) |->
      dst_q == ($past(wr_dst && !en_q) ? $past(dst_m[31:2])
               : $past(load_dst) ? $past(load_data[31:2])
               : $past(word_done && !cfg_q[1]) ? ($past(dst_q) + 30'd1) : $past(dst_q)))
    else $error("REQ-15: dst_q=%h inconsistent (wr_dst=%b en_q=%b load_dst=%b word_done=%b dst_fix=%b)",
                dst_q, $past(wr_dst), $past(en_q), $past(load_dst), $past(word_done), $past(cfg_q[1]));
  a_len_q_next: assert property ($past(rst_n) |->
      len_q == ($past(wr_len && !en_q)
                ? (($past(len_q) & ~$past(wmask[15:0])) | ($past(wdata[15:0]) & $past(wmask[15:0])))
                : $past(load_len) ? $past(load_data[15:0])
                : $past(word_done) ? ($past(len_q) - 16'd1) : $past(len_q)))
    else $error("REQ-15: len_q=%h inconsistent (wr_len=%b load_len=%b word_done=%b wmask=%h)",
                len_q, $past(wr_len), $past(load_len), $past(word_done), $past(wmask));
  a_next_q_next: assert property ($past(rst_n) |->
      next_q == ($past(wr_next && !en_q) ? $past(next_m[31:2])
                 : $past(load_next) ? $past(load_data[31:2]) : $past(next_q)))
    else $error("REQ-22: next_q=%h inconsistent (wr_next=%b load_next=%b)",
                next_q, $past(wr_next), $past(load_next));

  // ---------------------------------------------------------------------------
  // Spec-level invariants
  // ---------------------------------------------------------------------------
  // REQ-10: writing EN=1 to a stopped channel starts it and clears ERR and DONE.
  a_req10_restart: assert property (restart |=> (en_q && !err_q && !done_q))
    else $error("REQ-10: restart did not set EN and clear ERR/DONE");

  // REQ-11: while EN=1, writes to CH_SRC/CH_DST/CH_LEN/CH_NEXT and CH_CTRL[6:1] are ignored.
  a_req11_src: assert property ((en_q && wr_src && !owned) |=> src_q == $past(src_q))
    else $error("REQ-11: CH_SRC changed by a write while EN=1 (%h -> %h)", $past(src_q), src_q);
  a_req11_dst: assert property ((en_q && wr_dst && !owned) |=> dst_q == $past(dst_q))
    else $error("REQ-11: CH_DST changed by a write while EN=1 (%h -> %h)", $past(dst_q), dst_q);
  a_req11_len: assert property ((en_q && wr_len && !owned) |=> len_q == $past(len_q))
    else $error("REQ-11: CH_LEN changed by a write while EN=1 (%h -> %h)", $past(len_q), len_q);
  a_req11_next: assert property ((en_q && wr_next && !owned) |=> next_q == $past(next_q))
    else $error("REQ-11: CH_NEXT changed by a write while EN=1 (%h -> %h)", $past(next_q), next_q);
  a_req11_cfg: assert property ((en_q && wr_ctrl && !owned) |=> cfg_q == $past(cfg_q))
    else $error("REQ-11: CH_CTRL[6:1] changed by a write while EN=1 (%b -> %b)",
                $past(cfg_q), cfg_q);

  // REQ-32: status bits are set in the cycle the event occurs.
  a_req32_set_done: assert property (set_done |=> done_q)
    else $error("REQ-32: DONE not set by the event cycle");
  a_req32_set_err: assert property (set_err |=> err_q)
    else $error("REQ-28/32: ERR not set by the event cycle");
  // the event pulses only address an enabled channel
  a_set_needs_en: assert property (((set_done || set_err) |-> en_q))
    else $error("REQ-32: status event for a channel that is not enabled");

  // REQ-32: DONE and ERR stay set until the channel is started again.
  a_req32_done_sticky: assert property (done_q && !restart |=> done_q)
    else $error("REQ-32: CH_STAT.DONE cleared without a restart");
  a_req32_err_sticky: assert property (err_q && !restart |=> err_q)
    else $error("REQ-32: CH_STAT.ERR cleared without a restart");

  // REQ-31: an abort request clears EN once the engine releases the channel.
  a_req31_abort_clears_en: assert property ((abort_q && !owned) |=> !en_q)
    else $error("REQ-31: EN still set after the abort was granted (%0d cycles later)", $past(abort_q));
  a_req31_abort_self_clear: assert property ((abort_q && !owned) |=> !abort_q)
    else $error("REQ-31: abort request flag not cleared");

endmodule

// Reset state of one channel (REQ-01).
module chan_reset_sva (
  input logic        clk,
  input logic        rst_n,
  input logic        en_q,
  input logic        abort_q,
  input logic        err_q,
  input logic        done_q,
  input logic [5:0]  cfg_q,
  input logic [29:0] src_q,
  input logic [29:0] dst_q,
  input logic [15:0] len_q,
  input logic [29:0] next_q
);

  a_rst_en: assert property (@(posedge clk) !rst_n |-> !en_q)
    else $error("REQ-01/02: channel EN is set during reset");
  a_rst_abort: assert property (@(posedge clk) !rst_n |-> !abort_q)
    else $error("REQ-01: abort flag set during reset");
  a_rst_err: assert property (@(posedge clk) !rst_n |-> !err_q)
    else $error("REQ-01: CH_STAT.ERR set during reset");
  a_rst_done: assert property (@(posedge clk) !rst_n |-> !done_q)
    else $error("REQ-01: CH_STAT.DONE set during reset");
  a_rst_cfg: assert property (@(posedge clk) !rst_n |-> cfg_q == 6'd0)
    else $error("REQ-01: CH_CTRL fields nonzero during reset (%b)", cfg_q);
  a_rst_src: assert property (@(posedge clk) !rst_n |-> src_q == 30'd0)
    else $error("REQ-01: CH_SRC nonzero during reset");
  a_rst_dst: assert property (@(posedge clk) !rst_n |-> dst_q == 30'd0)
    else $error("REQ-01: CH_DST nonzero during reset");
  a_rst_len: assert property (@(posedge clk) !rst_n |-> len_q == 16'd0)
    else $error("REQ-01: CH_LEN nonzero during reset");
  a_rst_next: assert property (@(posedge clk) !rst_n |-> next_q == 30'd0)
    else $error("REQ-01: CH_NEXT nonzero during reset");
endmodule

// -----------------------------------------------------------------------------
// dma_irq: IRQ_STATUS (W1C + events) and IRQ_ENABLE (REQ-33/34/35, CR-D2 REQ-36)
// -----------------------------------------------------------------------------
module irq_sva (
  input logic        clk,
  input logic        rst_n,
  input logic        wr_status,
  input logic        wr_enable,
  input logic [31:0] wdata,
  input logic [31:0] wmask,
  input logic [3:0]  ev_done,
  input logic [3:0]  ev_err,
  input logic [3:0]  ev_desc,
  input logic [31:0] status_rd,
  input logic [31:0] enable_rd,
  input logic        irq,
  // internal state
  input logic [31:0] status_q,
  input logic [31:0] enable_q,
  input logic [31:0] set_vec,
  input logic [31:0] clr_vec
);

  default clocking cb @(posedge clk); endclocking
  default disable iff (!rst_n);

  localparam logic [31:0] IRQ_IMPL = 32'h0000_0FFF;  // DONE|ERR|DESC bits (CR-D2)

  // REQ-33/34/36 + CR-D2: the set vector is {DESC[3:0], ERR[3:0], DONE[3:0]}.
  a_set_vec_def: assert property (set_vec == {20'h0, ev_desc, ev_err, ev_done})
    else $error("CR-D2: set_vec=%h does not match {DESC,ERR,DONE} = {%h,%h,%h}",
                set_vec, ev_desc, ev_err, ev_done);
  // REQ-33 + REQ-08: the clear vector is the strobed write data of an IRQ_STATUS write.
  a_clr_vec_def: assert property (clr_vec == (wr_status ? (wdata & wmask) : 32'h0))
    else $error("REQ-33: clr_vec=%h wrong (wr_status=%b wdata=%h wmask=%h)",
                clr_vec, wr_status, wdata, wmask);
  a_status_rd_def: assert property (status_rd == status_q)
    else $error("REQ-06: IRQ_STATUS read-back %h != status_q %h", status_rd, status_q);
  a_enable_rd_def: assert property (enable_rd == enable_q)
    else $error("REQ-06: IRQ_ENABLE read-back %h != enable_q %h", enable_rd, enable_q);

  // REQ-35: irq is a function of the register state only.
  a_req35_irq: assert property (irq == |(status_q & enable_q))
    else $error("REQ-35: irq=%b but status&enable=%h", irq, status_q & enable_q);

  // Only bits [11:0] exist (section 5 + CR-D2); the rest reads 0 and cannot be set.
  a_status_hi_zero: assert property (status_q[31:12] == 20'h0)
    else $error("REQ-09/CR-D2: IRQ_STATUS bits above [11:0] are set (%h)", status_q);
  a_enable_hi_zero: assert property (enable_q[31:12] == 20'h0)
    else $error("REQ-09/CR-D2: IRQ_ENABLE bits above [11:0] are set (%h)", enable_q);

  // Next-state identities of the two interrupt registers.
  a_status_q_next: assert property ($past(rst_n) |->
      status_q == (((($past(status_q) & ~$past(clr_vec)) | $past(set_vec)) & IRQ_IMPL)))
    else $error("REQ-33/34: status_q=%h inconsistent (prev=%h set=%h clr=%h)",
                status_q, $past(status_q), $past(set_vec), $past(clr_vec));
  a_enable_q_next: assert property ($past(rst_n) |->
      enable_q == ($past(wr_enable)
                   ? (((($past(enable_q) & ~$past(wmask)) | ($past(wdata) & $past(wmask)))
                       & IRQ_IMPL))
                   : $past(enable_q)))
    else $error("REQ-08: enable_q=%h inconsistent (wr_enable=%b wdata=%h wmask=%h)",
                enable_q, $past(wr_enable), $past(wdata), $past(wmask));

  // REQ-33: a strobed write-1 clears the bit; REQ-34: a concurrent set wins.
  a_req33_w1c: assert property ($past(rst_n) |->
      ((status_q & $past(clr_vec) & ~$past(set_vec)) == 32'h0))
    else $error("REQ-33: a written-1 status bit was not cleared (status=%h clr=%h)",
                status_q, $past(clr_vec));
  a_req34_set_wins: assert property ($past(rst_n) |->
      ((status_q & $past(set_vec)) == $past(set_vec)))
    else $error("REQ-34: a status bit set in the same cycle as a clear is not 1 (status=%h set=%h)",
                status_q, $past(set_vec));

  // CR-D2 REQ-36: DESC[n] is set once per completed descriptor of channel n.
  a_req36_desc0: assert property (ev_desc[0] |=> status_q[8])
    else $error("REQ-36: DESC[0] not set after a descriptor completion");
  a_req36_desc1: assert property (ev_desc[1] |=> status_q[9])
    else $error("REQ-36: DESC[1] not set after a descriptor completion");
  a_req36_desc2: assert property (ev_desc[2] |=> status_q[10])
    else $error("REQ-36: DESC[2] not set after a descriptor completion");
  a_req36_desc3: assert property (ev_desc[3] |=> status_q[11])
    else $error("REQ-36: DESC[3] not set after a descriptor completion");
  // REQ-20/28/31 + CR-D2: one kind of event per cycle and per channel — an error never
  // accompanies DONE, and an error or abort never accompanies DESC of the same channel.
  a_ev_done_err_disjoint: assert property (
      (ev_done & ev_err) == 4'b0000 && (ev_desc & ev_err) == 4'b0000)
    else $error("REQ-20/28/36: conflicting events in one cycle (done=%b err=%b desc=%b)",
                ev_done, ev_err, ev_desc);

endmodule

// Reset state of the interrupt registers (REQ-01/REQ-02).
module irq_reset_sva (
  input logic        clk,
  input logic        rst_n,
  input logic [31:0] status_q,
  input logic [31:0] enable_q,
  input logic        irq
);

  a_rst_status: assert property (@(posedge clk) !rst_n |-> status_q == 32'h0)
    else $error("REQ-01: IRQ_STATUS nonzero during reset (%h)", status_q);
  a_rst_enable: assert property (@(posedge clk) !rst_n |-> enable_q == 32'h0)
    else $error("REQ-01: IRQ_ENABLE nonzero during reset (%h)", enable_q);
  a_rst_irq: assert property (@(posedge clk) !rst_n |-> !irq)
    else $error("REQ-02: irq is 1 during reset");
endmodule

/* verilator lint_on SYNCASYNCNET */

bind dma_chan chan_sva u_chan_sva (.*);
bind dma_chan chan_reset_sva u_chan_reset_sva (.*);
bind dma_irq irq_sva u_irq_sva (.*);
bind dma_irq irq_reset_sva u_irq_reset_sva (.*);
