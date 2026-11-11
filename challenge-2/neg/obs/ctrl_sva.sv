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
  // register read-back sources
  input logic [3:0]  ch_active,
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
