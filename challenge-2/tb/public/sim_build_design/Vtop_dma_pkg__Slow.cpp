// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop__pch.h"

// Parameter definitions for Vtop_dma_pkg
constexpr CData/*2:0*/ Vtop_dma_pkg::R_CTRL;
constexpr CData/*2:0*/ Vtop_dma_pkg::R_SRC;
constexpr CData/*2:0*/ Vtop_dma_pkg::R_DST;
constexpr CData/*2:0*/ Vtop_dma_pkg::R_LEN;
constexpr CData/*2:0*/ Vtop_dma_pkg::R_NEXT;
constexpr CData/*2:0*/ Vtop_dma_pkg::R_STAT;
constexpr CData/*1:0*/ Vtop_dma_pkg::RESP_OKAY;
constexpr SData/*9:0*/ Vtop_dma_pkg::W_ID;
constexpr SData/*9:0*/ Vtop_dma_pkg::W_GCTRL;
constexpr SData/*9:0*/ Vtop_dma_pkg::W_IRQ_ST;
constexpr SData/*9:0*/ Vtop_dma_pkg::W_IRQ_EN;
constexpr SData/*9:0*/ Vtop_dma_pkg::W_ACTIVE;
constexpr SData/*9:0*/ Vtop_dma_pkg::W_GRANT;
constexpr IData/*31:0*/ Vtop_dma_pkg::NUM_CH;
constexpr IData/*31:0*/ Vtop_dma_pkg::UNIT_ID;
constexpr IData/*31:0*/ Vtop_dma_pkg::IRQ_MASK;



Vtop_dma_pkg::Vtop_dma_pkg() = default;
Vtop_dma_pkg::~Vtop_dma_pkg() = default;

void Vtop_dma_pkg::ctor(Vtop__Syms* symsp, const char* namep) {
    vlSymsp = symsp;
    vlNamep = strdup(Verilated::catName(vlSymsp->name(), namep));
    // Reset structure values
}

void Vtop_dma_pkg::__Vconfigure(bool first) {
    (void)first;  // Prevent unused variable warning
}

void Vtop_dma_pkg::dtor() {
    VL_DO_DANGLING(std::free(const_cast<char*>(vlNamep)), vlNamep);
}
