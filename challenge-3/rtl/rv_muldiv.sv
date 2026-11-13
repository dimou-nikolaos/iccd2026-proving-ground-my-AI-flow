// rv_muldiv: iterative multiply and divide. A shift-add multiplier and a restoring
// divider share one 32-step sequencer and work on magnitudes.
module rv_muldiv (
  input  logic        clk,
  input  logic        rst_n,
  input  logic        start,
  input  logic [2:0]  op,
  input  logic [31:0] a,
  input  logic [31:0] b,
  output logic        done,
  output logic [31:0] result
);

  typedef enum logic [1:0] {S_IDLE, S_RUN, S_DONE} state_t;

  state_t      state_q;
  logic [5:0]  count_q;
  logic [63:0] acc_q;
  logic [31:0] op_b_q;
  logic [31:0] op_a_q;
  logic [2:0]  op_q;
  logic        res_neg_q, rem_neg_q;

  logic        is_div;
  logic        a_signed, b_signed;
  logic        a_neg, b_neg;
  logic [31:0] a_abs, b_abs;
  logic        run_div;
  logic [32:0] trial;
  logic [63:0] shifted;
  logic [32:0] mul_sum;
  logic [63:0] mul_out;
  logic [31:0] quo_out, rem_out;

  assign is_div   = op[2];
  assign a_signed = is_div ? !op[0] : (op[1:0] != 2'b11);
  assign b_signed = is_div ? !op[0] : (op[1:0] == 2'b01) || (op[1:0] == 2'b00);
  assign a_neg    = a_signed && a[31];
  assign b_neg    = b_signed && b[31];
  assign a_abs    = a_neg ? (~a + 32'd1) : a;
  assign b_abs    = b_neg ? (~b + 32'd1) : b;

  assign run_div = op_q[2];
  assign done    = (state_q == S_DONE);

  // Restoring divide step: acc_q[63:32] is the partial remainder, acc_q[31:0] the
  // remaining dividend bits and growing quotient.
  assign shifted = {acc_q[62:0], 1'b0};
  assign trial   = {1'b0, shifted[63:32]} - {1'b0, op_b_q};

  // Shift-add multiply step: acc_q[31:0] holds the remaining multiplier bits.
  assign mul_sum = {1'b0, acc_q[63:32]} + (acc_q[0] ? {1'b0, op_b_q} : 33'd0);

  assign mul_out = res_neg_q ? (~acc_q + 64'd1) : acc_q;
  assign quo_out = res_neg_q ? (~acc_q[31:0] + 32'd1) : acc_q[31:0];
  assign rem_out = (op_b_q == 32'd0) ? op_a_q : rem_neg_q ? (~acc_q[63:32] + 32'd1) : acc_q[63:32];

  always_comb begin
    unique case (op_q)
      3'b000:  result = mul_out[31:0];
      3'b001,
      3'b010,
      3'b011:  result = mul_out[63:32];
      3'b100,
      3'b101:  result = quo_out;
      default: result = rem_out;
    endcase
  end

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      state_q    <= S_IDLE;
      count_q    <= 6'd0;
      acc_q      <= 64'd0;
      op_b_q     <= 32'd0;
      op_a_q     <= 32'd0;
      op_q       <= 3'd0;
      res_neg_q  <= 1'b0;
      rem_neg_q  <= 1'b0;
    end else begin
      unique case (state_q)
        S_IDLE: begin
          if (start) begin
            state_q    <= S_RUN;
            count_q    <= 6'd32;
            op_q       <= op;
            op_b_q     <= b_abs;
            op_a_q     <= a;
            acc_q      <= {32'd0, a_abs};
            res_neg_q  <= (a_neg ^ b_neg) && (b != 32'd0);
            rem_neg_q  <= a_neg;
          end
        end
        S_RUN: begin
          count_q <= count_q - 6'd1;
          if (count_q == 6'd1) begin
            state_q <= S_DONE;
          end
          if (run_div) begin
            if (trial[32]) begin
              acc_q <= shifted;
            end else begin
              acc_q <= {trial[31:0], shifted[31:1], 1'b1};
            end
          end else begin
            acc_q <= {mul_sum, acc_q[31:1]};
          end
        end
        S_DONE: begin
          state_q <= S_IDLE;
        end
        default: begin
          state_q <= S_IDLE;
        end
      endcase
    end
  end

endmodule
