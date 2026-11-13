// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop__pch.h"

VL_ATTR_COLD void Vtop___024root___eval_static(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_static\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__clk__0 
        = vlSelfRef.rv_core__DOT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__rst_n__0 
        = vlSelfRef.rv_core__DOT__rst_n;
    vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_fetch__DOT__clk__0 
        = vlSelfRef.rv_core__DOT__u_fetch__DOT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_fetch__DOT__rst_n__0 
        = vlSelfRef.rv_core__DOT__u_fetch__DOT__rst_n;
    vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_regfile__DOT__clk__0 
        = vlSelfRef.rv_core__DOT__u_regfile__DOT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_regfile__DOT__rst_n__0 
        = vlSelfRef.rv_core__DOT__u_regfile__DOT__rst_n;
    vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_csr__DOT__clk__0 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_csr__DOT__rst_n__0 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__rst_n;
    vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__clk__0 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_muldiv__DOT__clk__0 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_muldiv__DOT__rst_n__0 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__rst_n;
    vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__clk__0 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__rst_n__0 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__rst_n;
    vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_core_defs__DOT__clk__0 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_stage_sva__DOT__clk__0 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_top_if__DOT__clk__0 
        = vlSelfRef.rv_core__DOT__u_top_if__DOT__clk;
}

VL_ATTR_COLD void Vtop___024root___eval_initial(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_initial\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_addr_isa = 0U;
    vlSelfRef.rv_core__DOT__unused_bits = 0U;
    vlSelfRef.rv_core__DOT__u_csr__DOT__unused_bits = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__if_valid = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__if_pc = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__if_instr = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_valid = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_instr = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_base = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_addr = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_store_data = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_we = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_re = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_size = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_rdata = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_load_data = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__wb_valid = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__wb_instr = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__wb_we = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__wb_rd = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__wb_data = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__redirect_valid = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__redirect_pc = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__flush_ex = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__flush_mem = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__flush_wb = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_opcode = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_funct3 = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__wb_opcode = 0U;
}

