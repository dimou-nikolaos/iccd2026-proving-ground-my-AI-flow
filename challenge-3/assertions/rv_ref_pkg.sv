// Pure reference functions for RV32IM + Zicsr encoding-level checks.
//
// Used by two consumers:
//   * rv_stage_sva.sv  - bind-style observers that compare DUT outputs with these
//   * stage_harness.sv - a synthetic DUT built from the same functions (self-test)
//
// Functions only: no state, no timing. That is deliberate - the reference is the
// ISA, so it must not encode any microarchitectural assumption.
package rv_ref_pkg;

  // ------------------------------------------------------------------ helpers
  function automatic logic is_cr_op(input logic [6:0] funct7);
    return (funct7 == 7'b0000101) || (funct7 == 7'b0010000);
  endfunction

  // Is this encoding in the ALU subset this observer can check?
  // (OP and OP-IMM only; LUI/AUIPC/JAL/branch/load/store have their own checks.)
  function automatic logic alu_known(input logic [6:0] opcode,
                                     input logic [2:0] funct3,
                                     input logic [6:0] funct7);
    if (opcode == 7'b0010011) begin
      if ((funct3 == 3'b001) || (funct3 == 3'b101))   // slli / srli / srai
        return (funct7 == 7'b0000000) || (funct7 == 7'b0100000);
      return 1'b1;                                    // funct7 is part of the immediate
    end
    if (opcode == 7'b0110011) begin
      case (funct7)
        7'b0000000: return 1'b1;
        7'b0100000: return (funct3 == 3'b000) || (funct3 == 3'b101);
        7'b0000001: return 1'b1;                                              // M
        7'b0000101: return (funct3 == 3'b100) || (funct3 == 3'b101) ||
                            (funct3 == 3'b110) || (funct3 == 3'b111);         // CR-D3 min/max
        7'b0010000: return (funct3 == 3'b010) || (funct3 == 3'b100) ||
                            (funct3 == 3'b110);                               // CR-D3 shNadd
        default:    return 1'b0;
      endcase
    end
    return 1'b0;
  endfunction

  // ---------------------------------------------------------------------- ALU
  function automatic logic [31:0] alu_ref(input logic [6:0] opcode,
                                          input logic [2:0] funct3,
                                          input logic [6:0] funct7,
                                          input logic [31:0] a,
                                          input logic [31:0] b);
    logic signed [63:0] sa, sb, sp, hs;
    logic        [63:0] ua, ub, up;
    logic signed [31:0] sdiv, smod;
    logic        [31:0] udiv, umod;

    sa = $signed(a);
    sb = $signed(b);
    ua = {32'b0, a};
    ub = {32'b0, b};

    if (opcode == 7'b0010011) begin                    // OP-IMM: b = decoded immediate
      case (funct3)
        3'b000:  return a + b;
        3'b001:  return a << b[4:0];
        3'b010:  return ($signed(a) < $signed(b)) ? 32'd1 : 32'd0;
        3'b011:  return (a < b) ? 32'd1 : 32'd0;
        3'b100:  return a ^ b;
        // Two separate returns on purpose: a `?:` conditional forces one shared
        // type context on both branches, which turns $signed(a) >>> n into a
        // LOGICAL shift. A plain logic [31:0] >>> n is also logical - both are
        // classic silent RV32 bugs (srai / sra).
        3'b101:  if (funct7[5]) return $signed(a) >>> b[4:0];
                 else           return a >> b[4:0];
        3'b110:  return a | b;
        3'b111:  return a & b;
        default: return 32'd0;
      endcase
    end

    case (funct7)
      7'b0000000: begin
        case (funct3)
          3'b000:  return a + b;
          3'b001:  return a << b[4:0];
          3'b010:  return ($signed(a) < $signed(b)) ? 32'd1 : 32'd0;
          3'b011:  return (a < b) ? 32'd1 : 32'd0;
          3'b100:  return a ^ b;
          3'b101:  return a >> b[4:0];
          3'b110:  return a | b;
          3'b111:  return a & b;
          default: return 32'd0;
        endcase
      end
      7'b0100000: begin
        case (funct3)
          3'b000:  return a - b;
          3'b101:  return $signed(a) >>> b[4:0];   // sra: signed
          default: return 32'd0;
        endcase
      end
      7'b0000001: begin                              // M extension
        // Every arm is a statement, never a nested `?:`. A conditional that mixes
        // signed and unsigned arms collapses to ONE unsigned context and silently
        // turns div/rem into unsigned division - the kind of bug that passes tests
        // whose operands happen to be positive.
        sp = sa * sb;
        hs = sa * $signed(ub);                      // mulhsu: b zero-extended
        up = ua * ub;
        case (funct3)
          3'b000:  return sp[31:0];
          3'b001:  return sp[63:32];
          3'b010:  return hs[63:32];
          3'b011:  return up[63:32];
          3'b100: begin
            if (b == 32'd0)                                    return 32'hFFFF_FFFF;
            else if (a == 32'h8000_0000 && b == 32'hFFFF_FFFF) return a;
            else begin sdiv = sa / sb;                         return sdiv; end
          end
          3'b101: begin
            if (b == 32'd0)                                    return 32'hFFFF_FFFF;
            else begin udiv = ua / ub;                         return udiv; end
          end
          3'b110: begin
            if (b == 32'd0)                                    return a;
            else if (a == 32'h8000_0000 && b == 32'hFFFF_FFFF) return 32'd0;
            else begin smod = sa % sb;                         return smod; end
          end
          3'b111: begin
            if (b == 32'd0)                                    return a;
            else begin umod = ua % ub;                         return umod; end
          end
          default: return 32'd0;
        endcase
      end
      7'b0000101: begin                              // CR-D3 signed/unsigned min/max
        case (funct3)
          3'b100:  return ($signed(a) < $signed(b)) ? a : b;   // min
          3'b101:  return (a < b) ? a : b;                     // minu
          3'b110:  return ($signed(a) > $signed(b)) ? a : b;   // max
          3'b111:  return (a > b) ? a : b;                     // maxu
          default: return 32'd0;
        endcase
      end
      7'b0010000: begin                              // CR-D3 address generation
        case (funct3)
          3'b010:  return (a << 1) + b;                  // sh1add
          3'b100:  return (a << 2) + b;                  // sh2add
          3'b110:  return (a << 3) + b;                  // sh3add
          default: return 32'd0;
        endcase
      end
      default: return 32'd0;
    endcase
  endfunction

  // ------------------------------------------------------------------ legality
  // Encoding legality for the subset the observer knows. `known` tells the caller
  // whether this function has an opinion (system/CSR legality is spec-dependent).
  function automatic logic legal_known(input logic [6:0] opcode);
    case (opcode)
      7'b0110011, 7'b0010011, 7'b0000011, 7'b0100011,
      7'b1100011, 7'b0110111, 7'b0010111, 7'b1101111: return 1'b1;
      default: return 1'b0;
    endcase
  endfunction

  function automatic logic legal_ref(input logic [6:0] opcode,
                                     input logic [2:0] funct3,
                                     input logic [6:0] funct7);
    case (opcode)
      7'b0110011, 7'b0010011: return alu_known(opcode, funct3, funct7);
      7'b0000011: return (funct3 == 3'b000) || (funct3 == 3'b001) || (funct3 == 3'b010) ||
                         (funct3 == 3'b100) || (funct3 == 3'b101);
      7'b0100011: return (funct3 == 3'b000) || (funct3 == 3'b001) || (funct3 == 3'b010);
      7'b1100011: return (funct3 == 3'b000) || (funct3 == 3'b001) || (funct3 == 3'b100) ||
                         (funct3 == 3'b101) || (funct3 == 3'b110) || (funct3 == 3'b111);
      7'b0110111, 7'b0010111, 7'b1101111: return 1'b1;   // LUI, AUIPC, JAL
      default: return 1'b0;
    endcase
  endfunction

  // ---------------------------------------------------------------- immediates
  function automatic logic imm_known(input logic [6:0] opcode);
    case (opcode)
      7'b0000011, 7'b0010011, 7'b1100111,   // I-type
      7'b0100011,                           // S-type
      7'b1100011,                           // B-type
      7'b0110111, 7'b0010111,               // U-type
      7'b1101111: return 1'b1;              // J-type
      default: return 1'b0;
    endcase
  endfunction

  function automatic logic [31:0] imm_ref(input logic [31:0] instr,
                                          input logic [6:0]  opcode);
    logic [31:0] i_imm, s_imm, b_imm, u_imm, j_imm;
    i_imm = {{20{instr[31]}}, instr[31:20]};
    s_imm = {{20{instr[31]}}, instr[31:25], instr[11:7]};
    b_imm = {{19{instr[31]}}, instr[31], instr[7], instr[30:25], instr[11:8], 1'b0};
    u_imm = {instr[31:12], 12'b0};
    j_imm = {{11{instr[31]}}, instr[31], instr[19:12], instr[20], instr[30:21], 1'b0};
    case (opcode)
      7'b0000011, 7'b0010011, 7'b1100111: return i_imm;
      7'b0100011:                         return s_imm;
      7'b1100011:                         return b_imm;
      7'b0110111, 7'b0010111:             return u_imm;
      7'b1101111:                         return j_imm;
      default:                            return 32'd0;
    endcase
  endfunction

  // ------------------------------------------------------------- address + size
  function automatic logic [31:0] agen_ref(input logic [31:0] base,
                                           input logic [31:0] offset);
    return base + offset;
  endfunction

  // --------------------------------------------------------------- instruction class
  function automatic logic is_load(input logic [6:0] opcode);
    return opcode == 7'b0000011;
  endfunction

  function automatic logic is_store(input logic [6:0] opcode);
    return opcode == 7'b0100011;
  endfunction

  function automatic logic is_branch(input logic [6:0] opcode);
    return opcode == 7'b1100011;
  endfunction

  function automatic logic is_jal(input logic [6:0] opcode);
    return opcode == 7'b1101111;
  endfunction

  function automatic logic is_jalr(input logic [6:0] opcode);
    return opcode == 7'b1100111;
  endfunction

  function automatic logic is_lui(input logic [6:0] opcode);
    return opcode == 7'b0110111;
  endfunction

  function automatic logic is_auipc(input logic [6:0] opcode);
    return opcode == 7'b0010111;
  endfunction

  // Does this instruction write a GPR (ignoring x0)?
  function automatic logic writes_rd(input logic [6:0] opcode);
    case (opcode)
      7'b0110011, 7'b0010011, 7'b0000011,      // OP, OP-IMM, LOAD
      7'b0110111, 7'b0010111, 7'b1101111,      // LUI, AUIPC, JAL
      7'b1100111: return 1'b1;                 // JALR
      default:    return 1'b0;
    endcase
  endfunction

  // ---------------------------------------------------------------- branch compare
  function automatic logic branch_ref(input logic [2:0] funct3,
                                      input logic [31:0] a,
                                      input logic [31:0] b);
    case (funct3)
      3'b000:  return a == b;                        // beq
      3'b001:  return a != b;                        // bne
      3'b100:  return $signed(a) <  $signed(b);      // blt
      3'b101:  return $signed(a) >= $signed(b);      // bge
      3'b110:  return a <  b;                        // bltu
      3'b111:  return a >= b;                        // bgeu
      default: return 1'b0;                          // 010/011 illegal
    endcase
  endfunction

  // ------------------------------------------------------------- load data extract
  // mem_rdata is the full aligned word read from memory; addr are the low address
  // bits (byte offset within the word). Result is the architecturally defined value.
  function automatic logic [31:0] load_ref(input logic [31:0] mem_rdata,
                                           input logic [2:0]  funct3,
                                           input logic [1:0]  addr);
    logic [7:0]  b;
    logic [15:0] h;
    b = mem_rdata[8*addr +: 8];
    h = mem_rdata[16*addr[1] +: 16];
    case (funct3)
      3'b000:  return {{24{b[7]}},  b};                     // lb
      3'b001:  return {{16{h[15]}}, h};                     // lh
      3'b010:  return mem_rdata;                            // lw
      3'b100:  return {24'b0, b};                           // lbu
      3'b101:  return {16'b0, h};                           // lhu
      default: return 32'd0;
    endcase
  endfunction

  // byte count of an access from funct3 (0 = illegal size)
  function automatic int unsigned access_size(input logic [2:0] funct3);
    case (funct3)
      3'b000:  return 1;   // byte
      3'b001:  return 2;   // half
      3'b010:  return 4;   // word
      3'b100:  return 1;   // byte unsigned
      3'b101:  return 2;   // half unsigned
      default: return 0;
    endcase
  endfunction

endpackage
