// Per-pipeline-stage contract assertions for an in-order RV32IM core.
//
// ============================================================================
// WHY THIS EXISTS
// ============================================================================
// The stage contracts are derived ONLY from the ISA: given the instruction bits
// present in a stage, every datapath value in that stage has exactly one legal
// value. That makes the checks microarchitecture-independent *in value* and lets
// them be written before the RTL is released. What is unknown pre-release is the
// SIGNAL NAMES, not the contract, so this file is a module with all-input ports
// and is attached with a one-page bind file on release day.
//
// ============================================================================
// WHAT IS ASSERTED (and why it catches silent mutations)
// ============================================================================
//   Decode   : instr field slicing (rs1/rs2/rd/funct3/opcode), immediate decode,
//              legality classification.
//   Execute  : ALU result == ISA function of (opcode, funct3, funct7, a, b).
//              Branch decision == ISA compare. Branch/JAL/JALR target == pc + imm.
//              This single equivalence kills every ALU / compare / immediate
//              mutant that a testbench could only catch with a directed vector.
//   Memory   : address == base + ISA immediate, size == ISA size, load data ==
//              ISA sign/zero extension of the observed read word.
//   Writeback: rd == instr[11:7], we == writes_rd(op) && rd != 0, LUI value.
//   Pipeline : fetch PC is a sequential successor, a hold, or an announced
//              redirect; optionally instruction identity is preserved.
//
// ============================================================================
// WHAT IS **NOT** ASSERTED (honest limits)
// ============================================================================
//   * Memory *contents*: the observer cannot see imem/dmem. Checking that the
//     fetched word equals program[pc] is a cocotb scoreboard job (the program
//     image lives in Python). Do not fake it in SVA.
//   * Hazard correctness (forwarding, stall timing, flush timing): that is
//     microarchitecture, not ISA. Enable only with HAZARD_MODE, and only for the
//     exact schedule the released core documents.
//   * Store byte lanes: the ISA fixes the memory image, not the DUT's internal
//     write-data layout. Check stores by reading the memory image back in cocotb.
//   * AUIPC / JAL link values: they depend on the instruction's own PC, which the
//     tap does not carry. Checked in cocotb against the retire model.
//   * CSR side effects / traps: deferred until the released spec pins them.
//
// ============================================================================
// BIND TEMPLATE (release day - fill from the released signal names)
// ============================================================================
//   bind rv_core rv_stage_sva u_stage_sva (
//     .clk(clk), .rst_n(rst_n),
//     .if_valid(if_valid), .if_pc(pc_f),            .if_instr(imem_rdata),
//     .id_valid(id_valid), .id_pc(pc_d),            .id_instr(instr_d),
//     .id_rs1(rs1_d), .id_rs2(rs2_d), .id_rd(rd_d),
//     .id_imm(imm_d), .id_illegal(illegal_d),
//     .ex_valid(ex_valid), .ex_pc(pc_e), .ex_instr(instr_e),
//     .ex_a(op_a_e), .ex_b(op_b_e), .ex_alu(alu_e),
//     .ex_taken(branch_taken_e), .ex_target(target_e),
//     .mem_valid(mem_valid), .mem_instr(instr_m),
//     .mem_base(alu_e), .mem_addr(mem_addr_m), .mem_store_data(store_data_m),
//     .mem_we(store_en_m), .mem_re(load_en_m), .mem_size(size_m),
//     .mem_rdata(dmem_rdata), .mem_load_data(load_data_m),
//     .wb_valid(wb_valid), .wb_instr(instr_w), .wb_we(reg_we_w),
//     .wb_rd(rd_w), .wb_data(wdata_w),
//     .redirect_valid(redirect_e), .redirect_pc(redirect_target_e),
//     .flush_ex(flush_e), .flush_mem(flush_m), .flush_wb(flush_w)
//   );
//
// `if_*` is the fetch stage AFTER the PC mux (so if_pc is the address whose data
// is being returned this cycle). If your core has a separate imem request
// register, tap the response side instead - the contract is address<->data.

