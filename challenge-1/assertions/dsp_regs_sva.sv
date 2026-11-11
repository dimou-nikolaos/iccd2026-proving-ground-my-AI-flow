module dsp_regs_sva (
  input  logic        clk,
  input  logic        rst_n,
  input  logic        cfg_we,
  input  logic [7:0]  cfg_addr,
  input  logic [31:0] cfg_wdata,
  input  logic [31:0] cfg_rdata,
  input  logic        cfg_err,
  input  logic        enable,
  input  logic        fir_bypass,
  input  logic [1:0]  round_mode,
  input  logic [1:0]  decim,
  input  logic        clear_on_last,
  input  logic [15:0] offset,
  input  logic [15:0] gain,
  input  logic [15:0] lim_hi,
  input  logic [15:0] lim_lo,
  input  logic [3:0]  ntaps,
  input  logic [15:0] coef_flat [dsp_pkg::MAX_TAPS],
  input  logic        busy,
  input  logic        fifo_empty,
  input  logic [15:0] stat_in,
  input  logic [15:0] stat_out,
  input  logic [15:0] stat_clip,
  input  logic [15:0] peak_max,
  input  logic [15:0] peak_min,
  input  logic        stat_clear,
  input  logic        ev_clip,
  input  logic        ev_frame,
  input  logic [15:0] last_len,
  input  logic        irq,
  input  logic [5:0]  word,
  input  logic [6:0]  ctrl_q,
  input  logic [15:0] offset_q,
  input  logic [15:0] gain_q,
  input  logic [15:0] hi_q,
  input  logic [15:0] lo_q,
  input  logic [3:0]  ntaps_q,
  input  logic [15:0] coef_q [dsp_pkg::MAX_TAPS],
  input  logic [1:0]  irq_stat_q,
  input  logic [1:0]  irq_en_q,
  input  logic        is_coef,
  input  logic [2:0]  coef_idx,
  input  logic        mapped
);

  logic        p_we, p_is_coef, p_ev_clip, p_ev_frame, p_rst = 1'b0;
  logic [5:0]  p_word;
  logic [31:0] p_wdata;
  logic [2:0]  p_cidx;
  logic [6:0]  p_ctrl;
  logic [15:0] p_off, p_gain, p_hi, p_lo;
  logic [3:0]  p_ntaps;
  logic [1:0]  p_irqen, p_irqstat;
  logic [15:0] p_coef [dsp_pkg::MAX_TAPS];

  always_comb begin
    if (!$isunknown({cfg_addr, cfg_wdata, cfg_rdata, cfg_err, cfg_we, enable, fir_bypass, round_mode,
                     decim, clear_on_last, offset, gain, lim_hi, lim_lo, ntaps, busy, fifo_empty,
                     stat_in, stat_out, stat_clip, peak_max, peak_min, stat_clear, ev_clip, ev_frame,
                     last_len, irq, word, ctrl_q, offset_q, gain_q, hi_q, lo_q, ntaps_q,
                     irq_stat_q, irq_en_q, is_coef, coef_idx, mapped})) begin
      a_word_def: assert (word == cfg_addr[7:2])
        else $error("regs word def wrong");
      a_iscoef_def: assert (is_coef == ((word >= dsp_pkg::A_COEF0) && (word <= dsp_pkg::A_COEF7)))
        else $error("regs is_coef def wrong");
      a_coefidx_def: assert (coef_idx == word[2:0])
        else $error("regs coef_idx def wrong");
      a_mapped_def: assert (mapped == (word <= dsp_pkg::A_LAST_LEN))
        else $error("regs mapped def wrong");
      a_err_def: assert (cfg_err == !mapped)
        else $error("regs cfg_err def wrong");
      a_enable_def: assert (enable == ctrl_q[0])
        else $error("regs enable def wrong");
      a_bypass_def: assert (fir_bypass == ctrl_q[1])
        else $error("regs fir_bypass def wrong");
      a_round_def: assert (round_mode == ctrl_q[3:2])
        else $error("regs round_mode def wrong");
      a_decim_def: assert (decim == ctrl_q[5:4])
        else $error("regs decim def wrong");
      a_col_def: assert (clear_on_last == ctrl_q[6])
        else $error("regs clear_on_last def wrong");
      a_offset_def: assert (offset == offset_q)
        else $error("regs offset def wrong");
      a_gain_def: assert (gain == gain_q)
        else $error("regs gain def wrong");
      a_hi_def: assert (lim_hi == hi_q)
        else $error("regs lim_hi def wrong");
      a_lo_def: assert (lim_lo == lo_q)
        else $error("regs lim_lo def wrong");
      a_ntaps_def: assert (ntaps == ntaps_q)
        else $error("regs ntaps def wrong");
      a_statclr_def: assert (stat_clear == (cfg_we && (word == dsp_pkg::A_STAT_CLR) && cfg_wdata[0]))
        else $error("regs stat_clear def wrong");
      a_irq_def: assert (irq == |(irq_stat_q & irq_en_q))
        else $error("regs irq def wrong");
      a_rdata_def: assert (cfg_rdata == (is_coef ? {16'h0, coef_q[coef_idx]}
                         : (word == dsp_pkg::A_ID)        ? dsp_pkg::UNIT_ID
                         : (word == dsp_pkg::A_CTRL)      ? {25'h0, ctrl_q}
                         : (word == dsp_pkg::A_OFFSET)    ? {16'h0, offset_q}
                         : (word == dsp_pkg::A_GAIN)      ? {16'h0, gain_q}
                         : (word == dsp_pkg::A_LIM_HI)    ? {16'h0, hi_q}
                         : (word == dsp_pkg::A_LIM_LO)    ? {16'h0, lo_q}
                         : (word == dsp_pkg::A_NTAPS)     ? {28'h0, ntaps_q}
                         : (word == dsp_pkg::A_STATUS)    ? {30'h0, fifo_empty, busy}
                         : (word == dsp_pkg::A_STAT_IN)   ? {16'h0, stat_in}
                         : (word == dsp_pkg::A_STAT_OUT)  ? {16'h0, stat_out}
                         : (word == dsp_pkg::A_STAT_CLIP) ? {16'h0, stat_clip}
                         : (word == dsp_pkg::A_PEAK_MAX)  ? {16'h0, peak_max}
                         : (word == dsp_pkg::A_PEAK_MIN)  ? {16'h0, peak_min}
                         : (word == dsp_pkg::A_IRQ_STAT)  ? {30'h0, irq_stat_q}
                         : (word == dsp_pkg::A_IRQ_EN)    ? {30'h0, irq_en_q}
                         : (word == dsp_pkg::A_LAST_LEN)  ? {16'h0, last_len}
                         : 32'h0))
        else $error("regs cfg_rdata def wrong");
      for (int g = 0; g < dsp_pkg::MAX_TAPS; g++) begin
        a_cflat_def: assert (coef_flat[g] == coef_q[g])
          else $error("regs coef_flat def wrong");
      end
    end
  end

  always @(posedge clk) begin
    if (p_rst && rst_n) begin
      a_ctrlq_ns: assert (ctrl_q == ((p_we && (p_word == dsp_pkg::A_CTRL)) ? p_wdata[6:0] : p_ctrl))
        else $error("regs ctrl_q next-state wrong");
      a_offq_ns: assert (offset_q == ((p_we && (p_word == dsp_pkg::A_OFFSET)) ? p_wdata[15:0] : p_off))
        else $error("regs offset_q next-state wrong");
      a_gainq_ns: assert (gain_q == ((p_we && (p_word == dsp_pkg::A_GAIN)) ? p_wdata[15:0] : p_gain))
        else $error("regs gain_q next-state wrong");
      a_hiq_ns: assert (hi_q == ((p_we && (p_word == dsp_pkg::A_LIM_HI)) ? p_wdata[15:0] : p_hi))
        else $error("regs hi_q next-state wrong");
      a_loq_ns: assert (lo_q == ((p_we && (p_word == dsp_pkg::A_LIM_LO)) ? p_wdata[15:0] : p_lo))
        else $error("regs lo_q next-state wrong");
      a_ntapsq_ns: assert (ntaps_q == ((p_we && (p_word == dsp_pkg::A_NTAPS) &&
                        (p_wdata[3:0] >= 4'd1) && (p_wdata[3:0] <= 4'd8)) ? p_wdata[3:0] : p_ntaps))
        else $error("regs ntaps_q next-state wrong");
      a_irqenq_ns: assert (irq_en_q == ((p_we && (p_word == dsp_pkg::A_IRQ_EN)) ? p_wdata[1:0] : p_irqen))
        else $error("regs irq_en_q next-state wrong");
      a_irqstatq_ns: assert (irq_stat_q == ((p_we && (p_word == dsp_pkg::A_IRQ_STAT))
                        ? ((p_irqstat & ~p_wdata[1:0]) | {p_ev_frame, p_ev_clip})
                        : (p_irqstat | {p_ev_frame, p_ev_clip})))
        else $error("regs irq_stat_q next-state wrong");
      for (int g = 0; g < dsp_pkg::MAX_TAPS; g++) begin
        a_coefq_ns: assert (coef_q[g] == ((p_we && p_is_coef && (p_cidx == g[2:0])) ? p_wdata[15:0] : p_coef[g]))
          else $error("regs coef_q next-state wrong");
      end
    end
    p_we       <= cfg_we;
    p_word     <= word;
    p_wdata    <= cfg_wdata;
    p_is_coef  <= is_coef;
    p_cidx     <= coef_idx;
    p_ev_clip  <= ev_clip;
    p_ev_frame <= ev_frame;
    p_ctrl     <= ctrl_q;
    p_off      <= offset_q;
    p_gain     <= gain_q;
    p_hi       <= hi_q;
    p_lo       <= lo_q;
    p_ntaps    <= ntaps_q;
    p_irqen    <= irq_en_q;
    p_irqstat  <= irq_stat_q;
    for (int i = 0; i < dsp_pkg::MAX_TAPS; i++) begin
      p_coef[i] <= coef_q[i];
    end
    p_rst      <= rst_n;
  end

endmodule

bind dsp_regs dsp_regs_sva u_dsp_regs_sva (.*);
