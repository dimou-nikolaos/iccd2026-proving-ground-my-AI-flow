// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop__pch.h"

void Vtop___024root___eval_triggers_vec__ico(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_triggers_vec__ico\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VicoTriggered[0U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VicoTriggered[0U]) 
                                     | (IData)((IData)(vlSelfRef.__VicoFirstIteration)));
}

bool Vtop___024root___trigger_anySet__ico(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___trigger_anySet__ico\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((1U > n));
    return (0U);
}

void Vtop___024root___ico_sequent__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___ico_sequent__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VdfgRegularize_h6e95ff9d_0_0;
    __VdfgRegularize_h6e95ff9d_0_0 = 0;
    CData/*0:0*/ __VdfgRegularize_h6e95ff9d_0_9;
    __VdfgRegularize_h6e95ff9d_0_9 = 0;
    // Body
    vlSelfRef.dma_ctrl__DOT__m_awready = vlSelfRef.m_awready;
    vlSelfRef.dma_ctrl__DOT__m_wready = vlSelfRef.m_wready;
    vlSelfRef.dma_ctrl__DOT__m_arready = vlSelfRef.m_arready;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_bready 
        = (4U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q));
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_arvalid 
        = ((1U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q)) 
           | (6U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q)));
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_rready 
        = ((2U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q)) 
           | (7U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q)));
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_wdata 
        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__data_q;
    vlSelfRef.dma_ctrl__DOT__burst = (3U & ((IData)(vlSelfRef.dma_ctrl__DOT__gctrl_q) 
                                            >> 1U));
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_awvalid 
        = ((~ (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__aw_done_q)) 
           & (3U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q)));
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_wvalid 
        = ((~ (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__w_done_q)) 
           & (3U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q)));
    vlSelfRef.dma_ctrl__DOT__u_irq__DOT__irq = (0U 
                                                != 
                                                (vlSelfRef.dma_ctrl__DOT__u_irq__DOT__status_q 
                                                 & vlSelfRef.dma_ctrl__DOT__u_irq__DOT__enable_q));
    vlSelfRef.dma_ctrl__DOT__clk = vlSelfRef.clk;
    vlSelfRef.dma_ctrl__DOT__rst_n = vlSelfRef.rst_n;
    vlSelfRef.dma_ctrl__DOT__m_rdata = vlSelfRef.m_rdata;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__last 
        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__last_q;
    vlSelfRef.dma_ctrl__DOT__m_bvalid = vlSelfRef.m_bvalid;
    vlSelfRef.dma_ctrl__DOT__m_bresp = vlSelfRef.m_bresp;
    vlSelfRef.dma_ctrl__DOT__u_irq__DOT__status_rd 
        = vlSelfRef.dma_ctrl__DOT__u_irq__DOT__status_q;
    vlSelfRef.dma_ctrl__DOT__u_irq__DOT__enable_rd 
        = vlSelfRef.dma_ctrl__DOT__u_irq__DOT__enable_q;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__last_grant 
        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__last_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__prio 
        = (3U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__prio 
        = (3U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__prio 
        = (3U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__prio 
        = (3U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__stat_rd 
        = (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__done_q) 
            << 2U) | (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__err_q) 
                       << 1U) | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__stat_rd 
        = (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__done_q) 
            << 2U) | (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__err_q) 
                       << 1U) | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__stat_rd 
        = (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__done_q) 
            << 2U) | (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__err_q) 
                       << 1U) | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__stat_rd 
        = (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__done_q) 
            << 2U) | (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__err_q) 
                       << 1U) | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q)));
    vlSelfRef.dma_ctrl__DOT__dreq = vlSelfRef.dreq;
    vlSelfRef.dma_ctrl__DOT__gen = (1U & (IData)(vlSelfRef.dma_ctrl__DOT__gctrl_q));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__ctrl_rd 
        = (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__cfg_q) 
            << 1U) | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__ctrl_rd 
        = (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__cfg_q) 
            << 1U) | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__ctrl_rd 
        = (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__cfg_q) 
            << 1U) | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__ctrl_rd 
        = (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__cfg_q) 
            << 1U) | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__abort 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__abort_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__abort 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__abort_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__abort 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__abort_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__abort 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__abort_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__hw_req 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__hw_req 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__hw_req 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__hw_req 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__active 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__active 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__active 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__active 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__len 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__len_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__len 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__len_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__len 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__len_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__len 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__len_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__link 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 5U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__link 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 5U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__link 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 5U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__link 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 5U));
    vlSelfRef.dma_ctrl__DOT__m_rvalid = vlSelfRef.m_rvalid;
    vlSelfRef.dma_ctrl__DOT__m_rresp = vlSelfRef.m_rresp;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__next 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__next_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__next 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__next_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__next 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__next_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__next 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__next_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__pwdata = vlSelfRef.pwdata;
    vlSelfRef.dma_ctrl__DOT__pwrite = vlSelfRef.pwrite;
    vlSelfRef.dma_ctrl__DOT__psel = vlSelfRef.psel;
    vlSelfRef.dma_ctrl__DOT__penable = vlSelfRef.penable;
    vlSelfRef.dma_ctrl__DOT__pstrb = vlSelfRef.pstrb;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask 
        = (0x0000000fU & ((IData)(1U) << (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q)));
    vlSelfRef.dma_ctrl__DOT__paddr = vlSelfRef.paddr;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_awready 
        = vlSelfRef.dma_ctrl__DOT__m_awready;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_wready 
        = vlSelfRef.dma_ctrl__DOT__m_wready;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_arready 
        = vlSelfRef.dma_ctrl__DOT__m_arready;
    vlSelfRef.dma_ctrl__DOT__m_bready = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_bready;
    vlSelfRef.dma_ctrl__DOT__m_arvalid = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_arvalid;
    vlSelfRef.dma_ctrl__DOT__m_rready = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_rready;
    vlSelfRef.dma_ctrl__DOT__m_wdata = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_wdata;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__burst = vlSelfRef.dma_ctrl__DOT__burst;
    vlSelfRef.dma_ctrl__DOT__m_awvalid = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_awvalid;
    vlSelfRef.dma_ctrl__DOT__m_wvalid = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_wvalid;
    vlSelfRef.dma_ctrl__DOT__irq = vlSelfRef.dma_ctrl__DOT__u_irq__DOT__irq;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__clk = vlSelfRef.dma_ctrl__DOT__clk;
    vlSelfRef.dma_ctrl__DOT__u_irq__DOT__clk = vlSelfRef.dma_ctrl__DOT__clk;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__clk 
        = vlSelfRef.dma_ctrl__DOT__clk;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__clk 
        = vlSelfRef.dma_ctrl__DOT__clk;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__clk 
        = vlSelfRef.dma_ctrl__DOT__clk;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__clk 
        = vlSelfRef.dma_ctrl__DOT__clk;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__rst_n = vlSelfRef.dma_ctrl__DOT__rst_n;
    vlSelfRef.dma_ctrl__DOT__u_irq__DOT__rst_n = vlSelfRef.dma_ctrl__DOT__rst_n;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__rst_n 
        = vlSelfRef.dma_ctrl__DOT__rst_n;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__rst_n 
        = vlSelfRef.dma_ctrl__DOT__rst_n;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__rst_n 
        = vlSelfRef.dma_ctrl__DOT__rst_n;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__rst_n 
        = vlSelfRef.dma_ctrl__DOT__rst_n;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_rdata 
        = vlSelfRef.dma_ctrl__DOT__m_rdata;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__start 
        = (3U & ((IData)(1U) + (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__last)));
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_bvalid 
        = vlSelfRef.dma_ctrl__DOT__m_bvalid;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_bresp 
        = vlSelfRef.dma_ctrl__DOT__m_bresp;
    vlSelfRef.dma_ctrl__DOT__irq_status_rd = vlSelfRef.dma_ctrl__DOT__u_irq__DOT__status_rd;
    vlSelfRef.dma_ctrl__DOT__irq_enable_rd = vlSelfRef.dma_ctrl__DOT__u_irq__DOT__enable_rd;
    vlSelfRef.dma_ctrl__DOT__last_grant = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__last_grant;
    vlSelfRef.dma_ctrl__DOT__ch_prio[0U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__prio;
    vlSelfRef.dma_ctrl__DOT__ch_prio[1U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__prio;
    vlSelfRef.dma_ctrl__DOT__ch_prio[2U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__prio;
    vlSelfRef.dma_ctrl__DOT__ch_prio[3U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__prio;
    vlSelfRef.dma_ctrl__DOT__ch_stat_rd[0U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__stat_rd;
    vlSelfRef.dma_ctrl__DOT__ch_stat_rd[1U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__stat_rd;
    vlSelfRef.dma_ctrl__DOT__ch_stat_rd[2U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__stat_rd;
    vlSelfRef.dma_ctrl__DOT__ch_stat_rd[3U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__stat_rd;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__dreq = vlSelfRef.dma_ctrl__DOT__dreq;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__gen = vlSelfRef.dma_ctrl__DOT__gen;
    vlSelfRef.dma_ctrl__DOT__ch_ctrl_rd[0U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__ctrl_rd;
    vlSelfRef.dma_ctrl__DOT__ch_ctrl_rd[1U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__ctrl_rd;
    vlSelfRef.dma_ctrl__DOT__ch_ctrl_rd[2U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__ctrl_rd;
    vlSelfRef.dma_ctrl__DOT__ch_ctrl_rd[3U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__ctrl_rd;
    vlSelfRef.dma_ctrl__DOT__ch_abort = ((((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__abort) 
                                           << 3U) | 
                                          ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__abort) 
                                           << 2U)) 
                                         | (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__abort) 
                                             << 1U) 
                                            | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__abort)));
    vlSelfRef.dma_ctrl__DOT__ch_dst[0U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst;
    vlSelfRef.dma_ctrl__DOT__ch_dst[1U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst;
    vlSelfRef.dma_ctrl__DOT__ch_dst[2U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst;
    vlSelfRef.dma_ctrl__DOT__ch_dst[3U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst;
    vlSelfRef.dma_ctrl__DOT__ch_src[0U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src;
    vlSelfRef.dma_ctrl__DOT__ch_src[1U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src;
    vlSelfRef.dma_ctrl__DOT__ch_src[2U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src;
    vlSelfRef.dma_ctrl__DOT__ch_src[3U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src;
    vlSelfRef.dma_ctrl__DOT__ch_hw_req = ((((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__hw_req) 
                                            << 3U) 
                                           | ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__hw_req) 
                                              << 2U)) 
                                          | (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__hw_req) 
                                              << 1U) 
                                             | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__hw_req)));
    vlSelfRef.dma_ctrl__DOT__ch_active = ((((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__active) 
                                            << 3U) 
                                           | ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__active) 
                                              << 2U)) 
                                          | (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__active) 
                                              << 1U) 
                                             | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__active)));
    vlSelfRef.dma_ctrl__DOT__ch_len[0U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__len;
    vlSelfRef.dma_ctrl__DOT__ch_len[1U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__len;
    vlSelfRef.dma_ctrl__DOT__ch_len[2U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__len;
    vlSelfRef.dma_ctrl__DOT__ch_len[3U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__len;
    vlSelfRef.dma_ctrl__DOT__ch_link = ((((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__link) 
                                          << 3U) | 
                                         ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__link) 
                                          << 2U)) | 
                                        (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__link) 
                                          << 1U) | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__link)));
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_rvalid 
        = vlSelfRef.dma_ctrl__DOT__m_rvalid;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_rresp 
        = vlSelfRef.dma_ctrl__DOT__m_rresp;
    vlSelfRef.dma_ctrl__DOT__ch_next[0U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__next;
    vlSelfRef.dma_ctrl__DOT__ch_next[1U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__next;
    vlSelfRef.dma_ctrl__DOT__ch_next[2U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__next;
    vlSelfRef.dma_ctrl__DOT__ch_next[3U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__next;
    vlSelfRef.dma_ctrl__DOT__u_irq__DOT__wdata = vlSelfRef.dma_ctrl__DOT__pwdata;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wdata 
        = vlSelfRef.dma_ctrl__DOT__pwdata;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wdata 
        = vlSelfRef.dma_ctrl__DOT__pwdata;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wdata 
        = vlSelfRef.dma_ctrl__DOT__pwdata;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wdata 
        = vlSelfRef.dma_ctrl__DOT__pwdata;
    vlSelfRef.dma_ctrl__DOT__u_apb__DOT__pwrite = vlSelfRef.dma_ctrl__DOT__pwrite;
    vlSelfRef.dma_ctrl__DOT__u_apb__DOT__psel = vlSelfRef.dma_ctrl__DOT__psel;
    vlSelfRef.dma_ctrl__DOT__u_apb__DOT__penable = vlSelfRef.dma_ctrl__DOT__penable;
    vlSelfRef.dma_ctrl__DOT__u_apb__DOT__pstrb = vlSelfRef.dma_ctrl__DOT__pstrb;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ev_desc = 0U;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__owned = 
        ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask) 
         & (- (IData)((0U != (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q)))));
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ev_err = 0U;
    vlSelfRef.dma_ctrl__DOT__u_apb__DOT__paddr = vlSelfRef.dma_ctrl__DOT__paddr;
    vlSelfRef.m_bready = vlSelfRef.dma_ctrl__DOT__m_bready;
    vlSelfRef.m_arvalid = vlSelfRef.dma_ctrl__DOT__m_arvalid;
    vlSelfRef.m_rready = vlSelfRef.dma_ctrl__DOT__m_rready;
    vlSelfRef.m_wdata = vlSelfRef.dma_ctrl__DOT__m_wdata;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__burst_words 
        = (0x0000000fU & ((IData)(1U) << (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__burst)));
    vlSelfRef.m_awvalid = vlSelfRef.dma_ctrl__DOT__m_awvalid;
    vlSelfRef.m_wvalid = vlSelfRef.dma_ctrl__DOT__m_wvalid;
    vlSelfRef.irq = vlSelfRef.dma_ctrl__DOT__irq;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_data 
        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_rdata;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__word_end 
        = ((4U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q)) 
           & ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_bvalid) 
              & (0U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_bresp))));
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_prio 
        = vlSelfRef.dma_ctrl__DOT__ch_prio;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_abort 
        = vlSelfRef.dma_ctrl__DOT__ch_abort;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_dst 
        = vlSelfRef.dma_ctrl__DOT__ch_dst;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_src 
        = vlSelfRef.dma_ctrl__DOT__ch_src;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_hw_req 
        = vlSelfRef.dma_ctrl__DOT__ch_hw_req;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_active 
        = vlSelfRef.dma_ctrl__DOT__ch_active;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_len 
        = vlSelfRef.dma_ctrl__DOT__ch_len;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_link 
        = vlSelfRef.dma_ctrl__DOT__ch_link;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_src = 0U;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_dst = 0U;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_len = 0U;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_next = 0U;
    if ((((7U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q)) 
          & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_rvalid)) 
         & (0U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_rresp)))) {
        if ((1U & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__fd_idx_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__fd_idx_q)))) {
                vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_src 
                    = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
            }
            if ((1U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__fd_idx_q))) {
                vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_dst 
                    = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
            }
        }
        if ((2U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__fd_idx_q))) {
            if ((1U & (~ (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__fd_idx_q)))) {
                vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_len 
                    = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
            }
            if ((1U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__fd_idx_q))) {
                vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_next 
                    = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
            }
        }
    }
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_next 
        = vlSelfRef.dma_ctrl__DOT__ch_next;
    vlSelfRef.dma_ctrl__DOT__u_apb__DOT__access = ((IData)(vlSelfRef.dma_ctrl__DOT__u_apb__DOT__psel) 
                                                   & (IData)(vlSelfRef.dma_ctrl__DOT__u_apb__DOT__penable));
    vlSelfRef.dma_ctrl__DOT__u_apb__DOT__reg_wmask 
        = ((((0x0000ff00U & ((- (IData)((1U & ((IData)(vlSelfRef.dma_ctrl__DOT__u_apb__DOT__pstrb) 
                                               >> 3U)))) 
                             << 8U)) | (0x000000ffU 
                                        & (- (IData)(
                                                     (1U 
                                                      & ((IData)(vlSelfRef.dma_ctrl__DOT__u_apb__DOT__pstrb) 
                                                         >> 2U)))))) 
            << 0x00000010U) | ((0x0000ff00U & ((- (IData)(
                                                          (1U 
                                                           & ((IData)(vlSelfRef.dma_ctrl__DOT__u_apb__DOT__pstrb) 
                                                              >> 1U)))) 
                                               << 8U)) 
                               | (0x000000ffU & (- (IData)(
                                                           (1U 
                                                            & (IData)(vlSelfRef.dma_ctrl__DOT__u_apb__DOT__pstrb)))))));
    if ((1U & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q) 
                  >> 3U)))) {
        if ((4U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
            if ((1U & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q) 
                          >> 1U)))) {
                if ((1U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
                    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ev_desc 
                        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
                }
            }
        }
    }
    vlSelfRef.dma_ctrl__DOT__ev_desc = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ev_desc;
    vlSelfRef.dma_ctrl__DOT__owned = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__owned;
    if ((8U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
        if ((1U & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q) 
                      >> 2U)))) {
            if ((1U & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q) 
                          >> 1U)))) {
                if ((1U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
                    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ev_err 
                        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
                }
            }
        }
    }
    vlSelfRef.dma_ctrl__DOT__ev_err = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ev_err;
    vlSelfRef.dma_ctrl__DOT__u_apb__DOT__word = (0x000003ffU 
                                                 & ((IData)(vlSelfRef.dma_ctrl__DOT__u_apb__DOT__paddr) 
                                                    >> 2U));
    vlSelfRef.dma_ctrl__DOT__load_data = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_data;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__word_done 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask) 
           & (- (IData)((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__word_end))));
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__prio 
        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_prio;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_awaddr 
        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_dst
        [vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q];
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i = 0U;
    while (VL_GTS_III(32, 4U, vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i)) {
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__eligible 
            = (((~ ((IData)(1U) << (3U & vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i))) 
                & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__eligible)) 
               | (0x0fU & (((((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__gen) 
                              & ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_active) 
                                 >> (3U & vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i))) 
                             & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_abort) 
                                   >> (3U & vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i)))) 
                            & (((~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_hw_req) 
                                    >> (3U & vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i))) 
                                | ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__dreq) 
                                   >> (3U & vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i))) 
                               | (0U == vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_len
                                  [(3U & vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i)]))) 
                           << (3U & vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i))));
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i 
            = ((IData)(1U) + vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i);
    }
    vlSelfRef.dma_ctrl__DOT__load_src = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_src;
    vlSelfRef.dma_ctrl__DOT__load_dst = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_dst;
    vlSelfRef.dma_ctrl__DOT__load_len = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_len;
    vlSelfRef.dma_ctrl__DOT__load_next = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_next;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_araddr 
        = ((6U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))
            ? (vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_next
               [vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q] 
               + ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__fd_idx_q) 
                  << 2U)) : vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_src
           [vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q]);
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__dack = 0U;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__stop = 0U;
    if ((8U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
        if ((1U & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q) 
                      >> 2U)))) {
            if ((1U & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q) 
                          >> 1U)))) {
                if ((1U & (~ (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q)))) {
                    if ((((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_hw_req) 
                          >> (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q)) 
                         & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__moved_q))) {
                        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__dack 
                            = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
                    }
                }
            }
            if ((2U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
                if ((1U & (~ (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q)))) {
                    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__stop 
                        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
                }
            } else if ((1U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
                vlSelfRef.dma_ctrl__DOT__u_engine__DOT__stop 
                    = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
            }
        }
    } else if ((4U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
        if ((1U & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
                if ((1U & (~ (((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_link) 
                               >> (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q)) 
                              & (0U != vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_next
                                 [vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q]))))) {
                    if ((((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_hw_req) 
                          >> (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q)) 
                         & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__moved_q))) {
                        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__dack 
                            = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
                    }
                    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__stop 
                        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
                }
            }
        }
    }
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ev_done = 0U;
    if ((1U & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q) 
                  >> 3U)))) {
        if ((4U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
            if ((1U & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q) 
                          >> 1U)))) {
                if ((1U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
                    if ((1U & (~ (((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_link) 
                                   >> (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q)) 
                                  & (0U != vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_next
                                     [vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q]))))) {
                        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ev_done 
                            = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
                    }
                }
            }
        }
    }
    vlSelfRef.dma_ctrl__DOT__reg_wmask = vlSelfRef.dma_ctrl__DOT__u_apb__DOT__reg_wmask;
    vlSelfRef.dma_ctrl__DOT__u_irq__DOT__ev_desc = vlSelfRef.dma_ctrl__DOT__ev_desc;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__owned 
        = (1U & (IData)(vlSelfRef.dma_ctrl__DOT__owned));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__owned 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__owned) 
                 >> 1U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__owned 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__owned) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__owned 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__owned) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__set_err 
        = (1U & (IData)(vlSelfRef.dma_ctrl__DOT__ev_err));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__set_err 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__ev_err) 
                 >> 1U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__set_err 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__ev_err) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__set_err 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__ev_err) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__u_irq__DOT__ev_err = vlSelfRef.dma_ctrl__DOT__ev_err;
    vlSelfRef.dma_ctrl__DOT__u_apb__DOT__glob_word 
        = vlSelfRef.dma_ctrl__DOT__u_apb__DOT__word;
    vlSelfRef.dma_ctrl__DOT__u_apb__DOT__chan_reg = 
        (7U & (IData)(vlSelfRef.dma_ctrl__DOT__u_apb__DOT__word));
    vlSelfRef.dma_ctrl__DOT__u_apb__DOT__chan_idx = 
        (3U & ((IData)(vlSelfRef.dma_ctrl__DOT__u_apb__DOT__word) 
               >> 3U));
    vlSelfRef.dma_ctrl__DOT__u_apb__DOT__is_glob = 
        (5U >= (IData)(vlSelfRef.dma_ctrl__DOT__u_apb__DOT__word));
    vlSelfRef.dma_ctrl__DOT__u_apb__DOT__is_chan = (IData)(
                                                           ((0x0040U 
                                                             == 
                                                             (0x03e0U 
                                                              & (IData)(vlSelfRef.dma_ctrl__DOT__u_apb__DOT__word))) 
                                                            & (5U 
                                                               >= 
                                                               (7U 
                                                                & (IData)(vlSelfRef.dma_ctrl__DOT__u_apb__DOT__word)))));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__load_data 
        = vlSelfRef.dma_ctrl__DOT__load_data;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__load_data 
        = vlSelfRef.dma_ctrl__DOT__load_data;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__load_data 
        = vlSelfRef.dma_ctrl__DOT__load_data;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__load_data 
        = vlSelfRef.dma_ctrl__DOT__load_data;
    vlSelfRef.dma_ctrl__DOT__word_done = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__word_done;
    vlSelfRef.dma_ctrl__DOT__m_awaddr = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_awaddr;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__eligible 
        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__eligible;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__load_src 
        = (1U & (IData)(vlSelfRef.dma_ctrl__DOT__load_src));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__load_src 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_src) 
                 >> 1U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__load_src 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_src) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__load_src 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_src) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__load_dst 
        = (1U & (IData)(vlSelfRef.dma_ctrl__DOT__load_dst));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__load_dst 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_dst) 
                 >> 1U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__load_dst 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_dst) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__load_dst 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_dst) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__load_len 
        = (1U & (IData)(vlSelfRef.dma_ctrl__DOT__load_len));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__load_len 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_len) 
                 >> 1U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__load_len 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_len) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__load_len 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_len) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__load_next 
        = (1U & (IData)(vlSelfRef.dma_ctrl__DOT__load_next));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__load_next 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_next) 
                 >> 1U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__load_next 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_next) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__load_next 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_next) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__m_araddr = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_araddr;
    vlSelfRef.dma_ctrl__DOT__dack = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__dack;
    vlSelfRef.dma_ctrl__DOT__stop = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__stop;
    vlSelfRef.dma_ctrl__DOT__ev_done = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ev_done;
    vlSelfRef.dma_ctrl__DOT__u_irq__DOT__wmask = vlSelfRef.dma_ctrl__DOT__reg_wmask;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wmask 
        = vlSelfRef.dma_ctrl__DOT__reg_wmask;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wmask 
        = vlSelfRef.dma_ctrl__DOT__reg_wmask;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wmask 
        = vlSelfRef.dma_ctrl__DOT__reg_wmask;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wmask 
        = vlSelfRef.dma_ctrl__DOT__reg_wmask;
    vlSelfRef.dma_ctrl__DOT__glob_word = vlSelfRef.dma_ctrl__DOT__u_apb__DOT__glob_word;
    vlSelfRef.dma_ctrl__DOT__chan_reg = vlSelfRef.dma_ctrl__DOT__u_apb__DOT__chan_reg;
    vlSelfRef.dma_ctrl__DOT__chan_idx = vlSelfRef.dma_ctrl__DOT__u_apb__DOT__chan_idx;
    vlSelfRef.dma_ctrl__DOT__u_apb__DOT__glob_sel = vlSelfRef.dma_ctrl__DOT__u_apb__DOT__is_glob;
    vlSelfRef.dma_ctrl__DOT__u_apb__DOT__chan_sel = vlSelfRef.dma_ctrl__DOT__u_apb__DOT__is_chan;
    vlSelfRef.dma_ctrl__DOT__u_apb__DOT__mapped = ((IData)(vlSelfRef.dma_ctrl__DOT__u_apb__DOT__is_glob) 
                                                   | (IData)(vlSelfRef.dma_ctrl__DOT__u_apb__DOT__is_chan));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__word_done 
        = (1U & (IData)(vlSelfRef.dma_ctrl__DOT__word_done));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__word_done 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__word_done) 
                 >> 1U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__word_done 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__word_done) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__word_done 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__word_done) 
                 >> 3U));
    vlSelfRef.m_awaddr = vlSelfRef.dma_ctrl__DOT__m_awaddr;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_valid = 0U;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_idx = 0U;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__best_prio = 0U;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__unnamedblk2__DOT__i = 0U;
    while (VL_GTS_III(32, 4U, vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__unnamedblk2__DOT__i)) {
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__cand 
            = (3U & ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__start) 
                     + vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__unnamedblk2__DOT__i));
        if ((1U & (((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__eligible) 
                    >> (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__cand)) 
                   & ((~ (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_valid)) 
                      | (vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__prio
                         [vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__cand] 
                         > (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__best_prio)))))) {
            vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_valid = 1U;
            vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_idx 
                = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__cand;
            vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__best_prio 
                = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__prio
                [vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__cand];
        }
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__unnamedblk2__DOT__i 
            = ((IData)(1U) + vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__unnamedblk2__DOT__i);
    }
    vlSelfRef.m_araddr = vlSelfRef.dma_ctrl__DOT__m_araddr;
    vlSelfRef.dack = vlSelfRef.dma_ctrl__DOT__dack;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__stop 
        = (1U & (IData)(vlSelfRef.dma_ctrl__DOT__stop));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__stop 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__stop) 
                 >> 1U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__stop 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__stop) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__stop 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__stop) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__set_done 
        = (1U & (IData)(vlSelfRef.dma_ctrl__DOT__ev_done));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__set_done 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__ev_done) 
                 >> 1U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__set_done 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__ev_done) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__set_done 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__ev_done) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__u_irq__DOT__ev_done = vlSelfRef.dma_ctrl__DOT__ev_done;
    vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2 = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wdata 
                                                & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wmask);
    vlSelfRef.__VdfgRegularize_h6e95ff9d_0_4 = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wdata 
                                                & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wmask);
    vlSelfRef.__VdfgRegularize_h6e95ff9d_0_6 = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wdata 
                                                & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wmask);
    vlSelfRef.__VdfgRegularize_h6e95ff9d_0_8 = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wdata 
                                                & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wmask);
    vlSelfRef.dma_ctrl__DOT__glob_sel = vlSelfRef.dma_ctrl__DOT__u_apb__DOT__glob_sel;
    vlSelfRef.dma_ctrl__DOT__chan_sel = vlSelfRef.dma_ctrl__DOT__u_apb__DOT__chan_sel;
    vlSelfRef.dma_ctrl__DOT__u_apb__DOT__pslverr = 
        ((~ (IData)(vlSelfRef.dma_ctrl__DOT__u_apb__DOT__mapped)) 
         & (IData)(vlSelfRef.dma_ctrl__DOT__u_apb__DOT__access));
    vlSelfRef.dma_ctrl__DOT__u_apb__DOT__reg_wr = ((IData)(vlSelfRef.dma_ctrl__DOT__u_apb__DOT__access) 
                                                   & ((IData)(vlSelfRef.dma_ctrl__DOT__u_apb__DOT__pwrite) 
                                                      & (IData)(vlSelfRef.dma_ctrl__DOT__u_apb__DOT__mapped)));
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__grant_valid 
        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_valid;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__grant_idx 
        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_idx;
    vlSelfRef.dma_ctrl__DOT__u_irq__DOT__set_vec = 
        (((IData)(vlSelfRef.dma_ctrl__DOT__u_irq__DOT__ev_desc) 
          << 8U) | (((IData)(vlSelfRef.dma_ctrl__DOT__u_irq__DOT__ev_err) 
                     << 4U) | (IData)(vlSelfRef.dma_ctrl__DOT__u_irq__DOT__ev_done)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__next_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__next_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__ctrl_wr 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wmask) 
              & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__ctrl_rd));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_4 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_4 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__next_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_4 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__next_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__ctrl_wr 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_4 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wmask) 
              & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__ctrl_rd));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_6 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_6 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__next_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_6 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__next_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__ctrl_wr 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_6 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wmask) 
              & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__ctrl_rd));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_8 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_8 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__next_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_8 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__next_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__ctrl_wr 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_8 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wmask) 
              & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__ctrl_rd));
    vlSelfRef.dma_ctrl__DOT__prdata = 0U;
    if ((((IData)(vlSelfRef.dma_ctrl__DOT__psel) & (IData)(vlSelfRef.dma_ctrl__DOT__penable)) 
         & (~ (IData)(vlSelfRef.dma_ctrl__DOT__pwrite)))) {
        if (vlSelfRef.dma_ctrl__DOT__glob_sel) {
            vlSelfRef.dma_ctrl__DOT__prdata = ((0U 
                                                == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word))
                                                ? 0x444d0100U
                                                : (
                                                   (1U 
                                                    == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word))
                                                    ? (IData)(vlSelfRef.dma_ctrl__DOT__gctrl_q)
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word))
                                                     ? vlSelfRef.dma_ctrl__DOT__irq_status_rd
                                                     : 
                                                    ((3U 
                                                      == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word))
                                                      ? vlSelfRef.dma_ctrl__DOT__irq_enable_rd
                                                      : 
                                                     ((4U 
                                                       == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word))
                                                       ? (IData)(vlSelfRef.dma_ctrl__DOT__ch_active)
                                                       : 
                                                      ((5U 
                                                        == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word))
                                                        ? (IData)(vlSelfRef.dma_ctrl__DOT__last_grant)
                                                        : 0U))))));
            if ((1U & (~ VL_ONEHOT_I(((((5U == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)) 
                                        << 5U) | ((
                                                   (4U 
                                                    == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)) 
                                                   << 4U) 
                                                  | ((3U 
                                                      == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)) 
                                                     << 3U))) 
                                      | (((2U == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)) 
                                          << 2U) | 
                                         (((1U == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)) 
                                           << 1U) | 
                                          (0U == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word))))))))) {
                if ((0U != ((((5U == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)) 
                              << 5U) | (((4U == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)) 
                                         << 4U) | (
                                                   (3U 
                                                    == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)) 
                                                   << 3U))) 
                            | (((2U == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)) 
                                << 2U) | (((1U == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)) 
                                           << 1U) | 
                                          (0U == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: dma_ctrl.sv:199: Assertion failed in %m: unique case, but multiple matches found for '10'h%X'\n",4, 'M',vlSymsp->name(),"dma_ctrl", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1000)
                                     , '#',10,(IData)(vlSelfRef.dma_ctrl__DOT__glob_word));
                        VL_STOP_MT("/design/rtl/dma_ctrl.sv", 199, "");
                    }
                }
            }
        } else if (vlSelfRef.dma_ctrl__DOT__chan_sel) {
            vlSelfRef.dma_ctrl__DOT__prdata = ((4U 
                                                & (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg))
                                                ? (
                                                   (2U 
                                                    & (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg))
                                                     ? vlSelfRef.dma_ctrl__DOT__ch_stat_rd
                                                    [vlSelfRef.dma_ctrl__DOT__chan_idx]
                                                     : vlSelfRef.dma_ctrl__DOT__ch_next
                                                    [vlSelfRef.dma_ctrl__DOT__chan_idx]))
                                                : (
                                                   (2U 
                                                    & (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg))
                                                     ? vlSelfRef.dma_ctrl__DOT__ch_len
                                                    [vlSelfRef.dma_ctrl__DOT__chan_idx]
                                                     : vlSelfRef.dma_ctrl__DOT__ch_dst
                                                    [vlSelfRef.dma_ctrl__DOT__chan_idx])
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg))
                                                     ? vlSelfRef.dma_ctrl__DOT__ch_src
                                                    [vlSelfRef.dma_ctrl__DOT__chan_idx]
                                                     : vlSelfRef.dma_ctrl__DOT__ch_ctrl_rd
                                                    [vlSelfRef.dma_ctrl__DOT__chan_idx])));
        }
    }
    vlSelfRef.dma_ctrl__DOT__pslverr = vlSelfRef.dma_ctrl__DOT__u_apb__DOT__pslverr;
    vlSelfRef.dma_ctrl__DOT__reg_wr = vlSelfRef.dma_ctrl__DOT__u_apb__DOT__reg_wr;
    vlSelfRef.prdata = vlSelfRef.dma_ctrl__DOT__prdata;
    vlSelfRef.pslverr = vlSelfRef.dma_ctrl__DOT__pslverr;
    __VdfgRegularize_h6e95ff9d_0_9 = ((IData)(vlSelfRef.dma_ctrl__DOT__reg_wr) 
                                      & (IData)(vlSelfRef.dma_ctrl__DOT__glob_sel));
    __VdfgRegularize_h6e95ff9d_0_0 = ((IData)(vlSelfRef.dma_ctrl__DOT__reg_wr) 
                                      & (IData)(vlSelfRef.dma_ctrl__DOT__chan_sel));
    vlSelfRef.dma_ctrl__DOT__u_irq__DOT__wr_enable 
        = ((3U == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)) 
           & (IData)(__VdfgRegularize_h6e95ff9d_0_9));
    vlSelfRef.dma_ctrl__DOT__u_irq__DOT__wr_status 
        = ((2U == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)) 
           & (IData)(__VdfgRegularize_h6e95ff9d_0_9));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__sel 
        = ((0U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_idx)) 
           & (IData)(__VdfgRegularize_h6e95ff9d_0_0));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__sel 
        = ((1U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_idx)) 
           & (IData)(__VdfgRegularize_h6e95ff9d_0_0));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__sel 
        = ((2U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_idx)) 
           & (IData)(__VdfgRegularize_h6e95ff9d_0_0));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__sel 
        = ((3U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_idx)) 
           & (IData)(__VdfgRegularize_h6e95ff9d_0_0));
    vlSelfRef.dma_ctrl__DOT__u_irq__DOT__clr_vec = 
        (vlSelfRef.dma_ctrl__DOT__u_irq__DOT__wdata 
         & (vlSelfRef.dma_ctrl__DOT__u_irq__DOT__wmask 
            & (- (IData)((IData)(vlSelfRef.dma_ctrl__DOT__u_irq__DOT__wr_status)))));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wr_src 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__sel) 
           & (1U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wr_dst 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__sel) 
           & (2U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wr_len 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__sel) 
           & (3U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wr_next 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__sel) 
           & (4U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wr_ctrl 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__sel) 
           & (0U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wr_src 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__sel) 
           & (1U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wr_dst 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__sel) 
           & (2U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wr_len 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__sel) 
           & (3U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wr_next 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__sel) 
           & (4U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wr_ctrl 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__sel) 
           & (0U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wr_src 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__sel) 
           & (1U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wr_dst 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__sel) 
           & (2U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wr_len 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__sel) 
           & (3U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wr_next 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__sel) 
           & (4U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wr_ctrl 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__sel) 
           & (0U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wr_src 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__sel) 
           & (1U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wr_dst 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__sel) 
           & (2U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wr_len 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__sel) 
           & (3U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wr_next 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__sel) 
           & (4U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wr_ctrl 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__sel) 
           & (0U == (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_wr_visible 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wr_ctrl) 
           & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wmask);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_wr_visible 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wr_ctrl) 
           & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wmask);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_wr_visible 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wr_ctrl) 
           & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wmask);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_wr_visible 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wr_ctrl) 
           & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wmask);
}

