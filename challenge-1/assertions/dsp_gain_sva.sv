module dsp_gain_sva (
  input  logic               clk,
  input  logic               rst_n,
  input  logic signed [15:0] offset,
  input  logic signed [15:0] gain,
  input  logic               in_valid,
  input  logic               in_ready,
  input  logic signed [15:0] in_data,
  input  logic               in_last,
  input  logic               out_valid,
  input  logic               out_ready,
  input  logic signed [15:0] out_data,
  input  logic               out_last,
  input  logic               out_clip,
  input  logic signed [16:0] centered_c,
  input  logic signed [32:0] product_c,
  input  logic signed [16:0] centered_q,
  input  logic signed [32:0] product_q,
  input  logic signed [15:0] scaled,
  input  logic               sat,
  input  logic               valid1_q,
  input  logic               last1_q,
  input  logic               valid2_q,
  input  logic               last2_q,
  input  logic               valid3_q,
  input  logic signed [15:0] data_q,
  input  logic               last3_q,
  input  logic               clip3_q,
  input  logic               stage3_free,
  input  logic               stage2_free,
  input  logic               stage1_free,
  input  logic               mult_en
);

  logic               p_in_ready, p_in_valid, p_in_last, p_sat, p_out_ready;
  logic               p_stage3_free, p_stage2_free, p_stage1_free;
  logic signed [15:0] p_scaled;
  logic signed [16:0] p_centered_c;
  logic signed [16:0] p_centered_q;
  logic signed [32:0] p_product_c;
  logic signed [32:0] p_product_q;
  logic               p_valid1_q, p_last1_q, p_valid2_q, p_last2_q;
  logic               p_valid3_q, p_last3_q, p_clip3_q;
  logic signed [15:0] p_data_q;
  logic               p_rst = 1'b0;

  always_comb begin
    if (!$isunknown({offset, gain, in_data, in_ready, out_valid, out_data, out_last, out_clip, centered_c, product_c, centered_q, product_q, scaled, sat, valid1_q, last1_q, valid2_q, last2_q, valid3_q, data_q, last3_q, clip3_q, stage3_free, stage2_free, stage1_free})) begin
      a_centeredc_def: assert (centered_c == (17'(in_data) - 17'(offset)))
        else $error("gain centered_c def wrong");
      a_productc_def: assert (product_c == ((mult_en ? centered_q : 17'sd0) * 33'(mult_en ? gain : 16'sd0)))
        else $error("gain product_c def wrong");
      a_stage3free_def: assert (stage3_free == (!valid3_q || out_ready))
        else $error("gain stage3_free def wrong");
      a_stage2free_def: assert (stage2_free == (!valid2_q || stage3_free))
        else $error("gain stage2_free def wrong");
      a_stage1free_def: assert (stage1_free == (!valid1_q || stage2_free))
        else $error("gain stage1_free def wrong");
      a_inready_def: assert (in_ready == stage1_free)
        else $error("gain in_ready def wrong");
      a_outvalid_def: assert (out_valid == valid3_q)
        else $error("gain out_valid def wrong");
      a_outdata_def: assert (out_data == data_q)
        else $error("gain out_data def wrong");
      a_outlast_def: assert (out_last == last3_q)
        else $error("gain out_last def wrong");
      a_outclip_def: assert (out_clip == clip3_q)
        else $error("gain out_clip def wrong");
    end
  end

  always @(posedge clk) begin
    if (p_rst && rst_n) begin
      a_v1q_ns: assert (valid1_q == (p_stage1_free ? p_in_valid : p_valid1_q))
        else $error("gain valid1_q next-state wrong");
      a_c1q_ns: assert (centered_q == ((p_stage1_free && p_in_valid) ? p_centered_c : p_centered_q))
        else $error("gain centered_q next-state wrong");
      a_l1q_ns: assert (last1_q == ((p_stage1_free && p_in_valid) ? p_in_last : p_last1_q))
        else $error("gain last1_q next-state wrong");
      a_v2q_ns: assert (valid2_q == (p_stage2_free ? p_valid1_q : p_valid2_q))
        else $error("gain valid2_q next-state wrong");
      a_p2q_ns: assert (product_q == ((p_stage2_free && p_valid1_q) ? p_product_c : p_product_q))
        else $error("gain product_q next-state wrong");
      a_l2q_ns: assert (last2_q == ((p_stage2_free && p_valid1_q) ? p_last1_q : p_last2_q))
        else $error("gain last2_q next-state wrong");
      a_v3q_ns: assert (valid3_q == (p_stage3_free ? p_valid2_q : p_valid3_q))
        else $error("gain valid3_q next-state wrong");
      a_dataq_ns: assert (data_q == (p_stage3_free ? (p_valid2_q ? p_scaled : p_data_q) : p_data_q))
        else $error("gain data_q next-state wrong");
      a_l3q_ns: assert (last3_q == ((p_stage3_free && p_valid2_q) ? p_last2_q : p_last3_q))
        else $error("gain last3_q next-state wrong");
      a_c3q_ns: assert (clip3_q == ((p_stage3_free && p_valid2_q) ? p_sat : p_clip3_q))
        else $error("gain clip3_q next-state wrong");
    end
    p_in_ready      <= in_ready;
    p_in_valid      <= in_valid;
    p_in_last       <= in_last;
    p_sat           <= sat;
    p_scaled        <= scaled;
    p_centered_c    <= centered_c;
    p_centered_q    <= centered_q;
    p_product_c     <= product_c;
    p_product_q     <= product_q;
    p_valid1_q      <= valid1_q;
    p_last1_q       <= last1_q;
    p_valid2_q      <= valid2_q;
    p_last2_q       <= last2_q;
    p_valid3_q      <= valid3_q;
    p_data_q        <= data_q;
    p_last3_q       <= last3_q;
    p_clip3_q       <= clip3_q;
    p_stage3_free   <= stage3_free;
    p_stage2_free   <= stage2_free;
    p_stage1_free   <= stage1_free;
    p_rst           <= rst_n;
  end

endmodule

bind dsp_gain dsp_gain_sva u_dsp_gain_sva (.*);
