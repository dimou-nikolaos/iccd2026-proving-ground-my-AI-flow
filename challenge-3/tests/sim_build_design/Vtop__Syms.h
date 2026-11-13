// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VTOP__SYMS_H_
#define VERILATED_VTOP__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vtop.h"

// INCLUDE MODULE CLASSES
#include "Vtop___024root.h"
#include "Vtop___024unit.h"
#include "Vtop_rv_pkg.h"

// DPI TYPES for DPI Export callbacks (Internal use)

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES) Vtop__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vtop* const __Vm_modelp;
    std::vector<VlEvent*> __Vm_triggeredEvents;
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vtop___024root                 TOP;
    Vtop_rv_pkg                    TOP__rv_pkg;

    // SCOPE NAMES
    VerilatedScope* __Vscopep_TOP;
    VerilatedScope* __Vscopep_rv_core;
    VerilatedScope* __Vscopep_rv_core__u_alu;
    VerilatedScope* __Vscopep_rv_core__u_core_defs;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_ALU_A;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_ALU_B;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_CRD3_LEGAL;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_EX_FIRE;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_EX_LINK;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_EX_RESULT;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_FLOW_CHANGE;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_FWD_RS1;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_FWD_RS2;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_LOAD_DATA;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_LOAD_USE;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_LS_MIS;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_MDU_BUSY;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_MDU_START;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_MEMV_NXT;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_MEM_FINAL;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_MEM_STALL;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_MRET;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_REDIRECT;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_REDIRECT_PC;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_STALL_FRONT;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_TARGET_MISALIGNED;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_TRAP;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_WBDATA_NXT;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_WBE_NXT;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_WBRD_NXT;
    VerilatedScope* __Vscopep_rv_core__u_core_defs__A_WBV_NXT;
    VerilatedScope* __Vscopep_rv_core__u_csr;
    VerilatedScope* __Vscopep_rv_core__u_csr__u_csr_defs;
    VerilatedScope* __Vscopep_rv_core__u_csr__u_csr_defs__A_CSR_MEPC;
    VerilatedScope* __Vscopep_rv_core__u_csr__u_csr_defs__A_CSR_MIE;
    VerilatedScope* __Vscopep_rv_core__u_csr__u_csr_defs__A_CSR_MPIE;
    VerilatedScope* __Vscopep_rv_core__u_decode;
    VerilatedScope* __Vscopep_rv_core__u_fetch;
    VerilatedScope* __Vscopep_rv_core__u_hazard;
    VerilatedScope* __Vscopep_rv_core__u_lsu;
    VerilatedScope* __Vscopep_rv_core__u_muldiv;
    VerilatedScope* __Vscopep_rv_core__u_muldiv__u_muldiv_defs;
    VerilatedScope* __Vscopep_rv_core__u_muldiv__u_muldiv_defs__A_MDU_DIV0;
    VerilatedScope* __Vscopep_rv_core__u_muldiv__u_muldiv_defs__A_MDU_REM0;
    VerilatedScope* __Vscopep_rv_core__u_regfile;
    VerilatedScope* __Vscopep_rv_core__u_regfile__unnamedblk1;
    VerilatedScope* __Vscopep_rv_core__u_stage_sva;
    VerilatedScope* __Vscopep_rv_core__u_stage_sva__A_DEC_FIELDS;
    VerilatedScope* __Vscopep_rv_core__u_stage_sva__A_DEC_IMM;
    VerilatedScope* __Vscopep_rv_core__u_stage_sva__A_EX_ALU;
    VerilatedScope* __Vscopep_rv_core__u_stage_sva__A_EX_BRANCH;
    VerilatedScope* __Vscopep_rv_core__u_stage_sva__A_EX_TARGET_BRANCH;
    VerilatedScope* __Vscopep_rv_core__u_stage_sva__A_EX_TARGET_JALR;
    VerilatedScope* __Vscopep_rv_core__u_stage_sva__g_align__A_ID_PC_ALIGN;
    VerilatedScope* __Vscopep_rv_core__u_top_if;
    VerilatedScope* __Vscopep_rv_core__u_top_if__A_DMEM_ALIGN;
    VerilatedScope* __Vscopep_rv_core__u_top_if__A_DMEM_HOLD;
    VerilatedScope* __Vscopep_rv_core__u_top_if__A_DMEM_WDATA_HOLD;
    VerilatedScope* __Vscopep_rv_core__u_top_if__A_FIRST_FETCH_ZERO;
    VerilatedScope* __Vscopep_rv_core__u_top_if__A_IMEM_ALIGN;
    VerilatedScope* __Vscopep_rv_core__u_top_if__A_IMEM_HOLD;
    VerilatedScope* __Vscopep_rv_core__u_top_if__A_LOAD_WSTRB;
    VerilatedScope* __Vscopep_rv_core__u_top_if__A_RST_DMEM_FIRST;
    VerilatedScope* __Vscopep_rv_pkg;

    // SCOPE HIERARCHY
    VerilatedHierarchy __Vhier;

    // CONSTRUCTORS
    Vtop__Syms(VerilatedContext* contextp, const char* namep, Vtop* modelp);
    ~Vtop__Syms();

    // METHODS
    const char* name() const { return TOP.vlNamep; }
    void fireEvent(VlEvent& event) {
        if (VL_LIKELY(!event.isTriggered())) {
            __Vm_triggeredEvents.push_back(&event);
        }
        event.fire();
    }
    void clearTriggeredEvents() {
        for (const auto eventp : __Vm_triggeredEvents) eventp->clearTriggered();
        __Vm_triggeredEvents.clear();
    }
};

#endif  // guard