void Vtop___024root___eval_ico(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_ico\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VicoTriggered[0U])) {
        Vtop___024root___ico_sequent__TOP__0(vlSelf);
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__ico(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

bool Vtop___024root___eval_phase__ico(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__ico\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VicoExecute;
    // Body
    Vtop___024root___eval_triggers_vec__ico(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtop___024root___dump_triggers__ico(vlSelfRef.__VicoTriggered, "ico"s);
    }
#endif
    __VicoExecute = Vtop___024root___trigger_anySet__ico(vlSelfRef.__VicoTriggered);
    if (__VicoExecute) {
        Vtop___024root___eval_ico(vlSelf);
    }
    return (__VicoExecute);
}

void Vtop___024root___eval_triggers_vec__act(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_triggers_vec__act\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered[0U] = (QData)((IData)(
                                                    (((((((~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__rst_n)) 
                                                          & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__rst_n__0)) 
                                                         << 5U) 
                                                        | (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__clk) 
                                                            & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__clk__0))) 
                                                           << 4U)) 
                                                       | (((((~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__rst_n)) 
                                                             & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__rst_n__0)) 
                                                            << 3U) 
                                                           | (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__clk) 
                                                               & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__clk__0))) 
                                                              << 2U)) 
                                                          | ((((~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__rst_n)) 
                                                               & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__rst_n__0)) 
                                                              << 1U) 
                                                             | ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__clk) 
                                                                & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__clk__0)))))) 
                                                      << 8U) 
                                                     | (((((((~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__rst_n)) 
                                                             & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__rst_n__0)) 
                                                            << 3U) 
                                                           | (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__clk) 
                                                               & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__clk__0))) 
                                                              << 2U)) 
                                                          | ((((~ (IData)(vlSelfRef.dma_ctrl__DOT__u_irq__DOT__rst_n)) 
                                                               & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__u_irq__DOT__rst_n__0)) 
                                                              << 1U) 
                                                             | ((IData)(vlSelfRef.dma_ctrl__DOT__u_irq__DOT__clk) 
                                                                & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__u_irq__DOT__clk__0))))) 
                                                         << 4U) 
                                                        | (((((~ (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__rst_n)) 
                                                              & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__u_engine__DOT__rst_n__0)) 
                                                             << 3U) 
                                                            | (((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__clk) 
                                                                & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__u_engine__DOT__clk__0))) 
                                                               << 2U)) 
                                                           | ((((~ (IData)(vlSelfRef.dma_ctrl__DOT__rst_n)) 
                                                                & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__rst_n__0)) 
                                                               << 1U) 
                                                              | ((IData)(vlSelfRef.dma_ctrl__DOT__clk) 
                                                                 & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__clk__0)))))))));
    vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__clk__0 
        = vlSelfRef.dma_ctrl__DOT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__rst_n__0 
        = vlSelfRef.dma_ctrl__DOT__rst_n;
    vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__u_engine__DOT__clk__0 
        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__u_engine__DOT__rst_n__0 
        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__rst_n;
    vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__u_irq__DOT__clk__0 
        = vlSelfRef.dma_ctrl__DOT__u_irq__DOT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__u_irq__DOT__rst_n__0 
        = vlSelfRef.dma_ctrl__DOT__u_irq__DOT__rst_n;
    vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__clk__0 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__rst_n__0 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__rst_n;
    vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__clk__0 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__rst_n__0 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__rst_n;
    vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__clk__0 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__rst_n__0 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__rst_n;
    vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__clk__0 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__rst_n__0 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__rst_n;
}

