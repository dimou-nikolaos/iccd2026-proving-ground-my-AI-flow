// rv_alu: arithmetic and logic unit.
module rv_alu (
  input  rv_pkg::alu_op_t op,
  input  logic [31:0]     a,
  input  logic [31:0]     b,
  output logic [31:0]     result
);
  import rv_pkg::*;

  logic [4:0] shamt;
  logic       lt_signed, lt_unsigned;

  assign shamt       = b[4:0];
  assign lt_signed   = ($signed(a) < $signed(b));
  assign lt_unsigned = (a < b);

  always_comb begin
    unique case (op)
      ALU_ADD:    result = a + b;
      ALU_SUB:    result = a - b;
      ALU_SLL:    result = a << shamt;
      ALU_SLT:    result = {31'd0, lt_signed};
      ALU_SLTU:   result = {31'd0, lt_unsigned};
      ALU_XOR:    result = a ^ b;
      ALU_SRL:    result = a >> shamt;
      ALU_SRA:    result = $unsigned($signed(a) >>> shamt);
      ALU_OR:     result = a | b;
      ALU_AND:    result = a & b;
      ALU_SH1ADD: result = (a << 1) + b;
      ALU_SH2ADD: result = (a << 2) + b;
      ALU_SH3ADD: result = (a << 3) + b;
      ALU_MIN:    result = lt_signed   ? a : b;
      ALU_MINU:   result = lt_unsigned ? a : b;
      ALU_MAX:    result = lt_signed   ? b : a;
      ALU_MAXU:   result = lt_unsigned ? b : a;
      default:    result = 32'd0;
    endcase
  end

endmodule
