// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vtop.h for the primary calling header

#ifndef VERILATED_VTOP_RV_PKG_H_
#define VERILATED_VTOP_RV_PKG_H_  // guard

#include "verilated.h"


class Vtop__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vtop_rv_pkg final {
  public:

    // INTERNAL VARIABLES
    Vtop__Syms* vlSymsp;
    const char* vlNamep;

    // PARAMETERS
    static constexpr CData/*4:0*/ OP_LOAD = 0U;
    static constexpr CData/*4:0*/ OP_FENCE = 3U;
    static constexpr CData/*4:0*/ OP_OPIMM = 4U;
    static constexpr CData/*4:0*/ OP_AUIPC = 5U;
    static constexpr CData/*4:0*/ OP_STORE = 8U;
    static constexpr CData/*4:0*/ OP_OP = 0x0cU;
    static constexpr CData/*4:0*/ OP_LUI = 0x0dU;
    static constexpr CData/*4:0*/ OP_BRANCH = 0x18U;
    static constexpr CData/*4:0*/ OP_JALR = 0x19U;
    static constexpr CData/*4:0*/ OP_JAL = 0x1bU;
    static constexpr CData/*4:0*/ OP_SYSTEM = 0x1cU;
    static constexpr SData/*11:0*/ CSR_MVENDORID = 0x0f11U;
    static constexpr SData/*11:0*/ CSR_MARCHID = 0x0f12U;
    static constexpr SData/*11:0*/ CSR_MIMPID = 0x0f13U;
    static constexpr SData/*11:0*/ CSR_MHARTID = 0x0f14U;
    static constexpr SData/*11:0*/ CSR_MSTATUS = 0x0300U;
    static constexpr SData/*11:0*/ CSR_MISA = 0x0301U;
    static constexpr SData/*11:0*/ CSR_MIE = 0x0304U;
    static constexpr SData/*11:0*/ CSR_MTVEC = 0x0305U;
    static constexpr SData/*11:0*/ CSR_MSCRATCH = 0x0340U;
    static constexpr SData/*11:0*/ CSR_MEPC = 0x0341U;
    static constexpr SData/*11:0*/ CSR_MCAUSE = 0x0342U;
    static constexpr SData/*11:0*/ CSR_MTVAL = 0x0343U;
    static constexpr SData/*11:0*/ CSR_MIP = 0x0344U;
    static constexpr SData/*11:0*/ CSR_MCYCLE = 0x0b00U;
    static constexpr SData/*11:0*/ CSR_MINSTRET = 0x0b02U;
    static constexpr SData/*11:0*/ CSR_MCYCLEH = 0x0b80U;
    static constexpr SData/*11:0*/ CSR_MINSTRETH = 0x0b82U;
    static constexpr IData/*31:0*/ CAUSE_INSTR_MISALIGNED = 0U;
    static constexpr IData/*31:0*/ CAUSE_ILLEGAL = 2U;
    static constexpr IData/*31:0*/ CAUSE_BREAKPOINT = 3U;
    static constexpr IData/*31:0*/ CAUSE_LOAD_MISALIGNED = 4U;
    static constexpr IData/*31:0*/ CAUSE_STORE_MISALIGNED = 6U;
    static constexpr IData/*31:0*/ CAUSE_ECALL = 0x0000000bU;
    static constexpr IData/*31:0*/ CAUSE_IRQ_TIMER = 0x80000007U;
    static constexpr IData/*31:0*/ CAUSE_IRQ_EXT = 0x8000000bU;
    static constexpr IData/*31:0*/ MISA_VALUE = 0x40001100U;

    // CONSTRUCTORS
    Vtop_rv_pkg();
    ~Vtop_rv_pkg();
    void ctor(Vtop__Syms* symsp, const char* namep);
    void dtor();
    VL_UNCOPYABLE(Vtop_rv_pkg);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
