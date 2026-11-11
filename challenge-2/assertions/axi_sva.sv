// axi_sva.sv — AXI4-Lite master contract of dma_ctrl
// (REQ-02 reset state, REQ-12 word strobes / alignment, REQ-13 single outstanding,
//  REQ-14 valid+payload hold while waiting for ready).
//
// Bound to the TOP module; only DUT *outputs* are checked (the public memory model
// deliberately backpressures and may drop its own valids, so slave-side stability is
// not asserted here).

/* verilator lint_off SYNCASYNCNET */

module axi_sva (
  input logic        clk,
  input logic        rst_n,
  // AXI4-Lite master side of dma_ctrl (names match dma_ctrl's ports)
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
  input logic [1:0]  m_rresp
);

  default clocking cb @(posedge clk); endclocking
  default disable iff (!rst_n);

  // REQ-13: at most one access outstanding — AR and AW are never valid together.
  a_req13_no_ar_aw: assert property (!(m_arvalid && m_awvalid))
    else $error("REQ-13: m_arvalid and m_awvalid both 1 (araddr=%h awaddr=%h)",
                m_araddr, m_awaddr);

  // REQ-12: word transfers only — the read address is always word aligned.
  a_req12_ar_aligned: assert property (m_arvalid |-> m_araddr[1:0] == 2'b00)
    else $error("REQ-12: m_araddr=%h not word aligned", m_araddr);
  a_req12_aw_aligned: assert property (m_awvalid |-> m_awaddr[1:0] == 2'b00)
    else $error("REQ-12: m_awaddr=%h not word aligned", m_awaddr);

  // REQ-12: full word write strobes.
  a_req12_wstrb: assert property (m_wvalid |-> m_wstrb == 4'hF)
    else $error("REQ-12: m_wstrb=%b while m_wvalid=1 (want 1111)", m_wstrb);

  // REQ-14: while valid is waiting for ready, valid and payload must hold.
  a_req14_ar_hold: assert property (
      (m_arvalid && !m_arready) |=> (m_arvalid && m_araddr == $past(m_araddr)))
    else $error("REQ-14: m_arvalid/m_araddr dropped or changed while m_arready=0 (was %h)",
                $past(m_araddr));
  a_req14_aw_hold: assert property (
      (m_awvalid && !m_awready) |=> (m_awvalid && m_awaddr == $past(m_awaddr)))
    else $error("REQ-14: m_awvalid/m_awaddr dropped or changed while m_awready=0 (was %h)",
                $past(m_awaddr));
  a_req14_w_hold: assert property (
      (m_wvalid && !m_wready) |=> (m_wvalid && m_wdata == $past(m_wdata)
                                   && m_wstrb == $past(m_wstrb)))
    else $error("REQ-14: m_wvalid/m_wdata/m_wstrb changed while m_wready=0");

endmodule

// Reset state of the AXI master outputs (REQ-02). Separate module: it must be live
// while rst_n is low, so it carries no `default disable iff`.
module axi_reset_sva (
  input logic        clk,
  input logic        rst_n,
  input logic        m_awvalid,
  input logic        m_wvalid,
  input logic        m_bready,
  input logic        m_arvalid,
  input logic        m_rready,
  input logic [3:0]  dack,
  input logic        irq
);

  a_rst_awvalid: assert property (@(posedge clk) !rst_n |-> !m_awvalid)
    else $error("REQ-02: m_awvalid is 1 during reset");
  a_rst_wvalid: assert property (@(posedge clk) !rst_n |-> !m_wvalid)
    else $error("REQ-02: m_wvalid is 1 during reset");
  a_rst_arvalid: assert property (@(posedge clk) !rst_n |-> !m_arvalid)
    else $error("REQ-02: m_arvalid is 1 during reset");
  a_rst_bready: assert property (@(posedge clk) !rst_n |-> !m_bready)
    else $error("REQ-02: m_bready is 1 during reset");
  a_rst_rready: assert property (@(posedge clk) !rst_n |-> !m_rready)
    else $error("REQ-02: m_rready is 1 during reset");
  a_rst_dack: assert property (@(posedge clk) !rst_n |-> dack == 4'b0000)
    else $error("REQ-02: dack is nonzero during reset");
  a_rst_irq: assert property (@(posedge clk) !rst_n |-> !irq)
    else $error("REQ-02: irq is 1 during reset");

  // REQ-02: at the first clock edge after reset release no AXI transaction has started.
  a_rst_no_start: assert property (@(posedge clk)
      (rst_n && $past(!rst_n)) |-> (!m_awvalid && !m_wvalid && !m_arvalid))
    else $error("REQ-02: an AXI valid is asserted on the first edge after reset release");
  a_rst_dack_after: assert property (@(posedge clk)
      (rst_n && $past(!rst_n)) |-> dack == 4'b0000)
    else $error("REQ-02: dack is nonzero on the first edge after reset release");
endmodule

/* verilator lint_on SYNCASYNCNET */

bind dma_ctrl axi_sva u_axi_sva (.*);
bind dma_ctrl axi_reset_sva u_axi_reset_sva (.*);
