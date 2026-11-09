// Self-test bind file: attaches the stage observer to the synthetic harness.
//
// On release day this file is replaced by a 20-line map from the released core's
// internal signal names to the same observer ports. The observer never changes.
//
//   bind rv_core rv_stage_sva #(...) u_stage_sva (
//     .clk(clk), .rst_n(rst_n), ... );

bind stage_harness rv_stage_sva #(
    .IALIGN32     (1'b1),
    .CHECK_ILLEGAL(1'b1),
    .HAZARD_MODE  (1'b1),
    .CR_D3        (1'b1)
) u_stage_sva (
    .clk(clk),
    .rst_n(rst_n),

    .if_valid(if_valid),
    .if_pc(if_pc),
    .if_instr(if_instr),

    .id_valid(id_valid),
    .id_pc(id_pc),
    .id_instr(id_instr),
    .id_rs1(id_rs1_d),
    .id_rs2(id_rs2_d),
    .id_rd(id_rd_d),
    .id_imm(id_imm),
    .id_illegal(id_illegal),

    .ex_valid(ex_valid),
    .ex_pc(ex_pc),
    .ex_instr(ex_instr),
    .ex_a(ex_a),
    .ex_b(ex_b),
    .ex_alu(ex_alu),
    .ex_taken(ex_taken),
    .ex_target(ex_target),

    .mem_valid(mem_valid),
    .mem_instr(mem_instr),
    .mem_base(mem_base),
    .mem_addr(mem_addr),
    .mem_store_data(mem_store_data),
    .mem_we(mem_we),
    .mem_re(mem_re),
    .mem_size(mem_size),
    .mem_rdata(mem_rdata),
    .mem_load_data(mem_load_data),

    .wb_valid(wb_valid),
    .wb_instr(wb_instr),
    .wb_we(wb_we),
    .wb_rd(wb_rd),
    .wb_data(wb_data),

    .redirect_valid(ex_redirect),
    .redirect_pc(ex_target),
    .flush_ex(flush_ex),
    .flush_mem(1'b0),
    .flush_wb(1'b0)
);

// The harness has no explicit flush wires; these are the schedule facts the
// HAZARD_MODE assertions need.
// (Declared in the bind scope above as expressions is illegal, so the harness
//  provides `flush_ex` as a real signal - see stage_harness.sv.)
