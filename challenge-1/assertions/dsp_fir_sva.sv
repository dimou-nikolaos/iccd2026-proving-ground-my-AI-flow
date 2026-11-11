module dsp_fir_sva (
  input  logic               clk,
  input  logic               rst_n,
  input  logic               bypass,
  input  logic               clear_on_last,
  input  logic [3:0]         ntaps,
  input  logic [15:0]        coef [dsp_pkg::MAX_TAPS],
  input  logic               in_valid,
  input  logic               in_ready,
  input  logic signed [15:0] in_data,
  input  logic               in_last,
  input  logic               in_clip,
  input  logic               out_valid,
  input  logic               out_ready,
  input  logic signed [15:0] out_data,
  input  logic               out_last,
  input  logic               out_clip,
  input  logic               active,
  input  logic [1:0]         state_q,
  input  logic [15:0]        delay_q [dsp_pkg::MAX_TAPS],
  input  logic [3:0]         idx_q,
  input  logic signed [31:0] prod_q,
  input  logic signed [39:0] acc_q,
  input  logic               last_q,
  input  logic               clip_q,
  input  logic               take,
  input  logic signed [15:0] filtered,
  input  logic               sat
);

  logic [1:0]  p_state;
  logic        p_take, p_ord, p_col, p_in_last, p_in_clip, p_rst = 1'b0;
  logic [3:0]  p_idx, p_nt;
  logic [15:0] p_in_data;
  logic signed [31:0] p_prod;
  logic signed [39:0] p_acc;
  logic        p_last, p_clip;
  logic [15:0] p_coef [dsp_pkg::MAX_TAPS];
  logic [15:0] p_delay [dsp_pkg::MAX_TAPS];

  always_comb begin
    if (!$isunknown({bypass, clear_on_last, ntaps, in_valid, in_ready, in_data, in_last, in_clip,
                     out_valid, out_ready, out_data, out_last, out_clip, active, state_q,
                     idx_q, prod_q, acc_q, last_q, clip_q, take, filtered, sat})) begin
      a_active_def: assert (active == (state_q != 2'd0))
        else $error("fir active def wrong");
      a_take_def: assert (take == (!bypass && in_valid && (state_q == 2'd0)))
        else $error("fir take def wrong");
      if (bypass && (state_q == 2'd0)) begin
        a_bypass_mux: assert ((in_ready == out_ready) && (out_valid == in_valid) &&
            (out_data == in_data) && (out_last == in_last) && (out_clip == in_clip))
          else $error("fir bypass mux wrong");
      end else begin
        a_normal_mux: assert ((in_ready == (state_q == 2'd0)) && (out_valid == (state_q == 2'd3)) &&
            (out_data == filtered) && (out_last == last_q) && (out_clip == (clip_q || sat)))
          else $error("fir normal mux wrong");
      end
    end
  end

  always @(posedge clk) begin
    if (p_rst && rst_n) begin
      a_state_ns: assert (state_q == ((p_state == 2'd0) ? (p_take ? 2'd1 : 2'd0)
                              : (p_state == 2'd1) ? 2'd2
                              : (p_state == 2'd2) ? ((p_idx + 4'd1 == p_nt) ? 2'd3 : 2'd1)
                              :                    (p_ord ? 2'd0 : 2'd3)))
        else $error("fir state_q next-state wrong");
      a_accq_ns: assert (acc_q == (p_take ? 40'sd0
                           : (p_state == 2'd2) ? (p_acc + 40'(p_prod))
                           : p_acc))
        else $error("fir acc_q next-state wrong");
      a_idxq_ns: assert (idx_q == (p_take ? 4'd0
                          : (p_state == 2'd2) ? (p_idx + 4'd1)
                          : p_idx))
        else $error("fir idx_q next-state wrong");
      a_prod_ns: assert (prod_q == (p_state == 2'd1
                          ? ($signed(p_coef[p_idx[2:0]]) * $signed(p_delay[p_idx[2:0]]))
                          : p_prod))
        else $error("fir prod_q next-state wrong");
      a_lastq_ns: assert (last_q == (p_take ? p_in_last : p_last))
        else $error("fir last_q next-state wrong");
      a_clipq_ns: assert (clip_q == (p_take ? p_in_clip : p_clip))
        else $error("fir clip_q next-state wrong");
      for (int gd = 0; gd < dsp_pkg::MAX_TAPS; gd++) begin
        a_dly_ns: assert (delay_q[gd] == (p_take ? ((gd == 0) ? p_in_data : p_delay[gd-1])
                            : ((p_state == 2'd3) && p_ord && p_last && p_col) ? 16'h0000
                            : p_delay[gd]))
          else $error("fir delay_q next-state wrong");
      end
    end
    p_state    <= state_q;
    p_take     <= take;
    p_ord      <= out_ready;
    p_col      <= clear_on_last;
    p_in_last  <= in_last;
    p_in_clip  <= in_clip;
    p_in_data  <= in_data;
    p_idx      <= idx_q;
    p_nt       <= ntaps;
    p_prod     <= prod_q;
    p_acc      <= acc_q;
    p_last     <= last_q;
    p_clip     <= clip_q;
    for (int i = 0; i < dsp_pkg::MAX_TAPS; i++) begin
      p_coef[i]  <= coef[i];
      p_delay[i] <= delay_q[i];
    end
    p_rst      <= rst_n;
  end

endmodule

bind dsp_fir dsp_fir_sva u_dsp_fir_sva (.*);
