// Scratch instrumentation (NOT shipped): counts how often the antecedents of the
// REQ-14 valid/payload-hold assertions actually occur during a run.
module obs_stall (
  input logic clk,
  input logic rst_n,
  input logic m_arvalid, input logic m_arready,
  input logic m_awvalid, input logic m_awready,
  input logic m_wvalid,  input logic m_wready
);
  int ar_stall = 0; int aw_stall = 0; int w_stall = 0;
  always @(posedge clk) begin
    if (rst_n) begin
      if (m_arvalid && !m_arready) ar_stall++;
      if (m_awvalid && !m_awready) aw_stall++;
      if (m_wvalid  && !m_wready)  w_stall++;
    end
  end
  final $display("OBS_STALL ar=%0d aw=%0d w=%0d", ar_stall, aw_stall, w_stall);
endmodule
bind dma_ctrl obs_stall u_obs_stall (.*);
