module dsp_stats_sva (
  input  logic               clk,
  input  logic               rst_n,
  input  logic               clear,
  input  logic               in_fire,
  input  logic               out_fire,
  input  logic signed [15:0] out_data,
  input  logic               clip_event,
  input  logic [15:0]        stat_in,
  input  logic [15:0]        stat_out,
  input  logic [15:0]        stat_clip,
  input  logic [15:0]        peak_max,
  input  logic [15:0]        peak_min,
  input  logic [15:0]        in_q,
  input  logic [15:0]        out_q,
  input  logic [15:0]        clip_q,
  input  logic signed [15:0] max_q,
  input  logic signed [15:0] min_q
);

  logic        p_clear, p_in_fire, p_out_fire, p_clip_event;
  logic signed [15:0] p_out_data;
  logic [15:0] p_in, p_out, p_clip;
  logic signed [15:0] p_max, p_min;
  logic        p_rst = 1'b0;

  always_comb begin
    if (!$isunknown({stat_in, stat_out, stat_clip, peak_max, peak_min, in_q, out_q, clip_q, max_q, min_q})) begin
      a_stat_in_def: assert (stat_in == in_q)
        else $error("stats stat_in def wrong");
      a_stat_out_def: assert (stat_out == out_q)
        else $error("stats stat_out def wrong");
      a_stat_clip_def: assert (stat_clip == clip_q)
        else $error("stats stat_clip def wrong");
      a_peak_max_def: assert (peak_max == max_q)
        else $error("stats peak_max def wrong");
      a_peak_min_def: assert (peak_min == min_q)
        else $error("stats peak_min def wrong");
    end
  end

  always @(posedge clk) begin
    if (p_rst && rst_n) begin
      a_inq_ns: assert (in_q == (p_clear ? 16'd0
                       : (p_in_fire && (p_in != 16'hFFFF)) ? (p_in + 16'd1)
                       : p_in))
        else $error("stats in_q next-state wrong");
      a_outq_ns: assert (out_q == (p_clear ? 16'd0
                        : (p_out_fire && (p_out != 16'hFFFF)) ? (p_out + 16'd1)
                        : p_out))
        else $error("stats out_q next-state wrong");
      a_clipq_ns: assert (clip_q == (p_clear ? 16'd0
                          : (p_clip_event && (p_clip != 16'hFFFF)) ? (p_clip + 16'd1)
                          : p_clip))
        else $error("stats clip_q next-state wrong");
      a_maxq_ns: assert (max_q == (p_clear ? $signed(dsp_pkg::SAMPLE_MIN)
                         : (p_out_fire && (p_out_data > p_max)) ? p_out_data
                         : p_max))
        else $error("stats max_q next-state wrong");
      a_minq_ns: assert (min_q == (p_clear ? $signed(dsp_pkg::SAMPLE_MAX)
                         : (p_out_fire && (p_out_data < p_min)) ? p_out_data
                         : p_min))
        else $error("stats min_q next-state wrong");
    end
    p_clear      <= clear;
    p_in_fire    <= in_fire;
    p_out_fire   <= out_fire;
    p_clip_event <= clip_event;
    p_out_data   <= out_data;
    p_in         <= in_q;
    p_out        <= out_q;
    p_clip       <= clip_q;
    p_max        <= max_q;
    p_min        <= min_q;
    p_rst        <= rst_n;
  end

endmodule

bind dsp_stats dsp_stats_sva u_dsp_stats_sva (.*);
