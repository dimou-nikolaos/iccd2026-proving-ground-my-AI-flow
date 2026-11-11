// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop__pch.h"

VL_ATTR_COLD void Vtop___024root___eval_static(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_static\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__cand = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 7085392763118456253ull);
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

VL_ATTR_COLD void Vtop___024root___eval_static__TOP(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_static__TOP\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__cand = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 7085392763118456253ull);
}

VL_ATTR_COLD void Vtop___024root___eval_initial(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_initial\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.pready = 1U;
    vlSelfRef.m_wstrb = 0x0fU;
    vlSelfRef.dma_ctrl__DOT__pready = 1U;
    vlSelfRef.dma_ctrl__DOT__u_apb__DOT__unused_bits = 0U;
    vlSelfRef.dma_ctrl__DOT__u_apb__DOT__pready = 1U;
    vlSelfRef.dma_ctrl__DOT__m_wstrb = 0x0fU;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_wstrb = 0x0fU;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__unused_bits = 0U;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__unused_bits = 0U;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__unused_bits = 0U;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__unused_bits = 0U;
}

VL_ATTR_COLD void Vtop___024root___eval_initial__TOP(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_initial__TOP\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.pready = 1U;
    vlSelfRef.m_wstrb = 0x0fU;
    vlSelfRef.dma_ctrl__DOT__pready = 1U;
    vlSelfRef.dma_ctrl__DOT__u_apb__DOT__unused_bits = 0U;
    vlSelfRef.dma_ctrl__DOT__u_apb__DOT__pready = 1U;
    vlSelfRef.dma_ctrl__DOT__m_wstrb = 0x0fU;
    vlSelfRef.dma_ctrl__DOT__u_engine__DOT__m_wstrb = 0x0fU;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__unused_bits = 0U;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__unused_bits = 0U;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__unused_bits = 0U;
    vlSelfRef.dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__unused_bits = 0U;
}

VL_ATTR_COLD void Vtop___024root___eval_final(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_final\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vtop___024root___eval_phase__stl(Vtop___024root* vlSelf);

VL_ATTR_COLD void Vtop___024root___eval_settle(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_settle\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VstlIterCount;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00002710U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            Vtop___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
#endif
            VL_FATAL_MT("/design/rtl/dma_ctrl.sv", 3, "", "DIDNOTCONVERGE: Settle region did not converge after '--converge-limit' of 10000 tries");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        vlSelfRef.__VstlPhaseResult = Vtop___024root___eval_phase__stl(vlSelf);
        vlSelfRef.__VstlFirstIteration = 0U;
    } while (vlSelfRef.__VstlPhaseResult);
}

VL_ATTR_COLD void Vtop___024root___eval_triggers_vec__stl(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_triggers_vec__stl\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VstlTriggered[0U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VstlTriggered[0U]) 
                                     | (IData)((IData)(vlSelfRef.__VstlFirstIteration)));
}

VL_ATTR_COLD bool Vtop___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(Vtop___024root___trigger_anySet__stl(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD bool Vtop___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___trigger_anySet__stl\n"); );
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

void Vtop___024root___ico_sequent__TOP__0(Vtop___024root* vlSelf);

VL_ATTR_COLD void Vtop___024root___eval_stl(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_stl\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered[0U])) {
        Vtop___024root___ico_sequent__TOP__0(vlSelf);
    }
}

