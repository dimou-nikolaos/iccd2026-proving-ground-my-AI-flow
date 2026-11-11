module dsp_decim_sva (
  input  logic               clk,
  input  logic               rst_n,
  input  logic [1:0]         decim,
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
  input  logic [2:0]         phase_q,
  input  logic signed [18:0] sum_q,
  input  logic               clip_q,
  input  logic               valid_q,
  input  logic signed [15:0] data_q,
  input  logic               last_q,
  input  logic               oclip_q,
  input  logic [2:0]         group_max,
  input  logic               group_full,
  input  logic               close_group,
  input  logic               accept,
  input  logic signed [18:0] sum_next,
  input  logic signed [15:0] avg,
  input  logic               sat
);

  logic        p_accept, p_close, p_in_clip, p_in_ready, p_in_valid, p_in_last, p_sat;
  logic [2:0]  p_phase;
  logic signed [18:0] p_sum, p_sumnext;
  logic        p_clip, p_valid, p_last, p_oclip;
  logic signed [15:0] p_data, p_avg;
  logic        p_rst = 1'b0;

  always_comb begin
    if (!$isunknown({decim, in_valid, in_ready, in_data, in_last, in_clip, out_valid, out_ready,
                     out_data, out_last, out_clip, phase_q, sum_q, clip_q, valid_q, data_q,
                     last_q, oclip_q, group_max, group_full, close_group, accept, sum_next, avg, sat})) begin
      a_gmax_def: assert (group_max == ((3'b001 << decim) - 3'b001))
        else $error("decim group_max def wrong");
      a_gfull_def: assert (group_full == (phase_q == group_max))
        else $error("decim group_full def wrong");
      a_close_def: assert (close_group == (group_full || in_last))
        else $error("decim close_group def wrong");
      a_sumnext_def: assert (sum_next == (sum_q + 19'(in_data)))
        else $error("decim sum_next def wrong");
      a_inready_def: assert (in_ready == (!valid_q || out_ready))
        else $error("decim in_ready def wrong");
      a_accept_def: assert (accept == (in_valid && in_ready))
        else $error("decim accept def wrong");
      a_outvalid_def: assert (out_valid == valid_q)
        else $error("decim out_valid def wrong");
      a_outdata_def: assert (out_data == data_q)
        else $error("decim out_data def wrong");
      a_outlast_def: assert (out_last == last_q)
        else $error("decim out_last def wrong");
      a_outclip_def: assert (out_clip == oclip_q)
        else $error("decim out_clip def wrong");
    end
  end

  always @(posedge clk) begin
    if (p_rst && rst_n) begin
      a_phaseq_ns: assert (phase_q == (p_accept ? (p_close ? 3'd0 : (p_phase + 3'd1)) : p_phase))
        else $error("decim phase_q next-state wrong");
      a_sumq_ns: assert (sum_q == (p_accept ? (p_close ? 19'sd0 : p_sumnext) : p_sum))
        else $error("decim sum_q next-state wrong");
      a_clipq_ns: assert (clip_q == (p_accept ? (p_close ? 1'b0 : (p_clip || p_in_clip)) : p_clip))
        else $error("decim clip_q next-state wrong");
      a_validq_ns: assert (valid_q == (p_in_ready ? (p_in_valid && p_close) : p_valid))
        else $error("decim valid_q next-state wrong");
      a_dataq_ns: assert (data_q == ((p_in_ready && p_in_valid && p_close) ? p_avg : p_data))
        else $error("decim data_q next-state wrong");
      a_lastq_ns: assert (last_q == ((p_in_ready && p_in_valid && p_close) ? p_in_last : p_last))
        else $error("decim last_q next-state wrong");
      a_oclipq_ns: assert (oclip_q == ((p_in_ready && p_in_valid && p_close) ? (p_clip || p_in_clip || p_sat) : p_oclip))
        else $error("decim oclip_q next-state wrong");
    end
    p_accept   <= accept;
    p_close    <= close_group;
    p_in_clip  <= in_clip;
    p_in_ready <= in_ready;
    p_in_valid <= in_valid;
    p_in_last  <= in_last;
    p_sat      <= sat;
    p_phase    <= phase_q;
    p_sum      <= sum_q;
    p_sumnext  <= sum_next;
    p_clip     <= clip_q;
    p_valid    <= valid_q;
    p_data     <= data_q;
    p_last     <= last_q;
    p_oclip    <= oclip_q;
    p_avg      <= avg;
    p_rst      <= rst_n;
  end

endmodule

bind dsp_decim dsp_decim_sva u_dsp_decim_sva (.*);
