// signal-definition and next-state pins inside rv_core (kills arith/negate/relational/constant/drop_term mutations at rv_core wires)
/* verilator lint_off SYNCASYNCNET */
module rv_core_defs (
    input  logic        clk,
    input  logic        rst_n,
    input  logic        dmem_valid,
    input  logic        dmem_ready,
    input  logic [31:0] dmem_rdata,
    input  logic        id_valid,
    input  logic [31:0] id_instr,
    input  rv_pkg::decoded_t id_dec,
    input  logic        ex_valid_q,
    input  logic [31:0] ex_pc_q,
    input  logic [31:0] ex_rs1_q,
    input  logic [31:0] ex_rs2_q,
    input  rv_pkg::decoded_t ex_dec_q,
    input  logic        mem_valid_q,
    input  logic [31:0] mem_result_q,
    input  logic [4:0]  mem_rd_q,
    input  logic        mem_wb_en_q,
    input  logic        mem_is_load_q,
    input  logic [1:0]  mem_size_q,
    input  logic        mem_unsigned_q,
    input  logic        wb_valid_q,
    input  logic        wb_en_q,
    input  logic [4:0]  wb_rd_q,
    input  logic [31:0] wb_data_q,
    input  logic [31:0] fwd_rs1,
    input  logic [31:0] fwd_rs2,
    input  logic        load_use,
    input  logic        mem_stall,
    input  logic        mdu_busy,
    input  logic        stall_front,
    input  logic        ex_fire,
    input  logic        redirect,
    input  logic [31:0] redirect_pc,
    input  logic [31:0] alu_a,
    input  logic [31:0] alu_b,
    input  logic [31:0] alu_result,
    input  logic        branch_taken,
    input  logic [31:0] target,
    input  logic [31:0] ex_result,
    input  logic [31:0] ex_link,
    input  logic        target_misaligned,
    input  logic        flow_change,
    input  logic        ls_misaligned,
    input  logic        exc,
    input  logic        trap,
    input  logic        mret,
    input  logic        irq_pending,
    input  logic [31:0] csr_rdata,
    input  logic [31:0] csr_mtvec,
    input  logic [31:0] csr_mepc,
    input  logic        mdu_start,
    input  logic        mdu_done,
    input  logic        mdu_active_q,
    input  logic [31:0] mdu_result,
    input  logic [31:0] load_data,
    input  logic [31:0] mem_final
);
    import rv_pkg::*;

    default clocking cb @(posedge clk); endclocking
    default disable iff (!rst_n);

    logic [1:0]  ld_off;
    logic [7:0]  ld_byte;
    logic [15:0] ld_half;
    logic [31:0] ld_ref;
    always_comb begin
        ld_off  = mem_result_q[1:0];
        ld_byte = (ld_off == 2'b00) ? dmem_rdata[7:0]   :
                  (ld_off == 2'b01) ? dmem_rdata[15:8]  :
                  (ld_off == 2'b10) ? dmem_rdata[23:16] : dmem_rdata[31:24];
        ld_half = ld_off[1] ? dmem_rdata[31:16] : dmem_rdata[15:0];
        ld_ref  = (mem_size_q == 2'b00) ? {{24{(!mem_unsigned_q && ld_byte[7])}}, ld_byte} :
                  (mem_size_q == 2'b01) ? {{16{(!mem_unsigned_q && ld_half[15])}}, ld_half} : dmem_rdata;
    end

    A_MEM_STALL:    assert property (mem_stall == (dmem_valid && !dmem_ready));
    A_MDU_BUSY:     assert property (mdu_busy == (ex_valid_q && ex_dec_q.is_mdu && !mdu_done));
    A_STALL_FRONT:  assert property (stall_front == (mem_stall || mdu_busy || load_use));
    A_EX_FIRE:      assert property (ex_fire == (ex_valid_q && !mem_stall && !mdu_busy));
    A_MDU_START:    assert property (mdu_start == (ex_valid_q && ex_dec_q.is_mdu && !mdu_active_q && !mem_stall));
    A_TRAP:         assert property (trap == (ex_fire && (irq_pending || exc)));
    A_MRET:         assert property (mret == (ex_fire && !trap && ex_dec_q.is_mret));
    A_REDIRECT:     assert property (redirect == (trap || mret || (ex_fire && flow_change)));
    A_REDIRECT_PC:  assert property (redirect_pc == (trap ? csr_mtvec : (mret ? csr_mepc : ((ex_fire && flow_change) ? target : 32'd0))));
    A_EX_LINK:      assert property (ex_link == (ex_pc_q + 32'd4));
    A_FLOW_CHANGE:  assert property (flow_change == (ex_dec_q.is_jal || ex_dec_q.is_jalr || (ex_dec_q.is_branch && branch_taken)));
    A_TARGET_MISALIGNED: assert property (target_misaligned == (flow_change && target[1]));
    A_ALU_A:        assert property (alu_a == ((ex_dec_q.src_a == SRC_A_PC) ? ex_pc_q : ((ex_dec_q.src_a == SRC_A_ZERO) ? 32'd0 : fwd_rs1)));
    A_ALU_B:        assert property (alu_b == (ex_dec_q.src_b_imm ? ex_dec_q.imm : fwd_rs2));
    A_EX_RESULT:    assert property (ex_result == ((ex_dec_q.is_jal || ex_dec_q.is_jalr) ? ex_link : ((ex_dec_q.csr_op != CSR_NONE) ? csr_rdata : (ex_dec_q.is_mdu ? mdu_result : alu_result))));
    A_MEM_FINAL:    assert property (mem_final == (mem_is_load_q ? load_data : mem_result_q));
    A_LS_MIS:       assert property (ls_misaligned == ((ex_dec_q.is_load || ex_dec_q.is_store) ? ((ex_dec_q.mem_size == 2'b00) ? 1'b0 : ((ex_dec_q.mem_size == 2'b01) ? alu_result[0] : (alu_result[1:0] != 2'b00))) : 1'b0));

    A_FWD_RS1:      assert property (fwd_rs1 == (((mem_valid_q && mem_wb_en_q) && (mem_rd_q == ex_dec_q.rs1) && (ex_dec_q.rs1 != 5'd0)) ? mem_result_q : (((wb_valid_q && wb_en_q) && (wb_rd_q == ex_dec_q.rs1) && (ex_dec_q.rs1 != 5'd0)) ? wb_data_q : ex_rs1_q)));
    A_FWD_RS2:      assert property (fwd_rs2 == (((mem_valid_q && mem_wb_en_q) && (mem_rd_q == ex_dec_q.rs2) && (ex_dec_q.rs2 != 5'd0)) ? mem_result_q : (((wb_valid_q && wb_en_q) && (wb_rd_q == ex_dec_q.rs2) && (ex_dec_q.rs2 != 5'd0)) ? wb_data_q : ex_rs2_q)));
    A_LOAD_USE:     assert property (load_use == (id_valid && (ex_valid_q && ex_dec_q.is_load) && (ex_dec_q.rd != 5'd0) && ((id_dec.uses_rs1 && (id_dec.rs1 == ex_dec_q.rd)) || (id_dec.uses_rs2 && (id_dec.rs2 == ex_dec_q.rd)))));

    A_LOAD_DATA:    assert property (load_data == ld_ref);

    A_MEMV_NXT:     assert property (mem_valid_q == ($past(mem_stall) ? $past(mem_valid_q) : ($past(ex_fire) && !$past(trap))));
    A_WBV_NXT:      assert property (wb_valid_q == ($past(mem_valid_q) && !$past(mem_stall)));
    A_WBE_NXT:      assert property (wb_en_q == (($past(mem_valid_q) && !$past(mem_stall)) ? $past(mem_wb_en_q) : $past(wb_en_q)));
    A_WBRD_NXT:     assert property (wb_rd_q == (($past(mem_valid_q) && !$past(mem_stall)) ? $past(mem_rd_q) : $past(wb_rd_q)));
    A_WBDATA_NXT:   assert property (wb_data_q == (($past(mem_valid_q) && !$past(mem_stall)) ? $past(mem_final) : $past(wb_data_q)));

    A_CRD3_LEGAL:   assert property ((id_valid && (
                          ((id_instr[6:0] == 7'b0110011) && (id_instr[31:25] == 7'b0010000) && ((id_instr[14:12] == 3'b010) || (id_instr[14:12] == 3'b100) || (id_instr[14:12] == 3'b110))) ||
                          ((id_instr[6:0] == 7'b0110011) && (id_instr[31:25] == 7'b0000101) && ((id_instr[14:12] == 3'b100) || (id_instr[14:12] == 3'b101) || (id_instr[14:12] == 3'b110) || (id_instr[14:12] == 3'b111)))
                        )) |-> !id_dec.illegal);

endmodule
/* verilator lint_on SYNCASYNCNET */

bind rv_core rv_core_defs u_core_defs (.*);
