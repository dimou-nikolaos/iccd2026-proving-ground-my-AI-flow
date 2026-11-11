/* verilator lint_off SYNCASYNCNET */

module dsp_gain_reset_sva (
  input  logic               clk,
  input  logic               rst_n,
  input  logic               valid1_q,
  input  logic signed [16:0] centered_q,
  input  logic               last1_q,
  input  logic               valid2_q,
  input  logic signed [32:0] product_q,
  input  logic               last2_q,
  input  logic               valid3_q,
  input  logic signed [15:0] data_q,
  input  logic               last3_q,
  input  logic               clip3_q
);
  logic p_rst_lo = 1'b0;
  always @(posedge clk) begin
    p_rst_lo <= !rst_n;
    if (!rst_n && p_rst_lo) begin
      a_g_v1q:    assert (valid1_q == 1'b0);
      a_g_c1q:    assert (centered_q == 17'sd0);
      a_g_l1q:    assert (last1_q == 1'b0);
      a_g_v2q:    assert (valid2_q == 1'b0);
      a_g_p2q:    assert (product_q == 33'sd0);
      a_g_l2q:    assert (last2_q == 1'b0);
      a_g_v3q:    assert (valid3_q == 1'b0);
      a_g_dataq:  assert (data_q == 16'sd0);
      a_g_l3q:    assert (last3_q == 1'b0);
      a_g_c3q:    assert (clip3_q == 1'b0);
    end
  end
endmodule
bind dsp_gain dsp_gain_reset_sva u_dsp_gain_reset (.*);

module dsp_fir_reset_sva (
  input  logic               clk,
  input  logic               rst_n,
  input  logic [1:0]         state_q,
  input  logic [15:0]        delay_q [dsp_pkg::MAX_TAPS]
);
  logic p_rst_lo = 1'b0;
  always @(posedge clk) begin
    p_rst_lo <= !rst_n;
    if (!rst_n && p_rst_lo) begin
      a_f_state: assert (state_q == 2'd0);
      for (int g = 0; g < dsp_pkg::MAX_TAPS; g++) begin
        a_f_dly: assert (delay_q[g] == 16'h0000);
      end
    end
  end
endmodule
bind dsp_fir dsp_fir_reset_sva u_dsp_fir_reset (.*);

module dsp_decim_reset_sva (
  input  logic               clk,
  input  logic               rst_n,
  input  logic [2:0]         phase_q,
  input  logic signed [18:0] sum_q,
  input  logic               clip_q,
  input  logic               valid_q,
  input  logic signed [15:0] data_q,
  input  logic               last_q,
  input  logic               oclip_q
);
  logic p_rst_lo = 1'b0;
  always @(posedge clk) begin
    p_rst_lo <= !rst_n;
    if (!rst_n && p_rst_lo) begin
      a_d_phase: assert (phase_q == 3'd0);
      a_d_sum:   assert (sum_q == 19'sd0);
      a_d_clip:  assert (clip_q == 1'b0);
      a_d_valid: assert (valid_q == 1'b0);
      a_d_data:  assert (data_q == 16'sd0);
      a_d_last:  assert (last_q == 1'b0);
      a_d_oclip: assert (oclip_q == 1'b0);
    end
  end
endmodule
bind dsp_decim dsp_decim_reset_sva u_dsp_decim_reset (.*);

module dsp_stats_reset_sva (
  input  logic               clk,
  input  logic               rst_n,
  input  logic [15:0]        in_q,
  input  logic [15:0]        out_q,
  input  logic [15:0]        clip_q,
  input  logic signed [15:0] max_q,
  input  logic signed [15:0] min_q
);
  logic p_rst_lo = 1'b0;
  always @(posedge clk) begin
    p_rst_lo <= !rst_n;
    if (!rst_n && p_rst_lo) begin
      a_s_in:   assert (in_q == 16'd0);
      a_s_out:  assert (out_q == 16'd0);
      a_s_clip: assert (clip_q == 16'd0);
      a_s_max:  assert (max_q == $signed(dsp_pkg::SAMPLE_MIN));
      a_s_min:  assert (min_q == $signed(dsp_pkg::SAMPLE_MAX));
    end
  end
endmodule
bind dsp_stats dsp_stats_reset_sva u_dsp_stats_reset (.*);

module dsp_regs_reset_sva (
  input  logic        clk,
  input  logic        rst_n,
  input  logic [6:0]  ctrl_q,
  input  logic [15:0] offset_q,
  input  logic [15:0] gain_q,
  input  logic [15:0] hi_q,
  input  logic [15:0] lo_q,
  input  logic [3:0]  ntaps_q,
  input  logic [1:0]  irq_stat_q,
  input  logic [1:0]  irq_en_q,
  input  logic [15:0] coef_q [dsp_pkg::MAX_TAPS]
);
  logic p_rst_lo = 1'b0;
  always @(posedge clk) begin
    p_rst_lo <= !rst_n;
    if (!rst_n && p_rst_lo) begin
      a_r_ctrl:   assert (ctrl_q == 7'b000_0010);
      a_r_offset: assert (offset_q == 16'h0000);
      a_r_gain:   assert (gain_q == dsp_pkg::GAIN_UNITY);
      a_r_hi:     assert (hi_q == dsp_pkg::SAMPLE_MAX);
      a_r_lo:     assert (lo_q == dsp_pkg::SAMPLE_MIN);
      a_r_ntaps:  assert (ntaps_q == 4'd1);
      a_r_istat:  assert (irq_stat_q == 2'b00);
      a_r_ien:    assert (irq_en_q == 2'b00);
      for (int g = 0; g < dsp_pkg::MAX_TAPS; g++) begin
        a_r_coef: assert (coef_q[g] == 16'h0000);
      end
    end
  end
endmodule
bind dsp_regs dsp_regs_reset_sva u_dsp_regs_reset (.*);

module dsp_fifo_reset_sva #(
  parameter int WIDTH = 18,
  parameter int DEPTH = 8
) (
  input  logic        clk,
  input  logic        rst_n,
  input  logic [$clog2(DEPTH)-1:0] wptr_q,
  input  logic [$clog2(DEPTH)-1:0] rptr_q,
  input  logic [$clog2(DEPTH):0]   count_q
);
  logic p_rst_lo = 1'b0;
  always @(posedge clk) begin
    p_rst_lo <= !rst_n;
    if (!rst_n && p_rst_lo) begin
      a_fifo_wptr:  assert (wptr_q == '0);
      a_fifo_rptr:  assert (rptr_q == '0);
      a_fifo_count: assert (count_q == '0);
    end
  end
endmodule
bind dsp_fifo dsp_fifo_reset_sva #(.WIDTH(WIDTH), .DEPTH(DEPTH)) u_dsp_fifo_reset (.*);

module dsp_stream_reset_sva (
  input  logic        clk,
  input  logic        rst_n,
  input  logic        m_valid,
  input  logic        irq,
  input  logic [15:0] last_len_q,
  input  logic [15:0] frame_cnt_q
);
  logic p_rst_lo = 1'b0;
  always @(posedge clk) begin
    p_rst_lo <= !rst_n;
    if (!rst_n && p_rst_lo) begin
      a_t_mvalid:  assert (m_valid == 1'b0);
      a_t_irq:     assert (irq == 1'b0);
      a_t_lastlen: assert (last_len_q == 16'd0);
      a_t_fcnt:    assert (frame_cnt_q == 16'd0);
    end
  end
endmodule
bind dsp_stream dsp_stream_reset_sva u_dsp_stream_reset (.*);

/* verilator lint_on SYNCASYNCNET */