bool Vtop___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___trigger_anySet__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((1U > n));
    return (0U);
}

void Vtop___024root___nba_sequent__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (vlSelfRef.dma_ctrl__DOT__u_irq__DOT__rst_n) {
        vlSelfRef.dma_ctrl__DOT__u_irq__DOT__status_q 
            = (0x00000fffU & ((vlSelfRef.dma_ctrl__DOT__u_irq__DOT__status_q 
                               & (~ vlSelfRef.dma_ctrl__DOT__u_irq__DOT__clr_vec)) 
                              | vlSelfRef.dma_ctrl__DOT__u_irq__DOT__set_vec));
        if (vlSelfRef.dma_ctrl__DOT__u_irq__DOT__wr_enable) {
            vlSelfRef.dma_ctrl__DOT__u_irq__DOT__enable_q 
                = (0x00000fffU & ((vlSelfRef.dma_ctrl__DOT__u_irq__DOT__enable_q 
                                   & (~ vlSelfRef.dma_ctrl__DOT__u_irq__DOT__wmask)) 
                                  | (vlSelfRef.dma_ctrl__DOT__u_irq__DOT__wdata 
                                     & vlSelfRef.dma_ctrl__DOT__u_irq__DOT__wmask)));
        }
    } else {
        vlSelfRef.dma_ctrl__DOT__u_irq__DOT__status_q = 0U;
        vlSelfRef.dma_ctrl__DOT__u_irq__DOT__enable_q = 0U;
    }
    vlSelfRef.dma_ctrl__DOT__u_irq__DOT__status_rd 
        = vlSelfRef.dma_ctrl__DOT__u_irq__DOT__status_q;
    vlSelfRef.dma_ctrl__DOT__u_irq__DOT__irq = (0U 
                                                != 
                                                (vlSelfRef.dma_ctrl__DOT__u_irq__DOT__status_q 
                                                 & vlSelfRef.dma_ctrl__DOT__u_irq__DOT__enable_q));
    vlSelfRef.dma_ctrl__DOT__u_irq__DOT__enable_rd 
        = vlSelfRef.dma_ctrl__DOT__u_irq__DOT__enable_q;
    vlSelfRef.dma_ctrl__DOT__irq_status_rd = vlSelfRef.dma_ctrl__DOT__u_irq__DOT__status_rd;
    vlSelfRef.dma_ctrl__DOT__irq = vlSelfRef.dma_ctrl__DOT__u_irq__DOT__irq;
    vlSelfRef.dma_ctrl__DOT__irq_enable_rd = vlSelfRef.dma_ctrl__DOT__u_irq__DOT__enable_rd;
    vlSelfRef.irq = vlSelfRef.dma_ctrl__DOT__irq;
}

