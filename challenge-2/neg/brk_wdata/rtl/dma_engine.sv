// dma_engine: the single transfer engine shared by all channels. Owns the AXI4-Lite
// master. Moves one word at a time and fetches linked descriptors.
module dma_engine (
  input  logic        clk,
  input  logic        rst_n,
  // Global configuration
  input  logic        gen,
  input  logic [1:0]  burst,
  // Selected channel view
  input  logic [3:0]  ch_active,
  input  logic [3:0]  ch_abort,
  input  logic [3:0]  ch_hw_req,
  input  logic [3:0]  ch_link,
  input  logic [1:0]  ch_prio [dma_pkg::NUM_CH],
  input  logic [31:0] ch_src [dma_pkg::NUM_CH],
  input  logic [31:0] ch_dst [dma_pkg::NUM_CH],
  input  logic [15:0] ch_len [dma_pkg::NUM_CH],
  input  logic [31:0] ch_next [dma_pkg::NUM_CH],
  input  logic [3:0]  dreq,
  output logic [3:0]  dack,
  // Channel updates
  output logic [3:0]  owned,
  output logic [3:0]  word_done,
  output logic [3:0]  load_src,
  output logic [3:0]  load_dst,
  output logic [3:0]  load_len,
  output logic [3:0]  load_next,
  output logic [31:0] load_data,
  output logic [3:0]  stop,
  output logic [3:0]  ev_done,
  output logic [3:0]  ev_err,
  output logic [3:0]  ev_desc,
  output logic [1:0]  last_grant,
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
  input  logic [1:0]  m_rresp
);

  typedef enum logic [3:0] {
    S_IDLE, S_RD_ADDR, S_RD_DATA, S_WR, S_WR_RESP, S_END, S_FD_ADDR, S_FD_DATA,
    S_RELEASE, S_ERR, S_STOP
  } state_t;

  state_t      state_q;
  logic [1:0]  own_q;
  logic [1:0]  last_q;
  logic [3:0]  words_q;
  logic [1:0]  fd_idx_q;
  logic [31:0] data_q;
  logic        aw_done_q, w_done_q;
  logic        moved_q;

  logic [3:0]  eligible;
  logic        grant_valid;
  logic [1:0]  grant_idx;
  logic [3:0]  own_mask;
  logic        word_end;
  logic [3:0]  burst_words;

  assign burst_words = 4'd1 << burst;
  assign last_grant  = last_q;
  assign own_mask    = 4'b0001 << own_q;

  always_comb begin
    for (int i = 0; i < dma_pkg::NUM_CH; i++) begin
      eligible[i] = gen && ch_active[i] && !ch_abort[i] &&
                    (!ch_hw_req[i] || dreq[i] || (ch_len[i] == 16'd0));
    end
  end

  dma_arb u_arb (
    .eligible    (eligible),
    .prio        (ch_prio),
    .last        (last_q),
    .grant_valid (grant_valid),
    .grant_idx   (grant_idx)
  );

  assign word_end = (state_q == S_WR_RESP) && m_bvalid && (m_bresp == dma_pkg::RESP_OKAY);

  assign owned     = (state_q == S_IDLE) ? 4'b0000 : own_mask;
  assign word_done = word_end ? own_mask : 4'b0000;

  assign m_arvalid = (state_q == S_RD_ADDR) || (state_q == S_FD_ADDR);
  assign m_araddr  = (state_q == S_FD_ADDR) ? (ch_next[own_q] + {28'd0, fd_idx_q, 2'b00}) : ch_src[own_q];
  assign m_rready  = (state_q == S_RD_DATA) || (state_q == S_FD_DATA);
  assign m_awvalid = (state_q == S_WR) && !aw_done_q;
  assign m_awaddr  = ch_dst[own_q];
  assign m_wvalid  = (state_q == S_WR) && !w_done_q;
  assign m_wdata   = data_q + 32'd1;
  assign m_wstrb   = 4'hF;
  assign m_bready  = (state_q == S_WR_RESP);

  assign load_data = m_rdata;

  always_comb begin
    load_src  = 4'b0000;
    load_dst  = 4'b0000;
    load_len  = 4'b0000;
    load_next = 4'b0000;
    if ((state_q == S_FD_DATA) && m_rvalid && (m_rresp == dma_pkg::RESP_OKAY)) begin
      unique case (fd_idx_q)
        2'd0: load_src  = own_mask;
        2'd1: load_dst  = own_mask;
        2'd2: load_len  = own_mask;
        2'd3: load_next = own_mask;
      endcase
    end
  end

  always_comb begin
    stop    = 4'b0000;
    ev_done = 4'b0000;
    ev_desc = 4'b0000;
    ev_err  = 4'b0000;
    dack    = 4'b0000;
    unique case (state_q)
      S_END: begin
        // Descriptor completed (reached only on LEN=0 completion, incl. the
        // zero-length-at-grant case). REQ-36 (CR-D2): DESC pulses per completion.
        ev_desc = own_mask;
        if (!(ch_link[own_q] && (ch_next[own_q] != 32'd0))) begin
          stop    = own_mask;
          ev_done = own_mask;
          // REQ-27: dack when this burst ends; a chain-continuing end releases
          // at S_RELEASE instead (exactly one pulse per burst that moved data).
          if (ch_hw_req[own_q] && moved_q) begin
            dack = own_mask;
          end
        end
      end
      S_RELEASE: begin
        if (ch_hw_req[own_q] && moved_q) begin
          dack = own_mask;
        end
      end
      S_ERR: begin
        stop   = own_mask;
        ev_err = own_mask;
      end
      S_STOP: begin
        // REQ-31: abort sets neither DONE nor ERR (a failing last word routes via S_ERR).
        stop    = own_mask;
      end
      default: ;
    endcase
  end

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      state_q   <= S_IDLE;
      own_q     <= 2'd0;
      last_q    <= 2'd3;
      words_q   <= 4'd0;
      fd_idx_q  <= 2'd0;
      data_q    <= 32'd0;
      aw_done_q <= 1'b0;
      w_done_q  <= 1'b0;
      moved_q   <= 1'b0;
    end else begin
      unique case (state_q)
        S_IDLE: begin
          if (grant_valid) begin
            own_q   <= grant_idx;
            last_q  <= grant_idx;
            words_q <= burst_words;
            moved_q <= 1'b0;
            // REQ-18: a LEN=0 descriptor ends at once on grant - no data access.
            state_q <= (ch_len[grant_idx] == 16'd0) ? S_END : S_RD_ADDR;
          end
        end
        S_RD_ADDR: begin
          if (m_arready) begin
            state_q <= S_RD_DATA;
          end
        end
        S_RD_DATA: begin
          if (m_rvalid) begin
            if (m_rresp != dma_pkg::RESP_OKAY) begin
              state_q <= S_ERR;
            end else begin
              data_q    <= m_rdata;
              aw_done_q <= 1'b0;
              w_done_q  <= 1'b0;
              state_q   <= S_WR;
            end
          end
        end
        S_WR: begin
          if (m_awvalid && m_awready) begin
            aw_done_q <= 1'b1;
          end
          if (m_wvalid && m_wready) begin
            w_done_q <= 1'b1;
          end
          if ((aw_done_q || m_awready) && (w_done_q || m_wready)) begin
            state_q <= S_WR_RESP;
          end
        end
        S_WR_RESP: begin
          if (m_bvalid) begin
            if (m_bresp != dma_pkg::RESP_OKAY) begin
              state_q <= S_ERR;
            end else begin
              moved_q <= 1'b1;
              words_q <= words_q - 4'd1;
              if (ch_abort[own_q]) begin
                state_q <= S_STOP;
              end else if (ch_len[own_q] <= 16'd1) begin
                state_q <= S_END;
              end else if (words_q == 4'd1) begin
                state_q <= S_RELEASE;
              end else begin
                state_q <= S_RD_ADDR;
              end
            end
          end
        end
        S_END: begin
          if (ch_link[own_q] && (ch_next[own_q] != 32'd0)) begin
            fd_idx_q <= 2'd0;
            state_q  <= S_FD_ADDR;
          end else begin
            state_q <= S_IDLE;
          end
        end
        S_FD_ADDR: begin
          if (m_arready) begin
            state_q <= S_FD_DATA;
          end
        end
        S_FD_DATA: begin
          if (m_rvalid) begin
            if (m_rresp != dma_pkg::RESP_OKAY) begin
              state_q <= S_ERR;
            end else if (fd_idx_q == 2'd3) begin
              state_q <= S_RELEASE;
            end else begin
              fd_idx_q <= fd_idx_q + 2'd1;
              state_q  <= S_FD_ADDR;
            end
          end
        end
        S_RELEASE, S_ERR, S_STOP: begin
          state_q <= S_IDLE;
        end
        default: begin
          state_q <= S_IDLE;
        end
      endcase
    end
  end

endmodule
