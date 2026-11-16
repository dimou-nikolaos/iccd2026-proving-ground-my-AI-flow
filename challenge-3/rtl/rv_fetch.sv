// rv_fetch: program counter and instruction fetch. The request address holds until
// imem_ready; a redirect during a wait is remembered and the late word discarded.
module rv_fetch (
  input  logic        clk,
  input  logic        rst_n,
  input  logic        stall,
  input  logic        redirect,
  input  logic [31:0] redirect_pc,
  output logic        imem_valid,
  input  logic        imem_ready,
  output logic [31:0] imem_addr,
  input  logic [31:0] imem_rdata,
  output logic        id_valid,
  output logic [31:0] id_pc,
  output logic [31:0] id_instr
);

  logic [31:0] pc_q, pend_pc_q;
  logic        pend_q;
  logic        started_q;

  logic        id_valid_q;
  logic [31:0] id_pc_q, id_instr_q;

  assign imem_valid = started_q;
  assign imem_addr  = pc_q;
  assign id_valid   = id_valid_q;
  assign id_pc      = id_pc_q;
  assign id_instr   = id_instr_q;

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      pc_q      <= 32'd0;
      pend_pc_q <= 32'd0;
      pend_q    <= 1'b0;
      started_q <= 1'b0;
    end else begin
      started_q <= 1'b1;
      if (redirect) begin
        if (imem_ready) begin
          pc_q   <= redirect_pc;
          pend_q <= 1'b0;
        end else begin
          pend_q    <= 1'b1;
          pend_pc_q <= redirect_pc;
        end
      end else if (pend_q) begin
        if (imem_ready) begin
          pc_q   <= pend_pc_q;
          pend_q <= 1'b0;
        end
      end else if (started_q && imem_ready && !stall) begin
        pc_q <= pc_q + 32'd4;
      end
    end
  end

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      id_valid_q <= 1'b0;
      id_pc_q    <= 32'd0;
      id_instr_q <= 32'h0000_0013;
    end else if (redirect || pend_q) begin
      id_valid_q <= 1'b0;
    end else if (!stall) begin
      id_valid_q <= started_q && imem_ready;
      if (started_q && imem_ready) begin
        id_pc_q    <= pc_q;
        id_instr_q <= imem_rdata;
      end
    end
  end

endmodule
