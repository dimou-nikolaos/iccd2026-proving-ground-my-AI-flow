// dsp_rndsat formal verification (Phase A1)
// Independent golden model derived from spec text (REQ-14/15/16/17/18).
// SBY proves the RTL implementation matches the spec golden for ALL inputs.

module dsp_rndsat_sva #(
  parameter int IN_W = 40
) (
  input logic signed [IN_W-1:0] value,
  input logic [3:0]             shift,
  input logic [1:0]             mode,
  input logic signed [15:0]     result,
  input logic                   saturated
);

  // --- Golden model per spec text ---
  logic signed [IN_W-1:0] g_floor;
  assign g_floor = value >>> shift;

  logic [IN_W-1:0] g_dropped;
  logic [IN_W-1:0] g_half;
  logic            g_has_frac;
  logic [IN_W-1:0] g_mask;
  assign g_has_frac = (shift != 4'd0);
  assign g_mask = (shift == 4'd0) ? '0 : (({{(IN_W-1){1'b0}}, 1'b1} << shift) - {{(IN_W-1){1'b0}}, 1'b1});
  assign g_dropped = $unsigned(value) & g_mask;
  assign g_half    = g_has_frac ? ({{(IN_W-1){1'b0}}, 1'b1} << (shift - 4'd1)) : {IN_W{1'b0}};

  logic g_inc;
  always_comb begin
    unique case (mode)
      2'd0: g_inc = 1'b0;
      2'd1: g_inc = g_has_frac && (g_dropped >= g_half);
      2'd2: g_inc = g_has_frac && ((g_dropped > g_half) ||
                                   ((g_dropped == g_half) && g_floor[0]));
      2'd3: g_inc = g_has_frac && (g_dropped != '0) && g_floor[IN_W-1];
      default: g_inc = 1'b0;
    endcase
  end

  logic signed [IN_W:0] g_rounded;
  assign g_rounded = $signed({g_floor[IN_W-1], g_floor}) + $signed({{(IN_W){1'b0}}, g_inc});

  logic signed [IN_W:0] g_sat_hi, g_sat_lo;
  assign g_sat_hi = $signed({{(IN_W-15){1'b0}}, 16'sh7FFF});
  assign g_sat_lo = $signed({{(IN_W-15){1'b1}}, 16'sh8000});

  logic g_sat;
  logic signed [15:0] g_result;
  assign g_sat = (g_rounded > g_sat_hi) || (g_rounded < g_sat_lo);
  always_comb begin
    if      (g_rounded > g_sat_hi) g_result = 16'sh7FFF;
    else if (g_rounded < g_sat_lo) g_result = 16'sh8000;
    else                            g_result = g_rounded[15:0];
  end

  // --- Properties (immediate assertions, combinational) ---
  a_result_match: assert (result == g_result)
    else $error("result mismatch: value=%b shift=%0d mode=%0d result=%b expected=%b", value, shift, mode, result, g_result);
  a_sat_match: assert (saturated == g_sat)
    else $error("saturated mismatch: value=%b shift=%0d mode=%0d sat=%b expected=%b", value, shift, mode, saturated, g_sat);

endmodule

bind dsp_rndsat dsp_rndsat_sva #(.IN_W(IN_W)) u_rndsat_sva (.*);
