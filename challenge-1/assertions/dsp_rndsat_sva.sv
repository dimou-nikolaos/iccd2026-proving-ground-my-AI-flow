module dsp_rndsat_sva #(
  parameter int IN_W = 40
) (
  input  logic signed [IN_W-1:0] value,
  input  logic [3:0]             shift,
  input  logic [1:0]             mode,
  input  logic signed [15:0]     result,
  input  logic                   saturated,
  input  logic signed [IN_W-1:0] kept,
  input  logic [IN_W-1:0]        mask,
  input  logic [IN_W-1:0]        dropped,
  input  logic [IN_W-1:0]        half,
  input  logic                   has_frac,
  input  logic                   frac_ge_half,
  input  logic                   frac_gt_half,
  input  logic                   inc,
  input  logic signed [IN_W:0]   rounded,
  input  logic                   over_hi,
  input  logic                   over_lo
);

  always_comb begin
    if (!$isunknown({value, shift, mode, result, saturated, kept, mask, dropped, half, has_frac, frac_ge_half, inc, rounded, over_hi, over_lo})) begin
      a_kept_def: assert (kept == (value >>> shift))
        else $error("kept def wrong");
      a_mask_def: assert (mask == ~(({IN_W{1'b1}} << shift)))
        else $error("mask def wrong");
      a_dropped_def: assert (dropped == ($unsigned(value) & mask))
        else $error("dropped def wrong");
      a_hasfrac_def: assert (has_frac == (shift != 4'd0))
        else $error("has_frac def wrong");
      a_half_def: assert (half == ({{(IN_W-1){1'b0}}, has_frac} << (shift - 4'd1)))
        else $error("half def wrong");
      a_fgh_def: assert (frac_ge_half == (has_frac && (dropped >= half)))
        else $error("frac_ge_half def wrong");
      a_inc_def: assert (inc == ((mode == 2'd0) ? 1'b0
                              : (mode == 2'd1) ? frac_ge_half
                              : (mode == 2'd2) ? (frac_ge_half && (frac_gt_half || kept[0]))
                              :                    ((dropped != '0) && kept[IN_W-1])))
        else $error("inc def wrong");
      a_rounded_def: assert (rounded == ($signed({kept[IN_W-1], kept}) + $signed({{IN_W{1'b0}}, inc})))
        else $error("rounded def wrong");
      a_overhi_def: assert (over_hi == (rounded > $signed((IN_W+1)'(32767))))
        else $error("over_hi def wrong");
      a_overlo_def: assert (over_lo == (rounded < -$signed((IN_W+1)'(32768))))
        else $error("over_lo def wrong");
      a_sat_def: assert (saturated == (over_hi || over_lo))
        else $error("saturated def wrong");
      a_result_def: assert (result == (over_hi ? 16'sh7FFF : (over_lo ? 16'sh8000 : rounded[15:0])))
        else $error("result def wrong");
    end
  end

endmodule

bind dsp_rndsat dsp_rndsat_sva #(.IN_W(IN_W)) u_dsp_rndsat_sva (.*);
