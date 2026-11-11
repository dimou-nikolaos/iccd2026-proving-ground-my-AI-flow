// dsp_limit formal verification (Phase A2).
// REQ-28/29/30 verified via immediate assertions.

module dsp_limit_sva (
  input  logic signed [15:0] lim_hi,
  input  logic signed [15:0] lim_lo,
  input  logic signed [15:0] in_data,
  input  logic signed [15:0] out_data,
  input  logic               clipped
);

  logic g_over, g_under;
  assign g_over  = (in_data > lim_hi);
  assign g_under = (in_data < lim_lo);

  logic signed [15:0] g_out;
  logic               g_clip;
  always_comb begin
    if (g_over) begin
      g_out  = lim_hi;
      g_clip = 1'b1;
    end else if (g_under) begin
      g_out  = lim_lo;
      g_clip = 1'b1;
    end else begin
      g_out  = in_data;
      g_clip = 1'b0;
    end
  end

  always_comb begin
    a_out_match: assert (out_data == g_out)
      else $error("out mismatch: hi=%0d lo=%0d in=%0d got=%0d expected=%0d", lim_hi, lim_lo, in_data, out_data, g_out);
    a_clip_match: assert (clipped == g_clip)
      else $error("clip mismatch: hi=%0d lo=%0d in=%0d got=%0b expected=%0b", lim_hi, lim_lo, in_data, clipped, g_clip);

    // REQ-28: in > LIM_HI → out=LIM_HI, clipped=1
    if (in_data > lim_hi) begin
      a_req28: assert (out_data == lim_hi && clipped)
        else $error("REQ-28 violated: hi=%0d lo=%0d in=%0d out=%0d clip=%0b", lim_hi, lim_lo, in_data, out_data, clipped);
    end

    // REQ-29: LIM_HI wins when window inverted (hi < lo, in > hi but in < lo)
    if ((lim_hi < lim_lo) && (in_data > lim_hi) && (in_data < lim_lo)) begin
      a_req29_inv: assert (out_data == lim_hi)
        else $error("REQ-29 inverted: hi=%0d lo=%0d in=%0d out=%0d", lim_hi, lim_lo, in_data, out_data);
    end

    // REQ-30: equal to a limit (within valid window) is not changed and not marked
    if ((in_data == lim_hi) && (lim_hi >= lim_lo)) begin
      a_req30_eq_hi: assert (!clipped && out_data == in_data)
        else $error("REQ-30 eq LIM_HI: hi=%0d in=%0d clip=%0b out=%0d", lim_hi, in_data, clipped, out_data);
    end
    if ((in_data == lim_lo) && (lim_hi >= lim_lo)) begin
      a_req30_eq_lo: assert (!clipped && out_data == in_data)
        else $error("REQ-30 eq LIM_LO: lo=%0d in=%0d clip=%0b out=%0d", lim_lo, in_data, clipped, out_data);
    end
  end

endmodule

bind dsp_limit dsp_limit_sva u_limit_sva (.*);
