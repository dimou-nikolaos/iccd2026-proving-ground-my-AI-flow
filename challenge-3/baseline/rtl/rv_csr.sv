// rv_csr: machine-mode control and status registers, trap entry and return.
module rv_csr (
  input  logic        clk,
  input  logic        rst_n,
  // CSR instruction, valid in the cycle it executes
  input  rv_pkg::csr_op_t op,
  input  logic        commit,
  input  logic [11:0] addr,
  input  logic [31:0] wdata,
  input  logic        write_masked_zero,
  output logic [31:0] rdata,
  output logic        illegal,
  // Interrupt lines
  input  logic        irq_ext,
  input  logic        irq_timer,
  output logic        irq_pending,
  output logic [31:0] irq_cause,
  // Trap entry and return
  input  logic        trap,
  input  logic [31:0] trap_pc,
  input  logic [31:0] trap_cause,
  input  logic [31:0] trap_val,
  input  logic        mret,
  output logic [31:0] mtvec,
  output logic [31:0] mepc,
  // Counters
  input  logic        retire
);
  import rv_pkg::*;

  logic        mie_q, mpie_q;
  logic        meie_q, mtie_q;
  logic [31:2] mtvec_q, mepc_q;
  logic [31:0] mscratch_q, mcause_q, mtval_q;
  logic [63:0] mcycle_q, minstret_q;

  logic        known, is_write, ro;
  logic [31:0] wr_val;
  logic [31:0] mstatus_rd, mie_rd, mip_rd;
  logic        do_write;
  logic        unused_bits;

  assign unused_bits = &{1'b0, trap_pc[1:0]};

  assign mstatus_rd = {19'd0, 2'b11, 3'd0, mpie_q, 3'd0, mie_q, 3'd0};
  assign mie_rd     = {20'd0, meie_q, 3'd0, mtie_q, 7'd0};
  assign mip_rd     = {20'd0, irq_ext, 3'd0, irq_timer, 7'd0};

  assign mtvec = {mtvec_q, 2'b00};
  assign mepc  = {mepc_q, 2'b00};

  assign irq_pending = mie_q && ((meie_q && irq_ext) || (mtie_q && irq_timer));
  assign irq_cause   = (meie_q && irq_ext) ? CAUSE_IRQ_EXT : CAUSE_IRQ_TIMER;

  always_comb begin
    known = 1'b1;
    rdata = 32'd0;
    unique case (addr)
      CSR_MVENDORID, CSR_MARCHID, CSR_MIMPID, CSR_MHARTID: rdata = 32'd0;
      CSR_MSTATUS:   rdata = mstatus_rd;
      CSR_MISA:      rdata = MISA_VALUE;
      CSR_MIE:       rdata = mie_rd;
      CSR_MTVEC:     rdata = mtvec;
      CSR_MSCRATCH:  rdata = mscratch_q;
      CSR_MEPC:      rdata = mepc;
      CSR_MCAUSE:    rdata = mcause_q;
      CSR_MTVAL:     rdata = mtval_q;
      CSR_MIP:       rdata = mip_rd;
      CSR_MCYCLE:    rdata = mcycle_q[31:0];
      CSR_MCYCLEH:   rdata = mcycle_q[63:32];
      CSR_MINSTRET:  rdata = minstret_q[31:0];
      CSR_MINSTRETH: rdata = minstret_q[63:32];
      default:       known = 1'b0;
    endcase
  end

  assign ro       = (addr[11:10] == 2'b11);
  assign is_write = (op == CSR_RW) || !write_masked_zero;
  assign illegal  = (op != CSR_NONE) && (!known || (ro && is_write));
  assign do_write = commit && (op != CSR_NONE) && is_write;

  always_comb begin
    unique case (op)
      CSR_RW:  wr_val = wdata;
      CSR_RS:  wr_val = rdata | wdata;
      CSR_RC:  wr_val = rdata & ~wdata;
      default: wr_val = rdata;
    endcase
  end

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      mie_q      <= 1'b0;
      mpie_q     <= 1'b0;
      meie_q     <= 1'b0;
      mtie_q     <= 1'b0;
      mtvec_q    <= 30'd0;
      mepc_q     <= 30'd0;
      mscratch_q <= 32'd0;
      mcause_q   <= 32'd0;
      mtval_q    <= 32'd0;
    end else if (trap) begin
      mpie_q   <= mie_q;
      mie_q    <= 1'b0;
      mepc_q   <= trap_pc[31:2];
      mcause_q <= trap_cause;
      mtval_q  <= trap_val;
    end else if (mret) begin
      mie_q  <= mpie_q;
    end else if (do_write) begin
      unique case (addr)
        CSR_MSTATUS: begin
          mie_q  <= wr_val[3];
          mpie_q <= wr_val[7];
        end
        CSR_MIE: begin
          meie_q <= wr_val[11];
          mtie_q <= wr_val[7];
        end
        CSR_MTVEC:    mtvec_q    <= wr_val[31:2];
        CSR_MSCRATCH: mscratch_q <= wr_val;
        CSR_MEPC:     mepc_q     <= wr_val[31:2];
        CSR_MCAUSE:   mcause_q   <= wr_val;
        CSR_MTVAL:    mtval_q    <= wr_val;
        default: ;
      endcase
    end
  end

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      mcycle_q   <= 64'd0;
      minstret_q <= 64'd0;
    end else begin
      if (do_write && (addr == CSR_MCYCLE)) begin
        mcycle_q[31:0] <= wr_val;
      end else if (do_write && (addr == CSR_MCYCLEH)) begin
        mcycle_q[63:32] <= wr_val;
      end else begin
        mcycle_q <= mcycle_q + 64'd1;
      end
      if (do_write && (addr == CSR_MINSTRET)) begin
        minstret_q[31:0] <= wr_val;
      end else if (do_write && (addr == CSR_MINSTRETH)) begin
        minstret_q[63:32] <= wr_val;
      end else if (retire) begin
        minstret_q <= minstret_q + 64'd1;
      end
    end
  end

endmodule
