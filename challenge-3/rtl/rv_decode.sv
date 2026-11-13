// rv_decode: combinational instruction decoder.
module rv_decode (
  input  logic [31:0]      instr,
  output rv_pkg::decoded_t dec
);
  import rv_pkg::*;

  logic [4:0]  opcode;
  logic [2:0]  f3;
  logic [6:0]  f7;
  logic [31:0] imm_i, imm_s, imm_b, imm_u, imm_j;

  assign opcode = instr[6:2];
  assign f3     = instr[14:12];
  assign f7     = instr[31:25];

  assign imm_i = {{20{instr[31]}}, instr[31:20]};
  assign imm_s = {{20{instr[31]}}, instr[31:25], instr[11:7]};
  assign imm_b = {{19{instr[31]}}, instr[31], instr[7], instr[30:25], instr[11:8], 1'b0};
  assign imm_u = {instr[31:12], 12'd0};
  assign imm_j = {{11{instr[31]}}, instr[31], instr[19:12], instr[20], instr[30:21], 1'b0};

  always_comb begin
    dec              = '0;
    dec.rs1          = instr[19:15];
    dec.rs2          = instr[24:20];
    dec.rd           = instr[11:7];
    dec.alu_op       = ALU_ADD;
    dec.src_a        = SRC_A_RS1;
    dec.mem_size     = f3[1:0];
    dec.mem_unsigned = f3[2];
    dec.branch_f3    = f3;
    dec.csr_op       = CSR_NONE;
    dec.csr_uses_imm = f3[2];
    dec.csr_addr     = instr[31:20];
    dec.mdu_op       = f3;

    if (instr[1:0] != 2'b11) begin
      dec.illegal = 1'b1;
    end else begin
      unique case (opcode)
        OP_LUI: begin
          dec.imm       = imm_u;
          dec.src_a     = SRC_A_ZERO;
          dec.src_b_imm = 1'b1;
          dec.wb_en     = 1'b1;
        end
        OP_AUIPC: begin
          dec.imm       = imm_u;
          dec.src_a     = SRC_A_PC;
          dec.src_b_imm = 1'b1;
          dec.wb_en     = 1'b1;
        end
        OP_JAL: begin
          dec.imm    = imm_j;
          dec.is_jal = 1'b1;
          dec.wb_en  = 1'b1;
        end
        OP_JALR: begin
          dec.imm      = imm_i;
          dec.is_jalr  = 1'b1;
          dec.uses_rs1 = 1'b1;
          dec.wb_en    = 1'b1;
          dec.illegal  = (f3 != 3'b000);
        end
        OP_BRANCH: begin
          dec.imm       = imm_b;
          dec.is_branch = 1'b1;
          dec.uses_rs1  = 1'b1;
          dec.uses_rs2  = 1'b1;
          dec.illegal   = (f3 == 3'b010) || (f3 == 3'b011);
        end
        OP_LOAD: begin
          dec.imm       = imm_i;
          dec.is_load   = 1'b1;
          dec.src_b_imm = 1'b1;
          dec.uses_rs1  = 1'b1;
          dec.wb_en     = 1'b1;
          dec.illegal   = (f3 == 3'b011) || (f3 == 3'b110) || (f3 == 3'b111);
        end
        OP_STORE: begin
          dec.imm       = imm_s;
          dec.is_store  = 1'b1;
          dec.src_b_imm = 1'b1;
          dec.uses_rs1  = 1'b1;
          dec.uses_rs2  = 1'b1;
          dec.illegal   = (f3 > 3'b010);
        end
        OP_OPIMM: begin
          dec.imm       = imm_i;
          dec.src_b_imm = 1'b1;
          dec.uses_rs1  = 1'b1;
          dec.wb_en     = 1'b1;
          unique case (f3)
            3'b000: dec.alu_op = ALU_ADD;
            3'b010: dec.alu_op = ALU_SLT;
            3'b011: dec.alu_op = ALU_SLTU;
            3'b100: dec.alu_op = ALU_XOR;
            3'b110: dec.alu_op = ALU_OR;
            3'b111: dec.alu_op = ALU_AND;
            3'b001: begin
              dec.alu_op  = ALU_SLL;
              dec.illegal = (f7 != 7'b0000000);
            end
            3'b101: begin
              dec.alu_op  = f7[5] ? ALU_SRA : ALU_SRL;
              dec.illegal = (f7 != 7'b0000000) && (f7 != 7'b0100000);
            end
          endcase
        end
        OP_OP: begin
          dec.uses_rs1 = 1'b1;
          dec.uses_rs2 = 1'b1;
          dec.wb_en    = 1'b1;
          unique case (f7)
            7'b0000000: begin
              unique case (f3)
                3'b000: dec.alu_op = ALU_ADD;
                3'b001: dec.alu_op = ALU_SLL;
                3'b010: dec.alu_op = ALU_SLT;
                3'b011: dec.alu_op = ALU_SLTU;
                3'b100: dec.alu_op = ALU_XOR;
                3'b101: dec.alu_op = ALU_SRL;
                3'b110: dec.alu_op = ALU_OR;
                3'b111: dec.alu_op = ALU_AND;
              endcase
            end
            7'b0100000: begin
              if (f3 == 3'b000) begin
                dec.alu_op = ALU_SUB;
              end else if (f3 == 3'b101) begin
                dec.alu_op = ALU_SRA;
              end else begin
                dec.illegal = 1'b1;
              end
            end
            7'b0000001: begin
              dec.is_mdu = 1'b1;
            end
            7'b0010000: begin
              unique case (f3)
                3'b010: dec.alu_op = ALU_SH1ADD;
                3'b100: dec.alu_op = ALU_SH2ADD;
                3'b110: dec.alu_op = ALU_SH3ADD;
                default: dec.illegal = 1'b1;
              endcase
            end
            7'b0000101: begin
              unique case (f3)
                3'b100: dec.alu_op = ALU_MIN;
                3'b101: dec.alu_op = ALU_MINU;
                3'b110: dec.alu_op = ALU_MAX;
                3'b111: dec.alu_op = ALU_MAXU;
                default: dec.illegal = 1'b1;
              endcase
            end
            default: begin
              dec.illegal = 1'b1;
            end
          endcase
        end
        OP_FENCE: begin
          dec.illegal = (f3 != 3'b000) && (f3 != 3'b001);
        end
        OP_SYSTEM: begin
          if (f3 == 3'b000) begin
            if (instr[31:7] == 25'h0000000) begin
              dec.is_ecall = 1'b1;
            end else if (instr[31:7] == 25'h0002000) begin
              dec.is_ebreak = 1'b1;
            end else if (instr[31:7] == 25'h0604000) begin
              dec.is_mret = 1'b1;
            end else if (instr[31:7] != 25'h020A000) begin
              // Anything other than WFI, which runs as a no-op.
              dec.illegal = 1'b1;
            end
          end else if (f3 == 3'b100) begin
            dec.illegal = 1'b1;
          end else begin
            dec.wb_en    = 1'b1;
            dec.uses_rs1 = !f3[2];
            unique case (f3[1:0])
              2'b01:   dec.csr_op = CSR_RW;
              2'b10:   dec.csr_op = CSR_RS;
              default: dec.csr_op = CSR_RC;
            endcase
          end
        end
        default: begin
          dec.illegal = 1'b1;
        end
      endcase
    end

    if (dec.illegal) begin
      dec.wb_en     = 1'b0;
      dec.is_load   = 1'b0;
      dec.is_store  = 1'b0;
      dec.is_branch = 1'b0;
      dec.is_jal    = 1'b0;
      dec.is_jalr   = 1'b0;
      dec.is_mdu    = 1'b0;
      dec.csr_op    = CSR_NONE;
    end
  end

endmodule
