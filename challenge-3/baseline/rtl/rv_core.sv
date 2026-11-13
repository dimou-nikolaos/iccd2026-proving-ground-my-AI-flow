// rv_core: five-stage RV32IM core with machine-mode traps.
//   fetch -> decode -> execute -> memory -> writeback
module rv_core (
  input  logic        clk,
  input  logic        rst_n,
  // Instruction memory
  output logic        imem_valid,
  input  logic        imem_ready,
  output logic [31:0] imem_addr,
  input  logic [31:0] imem_rdata,
  // Data memory
  output logic        dmem_valid,
  input  logic        dmem_ready,
  output logic        dmem_write,
  output logic [31:0] dmem_addr,
  output logic [3:0]  dmem_wstrb,
  output logic [31:0] dmem_wdata,
  input  logic [31:0] dmem_rdata,
  // Interrupts
  input  logic        irq_ext,
  input  logic        irq_timer
);
  import rv_pkg::*;

  // Decode stage.
  logic             id_valid;
  logic [31:0]      id_pc, id_instr;
  decoded_t         id_dec;
  logic [31:0]      id_rs1_val, id_rs2_val;

  // Execute stage registers.
  logic             ex_valid_q;
  logic [31:0]      ex_pc_q, ex_instr_q, ex_rs1_q, ex_rs2_q;
  decoded_t         ex_dec_q;

  // Memory stage registers.
  logic             mem_valid_q;
  logic [31:0]      mem_result_q, mem_store_q;
  logic [4:0]       mem_rd_q;
  logic             mem_wb_en_q, mem_is_load_q, mem_is_store_q, mem_unsigned_q;
  logic [1:0]       mem_size_q;

  // Writeback stage registers.
  logic             wb_valid_q, wb_en_q;
  logic [4:0]       wb_rd_q;
  logic [31:0]      wb_data_q;

  logic [31:0]      fwd_rs1, fwd_rs2;
  logic             load_use;
  logic             mem_stall, mdu_busy, stall_front, ex_fire;
  logic             redirect;
  logic [31:0]      redirect_pc;

  logic [31:0]      alu_a, alu_b, alu_result;
  logic             branch_taken;
  logic [31:0]      target, ex_result, ex_link;
  logic             target_misaligned;
  logic             flow_change;

  logic             ls_misaligned;
  logic [1:0]       ex_offset;

  logic             exc;
  logic [31:0]      exc_cause, exc_val;
  logic             irq_pending;
  logic [31:0]      irq_cause;
  logic             trap, mret;
  logic [31:0]      csr_rdata, csr_wdata, csr_mtvec, csr_mepc;
  logic             csr_illegal, csr_masked_zero;

  logic             mdu_start, mdu_done, mdu_active_q;
  logic [31:0]      mdu_result;

  logic [31:0]      load_data, mem_final;
  logic             unused_bits;

  assign unused_bits = &{1'b0, ex_dec_q};

  assign mem_stall   = dmem_valid && !dmem_ready;
  assign mdu_busy    = ex_valid_q && ex_dec_q.is_mdu && !mdu_done;
  assign stall_front = mem_stall || mdu_busy || load_use;
  assign ex_fire     = ex_valid_q && !mem_stall && !mdu_busy;

  rv_fetch u_fetch (
    .clk         (clk),
    .rst_n       (rst_n),
    .stall       (stall_front),
    .redirect    (redirect),
    .redirect_pc (redirect_pc),
    .imem_valid  (imem_valid),
    .imem_ready  (imem_ready),
    .imem_addr   (imem_addr),
    .imem_rdata  (imem_rdata),
    .id_valid    (id_valid),
    .id_pc       (id_pc),
    .id_instr    (id_instr)
  );

  rv_decode u_decode (
    .instr (id_instr),
    .dec   (id_dec)
  );

  rv_regfile u_regfile (
    .clk    (clk),
    .rst_n  (rst_n),
    .we     (wb_valid_q && wb_en_q),
    .waddr  (wb_rd_q),
    .wdata  (wb_data_q),
    .raddr1 (id_dec.rs1),
    .raddr2 (id_dec.rs2),
    .rdata1 (id_rs1_val),
    .rdata2 (id_rs2_val)
  );

  rv_hazard u_hazard (
    .ex_rs1      (ex_dec_q.rs1),
    .ex_rs2      (ex_dec_q.rs2),
    .ex_rs1_val  (ex_rs1_q),
    .ex_rs2_val  (ex_rs2_q),
    .fwd_rs1     (fwd_rs1),
    .fwd_rs2     (fwd_rs2),
    .mem_wb_en   (mem_valid_q && mem_wb_en_q),
    .mem_rd      (mem_rd_q),
    .mem_result  (mem_result_q),
    .wb_en       (wb_valid_q && wb_en_q),
    .wb_rd       (wb_rd_q),
    .wb_data     (wb_data_q),
    .id_valid    (id_valid),
    .id_uses_rs1 (id_dec.uses_rs1),
    .id_uses_rs2 (id_dec.uses_rs2),
    .id_rs1      (id_dec.rs1),
    .id_rs2      (id_dec.rs2),
    .ex_is_load  (ex_valid_q && ex_dec_q.is_load),
    .ex_rd       (ex_dec_q.rd),
    .load_use    (load_use)
  );

  // Decode to execute.
  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      ex_valid_q <= 1'b0;
      ex_pc_q    <= 32'd0;
      ex_instr_q <= 32'd0;
      ex_rs1_q   <= 32'd0;
      ex_rs2_q   <= 32'd0;
      ex_dec_q   <= '0;
    end else if (mem_stall || mdu_busy) begin
      // The instruction waits here: keep values that are forwarded this cycle.
      ex_rs1_q <= fwd_rs1;
      ex_rs2_q <= fwd_rs2;
    end else if (redirect || load_use) begin
      ex_valid_q <= 1'b0;
      ex_dec_q   <= '0;
    end else begin
      ex_valid_q <= id_valid;
      ex_pc_q    <= id_pc;
      ex_instr_q <= id_instr;
      ex_rs1_q   <= id_rs1_val;
      ex_rs2_q   <= id_rs2_val;
      ex_dec_q   <= id_valid ? id_dec : '0;
    end
  end

  // Execute.
  always_comb begin
    unique case (ex_dec_q.src_a)
      SRC_A_PC:   alu_a = ex_pc_q;
      SRC_A_ZERO: alu_a = 32'd0;
      default:    alu_a = fwd_rs1;
    endcase
  end
  assign alu_b = ex_dec_q.src_b_imm ? ex_dec_q.imm : fwd_rs2;

  rv_alu u_alu (
    .op     (ex_dec_q.alu_op),
    .a      (alu_a),
    .b      (alu_b),
    .result (alu_result)
  );

  always_comb begin
    unique case (ex_dec_q.branch_f3)
      3'b000:  branch_taken = (fwd_rs1 == fwd_rs2);
      3'b001:  branch_taken = (fwd_rs1 != fwd_rs2);
      3'b100:  branch_taken = ($signed(fwd_rs1) < $signed(fwd_rs2));
      3'b101:  branch_taken = ($signed(fwd_rs1) >= $signed(fwd_rs2));
      3'b110:  branch_taken = (fwd_rs1 < fwd_rs2);
      3'b111:  branch_taken = (fwd_rs1 >= fwd_rs2);
      default: branch_taken = 1'b0;
    endcase
  end

  always_comb begin
    if (ex_dec_q.is_jalr) begin
      target = (fwd_rs1 + ex_dec_q.imm) & 32'hFFFF_FFFE;
    end else begin
      target = ex_pc_q + ex_dec_q.imm;
    end
  end

  assign flow_change = ex_dec_q.is_jal || ex_dec_q.is_jalr || (ex_dec_q.is_branch && branch_taken);
  assign target_misaligned = flow_change && target[1];
  assign ex_link = ex_pc_q + 32'd4;

  assign ex_offset = alu_result[1:0];
  always_comb begin
    ls_misaligned = 1'b0;
    if (ex_dec_q.is_load || ex_dec_q.is_store) begin
      unique case (ex_dec_q.mem_size)
        2'b00:   ls_misaligned = 1'b0;
        2'b01:   ls_misaligned = ex_offset[0];
        default: ls_misaligned = (ex_offset != 2'b00);
      endcase
    end
  end

  assign csr_wdata       = ex_dec_q.csr_uses_imm ? {27'd0, ex_dec_q.rs1} : fwd_rs1;
  assign csr_masked_zero = (ex_dec_q.rs1 == 5'd0);

  always_comb begin
    exc       = 1'b1;
    exc_cause = CAUSE_ILLEGAL;
    exc_val   = 32'd0;
    if (ex_dec_q.illegal || csr_illegal) begin
      exc_cause = CAUSE_ILLEGAL;
      exc_val   = ex_instr_q;
    end else if (ex_dec_q.is_ecall) begin
      exc_cause = CAUSE_ECALL;
    end else if (ex_dec_q.is_ebreak) begin
      exc_cause = CAUSE_BREAKPOINT;
      exc_val   = ex_pc_q;
    end else if (target_misaligned) begin
      exc_cause = CAUSE_INSTR_MISALIGNED;
      exc_val   = target;
    end else if (ls_misaligned) begin
      exc_cause = ex_dec_q.is_load ? CAUSE_LOAD_MISALIGNED : CAUSE_STORE_MISALIGNED;
      exc_val   = alu_result;
    end else begin
      exc = 1'b0;
    end
  end

  assign trap = ex_fire && (irq_pending || exc);
  assign mret = ex_fire && !trap && ex_dec_q.is_mret;

  always_comb begin
    redirect    = 1'b0;
    redirect_pc = 32'd0;
    if (trap) begin
      redirect    = 1'b1;
      redirect_pc = csr_mtvec;
    end else if (mret) begin
      redirect    = 1'b1;
      redirect_pc = csr_mepc;
    end else if (ex_fire && flow_change) begin
      redirect    = 1'b1;
      redirect_pc = target;
    end
  end

  rv_csr u_csr (
    .clk               (clk),
    .rst_n             (rst_n),
    .op                (ex_valid_q ? ex_dec_q.csr_op : CSR_NONE),
    .commit            (ex_fire && !trap),
    .addr              (ex_dec_q.csr_addr),
    .wdata             (csr_wdata),
    .write_masked_zero (csr_masked_zero),
    .rdata             (csr_rdata),
    .illegal           (csr_illegal),
    .irq_ext           (irq_ext),
    .irq_timer         (irq_timer),
    .irq_pending       (irq_pending),
    .irq_cause         (irq_cause),
    .trap              (trap),
    .trap_pc           (ex_pc_q),
    .trap_cause        (irq_pending ? irq_cause : exc_cause),
    .trap_val          (irq_pending ? 32'd0 : exc_val),
    .mret              (mret),
    .mtvec             (csr_mtvec),
    .mepc              (csr_mepc),
    .retire            (ex_fire && !trap)
  );

  // Multiply and divide run in the execute stage.
  assign mdu_start = ex_valid_q && ex_dec_q.is_mdu && !mdu_active_q && !mem_stall;

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      mdu_active_q <= 1'b0;
    end else if (mdu_done && !mem_stall) begin
      mdu_active_q <= 1'b0;
    end else if (mdu_start) begin
      mdu_active_q <= 1'b1;
    end
  end

  rv_muldiv u_muldiv (
    .clk    (clk),
    .rst_n  (rst_n),
    .start  (mdu_start),
    .op     (ex_dec_q.mdu_op),
    .a      (fwd_rs1),
    .b      (fwd_rs2),
    .done   (mdu_done),
    .result (mdu_result)
  );

  always_comb begin
    if (ex_dec_q.is_jal || ex_dec_q.is_jalr) begin
      ex_result = ex_link;
    end else if (ex_dec_q.csr_op != CSR_NONE) begin
      ex_result = csr_rdata;
    end else if (ex_dec_q.is_mdu) begin
      ex_result = mdu_result;
    end else begin
      ex_result = alu_result;
    end
  end

  // Execute to memory.
  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      mem_valid_q    <= 1'b0;
      mem_result_q   <= 32'd0;
      mem_store_q    <= 32'd0;
      mem_rd_q       <= 5'd0;
      mem_wb_en_q    <= 1'b0;
      mem_is_load_q  <= 1'b0;
      mem_is_store_q <= 1'b0;
      mem_unsigned_q <= 1'b0;
      mem_size_q     <= 2'd0;
    end else if (!mem_stall) begin
      mem_valid_q <= ex_fire && !trap;
      if (ex_fire && !trap) begin
        mem_result_q   <= ex_result;
        mem_store_q    <= fwd_rs2;
        mem_rd_q       <= ex_dec_q.rd;
        mem_wb_en_q    <= ex_dec_q.wb_en;
        mem_is_load_q  <= ex_dec_q.is_load;
        mem_is_store_q <= ex_dec_q.is_store;
        mem_unsigned_q <= ex_dec_q.mem_unsigned;
        mem_size_q     <= ex_dec_q.mem_size;
      end
    end
  end

  rv_lsu u_lsu (
    .mem_valid   (mem_valid_q),
    .is_load     (mem_is_load_q),
    .is_store    (mem_is_store_q),
    .size        (mem_size_q),
    .is_unsigned (mem_unsigned_q),
    .addr        (mem_result_q),
    .store_data  (mem_store_q),
    .dmem_valid  (dmem_valid),
    .dmem_write  (dmem_write),
    .dmem_addr   (dmem_addr),
    .dmem_wstrb  (dmem_wstrb),
    .dmem_wdata  (dmem_wdata),
    .dmem_rdata  (dmem_rdata),
    .load_data   (load_data)
  );

  assign mem_final = mem_is_load_q ? load_data : mem_result_q;

  // Memory to writeback.
  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      wb_valid_q <= 1'b0;
      wb_en_q    <= 1'b0;
      wb_rd_q    <= 5'd0;
      wb_data_q  <= 32'd0;
    end else begin
      wb_valid_q <= mem_valid_q && !mem_stall;
      if (mem_valid_q && !mem_stall) begin
        wb_en_q   <= mem_wb_en_q;
        wb_rd_q   <= mem_rd_q;
        wb_data_q <= mem_final;
      end
    end
  end

endmodule