void Vtop___024root___nba_sequent__TOP__1(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__1\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q = 0;
    CData/*0:0*/ __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__abort_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__abort_q = 0;
    IData/*29:0*/ __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst_q = 0;
    SData/*15:0*/ __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__len_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__len_q = 0;
    IData/*29:0*/ __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src_q = 0;
    // Body
    __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__len_q 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__len_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__abort_q 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__abort_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst_q 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src_q 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src_q;
    if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__rst_n) {
        if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wr_len) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__len_q 
                = (0x0000ffffU & (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__len_q) 
                                   & (~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wmask)) 
                                  | (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wdata 
                                     & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wmask)));
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__load_len) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__len_q 
                = (0x0000ffffU & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__load_data);
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__word_done) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__len_q 
                = (0x0000ffffU & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__len_q) 
                                  - (IData)(1U)));
        }
        if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wr_dst) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst_m 
                   >> 2U);
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__load_dst) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__load_data 
                   >> 2U);
        } else if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__word_done) 
                    & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__cfg_q) 
                          >> 1U)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst_q 
                = (0x3fffffffU & ((IData)(1U) + vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst_q));
        }
        if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wr_src) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src_m 
                   >> 2U);
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__load_src) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__load_data 
                   >> 2U);
        } else if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__word_done) 
                    & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__cfg_q)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src_q 
                = (0x3fffffffU & ((IData)(1U) + vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src_q));
        }
        if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wr_next) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q)))) {
            vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__next_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__next_m 
                   >> 2U);
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__load_next) {
            vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__next_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__load_data 
                   >> 2U);
        }
        if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__stop) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q = 0U;
            __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__abort_q = 0U;
        } else if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__abort_q) 
                    & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__owned)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q = 0U;
            __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__abort_q = 0U;
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_wr_visible) {
            if ((1U & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wdata 
                       & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q))))) {
                __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q = 1U;
            } else if (((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wdata) 
                        & (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q))) {
                __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__abort_q = 1U;
            }
        }
        if ((((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_wr_visible) 
              & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wdata) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q)))) {
            vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__err_q = 0U;
            vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__done_q = 0U;
        } else {
            if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__set_err) {
                vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__err_q = 1U;
            }
            if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__set_done) {
                vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__done_q = 1U;
            }
        }
        if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wr_ctrl) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q)))) {
            vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__cfg_q 
                = (0x0000003fU & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__ctrl_wr 
                                  >> 1U));
        }
    } else {
        __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__len_q = 0U;
        __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst_q = 0U;
        __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src_q = 0U;
        vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__next_q = 0U;
        __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q = 0U;
        __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__abort_q = 0U;
        vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__err_q = 0U;
        vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__done_q = 0U;
        vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__cfg_q = 0U;
    }
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__len_q 
        = __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__len_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst_q 
        = __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src_q 
        = __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__abort_q 
        = __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__abort_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__len 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__len_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__next 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__next_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__next_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__next_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__abort 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__abort_q;
    vlSelfRef.dma_ctrl__DOT__ch_len[0U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__len;
    vlSelfRef.dma_ctrl__DOT__ch_dst[0U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst;
    vlSelfRef.dma_ctrl__DOT__ch_src[0U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q 
        = __Vdly__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q;
    vlSelfRef.dma_ctrl__DOT__ch_next[0U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__next;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__stat_rd 
        = (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__done_q) 
            << 2U) | (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__err_q) 
                       << 1U) | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__active 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__prio 
        = (3U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__ctrl_rd 
        = (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__cfg_q) 
            << 1U) | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__hw_req 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__link 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 5U));
    vlSelfRef.dma_ctrl__DOT__ch_stat_rd[0U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__stat_rd;
    vlSelfRef.dma_ctrl__DOT__ch_prio[0U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__prio;
    vlSelfRef.dma_ctrl__DOT__ch_ctrl_rd[0U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__ctrl_rd;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__ctrl_wr 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wmask) 
              & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__ctrl_rd));
}