VL_ATTR_COLD void Vtop___024root___eval_initial__TOP(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_initial__TOP\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_addr_isa = 0U;
    vlSelfRef.rv_core__DOT__unused_bits = 0U;
    vlSelfRef.rv_core__DOT__u_csr__DOT__unused_bits = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__if_valid = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__if_pc = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__if_instr = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_valid = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_instr = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_base = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_addr = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_store_data = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_we = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_re = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_size = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_rdata = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_load_data = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__wb_valid = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__wb_instr = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__wb_we = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__wb_rd = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__wb_data = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__redirect_valid = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__redirect_pc = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__flush_ex = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__flush_mem = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__flush_wb = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_opcode = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_funct3 = 0U;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__wb_opcode = 0U;
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
            VL_FATAL_MT("/design/rtl/rv_core.sv", 3, "", "DIDNOTCONVERGE: Settle region did not converge after '--converge-limit' of 10000 tries");
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
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @(posedge rv_core.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 1U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 1 is active: @(negedge rv_core.rst_n)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 2U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 2 is active: @(posedge rv_core.u_fetch.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 3U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 3 is active: @(negedge rv_core.u_fetch.rst_n)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 4U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 4 is active: @(posedge rv_core.u_regfile.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 5U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 5 is active: @(negedge rv_core.u_regfile.rst_n)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 6U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 6 is active: @(posedge rv_core.u_csr.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 7U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 7 is active: @(negedge rv_core.u_csr.rst_n)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 8U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 8 is active: @(posedge rv_core.u_csr.u_csr_defs.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 9U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 9 is active: @(posedge rv_core.u_muldiv.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 0x0000000aU)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 10 is active: @(negedge rv_core.u_muldiv.rst_n)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 0x0000000bU)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 11 is active: @(posedge rv_core.u_muldiv.u_muldiv_defs.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 0x0000000cU)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 12 is active: @(negedge rv_core.u_muldiv.u_muldiv_defs.rst_n)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 0x0000000dU)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 13 is active: @(posedge rv_core.u_core_defs.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 0x0000000eU)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 14 is active: @(posedge rv_core.u_stage_sva.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 0x0000000fU)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 15 is active: @(posedge rv_core.u_top_if.clk)\n");
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
    vlSelf->imem_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6586433273532549467ull);
    vlSelf->imem_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1547667825581614077ull);
    vlSelf->imem_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3458333758007846314ull);
    vlSelf->imem_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8971493894096535516ull);
    vlSelf->dmem_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15097244726648949164ull);
    vlSelf->dmem_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4455132998437284282ull);
    vlSelf->dmem_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 287674497343448614ull);
    vlSelf->dmem_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1850890521623420478ull);
    vlSelf->dmem_wstrb = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 7859379407741398197ull);
    vlSelf->dmem_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14835905180964925233ull);
    vlSelf->dmem_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2017903772712052182ull);
    vlSelf->irq_ext = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18427748620615346243ull);
    vlSelf->irq_timer = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1292074699648842554ull);
    vlSelf->rv_core__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9296170111601139330ull);
    vlSelf->rv_core__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8375468262332146845ull);
    vlSelf->rv_core__DOT__imem_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6867772035154699962ull);
    vlSelf->rv_core__DOT__imem_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7180029558221945297ull);
    vlSelf->rv_core__DOT__imem_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 850582588357674619ull);
    vlSelf->rv_core__DOT__imem_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15720574082546469014ull);
    vlSelf->rv_core__DOT__dmem_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 868380905976843528ull);
    vlSelf->rv_core__DOT__dmem_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 424294166935253683ull);
    vlSelf->rv_core__DOT__dmem_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 533982219121110510ull);
    vlSelf->rv_core__DOT__dmem_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17164246155575670354ull);
    vlSelf->rv_core__DOT__dmem_wstrb = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 784121103856305099ull);
    vlSelf->rv_core__DOT__dmem_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10348286251225609075ull);
    vlSelf->rv_core__DOT__dmem_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16362864712881320322ull);
    vlSelf->rv_core__DOT__irq_ext = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16777483091988484671ull);
    vlSelf->rv_core__DOT__irq_timer = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14073614680365839087ull);
    vlSelf->rv_core__DOT__id_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4423259744915080844ull);
    vlSelf->rv_core__DOT__id_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9289927246890815523ull);
    vlSelf->rv_core__DOT__id_instr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13176691549467592483ull);
    VL_SCOPED_RAND_RESET_W(92, vlSelf->rv_core__DOT__id_dec, __VscopeHash, 1498795493965696871ull);
    vlSelf->rv_core__DOT__id_rs1_val = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11613251363899174831ull);
    vlSelf->rv_core__DOT__id_rs2_val = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11619995498937578255ull);
    vlSelf->rv_core__DOT__ex_valid_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12010774329956816969ull);
    vlSelf->rv_core__DOT__ex_pc_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1618257007705094711ull);
    vlSelf->rv_core__DOT__ex_instr_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3843147434644021169ull);
    vlSelf->rv_core__DOT__ex_rs1_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8394567222033129250ull);
    vlSelf->rv_core__DOT__ex_rs2_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6950261089535357334ull);
    VL_SCOPED_RAND_RESET_W(92, vlSelf->rv_core__DOT__ex_dec_q, __VscopeHash, 2750386235238977440ull);
    vlSelf->rv_core__DOT__mem_valid_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1517271591929659593ull);
    vlSelf->rv_core__DOT__mem_result_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 873492168001821656ull);
    vlSelf->rv_core__DOT__mem_store_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5229111811542995911ull);
    vlSelf->rv_core__DOT__mem_rd_q = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 15351298624398606204ull);
    vlSelf->rv_core__DOT__mem_wb_en_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2444858388007146169ull);
    vlSelf->rv_core__DOT__mem_is_load_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5271494522016992650ull);
    vlSelf->rv_core__DOT__mem_is_store_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10296076695578440348ull);
    vlSelf->rv_core__DOT__mem_unsigned_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14981118860354124045ull);
    vlSelf->rv_core__DOT__mem_size_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 9707729612447623009ull);
    vlSelf->rv_core__DOT__wb_valid_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12546348097586602933ull);
    vlSelf->rv_core__DOT__wb_en_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5897948728232742346ull);
    vlSelf->rv_core__DOT__wb_rd_q = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 10605622101255132113ull);
    vlSelf->rv_core__DOT__wb_data_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10603028470652233676ull);
    vlSelf->rv_core__DOT__fwd_rs1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9322128408974306213ull);
    vlSelf->rv_core__DOT__fwd_rs2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 150171085051434037ull);
    vlSelf->rv_core__DOT__load_use = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 724980405157988512ull);
    vlSelf->rv_core__DOT__mem_stall = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9843074352139659031ull);
    vlSelf->rv_core__DOT__mdu_busy = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12879795753054447139ull);
    vlSelf->rv_core__DOT__stall_front = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13282153787961124091ull);
    vlSelf->rv_core__DOT__ex_fire = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6701359158558573237ull);
    vlSelf->rv_core__DOT__redirect = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13316537596191258913ull);
    vlSelf->rv_core__DOT__redirect_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9216325020273524147ull);
    vlSelf->rv_core__DOT__alu_a = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17658938261126567509ull);
    vlSelf->rv_core__DOT__alu_b = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6392944120603366363ull);
    vlSelf->rv_core__DOT__alu_result = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13869698785722749804ull);
    vlSelf->rv_core__DOT__branch_taken = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5014084766481642536ull);
    vlSelf->rv_core__DOT__target = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12379507860426032086ull);
    vlSelf->rv_core__DOT__ex_result = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14996806799388939333ull);
    vlSelf->rv_core__DOT__ex_link = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15173560046975024150ull);
    vlSelf->rv_core__DOT__target_misaligned = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8384503070158430389ull);
    vlSelf->rv_core__DOT__flow_change = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11677070065433699565ull);
    vlSelf->rv_core__DOT__ls_misaligned = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17301821596026284933ull);
    vlSelf->rv_core__DOT__ex_offset = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 3487435137476252293ull);
    vlSelf->rv_core__DOT__exc = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5796110895160692108ull);
    vlSelf->rv_core__DOT__exc_cause = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14195316158524540435ull);
    vlSelf->rv_core__DOT__exc_val = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9837121859128696196ull);
    vlSelf->rv_core__DOT__irq_pending = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5663594201264365563ull);
    vlSelf->rv_core__DOT__irq_cause = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6666273941602326313ull);
    vlSelf->rv_core__DOT__trap = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4967102428925606744ull);
    vlSelf->rv_core__DOT__mret = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9183981662616711665ull);
    vlSelf->rv_core__DOT__csr_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10870770778292679396ull);
    vlSelf->rv_core__DOT__csr_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3635735094269948181ull);
    vlSelf->rv_core__DOT__csr_mtvec = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1291707511520529808ull);
    vlSelf->rv_core__DOT__csr_mepc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 448081021568009822ull);
    vlSelf->rv_core__DOT__csr_illegal = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4228160272365839821ull);
    vlSelf->rv_core__DOT__csr_masked_zero = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16099880797243113719ull);
    vlSelf->rv_core__DOT__mdu_start = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10589308925999979585ull);
    vlSelf->rv_core__DOT__mdu_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7317280706286337295ull);
    vlSelf->rv_core__DOT__mdu_active_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3415666343345857228ull);
    vlSelf->rv_core__DOT__mdu_result = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4077627639421355693ull);
    vlSelf->rv_core__DOT__load_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5861013989651379907ull);
    vlSelf->rv_core__DOT__mem_final = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16999304627634693361ull);
    vlSelf->rv_core__DOT__unused_bits = 0U;
    ;
    vlSelf->rv_core__DOT__u_fetch__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2771638483059127892ull);
    vlSelf->rv_core__DOT__u_fetch__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10958330119986760923ull);
    vlSelf->rv_core__DOT__u_fetch__DOT__stall = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 616844996099529685ull);
    vlSelf->rv_core__DOT__u_fetch__DOT__redirect = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8135240233765213695ull);
    vlSelf->rv_core__DOT__u_fetch__DOT__redirect_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11337960804035518158ull);
    vlSelf->rv_core__DOT__u_fetch__DOT__imem_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13970787854081832352ull);
    vlSelf->rv_core__DOT__u_fetch__DOT__imem_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 362047461615418290ull);
    vlSelf->rv_core__DOT__u_fetch__DOT__imem_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 18135663321028929523ull);
    vlSelf->rv_core__DOT__u_fetch__DOT__imem_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8817373077107440938ull);
    vlSelf->rv_core__DOT__u_fetch__DOT__id_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14755836387254291179ull);
    vlSelf->rv_core__DOT__u_fetch__DOT__id_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3432964996133677455ull);
    vlSelf->rv_core__DOT__u_fetch__DOT__id_instr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3503788634516478057ull);
    vlSelf->rv_core__DOT__u_fetch__DOT__pc_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16680462583396780350ull);
    vlSelf->rv_core__DOT__u_fetch__DOT__pend_pc_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15737900414223065067ull);
    vlSelf->rv_core__DOT__u_fetch__DOT__pend_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9151129458697901064ull);
    vlSelf->rv_core__DOT__u_fetch__DOT__started_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13420607060646294758ull);
    vlSelf->rv_core__DOT__u_fetch__DOT__id_valid_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7427800108062527161ull);
    vlSelf->rv_core__DOT__u_fetch__DOT__id_pc_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16799059793497837276ull);
    vlSelf->rv_core__DOT__u_fetch__DOT__id_instr_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10585876982841589310ull);
    vlSelf->rv_core__DOT__u_decode__DOT__instr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14573342237762636002ull);
    VL_SCOPED_RAND_RESET_W(92, vlSelf->rv_core__DOT__u_decode__DOT__dec, __VscopeHash, 18084131348817203769ull);
    vlSelf->rv_core__DOT__u_decode__DOT__opcode = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 16057027041057155594ull);
    vlSelf->rv_core__DOT__u_decode__DOT__f3 = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 17525291648572669527ull);
    vlSelf->rv_core__DOT__u_decode__DOT__f7 = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 10801178302462080952ull);
    vlSelf->rv_core__DOT__u_decode__DOT__imm_i = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16049294316905321733ull);
    vlSelf->rv_core__DOT__u_decode__DOT__imm_s = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4284676225641997922ull);
    vlSelf->rv_core__DOT__u_decode__DOT__imm_b = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 209181455873962974ull);
    vlSelf->rv_core__DOT__u_decode__DOT__imm_u = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 937108618514018911ull);
    vlSelf->rv_core__DOT__u_decode__DOT__imm_j = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8080667569430262347ull);
    vlSelf->rv_core__DOT__u_regfile__DOT____Vxrand___1 = VL_SCOPED_RAND_RESET_ASSIGN_I(32, __VscopeHash, 10867259823801006383ull);
    vlSelf->rv_core__DOT__u_regfile__DOT____Vxrand___0 = VL_SCOPED_RAND_RESET_ASSIGN_I(32, __VscopeHash, 18300596802054304413ull);
    vlSelf->rv_core__DOT__u_regfile__DOT____Vlvbound_he35c63e1__0 = 0;
    vlSelf->rv_core__DOT__u_regfile__DOT____Vlvbound_hf0d5df5e__0 = 0;
    vlSelf->rv_core__DOT__u_regfile__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6828257062659395420ull);
    vlSelf->rv_core__DOT__u_regfile__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18062416899414938893ull);
    vlSelf->rv_core__DOT__u_regfile__DOT__we = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15039711939985418908ull);
    vlSelf->rv_core__DOT__u_regfile__DOT__waddr = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 12293959887366769990ull);
    vlSelf->rv_core__DOT__u_regfile__DOT__wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12834136863734816848ull);
    vlSelf->rv_core__DOT__u_regfile__DOT__raddr1 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 6527680781297275246ull);
    vlSelf->rv_core__DOT__u_regfile__DOT__raddr2 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 16635356293994197770ull);
    vlSelf->rv_core__DOT__u_regfile__DOT__rdata1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15398622303659202698ull);
    vlSelf->rv_core__DOT__u_regfile__DOT__rdata2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8117369608265616408ull);
    for (int __Vi0 = 0; __Vi0 < 31; ++__Vi0) {
        vlSelf->rv_core__DOT__u_regfile__DOT__regs_q[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5902289559329252534ull);
    }
    vlSelf->rv_core__DOT__u_regfile__DOT__unnamedblk1__DOT__i = 0;
    vlSelf->rv_core__DOT__u_hazard__DOT__ex_rs1 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 9667421061555135747ull);
    vlSelf->rv_core__DOT__u_hazard__DOT__ex_rs2 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 1374923666937662823ull);
    vlSelf->rv_core__DOT__u_hazard__DOT__ex_rs1_val = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10218443951657945292ull);
    vlSelf->rv_core__DOT__u_hazard__DOT__ex_rs2_val = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9721957112399056169ull);
    vlSelf->rv_core__DOT__u_hazard__DOT__fwd_rs1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3248522419393696279ull);
    vlSelf->rv_core__DOT__u_hazard__DOT__fwd_rs2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4861514896822716525ull);
    vlSelf->rv_core__DOT__u_hazard__DOT__mem_wb_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14546105440527375262ull);
    vlSelf->rv_core__DOT__u_hazard__DOT__mem_rd = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 14496552100788713409ull);
    vlSelf->rv_core__DOT__u_hazard__DOT__mem_result = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4594098052106902788ull);
    vlSelf->rv_core__DOT__u_hazard__DOT__wb_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2636877843107136966ull);
    vlSelf->rv_core__DOT__u_hazard__DOT__wb_rd = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 10994132431399340506ull);
    vlSelf->rv_core__DOT__u_hazard__DOT__wb_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2437429052632929796ull);
    vlSelf->rv_core__DOT__u_hazard__DOT__id_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5157736010053464537ull);
    vlSelf->rv_core__DOT__u_hazard__DOT__id_uses_rs1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12721692322255641589ull);
    vlSelf->rv_core__DOT__u_hazard__DOT__id_uses_rs2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16869759428637814958ull);
    vlSelf->rv_core__DOT__u_hazard__DOT__id_rs1 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 3901167935057326180ull);
    vlSelf->rv_core__DOT__u_hazard__DOT__id_rs2 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 13246139053822510814ull);
    vlSelf->rv_core__DOT__u_hazard__DOT__ex_is_load = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14051247785179137656ull);
    vlSelf->rv_core__DOT__u_hazard__DOT__ex_rd = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 16217240622864340276ull);
    vlSelf->rv_core__DOT__u_hazard__DOT__load_use = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11329101737527355198ull);
    vlSelf->rv_core__DOT__u_alu__DOT__op = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 9026508362973635358ull);
    vlSelf->rv_core__DOT__u_alu__DOT__a = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14660180085291166664ull);
    vlSelf->rv_core__DOT__u_alu__DOT__b = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7346587819486453808ull);
    vlSelf->rv_core__DOT__u_alu__DOT__result = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15049340168781347140ull);
    vlSelf->rv_core__DOT__u_alu__DOT__shamt = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 13835086885737223873ull);
    vlSelf->rv_core__DOT__u_alu__DOT__lt_signed = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5839362426439215264ull);
    vlSelf->rv_core__DOT__u_alu__DOT__lt_unsigned = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5780086311075742341ull);
    vlSelf->rv_core__DOT__u_csr__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2630057301932601314ull);
    vlSelf->rv_core__DOT__u_csr__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 528523983823088501ull);
    vlSelf->rv_core__DOT__u_csr__DOT__op = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 1510634652735485071ull);
    vlSelf->rv_core__DOT__u_csr__DOT__commit = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10543281369716627226ull);
    vlSelf->rv_core__DOT__u_csr__DOT__addr = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 15872849639900178880ull);
    vlSelf->rv_core__DOT__u_csr__DOT__wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17588088263940111424ull);
    vlSelf->rv_core__DOT__u_csr__DOT__write_masked_zero = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3778711166530989046ull);
    vlSelf->rv_core__DOT__u_csr__DOT__rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10826876199860188063ull);
    vlSelf->rv_core__DOT__u_csr__DOT__illegal = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2401886035169469799ull);
    vlSelf->rv_core__DOT__u_csr__DOT__irq_ext = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5423904896787007196ull);
    vlSelf->rv_core__DOT__u_csr__DOT__irq_timer = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5555563400899719197ull);
    vlSelf->rv_core__DOT__u_csr__DOT__irq_pending = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4763675670360672251ull);
    vlSelf->rv_core__DOT__u_csr__DOT__irq_cause = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3313402631012409705ull);
    vlSelf->rv_core__DOT__u_csr__DOT__trap = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5893404746056694885ull);
    vlSelf->rv_core__DOT__u_csr__DOT__trap_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14704208450895396613ull);
    vlSelf->rv_core__DOT__u_csr__DOT__trap_cause = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13425224917799677755ull);
    vlSelf->rv_core__DOT__u_csr__DOT__trap_val = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2140441891558303490ull);
    vlSelf->rv_core__DOT__u_csr__DOT__mret = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10926111937384897309ull);
    vlSelf->rv_core__DOT__u_csr__DOT__mtvec = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10914770826155660268ull);
    vlSelf->rv_core__DOT__u_csr__DOT__mepc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11760224361747718068ull);
    vlSelf->rv_core__DOT__u_csr__DOT__retire = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14649281315744631543ull);
    vlSelf->rv_core__DOT__u_csr__DOT__mie_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 382850610947384907ull);
    vlSelf->rv_core__DOT__u_csr__DOT__mpie_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1039511289489883181ull);
    vlSelf->rv_core__DOT__u_csr__DOT__meie_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15096465111450142143ull);
    vlSelf->rv_core__DOT__u_csr__DOT__mtie_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 633644957312225862ull);
    vlSelf->rv_core__DOT__u_csr__DOT__mtvec_q = VL_SCOPED_RAND_RESET_I(30, __VscopeHash, 12274705468729795998ull);
    vlSelf->rv_core__DOT__u_csr__DOT__mepc_q = VL_SCOPED_RAND_RESET_I(30, __VscopeHash, 17145350480414995733ull);
    vlSelf->rv_core__DOT__u_csr__DOT__mscratch_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 562040282298009056ull);
    vlSelf->rv_core__DOT__u_csr__DOT__mcause_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8069683766019914754ull);
    vlSelf->rv_core__DOT__u_csr__DOT__mtval_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7190367200331924550ull);
    vlSelf->rv_core__DOT__u_csr__DOT__mcycle_q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 11849780123034480686ull);
    vlSelf->rv_core__DOT__u_csr__DOT__minstret_q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 12250973364836756842ull);
    vlSelf->rv_core__DOT__u_csr__DOT__known = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11647916051458953801ull);
    vlSelf->rv_core__DOT__u_csr__DOT__is_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15394438901091929089ull);
    vlSelf->rv_core__DOT__u_csr__DOT__ro = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 346819160223717683ull);
    vlSelf->rv_core__DOT__u_csr__DOT__wr_val = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3585720250305971014ull);
    vlSelf->rv_core__DOT__u_csr__DOT__mstatus_rd = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1038288789155288057ull);
    vlSelf->rv_core__DOT__u_csr__DOT__mie_rd = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14643980402590352479ull);
    vlSelf->rv_core__DOT__u_csr__DOT__mip_rd = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11869041763541410540ull);
    vlSelf->rv_core__DOT__u_csr__DOT__do_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16106537695868033442ull);
    vlSelf->rv_core__DOT__u_csr__DOT__unused_bits = 0U;
    ;
    vlSelf->rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6225232449836704638ull);
    vlSelf->rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1551085867863835850ull);
    vlSelf->rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__trap = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6977415037950376013ull);
    vlSelf->rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__trap_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2293250597193492588ull);
    vlSelf->rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mret = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13800370047960835198ull);
    vlSelf->rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__do_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6642569664319120956ull);
    vlSelf->rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__addr = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 7772720950132734983ull);
    vlSelf->rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__wr_val = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10846810109621331429ull);
    vlSelf->rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mie_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12639528453753751284ull);
    vlSelf->rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mpie_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8434072376925071777ull);
    vlSelf->rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mepc_q = VL_SCOPED_RAND_RESET_I(30, __VscopeHash, 10561886841218827495ull);
    vlSelf->rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_1_1 = 0;
    vlSelf->rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_2_1 = 0;
    vlSelf->rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_3_1 = 0;
    vlSelf->rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_4_1 = 0;
    vlSelf->rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_5_1 = 0;
    vlSelf->rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_6_1 = 0;
    vlSelf->rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_7_1 = 0;
    vlSelf->rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_9_1 = 0;
    vlSelf->rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_11_1 = 0;
    vlSelf->rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_12_1 = 0;
    vlSelf->rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_13_1 = 0;
    vlSelf->rv_core__DOT__u_muldiv__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2178474209076424615ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18372115091424355848ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__start = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2329049108652310464ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__op = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 2406387247831330786ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__a = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15286992265400848608ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__b = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6576647605561259058ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10495091652915555802ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__result = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15665985535157418105ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__state_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 8751630511145615749ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__count_q = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 11913871839527233344ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__acc_q = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 10992557085767290563ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__op_b_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11303915222071080915ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__op_a_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2570847091487391586ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__op_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 5808669503421431526ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__res_neg_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4046578734706763557ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__rem_neg_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18119441424037252626ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__is_div = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17595430953930333376ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__a_signed = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6481240909880821406ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__b_signed = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3998291275617137466ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__a_neg = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3381874798413388333ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__b_neg = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10910868035172296253ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__a_abs = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6552244754681616183ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__b_abs = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14738023311897510452ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__run_div = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10323655890130114236ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__trial = VL_SCOPED_RAND_RESET_Q(33, __VscopeHash, 15485035955082436737ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__shifted = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 2480169351220175038ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__mul_sum = VL_SCOPED_RAND_RESET_Q(33, __VscopeHash, 11790158893446457452ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__mul_out = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 16543466508972064212ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__quo_out = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1523391933748802949ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__rem_out = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2596211632999033680ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14000848907603174731ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4081922230482859033ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__start = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6265425151825852044ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__a = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5462194159968884889ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 519271746065916119ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__op_q = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 3077612416841602634ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__op_b_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9102798683065248840ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__result = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5726828564672311489ull);
    vlSelf->rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__a_ref_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3158030067215900742ull);
    vlSelf->rv_core__DOT__u_lsu__DOT__mem_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12176511298992347344ull);
    vlSelf->rv_core__DOT__u_lsu__DOT__is_load = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10108836962739932916ull);
    vlSelf->rv_core__DOT__u_lsu__DOT__is_store = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8593531822498841694ull);
    vlSelf->rv_core__DOT__u_lsu__DOT__size = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 5837910826175520947ull);
    vlSelf->rv_core__DOT__u_lsu__DOT__is_unsigned = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10813113243581776057ull);
    vlSelf->rv_core__DOT__u_lsu__DOT__addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8283787801580692604ull);
    vlSelf->rv_core__DOT__u_lsu__DOT__store_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9315574157414690105ull);
    vlSelf->rv_core__DOT__u_lsu__DOT__dmem_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12229804555859465047ull);
    vlSelf->rv_core__DOT__u_lsu__DOT__dmem_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1158198104411931972ull);
    vlSelf->rv_core__DOT__u_lsu__DOT__dmem_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13589226164151332320ull);
    vlSelf->rv_core__DOT__u_lsu__DOT__dmem_wstrb = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 16983615163159264242ull);
    vlSelf->rv_core__DOT__u_lsu__DOT__dmem_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14189006859122688398ull);
    vlSelf->rv_core__DOT__u_lsu__DOT__dmem_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13473855706979262102ull);
    vlSelf->rv_core__DOT__u_lsu__DOT__load_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7196577372963323362ull);
    vlSelf->rv_core__DOT__u_lsu__DOT__offset = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 533694906528178836ull);
    vlSelf->rv_core__DOT__u_lsu__DOT__half = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 7940460399878417726ull);
    vlSelf->rv_core__DOT__u_lsu__DOT__byte_val = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 4575457289421093121ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10217435034200408848ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18169045103001555645ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__dmem_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16945371452285461424ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__dmem_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8865479215431637067ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__dmem_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16527742025463550765ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__id_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6238261434280366423ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__id_instr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5735526864672646477ull);
    VL_SCOPED_RAND_RESET_W(92, vlSelf->rv_core__DOT__u_core_defs__DOT__id_dec, __VscopeHash, 3532595802481274768ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__ex_valid_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16416890106941376637ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__ex_pc_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 684878334511569548ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__ex_rs1_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13340420905586072615ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__ex_rs2_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9797360241490053532ull);
    VL_SCOPED_RAND_RESET_W(92, vlSelf->rv_core__DOT__u_core_defs__DOT__ex_dec_q, __VscopeHash, 17274212623611642021ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__mem_valid_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15676365587038914412ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__mem_result_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5418351226112794716ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__mem_rd_q = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 15564521080047020459ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__mem_wb_en_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14608903115596043041ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__mem_is_load_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6638337251537276829ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__mem_size_q = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 9693134466330049708ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__mem_unsigned_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1061956787477396112ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__wb_valid_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13256759815412679830ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__wb_en_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4890196715944604197ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__wb_rd_q = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 13197279391590530050ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__wb_data_q = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8957634051995729228ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__fwd_rs1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10164485527516213485ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__fwd_rs2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4342060801607403796ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__load_use = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11144284644639798826ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__mem_stall = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9221366622314136233ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__mdu_busy = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15232783539041256844ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__stall_front = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2022169306716540898ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__ex_fire = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13866954159893912716ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__redirect = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 877077089758127730ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__redirect_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7840015302806318262ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__alu_a = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2151018523342302962ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__alu_b = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15580332468836060798ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__alu_result = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16244821447695416203ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__branch_taken = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6864210597588509032ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__target = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8215907122148713867ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__ex_result = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3868436345016658495ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__ex_link = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7242220800916427467ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__target_misaligned = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 966665485842927626ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__flow_change = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4766059445516528914ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__ls_misaligned = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4896710569832424410ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__exc = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14334778816990551452ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__trap = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10793781357043797438ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__mret = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10251922865241007199ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__irq_pending = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17786777491465278919ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__csr_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4279129202864599259ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__csr_mtvec = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4645957431977232744ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__csr_mepc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17249033938651199234ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__mdu_start = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13388954991033494024ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__mdu_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10287513809169717785ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__mdu_active_q = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4828377825280348672ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__mdu_result = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8812351773784031867ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__load_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11082864946199517103ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__mem_final = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 18383194474481888507ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__ld_off = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 2789685141835855003ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__ld_byte = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 9853205888511334130ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__ld_half = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 10794292987597191370ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT__ld_ref = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8285254794357736138ull);
    vlSelf->rv_core__DOT__u_core_defs__DOT___Vpast_22_1 = 0;
    vlSelf->rv_core__DOT__u_core_defs__DOT___Vpast_23_1 = 0;
    vlSelf->rv_core__DOT__u_core_defs__DOT___Vpast_24_1 = 0;
    vlSelf->rv_core__DOT__u_core_defs__DOT___Vpast_25_1 = 0;
    vlSelf->rv_core__DOT__u_core_defs__DOT___Vpast_28_1 = 0;
    vlSelf->rv_core__DOT__u_core_defs__DOT___Vpast_29_1 = 0;
    vlSelf->rv_core__DOT__u_core_defs__DOT___Vpast_31_1 = 0;
    vlSelf->rv_core__DOT__u_core_defs__DOT___Vpast_32_1 = 0;
    vlSelf->rv_core__DOT__u_core_defs__DOT___Vpast_34_1 = 0;
    vlSelf->rv_core__DOT__u_core_defs__DOT___Vpast_35_1 = 0;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11876721219411208157ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9826413039160922137ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__if_valid = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__if_pc = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__if_instr = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__id_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9288491086414409796ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__id_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8027012238968734491ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__id_instr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17124121745749382603ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__id_rs1 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 5440424492817346036ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__id_rs2 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 7589665742774035145ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__id_rd = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 6833639121364079464ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__id_imm = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12349526745398921283ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__id_illegal = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18361929883783990975ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__ex_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16331039498483718036ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__ex_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 689651418425168403ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__ex_instr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4351383298926904961ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__ex_a = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1024372702382914422ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__ex_b = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6346350273608745130ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__ex_alu = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2467690856662586706ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__ex_taken = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6380233690956156975ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__ex_target = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14250276122878381762ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__mem_valid = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__mem_instr = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__mem_base = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__mem_addr = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__mem_store_data = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__mem_we = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__mem_re = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__mem_size = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__mem_rdata = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__mem_load_data = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__wb_valid = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__wb_instr = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__wb_we = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__wb_rd = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__wb_data = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__redirect_valid = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__redirect_pc = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__flush_ex = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__flush_mem = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__flush_wb = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__id_opcode = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 11379504093283145887ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__id_funct3 = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 12973674370424014676ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__id_funct7 = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 6646390346313539437ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__ex_opcode = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 7900692517250674820ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__ex_funct3 = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 6599534956595933686ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__ex_funct7 = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 16750088022088569207ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__mem_opcode = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__mem_funct3 = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__wb_opcode = 0U;
    ;
    vlSelf->rv_core__DOT__u_stage_sva__DOT__ex_b_ref = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 18242281123323046944ull);
    vlSelf->rv_core__DOT__u_stage_sva__DOT__mem_addr_isa = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3604902353348059907ull);
    vlSelf->rv_core__DOT__u_top_if__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17328411915980873310ull);
    vlSelf->rv_core__DOT__u_top_if__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8922871816723484645ull);
    vlSelf->rv_core__DOT__u_top_if__DOT__imem_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12828855991807812237ull);
    vlSelf->rv_core__DOT__u_top_if__DOT__imem_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 542303475214510538ull);
    vlSelf->rv_core__DOT__u_top_if__DOT__imem_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12452297512233780870ull);
    vlSelf->rv_core__DOT__u_top_if__DOT__dmem_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4875444579099133290ull);
    vlSelf->rv_core__DOT__u_top_if__DOT__dmem_ready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9833159116760782529ull);
    vlSelf->rv_core__DOT__u_top_if__DOT__dmem_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12676050290381082658ull);
    vlSelf->rv_core__DOT__u_top_if__DOT__dmem_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 479563181798162142ull);
    vlSelf->rv_core__DOT__u_top_if__DOT__dmem_wstrb = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 13681888291769805449ull);
    vlSelf->rv_core__DOT__u_top_if__DOT__dmem_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 273824347041088151ull);
    vlSelf->rv_core__DOT__u_top_if__DOT___Vpast_1_1 = 0;
    vlSelf->rv_core__DOT__u_top_if__DOT___Vpast_2_1 = 0;
    vlSelf->rv_core__DOT__u_top_if__DOT___Vpast_4_1 = 0;
    vlSelf->rv_core__DOT__u_top_if__DOT___Vpast_5_1 = 0;
    vlSelf->rv_core__DOT__u_top_if__DOT___Vpast_6_1 = 0;
    vlSelf->rv_core__DOT__u_top_if__DOT___Vpast_7_1 = 0;
    vlSelf->rv_core__DOT__u_top_if__DOT___Vpast_9_1 = 0;
    vlSelf->rv_core__DOT__u_top_if__DOT___Vpast_10_1 = 0;
    vlSelf->rv_core__DOT__u_top_if__DOT___Vpast_15_1 = 0;
    vlSelf->rv_core__DOT__u_top_if__DOT___Vpast_17_1 = 0;
    vlSelf->__Vfunc_imm_ref__0__Vfuncout = 0;
    vlSelf->__Vfunc_imm_ref__0__instr = 0;
    vlSelf->__Vfunc_imm_ref__0__opcode = 0;
    vlSelf->__Vfunc_imm_ref__0__i_imm = 0;
    vlSelf->__Vfunc_imm_ref__0__s_imm = 0;
    vlSelf->__Vfunc_imm_ref__0__b_imm = 0;
    vlSelf->__Vfunc_imm_ref__0__u_imm = 0;
    vlSelf->__Vfunc_imm_ref__0__j_imm = 0;
    vlSelf->__VdfgRegularize_h6e95ff9d_0_1 = 0;
    vlSelf->__VdfgRegularize_h6e95ff9d_0_2 = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mie_q = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mpie_q = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mepc_q = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__trap = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mret = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__do_write = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__addr = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__wr_val = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__trap_pc = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__done = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__op_q = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__op_b_q = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__result = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__a_ref_q = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_stall = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__dmem_valid = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__dmem_ready = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mdu_busy = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_valid_q = 0;
    VL_ZERO_RESET_W(92, vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q);
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mdu_done = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__stall_front = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__load_use = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_fire = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mdu_start = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mdu_active_q = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__trap = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__irq_pending = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__exc = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mret = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__redirect = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__flow_change = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__redirect_pc = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__csr_mtvec = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__csr_mepc = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__target = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_link = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_pc_q = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__branch_taken = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__target_misaligned = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__alu_a = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__fwd_rs1 = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__alu_b = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__fwd_rs2 = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_result = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__csr_rdata = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mdu_result = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__alu_result = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_final = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_is_load_q = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__load_data = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_result_q = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ls_misaligned = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_valid_q = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_wb_en_q = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_rd_q = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_valid_q = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_en_q = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_rd_q = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_data_q = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_rs1_q = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_rs2_q = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_valid = 0;
    VL_ZERO_RESET_W(92, vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_dec);
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ld_ref = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_instr = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_valid = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_rs1 = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_instr = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_rs2 = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_rd = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_funct3 = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_opcode = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_imm = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_funct7 = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_funct3 = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_opcode = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_b_ref = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_a = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_valid = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_alu = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_b = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_taken = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_instr = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_target = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_pc = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__mem_addr_isa = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_pc = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__imem_valid = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__imem_addr = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_valid = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_write = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_addr = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_wstrb = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_wdata = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__rst_n = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__imem_ready = 0;
    vlSelf->__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_ready = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VstlTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VicoTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggered[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__rv_core__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__rv_core__DOT__rst_n__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__rv_core__DOT__u_fetch__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__rv_core__DOT__u_fetch__DOT__rst_n__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__rv_core__DOT__u_regfile__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__rv_core__DOT__u_regfile__DOT__rst_n__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__rv_core__DOT__u_csr__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__rv_core__DOT__u_csr__DOT__rst_n__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__rv_core__DOT__u_muldiv__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__rv_core__DOT__u_muldiv__DOT__rst_n__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__rst_n__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__rv_core__DOT__u_core_defs__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__rv_core__DOT__u_stage_sva__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__rv_core__DOT__u_top_if__DOT__clk__0 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VnbaTriggered[__Vi0] = 0;
    }
}
