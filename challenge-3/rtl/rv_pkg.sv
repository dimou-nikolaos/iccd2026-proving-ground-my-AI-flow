// rv_pkg: types and constants for the rv_core pipeline.
package rv_pkg;

  typedef enum logic [4:0] {
    ALU_ADD, ALU_SUB, ALU_SLL, ALU_SLT, ALU_SLTU, ALU_XOR, ALU_SRL, ALU_SRA, ALU_OR, ALU_AND,
    ALU_SH1ADD, ALU_SH2ADD, ALU_SH3ADD, ALU_MIN, ALU_MINU, ALU_MAX, ALU_MAXU
  } alu_op_t;

  typedef enum logic [1:0] {SRC_A_RS1, SRC_A_PC, SRC_A_ZERO} src_a_t;

  typedef enum logic [1:0] {CSR_NONE, CSR_RW, CSR_RS, CSR_RC} csr_op_t;

  typedef struct packed {
    logic [4:0]  rs1;
    logic [4:0]  rs2;
    logic [4:0]  rd;
    logic        uses_rs1;
    logic        uses_rs2;
    logic        wb_en;
    logic [31:0] imm;
    alu_op_t     alu_op;
    src_a_t      src_a;
    logic        src_b_imm;
    logic        is_load;
    logic        is_store;
    logic [1:0]  mem_size;
    logic        mem_unsigned;
    logic        is_branch;
    logic [2:0]  branch_f3;
    logic        is_jal;
    logic        is_jalr;
    csr_op_t     csr_op;
    logic        csr_uses_imm;
    logic [11:0] csr_addr;
    logic        is_mdu;
    logic [2:0]  mdu_op;
    logic        is_ecall;
    logic        is_ebreak;
    logic        is_mret;
    logic        illegal;
  } decoded_t;

  // Opcodes, instr[6:2].
  parameter logic [4:0] OP_LOAD   = 5'b00000;
  parameter logic [4:0] OP_FENCE  = 5'b00011;
  parameter logic [4:0] OP_OPIMM  = 5'b00100;
  parameter logic [4:0] OP_AUIPC  = 5'b00101;
  parameter logic [4:0] OP_STORE  = 5'b01000;
  parameter logic [4:0] OP_OP     = 5'b01100;
  parameter logic [4:0] OP_LUI    = 5'b01101;
  parameter logic [4:0] OP_BRANCH = 5'b11000;
  parameter logic [4:0] OP_JALR   = 5'b11001;
  parameter logic [4:0] OP_JAL    = 5'b11011;
  parameter logic [4:0] OP_SYSTEM = 5'b11100;

  // Machine CSR addresses.
  parameter logic [11:0] CSR_MVENDORID = 12'hF11;
  parameter logic [11:0] CSR_MARCHID   = 12'hF12;
  parameter logic [11:0] CSR_MIMPID    = 12'hF13;
  parameter logic [11:0] CSR_MHARTID   = 12'hF14;
  parameter logic [11:0] CSR_MSTATUS   = 12'h300;
  parameter logic [11:0] CSR_MISA      = 12'h301;
  parameter logic [11:0] CSR_MIE       = 12'h304;
  parameter logic [11:0] CSR_MTVEC     = 12'h305;
  parameter logic [11:0] CSR_MSCRATCH  = 12'h340;
  parameter logic [11:0] CSR_MEPC      = 12'h341;
  parameter logic [11:0] CSR_MCAUSE    = 12'h342;
  parameter logic [11:0] CSR_MTVAL     = 12'h343;
  parameter logic [11:0] CSR_MIP       = 12'h344;
  parameter logic [11:0] CSR_MCYCLE    = 12'hB00;
  parameter logic [11:0] CSR_MINSTRET  = 12'hB02;
  parameter logic [11:0] CSR_MCYCLEH   = 12'hB80;
  parameter logic [11:0] CSR_MINSTRETH = 12'hB82;

  // Exception and interrupt causes.
  parameter logic [31:0] CAUSE_INSTR_MISALIGNED = 32'd0;
  parameter logic [31:0] CAUSE_ILLEGAL          = 32'd2;
  parameter logic [31:0] CAUSE_BREAKPOINT       = 32'd3;
  parameter logic [31:0] CAUSE_LOAD_MISALIGNED  = 32'd4;
  parameter logic [31:0] CAUSE_STORE_MISALIGNED = 32'd6;
  parameter logic [31:0] CAUSE_ECALL            = 32'd11;
  parameter logic [31:0] CAUSE_IRQ_TIMER        = 32'h8000_0007;
  parameter logic [31:0] CAUSE_IRQ_EXT          = 32'h8000_000B;

  parameter logic [31:0] MISA_VALUE = 32'h4000_1100;

endpackage