void Vtop___024root___nba_sequent__TOP__2(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__2\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q = 0;
    CData/*0:0*/ __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__abort_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__abort_q = 0;
    IData/*29:0*/ __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst_q = 0;
    SData/*15:0*/ __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__len_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__len_q = 0;
    IData/*29:0*/ __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src_q = 0;
    // Body
    __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__len_q 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__len_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__abort_q 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__abort_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst_q 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src_q 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src_q;
    if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__rst_n) {
        if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wr_len) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__len_q 
                = (0x0000ffffU & (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__len_q) 
                                   & (~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wmask)) 
                                  | (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wdata 
                                     & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wmask)));
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__load_len) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__len_q 
                = (0x0000ffffU & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__load_data);
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__word_done) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__len_q 
                = (0x0000ffffU & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__len_q) 
                                  - (IData)(1U)));
        }
        if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wr_dst) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst_m 
                   >> 2U);
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__load_dst) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__load_data 
                   >> 2U);
        } else if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__word_done) 
                    & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__cfg_q) 
                          >> 1U)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst_q 
                = (0x3fffffffU & ((IData)(1U) + vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst_q));
        }
        if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wr_src) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src_m 
                   >> 2U);
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__load_src) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__load_data 
                   >> 2U);
        } else if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__word_done) 
                    & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__cfg_q)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src_q 
                = (0x3fffffffU & ((IData)(1U) + vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src_q));
        }
        if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wr_next) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q)))) {
            vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__next_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__next_m 
                   >> 2U);
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__load_next) {
            vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__next_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__load_data 
                   >> 2U);
        }
        if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__stop) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q = 0U;
            __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__abort_q = 0U;
        } else if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__abort_q) 
                    & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__owned)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q = 0U;
            __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__abort_q = 0U;
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_wr_visible) {
            if ((1U & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wdata 
                       & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q))))) {
                __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q = 1U;
            } else if (((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wdata) 
                        & (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q))) {
                __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__abort_q = 1U;
            }
        }
        if ((((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_wr_visible) 
              & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wdata) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q)))) {
            vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__err_q = 0U;
            vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__done_q = 0U;
        } else {
            if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__set_err) {
                vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__err_q = 1U;
            }
            if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__set_done) {
                vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__done_q = 1U;
            }
        }
        if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wr_ctrl) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q)))) {
            vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__cfg_q 
                = (0x0000003fU & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__ctrl_wr 
                                  >> 1U));
        }
    } else {
        __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__len_q = 0U;
        __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst_q = 0U;
        __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src_q = 0U;
        vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__next_q = 0U;
        __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q = 0U;
        __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__abort_q = 0U;
        vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__err_q = 0U;
        vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__done_q = 0U;
        vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__cfg_q = 0U;
    }
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__len_q 
        = __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__len_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst_q 
        = __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src_q 
        = __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__abort_q 
        = __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__abort_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__len 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__len_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_4 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_4 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__next 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__next_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__next_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_4 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__next_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__abort 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__abort_q;
    vlSelfRef.dma_ctrl__DOT__ch_len[1U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__len;
    vlSelfRef.dma_ctrl__DOT__ch_dst[1U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst;
    vlSelfRef.dma_ctrl__DOT__ch_src[1U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q 
        = __Vdly__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q;
    vlSelfRef.dma_ctrl__DOT__ch_next[1U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__next;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__stat_rd 
        = (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__done_q) 
            << 2U) | (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__err_q) 
                       << 1U) | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__active 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__prio 
        = (3U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__ctrl_rd 
        = (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__cfg_q) 
            << 1U) | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__hw_req 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__link 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 5U));
    vlSelfRef.dma_ctrl__DOT__ch_stat_rd[1U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__stat_rd;
    vlSelfRef.dma_ctrl__DOT__ch_prio[1U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__prio;
    vlSelfRef.dma_ctrl__DOT__ch_ctrl_rd[1U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__ctrl_rd;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__ctrl_wr 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_4 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wmask) 
              & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__ctrl_rd));
}

void Vtop___024root___nba_sequent__TOP__3(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__3\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q = 0;
    CData/*0:0*/ __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__abort_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__abort_q = 0;
    IData/*29:0*/ __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst_q = 0;
    SData/*15:0*/ __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__len_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__len_q = 0;
    IData/*29:0*/ __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src_q = 0;
    // Body
    __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__len_q 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__len_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__abort_q 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__abort_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst_q 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src_q 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src_q;
    if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__rst_n) {
        if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wr_len) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__len_q 
                = (0x0000ffffU & (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__len_q) 
                                   & (~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wmask)) 
                                  | (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wdata 
                                     & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wmask)));
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__load_len) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__len_q 
                = (0x0000ffffU & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__load_data);
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__word_done) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__len_q 
                = (0x0000ffffU & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__len_q) 
                                  - (IData)(1U)));
        }
        if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wr_dst) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst_m 
                   >> 2U);
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__load_dst) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__load_data 
                   >> 2U);
        } else if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__word_done) 
                    & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__cfg_q) 
                          >> 1U)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst_q 
                = (0x3fffffffU & ((IData)(1U) + vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst_q));
        }
        if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wr_src) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src_m 
                   >> 2U);
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__load_src) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__load_data 
                   >> 2U);
        } else if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__word_done) 
                    & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__cfg_q)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src_q 
                = (0x3fffffffU & ((IData)(1U) + vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src_q));
        }
        if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wr_next) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q)))) {
            vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__next_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__next_m 
                   >> 2U);
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__load_next) {
            vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__next_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__load_data 
                   >> 2U);
        }
        if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__stop) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q = 0U;
            __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__abort_q = 0U;
        } else if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__abort_q) 
                    & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__owned)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q = 0U;
            __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__abort_q = 0U;
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_wr_visible) {
            if ((1U & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wdata 
                       & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q))))) {
                __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q = 1U;
            } else if (((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wdata) 
                        & (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q))) {
                __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__abort_q = 1U;
            }
        }
        if ((((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_wr_visible) 
              & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wdata) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q)))) {
            vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__err_q = 0U;
            vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__done_q = 0U;
        } else {
            if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__set_err) {
                vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__err_q = 1U;
            }
            if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__set_done) {
                vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__done_q = 1U;
            }
        }
        if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wr_ctrl) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q)))) {
            vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__cfg_q 
                = (0x0000003fU & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__ctrl_wr 
                                  >> 1U));
        }
    } else {
        __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__len_q = 0U;
        __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst_q = 0U;
        __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src_q = 0U;
        vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__next_q = 0U;
        __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q = 0U;
        __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__abort_q = 0U;
        vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__err_q = 0U;
        vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__done_q = 0U;
        vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__cfg_q = 0U;
    }
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__len_q 
        = __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__len_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst_q 
        = __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src_q 
        = __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__abort_q 
        = __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__abort_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__len 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__len_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_6 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_6 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__next 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__next_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__next_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_6 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__next_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__abort 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__abort_q;
    vlSelfRef.dma_ctrl__DOT__ch_len[2U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__len;
    vlSelfRef.dma_ctrl__DOT__ch_dst[2U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst;
    vlSelfRef.dma_ctrl__DOT__ch_src[2U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q 
        = __Vdly__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q;
    vlSelfRef.dma_ctrl__DOT__ch_next[2U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__next;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__stat_rd 
        = (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__done_q) 
            << 2U) | (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__err_q) 
                       << 1U) | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__active 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__prio 
        = (3U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__ctrl_rd 
        = (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__cfg_q) 
            << 1U) | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__hw_req 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__link 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 5U));
    vlSelfRef.dma_ctrl__DOT__ch_stat_rd[2U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__stat_rd;
    vlSelfRef.dma_ctrl__DOT__ch_prio[2U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__prio;
    vlSelfRef.dma_ctrl__DOT__ch_ctrl_rd[2U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__ctrl_rd;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__ctrl_wr 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_6 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wmask) 
              & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__ctrl_rd));
}

void Vtop___024root___nba_sequent__TOP__4(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__4\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q = 0;
    CData/*0:0*/ __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__abort_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__abort_q = 0;
    IData/*29:0*/ __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst_q = 0;
    SData/*15:0*/ __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__len_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__len_q = 0;
    IData/*29:0*/ __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src_q = 0;
    // Body
    __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__len_q 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__len_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__abort_q 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__abort_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst_q 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst_q;
    __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src_q 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src_q;
    if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__rst_n) {
        if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wr_len) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__len_q 
                = (0x0000ffffU & (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__len_q) 
                                   & (~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wmask)) 
                                  | (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wdata 
                                     & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wmask)));
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__load_len) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__len_q 
                = (0x0000ffffU & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__load_data);
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__word_done) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__len_q 
                = (0x0000ffffU & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__len_q) 
                                  - (IData)(1U)));
        }
        if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wr_dst) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst_m 
                   >> 2U);
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__load_dst) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__load_data 
                   >> 2U);
        } else if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__word_done) 
                    & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__cfg_q) 
                          >> 1U)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst_q 
                = (0x3fffffffU & ((IData)(1U) + vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst_q));
        }
        if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wr_src) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src_m 
                   >> 2U);
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__load_src) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__load_data 
                   >> 2U);
        } else if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__word_done) 
                    & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__cfg_q)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src_q 
                = (0x3fffffffU & ((IData)(1U) + vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src_q));
        }
        if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wr_next) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q)))) {
            vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__next_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__next_m 
                   >> 2U);
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__load_next) {
            vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__next_q 
                = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__load_data 
                   >> 2U);
        }
        if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__stop) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q = 0U;
            __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__abort_q = 0U;
        } else if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__abort_q) 
                    & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__owned)))) {
            __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q = 0U;
            __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__abort_q = 0U;
        } else if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_wr_visible) {
            if ((1U & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wdata 
                       & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q))))) {
                __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q = 1U;
            } else if (((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wdata) 
                        & (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q))) {
                __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__abort_q = 1U;
            }
        }
        if ((((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_wr_visible) 
              & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wdata) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q)))) {
            vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__err_q = 0U;
            vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__done_q = 0U;
        } else {
            if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__set_err) {
                vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__err_q = 1U;
            }
            if (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__set_done) {
                vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__done_q = 1U;
            }
        }
        if (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wr_ctrl) 
             & (~ (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q)))) {
            vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__cfg_q 
                = (0x0000003fU & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__ctrl_wr 
                                  >> 1U));
        }
    } else {
        __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__len_q = 0U;
        __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst_q = 0U;
        __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src_q = 0U;
        vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__next_q = 0U;
        __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q = 0U;
        __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__abort_q = 0U;
        vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__err_q = 0U;
        vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__done_q = 0U;
        vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__cfg_q = 0U;
    }
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__len_q 
        = __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__len_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst_q 
        = __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src_q 
        = __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__abort_q 
        = __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__abort_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__len 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__len_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_8 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_8 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__next 
        = (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__next_q 
           << 2U);
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__next_m 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_8 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wmask) 
              & (vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__next_q 
                 << 2U)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__abort 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__abort_q;
    vlSelfRef.dma_ctrl__DOT__ch_len[3U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__len;
    vlSelfRef.dma_ctrl__DOT__ch_dst[3U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst;
    vlSelfRef.dma_ctrl__DOT__ch_src[3U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q 
        = __Vdly__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q;
    vlSelfRef.dma_ctrl__DOT__ch_next[3U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__next;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__stat_rd 
        = (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__done_q) 
            << 2U) | (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__err_q) 
                       << 1U) | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q)));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__active 
        = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__prio 
        = (3U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__ctrl_rd 
        = (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__cfg_q) 
            << 1U) | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__hw_req 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__link 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__cfg_q) 
                 >> 5U));
    vlSelfRef.dma_ctrl__DOT__ch_stat_rd[3U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__stat_rd;
    vlSelfRef.dma_ctrl__DOT__ch_prio[3U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__prio;
    vlSelfRef.dma_ctrl__DOT__ch_ctrl_rd[3U] = vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__ctrl_rd;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__ctrl_wr 
        = (vlSelfRef.__VdfgRegularize_h6e95ff9d_0_8 
           | ((~ vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wmask) 
              & vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__ctrl_rd));
}

void Vtop___024root___nba_sequent__TOP__5(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__5\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (vlSelfRef.dma_ctrl__DOT__rst_n) {
        if ((((IData)(vlSelfRef.dma_ctrl__DOT__reg_wr) 
              & (IData)(vlSelfRef.dma_ctrl__DOT__glob_sel)) 
             & (1U == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)))) {
            vlSelfRef.dma_ctrl__DOT__gctrl_q = (7U 
                                                & (((IData)(vlSelfRef.dma_ctrl__DOT__gctrl_q) 
                                                    & (~ vlSelfRef.dma_ctrl__DOT__reg_wmask)) 
                                                   | (vlSelfRef.dma_ctrl__DOT__pwdata 
                                                      & vlSelfRef.dma_ctrl__DOT__reg_wmask)));
        }
    } else {
        vlSelfRef.dma_ctrl__DOT__gctrl_q = 0U;
    }
    vlSelfRef.dma_ctrl__DOT__burst = (3U & ((IData)(vlSelfRef.dma_ctrl__DOT__gctrl_q) 
                                            >> 1U));
    vlSelfRef.dma_ctrl__DOT__gen = (1U & (IData)(vlSelfRef.dma_ctrl__DOT__gctrl_q));
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__burst = vlSelfRef.dma_ctrl__DOT__burst;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__gen = vlSelfRef.dma_ctrl__DOT__gen;
}

