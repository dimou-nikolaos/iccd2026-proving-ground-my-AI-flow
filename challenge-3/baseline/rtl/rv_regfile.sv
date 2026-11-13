// rv_regfile: 31 general registers with write-through reads. x0 reads as zero.
module rv_regfile (
  input  logic        clk,
  input  logic        rst_n,
  input  logic        we,
  input  logic [4:0]  waddr,
  input  logic [31:0] wdata,
  input  logic [4:0]  raddr1,
  input  logic [4:0]  raddr2,
  output logic [31:0] rdata1,
  output logic [31:0] rdata2
);

  logic [31:0] regs_q [1:31];

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      for (int i = 1; i < 32; i++) begin
        regs_q[i] <= 32'd0;
      end
    end else if (we && (waddr != 5'd0)) begin
      regs_q[waddr] <= wdata;
    end
  end

  always_comb begin
    if (raddr1 == 5'd0) begin
      rdata1 = 32'd0;
    end else if (we && (waddr == raddr1)) begin
      rdata1 = wdata;
    end else begin
      rdata1 = regs_q[raddr1];
    end
  end

  always_comb begin
    if (raddr2 == 5'd0) begin
      rdata2 = 32'd0;
    end else if (we && (waddr == raddr2)) begin
      rdata2 = wdata;
    end else begin
      rdata2 = regs_q[raddr2];
    end
  end

endmodule
