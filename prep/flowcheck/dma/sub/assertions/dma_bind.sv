// Bind file for the DMA fixture self-test (prep). On day 1 write the same
// three lines against the released top + port names (flow-lessons L25:
// explicit port map, observer ports all `input`).

bind dma_ctrl dma_irq_sva u_dma_irq_sva (
    .clk(clk), .rst_n(rst_n), .irq(irq_done));

bind dma_ctrl dma_fsm_sva u_dma_fsm_sva (
    .clk(clk), .rst_n(rst_n), .state(obs_state), .busy(obs_busy),
    .psel(psel), .penable(penable),
    .arvalid(m_arvalid), .awvalid(m_awvalid), .wvalid(m_wvalid), .pready(pready));

bind dma_ctrl apb4_protocol_sva u_dma_apb_sva (
    .clk(clk), .rst_n(rst_n), .psel(psel), .penable(penable), .pwrite(pwrite),
    .paddr(paddr), .pwdata(pwdata), .pstrb(pstrb), .pprot(4'd0),
    .pready(pready), .prdata(prdata), .pslverr(pslverr));

bind dma_ctrl axi4lite_protocol_sva u_dma_axi_sva (
    .clk(clk), .rst_n(rst_n),
    .awvalid(m_awvalid), .awready(m_awready), .awaddr(m_awaddr), .awprot(3'd0),
    .wvalid(m_wvalid), .wready(m_wready), .wdata(m_wdata), .wstrb(m_wstrb),
    .bvalid(m_bvalid), .bready(m_bready), .bresp(m_bresp),
    .arvalid(m_arvalid), .arready(m_arready), .araddr(m_araddr), .arprot(3'd0),
    .rvalid(m_rvalid), .rready(m_rready), .rdata(m_rdata), .rresp(m_rresp));
