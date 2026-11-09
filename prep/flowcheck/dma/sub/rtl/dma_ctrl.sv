// Synthetic DMA controller used to build and self-test the challenge-2 verification
// kit BEFORE the real design is released (prep only - never submitted).
//
// Shape follows the released challenge-2 blurb and docs/designs/challenge-2-playbook.md:
//   - APB4 slave config interface (software programs the engine),
//   - AXI4-Lite master for all data movement (one shared engine performs every transfer),
//   - linked descriptors (src/dst/len/next) fetched over that same AXI master.
//
// The register map and the descriptor layout below are OUR choices for this fixture; the
// released design's map will differ. What transfers is the *shape* of the checks - word
// count boundaries, chain follow/end, read-only writes, backpressure races, status and
// interrupt lifecycle - which is what ../dma_env.py exercises.
//
// `bug` selects deviations for the self-test negative matrix (rtl/dma_top.sv ties it to
// 0, which is the shape used for the flow rehearsal):
//   bit0  LEN off-by-one: transfers LEN-1 words                        (playbook D1)
//   bit1  write to a read-only register is accepted                    (playbook D10)
//   bit2  chain end ignored: follows next==0 forever                    (playbook D7)
//   bit3  BUSY never clears and DONE is never set                       (status lifecycle)
//   bit4  data transfer starts before the descriptor is fully latched   (playbook D8)
//   bit5  irq_done held as a level instead of a 1-cycle pulse
//   bit6  write strobes narrowed to two byte lanes                      (data integrity)
//   bit7  address increment uses the wrong stride                       (addressing)