void Vtop___024root___nba_sequent__TOP__6(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__6\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*3:0*/ __Vdly__dma_ctrl__DOT__u_engine__DOT__state_q;
    __Vdly__dma_ctrl__DOT__u_engine__DOT__state_q = 0;
    CData/*3:0*/ __Vdly__dma_ctrl__DOT__u_engine__DOT__words_q;
    __Vdly__dma_ctrl__DOT__u_engine__DOT__words_q = 0;
    // Body
    __Vdly__dma_ctrl__DOT__u_engine__DOT__words_q = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__words_q;
    __Vdly__dma_ctrl__DOT__u_engine__DOT__state_q = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q;
    if (vlSelfRef.dma_ctrl__DOT__u_engine__DOT__rst_n) {
        if ((8U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
            __Vdly__dma_ctrl__DOT__u_engine__DOT__state_q = 0U;
        } else if ((4U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
            if ((2U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
                if ((1U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
                    if (vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_rvalid) {
                        if ((0U != (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_rresp))) {
                            __Vdly__dma_ctrl__DOT__u_engine__DOT__state_q = 9U;
                        } else if ((3U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__fd_idx_q))) {
                            __Vdly__dma_ctrl__DOT__u_engine__DOT__state_q = 8U;
                        } else {
                            vlSelfRef.dma_ctrl__DOT__u_engine__DOT__fd_idx_q 
                                = (3U & ((IData)(1U) 
                                         + (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__fd_idx_q)));
                            __Vdly__dma_ctrl__DOT__u_engine__DOT__state_q = 6U;
                        }
                    }
                } else if (vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_arready) {
                    __Vdly__dma_ctrl__DOT__u_engine__DOT__state_q = 7U;
                }
            } else if ((1U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
                if ((((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_link) 
                      >> (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q)) 
                     & (0U != vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_next
                        [vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q]))) {
                    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__fd_idx_q = 0U;
                    __Vdly__dma_ctrl__DOT__u_engine__DOT__state_q = 6U;
                } else {
                    __Vdly__dma_ctrl__DOT__u_engine__DOT__state_q = 0U;
                }
            } else if (vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_bvalid) {
                if ((0U != (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_bresp))) {
                    __Vdly__dma_ctrl__DOT__u_engine__DOT__state_q = 9U;
                } else {
                    __Vdly__dma_ctrl__DOT__u_engine__DOT__words_q 
                        = (0x0000000fU & ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__words_q) 
                                          - (IData)(1U)));
                    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__moved_q = 1U;
                    __Vdly__dma_ctrl__DOT__u_engine__DOT__state_q 
                        = ((1U & ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_abort) 
                                  >> (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q)))
                            ? 0x0aU : ((1U >= vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_len
                                        [vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q])
                                        ? 5U : ((1U 
                                                 == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__words_q))
                                                 ? 8U
                                                 : 1U)));
                }
            }
        } else if ((2U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
            if ((1U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
                if ((((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__aw_done_q) 
                      | (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_awready)) 
                     & ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__w_done_q) 
                        | (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_wready)))) {
                    __Vdly__dma_ctrl__DOT__u_engine__DOT__state_q = 4U;
                }
                if (((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_awvalid) 
                     & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_awready))) {
                    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__aw_done_q = 1U;
                }
                if (((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_wvalid) 
                     & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_wready))) {
                    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__w_done_q = 1U;
                }
            } else if (vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_rvalid) {
                if ((0U != (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_rresp))) {
                    __Vdly__dma_ctrl__DOT__u_engine__DOT__state_q = 9U;
                } else {
                    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__data_q 
                        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_rdata;
                    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__aw_done_q = 0U;
                    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__w_done_q = 0U;
                    __Vdly__dma_ctrl__DOT__u_engine__DOT__state_q = 3U;
                }
            }
        } else if ((1U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
            if (vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_arready) {
                __Vdly__dma_ctrl__DOT__u_engine__DOT__state_q = 2U;
            }
        } else if (vlSelfRef.dma_ctrl__DOT__u_engine__DOT__grant_valid) {
            vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q 
                = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__grant_idx;
            vlSelfRef.dma_ctrl__DOT__u_engine__DOT__last_q 
                = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__grant_idx;
            __Vdly__dma_ctrl__DOT__u_engine__DOT__words_q 
                = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__burst_words;
            vlSelfRef.dma_ctrl__DOT__u_engine__DOT__moved_q = 0U;
            __Vdly__dma_ctrl__DOT__u_engine__DOT__state_q 
                = ((0U == vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_len
                    [vlSelfRef.dma_ctrl__DOT__u_engine__DOT__grant_idx])
                    ? 5U : 1U);
        }
    } else {
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__fd_idx_q = 0U;
        __Vdly__dma_ctrl__DOT__u_engine__DOT__state_q = 0U;
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q = 0U;
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__last_q = 3U;
        __Vdly__dma_ctrl__DOT__u_engine__DOT__words_q = 0U;
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__data_q = 0U;
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__aw_done_q = 0U;
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__w_done_q = 0U;
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__moved_q = 0U;
    }
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__words_q 
        = __Vdly__dma_ctrl__DOT__u_engine__DOT__words_q;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q 
        = __Vdly__dma_ctrl__DOT__u_engine__DOT__state_q;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_wdata 
        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__data_q;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__last 
        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__last_q;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__last_grant 
        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__last_q;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_bready 
        = (4U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q));
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_arvalid 
        = ((1U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q)) 
           | (6U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q)));
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_rready 
        = ((2U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q)) 
           | (7U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q)));
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_awvalid 
        = ((~ (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__aw_done_q)) 
           & (3U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q)));
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_wvalid 
        = ((~ (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__w_done_q)) 
           & (3U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q)));
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__word_end 
        = ((4U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q)) 
           & ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_bvalid) 
              & (0U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_bresp))));
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask 
        = (0x0000000fU & ((IData)(1U) << (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q)));
    vlSelfRef.dma_ctrl__DOT__m_wdata = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_wdata;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__start 
        = (3U & ((IData)(1U) + (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__last)));
    vlSelfRef.dma_ctrl__DOT__last_grant = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__last_grant;
    vlSelfRef.dma_ctrl__DOT__m_bready = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_bready;
    vlSelfRef.dma_ctrl__DOT__m_arvalid = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_arvalid;
    vlSelfRef.dma_ctrl__DOT__m_rready = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_rready;
    vlSelfRef.dma_ctrl__DOT__m_awvalid = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_awvalid;
    vlSelfRef.dma_ctrl__DOT__m_wvalid = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_wvalid;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ev_desc = 0U;
    if ((1U & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q) 
                  >> 3U)))) {
        if ((4U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
            if ((1U & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q) 
                          >> 1U)))) {
                if ((1U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
                    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ev_desc 
                        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
                }
            }
        }
    }
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__owned = 
        ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask) 
         & (- (IData)((0U != (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q)))));
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__word_done 
        = ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask) 
           & (- (IData)((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__word_end))));
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_src = 0U;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_dst = 0U;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_len = 0U;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_next = 0U;
    if ((((7U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q)) 
          & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_rvalid)) 
         & (0U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_rresp)))) {
        if ((1U & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__fd_idx_q) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__fd_idx_q)))) {
                vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_src 
                    = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
            }
            if ((1U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__fd_idx_q))) {
                vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_dst 
                    = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
            }
        }
        if ((2U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__fd_idx_q))) {
            if ((1U & (~ (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__fd_idx_q)))) {
                vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_len 
                    = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
            }
            if ((1U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__fd_idx_q))) {
                vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_next 
                    = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
            }
        }
    }
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ev_err = 0U;
    if ((8U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
        if ((1U & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q) 
                      >> 2U)))) {
            if ((1U & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q) 
                          >> 1U)))) {
                if ((1U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
                    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ev_err 
                        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
                }
            }
        }
    }
    vlSelfRef.m_wdata = vlSelfRef.dma_ctrl__DOT__m_wdata;
    vlSelfRef.m_bready = vlSelfRef.dma_ctrl__DOT__m_bready;
    vlSelfRef.m_arvalid = vlSelfRef.dma_ctrl__DOT__m_arvalid;
    vlSelfRef.m_rready = vlSelfRef.dma_ctrl__DOT__m_rready;
    vlSelfRef.m_awvalid = vlSelfRef.dma_ctrl__DOT__m_awvalid;
    vlSelfRef.m_wvalid = vlSelfRef.dma_ctrl__DOT__m_wvalid;
    vlSelfRef.dma_ctrl__DOT__ev_desc = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ev_desc;
    vlSelfRef.dma_ctrl__DOT__owned = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__owned;
    vlSelfRef.dma_ctrl__DOT__word_done = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__word_done;
    vlSelfRef.dma_ctrl__DOT__load_src = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_src;
    vlSelfRef.dma_ctrl__DOT__load_dst = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_dst;
    vlSelfRef.dma_ctrl__DOT__load_len = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_len;
    vlSelfRef.dma_ctrl__DOT__load_next = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__load_next;
    vlSelfRef.dma_ctrl__DOT__ev_err = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ev_err;
    vlSelfRef.dma_ctrl__DOT__u_irq__DOT__ev_desc = vlSelfRef.dma_ctrl__DOT__ev_desc;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__owned 
        = (1U & (IData)(vlSelfRef.dma_ctrl__DOT__owned));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__owned 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__owned) 
                 >> 1U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__owned 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__owned) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__owned 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__owned) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__word_done 
        = (1U & (IData)(vlSelfRef.dma_ctrl__DOT__word_done));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__word_done 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__word_done) 
                 >> 1U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__word_done 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__word_done) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__word_done 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__word_done) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__load_src 
        = (1U & (IData)(vlSelfRef.dma_ctrl__DOT__load_src));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__load_src 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_src) 
                 >> 1U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__load_src 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_src) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__load_src 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_src) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__load_dst 
        = (1U & (IData)(vlSelfRef.dma_ctrl__DOT__load_dst));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__load_dst 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_dst) 
                 >> 1U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__load_dst 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_dst) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__load_dst 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_dst) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__load_len 
        = (1U & (IData)(vlSelfRef.dma_ctrl__DOT__load_len));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__load_len 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_len) 
                 >> 1U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__load_len 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_len) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__load_len 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_len) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__load_next 
        = (1U & (IData)(vlSelfRef.dma_ctrl__DOT__load_next));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__load_next 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_next) 
                 >> 1U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__load_next 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_next) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__load_next 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__load_next) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__set_err 
        = (1U & (IData)(vlSelfRef.dma_ctrl__DOT__ev_err));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__set_err 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__ev_err) 
                 >> 1U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__set_err 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__ev_err) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__set_err 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__ev_err) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__u_irq__DOT__ev_err = vlSelfRef.dma_ctrl__DOT__ev_err;
}