`default_nettype none

module rv_stage_sva #(
    // 1 = RV32I without the C extension: every fetched PC is word aligned.
    parameter bit IALIGN32 = 1'b1,
    // 1 = DUT flags illegal instructions at decode and `id_illegal` is valid.
    parameter bit CHECK_ILLEGAL = 1'b1,
    // 1 = assert instruction identity across stages while flushes happen.
    // Only enable when the release documents that a flushed entry is squashed in
    // place (never re-labelled). Off by default: it is a microarchitecture fact.
    parameter bit HAZARD_MODE = 1'b0,
    // 1 = DUT implements CR-D3 (sh1add/sh2add/sh3add, min/minu/max/maxu).
    parameter bit CR_D3 = 1'b1
) (
    input logic        clk,
    input logic        rst_n,

    // ---------------- fetch (post-PC-mux address and the data returned for it)
    input logic        if_valid,
    input logic [31:0] if_pc,
    input logic [31:0] if_instr,

    // ---------------- decode
    input logic        id_valid,
    input logic [31:0] id_pc,
    input logic [31:0] id_instr,
    input logic [4:0]  id_rs1,
    input logic [4:0]  id_rs2,
    input logic [4:0]  id_rd,
    input logic [31:0] id_imm,
    input logic        id_illegal,

    // ---------------- execute
    input logic        ex_valid,
    input logic [31:0] ex_pc,
    input logic [31:0] ex_instr,
    input logic [31:0] ex_a,
    input logic [31:0] ex_b,
    input logic [31:0] ex_alu,
    input logic        ex_taken,
    input logic [31:0] ex_target,

    // ---------------- memory
    input logic        mem_valid,
    input logic [31:0] mem_instr,
    input logic [31:0] mem_base,
    input logic [31:0] mem_addr,
    input logic [31:0] mem_store_data,
    input logic        mem_we,
    input logic        mem_re,
    input logic [2:0]  mem_size,
    input logic [31:0] mem_rdata,
    input logic [31:0] mem_load_data,

    // ---------------- writeback
    input logic        wb_valid,
    input logic [31:0] wb_instr,
    input logic        wb_we,
    input logic [4:0]  wb_rd,
    input logic [31:0] wb_data,

    // ---------------- redirect / flush (optional; tie 0 if the core has none)
    input logic        redirect_valid,
    input logic [31:0] redirect_pc,
    input logic        flush_ex,
    input logic        flush_mem,
    input logic        flush_wb
);

  import rv_ref_pkg::*;

  // --------------------------------------------------------------- decode views
  // Explicit continuous assignments: a declaration initializer would only take
  // effect once, at time zero (a classic SVA-observer bug).
  logic [6:0] id_opcode, id_funct3, id_funct7;
  assign id_opcode = id_instr[6:0];
  assign id_funct3 = id_instr[14:12];
  assign id_funct7 = id_instr[31:25];

  logic [6:0] ex_opcode, ex_funct3, ex_funct7;
  assign ex_opcode = ex_instr[6:0];
  assign ex_funct3 = ex_instr[14:12];
  assign ex_funct7 = ex_instr[31:25];

  logic [6:0] mem_opcode;
  logic [2:0] mem_funct3;
  assign mem_opcode = mem_instr[6:0];
  assign mem_funct3 = mem_instr[14:12];

  logic [6:0] wb_opcode;
  assign wb_opcode = wb_instr[6:0];

  // Value the EX stage must place on its second ALU operand:
  // OP-IMM uses the decoded immediate, OP uses rs2.
  logic [31:0] ex_b_ref;
  always_comb begin
    if (ex_opcode == 7'b0010011) ex_b_ref = imm_ref(ex_instr, ex_opcode);
    else                         ex_b_ref = ex_b;
  end

  // ==========================================================================
  // D1. Decode field slicing - the fields must come from the instruction.
  // ==========================================================================
  property p_dec_fields;
    @(posedge clk) disable iff (!rst_n)
      id_valid |-> (id_rs1    == id_instr[19:15]) &&
                   (id_rs2    == id_instr[24:20]) &&
                   (id_rd     == id_instr[11:7])  &&
                   (id_funct3 == id_instr[14:12]) &&
                   (id_opcode == id_instr[6:0]);
  endproperty
  A_DEC_FIELDS: assert property (p_dec_fields);

  // ==========================================================================
  // D2. Immediate decode - sign extension of every format.
  // ==========================================================================
  property p_dec_imm;
    @(posedge clk) disable iff (!rst_n)
      id_valid && imm_known(id_opcode) |-> (id_imm == imm_ref(id_instr, id_opcode));
  endproperty
  A_DEC_IMM: assert property (p_dec_imm);

  // ==========================================================================
  // D3. Legality classification (only for opcodes where the ISA is unambiguous).
  // ==========================================================================
  if (CHECK_ILLEGAL) begin : g_legal
    property p_dec_legal;
      @(posedge clk) disable iff (!rst_n)
        id_valid && legal_known(id_opcode)
          |-> (id_illegal == !legal_ref(id_opcode, id_funct3, id_funct7));
    endproperty
    A_DEC_LEGAL: assert property (p_dec_legal);
  end

  // ==========================================================================
  // X1. ALU equivalence - the highest-value assertion in this file.
  //     For every instruction the ALU subset covers, the result is fully
  //     determined by the instruction word and the two operands.
  // ==========================================================================
  property p_ex_alu;
    @(posedge clk) disable iff (!rst_n)
      ex_valid && alu_known(ex_opcode, ex_funct3, ex_funct7)
        |-> (ex_alu == alu_ref(ex_opcode, ex_funct3, ex_funct7, ex_a, ex_b_ref));
  endproperty
  A_EX_ALU: assert property (p_ex_alu);

  // ==========================================================================
  // X2. Branch decision equivalence.
  // ==========================================================================
  property p_ex_branch;
    @(posedge clk) disable iff (!rst_n)
      ex_valid && is_branch(ex_opcode)
        |-> (ex_taken == branch_ref(ex_funct3, ex_a, ex_b));
  endproperty
  A_EX_BRANCH: assert property (p_ex_branch);

  // ==========================================================================
  // X3. Branch / JAL target, and JALR target (bit 0 cleared).
  // ==========================================================================
  property p_ex_target_branch;
    @(posedge clk) disable iff (!rst_n)
      ex_valid && (is_branch(ex_opcode) || is_jal(ex_opcode)) |-> (
        ex_target == (ex_pc + imm_ref(ex_instr, ex_opcode)) &&
        ex_target[1:0] == 2'b00);
  endproperty
  A_EX_TARGET_BRANCH: assert property (p_ex_target_branch);

  property p_ex_target_jalr;
    @(posedge clk) disable iff (!rst_n)
      ex_valid && is_jalr(ex_opcode) |-> (
        ex_target == ((ex_a + imm_ref(ex_instr, ex_opcode)) & 32'hFFFF_FFFE));
  endproperty
  A_EX_TARGET_JALR: assert property (p_ex_target_jalr);

  // ==========================================================================
  // M1. Address generation and access size.
  // ==========================================================================
  property p_mem_addr;
    @(posedge clk) disable iff (!rst_n)
      mem_valid && (is_load(mem_opcode) || is_store(mem_opcode))
        |-> (mem_addr == mem_addr_isa);
  endproperty
  A_MEM_ADDR: assert property (p_mem_addr);

  property p_mem_size;
    @(posedge clk) disable iff (!rst_n)
      mem_valid && (is_load(mem_opcode) || is_store(mem_opcode))
        |-> (mem_size == access_size(mem_funct3));
  endproperty
  A_MEM_SIZE: assert property (p_mem_size);

  logic [31:0] mem_addr_isa;
  assign mem_addr_isa = mem_base + imm_ref(mem_instr, mem_opcode);

  property p_mem_exclusive;
    @(posedge clk) disable iff (!rst_n)
      mem_valid |-> (!(mem_we && mem_re));
  endproperty
  A_MEM_EXCLUSIVE: assert property (p_mem_exclusive);

  property p_mem_we_is_store;
    @(posedge clk) disable iff (!rst_n)
      mem_valid && mem_we |-> is_store(mem_opcode);
  endproperty
  A_MEM_WE_IS_STORE: assert property (p_mem_we_is_store);

  property p_mem_re_is_load;
    @(posedge clk) disable iff (!rst_n)
      mem_valid && mem_re |-> is_load(mem_opcode);
  endproperty
  A_MEM_RE_IS_LOAD: assert property (p_mem_re_is_load);

  // ==========================================================================
  // M2. Load data extension - the exact ISA sign/zero extension of the word the
  //     memory returned for the low address bits.
  // ==========================================================================
  property p_mem_load_data;
    @(posedge clk) disable iff (!rst_n)
      mem_valid && is_load(mem_opcode) |-> (
        mem_load_data == load_ref(mem_rdata, mem_funct3, mem_addr_isa[1:0]));
  endproperty
  A_MEM_LOAD_DATA: assert property (p_mem_load_data);

  // ==========================================================================
  // W1. Writeback destination, write-enable and the fully determined LUI value.
  // ==========================================================================
  property p_wb_rd;
    @(posedge clk) disable iff (!rst_n)
      wb_valid |-> (wb_rd == wb_instr[11:7]);
  endproperty
  A_WB_RD: assert property (p_wb_rd);

  property p_wb_we;
    @(posedge clk) disable iff (!rst_n)
      wb_valid |-> (wb_we == (writes_rd(wb_opcode) && (wb_instr[11:7] != 5'd0)));
  endproperty
  A_WB_WE: assert property (p_wb_we);

  property p_wb_data_lui;
    @(posedge clk) disable iff (!rst_n)
      wb_valid && is_lui(wb_opcode) && wb_we
        |-> (wb_data == {wb_instr[31:12], 12'b0});
  endproperty
  A_WB_DATA_LUI: assert property (p_wb_data_lui);

  // ==========================================================================
  // P1. Fetch PC: sequential successor, hold (stall), or announced redirect.
  //     Anything else means the fetch unit lost or invented an address.
  // ==========================================================================
  property p_if_pc_flow;
    @(posedge clk) disable iff (!rst_n)
      if_valid && $past(rst_n) && $past(if_valid) |-> (
        (if_pc == $past(if_pc))          ||   // stalled
        (if_pc == $past(if_pc) + 32'd4)  ||   // sequential
        ($past(redirect_valid) && if_pc == $past(redirect_pc)));
  endproperty
  A_IF_PC_FLOW: assert property (p_if_pc_flow);

  if (IALIGN32) begin : g_align
    property p_if_pc_align;
      @(posedge clk) disable iff (!rst_n)
        if_valid |-> (if_pc[1:0] == 2'b00);
    endproperty
    A_IF_PC_ALIGN: assert property (p_if_pc_align);

    property p_id_pc_align;
      @(posedge clk) disable iff (!rst_n)
        id_valid |-> (id_pc[1:0] == 2'b00);
    endproperty
    A_ID_PC_ALIGN: assert property (p_id_pc_align);
  end

  // ==========================================================================
  // P2. Instruction identity across stages (optional; microarchitecture flavour).
  // ==========================================================================
  if (HAZARD_MODE) begin : g_identity
    property p_id_to_ex;
      @(posedge clk) disable iff (!rst_n)
        !flush_ex && ex_valid && $past(id_valid)
          |-> (ex_instr == $past(id_instr)) && (ex_pc == $past(id_pc));
    endproperty
    A_ID_TO_EX: assert property (p_id_to_ex);

    property p_ex_to_mem;
      @(posedge clk) disable iff (!rst_n)
        !flush_mem && mem_valid && $past(ex_valid)
          |-> (mem_instr == $past(ex_instr));
    endproperty
    A_EX_TO_MEM: assert property (p_ex_to_mem);

    property p_mem_to_wb;
      @(posedge clk) disable iff (!rst_n)
        !flush_wb && wb_valid && $past(mem_valid)
          |-> (wb_instr == $past(mem_instr));
    endproperty
    A_MEM_TO_WB: assert property (p_mem_to_wb);

    // A taken branch must raise exactly one redirect in the cycle it executes.
    property p_ex_redirect;
      @(posedge clk) disable iff (!rst_n)
        ex_valid && is_branch(ex_opcode) |-> (redirect_valid == ex_taken);
    endproperty
    A_EX_REDIRECT: assert property (p_ex_redirect);

    // NOTE: "the cycle after a redirect squashes ID" is deliberately NOT asserted
    // here. The squash can be combinational (same cycle) or registered (next
    // cycle) and both are legal; only the released microarchitecture can say
    // which. Wrong-path execution is caught functionally by the retire-stream
    // scoreboard instead (it observes an instruction the ISS never retired).
  end

  // ==========================================================================
  // CR-D3 guard: if the core claims the extension, those encodings must not be
  // classified illegal (the opposite of a missing-feature mutant).
  // ==========================================================================
  if (CR_D3 && CHECK_ILLEGAL) begin : g_crd3
    property p_crd3_legal;
      @(posedge clk) disable iff (!rst_n)
        id_valid && is_cr_op(id_funct7) && (id_opcode == 7'b0110011)
          |-> (id_illegal == 1'b0);
    endproperty
    A_CRD3_LEGAL: assert property (p_crd3_legal);
  end

endmodule

`default_nettype wire
