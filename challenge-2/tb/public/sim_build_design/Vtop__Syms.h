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
#include "Vtop_dma_pkg.h"

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
    Vtop_dma_pkg                   TOP__dma_pkg;

    // SCOPE NAMES
    VerilatedScope* __Vscopep_TOP;
    VerilatedScope* __Vscopep_dma_ctrl;
    VerilatedScope* __Vscopep_dma_ctrl__g_chan__BRA__0__KET__;
    VerilatedScope* __Vscopep_dma_ctrl__g_chan__BRA__0__KET____u_chan;
    VerilatedScope* __Vscopep_dma_ctrl__g_chan__BRA__1__KET__;
    VerilatedScope* __Vscopep_dma_ctrl__g_chan__BRA__1__KET____u_chan;
    VerilatedScope* __Vscopep_dma_ctrl__g_chan__BRA__2__KET__;
    VerilatedScope* __Vscopep_dma_ctrl__g_chan__BRA__2__KET____u_chan;
    VerilatedScope* __Vscopep_dma_ctrl__g_chan__BRA__3__KET__;
    VerilatedScope* __Vscopep_dma_ctrl__g_chan__BRA__3__KET____u_chan;
    VerilatedScope* __Vscopep_dma_ctrl__u_apb;
    VerilatedScope* __Vscopep_dma_ctrl__u_engine;
    VerilatedScope* __Vscopep_dma_ctrl__u_engine__u_arb;
    VerilatedScope* __Vscopep_dma_ctrl__u_engine__u_arb__unnamedblk1;
    VerilatedScope* __Vscopep_dma_ctrl__u_engine__u_arb__unnamedblk1__unnamedblk2;
    VerilatedScope* __Vscopep_dma_ctrl__u_engine__unnamedblk1;
    VerilatedScope* __Vscopep_dma_ctrl__u_irq;
    VerilatedScope* __Vscopep_dma_pkg;

    // SCOPE HIERARCHY
    VerilatedHierarchy __Vhier;

    // CONSTRUCTORS
    Vtop__Syms(VerilatedContext* contextp, const char* namep, Vtop* modelp);
    ~Vtop__Syms();

    // METHODS
    const char* name() const { return TOP.vlNamep; }
};

#endif  // guard
