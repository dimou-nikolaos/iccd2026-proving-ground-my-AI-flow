// e22 explorer marker alt
// dsp_gain: subtracts the programmable offset and applies a Q8.8 gain.
// 3-stage pipeline: stage1 = subtract, stage2 = multiply, stage3 = rndsat.
module dsp_gain (
  input  logic               clk,
  input  logic               rst_n,
  input  logic signed [15:0] offset,
  input  logic signed [15:0] gain,
  input  logic [1:0]         round_mode,
  input  logic               in_valid,
  output logic               in_ready,
  input  logic signed [15:0] in_data,
  input  logic               in_last,
  output logic               out_valid,
  input  logic               out_ready,
  output logic signed [15:0] out_data,
  output logic               out_last,
  output logic               out_clip
);

  logic signed [16:0] centered_c;
  logic signed [32:0] product_c;
  logic signed [15:0] scaled;
  logic               sat;

  logic               valid1_q, last1_q;
  logic signed [16:0] centered_q;

  logic               valid2_q, last2_q;
  logic signed [32:0] product_q;

  logic               valid3_q, last3_q, clip3_q;
  logic signed [15:0] data_q;

  logic stage3_free, stage2_free, stage1_free;
  assign stage3_free = !valid3_q || out_ready;
  assign stage2_free = !valid2_q || stage3_free;
  assign stage1_free = !valid1_q || stage2_free;
  assign in_ready    = stage1_free;

  // W7: mult-only operand isolation (gate mult inputs when not firing)
  logic signed [16:0] centered_iso;
  logic signed [15:0] gain_iso;
  logic               mult_en;
  assign mult_en     = stage2_free && valid1_q;
  assign centered_iso = mult_en ? centered_q : 17'sd0;
  assign gain_iso     = mult_en ? gain : 16'sd0;

  assign centered_c = 17'(in_data) - 17'(offset);
  assign product_c  = centered_iso * 33'(gain_iso);

  dsp_rndsat #(.IN_W(33)) u_round (
    .value     (product_q),
    .shift     (4'd8),
    .mode      (round_mode),
    .result    (scaled),
    .saturated (sat)
  );

  assign out_valid = valid3_q;
  assign out_data  = data_q;
  assign out_last  = last3_q;
  assign out_clip  = clip3_q;

  // Stage 1: subtract
  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      valid1_q   <= 1'b0;
      centered_q <= 17'sd0;
      last1_q    <= 1'b0;
    end else if (stage1_free) begin
      valid1_q <= in_valid;
      if (in_valid) begin
        centered_q <= centered_c;
        last1_q    <= in_last;
      end
    end
  end

  // Stage 2: multiply
  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      valid2_q  <= 1'b0;
      product_q <= 33'sd0;
      last2_q   <= 1'b0;
    end else if (stage2_free) begin
      valid2_q <= valid1_q;
      if (valid1_q) begin
        product_q <= product_c;
        last2_q   <= last1_q;
      end
    end
  end

  // Stage 3: rndsat output
  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      valid3_q <= 1'b0;
      data_q   <= 16'sd0;
      last3_q  <= 1'b0;
      clip3_q  <= 1'b0;
    end else if (stage3_free) begin
      valid3_q <= valid2_q;
      if (valid2_q) begin
        data_q  <= scaled;
        last3_q <= last2_q;
        clip3_q <= sat;
      end
    end
  end

endmodule