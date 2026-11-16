// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop__pch.h"

// Parameter definitions for Vtop_rv_pkg
constexpr CData/*4:0*/ Vtop_rv_pkg::OP_LOAD;
constexpr CData/*4:0*/ Vtop_rv_pkg::OP_FENCE;
constexpr CData/*4:0*/ Vtop_rv_pkg::OP_OPIMM;
constexpr CData/*4:0*/ Vtop_rv_pkg::OP_AUIPC;
constexpr CData/*4:0*/ Vtop_rv_pkg::OP_STORE;
constexpr CData/*4:0*/ Vtop_rv_pkg::OP_OP;
constexpr CData/*4:0*/ Vtop_rv_pkg::OP_LUI;
constexpr CData/*4:0*/ Vtop_rv_pkg::OP_BRANCH;
constexpr CData/*4:0*/ Vtop_rv_pkg::OP_JALR;
constexpr CData/*4:0*/ Vtop_rv_pkg::OP_JAL;
constexpr CData/*4:0*/ Vtop_rv_pkg::OP_SYSTEM;
constexpr SData/*11:0*/ Vtop_rv_pkg::CSR_MVENDORID;
constexpr SData/*11:0*/ Vtop_rv_pkg::CSR_MARCHID;
constexpr SData/*11:0*/ Vtop_rv_pkg::CSR_MIMPID;
constexpr SData/*11:0*/ Vtop_rv_pkg::CSR_MHARTID;
constexpr SData/*11:0*/ Vtop_rv_pkg::CSR_MSTATUS;
constexpr SData/*11:0*/ Vtop_rv_pkg::CSR_MISA;
constexpr SData/*11:0*/ Vtop_rv_pkg::CSR_MIE;
constexpr SData/*11:0*/ Vtop_rv_pkg::CSR_MTVEC;
constexpr SData/*11:0*/ Vtop_rv_pkg::CSR_MSCRATCH;
constexpr SData/*11:0*/ Vtop_rv_pkg::CSR_MEPC;
constexpr SData/*11:0*/ Vtop_rv_pkg::CSR_MCAUSE;
constexpr SData/*11:0*/ Vtop_rv_pkg::CSR_MTVAL;
constexpr SData/*11:0*/ Vtop_rv_pkg::CSR_MIP;
constexpr SData/*11:0*/ Vtop_rv_pkg::CSR_MCYCLE;
constexpr SData/*11:0*/ Vtop_rv_pkg::CSR_MINSTRET;
constexpr SData/*11:0*/ Vtop_rv_pkg::CSR_MCYCLEH;
constexpr SData/*11:0*/ Vtop_rv_pkg::CSR_MINSTRETH;
constexpr IData/*31:0*/ Vtop_rv_pkg::CAUSE_INSTR_MISALIGNED;
constexpr IData/*31:0*/ Vtop_rv_pkg::CAUSE_ILLEGAL;
constexpr IData/*31:0*/ Vtop_rv_pkg::CAUSE_BREAKPOINT;
constexpr IData/*31:0*/ Vtop_rv_pkg::CAUSE_LOAD_MISALIGNED;
constexpr IData/*31:0*/ Vtop_rv_pkg::CAUSE_STORE_MISALIGNED;
constexpr IData/*31:0*/ Vtop_rv_pkg::CAUSE_ECALL;
constexpr IData/*31:0*/ Vtop_rv_pkg::CAUSE_IRQ_TIMER;
constexpr IData/*31:0*/ Vtop_rv_pkg::CAUSE_IRQ_EXT;
constexpr IData/*31:0*/ Vtop_rv_pkg::MISA_VALUE;



Vtop_rv_pkg::Vtop_rv_pkg() = default;
Vtop_rv_pkg::~Vtop_rv_pkg() = default;

void Vtop_rv_pkg::ctor(Vtop__Syms* symsp, const char* namep) {
    vlSymsp = symsp;
    vlNamep = strdup(Verilated::catName(vlSymsp->name(), namep));
    // Reset structure values
}

void Vtop_rv_pkg::__Vconfigure(bool first) {
    (void)first;  // Prevent unused variable warning
}

void Vtop_rv_pkg::dtor() {
    VL_DO_DANGLING(std::free(const_cast<char*>(vlNamep)), vlNamep);
}
