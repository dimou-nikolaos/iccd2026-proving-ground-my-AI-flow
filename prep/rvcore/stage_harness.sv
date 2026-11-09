// Synthetic 5-stage in-order RV32I(+CR-D3) harness.
//
// ============================================================================
// WHAT THIS IS - AND IS NOT
// ============================================================================
// This is NOT a guess at the released core. It is a *test fixture* whose only
// job is to (a) exercise every stage contract in rv_stage_sva.sv and (b) give us
// a working pipeline shape (IF/ID/EX/MEM/WB + forwarding + load-use stall) that
// the release-day tap adapter can be modelled on. It deliberately implements the
// ALU and load-extension logic *independently* of rv_ref_pkg, so that the
// observer is checking a second implementation rather than itself.
//
// Subset: RV32I ALU/branch/load/store/LUI/AUIPC/JAL/JALR + CR-D3. No CSR, no
// traps, no misalignment handling (programs must be aligned).
//
// Hazard scheme (what the optional HAZARD_MODE assertions assume):
//   * 1-cycle load-use stall, bubble injected into EX.
//   * MEM->EX and WB->EX forwarding; ID reads with WB write-first bypass.
//   * Branch/jump resolved in EX; IF/ID squashed the same cycle; no delay slot.
//
// BUG != 0 injects a deliberate deviation so the negative test can prove the
// assertions are live. Never used for anything else.
`default_nettype none

module stage_harness #(
    parameter int BUG = 0
) (
    input  logic        clk,
    input  logic        rst_n,

    // instruction memory load port (drive before releasing reset)
    input  logic        imem_we,
    input  logic [9:0]  imem_waddr,
    input  logic [31:0] imem_wdata,

    // data memory clear + observation (for the cocotb scoreboard)
    input  logic        dm_we,
    input  logic [9:0]  dm_waddr,
    input  logic [31:0] dm_wdata,
    input  logic [9:0]  dm_obs_addr,
    output logic [31:0] dm_obs_data,

    // retire accounting
    output logic [31:0] retire_count,
    output logic [31:0] pc_obs,

    // writeback observation (retire stream scoreboard)
    output logic        obs_wb_valid,
    output logic        obs_wb_we,
    output logic [31:0] obs_wb_pc,
    output logic [31:0] obs_wb_instr,
    output logic [31:0] obs_wb_data,
    output logic [4:0]  obs_wb_rd,

    // register-file observation
    input  logic [4:0]  obs_reg_addr,
    output logic [31:0] obs_reg_data
);

  // ------------------------------------------------------------------ memories
  logic [31:0] imem [0:1023];
  logic [31:0] dmem [0:1023];

  always_ff @(posedge clk) if (imem_we) imem[imem_waddr] <= imem_wdata;

  always_ff @(posedge clk) begin
    if (dm_we) dmem[dm_waddr] <= dm_wdata;          // testbench clear/init
    if (mem_valid && mem_we) begin
      unique case (mem_size)
        3'd1: begin
          unique case (mem_addr[1:0])
            2'd0: dmem[mem_addr[11:2]][7:0]   <= mem_store_data[7:0];
            2'd1: dmem[mem_addr[11:2]][15:8]  <= mem_store_data[7:0];
            2'd2: dmem[mem_addr[11:2]][23:16] <= mem_store_data[7:0];
            default: dmem[mem_addr[11:2]][31:24] <= mem_store_data[7:0];
          endcase
        end
        3'd2: begin
          if (!mem_addr[1]) dmem[mem_addr[11:2]][15:0]  <= mem_store_data[15:0];
          else              dmem[mem_addr[11:2]][31:16] <= mem_store_data[15:0];
        end
        default: dmem[mem_addr[11:2]] <= mem_store_data;
      endcase
    end
  end

  assign dm_obs_data = dmem[dm_obs_addr];

  // --------------------------------------------------------------------- state
  logic [31:0] pc;
  logic [31:0] regs [0:31];

  // IF/ID
  logic        id_valid;
  logic [31:0] id_pc, id_instr;

  // ID/EX
  logic        ex_valid;
  logic [31:0] ex_pc, ex_instr, ex_a_raw, ex_b_raw;
  logic [4:0]  ex_rs1, ex_rs2, ex_rd;

  // EX/MEM
  logic        mem_valid, mem_regwe;
  logic [31:0] mem_pc, mem_instr, mem_a, mem_result, mem_sd;
  logic [4:0]  mem_rd;

  // MEM/WB
  logic        wb_valid, wb_we;
  logic [31:0] wb_pc, wb_instr, wb_data;
  logic [4:0]  wb_rd;

  // ------------------------------------------------------------------ tap view
  logic        if_valid, if_valid_q;
  logic [31:0] if_pc, if_instr;
  logic [31:0] id_imm;
  logic        id_illegal;
  logic [31:0] ex_a, ex_b, ex_alu, ex_result, ex_target;
  logic        ex_taken, ex_redirect;
  logic [31:0] mem_base, mem_addr, mem_store_data, mem_load_data;
  logic        mem_we, mem_re;
  logic [2:0]  mem_size;
  logic [31:0] mem_rdata;
  logic [31:0] wb_data_o;

  // --------------------------------------------------------------- decode (ID)
  logic [6:0] id_op;
  logic [2:0] id_f3;
  logic [6:0] id_f7;
  assign id_op = id_instr[6:0];
  assign id_f3 = id_instr[14:12];
  assign id_f7 = id_instr[31:25];

  logic [31:0] id_imm_int;
  always_comb begin
    unique case (id_op)
      7'b0000011, 7'b0010011, 7'b1100111: id_imm_int = {{20{id_instr[31]}}, id_instr[31:20]};
      7'b0100011: id_imm_int = {{20{id_instr[31]}}, id_instr[31:25], id_instr[11:7]};
      7'b1100011: begin
        if (BUG == 9)  // missing one immediate bit -> redirect off by one instruction
          id_imm_int = {{19{id_instr[31]}}, id_instr[31], id_instr[7],
                        id_instr[30:25], id_instr[11:8], 1'b0} + 32'd2;
        else
          id_imm_int = {{19{id_instr[31]}}, id_instr[31], id_instr[7],
                        id_instr[30:25], id_instr[11:8], 1'b0};
      end
      7'b0110111, 7'b0010111: id_imm_int = {id_instr[31:12], 12'b0};
      7'b1101111: id_imm_int = {{11{id_instr[31]}}, id_instr[31], id_instr[19:12],
                                id_instr[20], id_instr[30:21], 1'b0};
      default: id_imm_int = 32'd0;
    endcase
  end

  assign id_imm = (BUG == 2 && (id_op == 7'b0000011))
                  ? {20'b0, id_instr[31:20]}   // load immediate zero-extended
                  : id_imm_int;

  // Explicit decode-stage tap signals. A real core must derive these from the
  // instruction word; keeping them separate signals is what makes A_DEC_FIELDS
  // a real check instead of a tautology.
  logic [4:0] id_rs1_d, id_rs2_d, id_rd_d;
  logic [2:0] id_funct3_d;
  logic [6:0] id_opcode_d;
  assign id_rs1_d    = id_instr[19:15];
  assign id_rs2_d    = id_instr[24:20];
  assign id_rd_d     = (BUG == 8) ? id_instr[15:11] : id_instr[11:7];
  assign id_funct3_d = id_instr[14:12];
  assign id_opcode_d = id_instr[6:0];

  // legality: what this harness supports
  logic id_illegal_int;
  always_comb begin
    id_illegal_int = 1'b0;
    unique case (id_op)
      7'b0110011: begin
        unique case (id_f7)
          7'b0000000, 7'b0000001: ;
          7'b0100000: if (!(id_f3 inside {3'b000, 3'b101})) id_illegal_int = 1'b1;
          7'b0000101: if (!(id_f3 inside {3'b100, 3'b101, 3'b110, 3'b111})) id_illegal_int = 1'b1;
          7'b0010000: if (!(id_f3 inside {3'b010, 3'b100, 3'b110})) id_illegal_int = 1'b1;
          default: id_illegal_int = 1'b1;
        endcase
      end
      7'b0010011: begin
        if (id_f3 inside {3'b001, 3'b101})
          if (!(id_f7 inside {7'b0000000, 7'b0100000})) id_illegal_int = 1'b1;
      end
      7'b0000011: if (!(id_f3 inside {3'b000, 3'b001, 3'b010, 3'b100, 3'b101})) id_illegal_int = 1'b1;
      7'b0100011: if (!(id_f3 inside {3'b000, 3'b001, 3'b010})) id_illegal_int = 1'b1;
      7'b1100011: if (!(id_f3 inside {3'b000, 3'b001, 3'b100, 3'b101, 3'b110, 3'b111})) id_illegal_int = 1'b1;
      7'b0110111, 7'b0010111, 7'b1101111, 7'b1100111: ;
      default: id_illegal_int = 1'b1;
    endcase
  end
  assign id_illegal = id_illegal_int;

  // ---------------------------------------------------------------- hazard (ID)
  logic load_use;
  always_comb begin
    load_use = id_valid && ex_valid &&
               (ex_instr[6:0] == 7'b0000011) && (ex_instr[11:7] != 5'd0) &&
               ((ex_instr[11:7] == id_instr[19:15]) || (ex_instr[11:7] == id_instr[24:20]));
  end

  // ------------------------------------------------------------- ALU (EX view)
  // Field extracts are module-level continuous assignments: block-local variables
  // in a combinational process are a Verilator portability trap (see A_EX_BRANCH /
  // A_EX_ALU notes in the harness header).
  logic [31:0] ex_b_sel;
  logic [31:0] alu_out;
  logic [6:0]  ex_op;
  logic [2:0]  ex_f3;
  logic [6:0]  ex_f7;
  logic signed [63:0] mul_sp, mul_hs;
  logic        [63:0] mul_up;
  assign ex_op = ex_instr[6:0];
  assign ex_f3 = ex_instr[14:12];
  assign ex_f7 = ex_instr[31:25];

  always_comb begin
    if (ex_instr[6:0] == 7'b0010011)
      ex_b_sel = imm_ref_local(ex_instr);
    else
      ex_b_sel = ex_b;
  end

  function automatic logic [31:0] imm_ref_local(input logic [31:0] instr);
    logic [6:0] op = instr[6:0];
    unique case (op)
      7'b0000011, 7'b0010011, 7'b1100111: return {{20{instr[31]}}, instr[31:20]};
      7'b0100011: return {{20{instr[31]}}, instr[31:25], instr[11:7]};
      7'b1100011: return {{19{instr[31]}}, instr[31], instr[7], instr[30:25], instr[11:8], 1'b0};
      7'b0110111, 7'b0010111: return {instr[31:12], 12'b0};
      7'b1101111: return {{11{instr[31]}}, instr[31], instr[19:12], instr[20], instr[30:21], 1'b0};
      default: return 32'd0;
    endcase
  endfunction

  always_comb begin
    alu_out = 32'd0;
    unique case (ex_op)
      7'b0010011: begin
        unique case (ex_f3)
          3'b000:  alu_out = (BUG == 1) ? ex_a - ex_b_sel : ex_a + ex_b_sel;
          3'b001:  alu_out = ex_a << ex_b_sel[4:0];
          3'b010:  alu_out = ($signed(ex_a) < $signed(ex_b_sel)) ? 32'd1 : 32'd0;
          3'b011:  alu_out = (ex_a < ex_b_sel) ? 32'd1 : 32'd0;
          3'b100:  alu_out = ex_a ^ ex_b_sel;
          // NOTE the explicit if/else: in a `?:` conditional the two branches
          // share one type context, so $signed(a) >>> n becomes a LOGICAL shift
          // because the other branch is unsigned. Verilator reproduces this.
          3'b101:  if (ex_f7[5]) alu_out = $signed(ex_a) >>> ex_b_sel[4:0];
                   else          alu_out = ex_a >> ex_b_sel[4:0];
          3'b110:  alu_out = ex_a | ex_b_sel;
          3'b111:  alu_out = ex_a & ex_b_sel;
          default: alu_out = 32'd0;
        endcase
      end
      7'b0110011: begin
        unique case (ex_f7)
          7'b0000000: begin
            unique case (ex_f3)
              3'b000:  alu_out = ex_a + ex_b;
              3'b001:  alu_out = ex_a << ex_b[4:0];
              3'b010:  alu_out = ($signed(ex_a) < $signed(ex_b)) ? 32'd1 : 32'd0;
              3'b011:  alu_out = (ex_a < ex_b) ? 32'd1 : 32'd0;
              3'b100:  alu_out = ex_a ^ ex_b;
              3'b101:  alu_out = ex_a >> ex_b[4:0];
              3'b110:  alu_out = ex_a | ex_b;
              3'b111:  alu_out = ex_a & ex_b;
              default: alu_out = 32'd0;
            endcase
          end
          7'b0100000: begin
            unique case (ex_f3)
              3'b000:  if (BUG == 10) alu_out = ex_a + ex_b;   // injected deviation
                       else           alu_out = ex_a - ex_b;
              3'b101:  alu_out = $signed(ex_a) >>> ex_b[4:0];
              default: alu_out = 32'd0;
            endcase
          end
          7'b0000001: begin
            // Products need a 64-bit assignment context (the self-determined width
            // of (a*b)>>32 truncates before the shift), and the div/rem arms must
            // be statements: a conditional with an unsigned arm forces UNSIGNED
            // division on the signed branch. Exactly the class of silent RV32 bug a
            // seeded deviation is likely to be.
            mul_sp = $signed(ex_a) * $signed(ex_b);
            mul_hs = $signed(ex_a) * $signed({1'b0, ex_b});   // b zero-extended
            mul_up = {32'b0, ex_a} * {32'b0, ex_b};
            unique case (ex_f3)
              3'b000:  alu_out = mul_sp[31:0];
              3'b001:  alu_out = mul_sp[63:32];
              3'b010:  alu_out = mul_hs[63:32];
              3'b011:  alu_out = mul_up[63:32];
              3'b100: begin
                if (ex_b == 32'd0) alu_out = 32'hFFFF_FFFF;
                else if (ex_a == 32'h8000_0000 && ex_b == 32'hFFFF_FFFF) alu_out = ex_a;
                else alu_out = $signed(ex_a) / $signed(ex_b);
              end
              3'b101: begin
                if (ex_b == 32'd0) alu_out = 32'hFFFF_FFFF;
                else alu_out = ex_a / ex_b;
              end
              3'b110: begin
                if (ex_b == 32'd0) alu_out = ex_a;
                else if (ex_a == 32'h8000_0000 && ex_b == 32'hFFFF_FFFF) alu_out = 32'd0;
                else alu_out = $signed(ex_a) % $signed(ex_b);
              end
              3'b111: begin
                if (ex_b == 32'd0) alu_out = ex_a;
                else alu_out = ex_a % ex_b;
              end
              default: alu_out = 32'd0;
            endcase
          end
          7'b0000101: begin
            unique case (ex_f3)
              3'b100:  alu_out = ($signed(ex_a) < $signed(ex_b)) ? ex_a : ex_b;
              3'b101:  alu_out = (ex_a < ex_b) ? ex_a : ex_b;
              3'b110:  alu_out = ($signed(ex_a) > $signed(ex_b)) ? ex_a : ex_b;
              3'b111:  alu_out = (ex_a > ex_b) ? ex_a : ex_b;
              default: alu_out = 32'd0;
            endcase
            if (BUG == 4) alu_out = ($signed(ex_a) < $signed(ex_b)) ? ex_b : ex_a;  // min -> max
          end
          7'b0010000: begin
            unique case (ex_f3)
              3'b010:  alu_out = (ex_a << 1) + ex_b;
              3'b100:  alu_out = (ex_a << 2) + ex_b;
              3'b110:  alu_out = (ex_a << 3) + ex_b;
              default: alu_out = 32'd0;
            endcase
          end
          default: alu_out = 32'd0;
        endcase
      end
      default: alu_out = 32'd0;
    endcase
  end

  assign ex_alu = alu_out;

  // branch compare
  // NOTE: the field extracts are module-level continuous assignments on purpose.
  // A declaration-with-initializer inside a procedural block is a known portability
  // trap (it can behave as a one-time init), and it silently broke this comparator
  // under Verilator 5.048 - caught by A_EX_BRANCH.
  logic branch_taken;
  always_comb begin
    branch_taken = 1'b0;
    if (ex_instr[6:0] == 7'b1100011) begin
      unique case (ex_f3)
        3'b000:  branch_taken = (ex_a == ex_b);
        3'b001:  branch_taken = (ex_a != ex_b);
        3'b100:  branch_taken = ($signed(ex_a) < $signed(ex_b));
        3'b101:  branch_taken = ($signed(ex_a) >= $signed(ex_b));
        3'b110:  branch_taken = (BUG == 3) ? ($signed(ex_a) < $signed(ex_b)) : (ex_a < ex_b);
        3'b111:  branch_taken = (ex_a >= ex_b);
        default: branch_taken = 1'b0;
      endcase
    end
  end

  assign ex_taken = branch_taken;
  assign ex_target = (ex_instr[6:0] == 7'b1100111)
                     ? ((ex_a + imm_ref_local(ex_instr)) & 32'hFFFF_FFFE)
                     : (ex_pc + imm_ref_local(ex_instr));

  assign ex_redirect = ex_valid &&
                       ((ex_instr[6:0] == 7'b1101111) ||
                        (ex_instr[6:0] == 7'b1100111) ||
                        (ex_instr[6:0] == 7'b1100011 && branch_taken));

  // EX result (writeback / address)
  always_comb begin
    unique case (ex_instr[6:0])
      7'b0000011, 7'b0100011: ex_result = ex_a + imm_ref_local(ex_instr);  // address
      7'b0110111:             ex_result = imm_ref_local(ex_instr);
      7'b0010111:             ex_result = ex_pc + imm_ref_local(ex_instr);
      7'b1101111, 7'b1100111: ex_result = ex_pc + 32'd4;                   // link
      default:                ex_result = alu_out;
    endcase
  end

  // ------------------------------------------------------- memory-stage view
  assign mem_base       = mem_a;
  assign mem_addr       = mem_result;
  assign mem_store_data = mem_sd;
  assign mem_re         = mem_valid && (mem_instr[6:0] == 7'b0000011);
  assign mem_we         = mem_valid && (mem_instr[6:0] == 7'b0100011);
  assign mem_size       = size_of(mem_instr[14:12]);
  assign mem_rdata      = dmem[mem_addr[11:2]];

  // load extension, implemented independently of rv_ref_pkg
  logic [31:0] load_ext;
  logic [7:0]  ld_b;
  logic [15:0] ld_h;
  logic [2:0]  mem_f3;
  assign ld_b   = mem_rdata[8*mem_addr[1:0] +: 8];
  assign ld_h   = mem_addr[1] ? mem_rdata[31:16] : mem_rdata[15:0];
  assign mem_f3 = mem_instr[14:12];
  always_comb begin
    unique case (mem_f3)
      3'b000:  load_ext = {{24{ld_b[7]}},  ld_b};
      3'b001:  load_ext = (BUG == 6) ? {16'b0, ld_h} : {{16{ld_h[15]}}, ld_h};
      3'b010:  load_ext = mem_rdata;
      3'b100:  load_ext = {24'b0, ld_b};
      3'b101:  load_ext = {16'b0, ld_h};
      default: load_ext = 32'd0;
    endcase
  end
  assign mem_load_data = load_ext;

  // --------------------------------------------------------------- writeback
  assign wb_data_o = wb_data;

  // -------------------------------------------------------------- observation
  assign obs_wb_valid = wb_valid;
  assign obs_wb_we    = wb_we;
  assign obs_wb_pc    = wb_pc;
  assign obs_wb_instr = wb_instr;
  assign obs_wb_data  = wb_data;
  assign obs_wb_rd    = wb_rd;
  assign obs_reg_data = (obs_reg_addr == 5'd0) ? 32'd0 : regs[obs_reg_addr];
  assign pc_obs       = pc;

  logic flush_ex;
  assign flush_ex = ex_redirect || load_use;

  // ------------------------------------------------------------------ tap view
  assign if_valid = rst_n && !load_use;
  assign if_pc    = pc;
  assign if_instr = imem[pc[11:2]];

  // ------------------------------------------------------------------ pipeline
  logic [31:0] retire_count_q;
  logic [31:0] id_op_a, id_op_b;
  assign retire_count = retire_count_q;

  // EX operand forwarding
  logic [31:0] mem_fwd;
  assign mem_fwd = (mem_instr[6:0] == 7'b0000011) ? load_ext : mem_result;

  logic [31:0] ex_a_f, ex_b_f;
  always_comb begin
    ex_a_f = ex_a_raw;
    ex_b_f = ex_b_raw;
    if (mem_valid && mem_regwe && (mem_rd != 5'd0) && (mem_rd == ex_rs1)) ex_a_f = mem_fwd;
    else if (wb_we && (wb_rd != 5'd0) && (wb_rd == ex_rs1))                ex_a_f = wb_data;
    if (mem_valid && mem_regwe && (mem_rd != 5'd0) && (mem_rd == ex_rs2)) ex_b_f = mem_fwd;
    else if (wb_we && (wb_rd != 5'd0) && (wb_rd == ex_rs2))                ex_b_f = wb_data;
  end
  assign ex_a = ex_a_f;
  assign ex_b = ex_b_f;

  always_ff @(posedge clk) begin
    if (!rst_n) begin
      pc          <= 32'd0;
      id_valid    <= 1'b0;
      id_pc       <= 32'd0;
      id_instr    <= 32'h0000_0013;
      ex_valid    <= 1'b0;
      ex_pc       <= 32'd0;
      ex_instr    <= 32'h0000_0013;
      ex_a_raw    <= 32'd0;
      ex_b_raw    <= 32'd0;
      ex_rs1      <= 5'd0;
      ex_rs2      <= 5'd0;
      ex_rd       <= 5'd0;
      mem_valid   <= 1'b0;
      mem_regwe   <= 1'b0;
      mem_pc      <= 32'd0;
      mem_instr   <= 32'h0000_0013;
      mem_a       <= 32'd0;
      mem_result  <= 32'd0;
      mem_sd      <= 32'd0;
      mem_rd      <= 5'd0;
      wb_valid    <= 1'b0;
      wb_we       <= 1'b0;
      wb_pc       <= 32'd0;
      wb_instr    <= 32'h0000_0013;
      wb_data     <= 32'd0;
      wb_rd       <= 5'd0;
      retire_count_q <= 32'd0;
      for (int i = 0; i < 32; i++) regs[i] <= 32'd0;
    end else begin
      // ---------------- WB
      wb_valid <= mem_valid;
      wb_we    <= mem_regwe;
      wb_pc    <= mem_pc;
      wb_instr <= mem_instr;
      wb_rd    <= mem_rd;
      wb_data  <= (mem_instr[6:0] == 7'b0000011) ? load_ext : mem_result;

      if (wb_valid && wb_we && (wb_rd != 5'd0)) regs[wb_rd] <= wb_data;

      if (wb_valid) retire_count_q <= retire_count_q + 32'd1;

      // ---------------- MEM
      mem_valid  <= ex_valid;
      mem_regwe  <= ex_valid && writes_rd_local(ex_instr[6:0]) &&
                    ((BUG == 5) ? 1'b1 : (ex_instr[11:7] != 5'd0));
      mem_pc     <= ex_pc;
      mem_instr  <= ex_instr;
      mem_a      <= ex_a;
      mem_result <= ex_result;
      mem_sd     <= ex_b;
      mem_rd     <= (BUG == 8) ? ex_instr[15:11] : ex_instr[11:7];

      // ---------------- EX
      if (ex_redirect || load_use) begin
        ex_valid <= 1'b0;
      end else begin
        ex_valid <= id_valid;
        ex_pc    <= id_pc;
        ex_instr <= id_instr;
        ex_rs1   <= id_instr[19:15];
        ex_rs2   <= id_instr[24:20];
        ex_rd    <= id_instr[11:7];
        ex_a_raw <= id_op_a;
        ex_b_raw <= id_op_b;
      end

      // ---------------- ID
      if (ex_redirect) begin
        id_valid <= 1'b0;
      end else if (!load_use) begin
        id_valid <= if_valid;
        id_pc    <= if_pc;
        id_instr <= if_instr;
      end

      // ---------------- IF
      if (ex_redirect)
        pc <= (BUG == 7) ? ex_target + 32'd4 : ex_target;
      else if (load_use)
        pc <= pc;
      else
        pc <= pc + 32'd4;
    end
  end

  // ID operand read with write-first bypass
  always_comb begin
    id_op_a = regs[id_instr[19:15]];
    id_op_b = regs[id_instr[24:20]];
    if (wb_we && (wb_rd != 5'd0) && (wb_rd == id_instr[19:15])) id_op_a = wb_data;
    if (wb_we && (wb_rd != 5'd0) && (wb_rd == id_instr[24:20])) id_op_b = wb_data;
  end

  function automatic logic [2:0] size_of(input logic [2:0] f3);
    unique case (f3)
      3'b000, 3'b100: return 3'd1;
      3'b001, 3'b101: return 3'd2;
      3'b010:         return 3'd4;
      default:        return 3'd0;
    endcase
  endfunction

  function automatic logic writes_rd_local(input logic [6:0] op);
    unique case (op)
      7'b0110011, 7'b0010011, 7'b0000011,
      7'b0110111, 7'b0010111, 7'b1101111, 7'b1100111: return 1'b1;
      default: return 1'b0;
    endcase
  endfunction

endmodule

`default_nettype wire
