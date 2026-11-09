// Flow-rehearsal top (SUBMISSION copy): the fixture with the deviation selector
// tied to 0. The design/ copy ties it to 8'h01 to stand in for a released
// baseline carrying a seeded LEN off-by-one; this file is what our submission
// ships - i.e. the Phase-2 fix.

`default_nettype none

module dma_top (
    input  logic         clk,
    input  logic         rst_n,

    input  logic         psel,
    input  logic         penable,
    input  logic         pwrite,
    input  logic [11:0]  paddr,
    input  logic [31:0]  pwdata,
    input  logic [3:0]   pstrb,
    output logic [31:0]  prdata,
    output logic         pready,
    output logic         pslverr,

    output logic         m_awvalid,
    input  logic         m_awready,
    output logic [31:0]  m_awaddr,
    output logic         m_wvalid,
    input  logic         m_wready,
    output logic [31:0]  m_wdata,
    output logic [3:0]   m_wstrb,
    input  logic         m_bvalid,
    output logic         m_bready,
    input  logic [1:0]   m_bresp,
    output logic         m_arvalid,
    input  logic         m_arready,
    output logic [31:0]  m_araddr,
    input  logic         m_rvalid,
    output logic         m_rready,
    input  logic [31:0]  m_rdata,
    input  logic [1:0]   m_rresp,

    output logic         irq_done
);

  logic [3:0] obs_state;
  logic       obs_busy, obs_done, obs_err;

  /* verilator lint_off UNUSEDSIGNAL */
  // observation taps: present for bind-style assertions in the prep harness;
  // a released top would not expose them at all (kept unused here on purpose)
  /* verilator lint_on UNUSEDSIGNAL */

  dma_ctrl u_dma (
      .clk(clk), .rst_n(rst_n), .bug(8'd0),
      .psel(psel), .penable(penable), .pwrite(pwrite), .paddr(paddr),
      .pwdata(pwdata), .pstrb(pstrb), .prdata(prdata), .pready(pready),
      .pslverr(pslverr),
      .m_awvalid(m_awvalid), .m_awready(m_awready), .m_awaddr(m_awaddr),
      .m_wvalid(m_wvalid), .m_wready(m_wready), .m_wdata(m_wdata), .m_wstrb(m_wstrb),
      .m_bvalid(m_bvalid), .m_bready(m_bready), .m_bresp(m_bresp),
      .m_arvalid(m_arvalid), .m_arready(m_arready), .m_araddr(m_araddr),
      .m_rvalid(m_rvalid), .m_rready(m_rready), .m_rdata(m_rdata), .m_rresp(m_rresp),
      .irq_done(irq_done),
      .obs_state(obs_state), .obs_busy(obs_busy), .obs_done(obs_done), .obs_err(obs_err)
  );

endmodule

`default_nettype wire
