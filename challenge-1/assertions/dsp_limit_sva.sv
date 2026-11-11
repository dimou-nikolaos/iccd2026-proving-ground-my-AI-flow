module dsp_limit_sva (
  input  logic signed [15:0] lim_hi,
  input  logic signed [15:0] lim_lo,
  input  logic signed [15:0] in_data,
  input  logic signed [15:0] out_data,
  input  logic               clipped,
  input  logic               over,
  input  logic               under
);

  always_comb begin
    if (!$isunknown({lim_hi, lim_lo, in_data, out_data, clipped, over, under})) begin
      a_over_def: assert (over == (in_data > lim_hi))
        else $error("over def wrong");
      a_under_def: assert (under == (in_data < lim_lo))
        else $error("under def wrong");
      a_clipped_def: assert (clipped == (over || under))
        else $error("clipped def wrong");
      a_outdata_def: assert (out_data == (over ? lim_hi : (under ? lim_lo : in_data)))
        else $error("out_data def wrong");
    end
  end

endmodule

bind dsp_limit dsp_limit_sva u_dsp_limit_sva (.*);
