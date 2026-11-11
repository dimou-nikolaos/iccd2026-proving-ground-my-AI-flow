// ctrl_sva.sv — top-level glue of dma_ctrl: the prdata read mux (REQ-06), the GCTRL
// register (REQ-01/REQ-04/REQ-08/REQ-09) and the fields derived from it (REQ-16/REQ-24).
//
// Bound to dma_ctrl; every observed signal is a top-level port or a net in dma_ctrl scope.

/* verilator lint_off SYNCASYNCNET */

module ctrl_sva (
  input logic        clk,
  input logic        rst_n,
  // APB side
  input logic        psel,
  input logic        penable,
  input logic        pwrite,
  input logic [11:0] paddr,
  input logic [31:0] pwdata,
  input logic [3:0]  pstrb,
  input logic [31:0] prdata,
  // dma_apb decode outputs (nets in dma_ctrl scope)
  input logic        reg_wr,
  input logic [31:0] reg_wmask,
  input logic        glob_sel,
  input logic [9:0]  glob_word,
  input logic        chan_sel,
  input logic [1:0]  chan_idx,
  input logic [2:0]  chan_reg,
  // GCTRL and what is derived from it
  input logic [2:0]  gctrl_q,
  input logic        gen,
  input logic [1:0]  burst,
  // register read-back sources and the channel nets of dma_ctrl
  input logic [3:0]  ch_active,
  input logic [3:0]  ch_abort,
  input logic [3:0]  word_done,
  input logic [31:0] ch_src [4],
  input logic [31:0] ch_dst [4],
  input logic [15:0] ch_len [4],
  input logic [31:0] ch_next [4],
  input logic [31:0] ch_ctrl_rd [4],
  input logic [31:0] ch_stat_rd [4],
  input logic [1:0]  last_grant,
  input logic [31:0] irq_status_rd,
  input logic [31:0] irq_enable_rd
);

  default clocking cb @(posedge clk); endclocking
  default disable iff (!rst_n);

  localparam logic [9:0] W_ID = 10'h000, W_GCTRL = 10'h001, W_IRQ_ST = 10'h002,
                         W_IRQ_EN = 10'h003, W_ACTIVE = 10'h004, W_GRANT = 10'h005;
  localparam logic [2:0] R_CTRL = 3'd0, R_SRC = 3'd1, R_DST = 3'd2, R_LEN = 3'd3,
                         R_NEXT = 3'd4, R_STAT = 3'd5;

  // the access phase of a read
  logic rd_acc;
  assign rd_acc = psel && penable && !pwrite;
  // a write to GCTRL in its access phase
  logic wr_gctrl;
  assign wr_gctrl = reg_wr && glob_sel && (glob_word == W_GCTRL);

  // ---------------------------------------------------------------------------
  // REQ-06: in the access phase of a read, prdata carries the register value.
  // ---------------------------------------------------------------------------
  a_prdata_id: assert property (
      (rd_acc && glob_sel && glob_word == W_ID) |-> prdata == 32'h444D_0100)
    else $error("REQ-06: ID reads %h, expected 444D0100", prdata);
  a_prdata_gctrl: assert property (
      (rd_acc && glob_sel && glob_word == W_GCTRL) |-> prdata == {29'h0, gctrl_q})
    else $error("REQ-06: GCTRL reads %h but gctrl_q=%b", prdata, gctrl_q);
  a_prdata_irq_status: assert property (
      (rd_acc && glob_sel && glob_word == W_IRQ_ST) |-> prdata == irq_status_rd)
    else $error("REQ-06: IRQ_STATUS reads %h, register is %h", prdata, irq_status_rd);
  a_prdata_irq_enable: assert property (
      (rd_acc && glob_sel && glob_word == W_IRQ_EN) |-> prdata == irq_enable_rd)
    else $error("REQ-06: IRQ_ENABLE reads %h, register is %h", prdata, irq_enable_rd);
  a_prdata_active: assert property (
      (rd_acc && glob_sel && glob_word == W_ACTIVE) |-> prdata == {28'h0, ch_active})
    else $error("REQ-06: ACTIVE reads %h, channel EN bits are %b", prdata, ch_active);
  a_prdata_last_grant: assert property (
      (rd_acc && glob_sel && glob_word == W_GRANT) |-> prdata == {30'h0, last_grant})
    else $error("REQ-06/26: LAST_GRANT reads %h, register is %0d", prdata, last_grant);

  // REQ-06: bits not defined for a register read as 0.
  a_prdata_ctrl_hi: assert property (
      (rd_acc && chan_sel && chan_reg == R_CTRL) |-> prdata[31:7] == 25'h0)
    else $error("REQ-06/09: CH_CTRL read has bits above [6] set (%h)", prdata);
  a_prdata_stat_hi: assert property (
      (rd_acc && chan_sel && chan_reg == R_STAT) |-> prdata[31:3] == 29'h0)
    else $error("REQ-06/09: CH_STAT read has bits above [2] set (%h)", prdata);
  a_prdata_len_hi: assert property (
      (rd_acc && chan_sel && chan_reg == R_LEN) |-> prdata[31:16] == 16'h0)
    else $error("REQ-06/09: CH_LEN read has bits above [15] set (%h)", prdata);
  a_prdata_word_addr: assert property (
      (rd_acc && chan_sel && (chan_reg == R_SRC || chan_reg == R_DST || chan_reg == R_NEXT))
      |-> prdata[1:0] == 2'b00)
    else $error("REQ-06: CH_SRC/CH_DST/CH_NEXT read %h with [1:0] nonzero", prdata);

  // REQ-06: the channel-register read mux returns that channel's register value
  a_prdata_chan_ctrl: assert property (
      (rd_acc && chan_sel && chan_reg == R_CTRL) |-> prdata == ch_ctrl_rd[chan_idx])
    else $error("REQ-06: CH_CTRL(%0d) read returns %h, register holds %h", chan_idx, prdata, ch_ctrl_rd[chan_idx]);
  a_prdata_chan_src: assert property (
      (rd_acc && chan_sel && chan_reg == R_SRC) |-> prdata == ch_src[chan_idx])
    else $error("REQ-06: CH_SRC(%0d) read returns %h, register holds %h", chan_idx, prdata, ch_src[chan_idx]);
  a_prdata_chan_dst: assert property (
      (rd_acc && chan_sel && chan_reg == R_DST) |-> prdata == ch_dst[chan_idx])
    else $error("REQ-06: CH_DST(%0d) read returns %h, register holds %h", chan_idx, prdata, ch_dst[chan_idx]);
  a_prdata_chan_len: assert property (
      (rd_acc && chan_sel && chan_reg == R_LEN) |-> prdata == {16'h0, ch_len[chan_idx]})
    else $error("REQ-06: CH_LEN(%0d) read returns %h, register holds %h", chan_idx, prdata, ch_len[chan_idx]);
  a_prdata_chan_next: assert property (
      (rd_acc && chan_sel && chan_reg == R_NEXT) |-> prdata == ch_next[chan_idx])
    else $error("REQ-06: CH_NEXT(%0d) read returns %h, register holds %h", chan_idx, prdata, ch_next[chan_idx]);
  a_prdata_chan_stat: assert property (
      (rd_acc && chan_sel && chan_reg == R_STAT) |-> prdata == ch_stat_rd[chan_idx])
    else $error("REQ-06/10: CH_STAT(%0d) read returns %h, register holds %h", chan_idx, prdata, ch_stat_rd[chan_idx]);

  // ---------------------------------------------------------------------------
  // GCTRL register (REQ-01/04/08)
  // ---------------------------------------------------------------------------
  a_gen_def: assert property (gen == gctrl_q[0])
    else $error("REQ-24/25: GEN does not read GCTRL[0] (%b vs %b)", gen, gctrl_q[0]);
  a_burst_def: assert property (burst == gctrl_q[2:1])
    else $error("REQ-16: BURST does not read GCTRL[2:1] (%0d vs %b)", burst, gctrl_q[2:1]);
  a_gctrl_next: assert property ($past(rst_n) |->
      gctrl_q == ($past(wr_gctrl)
                  ? (($past(gctrl_q) & ~$past(reg_wmask[2:0]))
                     | ($past(pwdata[2:0]) & $past(reg_wmask[2:0])))
                  : $past(gctrl_q)))
    else $error("REQ-08: GCTRL %b inconsistent (write=%b wdata=%b wmask=%b prev=%b)",
                gctrl_q, $past(wr_gctrl), $past(pwdata[2:0]), $past(reg_wmask[2:0]),
                $past(gctrl_q));

  // ---------------------------------------------------------------------------
  // REQ-04/05/08 write decode, checked through the observable register outputs:
  // a write in the access phase of CH_x(n) must land in CH_x of channel n and nowhere
  // else (kills mutations of the per-channel write decode in rtl/dma_ctrl.sv).
  // Guarded so the engine cannot be moving the same register in that cycle.
  // ---------------------------------------------------------------------------
  // NOTE: deliberately unrolled (no generate loop): Verilator 5.048 aliases the
  // sampled-value ($past) expressions of concurrent assertions that differ only by a
  // genvar index, which makes such assertions fire on correct RTL. Literal indices here.

  // write-decode guards, per channel and register
  logic wr_ctrl_0, wr_src_0, wr_dst_0, wr_len_0, wr_next_0;
  logic wr_ctrl_1, wr_src_1, wr_dst_1, wr_len_1, wr_next_1;
  logic wr_ctrl_2, wr_src_2, wr_dst_2, wr_len_2, wr_next_2;
  logic wr_ctrl_3, wr_src_3, wr_dst_3, wr_len_3, wr_next_3;

  // ---- channel 0 ----
  assign wr_ctrl_0 = reg_wr && chan_sel && (chan_idx == 2'd0) && (chan_reg == R_CTRL) && !ch_active[0] && !ch_abort[0];
  a_ctrl_rd_fmt_0: assert property (ch_ctrl_rd[0][31:7] == 25'h0)
    else $error("REQ-09: CH_CTRL(0) read-back has bits above [6] set: %h", ch_ctrl_rd[0]);
  a_wr_ctrl_0: assert property (
      (wr_ctrl_0 && !ch_ctrl_rd[0][0]) |=> ch_ctrl_rd[0][6:1] ==
        $past((ch_ctrl_rd[0][6:1] & ~reg_wmask[6:1]) | (pwdata[6:1] & reg_wmask[6:1])))
    else $error("REQ-04/08: write to CH_CTRL(0) did not merge into the cfg field: now %h", ch_ctrl_rd[0]);
  assign wr_src_0 = reg_wr && chan_sel && (chan_idx == 2'd0) && (chan_reg == R_SRC) && !ch_active[0] && !word_done[0];
  a_wr_src_0: assert property (wr_src_0 |=> ch_src[0] == $past(((ch_src[0] & ~reg_wmask) | (pwdata & reg_wmask)) & 32'hFFFF_FFFC))
    else $error("REQ-04/08: write to CH_SRC(0) lost its data: now %h", ch_src[0]);
  assign wr_dst_0 = reg_wr && chan_sel && (chan_idx == 2'd0) && (chan_reg == R_DST) && !ch_active[0] && !word_done[0];
  a_wr_dst_0: assert property (wr_dst_0 |=> ch_dst[0] == $past(((ch_dst[0] & ~reg_wmask) | (pwdata & reg_wmask)) & 32'hFFFF_FFFC))
    else $error("REQ-04/08: write to CH_DST(0) lost its data: now %h", ch_dst[0]);
  assign wr_len_0 = reg_wr && chan_sel && (chan_idx == 2'd0) && (chan_reg == R_LEN) && !ch_active[0] && !word_done[0];
  a_wr_len_0: assert property (wr_len_0 |=> ch_len[0] == $past((ch_len[0] & ~reg_wmask[15:0]) | (pwdata[15:0] & reg_wmask[15:0])))
    else $error("REQ-04/08: write to CH_LEN(0) lost its data: now %h", ch_len[0]);
  assign wr_next_0 = reg_wr && chan_sel && (chan_idx == 2'd0) && (chan_reg == R_NEXT) && !ch_active[0];
  a_wr_next_0: assert property (wr_next_0 |=> ch_next[0] == $past(((ch_next[0] & ~reg_wmask) | (pwdata & reg_wmask)) & 32'hFFFF_FFFC))
    else $error("REQ-04/08: write to CH_NEXT(0) lost its data: now %h", ch_next[0]);

  // ---- channel 1 ----
  assign wr_ctrl_1 = reg_wr && chan_sel && (chan_idx == 2'd1) && (chan_reg == R_CTRL) && !ch_active[1] && !ch_abort[1];
  a_ctrl_rd_fmt_1: assert property (ch_ctrl_rd[1][31:7] == 25'h0)
    else $error("REQ-09: CH_CTRL(1) read-back has bits above [6] set: %h", ch_ctrl_rd[1]);
  a_wr_ctrl_1: assert property (
      (wr_ctrl_1 && !ch_ctrl_rd[1][0]) |=> ch_ctrl_rd[1][6:1] ==
        $past((ch_ctrl_rd[1][6:1] & ~reg_wmask[6:1]) | (pwdata[6:1] & reg_wmask[6:1])))
    else $error("REQ-04/08: write to CH_CTRL(1) did not merge into the cfg field: now %h", ch_ctrl_rd[1]);
  assign wr_src_1 = reg_wr && chan_sel && (chan_idx == 2'd1) && (chan_reg == R_SRC) && !ch_active[1] && !word_done[1];
  a_wr_src_1: assert property (wr_src_1 |=> ch_src[1] == $past(((ch_src[1] & ~reg_wmask) | (pwdata & reg_wmask)) & 32'hFFFF_FFFC))
    else $error("REQ-04/08: write to CH_SRC(1) lost its data: now %h", ch_src[1]);
  assign wr_dst_1 = reg_wr && chan_sel && (chan_idx == 2'd1) && (chan_reg == R_DST) && !ch_active[1] && !word_done[1];
  a_wr_dst_1: assert property (wr_dst_1 |=> ch_dst[1] == $past(((ch_dst[1] & ~reg_wmask) | (pwdata & reg_wmask)) & 32'hFFFF_FFFC))
    else $error("REQ-04/08: write to CH_DST(1) lost its data: now %h", ch_dst[1]);
  assign wr_len_1 = reg_wr && chan_sel && (chan_idx == 2'd1) && (chan_reg == R_LEN) && !ch_active[1] && !word_done[1];
  a_wr_len_1: assert property (wr_len_1 |=> ch_len[1] == $past((ch_len[1] & ~reg_wmask[15:0]) | (pwdata[15:0] & reg_wmask[15:0])))
    else $error("REQ-04/08: write to CH_LEN(1) lost its data: now %h", ch_len[1]);
  assign wr_next_1 = reg_wr && chan_sel && (chan_idx == 2'd1) && (chan_reg == R_NEXT) && !ch_active[1];
  a_wr_next_1: assert property (wr_next_1 |=> ch_next[1] == $past(((ch_next[1] & ~reg_wmask) | (pwdata & reg_wmask)) & 32'hFFFF_FFFC))
    else $error("REQ-04/08: write to CH_NEXT(1) lost its data: now %h", ch_next[1]);

  // ---- channel 2 ----
  assign wr_ctrl_2 = reg_wr && chan_sel && (chan_idx == 2'd2) && (chan_reg == R_CTRL) && !ch_active[2] && !ch_abort[2];
  a_ctrl_rd_fmt_2: assert property (ch_ctrl_rd[2][31:7] == 25'h0)
    else $error("REQ-09: CH_CTRL(2) read-back has bits above [6] set: %h", ch_ctrl_rd[2]);
  a_wr_ctrl_2: assert property (
      (wr_ctrl_2 && !ch_ctrl_rd[2][0]) |=> ch_ctrl_rd[2][6:1] ==
        $past((ch_ctrl_rd[2][6:1] & ~reg_wmask[6:1]) | (pwdata[6:1] & reg_wmask[6:1])))
    else $error("REQ-04/08: write to CH_CTRL(2) did not merge into the cfg field: now %h", ch_ctrl_rd[2]);
  assign wr_src_2 = reg_wr && chan_sel && (chan_idx == 2'd2) && (chan_reg == R_SRC) && !ch_active[2] && !word_done[2];
  a_wr_src_2: assert property (wr_src_2 |=> ch_src[2] == $past(((ch_src[2] & ~reg_wmask) | (pwdata & reg_wmask)) & 32'hFFFF_FFFC))
    else $error("REQ-04/08: write to CH_SRC(2) lost its data: now %h", ch_src[2]);
  assign wr_dst_2 = reg_wr && chan_sel && (chan_idx == 2'd2) && (chan_reg == R_DST) && !ch_active[2] && !word_done[2];
  a_wr_dst_2: assert property (wr_dst_2 |=> ch_dst[2] == $past(((ch_dst[2] & ~reg_wmask) | (pwdata & reg_wmask)) & 32'hFFFF_FFFC))
    else $error("REQ-04/08: write to CH_DST(2) lost its data: now %h", ch_dst[2]);
  assign wr_len_2 = reg_wr && chan_sel && (chan_idx == 2'd2) && (chan_reg == R_LEN) && !ch_active[2] && !word_done[2];
  a_wr_len_2: assert property (wr_len_2 |=> ch_len[2] == $past((ch_len[2] & ~reg_wmask[15:0]) | (pwdata[15:0] & reg_wmask[15:0])))
    else $error("REQ-04/08: write to CH_LEN(2) lost its data: now %h", ch_len[2]);
  assign wr_next_2 = reg_wr && chan_sel && (chan_idx == 2'd2) && (chan_reg == R_NEXT) && !ch_active[2];
  a_wr_next_2: assert property (wr_next_2 |=> ch_next[2] == $past(((ch_next[2] & ~reg_wmask) | (pwdata & reg_wmask)) & 32'hFFFF_FFFC))
    else $error("REQ-04/08: write to CH_NEXT(2) lost its data: now %h", ch_next[2]);

  // ---- channel 3 ----
  assign wr_ctrl_3 = reg_wr && chan_sel && (chan_idx == 2'd3) && (chan_reg == R_CTRL) && !ch_active[3] && !ch_abort[3];
  a_ctrl_rd_fmt_3: assert property (ch_ctrl_rd[3][31:7] == 25'h0)
    else $error("REQ-09: CH_CTRL(3) read-back has bits above [6] set: %h", ch_ctrl_rd[3]);
  a_wr_ctrl_3: assert property (
      (wr_ctrl_3 && !ch_ctrl_rd[3][0]) |=> ch_ctrl_rd[3][6:1] ==
        $past((ch_ctrl_rd[3][6:1] & ~reg_wmask[6:1]) | (pwdata[6:1] & reg_wmask[6:1])))
    else $error("REQ-04/08: write to CH_CTRL(3) did not merge into the cfg field: now %h", ch_ctrl_rd[3]);
  assign wr_src_3 = reg_wr && chan_sel && (chan_idx == 2'd3) && (chan_reg == R_SRC) && !ch_active[3] && !word_done[3];
  a_wr_src_3: assert property (wr_src_3 |=> ch_src[3] == $past(((ch_src[3] & ~reg_wmask) | (pwdata & reg_wmask)) & 32'hFFFF_FFFC))
    else $error("REQ-04/08: write to CH_SRC(3) lost its data: now %h", ch_src[3]);
  assign wr_dst_3 = reg_wr && chan_sel && (chan_idx == 2'd3) && (chan_reg == R_DST) && !ch_active[3] && !word_done[3];
  a_wr_dst_3: assert property (wr_dst_3 |=> ch_dst[3] == $past(((ch_dst[3] & ~reg_wmask) | (pwdata & reg_wmask)) & 32'hFFFF_FFFC))
    else $error("REQ-04/08: write to CH_DST(3) lost its data: now %h", ch_dst[3]);
  assign wr_len_3 = reg_wr && chan_sel && (chan_idx == 2'd3) && (chan_reg == R_LEN) && !ch_active[3] && !word_done[3];
  a_wr_len_3: assert property (wr_len_3 |=> ch_len[3] == $past((ch_len[3] & ~reg_wmask[15:0]) | (pwdata[15:0] & reg_wmask[15:0])))
    else $error("REQ-04/08: write to CH_LEN(3) lost its data: now %h", ch_len[3]);
  assign wr_next_3 = reg_wr && chan_sel && (chan_idx == 2'd3) && (chan_reg == R_NEXT) && !ch_active[3];
  a_wr_next_3: assert property (wr_next_3 |=> ch_next[3] == $past(((ch_next[3] & ~reg_wmask) | (pwdata & reg_wmask)) & 32'hFFFF_FFFC))
    else $error("REQ-04/08: write to CH_NEXT(3) lost its data: now %h", ch_next[3]);
endmodule

// Reset state of the top-level register (REQ-01: every register field resets to 0).
module ctrl_reset_sva (
  input logic        clk,
  input logic        rst_n,
  input logic [2:0]  gctrl_q
);

  a_rst_gctrl: assert property (@(posedge clk) !rst_n |-> gctrl_q == 3'd0)
    else $error("REQ-01: GCTRL is nonzero during reset (%b)", gctrl_q);
endmodule

/* verilator lint_on SYNCASYNCNET */

bind dma_ctrl ctrl_sva u_ctrl_sva (.*);
bind dma_ctrl ctrl_reset_sva u_ctrl_reset_sva (.*);
