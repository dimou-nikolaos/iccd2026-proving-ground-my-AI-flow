// dma_ctrl: four-channel DMA controller with an APB4 configuration port and an
// AXI4-Lite memory master.
module dma_ctrl (
  input  logic        clk,
  input  logic        rst_n,
  // APB4 completer
  input  logic        psel,
  input  logic        penable,
  input  logic        pwrite,
  input  logic [11:0] paddr,
  input  logic [31:0] pwdata,
  input  logic [3:0]  pstrb,
  output logic [31:0] prdata,
  output logic        pready,
  output logic        pslverr,
  // AXI4-Lite master
  output logic        m_awvalid,
  input  logic        m_awready,
  output logic [31:0] m_awaddr,
  output logic        m_wvalid,
  input  logic        m_wready,
  output logic [31:0] m_wdata,
  output logic [3:0]  m_wstrb,
  input  logic        m_bvalid,
  output logic        m_bready,
  input  logic [1:0]  m_bresp,
  output logic        m_arvalid,
  input  logic        m_arready,
  output logic [31:0] m_araddr,
  input  logic        m_rvalid,
  output logic        m_rready,
  input  logic [31:0] m_rdata,
  input  logic [1:0]  m_rresp,
  // Peripheral requests
  input  logic [3:0]  dreq,
  output logic [3:0]  dack,
  // Interrupt
  output logic        irq
);

  logic        reg_wr, glob_sel, chan_sel;
  logic [31:0] reg_wmask;
  logic [9:0]  glob_word;
  logic [1:0]  chan_idx;
  logic [2:0]  chan_reg;

  logic [2:0]  gctrl_q;
  logic        gen;
  logic [1:0]  burst;

  logic [3:0]  ch_active, ch_abort, ch_hw_req, ch_link;
  logic [1:0]  ch_prio [dma_pkg::NUM_CH];
  logic [31:0] ch_src [dma_pkg::NUM_CH];
  logic [31:0] ch_dst [dma_pkg::NUM_CH];
  logic [15:0] ch_len [dma_pkg::NUM_CH];
  logic [31:0] ch_next [dma_pkg::NUM_CH];
  logic [31:0] ch_ctrl_rd [dma_pkg::NUM_CH];
  logic [31:0] ch_stat_rd [dma_pkg::NUM_CH];

  logic [3:0]  owned, word_done, load_src, load_dst, load_len, load_next, stop;
  logic [3:0]  ev_done, ev_err;
  logic [31:0] load_data;
  logic [1:0]  last_grant;

  logic [31:0] irq_status_rd, irq_enable_rd;

  dma_apb u_apb (
    .psel      (psel),
    .penable   (penable),
    .pwrite    (pwrite),
    .paddr     (paddr),
    .pstrb     (pstrb),
    .pready    (pready),
    .pslverr   (pslverr),
    .reg_wr    (reg_wr),
    .reg_wmask (reg_wmask),
    .glob_sel  (glob_sel),
    .glob_word (glob_word),
    .chan_sel  (chan_sel),
    .chan_idx  (chan_idx),
    .chan_reg  (chan_reg)
  );

  assign gen   = gctrl_q[0];
  assign burst = gctrl_q[2:1];

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      gctrl_q <= 3'd0;
    end else if (reg_wr && glob_sel && (glob_word == dma_pkg::W_GCTRL)) begin
      gctrl_q <= (gctrl_q & ~reg_wmask[2:0]) | (pwdata[2:0] & reg_wmask[2:0]);
    end
  end

  for (genvar n = 0; n < dma_pkg::NUM_CH; n++) begin : g_chan
    logic sel;
    assign sel = reg_wr && chan_sel && (chan_idx == 2'(n));

    dma_chan u_chan (
      .clk       (clk),
      .rst_n     (rst_n),
      .wr_ctrl   (sel && (chan_reg == dma_pkg::R_CTRL)),
      .wr_src    (sel && (chan_reg == dma_pkg::R_SRC)),
      .wr_dst    (sel && (chan_reg == dma_pkg::R_DST)),
      .wr_len    (sel && (chan_reg == dma_pkg::R_LEN)),
      .wr_next   (sel && (chan_reg == dma_pkg::R_NEXT)),
      .wdata     (pwdata),
      .wmask     (reg_wmask),
      .owned     (owned[n]),
      .word_done (word_done[n]),
      .load_src  (load_src[n]),
      .load_dst  (load_dst[n]),
      .load_len  (load_len[n]),
      .load_next (load_next[n]),
      .load_data (load_data),
      .stop      (stop[n]),
      .set_err   (ev_err[n]),
      .set_done  (ev_done[n]),
      .active    (ch_active[n]),
      .abort     (ch_abort[n]),
      .hw_req    (ch_hw_req[n]),
      .prio      (ch_prio[n]),
      .link      (ch_link[n]),
      .src       (ch_src[n]),
      .dst       (ch_dst[n]),
      .len       (ch_len[n]),
      .next      (ch_next[n]),
      .ctrl_rd   (ch_ctrl_rd[n]),
      .stat_rd   (ch_stat_rd[n])
    );
  end

  dma_engine u_engine (
    .clk        (clk),
    .rst_n      (rst_n),
    .gen        (gen),
    .burst      (burst),
    .ch_active  (ch_active),
    .ch_abort   (ch_abort),
    .ch_hw_req  (ch_hw_req),
    .ch_link    (ch_link),
    .ch_prio    (ch_prio),
    .ch_src     (ch_src),
    .ch_dst     (ch_dst),
    .ch_len     (ch_len),
    .ch_next    (ch_next),
    .dreq       (dreq),
    .dack       (dack),
    .owned      (owned),
    .word_done  (word_done),
    .load_src   (load_src),
    .load_dst   (load_dst),
    .load_len   (load_len),
    .load_next  (load_next),
    .load_data  (load_data),
    .stop       (stop),
    .ev_done    (ev_done),
    .ev_err     (ev_err),
    .last_grant (last_grant),
    .m_awvalid  (m_awvalid),
    .m_awready  (m_awready),
    .m_awaddr   (m_awaddr),
    .m_wvalid   (m_wvalid),
    .m_wready   (m_wready),
    .m_wdata    (m_wdata),
    .m_wstrb    (m_wstrb),
    .m_bvalid   (m_bvalid),
    .m_bready   (m_bready),
    .m_bresp    (m_bresp),
    .m_arvalid  (m_arvalid),
    .m_arready  (m_arready),
    .m_araddr   (m_araddr),
    .m_rvalid   (m_rvalid),
    .m_rready   (m_rready),
    .m_rdata    (m_rdata),
    .m_rresp    (m_rresp)
  );

  dma_irq u_irq (
    .clk       (clk),
    .rst_n     (rst_n),
    .wr_status (reg_wr && glob_sel && (glob_word == dma_pkg::W_IRQ_ST)),
    .wr_enable (reg_wr && glob_sel && (glob_word == dma_pkg::W_IRQ_EN)),
    .wdata     (pwdata),
    .wmask     (reg_wmask),
    .ev_done   (ev_done),
    .ev_err    (ev_err),
    .status_rd (irq_status_rd),
    .enable_rd (irq_enable_rd),
    .irq       (irq)
  );

  always_comb begin
    prdata = 32'h0;
    if (psel && penable && !pwrite) begin
      if (glob_sel) begin
        unique case (glob_word)
          dma_pkg::W_ID:     prdata = dma_pkg::UNIT_ID;
          dma_pkg::W_GCTRL:  prdata = {29'h0, gctrl_q};
          dma_pkg::W_IRQ_ST: prdata = irq_status_rd;
          dma_pkg::W_IRQ_EN: prdata = irq_enable_rd;
          dma_pkg::W_ACTIVE: prdata = {28'h0, ch_active};
          dma_pkg::W_GRANT:  prdata = {30'h0, last_grant};
          default:           prdata = 32'h0;
        endcase
      end else if (chan_sel) begin
        unique case (chan_reg)
          dma_pkg::R_CTRL: prdata = ch_ctrl_rd[chan_idx];
          dma_pkg::R_SRC:  prdata = ch_src[chan_idx];
          dma_pkg::R_DST:  prdata = ch_dst[chan_idx];
          dma_pkg::R_LEN:  prdata = {16'h0, ch_len[chan_idx]};
          dma_pkg::R_NEXT: prdata = ch_next[chan_idx];
          dma_pkg::R_STAT: prdata = ch_stat_rd[chan_idx];
          default:         prdata = 32'h0;
        endcase
      end
    end
  end

endmodule
