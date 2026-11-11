// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vtop.h for the primary calling header

#ifndef VERILATED_VTOP_DMA_PKG_H_
#define VERILATED_VTOP_DMA_PKG_H_  // guard

#include "verilated.h"


class Vtop__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vtop_dma_pkg final {
  public:

    // INTERNAL VARIABLES
    Vtop__Syms* vlSymsp;
    const char* vlNamep;

    // PARAMETERS
    static constexpr CData/*2:0*/ R_CTRL = 0U;
    static constexpr CData/*2:0*/ R_SRC = 1U;
    static constexpr CData/*2:0*/ R_DST = 2U;
    static constexpr CData/*2:0*/ R_LEN = 3U;
    static constexpr CData/*2:0*/ R_NEXT = 4U;
    static constexpr CData/*2:0*/ R_STAT = 5U;
    static constexpr CData/*1:0*/ RESP_OKAY = 0U;
    static constexpr SData/*9:0*/ W_ID = 0U;
    static constexpr SData/*9:0*/ W_GCTRL = 1U;
    static constexpr SData/*9:0*/ W_IRQ_ST = 2U;
    static constexpr SData/*9:0*/ W_IRQ_EN = 3U;
    static constexpr SData/*9:0*/ W_ACTIVE = 4U;
    static constexpr SData/*9:0*/ W_GRANT = 5U;
    static constexpr IData/*31:0*/ NUM_CH = 4U;
    static constexpr IData/*31:0*/ UNIT_ID = 0x444d0100U;
    static constexpr IData/*31:0*/ IRQ_MASK = 0x00000fffU;

    // CONSTRUCTORS
    Vtop_dma_pkg();
    ~Vtop_dma_pkg();
    void ctor(Vtop__Syms* symsp, const char* namep);
    void dtor();
    VL_UNCOPYABLE(Vtop_dma_pkg);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