void Vtop___024root___nba_comb__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.dma_ctrl__DOT__ch_abort = ((((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__abort) 
                                           << 3U) | 
                                          ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__abort) 
                                           << 2U)) 
                                         | (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__abort) 
                                             << 1U) 
                                            | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__abort)));
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_dst 
        = vlSelfRef.dma_ctrl__DOT__ch_dst;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_src 
        = vlSelfRef.dma_ctrl__DOT__ch_src;
    vlSelfRef.dma_ctrl__DOT__ch_active = ((((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__active) 
                                            << 3U) 
                                           | ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__active) 
                                              << 2U)) 
                                          | (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__active) 
                                              << 1U) 
                                             | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__active)));
    vlSelfRef.dma_ctrl__DOT__ch_hw_req = ((((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__hw_req) 
                                            << 3U) 
                                           | ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__hw_req) 
                                              << 2U)) 
                                          | (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__hw_req) 
                                              << 1U) 
                                             | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__hw_req)));
    vlSelfRef.dma_ctrl__DOT__ch_link = ((((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__link) 
                                          << 3U) | 
                                         ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__link) 
                                          << 2U)) | 
                                        (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__link) 
                                          << 1U) | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__link)));
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_prio 
        = vlSelfRef.dma_ctrl__DOT__ch_prio;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_len 
        = vlSelfRef.dma_ctrl__DOT__ch_len;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_next 
        = vlSelfRef.dma_ctrl__DOT__ch_next;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_abort 
        = vlSelfRef.dma_ctrl__DOT__ch_abort;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_active 
        = vlSelfRef.dma_ctrl__DOT__ch_active;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_hw_req 
        = vlSelfRef.dma_ctrl__DOT__ch_hw_req;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_link 
        = vlSelfRef.dma_ctrl__DOT__ch_link;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__prio 
        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_prio;
}

void Vtop___024root___nba_sequent__TOP__7(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__7\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__burst_words 
        = (0x0000000fU & ((IData)(1U) << (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__burst)));
}

void Vtop___024root___nba_comb__TOP__1(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__1\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_awaddr 
        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_dst
        [vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q];
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_araddr 
        = ((6U == (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))
            ? (vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_next
               [vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q] 
               + ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__fd_idx_q) 
                  << 2U)) : vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_src
           [vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q]);
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__dack = 0U;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__stop = 0U;
    if ((8U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
        if ((1U & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q) 
                      >> 2U)))) {
            if ((1U & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q) 
                          >> 1U)))) {
                if ((1U & (~ (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q)))) {
                    if ((((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_hw_req) 
                          >> (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q)) 
                         & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__moved_q))) {
                        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__dack 
                            = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
                    }
                }
            }
            if ((2U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
                if ((1U & (~ (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q)))) {
                    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__stop 
                        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
                }
            } else if ((1U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
                vlSelfRef.dma_ctrl__DOT__u_engine__DOT__stop 
                    = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
            }
        }
    } else if ((4U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
        if ((1U & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q) 
                      >> 1U)))) {
            if ((1U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
                if ((1U & (~ (((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_link) 
                               >> (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q)) 
                              & (0U != vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_next
                                 [vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q]))))) {
                    if ((((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_hw_req) 
                          >> (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q)) 
                         & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__moved_q))) {
                        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__dack 
                            = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
                    }
                    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__stop 
                        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
                }
            }
        }
    }
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ev_done = 0U;
    if ((1U & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q) 
                  >> 3U)))) {
        if ((4U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
            if ((1U & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q) 
                          >> 1U)))) {
                if ((1U & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__state_q))) {
                    if ((1U & (~ (((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_link) 
                                   >> (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q)) 
                                  & (0U != vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_next
                                     [vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_q]))))) {
                        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ev_done 
                            = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__own_mask;
                    }
                }
            }
        }
    }
    vlSelfRef.dma_ctrl__DOT__m_awaddr = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_awaddr;
    vlSelfRef.dma_ctrl__DOT__m_araddr = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_araddr;
    vlSelfRef.dma_ctrl__DOT__dack = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__dack;
    vlSelfRef.dma_ctrl__DOT__stop = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__stop;
    vlSelfRef.dma_ctrl__DOT__ev_done = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ev_done;
    vlSelfRef.m_awaddr = vlSelfRef.dma_ctrl__DOT__m_awaddr;
    vlSelfRef.m_araddr = vlSelfRef.dma_ctrl__DOT__m_araddr;
    vlSelfRef.dack = vlSelfRef.dma_ctrl__DOT__dack;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__stop 
        = (1U & (IData)(vlSelfRef.dma_ctrl__DOT__stop));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__stop 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__stop) 
                 >> 1U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__stop 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__stop) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__stop 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__stop) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__set_done 
        = (1U & (IData)(vlSelfRef.dma_ctrl__DOT__ev_done));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__set_done 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__ev_done) 
                 >> 1U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__set_done 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__ev_done) 
                 >> 2U));
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__set_done 
        = (1U & ((IData)(vlSelfRef.dma_ctrl__DOT__ev_done) 
                 >> 3U));
    vlSelfRef.dma_ctrl__DOT__u_irq__DOT__ev_done = vlSelfRef.dma_ctrl__DOT__ev_done;
    vlSelfRef.dma_ctrl__DOT__u_irq__DOT__set_vec = 
        (((IData)(vlSelfRef.dma_ctrl__DOT__u_irq__DOT__ev_desc) 
          << 8U) | (((IData)(vlSelfRef.dma_ctrl__DOT__u_irq__DOT__ev_err) 
                     << 4U) | (IData)(vlSelfRef.dma_ctrl__DOT__u_irq__DOT__ev_done)));
}

void Vtop___024root___nba_comb__TOP__2(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__2\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.dma_ctrl__DOT__prdata = 0U;
    if ((((IData)(vlSelfRef.dma_ctrl__DOT__psel) & (IData)(vlSelfRef.dma_ctrl__DOT__penable)) 
         & (~ (IData)(vlSelfRef.dma_ctrl__DOT__pwrite)))) {
        if (vlSelfRef.dma_ctrl__DOT__glob_sel) {
            vlSelfRef.dma_ctrl__DOT__prdata = ((0U 
                                                == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word))
                                                ? 0x444d0100U
                                                : (
                                                   (1U 
                                                    == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word))
                                                    ? (IData)(vlSelfRef.dma_ctrl__DOT__gctrl_q)
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word))
                                                     ? vlSelfRef.dma_ctrl__DOT__irq_status_rd
                                                     : 
                                                    ((3U 
                                                      == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word))
                                                      ? vlSelfRef.dma_ctrl__DOT__irq_enable_rd
                                                      : 
                                                     ((4U 
                                                       == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word))
                                                       ? (IData)(vlSelfRef.dma_ctrl__DOT__ch_active)
                                                       : 
                                                      ((5U 
                                                        == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word))
                                                        ? (IData)(vlSelfRef.dma_ctrl__DOT__last_grant)
                                                        : 0U))))));
            if ((1U & (~ VL_ONEHOT_I(((((5U == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)) 
                                        << 5U) | ((
                                                   (4U 
                                                    == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)) 
                                                   << 4U) 
                                                  | ((3U 
                                                      == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)) 
                                                     << 3U))) 
                                      | (((2U == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)) 
                                          << 2U) | 
                                         (((1U == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)) 
                                           << 1U) | 
                                          (0U == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word))))))))) {
                if ((0U != ((((5U == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)) 
                              << 5U) | (((4U == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)) 
                                         << 4U) | (
                                                   (3U 
                                                    == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)) 
                                                   << 3U))) 
                            | (((2U == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)) 
                                << 2U) | (((1U == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)) 
                                           << 1U) | 
                                          (0U == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: dma_ctrl.sv:199: Assertion failed in %m: unique case, but multiple matches found for '10'h%X'\n",4, 'M',vlSymsp->name(),"dma_ctrl", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1000)
                                     , '#',10,(IData)(vlSelfRef.dma_ctrl__DOT__glob_word));
                        VL_STOP_MT("/design/rtl/dma_ctrl.sv", 199, "");
                    }
                }
            }
        } else if (vlSelfRef.dma_ctrl__DOT__chan_sel) {
            vlSelfRef.dma_ctrl__DOT__prdata = ((4U 
                                                & (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg))
                                                ? (
                                                   (2U 
                                                    & (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg))
                                                     ? vlSelfRef.dma_ctrl__DOT__ch_stat_rd
                                                    [vlSelfRef.dma_ctrl__DOT__chan_idx]
                                                     : vlSelfRef.dma_ctrl__DOT__ch_next
                                                    [vlSelfRef.dma_ctrl__DOT__chan_idx]))
                                                : (
                                                   (2U 
                                                    & (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg))
                                                     ? vlSelfRef.dma_ctrl__DOT__ch_len
                                                    [vlSelfRef.dma_ctrl__DOT__chan_idx]
                                                     : vlSelfRef.dma_ctrl__DOT__ch_dst
                                                    [vlSelfRef.dma_ctrl__DOT__chan_idx])
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.dma_ctrl__DOT__chan_reg))
                                                     ? vlSelfRef.dma_ctrl__DOT__ch_src
                                                    [vlSelfRef.dma_ctrl__DOT__chan_idx]
                                                     : vlSelfRef.dma_ctrl__DOT__ch_ctrl_rd
                                                    [vlSelfRef.dma_ctrl__DOT__chan_idx])));
        }
    }
    vlSelfRef.prdata = vlSelfRef.dma_ctrl__DOT__prdata;
}

void Vtop___024root___nba_comb__TOP__3(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__3\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i = 0U;
    while (VL_GTS_III(32, 4U, vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i)) {
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__eligible 
            = (((~ ((IData)(1U) << (3U & vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i))) 
                & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__eligible)) 
               | (0x0fU & (((((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__gen) 
                              & ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_active) 
                                 >> (3U & vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i))) 
                             & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_abort) 
                                   >> (3U & vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i)))) 
                            & (((~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_hw_req) 
                                    >> (3U & vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i))) 
                                | ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__dreq) 
                                   >> (3U & vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i))) 
                               | (0U == vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_len
                                  [(3U & vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i)]))) 
                           << (3U & vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i))));
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i 
            = ((IData)(1U) + vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i);
    }
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__eligible 
        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__eligible;
}

void Vtop___024root___nba_comb__TOP__4(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__4\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_valid = 0U;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_idx = 0U;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__best_prio = 0U;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__unnamedblk2__DOT__i = 0U;
    while (VL_GTS_III(32, 4U, vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__unnamedblk2__DOT__i)) {
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__cand 
            = (3U & ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__start) 
                     + vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__unnamedblk2__DOT__i));
        if ((1U & (((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__eligible) 
                    >> (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__cand)) 
                   & ((~ (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_valid)) 
                      | (vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__prio
                         [vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__cand] 
                         > (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__best_prio)))))) {
            vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_valid = 1U;
            vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_idx 
                = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__cand;
            vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__best_prio 
                = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__prio
                [vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__cand];
        }
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__unnamedblk2__DOT__i 
            = ((IData)(1U) + vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__unnamedblk2__DOT__i);
    }
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__grant_valid 
        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_valid;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__grant_idx 
        = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_idx;
}

