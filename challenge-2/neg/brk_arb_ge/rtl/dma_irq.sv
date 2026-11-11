// dma_irq: interrupt status and enable registers.
module dma_irq (
  input  logic        clk,
  input  logic        rst_n,
  input  logic        wr_status,
  input  logic        wr_enable,
  input  logic [31:0] wdata,
  input  logic [31:0] wmask,
  input  logic [3:0]  ev_done,
  input  logic [3:0]  ev_err,
  input  logic [3:0]  ev_desc,
  output logic [31:0] status_rd,
  output logic [31:0] enable_rd,
  output logic        irq
);

  logic [31:0] status_q, enable_q;
  logic [31:0] set_vec, clr_vec;

  assign set_vec = {20'h0, ev_desc, ev_err, ev_done};
  assign clr_vec = wr_status ? (wdata & wmask) : 32'h0;

  assign status_rd = status_q;
  assign enable_rd = enable_q;
  assign irq       = |(status_q & enable_q);

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      status_q <= 32'h0;
      enable_q <= 32'h0;
    end else begin
      status_q <= ((status_q & ~clr_vec) | set_vec) & dma_pkg::IRQ_MASK;
      if (wr_enable) begin
        enable_q <= ((enable_q & ~wmask) | (wdata & wmask)) & dma_pkg::IRQ_MASK;
      end
    end
  end

endmodule
