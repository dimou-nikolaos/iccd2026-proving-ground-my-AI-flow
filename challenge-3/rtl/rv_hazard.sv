// rv_hazard: operand forwarding into the execute stage and the load-use interlock.
module rv_hazard (
  // Execute stage operands
  input  logic [4:0]  ex_rs1,
  input  logic [4:0]  ex_rs2,
  input  logic [31:0] ex_rs1_val,
  input  logic [31:0] ex_rs2_val,
  output logic [31:0] fwd_rs1,
  output logic [31:0] fwd_rs2,
  // Memory stage result
  input  logic        mem_wb_en,
  input  logic [4:0]  mem_rd,
  input  logic [31:0] mem_result,
  // Writeback stage result
  input  logic        wb_en,
  input  logic [4:0]  wb_rd,
  input  logic [31:0] wb_data,
  // Load-use interlock
  input  logic        id_valid,
  input  logic        id_uses_rs1,
  input  logic        id_uses_rs2,
  input  logic [4:0]  id_rs1,
  input  logic [4:0]  id_rs2,
  input  logic        ex_is_load,
  input  logic [4:0]  ex_rd,
  output logic        load_use
);

  always_comb begin
    if (mem_wb_en && (mem_rd == ex_rs1) && (ex_rs1 != 5'd0)) begin
      fwd_rs1 = mem_result;
    end else if (wb_en && (wb_rd == ex_rs1) && (ex_rs1 != 5'd0)) begin
      fwd_rs1 = wb_data;
    end else begin
      fwd_rs1 = ex_rs1_val;
    end
  end

  always_comb begin
    if (mem_wb_en && (mem_rd == ex_rs2) && (ex_rs2 != 5'd0)) begin
      fwd_rs2 = mem_result;
    end else if (wb_en && (wb_rd == ex_rs2) && (ex_rs2 != 5'd0)) begin
      fwd_rs2 = wb_data;
    end else begin
      fwd_rs2 = ex_rs2_val;
    end
  end

  assign load_use = id_valid && ex_is_load && (ex_rd != 5'd0) &&
                    ((id_uses_rs1 && (id_rs1 == ex_rd)) || (id_uses_rs2 && (id_rs2 == ex_rd)));

endmodule