`default_nettype none

module dma_ctrl (
    input  logic         clk,
    input  logic         rst_n,
    input  logic [7:0]   bug,

    // ---- APB4 slave (config) ------------------------------------------
    input  logic         psel,
    input  logic         penable,
    input  logic         pwrite,
    input  logic [11:0]  paddr,
    input  logic [31:0]  pwdata,
    input  logic [3:0]   pstrb,
    output logic [31:0]  prdata,
    output logic         pready,
    output logic         pslverr,

    // ---- AXI4-Lite master (memory) ------------------------------------
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

    output logic         irq_done,

    // ---- observation taps for the SVA observer -------------------------
    output logic [3:0]   obs_state,
    output logic         obs_busy,
    output logic         obs_done,
    output logic         obs_err
);

  // ---- register map (fixture) -----------------------------------------
  localparam logic [11:0] R_CTRL   = 12'h000;  // [0] EN [1] START [2] RELOAD [3] MODE [4] IE
  localparam logic [11:0] R_STATUS = 12'h004;  // [0] BUSY [1] DONE [2] ERR (DONE/ERR W1C)
  localparam logic [11:0] R_SRC    = 12'h008;
  localparam logic [11:0] R_DST    = 12'h00C;
  localparam logic [11:0] R_LEN    = 12'h010;  // word count
  localparam logic [11:0] R_DESC   = 12'h014;  // descriptor address (MODE=1)
  localparam logic [11:0] R_NEXT   = 12'h018;  // RO: last fetched next pointer
  localparam logic [11:0] R_ID     = 12'h01C;  // RO
  localparam logic [31:0] ID_VALUE = 32'hD1A4_0000;

  // ---- engine states ---------------------------------------------------
  localparam logic [3:0] S_IDLE     = 4'd0,
                         S_DESC_AR  = 4'd1,
                         S_DESC_R   = 4'd2,
                         S_LATCH    = 4'd3,
                         S_XFER_AR  = 4'd4,
                         S_XFER_R   = 4'd5,
                         S_XFER_AW  = 4'd6,
                         S_XFER_W   = 4'd7,
                         S_XFER_B   = 4'd8,
                         S_CHAIN    = 4'd9,
                         S_DONE     = 4'd10;

  // ---- config registers -------------------------------------------------
  logic        en, reload, mode, ie;
  logic        busy, done, err;
  logic [31:0] r_src, r_dst, r_len, r_desc, r_next;
  logic [31:0] id_shadow;            // only reachable when bug[1]

  // ---- engine state -----------------------------------------------------
  logic [3:0]  state;
  logic [31:0] d_desc;               // engine's descriptor pointer (engine-driven)
  logic [31:0] w_src, w_dst, w_len, w_next;
  logic [31:0] dwords [0:3];
  logic [1:0]  dfield;                // descriptor word being fetched (0..3)
  logic [31:0] cnt;
  logic [31:0] data_r;

  // ---- APB4 slave -------------------------------------------------------
  // pready is gated with PSEL (AMBA spec-legal either way, but an ungated
  // constant-1 PREADY trips the protocol observer A5 and would look like a
  // protocol deviation to any checker bound to the bus - gate it).
  assign pready  = psel;                      // single-cycle config access
  assign pslverr = 1'b0;

  wire apb_wr = psel & penable & pwrite;      // commit in the ACCESS phase only
  wire start_req = apb_wr && (paddr == R_CTRL) && pstrb[0] && pwdata[1];
  wire start_pulse = start_req && en && !busy;

  function automatic logic [31:0] apply_strb(input logic [31:0] old,
                                             input logic [31:0] nw,
                                             input logic [3:0]  strb);
    logic [31:0] res;
    for (int i = 0; i < 4; i++) res[i*8 +: 8] = strb[i] ? nw[i*8 +: 8] : old[i*8 +: 8];
    return res;
  endfunction

  function automatic logic [31:0] rd_reg(input logic [11:0] a);
    case (a)
      R_CTRL:   return {27'd0, ie, mode, reload, 1'b0, en};
      R_STATUS: return {29'd0, err, done, busy};
      R_SRC:    return r_src;
      R_DST:    return r_dst;
      R_LEN:    return r_len;
      R_DESC:   return r_desc;
      R_NEXT:   return r_next;
      R_ID:     return bug[1] ? id_shadow : ID_VALUE;
      default:  return 32'h0;
    endcase
  endfunction

  assign prdata = rd_reg(paddr);

  // ---- effective transfer length (bug[0] = one word short) ---------------
  function automatic logic [31:0] eff_len(input logic [31:0] n);
    if (n == 32'd0) return 32'd0;
    return bug[0] ? (n - 32'd1) : n;
  endfunction

  // ---- address generation (bug[7] = wrong stride) ------------------------
  wire [31:0] desc_off = {28'd0, dfield, 2'b00};        // 2+2 bits -> pad to 32
  wire [31:0] stride   = bug[7] ? 32'd16 : 32'd4;
  wire [31:0] raddr = (state == S_DESC_AR)
                      ? (d_desc + desc_off)
                      : (w_src + cnt * stride);
  wire [31:0] waddr = w_dst + cnt * stride;

  // ---- AXI4-Lite master --------------------------------------------------
  assign m_arvalid = (state == S_DESC_AR) || (state == S_XFER_AR);
  assign m_araddr  = raddr;
  assign m_rready  = 1'b1;
  assign m_awvalid = (state == S_XFER_AW);
  assign m_awaddr  = waddr;
  assign m_wvalid  = (state == S_XFER_W);
  assign m_wdata   = data_r;
  assign m_wstrb   = bug[6] ? 4'b0011 : 4'b1111;
  assign m_bready  = (state == S_XFER_B);

  // ---- status set/clear requests (one sequential owner: the config block)
  // NOTE: one sequential driver per register - Verilator merges multiple
  // always_ff writers but Yosys reports them as conflicting cells, and the
  // flow lints with yosys. Signals owned here: en/reload/mode/ie, r_* address
  // regs, done, err. Everything else (state/busy/working regs) is engine-owned.
  logic set_done, clr_done, set_err, clr_err;
  assign set_done = (state == S_DONE) && !bug[3];
  assign set_err  = ((state == S_DESC_R) && m_rvalid  && (m_rresp != 2'b00)) ||
                    ((state == S_XFER_R) && m_rvalid  && (m_rresp != 2'b00)) ||
                    ((state == S_XFER_B) && m_bvalid  && (m_bresp != 2'b00));
  assign clr_done = apb_wr && (paddr == R_STATUS) && pstrb[0] && pwdata[1];
  assign clr_err  = apb_wr && (paddr == R_STATUS) && pstrb[0] && pwdata[2];

  // ---- config registers --------------------------------------------------
  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      en <= 1'b0; reload <= 1'b0; mode <= 1'b0; ie <= 1'b0;
      r_src <= 32'd0; r_dst <= 32'd0; r_len <= 32'd0; r_desc <= 32'd0;
      id_shadow <= 32'd0;
      done <= 1'b0; err <= 1'b0;
    end else begin
      if (set_done) done <= 1'b1;
      if (clr_done) done <= 1'b0;          // W1C wins over a same-cycle set
      if (set_err)  err  <= 1'b1;
      if (clr_err)  err  <= 1'b0;
      if (apb_wr) begin
        case (paddr)
          R_CTRL: if (pstrb[0]) begin
            en     <= pwdata[0];
            reload <= pwdata[2];
            mode   <= pwdata[3];
            ie     <= pwdata[4];
          end
          R_SRC:  r_src  <= apply_strb(r_src, pwdata, pstrb);
          R_DST:  r_dst  <= apply_strb(r_dst, pwdata, pstrb);
          R_LEN:  r_len  <= apply_strb(r_len, pwdata, pstrb);
          R_DESC: r_desc <= apply_strb(r_desc, pwdata, pstrb);
          R_ID:   if (bug[1]) id_shadow <= apply_strb(id_shadow, pwdata, pstrb);
          default: ;
        endcase
      end
    end
  end

  // ---- engine -----------------------------------------------------------
  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      state <= S_IDLE; busy <= 1'b0;
      cnt <= 32'd0; dfield <= 2'd0;
      d_desc <= 32'd0; r_next <= 32'd0;
      w_src <= 32'd0; w_dst <= 32'd0; w_len <= 32'd0; w_next <= 32'd0;
      dwords[0] <= 32'd0; dwords[1] <= 32'd0; dwords[2] <= 32'd0; dwords[3] <= 32'd0;
      data_r <= 32'd0;
    end else begin
      case (state)
        S_IDLE: if (start_pulse) begin
          busy   <= 1'b1;
          cnt    <= 32'd0;
          dfield <= 2'd0;
          if (mode) begin
            d_desc <= r_desc;
            state  <= S_DESC_AR;
          end else begin
            w_src <= r_src; w_dst <= r_dst; w_len <= r_len; w_next <= 32'd0;
            state <= (eff_len(r_len) == 32'd0) ? S_DONE : S_XFER_AR;
          end
        end

        S_DESC_AR: if (m_arready) state <= S_DESC_R;

        S_DESC_R: if (m_rvalid) begin
          dwords[dfield] <= m_rdata;
          // bug[4]: begin the data phase once src/dst/len have come back, latching
          // `next` from a word that has not been fetched yet (descriptor race).
          if (bug[4] && dfield == 2'd2) begin
            w_src  <= dwords[0];
            w_dst  <= dwords[1];
            w_len  <= m_rdata;
            w_next <= dwords[3];
            state  <= (m_rdata == 32'd0) ? S_CHAIN : S_XFER_AR;
          end else if (dfield == 2'd3) begin
            state <= S_LATCH;
          end else begin
            dfield <= dfield + 2'd1;
            state  <= S_DESC_AR;
          end
        end

        S_LATCH: begin
          w_src  <= dwords[0];
          w_dst  <= dwords[1];
          w_len  <= dwords[2];
          w_next <= dwords[3];
          r_next <= dwords[3];
          state  <= (eff_len(dwords[2]) == 32'd0) ? S_CHAIN : S_XFER_AR;
        end

        S_XFER_AR: if (m_arready) state <= S_XFER_R;

        S_XFER_R: if (m_rvalid) begin
          data_r <= m_rdata;
          state  <= S_XFER_AW;
        end

        S_XFER_AW: if (m_awready) state <= S_XFER_W;

        S_XFER_W: if (m_wready) state <= S_XFER_B;

        S_XFER_B: if (m_bvalid) begin
          cnt   <= cnt + 32'd1;
          state <= ((cnt + 32'd1) >= eff_len(w_len)) ? S_CHAIN : S_XFER_AR;
        end

        S_CHAIN: if (mode && (w_next != 32'd0 || bug[2])) begin
          d_desc <= w_next;
          cnt    <= 32'd0;
          dfield <= 2'd0;
          state  <= S_DESC_AR;
        end else begin
          state <= S_DONE;
        end

        S_DONE: begin
          // `done` is set by the config block via set_done (single owner);
          // bug[3] suppresses that pulse, keeping DONE clear and BUSY stuck.
          busy  <= bug[3];
          state <= S_IDLE;
        end

        default: state <= S_IDLE;
      endcase
    end
  end

  // ---- interrupt: a one-cycle pulse when DONE rises (bug[5] holds it) ----
  logic done_d;
  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) done_d <= 1'b0;
    else        done_d <= done;
  end
  wire done_pulse = done & ~done_d;

  assign irq_done = ie && (bug[5] ? done : done_pulse);

  // ---- observation taps --------------------------------------------------
  assign obs_state = state;
  assign obs_busy  = busy;
  assign obs_done  = done;
  assign obs_err   = err;

endmodule

`default_nettype wire
