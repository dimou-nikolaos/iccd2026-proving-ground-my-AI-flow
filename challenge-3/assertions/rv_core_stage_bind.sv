// bind the ISA stage-contract observer to the real rv_core pipeline taps
bind rv_core rv_stage_sva #(
    .IALIGN32     (1'b1),
    .CHECK_ILLEGAL(1'b0),
    .HAZARD_MODE  (1'b0),
    .CR_D3        (1'b1)
) u_stage_sva (
    .clk            (clk),
    .rst_n          (rst_n),

    .if_valid       (1'b0),
    .if_pc          (32'd0),
    .if_instr       (32'd0),

    .id_valid       (id_valid),
    .id_pc          (id_pc),
    .id_instr       (id_instr),
    .id_rs1         (id_dec.rs1),
    .id_rs2         (id_dec.rs2),
    .id_rd          (id_dec.rd),
    .id_imm         (id_dec.imm),
    .id_illegal     (id_dec.illegal),

    .ex_valid       (ex_valid_q && !ex_dec_q.is_mdu &&
                     !((ex_dec_q.is_branch || ex_dec_q.is_jal) && target[1])),
    .ex_pc          (ex_pc_q),
    .ex_instr       (ex_instr_q),
    .ex_a           (alu_a),
    .ex_b           (alu_b),
    .ex_alu         (alu_result),
    .ex_taken       (branch_taken),
    .ex_target      (target),

    .mem_valid      (1'b0),
    .mem_instr      (32'd0),
    .mem_base       (32'd0),
    .mem_addr       (32'd0),
    .mem_store_data (32'd0),
    .mem_we         (1'b0),
    .mem_re         (1'b0),
    .mem_size       (3'd0),
    .mem_rdata      (32'd0),
    .mem_load_data  (32'd0),

    .wb_valid       (1'b0),
    .wb_instr       (32'd0),
    .wb_we          (1'b0),
    .wb_rd          (5'd0),
    .wb_data        (32'd0),

    .redirect_valid (1'b0),
    .redirect_pc    (32'd0),
    .flush_ex       (1'b0),
    .flush_mem      (1'b0),
    .flush_wb       (1'b0)
);
