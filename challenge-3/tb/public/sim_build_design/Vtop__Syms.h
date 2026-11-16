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
#include "Vtop_rv_pkg.h"

// DPI TYPES for DPI Export callbacks (Internal use)

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES) Vtop__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vtop* const __Vm_modelp;
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vtop___024root                 TOP;
    Vtop_rv_pkg                    TOP__rv_pkg;

    // SCOPE NAMES
    VerilatedScope* __Vscopep_TOP;
    VerilatedScope* __Vscopep_rv_core;
    VerilatedScope* __Vscopep_rv_core__u_alu;
    VerilatedScope* __Vscopep_rv_core__u_csr;
    VerilatedScope* __Vscopep_rv_core__u_decode;
    VerilatedScope* __Vscopep_rv_core__u_fetch;
    VerilatedScope* __Vscopep_rv_core__u_hazard;
    VerilatedScope* __Vscopep_rv_core__u_lsu;
    VerilatedScope* __Vscopep_rv_core__u_muldiv;
    VerilatedScope* __Vscopep_rv_core__u_regfile;
    VerilatedScope* __Vscopep_rv_core__u_regfile__unnamedblk1;
    VerilatedScope* __Vscopep_rv_pkg;

    // SCOPE HIERARCHY
    VerilatedHierarchy __Vhier;

    // CONSTRUCTORS
    Vtop__Syms(VerilatedContext* contextp, const char* namep, Vtop* modelp);
    ~Vtop__Syms();

    // METHODS
    const char* name() const { return TOP.vlNamep; }
};

#endif  // guard