void Vtop___024root___eval_nba(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_nba\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((0x0000000000000030ULL & vlSelfRef.__VnbaTriggered[0U])) {
        if (vlSelfRef.dma_ctrl__DOT__u_irq__DOT__rst_n) {
            vlSelfRef.dma_ctrl__DOT__u_irq__DOT__status_q 
                = (0x00000fffU & ((vlSelfRef.dma_ctrl__DOT__u_irq__DOT__status_q 
                                   & (~ vlSelfRef.dma_ctrl__DOT__u_irq__DOT__clr_vec)) 
                                  | vlSelfRef.dma_ctrl__DOT__u_irq__DOT__set_vec));
            if (vlSelfRef.dma_ctrl__DOT__u_irq__DOT__wr_enable) {
                vlSelfRef.dma_ctrl__DOT__u_irq__DOT__enable_q 
                    = (0x00000fffU & ((vlSelfRef.dma_ctrl__DOT__u_irq__DOT__enable_q 
                                       & (~ vlSelfRef.dma_ctrl__DOT__u_irq__DOT__wmask)) 
                                      | (vlSelfRef.dma_ctrl__DOT__u_irq__DOT__wdata 
                                         & vlSelfRef.dma_ctrl__DOT__u_irq__DOT__wmask)));
            }
        } else {
            vlSelfRef.dma_ctrl__DOT__u_irq__DOT__status_q = 0U;
            vlSelfRef.dma_ctrl__DOT__u_irq__DOT__enable_q = 0U;
        }
        vlSelfRef.dma_ctrl__DOT__u_irq__DOT__status_rd 
            = vlSelfRef.dma_ctrl__DOT__u_irq__DOT__status_q;
        vlSelfRef.dma_ctrl__DOT__u_irq__DOT__irq = 
            (0U != (vlSelfRef.dma_ctrl__DOT__u_irq__DOT__status_q 
                    & vlSelfRef.dma_ctrl__DOT__u_irq__DOT__enable_q));
        vlSelfRef.dma_ctrl__DOT__u_irq__DOT__enable_rd 
            = vlSelfRef.dma_ctrl__DOT__u_irq__DOT__enable_q;
        vlSelfRef.dma_ctrl__DOT__irq_status_rd = vlSelfRef.dma_ctrl__DOT__u_irq__DOT__status_rd;
        vlSelfRef.dma_ctrl__DOT__irq = vlSelfRef.dma_ctrl__DOT__u_irq__DOT__irq;
        vlSelfRef.dma_ctrl__DOT__irq_enable_rd = vlSelfRef.dma_ctrl__DOT__u_irq__DOT__enable_rd;
        vlSelfRef.irq = vlSelfRef.dma_ctrl__DOT__irq;
    }
    if ((0x00000000000000c0ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__1(vlSelf);
    }
    if ((0x0000000000000300ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__2(vlSelf);
    }
    if ((0x0000000000000c00ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__3(vlSelf);
    }
    if ((0x0000000000003000ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__4(vlSelf);
    }
    if ((3ULL & vlSelfRef.__VnbaTriggered[0U])) {
        if (vlSelfRef.dma_ctrl__DOT__rst_n) {
            if ((((IData)(vlSelfRef.dma_ctrl__DOT__reg_wr) 
                  & (IData)(vlSelfRef.dma_ctrl__DOT__glob_sel)) 
                 & (1U == (IData)(vlSelfRef.dma_ctrl__DOT__glob_word)))) {
                vlSelfRef.dma_ctrl__DOT__gctrl_q = 
                    (7U & (((IData)(vlSelfRef.dma_ctrl__DOT__gctrl_q) 
                            & (~ vlSelfRef.dma_ctrl__DOT__reg_wmask)) 
                           | (vlSelfRef.dma_ctrl__DOT__pwdata 
                              & vlSelfRef.dma_ctrl__DOT__reg_wmask)));
            }
        } else {
            vlSelfRef.dma_ctrl__DOT__gctrl_q = 0U;
        }
        vlSelfRef.dma_ctrl__DOT__burst = (3U & ((IData)(vlSelfRef.dma_ctrl__DOT__gctrl_q) 
                                                >> 1U));
        vlSelfRef.dma_ctrl__DOT__gen = (1U & (IData)(vlSelfRef.dma_ctrl__DOT__gctrl_q));
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__burst 
            = vlSelfRef.dma_ctrl__DOT__burst;
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__gen 
            = vlSelfRef.dma_ctrl__DOT__gen;
    }
    if ((0x000000000000000cULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__6(vlSelf);
    }
    if ((0x0000000000003fc0ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.dma_ctrl__DOT__ch_abort = ((((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__abort) 
                                               << 3U) 
                                              | ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__abort) 
                                                 << 2U)) 
                                             | (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__abort) 
                                                 << 1U) 
                                                | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__abort)));
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_dst 
            = vlSelfRef.dma_ctrl__DOT__ch_dst;
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_src 
            = vlSelfRef.dma_ctrl__DOT__ch_src;
        vlSelfRef.dma_ctrl__DOT__ch_active = ((((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__active) 
                                                << 3U) 
                                               | ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__active) 
                                                  << 2U)) 
                                              | (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__active) 
                                                  << 1U) 
                                                 | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__active)));
        vlSelfRef.dma_ctrl__DOT__ch_hw_req = ((((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__hw_req) 
                                                << 3U) 
                                               | ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__hw_req) 
                                                  << 2U)) 
                                              | (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__hw_req) 
                                                  << 1U) 
                                                 | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__hw_req)));
        vlSelfRef.dma_ctrl__DOT__ch_link = ((((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__link) 
                                              << 3U) 
                                             | ((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__link) 
                                                << 2U)) 
                                            | (((IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__link) 
                                                << 1U) 
                                               | (IData)(vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__link)));
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_prio 
            = vlSelfRef.dma_ctrl__DOT__ch_prio;
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_len 
            = vlSelfRef.dma_ctrl__DOT__ch_len;
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_next 
            = vlSelfRef.dma_ctrl__DOT__ch_next;
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_abort 
            = vlSelfRef.dma_ctrl__DOT__ch_abort;
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_active 
            = vlSelfRef.dma_ctrl__DOT__ch_active;
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_hw_req 
            = vlSelfRef.dma_ctrl__DOT__ch_hw_req;
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_link 
            = vlSelfRef.dma_ctrl__DOT__ch_link;
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__prio 
            = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_prio;
    }
    if ((3ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__burst_words 
            = (0x0000000fU & ((IData)(1U) << (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__burst)));
    }
    if ((0x0000000000003fccULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_comb__TOP__1(vlSelf);
    }
    if ((0x0000000000003fffULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_comb__TOP__2(vlSelf);
    }
    if ((0x0000000000003fc3ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i = 0U;
        while (VL_GTS_III(32, 4U, vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i)) {
            vlSelfRef.dma_ctrl__DOT__u_engine__DOT__eligible 
                = (((~ ((IData)(1U) << (3U & vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i))) 
                    & (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__eligible)) 
                   | (0x0fU & (((((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__gen) 
                                  & ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_active) 
                                     >> (3U & vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i))) 
                                 & (~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_abort) 
                                       >> (3U & vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i)))) 
                                & (((~ ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_hw_req) 
                                        >> (3U & vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i))) 
                                    | ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__dreq) 
                                       >> (3U & vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i))) 
                                   | (0U == vlSelfRef.dma_ctrl__DOT__u_engine__DOT__ch_len
                                      [(3U & vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i)]))) 
                               << (3U & vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i))));
            vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i 
                = ((IData)(1U) + vlSelfRef.dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i);
        }
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__eligible 
            = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__eligible;
    }
    if ((0x0000000000003fcfULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_valid = 0U;
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_idx = 0U;
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__best_prio = 0U;
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__unnamedblk2__DOT__i = 0U;
        while (VL_GTS_III(32, 4U, vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__unnamedblk2__DOT__i)) {
            vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__cand 
                = (3U & ((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__start) 
                         + vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__unnamedblk2__DOT__i));
            if ((1U & (((IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__eligible) 
                        >> (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__cand)) 
                       & ((~ (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_valid)) 
                          | (vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__prio
                             [vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__cand] 
                             > (IData)(vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__best_prio)))))) {
                vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_valid = 1U;
                vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_idx 
                    = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__cand;
                vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__best_prio 
                    = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__prio
                    [vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__cand];
            }
            vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__unnamedblk2__DOT__i 
                = ((IData)(1U) + vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__unnamedblk2__DOT__i);
        }
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__grant_valid 
            = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_valid;
        vlSelfRef.dma_ctrl__DOT__u_engine__DOT__grant_idx 
            = vlSelfRef.dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_idx;
    }
}

void Vtop___024root___trigger_orInto__act_vec_vec(VlUnpacked<QData/*63:0*/, 1> &out, const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___trigger_orInto__act_vec_vec\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = (out[n] | in[n]);
        n = ((IData)(1U) + n);
    } while ((0U >= n));
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

bool Vtop___024root___eval_phase__act(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__act\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vtop___024root___eval_triggers_vec__act(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtop___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
    }
#endif
    Vtop___024root___trigger_orInto__act_vec_vec(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    return (0U);
}

void Vtop___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 1> &out) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___trigger_clear__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = 0ULL;
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vtop___024root___eval_phase__nba(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__nba\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = Vtop___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        Vtop___024root___eval_nba(vlSelf);
        Vtop___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

void Vtop___024root___eval(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VicoIterCount;
    IData/*31:0*/ __VnbaIterCount;
    // Body
    __VicoIterCount = 0U;
    vlSelfRef.__VicoFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00002710U < __VicoIterCount)))) {
#ifdef VL_DEBUG
            Vtop___024root___dump_triggers__ico(vlSelfRef.__VicoTriggered, "ico"s);
#endif
            VL_FATAL_MT("/design/rtl/dma_ctrl.sv", 3, "", "DIDNOTCONVERGE: Input combinational region did not converge after '--converge-limit' of 10000 tries");
        }
        __VicoIterCount = ((IData)(1U) + __VicoIterCount);
        vlSelfRef.__VicoPhaseResult = Vtop___024root___eval_phase__ico(vlSelf);
        vlSelfRef.__VicoFirstIteration = 0U;
    } while (vlSelfRef.__VicoPhaseResult);
    __VnbaIterCount = 0U;
    do {
        if (VL_UNLIKELY(((0x00002710U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vtop___024root___dump_triggers__act(vlSelfRef.__VnbaTriggered, "nba"s);
#endif
            VL_FATAL_MT("/design/rtl/dma_ctrl.sv", 3, "", "DIDNOTCONVERGE: NBA region did not converge after '--converge-limit' of 10000 tries");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        vlSelfRef.__VactIterCount = 0U;
        do {
            if (VL_UNLIKELY(((0x00002710U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vtop___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
                VL_FATAL_MT("/design/rtl/dma_ctrl.sv", 3, "", "DIDNOTCONVERGE: Active region did not converge after '--converge-limit' of 10000 tries");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
            vlSelfRef.__VactPhaseResult = Vtop___024root___eval_phase__act(vlSelf);
        } while (vlSelfRef.__VactPhaseResult);
        vlSelfRef.__VnbaPhaseResult = Vtop___024root___eval_phase__nba(vlSelf);
    } while (vlSelfRef.__VnbaPhaseResult);
}

#ifdef VL_DEBUG
void Vtop___024root___eval_debug_assertions(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_debug_assertions\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (VL_UNLIKELY(((vlSelfRef.clk & 0xfeU)))) {
        Verilated::overWidthError("clk");
    }
    if (VL_UNLIKELY(((vlSelfRef.rst_n & 0xfeU)))) {
        Verilated::overWidthError("rst_n");
    }
    if (VL_UNLIKELY(((vlSelfRef.psel & 0xfeU)))) {
        Verilated::overWidthError("psel");
    }
    if (VL_UNLIKELY(((vlSelfRef.penable & 0xfeU)))) {
        Verilated::overWidthError("penable");
    }
    if (VL_UNLIKELY(((vlSelfRef.pwrite & 0xfeU)))) {
        Verilated::overWidthError("pwrite");
    }
    if (VL_UNLIKELY(((vlSelfRef.paddr & 0xf000U)))) {
        Verilated::overWidthError("paddr");
    }
    if (VL_UNLIKELY(((vlSelfRef.pstrb & 0xf0U)))) {
        Verilated::overWidthError("pstrb");
    }
    if (VL_UNLIKELY(((vlSelfRef.m_awready & 0xfeU)))) {
        Verilated::overWidthError("m_awready");
    }
    if (VL_UNLIKELY(((vlSelfRef.m_wready & 0xfeU)))) {
        Verilated::overWidthError("m_wready");
    }
    if (VL_UNLIKELY(((vlSelfRef.m_bvalid & 0xfeU)))) {
        Verilated::overWidthError("m_bvalid");
    }
    if (VL_UNLIKELY(((vlSelfRef.m_bresp & 0xfcU)))) {
        Verilated::overWidthError("m_bresp");
    }
    if (VL_UNLIKELY(((vlSelfRef.m_arready & 0xfeU)))) {
        Verilated::overWidthError("m_arready");
    }
    if (VL_UNLIKELY(((vlSelfRef.m_rvalid & 0xfeU)))) {
        Verilated::overWidthError("m_rvalid");
    }
    if (VL_UNLIKELY(((vlSelfRef.m_rresp & 0xfcU)))) {
        Verilated::overWidthError("m_rresp");
    }
    if (VL_UNLIKELY(((vlSelfRef.dreq & 0xf0U)))) {
        Verilated::overWidthError("dreq");
    }
}
#endif  // VL_DEBUG