VL_ATTR_COLD bool Vtop___024root___eval_phase__stl(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__stl\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VstlExecute;
    // Body
    Vtop___024root___eval_triggers_vec__stl(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtop___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
    }
#endif
    __VstlExecute = Vtop___024root___trigger_anySet__stl(vlSelfRef.__VstlTriggered);
    if (__VstlExecute) {
        Vtop___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

bool Vtop___024root___trigger_anySet__ico(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__ico(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__ico\n"); );
    // Body
    if ((1U & (~ (IData)(Vtop___024root___trigger_anySet__ico(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'ico' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

bool Vtop___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(Vtop___024root___trigger_anySet__act(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @(posedge dma_ctrl.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 1U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 1 is active: @(negedge dma_ctrl.rst_n)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 2U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 2 is active: @(posedge dma_ctrl.u_engine.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 3U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 3 is active: @(negedge dma_ctrl.u_engine.rst_n)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 4U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 4 is active: @(posedge dma_ctrl.u_irq.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 5U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 5 is active: @(negedge dma_ctrl.u_irq.rst_n)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 6U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 6 is active: @(posedge dma_ctrl.g_chan[0].u_chan.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 7U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 7 is active: @(negedge dma_ctrl.g_chan[0].u_chan.rst_n)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 8U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 8 is active: @(posedge dma_ctrl.g_chan[1].u_chan.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 9U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 9 is active: @(negedge dma_ctrl.g_chan[1].u_chan.rst_n)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 0x0000000aU)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 10 is active: @(posedge dma_ctrl.g_chan[2].u_chan.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 0x0000000bU)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 11 is active: @(negedge dma_ctrl.g_chan[2].u_chan.rst_n)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 0x0000000cU)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 12 is active: @(posedge dma_ctrl.g_chan[3].u_chan.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 0x0000000dU)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 13 is active: @(negedge dma_ctrl.g_chan[3].u_chan.rst_n)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtop___024root___ctor_var_reset(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___ctor_var_reset\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16707436170211756652ull);
    vlSelf->rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1638864771569018232ull);
    vlSelf->psel = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5365422930610402651ull);
    vlSelf->penable = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10310706929790612258ull);
    vlSelf->pwrite = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3040259829508521558ull);
    vlSelf->paddr = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 14875722346688934266ull);
    vlSelf->pwdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 876536812074066652ull);
    vlSelf->pstrb = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 2866806653532119953ull);
    vlSelf->prdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15696981534648657427ull);
    vlSelf->pready = 1U;
    ;
    vlSelf->pslverr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8826578892537611518ull);
    vlSelf->m_awvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6959745953305777653ull);
    vlSelf->m_awready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4259156306466528033ull);
    vlSelf->m_awaddr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3169234031117663542ull);
    vlSelf->m_wvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14788020737313657040ull);
    vlSelf->m_wready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1728470165562692237ull);
    vlSelf->m_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15494961946000700504ull);
    vlSelf->m_wstrb = 15U;
    ;
    vlSelf->m_bvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8973761176877109757ull);
    vlSelf->m_bready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 866843429093315018ull);
    vlSelf->m_bresp = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 13089959803563711611ull);
    vlSelf->m_arvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14018080948827659885ull);
    vlSelf->m_arready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11734703036927835001ull);
    vlSelf->m_araddr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2374049049873106714ull);
    vlSelf->m_rvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3102679248430181142ull);
    vlSelf->m_rready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1313307773326899870ull);
    vlSelf->m_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14308343192490993725ull);
    vlSelf->m_rresp = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 6332025148105678889ull);
    vlSelf->dreq = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 2094054765515634501ull);
    vlSelf->dack = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 3939059551754344240ull);
    vlSelf->irq = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14145092342636110857ull);
    vlSelf->dma_ctrl__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 70680415479999373ull);
    vlSelf->dma_ctrl__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17103712222055831370ull);
    vlSelf->dma_ctrl__DOT__psel = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9712808186265818952ull);
    vlSelf->dma_ctrl__DOT__penable = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12008075038952813538ull);
    vlSelf->dma_ctrl__DOT__pwrite = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11087614523838536728ull);
    vlSelf->dma_ctrl__DOT__paddr = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 15221257627634254430ull);
    vlSelf->dma_ctrl__DOT__pwdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6087163052998958564ull);
    vlSelf->dma_ctrl__DOT__pstrb = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 16520499195006541580ull);
    vlSelf->dma_ctrl__DOT__prdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12832279107781513577ull);
    vlSelf->dma_ctrl__DOT__pready = 1U;
    ;
    vlSelf->dma_ctrl__DOT__pslverr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1958353716350998226ull);
    vlSelf->dma_ctrl__DOT__m_awvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12787614857809791279ull);
    vlSelf->dma_ctrl__DOT__m_awready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14421433938662346630ull);
    vlSelf->dma_ctrl__DOT__m_awaddr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8009689707205139659ull);
    vlSelf->dma_ctrl__DOT__m_wvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14037521250492926718ull);
    vlSelf->dma_ctrl__DOT__m_wready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 439549904674067170ull);
    vlSelf->dma_ctrl__DOT__m_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12504447219761460882ull);
    vlSelf->dma_ctrl__DOT__m_wstrb = 15U;
    ;
    vlSelf->dma_ctrl__DOT__m_bvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9486126054078941833ull);
    vlSelf->dma_ctrl__DOT__m_bready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5270571812696196100ull);
    vlSelf->dma_ctrl__DOT__m_bresp = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 18175884354127095217ull);
    vlSelf->dma_ctrl__DOT__m_arvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3732171784015166014ull);
    vlSelf->dma_ctrl__DOT__m_arready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2662414529521764935ull);
    vlSelf->dma_ctrl__DOT__m_araddr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13011925080555813159ull);
    vlSelf->dma_ctrl__DOT__m_rvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5918135536278173283ull);
    vlSelf->dma_ctrl__DOT__m_rready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12444180925623043133ull);
    vlSelf->dma_ctrl__DOT__m_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1313669120496508408ull);
    vlSelf->dma_ctrl__DOT__m_rresp = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 5342172261010075844ull);
    vlSelf->dma_ctrl__DOT__dreq = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14932666786149646456ull);
    vlSelf->dma_ctrl__DOT__dack = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 4318208363370615430ull);
    vlSelf->dma_ctrl__DOT__irq = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16113983044044099580ull);
    vlSelf->dma_ctrl__DOT__reg_wr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14456616586076245937ull);
    vlSelf->dma_ctrl__DOT__glob_sel = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17050785469110230277ull);
    vlSelf->dma_ctrl__DOT__chan_sel = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13498744118808479410ull);
    vlSelf->dma_ctrl__DOT__reg_wmask = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16263108474933986681ull);
    vlSelf->dma_ctrl__DOT__glob_word = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 16150555182997158931ull);
    vlSelf->dma_ctrl__DOT__chan_idx = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 370177167112584412ull);
    vlSelf->dma_ctrl__DOT__chan_reg = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 12211950895223383245ull);
    vlSelf->dma_ctrl__DOT__gctrl_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 1466374487232898446ull);
    vlSelf->dma_ctrl__DOT__gen = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1647694506708269548ull);
    vlSelf->dma_ctrl__DOT__burst = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 6824844707944216890ull);
    vlSelf->dma_ctrl__DOT__ch_active = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 16174710403572598174ull);
    vlSelf->dma_ctrl__DOT__ch_abort = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14933457856796651944ull);
    vlSelf->dma_ctrl__DOT__ch_hw_req = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 17460646320776363808ull);
    vlSelf->dma_ctrl__DOT__ch_link = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 13845031892265234755ull);
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->dma_ctrl__DOT__ch_prio[__Vi0] = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 10658169403846399405ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->dma_ctrl__DOT__ch_src[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15922613086295900629ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->dma_ctrl__DOT__ch_dst[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5896461178746471710ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->dma_ctrl__DOT__ch_len[__Vi0] = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 11457057151236730375ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->dma_ctrl__DOT__ch_next[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7804537337191048471ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->dma_ctrl__DOT__ch_ctrl_rd[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14407258961765255173ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->dma_ctrl__DOT__ch_stat_rd[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 18428567588006498281ull);
    }
    vlSelf->dma_ctrl__DOT__owned = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 12141990002651129614ull);
    vlSelf->dma_ctrl__DOT__word_done = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 564662029764134552ull);
    vlSelf->dma_ctrl__DOT__load_src = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 10422747344600105923ull);
    vlSelf->dma_ctrl__DOT__load_dst = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14092669995779623945ull);
    vlSelf->dma_ctrl__DOT__load_len = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 9573777539553858625ull);
    vlSelf->dma_ctrl__DOT__load_next = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 10885092463151221989ull);
    vlSelf->dma_ctrl__DOT__stop = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 17124726982934671265ull);
    vlSelf->dma_ctrl__DOT__ev_done = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14606404314187008430ull);
    vlSelf->dma_ctrl__DOT__ev_err = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 12360410824620029113ull);
    vlSelf->dma_ctrl__DOT__ev_desc = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 848442490074874601ull);
    vlSelf->dma_ctrl__DOT__load_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2315819059062037733ull);
    vlSelf->dma_ctrl__DOT__last_grant = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 3755944947603539ull);
    vlSelf->dma_ctrl__DOT__irq_status_rd = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8158507574105380354ull);
    vlSelf->dma_ctrl__DOT__irq_enable_rd = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8510177049068870411ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__sel = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13366576225988669131ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__sel = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15775427438853194003ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__sel = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8410731553429102118ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__sel = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10969952565495503658ull);
    vlSelf->dma_ctrl__DOT__u_apb__DOT__psel = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16768793735677386528ull);
    vlSelf->dma_ctrl__DOT__u_apb__DOT__penable = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3637494433287560572ull);
    vlSelf->dma_ctrl__DOT__u_apb__DOT__pwrite = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5347933978391297875ull);
    vlSelf->dma_ctrl__DOT__u_apb__DOT__paddr = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 8966414758408580788ull);
    vlSelf->dma_ctrl__DOT__u_apb__DOT__pstrb = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 6902382765139143829ull);
    vlSelf->dma_ctrl__DOT__u_apb__DOT__pready = 1U;
    ;
    vlSelf->dma_ctrl__DOT__u_apb__DOT__pslverr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10103738636858755458ull);
    vlSelf->dma_ctrl__DOT__u_apb__DOT__reg_wr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7044472128136064680ull);
    vlSelf->dma_ctrl__DOT__u_apb__DOT__reg_wmask = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5821803110968022082ull);
    vlSelf->dma_ctrl__DOT__u_apb__DOT__glob_sel = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9538037701873283171ull);
    vlSelf->dma_ctrl__DOT__u_apb__DOT__glob_word = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 2047457898648392025ull);
    vlSelf->dma_ctrl__DOT__u_apb__DOT__chan_sel = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9587958681222388782ull);
    vlSelf->dma_ctrl__DOT__u_apb__DOT__chan_idx = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 9215140745794866175ull);
    vlSelf->dma_ctrl__DOT__u_apb__DOT__chan_reg = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 7964311835427617963ull);
    vlSelf->dma_ctrl__DOT__u_apb__DOT__access = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11603326671560615694ull);
    vlSelf->dma_ctrl__DOT__u_apb__DOT__word = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 16843670256859458062ull);
    vlSelf->dma_ctrl__DOT__u_apb__DOT__is_glob = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3427366550429807305ull);
    vlSelf->dma_ctrl__DOT__u_apb__DOT__is_chan = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16560983351741453479ull);
    vlSelf->dma_ctrl__DOT__u_apb__DOT__mapped = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5091784615595220630ull);
    vlSelf->dma_ctrl__DOT__u_apb__DOT__unused_bits = 0U;
    ;
    vlSelf->dma_ctrl__DOT__u_engine__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2861127167054708197ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16455147098193027609ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__gen = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12899763561131620117ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__burst = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 4312607438891852002ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__ch_active = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 5306217286769052760ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__ch_abort = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 2264569871641315870ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__ch_hw_req = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 13820621586888060205ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__ch_link = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 11546743371557490665ull);
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->dma_ctrl__DOT__u_engine__DOT__ch_prio[__Vi0] = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 12664209633009249577ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->dma_ctrl__DOT__u_engine__DOT__ch_src[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13138378105789066543ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->dma_ctrl__DOT__u_engine__DOT__ch_dst[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14707057081455254422ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->dma_ctrl__DOT__u_engine__DOT__ch_len[__Vi0] = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 7251902389341212284ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->dma_ctrl__DOT__u_engine__DOT__ch_next[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7369910711590969840ull);
    }
    vlSelf->dma_ctrl__DOT__u_engine__DOT__dreq = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14591634725386577251ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__dack = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 15333976883347820813ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__owned = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 6307092085003200206ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__word_done = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14463212567441960794ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__load_src = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 4212272989091680978ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__load_dst = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 7871510413938934174ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__load_len = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 11248673482893754487ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__load_next = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 15153018722656328930ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__load_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3874461504762554771ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__stop = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 10155634047272736085ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__ev_done = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 153665045623745718ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__ev_err = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 509798362733756087ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__ev_desc = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 17426609540012115614ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__last_grant = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 7597233721412522612ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__m_awvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3138132125816750272ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__m_awready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1292830389554971698ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__m_awaddr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2631822709739496009ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__m_wvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10989554064436925899ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__m_wready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10674305633540222821ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__m_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2292584618014356962ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__m_wstrb = 15U;
    ;
    vlSelf->dma_ctrl__DOT__u_engine__DOT__m_bvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14493206684893477908ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__m_bready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11703346971025872357ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__m_bresp = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 7379979761416256977ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__m_arvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15843982918643428048ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__m_arready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15704156854115515496ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__m_araddr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7513444832072949914ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__m_rvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14437449651049270382ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__m_rready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16827779238224217503ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__m_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10768364668851192269ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__m_rresp = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 17906603029202699893ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__state_q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14626145164308793493ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__own_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 2884090000631189872ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__last_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 12743675232621732131ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__words_q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 18016975721899049591ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__fd_idx_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 16447055932630023648ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__data_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6669667907381620789ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__aw_done_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4517841153450436962ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__w_done_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2430799766659597826ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__moved_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14775481947663310623ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__eligible = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 8696649560726695991ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__grant_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5895121313017651924ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__grant_idx = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 9598113281608796415ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__own_mask = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 3588074181578900252ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__word_end = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3738439001555562426ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__burst_words = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 10975957135111619250ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__unnamedblk1__DOT__i = 0;
    vlSelf->dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__eligible = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 8469848916143821714ull);
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__prio[__Vi0] = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 12941066465514389210ull);
    }
    vlSelf->dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__last = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 4213113440242262338ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2022386443280223975ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__grant_idx = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 2351467296574585723ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__start = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 4846931454481464818ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__best_prio = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 5332538011292693934ull);
    vlSelf->dma_ctrl__DOT__u_engine__DOT__u_arb__DOT__unnamedblk1__DOT__unnamedblk2__DOT__i = 0;
    vlSelf->dma_ctrl__DOT__u_irq__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5075038263662212804ull);
    vlSelf->dma_ctrl__DOT__u_irq__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4740356150554710661ull);
    vlSelf->dma_ctrl__DOT__u_irq__DOT__wr_status = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8261430531298540348ull);
    vlSelf->dma_ctrl__DOT__u_irq__DOT__wr_enable = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17634005645950003894ull);
    vlSelf->dma_ctrl__DOT__u_irq__DOT__wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4302665542246717499ull);
    vlSelf->dma_ctrl__DOT__u_irq__DOT__wmask = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16033159064434609725ull);
    vlSelf->dma_ctrl__DOT__u_irq__DOT__ev_done = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 12512470465024328656ull);
    vlSelf->dma_ctrl__DOT__u_irq__DOT__ev_err = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14022575843888561109ull);
    vlSelf->dma_ctrl__DOT__u_irq__DOT__ev_desc = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 687027868129730852ull);
    vlSelf->dma_ctrl__DOT__u_irq__DOT__status_rd = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1864731130611958327ull);
    vlSelf->dma_ctrl__DOT__u_irq__DOT__enable_rd = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1928693705932448357ull);
    vlSelf->dma_ctrl__DOT__u_irq__DOT__irq = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3186007675522693170ull);
    vlSelf->dma_ctrl__DOT__u_irq__DOT__status_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1623523115797507620ull);
    vlSelf->dma_ctrl__DOT__u_irq__DOT__enable_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16785159229509589134ull);
    vlSelf->dma_ctrl__DOT__u_irq__DOT__set_vec = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 18272890100103806865ull);
    vlSelf->dma_ctrl__DOT__u_irq__DOT__clr_vec = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9832076426941686219ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3496387975546088009ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15966623432303365675ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wr_ctrl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16538843422400210041ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wr_src = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5520653291375582828ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wr_dst = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13045299020652210038ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wr_len = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17971119346647863064ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wr_next = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5723831181571764521ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3938706140300759538ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__wmask = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10312802763527431033ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__owned = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4912834779044523643ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__word_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11158985089934974126ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__load_src = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11664782949167676866ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__load_dst = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5054679863588087834ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__load_len = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4664494670741357906ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__load_next = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 320874167406586736ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__load_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6986326883040186361ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__stop = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14637887071416365157ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__set_err = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15843976929672973701ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__set_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11230276913733732978ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__active = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4961901959227675257ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__abort = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1743773188962320058ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__hw_req = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 437180470753726461ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__prio = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 8064219662788783777ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__link = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3362374780432644442ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16485584645727714792ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13020630740165472253ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__len = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 8510028276371055225ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__next = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11739235856444198868ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__ctrl_rd = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9029646584655748690ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__stat_rd = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16542624035574392792ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10633532737227519211ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__abort_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 574603191878308504ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__err_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3748123543515365475ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__done_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16271550206999726150ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__cfg_q = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 14577179535126341310ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src_q = VL_SCOPED_RAND_RESET_I(30, __VscopeHash, 9837398067818041250ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst_q = VL_SCOPED_RAND_RESET_I(30, __VscopeHash, 8258376748250926367ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__next_q = VL_SCOPED_RAND_RESET_I(30, __VscopeHash, 14214667963685909951ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__len_q = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 12177671745743790506ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__ctrl_wr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13230425944859049581ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__src_m = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10581948358824589749ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__dst_m = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 616289388212284570ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__next_m = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15647848563936012848ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__en_wr_visible = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2992885523796708308ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__unused_bits = 0U;
    ;
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2673236078047390945ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13801162011072298323ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wr_ctrl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 573099573293972438ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wr_src = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2363799841715464215ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wr_dst = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16281651704009247927ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wr_len = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16355263701154426583ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wr_next = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6811712438112168563ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5875451901533616959ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__wmask = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16926344298342382699ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__owned = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16028098044926263458ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__word_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6439657390516111340ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__load_src = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15670066536476929877ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__load_dst = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16370541907082558145ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__load_len = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11870485495971164165ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__load_next = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8933823674584628008ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__load_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15916141930881614083ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__stop = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 441991936459789358ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__set_err = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12691966037022865546ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__set_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16496679909935178095ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__active = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10630612508448241505ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__abort = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9799022362385172475ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__hw_req = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1396718899074657450ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__prio = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 2088260678113553577ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__link = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4112868308966995743ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1882453360681136299ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4209273238543732271ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__len = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 17894351926930751412ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__next = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15863306841383507016ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__ctrl_rd = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 877424136506548030ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__stat_rd = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17881000067294990266ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 582546847805731348ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__abort_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5462421297771134668ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__err_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6396595995708307550ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__done_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6405369281174430663ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__cfg_q = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 17214299586663049907ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src_q = VL_SCOPED_RAND_RESET_I(30, __VscopeHash, 6151227319063827896ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst_q = VL_SCOPED_RAND_RESET_I(30, __VscopeHash, 15861337238741428530ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__next_q = VL_SCOPED_RAND_RESET_I(30, __VscopeHash, 10200805331370448969ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__len_q = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 10213771341928934655ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__ctrl_wr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3149797664957044009ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__src_m = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12218195670779744852ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__dst_m = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17756331814231679489ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__next_m = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5725154431581228945ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__en_wr_visible = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7561670923050513915ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__unused_bits = 0U;
    ;
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11638366802059613492ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2868110735226800067ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wr_ctrl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9705847958477747784ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wr_src = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1650225849015756047ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wr_dst = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4933767931486060782ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wr_len = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5429412225017401198ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wr_next = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5335929297926126138ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4537395212635097681ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__wmask = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14176229434894090527ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__owned = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15219901748567275941ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__word_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8545205309335125218ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__load_src = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12763755460846992723ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__load_dst = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9754749029001101662ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__load_len = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14286614555203651983ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__load_next = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11403120900795741669ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__load_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17568215103604132296ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__stop = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7570469293641701302ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__set_err = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13523741696149102732ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__set_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13964958894469160555ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__active = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2539197913071737904ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__abort = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8596803563241789085ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__hw_req = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8659019685133151399ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__prio = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 5877708454817710109ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__link = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3710383101957883351ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10070964544008203935ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5070420105480551718ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__len = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 896036958323085224ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__next = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5978514484307606949ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__ctrl_rd = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5609660464261057807ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__stat_rd = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12547239144849023140ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5544126945692557130ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__abort_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14639042280700242044ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__err_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10145083598343366092ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__done_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15245401500709425664ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__cfg_q = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 8593976457347007901ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src_q = VL_SCOPED_RAND_RESET_I(30, __VscopeHash, 4756614925105658103ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst_q = VL_SCOPED_RAND_RESET_I(30, __VscopeHash, 11363922130105288053ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__next_q = VL_SCOPED_RAND_RESET_I(30, __VscopeHash, 5562559719186689212ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__len_q = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 7407592699883709730ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__ctrl_wr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15882476887715390042ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__src_m = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13061670060970412674ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__dst_m = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13965529373608469163ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__next_m = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7706377580011165234ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__en_wr_visible = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6056196234793892579ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__unused_bits = 0U;
    ;
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10561178885864799291ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12044538593387185892ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wr_ctrl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18194925656076170644ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wr_src = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1911329290861179870ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wr_dst = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3585864580990516616ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wr_len = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8844267965133396910ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wr_next = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16943544407209148127ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5420873647118747588ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__wmask = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2233641544127135566ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__owned = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 263090765360124839ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__word_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5567836475090004876ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__load_src = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4465507699026454746ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__load_dst = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14513105466386695603ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__load_len = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6922831778227719506ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__load_next = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8722767588196268285ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__load_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9638775975302903212ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__stop = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5943795054683549708ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__set_err = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7991665002059833897ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__set_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2226253898938018273ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__active = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11984472082643304760ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__abort = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9664682227042201085ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__hw_req = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5186438106081172122ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__prio = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 10223659833945387867ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__link = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4604311441179565133ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13123531113035702230ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12378102140588116124ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__len = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 5956804578279192105ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__next = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6057296848587736545ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__ctrl_rd = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16871724748439817164ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__stat_rd = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12285107515517482761ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6760475278966868991ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__abort_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5127822916639933772ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__err_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16938758489462672163ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__done_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14838146335503123762ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__cfg_q = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 7769426085182969950ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src_q = VL_SCOPED_RAND_RESET_I(30, __VscopeHash, 5825068334408141506ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst_q = VL_SCOPED_RAND_RESET_I(30, __VscopeHash, 10268139090330058924ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__next_q = VL_SCOPED_RAND_RESET_I(30, __VscopeHash, 2052802514726104032ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__len_q = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 2539806693987615690ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__ctrl_wr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14999947474369088466ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__src_m = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8289700565058950066ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__dst_m = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17786395788488649632ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__next_m = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 97221370621346139ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__en_wr_visible = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7536678984877285957ull);
    vlSelf->dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__unused_bits = 0U;
    ;
    vlSelf->__VdfgRegularize_h6e95ff9d_0_2 = 0;
    vlSelf->__VdfgRegularize_h6e95ff9d_0_4 = 0;
    vlSelf->__VdfgRegularize_h6e95ff9d_0_6 = 0;
    vlSelf->__VdfgRegularize_h6e95ff9d_0_8 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VstlTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VicoTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggered[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__dma_ctrl__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__dma_ctrl__DOT__rst_n__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__dma_ctrl__DOT__u_engine__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__dma_ctrl__DOT__u_engine__DOT__rst_n__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__dma_ctrl__DOT__u_irq__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__dma_ctrl__DOT__u_irq__DOT__rst_n__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__0__KET____DOT__u_chan__DOT__rst_n__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__1__KET____DOT__u_chan__DOT__rst_n__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__2__KET____DOT__u_chan__DOT__rst_n__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__dma_ctrl__DOT__g_chan__BRA__3__KET____DOT__u_chan__DOT__rst_n__0 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VnbaTriggered[__Vi0] = 0;
    }
}
