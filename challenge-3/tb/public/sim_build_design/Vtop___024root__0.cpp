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
    CData/*0:0*/ rv_core__DOT____Vcellinp__u_regfile__we;
    rv_core__DOT____Vcellinp__u_regfile__we = 0;
    CData/*0:0*/ rv_core__DOT____Vcellinp__u_csr__commit;
    rv_core__DOT____Vcellinp__u_csr__commit = 0;
    CData/*0:0*/ __VdfgRegularize_h6e95ff9d_0_5;
    __VdfgRegularize_h6e95ff9d_0_5 = 0;
    // Body
    if ((1U & (~ VL_ONEHOT_I((((2U == (3U & (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                             >> 3U))) 
                               << 1U) | (1U == (3U 
                                                & (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                                   >> 3U)))))))) {
        if ((0U != (((2U == (3U & (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                   >> 3U))) << 1U) 
                    | (1U == (3U & (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                    >> 3U)))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core.sv:168: Assertion failed in %m: unique case, but multiple matches found for '2'h%X'\n",4, 'M',vlSymsp->name(),"rv_core", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000)
                             , '#',2,(3U & (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                            >> 3U)));
                VL_STOP_MT("/design/rtl/rv_core.sv", 168, "");
            }
        }
    }
    vlSelfRef.rv_core__DOT__u_csr__DOT__trap_pc = vlSelfRef.rv_core__DOT__ex_pc_q;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__run_div 
        = (1U & ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_q) 
                 >> 2U));
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__mul_sum 
        = (0x00000001ffffffffULL & ((QData)((IData)(
                                                    (vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q 
                                                     >> 0x00000020U))) 
                                    + ((- (QData)((IData)(
                                                          (1U 
                                                           & (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q))))) 
                                       & (QData)((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_b_q)))));
    vlSelfRef.rv_core__DOT__imem_ready = vlSelfRef.imem_ready;
    vlSelfRef.rv_core__DOT__imem_rdata = vlSelfRef.imem_rdata;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__id_pc = vlSelfRef.rv_core__DOT__u_fetch__DOT__id_pc_q;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__shifted 
        = VL_SHIFTL_QQI(64,64,32, vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q, 1U);
    vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_valid 
        = vlSelfRef.rv_core__DOT__u_fetch__DOT__started_q;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_addr 
        = vlSelfRef.rv_core__DOT__u_fetch__DOT__pc_q;
    vlSelfRef.rv_core__DOT__ex_link = ((IData)(4U) 
                                       + vlSelfRef.rv_core__DOT__ex_pc_q);
    vlSelfRef.rv_core__DOT__clk = vlSelfRef.clk;
    vlSelfRef.rv_core__DOT__rst_n = vlSelfRef.rst_n;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__is_unsigned 
        = vlSelfRef.rv_core__DOT__mem_unsigned_q;
    if (vlSelfRef.rv_core__DOT__u_muldiv__DOT__res_neg_q) {
        vlSelfRef.rv_core__DOT__u_muldiv__DOT__mul_out 
            = (1ULL + (~ vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q));
        vlSelfRef.rv_core__DOT__u_muldiv__DOT__quo_out 
            = ((IData)(1U) + (~ (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q)));
    } else {
        vlSelfRef.rv_core__DOT__u_muldiv__DOT__mul_out 
            = vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q;
        vlSelfRef.rv_core__DOT__u_muldiv__DOT__quo_out 
            = (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q);
    }
    vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rd = 
        (0x0000001fU & (vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                        >> 0x0000000dU));
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__rem_out 
        = ((0U == vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_b_q)
            ? vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_a_q
            : ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__rem_neg_q)
                ? ((IData)(1U) + (~ (IData)((vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q 
                                             >> 0x00000020U))))
                : (IData)((vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q 
                           >> 0x00000020U))));
    vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_is_load 
        = ((IData)(vlSelfRef.rv_core__DOT__ex_valid_q) 
           & (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
              >> 1U));
    vlSelfRef.rv_core__DOT__u_lsu__DOT__store_data 
        = vlSelfRef.rv_core__DOT__mem_store_q;
    vlSelfRef.rv_core__DOT__u_regfile__DOT__waddr = vlSelfRef.rv_core__DOT__wb_rd_q;
    vlSelfRef.rv_core__DOT__u_regfile__DOT__wdata = vlSelfRef.rv_core__DOT__wb_data_q;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__id_valid 
        = vlSelfRef.rv_core__DOT__u_fetch__DOT__id_valid_q;
    vlSelfRef.rv_core__DOT__u_csr__DOT__mstatus_rd 
        = (0x00001800U | (((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__mpie_q) 
                           << 7U) | ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__mie_q) 
                                     << 3U)));
    vlSelfRef.rv_core__DOT__u_csr__DOT__mie_rd = (((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__meie_q) 
                                                   << 0x0000000bU) 
                                                  | ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__mtie_q) 
                                                     << 7U));
    vlSelfRef.rv_core__DOT__u_lsu__DOT__size = vlSelfRef.rv_core__DOT__mem_size_q;
    vlSelfRef.rv_core__DOT__u_csr__DOT__mepc = (vlSelfRef.rv_core__DOT__u_csr__DOT__mepc_q 
                                                << 2U);
    vlSelfRef.rv_core__DOT__u_csr__DOT__mtvec = (vlSelfRef.rv_core__DOT__u_csr__DOT__mtvec_q 
                                                 << 2U);
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__op = (7U 
                                                 & (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                    >> 4U));
    vlSelfRef.rv_core__DOT__dmem_rdata = vlSelfRef.dmem_rdata;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__addr = vlSelfRef.rv_core__DOT__mem_result_q;
    vlSelfRef.rv_core__DOT__csr_masked_zero = (0U == 
                                               (0x0000001fU 
                                                & (vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                                                   >> 0x00000017U)));
    vlSelfRef.rv_core__DOT__irq_timer = vlSelfRef.irq_timer;
    vlSelfRef.rv_core__DOT__irq_ext = vlSelfRef.irq_ext;
    vlSelfRef.rv_core__DOT__u_alu__DOT__op = (0x0000001fU 
                                              & (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                                 >> 5U));
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__done = (2U 
                                                   == (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__state_q));
    vlSelfRef.rv_core__DOT__dmem_ready = vlSelfRef.dmem_ready;
    vlSelfRef.__VdfgRegularize_h6e95ff9d_0_1 = ((IData)(vlSelfRef.rv_core__DOT__ex_valid_q) 
                                                & (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                   >> 7U));
    vlSelfRef.rv_core__DOT__u_lsu__DOT__is_load = vlSelfRef.rv_core__DOT__mem_is_load_q;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__mem_valid = vlSelfRef.rv_core__DOT__mem_valid_q;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__is_store = vlSelfRef.rv_core__DOT__mem_is_store_q;
    vlSelfRef.rv_core__DOT__u_csr__DOT__op = (3U & 
                                              (((vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                 << 0x0000000bU) 
                                                | (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                   >> 0x00000015U)) 
                                               & (- (IData)((IData)(vlSelfRef.rv_core__DOT__ex_valid_q)))));
    vlSelfRef.rv_core__DOT__u_csr__DOT__addr = (0x00000fffU 
                                                & (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                   >> 8U));
    vlSelfRef.rv_core__DOT__u_fetch__DOT__id_instr 
        = vlSelfRef.rv_core__DOT__u_fetch__DOT__id_instr_q;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs2 = 
        (0x0000001fU & (vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                        >> 0x00000012U));
    vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs2_val 
        = vlSelfRef.rv_core__DOT__ex_rs2_q;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs1 = 
        (0x0000001fU & (vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                        >> 0x00000017U));
    vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs1_val 
        = vlSelfRef.rv_core__DOT__ex_rs1_q;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__mem_rd = vlSelfRef.rv_core__DOT__mem_rd_q;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__mem_result 
        = vlSelfRef.rv_core__DOT__mem_result_q;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__wb_rd = vlSelfRef.rv_core__DOT__wb_rd_q;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__wb_data 
        = vlSelfRef.rv_core__DOT__wb_data_q;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__mem_wb_en 
        = ((IData)(vlSelfRef.rv_core__DOT__mem_valid_q) 
           & (IData)(vlSelfRef.rv_core__DOT__mem_wb_en_q));
    rv_core__DOT____Vcellinp__u_regfile__we = ((IData)(vlSelfRef.rv_core__DOT__wb_valid_q) 
                                               & (IData)(vlSelfRef.rv_core__DOT__wb_en_q));
    vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_ready 
        = vlSelfRef.rv_core__DOT__imem_ready;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_rdata 
        = vlSelfRef.rv_core__DOT__imem_rdata;
    vlSelfRef.rv_core__DOT__id_pc = vlSelfRef.rv_core__DOT__u_fetch__DOT__id_pc;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__trial = 
        (0x00000001ffffffffULL & ((QData)((IData)((vlSelfRef.rv_core__DOT__u_muldiv__DOT__shifted 
                                                   >> 0x00000020U))) 
                                  - (QData)((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_b_q))));
    vlSelfRef.rv_core__DOT__imem_valid = vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_valid;
    vlSelfRef.rv_core__DOT__imem_addr = vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_addr;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__clk = vlSelfRef.rv_core__DOT__clk;
    vlSelfRef.rv_core__DOT__u_regfile__DOT__clk = vlSelfRef.rv_core__DOT__clk;
    vlSelfRef.rv_core__DOT__u_csr__DOT__clk = vlSelfRef.rv_core__DOT__clk;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__clk = vlSelfRef.rv_core__DOT__clk;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__rst_n = vlSelfRef.rv_core__DOT__rst_n;
    vlSelfRef.rv_core__DOT__u_regfile__DOT__rst_n = vlSelfRef.rv_core__DOT__rst_n;
    vlSelfRef.rv_core__DOT__u_csr__DOT__rst_n = vlSelfRef.rv_core__DOT__rst_n;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__rst_n = vlSelfRef.rv_core__DOT__rst_n;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__result = 
        ((4U & (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_q))
          ? ((2U & (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_q))
              ? vlSelfRef.rv_core__DOT__u_muldiv__DOT__rem_out
              : vlSelfRef.rv_core__DOT__u_muldiv__DOT__quo_out)
          : ((2U & (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_q))
              ? (IData)((vlSelfRef.rv_core__DOT__u_muldiv__DOT__mul_out 
                         >> 0x00000020U)) : ((1U & (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_q))
                                              ? (IData)(
                                                        (vlSelfRef.rv_core__DOT__u_muldiv__DOT__mul_out 
                                                         >> 0x00000020U))
                                              : (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__mul_out))));
    vlSelfRef.rv_core__DOT__id_valid = vlSelfRef.rv_core__DOT__u_fetch__DOT__id_valid;
    if ((1U & (~ VL_ONEHOT_I((((1U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size)) 
                               << 1U) | (0U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size))))))) {
        if ((0U != (((1U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size)) 
                     << 1U) | (0U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_lsu.sv:62: Assertion failed in %m: unique case, but multiple matches found for '2'h%X'\n",4, 'M',vlSymsp->name(),"rv_core.u_lsu", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000)
                             , '#',2,(IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size));
                VL_STOP_MT("/design/rtl/rv_lsu.sv", 62, "");
            }
        }
    }
    vlSelfRef.rv_core__DOT__csr_mepc = vlSelfRef.rv_core__DOT__u_csr__DOT__mepc;
    vlSelfRef.rv_core__DOT__csr_mtvec = vlSelfRef.rv_core__DOT__u_csr__DOT__mtvec;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__is_div = 
        (1U & ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op) 
               >> 2U));
    vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_rdata 
        = vlSelfRef.rv_core__DOT__dmem_rdata;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__offset = (3U 
                                                  & vlSelfRef.rv_core__DOT__u_lsu__DOT__addr);
    vlSelfRef.rv_core__DOT__u_csr__DOT__write_masked_zero 
        = vlSelfRef.rv_core__DOT__csr_masked_zero;
    vlSelfRef.rv_core__DOT__u_csr__DOT__irq_timer = vlSelfRef.rv_core__DOT__irq_timer;
    vlSelfRef.rv_core__DOT__u_csr__DOT__irq_ext = vlSelfRef.rv_core__DOT__irq_ext;
    vlSelfRef.rv_core__DOT__mdu_done = vlSelfRef.rv_core__DOT__u_muldiv__DOT__done;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_write 
        = ((IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__mem_valid) 
           & (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__is_store));
    vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_valid 
        = ((IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__mem_valid) 
           & ((IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__is_load) 
              | (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__is_store)));
    if ((1U & (~ VL_ONEHOT_I((((3U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op)) 
                               << 2U) | (((2U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op)) 
                                          << 1U) | 
                                         (1U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op)))))))) {
        if ((0U != (((3U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op)) 
                     << 2U) | (((2U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op)) 
                                << 1U) | (1U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op)))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_csr.sv:83: Assertion failed in %m: unique case, but multiple matches found for '2'h%X'\n",4, 'M',vlSymsp->name(),"rv_core.u_csr", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000)
                             , '#',2,(IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op));
                VL_STOP_MT("/design/rtl/rv_csr.sv", 83, "");
            }
        }
    }
    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 1U;
    vlSelfRef.rv_core__DOT__u_csr__DOT__ro = (3U == 
                                              (3U & 
                                               ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                                >> 0x0000000aU)));
    vlSelfRef.rv_core__DOT__id_instr = vlSelfRef.rv_core__DOT__u_fetch__DOT__id_instr;
    vlSelfRef.rv_core__DOT__u_regfile__DOT__we = rv_core__DOT____Vcellinp__u_regfile__we;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__wb_en = rv_core__DOT____Vcellinp__u_regfile__we;
    vlSelfRef.imem_valid = vlSelfRef.rv_core__DOT__imem_valid;
    vlSelfRef.imem_addr = vlSelfRef.rv_core__DOT__imem_addr;
    vlSelfRef.rv_core__DOT__mdu_result = vlSelfRef.rv_core__DOT__u_muldiv__DOT__result;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__id_valid 
        = vlSelfRef.rv_core__DOT__id_valid;
    if (vlSelfRef.rv_core__DOT__u_muldiv__DOT__is_div) {
        vlSelfRef.rv_core__DOT__u_muldiv__DOT__a_signed 
            = (1U & (~ (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op)));
        vlSelfRef.rv_core__DOT__u_muldiv__DOT__b_signed 
            = (1U & (~ (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op)));
    } else {
        vlSelfRef.rv_core__DOT__u_muldiv__DOT__a_signed 
            = (1U & (3U != (3U & (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op))));
        vlSelfRef.rv_core__DOT__u_muldiv__DOT__b_signed 
            = (1U & ((1U == (3U & (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op))) 
                     | (0U == (3U & (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op)))));
    }
    if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__offset))) {
        vlSelfRef.rv_core__DOT__u_lsu__DOT__half = 
            (0x0000ffffU & (vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_rdata 
                            >> 0x00000010U));
        vlSelfRef.rv_core__DOT__u_lsu__DOT__byte_val 
            = (0x000000ffU & ((1U & (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__offset))
                               ? (vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_rdata 
                                  >> 0x00000018U) : 
                              (vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_rdata 
                               >> 0x00000010U)));
    } else {
        vlSelfRef.rv_core__DOT__u_lsu__DOT__half = 
            (0x0000ffffU & vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_rdata);
        vlSelfRef.rv_core__DOT__u_lsu__DOT__byte_val 
            = (0x000000ffU & ((1U & (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__offset))
                               ? (vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_rdata 
                                  >> 8U) : vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_rdata));
    }
    vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_wstrb = 0U;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_wdata = 0U;
    if (((IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__mem_valid) 
         & (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__is_store))) {
        if ((0U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size))) {
            vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_wstrb 
                = (0x0000000fU & ((IData)(1U) << (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__offset)));
            vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_wdata 
                = ((vlSelfRef.rv_core__DOT__u_lsu__DOT__store_data 
                    << 0x00000018U) | ((0x00ff0000U 
                                        & (vlSelfRef.rv_core__DOT__u_lsu__DOT__store_data 
                                           << 0x00000010U)) 
                                       | ((0x0000ff00U 
                                           & (vlSelfRef.rv_core__DOT__u_lsu__DOT__store_data 
                                              << 8U)) 
                                          | (0x000000ffU 
                                             & vlSelfRef.rv_core__DOT__u_lsu__DOT__store_data))));
        } else if ((1U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size))) {
            vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_wstrb 
                = ((2U & (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__offset))
                    ? 0x0cU : 3U);
            vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_wdata 
                = ((vlSelfRef.rv_core__DOT__u_lsu__DOT__store_data 
                    << 0x00000010U) | (0x0000ffffU 
                                       & vlSelfRef.rv_core__DOT__u_lsu__DOT__store_data));
        } else {
            vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_wstrb = 0x0fU;
            vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_wdata 
                = vlSelfRef.rv_core__DOT__u_lsu__DOT__store_data;
        }
        if ((1U & (~ VL_ONEHOT_I((((1U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size)) 
                                   << 1U) | (0U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size))))))) {
            if ((0U != (((1U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size)) 
                         << 1U) | (0U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size))))) {
                if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                    VL_WRITEF_NX("[%0t] %%Error: rv_lsu.sv:33: Assertion failed in %m: unique case, but multiple matches found for '2'h%X'\n",4, 'M',vlSymsp->name(),"rv_core.u_lsu", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1000)
                                 , '#',2,(IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size));
                    VL_STOP_MT("/design/rtl/rv_lsu.sv", 33, "");
                }
            }
        }
    }
    vlSelfRef.rv_core__DOT__u_csr__DOT__is_write = 
        (1U & ((~ (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__write_masked_zero)) 
               | (1U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op))));
    vlSelfRef.rv_core__DOT__u_csr__DOT__mip_rd = (((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__irq_ext) 
                                                   << 0x0000000bU) 
                                                  | ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__irq_timer) 
                                                     << 7U));
    __VdfgRegularize_h6e95ff9d_0_5 = ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__irq_ext) 
                                      & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__meie_q));
    vlSelfRef.rv_core__DOT__mdu_busy = ((~ (IData)(vlSelfRef.rv_core__DOT__mdu_done)) 
                                        & (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_1));
    vlSelfRef.rv_core__DOT__dmem_write = vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_write;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_addr = 
        (0xfffffffcU & (vlSelfRef.rv_core__DOT__u_lsu__DOT__addr 
                        & (- (IData)((IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_valid)))));
    vlSelfRef.rv_core__DOT__dmem_valid = vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_valid;
    vlSelfRef.rv_core__DOT__u_decode__DOT__instr = vlSelfRef.rv_core__DOT__id_instr;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__fwd_rs2 
        = (((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__mem_wb_en) 
            & ((0U != (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs2)) 
               & ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs2) 
                  == (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__mem_rd))))
            ? vlSelfRef.rv_core__DOT__u_hazard__DOT__mem_result
            : (((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__wb_en) 
                & ((0U != (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs2)) 
                   & ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs2) 
                      == (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__wb_rd))))
                ? vlSelfRef.rv_core__DOT__u_hazard__DOT__wb_data
                : vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs2_val));
    vlSelfRef.rv_core__DOT__u_hazard__DOT__fwd_rs1 
        = (((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__mem_wb_en) 
            & ((0U != (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs1)) 
               & ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs1) 
                  == (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__mem_rd))))
            ? vlSelfRef.rv_core__DOT__u_hazard__DOT__mem_result
            : (((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__wb_en) 
                & ((0U != (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs1)) 
                   & ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs1) 
                      == (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__wb_rd))))
                ? vlSelfRef.rv_core__DOT__u_hazard__DOT__wb_data
                : vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs1_val));
    vlSelfRef.rv_core__DOT__u_lsu__DOT__load_data = 
        ((0U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size))
          ? (((- (IData)(((~ (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__is_unsigned)) 
                          & ((IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__byte_val) 
                             >> 7U)))) << 8U) | (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__byte_val))
          : ((1U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size))
              ? (((- (IData)(((~ (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__is_unsigned)) 
                              & ((IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__half) 
                                 >> 0x0000000fU)))) 
                  << 0x00000010U) | (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__half))
              : vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_rdata));
    vlSelfRef.rv_core__DOT__dmem_wstrb = vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_wstrb;
    vlSelfRef.rv_core__DOT__dmem_wdata = vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_wdata;
    if ((0x00000800U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
        if ((0x00000400U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
            if ((0x00000200U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                if ((0x00000100U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    if ((0x00000080U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    } else if ((0x00000040U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    } else if ((0x00000020U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    } else if ((0x00000010U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        if ((8U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                            vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                        } else if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                            if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                                vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                            } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                                vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                            }
                        } else if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                             >> 1U)))) {
                            if ((1U & (~ (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)))) {
                                vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                            }
                        }
                    } else {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    }
                } else {
                    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                }
            } else {
                vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
            }
        } else if ((0x00000200U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
            if ((0x00000100U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                if ((0x00000080U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    if ((0x00000040U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    } else if ((0x00000020U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    } else if ((0x00000010U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    } else if ((8U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    } else if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    } else if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                            vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                        }
                    } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    }
                } else if ((0x00000040U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                } else if ((0x00000020U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                } else if ((0x00000010U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                } else if ((8U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                } else if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                } else if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    }
                } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                }
            } else {
                vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
            }
        } else {
            vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
        }
    } else if ((0x00000400U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
    } else if ((0x00000200U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
        if ((0x00000100U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
            if ((0x00000080U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
            } else if ((0x00000040U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                if ((0x00000020U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                } else if ((0x00000010U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                } else if ((8U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                } else if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    }
                }
            } else if ((0x00000020U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
            } else if ((0x00000010U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
            } else if ((8U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
            } else if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                }
            } else if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
            }
        } else {
            vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
        }
    } else {
        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
    }
    vlSelfRef.rv_core__DOT__u_csr__DOT__illegal = (
                                                   (0U 
                                                    != (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op)) 
                                                   & ((~ (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__known)) 
                                                      | ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__ro) 
                                                         & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__is_write))));
    vlSelfRef.rv_core__DOT__u_csr__DOT__rdata = 0U;
    if ((0x00000800U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
        if ((0x00000400U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
            if ((0x00000200U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                if ((0x00000100U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                  >> 7U)))) {
                        if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                      >> 6U)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                          >> 5U)))) {
                                if ((0x00000010U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                                    if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                                  >> 3U)))) {
                                        if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                                            if ((1U 
                                                 & (~ 
                                                    ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                                     >> 1U)))) {
                                                if (
                                                    (1U 
                                                     & (~ (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)))) {
                                                    vlSelfRef.rv_core__DOT__u_csr__DOT__rdata = 0U;
                                                }
                                            }
                                        } else if (
                                                   (2U 
                                                    & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                                            vlSelfRef.rv_core__DOT__u_csr__DOT__rdata = 0U;
                                        } else if (
                                                   (1U 
                                                    & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                                            vlSelfRef.rv_core__DOT__u_csr__DOT__rdata = 0U;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } else if ((0x00000200U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
            if ((0x00000100U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                if ((0x00000080U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                  >> 6U)))) {
                        if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                      >> 5U)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                          >> 4U)))) {
                                if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                              >> 3U)))) {
                                    if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                                  >> 2U)))) {
                                        if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                                            if ((1U 
                                                 & (~ (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)))) {
                                                vlSelfRef.rv_core__DOT__u_csr__DOT__rdata 
                                                    = (IData)(
                                                              (vlSelfRef.rv_core__DOT__u_csr__DOT__minstret_q 
                                                               >> 0x20U));
                                            }
                                        } else if (
                                                   (1U 
                                                    & (~ (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)))) {
                                            vlSelfRef.rv_core__DOT__u_csr__DOT__rdata 
                                                = (IData)(
                                                          (vlSelfRef.rv_core__DOT__u_csr__DOT__mcycle_q 
                                                           >> 0x20U));
                                        }
                                    }
                                }
                            }
                        }
                    }
                } else if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                     >> 6U)))) {
                    if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                  >> 5U)))) {
                        if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                      >> 4U)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                          >> 3U)))) {
                                if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                              >> 2U)))) {
                                    if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                                        if ((1U & (~ (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)))) {
                                            vlSelfRef.rv_core__DOT__u_csr__DOT__rdata 
                                                = (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__minstret_q);
                                        }
                                    } else if ((1U 
                                                & (~ (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)))) {
                                        vlSelfRef.rv_core__DOT__u_csr__DOT__rdata 
                                            = (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__mcycle_q);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } else if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                         >> 0x0000000aU)))) {
        if ((0x00000200U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
            if ((0x00000100U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                              >> 7U)))) {
                    if ((0x00000040U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                      >> 5U)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                          >> 4U)))) {
                                if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                              >> 3U)))) {
                                    if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                                        if ((1U & (~ 
                                                   ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                                    >> 1U)))) {
                                            if ((1U 
                                                 & (~ (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)))) {
                                                vlSelfRef.rv_core__DOT__u_csr__DOT__rdata 
                                                    = vlSelfRef.rv_core__DOT__u_csr__DOT__mip_rd;
                                            }
                                        }
                                    } else {
                                        vlSelfRef.rv_core__DOT__u_csr__DOT__rdata 
                                            = ((2U 
                                                & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))
                                                ? (
                                                   (1U 
                                                    & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))
                                                    ? vlSelfRef.rv_core__DOT__u_csr__DOT__mtval_q
                                                    : vlSelfRef.rv_core__DOT__u_csr__DOT__mcause_q)
                                                : (
                                                   (1U 
                                                    & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))
                                                    ? vlSelfRef.rv_core__DOT__u_csr__DOT__mepc
                                                    : vlSelfRef.rv_core__DOT__u_csr__DOT__mscratch_q));
                                    }
                                }
                            }
                        }
                    } else if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                         >> 5U)))) {
                        if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                      >> 4U)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                          >> 3U)))) {
                                if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                                    if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                                  >> 1U)))) {
                                        vlSelfRef.rv_core__DOT__u_csr__DOT__rdata 
                                            = ((1U 
                                                & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))
                                                ? vlSelfRef.rv_core__DOT__u_csr__DOT__mtvec
                                                : vlSelfRef.rv_core__DOT__u_csr__DOT__mie_rd);
                                    }
                                } else if ((1U & (~ 
                                                  ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                                   >> 1U)))) {
                                    vlSelfRef.rv_core__DOT__u_csr__DOT__rdata 
                                        = ((1U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))
                                            ? 0x40001100U
                                            : vlSelfRef.rv_core__DOT__u_csr__DOT__mstatus_rd);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.rv_core__DOT__u_csr__DOT__irq_cause = 
        ((IData)(__VdfgRegularize_h6e95ff9d_0_5) ? 0x8000000bU
          : 0x80000007U);
    vlSelfRef.rv_core__DOT__u_csr__DOT__irq_pending 
        = ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__mie_q) 
           & (((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__irq_timer) 
               & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__mtie_q)) 
              | (IData)(__VdfgRegularize_h6e95ff9d_0_5)));
    vlSelfRef.dmem_write = vlSelfRef.rv_core__DOT__dmem_write;
    vlSelfRef.rv_core__DOT__dmem_addr = vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_addr;
    vlSelfRef.dmem_valid = vlSelfRef.rv_core__DOT__dmem_valid;
    vlSelfRef.rv_core__DOT__mem_stall = ((~ (IData)(vlSelfRef.rv_core__DOT__dmem_ready)) 
                                         & (IData)(vlSelfRef.rv_core__DOT__dmem_valid));
    vlSelfRef.rv_core__DOT__u_decode__DOT__opcode = 
        (0x0000001fU & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                        >> 2U));
    vlSelfRef.rv_core__DOT__u_decode__DOT__f3 = (7U 
                                                 & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                    >> 0x0000000cU));
    vlSelfRef.rv_core__DOT__u_decode__DOT__f7 = (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                 >> 0x00000019U);
    vlSelfRef.rv_core__DOT__u_decode__DOT__imm_i = 
        (((- (IData)((vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                      >> 0x0000001fU))) << 0x0000000cU) 
         | (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
            >> 0x00000014U));
    vlSelfRef.rv_core__DOT__u_decode__DOT__imm_s = 
        (((- (IData)((vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                      >> 0x0000001fU))) << 0x0000000cU) 
         | ((0x00000fe0U & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                            >> 0x00000014U)) | (0x0000001fU 
                                                & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                   >> 7U))));
    vlSelfRef.rv_core__DOT__u_decode__DOT__imm_b = 
        (((- (IData)((vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                      >> 0x0000001fU))) << 0x0000000dU) 
         | ((((2U & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                     >> 0x0000001eU)) | (1U & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                               >> 7U))) 
             << 0x0000000bU) | ((0x000007e0U & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                >> 0x00000014U)) 
                                | (0x0000001eU & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                  >> 7U)))));
    vlSelfRef.rv_core__DOT__u_decode__DOT__imm_u = 
        (0xfffff000U & vlSelfRef.rv_core__DOT__u_decode__DOT__instr);
    vlSelfRef.rv_core__DOT__u_decode__DOT__imm_j = 
        ((((0x00000ffeU & ((- (IData)((vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                       >> 0x0000001fU))) 
                           << 1U)) | (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                      >> 0x0000001fU)) 
          << 0x00000014U) | ((((0x000001feU & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                               >> 0x0000000bU)) 
                               | (1U & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                        >> 0x00000014U))) 
                              << 0x0000000bU) | (0x000007feU 
                                                 & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                    >> 0x00000014U))));
    vlSelfRef.rv_core__DOT__fwd_rs2 = vlSelfRef.rv_core__DOT__u_hazard__DOT__fwd_rs2;
    vlSelfRef.rv_core__DOT__fwd_rs1 = vlSelfRef.rv_core__DOT__u_hazard__DOT__fwd_rs1;
    vlSelfRef.rv_core__DOT__load_data = vlSelfRef.rv_core__DOT__u_lsu__DOT__load_data;
    vlSelfRef.dmem_wstrb = vlSelfRef.rv_core__DOT__dmem_wstrb;
    vlSelfRef.dmem_wdata = vlSelfRef.rv_core__DOT__dmem_wdata;
    vlSelfRef.rv_core__DOT__csr_illegal = vlSelfRef.rv_core__DOT__u_csr__DOT__illegal;
    vlSelfRef.rv_core__DOT__csr_rdata = vlSelfRef.rv_core__DOT__u_csr__DOT__rdata;
    vlSelfRef.rv_core__DOT__irq_cause = vlSelfRef.rv_core__DOT__u_csr__DOT__irq_cause;
    vlSelfRef.rv_core__DOT__irq_pending = vlSelfRef.rv_core__DOT__u_csr__DOT__irq_pending;
    vlSelfRef.dmem_addr = vlSelfRef.rv_core__DOT__dmem_addr;
    vlSelfRef.rv_core__DOT__mdu_start = ((~ ((IData)(vlSelfRef.rv_core__DOT__mem_stall) 
                                             | (IData)(vlSelfRef.rv_core__DOT__mdu_active_q))) 
                                         & (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_1));
    vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2 = ((IData)(vlSelfRef.rv_core__DOT__mem_stall) 
                                                | (IData)(vlSelfRef.rv_core__DOT__mdu_busy));
    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] = 0U;
    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] = 0U;
    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] = 0U;
    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
        = ((0x00001fffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]) 
           | (0x0fffffffU & (((0x00007c00U & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                              >> 5U)) 
                              | ((0x000003e0U & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                 >> 0x0000000fU)) 
                                 | (0x0000001fU & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                   >> 7U)))) 
                             << 0x0000000dU)));
    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
        = (0xfffffc07U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]);
    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
        = ((0x1fffffffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
           | (((6U & ((IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3) 
                      << 1U)) | (1U & ((IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3) 
                                       >> 2U))) << 0x0000001dU));
    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
        = ((0xf1ffffffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
           | ((IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3) 
              << 0x00000019U));
    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
        = ((0xff8000ffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
           | (((0x00001000U & ((IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3) 
                               << 0x0000000aU)) | (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                   >> 0x14U)) 
              << 8U));
    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
        = ((0xffffff8fU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
           | ((IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3) 
              << 4U));
    if ((3U != (3U & vlSelfRef.rv_core__DOT__u_decode__DOT__instr))) {
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
    } else if ((0x00000010U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
        if ((8U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
            if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
                if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                        = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                        = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                } else if ((0U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                    if ((0U == (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                >> 7U))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (8U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((0x00002000U == (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                >> 7U))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (4U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((0x00604000U == (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                >> 7U))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (2U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((0x0020a000U != (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                >> 7U))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    }
                } else if ((4U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                        = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                } else {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                        = (0x00000400U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
                    if ((1U & (~ VL_ONEHOT_I((((2U 
                                                == 
                                                (3U 
                                                 & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) 
                                               << 1U) 
                                              | (1U 
                                                 == 
                                                 (3U 
                                                  & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)))))))) {
                        if ((0U != (((2U == (3U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) 
                                     << 1U) | (1U == 
                                               (3U 
                                                & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)))))) {
                            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                                VL_WRITEF_NX("[%0t] %%Error: rv_decode.sv:182: Assertion failed in %m: unique case, but multiple matches found for '2'h%X'\n",4, 'M',vlSymsp->name(),"rv_core.u_decode", 'T',-9
                                             , '#',64,VL_TIME_UNITED_Q(1000)
                                             , '#',2,
                                             (3U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)));
                                VL_STOP_MT("/design/rtl/rv_decode.sv", 182, "");
                            }
                        }
                    }
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                        = ((0x0fffefffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]) 
                           | (0x00001000U & ((~ ((IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3) 
                                                 >> 2U)) 
                                             << 0x0000000cU)));
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                        = ((0xff9fffffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
                           | (((1U == (3U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)))
                                ? 1U : ((2U == (3U 
                                                & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)))
                                         ? 2U : 3U)) 
                              << 0x00000015U));
                }
            } else if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
                if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                        = ((0x000003ffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                           | (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_j 
                              << 0x0000000aU));
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                        = ((0x0ffffc00U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]) 
                           | (0x0fffffffU & (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_j 
                                             >> 0x00000016U)));
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                        = (0x01000000U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                        = (0x00000400U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
                } else {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                        = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                }
            } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                    = ((0x000003ffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                       | (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_i 
                          << 0x0000000aU));
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                    = ((0x0ffffc00U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]) 
                       | (0x0fffffffU & (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_i 
                                         >> 0x00000016U)));
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                    = (0x00800000U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                    = (0x00001000U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                    = (0x00000400U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                    = ((0xfffffffeU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
                       | (0U != (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)));
            } else {
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                    = ((0x000003ffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                       | (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_b 
                          << 0x0000000aU));
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                    = ((0x0ffffc00U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]) 
                       | (0x0fffffffU & (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_b 
                                         >> 0x00000016U)));
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                    = (0x10000000U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                    = (0x00001800U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                    = ((0xfffffffeU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
                       | ((2U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)) 
                          | (3U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))));
            }
        } else {
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
        }
    } else if ((8U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
        if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
            if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                    = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
            } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                    = ((0x000003ffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                       | (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_u 
                          << 0x0000000aU));
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                    = ((0x0ffffc00U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]) 
                       | (0x0fffffffU & (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_u 
                                         >> 0x00000016U)));
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                    = (0x00000014U | (0xffffffe3U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]));
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                    = (0x00000400U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
            } else {
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                    = (0x00001c00U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
                if ((0x00000040U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                        = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                } else if ((0x00000020U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                    if ((0x00000010U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((8U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((0U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                            = (0x00000020U | (0xfffffc1fU 
                                              & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]));
                    } else if ((5U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                            = (0x000000e0U | (0xfffffc1fU 
                                              & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]));
                    } else {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    }
                } else if ((0x00000010U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                    if ((8U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else {
                        if ((2U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                                = (0x00000140U | (0xfffffc1fU 
                                                  & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]));
                        } else if ((4U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                                = (0x00000160U | (0xfffffc1fU 
                                                  & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]));
                        } else if ((6U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                                = (0x00000180U | (0xfffffc1fU 
                                                  & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]));
                        } else {
                            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                                = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                        }
                        if ((1U & (~ VL_ONEHOT_I(((
                                                   (6U 
                                                    == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)) 
                                                   << 2U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)) 
                                                      << 1U) 
                                                     | (2U 
                                                        == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)))))))) {
                            if ((0U != (((6U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)) 
                                         << 2U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)) 
                                                    << 1U) 
                                                   | (2U 
                                                      == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)))))) {
                                if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                                    VL_WRITEF_NX("[%0t] %%Error: rv_decode.sv:141: Assertion failed in %m: unique case, but multiple matches found for '3'h%X'\n",4, 'M',vlSymsp->name(),"rv_core.u_decode", 'T',-9
                                                 , '#',64,VL_TIME_UNITED_Q(1000)
                                                 , '#',3,(IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3));
                                    VL_STOP_MT("/design/rtl/rv_decode.sv", 141, "");
                                }
                            }
                        }
                    }
                } else if ((8U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                        = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                } else if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                    if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                        if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                                = ((0xfffffc1fU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                                   | (((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                                        ? ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                                            ? 0x10U
                                            : 0x0fU)
                                        : ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                                            ? 0x0eU
                                            : 0x0dU)) 
                                      << 5U));
                        } else {
                            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                                = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                        }
                    } else {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    }
                } else if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                        = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                        = (0x00000080U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                } else {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                        = ((0xfffffc1fU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                           | (((4U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                                ? ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                                    ? ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                                        ? 9U : 8U) : 
                                   ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                                     ? 6U : 5U)) : 
                               ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                                 ? ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                                     ? 4U : 3U) : (
                                                   (1U 
                                                    & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                                                    ? 2U
                                                    : 0U))) 
                              << 5U));
                }
            }
        } else if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
        } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
        } else {
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                = ((0x000003ffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                   | (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_s 
                      << 0x0000000aU));
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                = ((0x0ffffc00U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]) 
                   | (0x0fffffffU & (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_s 
                                     >> 0x00000016U)));
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]);
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                = (4U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]);
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                = (0x00001800U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                = ((0xfffffffeU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
                   | (2U < (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)));
        }
    } else if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
        if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
        } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                = ((0x000003ffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                   | (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_u 
                      << 0x0000000aU));
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                = ((0x0ffffc00U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]) 
                   | (0x0fffffffU & (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_u 
                                     >> 0x00000016U)));
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                = (0x0000000cU | (0xffffffe3U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]));
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                = (0x00000400U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
        } else {
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                = ((0x000003ffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                   | (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_i 
                      << 0x0000000aU));
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                = ((0x0ffffc00U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]) 
                   | (0x0fffffffU & (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_i 
                                     >> 0x00000016U)));
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                = (4U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]);
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                = (0x00001000U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                = (0x00000400U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
            if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                        = ((0xfffffc1fU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                           | (((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                                ? 9U : 8U) << 5U));
                } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                        = ((0xfffffc1fU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                           | (((0x00000020U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))
                                ? 7U : 6U) << 5U));
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                        = ((0xfffffffeU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
                           | ((0U != (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7)) 
                              & (0x20U != (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))));
                } else {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                        = (0x000000a0U | (0xfffffc1fU 
                                          & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]));
                }
            } else if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                    = ((0xfffffc1fU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                       | (((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                            ? 4U : 3U) << 5U));
            } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                    = (0x00000040U | (0xfffffc1fU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]));
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                    = ((0xfffffffeU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
                       | (0U != (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7)));
            } else {
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                    = (0xfffffc1fU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]);
            }
        }
    } else if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
            = ((0xfffffffeU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
               | ((1U & (~ (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) 
                  || ((0U != (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)) 
                      & (1U != (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)))));
    } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
    } else {
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
            = ((0x000003ffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
               | (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_i 
                  << 0x0000000aU));
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
            = ((0x0ffffc00U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]) 
               | (0x0fffffffU & (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_i 
                                 >> 0x00000016U)));
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
            = (6U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]);
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
            = (0x00001000U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
            = (0x00000400U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
            = ((0xfffffffeU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
               | (((3U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)) 
                   | (6U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) 
                  | (7U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))));
    }
    if ((1U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U])) {
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
            = (0x0ffffbffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
            = (0xfffffffcU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]);
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
            = (0xefffffffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
            = (0xfe7fffffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
            = (0xffffff7fU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
            = (0xff9fffffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
    }
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__b = vlSelfRef.rv_core__DOT__fwd_rs2;
    vlSelfRef.rv_core__DOT__alu_b = ((4U & vlSelfRef.rv_core__DOT__ex_dec_q[1U])
                                      ? ((vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                                          << 0x00000016U) 
                                         | (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                            >> 0x0000000aU))
                                      : vlSelfRef.rv_core__DOT__fwd_rs2);
    vlSelfRef.rv_core__DOT__csr_wdata = ((0x00100000U 
                                          & vlSelfRef.rv_core__DOT__ex_dec_q[0U])
                                          ? (0x0000001fU 
                                             & (vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                                                >> 0x00000017U))
                                          : vlSelfRef.rv_core__DOT__fwd_rs1);
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__a = vlSelfRef.rv_core__DOT__fwd_rs1;
    vlSelfRef.rv_core__DOT__target = ((0x00800000U 
                                       & vlSelfRef.rv_core__DOT__ex_dec_q[0U])
                                       ? (0xfffffffeU 
                                          & (vlSelfRef.rv_core__DOT__fwd_rs1 
                                             + ((vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                                                 << 0x00000016U) 
                                                | (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                                   >> 0x0000000aU))))
                                       : (vlSelfRef.rv_core__DOT__ex_pc_q 
                                          + ((vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                                              << 0x00000016U) 
                                             | (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                                >> 0x0000000aU))));
    vlSelfRef.rv_core__DOT__alu_a = ((1U == (3U & (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                                   >> 3U)))
                                      ? vlSelfRef.rv_core__DOT__ex_pc_q
                                      : (vlSelfRef.rv_core__DOT__fwd_rs1 
                                         & (- (IData)(
                                                      (2U 
                                                       != 
                                                       (3U 
                                                        & (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                                           >> 3U)))))));
    vlSelfRef.rv_core__DOT__mem_final = ((IData)(vlSelfRef.rv_core__DOT__mem_is_load_q)
                                          ? vlSelfRef.rv_core__DOT__load_data
                                          : vlSelfRef.rv_core__DOT__mem_result_q);
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__start = vlSelfRef.rv_core__DOT__mdu_start;
    vlSelfRef.rv_core__DOT__ex_fire = ((~ (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2)) 
                                       & (IData)(vlSelfRef.rv_core__DOT__ex_valid_q));
    vlSelfRef.rv_core__DOT__id_dec[0U] = vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U];
    vlSelfRef.rv_core__DOT__id_dec[1U] = vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U];
    vlSelfRef.rv_core__DOT__id_dec[2U] = vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U];
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__b_neg = 
        ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__b_signed) 
         & (vlSelfRef.rv_core__DOT__u_muldiv__DOT__b 
            >> 0x0000001fU));
    vlSelfRef.rv_core__DOT__u_alu__DOT__b = vlSelfRef.rv_core__DOT__alu_b;
    vlSelfRef.rv_core__DOT__u_csr__DOT__wdata = vlSelfRef.rv_core__DOT__csr_wdata;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__a_neg = 
        ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__a_signed) 
         & (vlSelfRef.rv_core__DOT__u_muldiv__DOT__a 
            >> 0x0000001fU));
    vlSelfRef.rv_core__DOT__u_alu__DOT__a = vlSelfRef.rv_core__DOT__alu_a;
    vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr1 
        = (0x0000001fU & (vlSelfRef.rv_core__DOT__id_dec[2U] 
                          >> 0x00000017U));
    vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr2 
        = (0x0000001fU & (vlSelfRef.rv_core__DOT__id_dec[2U] 
                          >> 0x00000012U));
    vlSelfRef.rv_core__DOT__u_hazard__DOT__id_uses_rs1 
        = (1U & (vlSelfRef.rv_core__DOT__id_dec[2U] 
                 >> 0x0000000cU));
    vlSelfRef.rv_core__DOT__u_hazard__DOT__id_uses_rs2 
        = (1U & (vlSelfRef.rv_core__DOT__id_dec[2U] 
                 >> 0x0000000bU));
    vlSelfRef.rv_core__DOT__u_hazard__DOT__id_rs1 = 
        (0x0000001fU & (vlSelfRef.rv_core__DOT__id_dec[2U] 
                        >> 0x00000017U));
    vlSelfRef.rv_core__DOT__u_hazard__DOT__id_rs2 = 
        (0x0000001fU & (vlSelfRef.rv_core__DOT__id_dec[2U] 
                        >> 0x00000012U));
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__b_abs = 
        ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__b_neg)
          ? ((IData)(1U) + (~ vlSelfRef.rv_core__DOT__u_muldiv__DOT__b))
          : vlSelfRef.rv_core__DOT__u_muldiv__DOT__b);
    vlSelfRef.rv_core__DOT__u_alu__DOT__shamt = (0x0000001fU 
                                                 & vlSelfRef.rv_core__DOT__u_alu__DOT__b);
    vlSelfRef.rv_core__DOT__u_csr__DOT__wr_val = ((1U 
                                                   == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op))
                                                   ? vlSelfRef.rv_core__DOT__u_csr__DOT__wdata
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op))
                                                    ? 
                                                   (vlSelfRef.rv_core__DOT__u_csr__DOT__wdata 
                                                    | vlSelfRef.rv_core__DOT__u_csr__DOT__rdata)
                                                    : 
                                                   ((3U 
                                                     == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op))
                                                     ? 
                                                    ((~ vlSelfRef.rv_core__DOT__u_csr__DOT__wdata) 
                                                     & vlSelfRef.rv_core__DOT__u_csr__DOT__rdata)
                                                     : vlSelfRef.rv_core__DOT__u_csr__DOT__rdata)));
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__a_abs = 
        ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__a_neg)
          ? ((IData)(1U) + (~ vlSelfRef.rv_core__DOT__u_muldiv__DOT__a))
          : vlSelfRef.rv_core__DOT__u_muldiv__DOT__a);
    vlSelfRef.rv_core__DOT__u_alu__DOT__lt_signed = 
        VL_LTS_III(32, vlSelfRef.rv_core__DOT__u_alu__DOT__a, vlSelfRef.rv_core__DOT__u_alu__DOT__b);
    vlSelfRef.rv_core__DOT__u_alu__DOT__lt_unsigned 
        = (vlSelfRef.rv_core__DOT__u_alu__DOT__a < vlSelfRef.rv_core__DOT__u_alu__DOT__b);
    vlSelfRef.rv_core__DOT__u_regfile__DOT__rdata1 
        = ((0U == (IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr1))
            ? 0U : (((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__we) 
                     & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__waddr) 
                        == (IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr1)))
                     ? vlSelfRef.rv_core__DOT__u_regfile__DOT__wdata
                     : ((0x1eU >= (0x0000001fU & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr1) 
                                                  - (IData)(1U))))
                         ? vlSelfRef.rv_core__DOT__u_regfile__DOT__regs_q
                        [(0x0000001fU & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr1) 
                                         - (IData)(1U)))]
                         : vlSelfRef.rv_core__DOT__u_regfile__DOT____Vxrand___0)));
    vlSelfRef.rv_core__DOT__u_regfile__DOT__rdata2 
        = ((0U == (IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr2))
            ? 0U : (((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__we) 
                     & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__waddr) 
                        == (IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr2)))
                     ? vlSelfRef.rv_core__DOT__u_regfile__DOT__wdata
                     : ((0x1eU >= (0x0000001fU & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr2) 
                                                  - (IData)(1U))))
                         ? vlSelfRef.rv_core__DOT__u_regfile__DOT__regs_q
                        [(0x0000001fU & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr2) 
                                         - (IData)(1U)))]
                         : vlSelfRef.rv_core__DOT__u_regfile__DOT____Vxrand___1)));
    vlSelfRef.rv_core__DOT__u_hazard__DOT__load_use 
        = ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__id_valid) 
           & ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_is_load) 
              & ((0U != (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rd)) 
                 & (((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__id_uses_rs1) 
                     & ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__id_rs1) 
                        == (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rd))) 
                    | ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__id_uses_rs2) 
                       & ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__id_rs2) 
                          == (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rd)))))));
    vlSelfRef.rv_core__DOT__u_alu__DOT__lt_s = vlSelfRef.rv_core__DOT__u_alu__DOT__lt_signed;
    vlSelfRef.rv_core__DOT__u_alu__DOT__lt_s = vlSelfRef.rv_core__DOT__u_alu__DOT__lt_signed;
    vlSelfRef.rv_core__DOT__u_alu__DOT__lt_s = vlSelfRef.rv_core__DOT__u_alu__DOT__lt_signed;
    vlSelfRef.rv_core__DOT__u_alu__DOT__result = ((0x00000010U 
                                                   & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                   ? 
                                                  ((- (IData)(
                                                              (1U 
                                                               & (~ 
                                                                  ((IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op) 
                                                                   >> 2U))))) 
                                                   & ((- (IData)(
                                                                 (1U 
                                                                  & (~ 
                                                                     ((IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op) 
                                                                      >> 1U))))) 
                                                      & ((- (IData)(
                                                                    (1U 
                                                                     & (~ (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))))) 
                                                         & (((IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__lt_unsigned)
                                                              ? vlSelfRef.rv_core__DOT__u_alu__DOT__b
                                                              : vlSelfRef.rv_core__DOT__u_alu__DOT__a) 
                                                            & (- (IData)(
                                                                         (1U 
                                                                          & (~ 
                                                                             ((IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op) 
                                                                              >> 3U)))))))))
                                                   : 
                                                  ((8U 
                                                    & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                    ? 
                                                   ((4U 
                                                     & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                     ? 
                                                    ((2U 
                                                      & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                      ? 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                       ? 
                                                      ((IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__lt_signed)
                                                        ? vlSelfRef.rv_core__DOT__u_alu__DOT__b
                                                        : vlSelfRef.rv_core__DOT__u_alu__DOT__a)
                                                       : 
                                                      ((IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__lt_unsigned)
                                                        ? vlSelfRef.rv_core__DOT__u_alu__DOT__a
                                                        : vlSelfRef.rv_core__DOT__u_alu__DOT__b))
                                                      : 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                       ? 
                                                      ((IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__lt_signed)
                                                        ? vlSelfRef.rv_core__DOT__u_alu__DOT__a
                                                        : vlSelfRef.rv_core__DOT__u_alu__DOT__b)
                                                       : 
                                                      (vlSelfRef.rv_core__DOT__u_alu__DOT__b 
                                                       + 
                                                       VL_SHIFTL_III(32,32,32, vlSelfRef.rv_core__DOT__u_alu__DOT__a, 3U))))
                                                     : 
                                                    ((2U 
                                                      & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                      ? 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                       ? 
                                                      (vlSelfRef.rv_core__DOT__u_alu__DOT__b 
                                                       + 
                                                       VL_SHIFTL_III(32,32,32, vlSelfRef.rv_core__DOT__u_alu__DOT__a, 2U))
                                                       : 
                                                      (vlSelfRef.rv_core__DOT__u_alu__DOT__b 
                                                       + 
                                                       VL_SHIFTL_III(32,32,32, vlSelfRef.rv_core__DOT__u_alu__DOT__a, 1U)))
                                                      : 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                       ? 
                                                      (vlSelfRef.rv_core__DOT__u_alu__DOT__a 
                                                       & vlSelfRef.rv_core__DOT__u_alu__DOT__b)
                                                       : 
                                                      (vlSelfRef.rv_core__DOT__u_alu__DOT__a 
                                                       | vlSelfRef.rv_core__DOT__u_alu__DOT__b))))
                                                    : 
                                                   ((4U 
                                                     & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                     ? 
                                                    ((2U 
                                                      & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                      ? 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                       ? 
                                                      VL_SHIFTRS_III(32,32,5, vlSelfRef.rv_core__DOT__u_alu__DOT__a, (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__shamt))
                                                       : 
                                                      (vlSelfRef.rv_core__DOT__u_alu__DOT__a 
                                                       >> (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__shamt)))
                                                      : 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                       ? 
                                                      (vlSelfRef.rv_core__DOT__u_alu__DOT__a 
                                                       ^ vlSelfRef.rv_core__DOT__u_alu__DOT__b)
                                                       : (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__lt_unsigned)))
                                                     : 
                                                    ((2U 
                                                      & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                      ? 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                       ? (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__lt_signed)
                                                       : 
                                                      (vlSelfRef.rv_core__DOT__u_alu__DOT__a 
                                                       << (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__shamt)))
                                                      : 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                       ? 
                                                      (vlSelfRef.rv_core__DOT__u_alu__DOT__a 
                                                       - vlSelfRef.rv_core__DOT__u_alu__DOT__b)
                                                       : 
                                                      (vlSelfRef.rv_core__DOT__u_alu__DOT__a 
                                                       + vlSelfRef.rv_core__DOT__u_alu__DOT__b))))));
    vlSelfRef.rv_core__DOT__u_alu__DOT__lt_u = vlSelfRef.rv_core__DOT__u_alu__DOT__lt_unsigned;
    vlSelfRef.rv_core__DOT__u_alu__DOT__lt_u = vlSelfRef.rv_core__DOT__u_alu__DOT__lt_unsigned;
    vlSelfRef.rv_core__DOT__u_alu__DOT__lt_u = vlSelfRef.rv_core__DOT__u_alu__DOT__lt_unsigned;
    vlSelfRef.rv_core__DOT__id_rs1_val = vlSelfRef.rv_core__DOT__u_regfile__DOT__rdata1;
    vlSelfRef.rv_core__DOT__id_rs2_val = vlSelfRef.rv_core__DOT__u_regfile__DOT__rdata2;
    vlSelfRef.rv_core__DOT__load_use = vlSelfRef.rv_core__DOT__u_hazard__DOT__load_use;
    vlSelfRef.rv_core__DOT__alu_lt_s = vlSelfRef.rv_core__DOT__u_alu__DOT__lt_s;
    vlSelfRef.rv_core__DOT__alu_result = vlSelfRef.rv_core__DOT__u_alu__DOT__result;
    vlSelfRef.rv_core__DOT__alu_lt_u = vlSelfRef.rv_core__DOT__u_alu__DOT__lt_u;
    vlSelfRef.rv_core__DOT__stall_front = ((IData)(vlSelfRef.rv_core__DOT__load_use) 
                                           | (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2));
    vlSelfRef.rv_core__DOT__ex_result = ((0U != (3U 
                                                 & (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                    >> 0x00000017U)))
                                          ? vlSelfRef.rv_core__DOT__ex_link
                                          : ((0U == 
                                              (3U & 
                                               (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                >> 0x00000015U)))
                                              ? ((0x00000080U 
                                                  & vlSelfRef.rv_core__DOT__ex_dec_q[0U])
                                                  ? vlSelfRef.rv_core__DOT__mdu_result
                                                  : vlSelfRef.rv_core__DOT__alu_result)
                                              : vlSelfRef.rv_core__DOT__csr_rdata));
    vlSelfRef.rv_core__DOT__ex_offset = (3U & vlSelfRef.rv_core__DOT__alu_result);
    vlSelfRef.rv_core__DOT__branch_taken = (1U & ((0x08000000U 
                                                   & vlSelfRef.rv_core__DOT__ex_dec_q[0U])
                                                   ? 
                                                  ((0x04000000U 
                                                    & vlSelfRef.rv_core__DOT__ex_dec_q[0U])
                                                    ? 
                                                   ((0x02000000U 
                                                     & vlSelfRef.rv_core__DOT__ex_dec_q[0U])
                                                     ? 
                                                    (~ (IData)(vlSelfRef.rv_core__DOT__alu_lt_u))
                                                     : (IData)(vlSelfRef.rv_core__DOT__alu_lt_u))
                                                    : 
                                                   ((0x02000000U 
                                                     & vlSelfRef.rv_core__DOT__ex_dec_q[0U])
                                                     ? 
                                                    (~ (IData)(vlSelfRef.rv_core__DOT__alu_lt_s))
                                                     : (IData)(vlSelfRef.rv_core__DOT__alu_lt_s)))
                                                   : 
                                                  ((~ 
                                                    (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                     >> 0x0000001aU)) 
                                                   & ((0x02000000U 
                                                       & vlSelfRef.rv_core__DOT__ex_dec_q[0U])
                                                       ? 
                                                      (vlSelfRef.rv_core__DOT__fwd_rs1 
                                                       != vlSelfRef.rv_core__DOT__fwd_rs2)
                                                       : 
                                                      (vlSelfRef.rv_core__DOT__fwd_rs1 
                                                       == vlSelfRef.rv_core__DOT__fwd_rs2)))));
    vlSelfRef.rv_core__DOT__u_fetch__DOT__stall = vlSelfRef.rv_core__DOT__stall_front;
    vlSelfRef.rv_core__DOT__ls_misaligned = 0U;
    if ((IData)((0U != (3U & vlSelfRef.rv_core__DOT__ex_dec_q[1U])))) {
        vlSelfRef.rv_core__DOT__ls_misaligned = ((0U 
                                                  != 
                                                  (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                   >> 0x0000001eU)) 
                                                 && (1U 
                                                     & ((1U 
                                                         == 
                                                         (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                          >> 0x0000001eU))
                                                         ? (IData)(vlSelfRef.rv_core__DOT__ex_offset)
                                                         : 
                                                        (0U 
                                                         != (IData)(vlSelfRef.rv_core__DOT__ex_offset)))));
        if (VL_UNLIKELY(((1U & (~ VL_ONEHOT_I((((1U 
                                                 == 
                                                 (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                  >> 0x0000001eU)) 
                                                << 1U) 
                                               | (0U 
                                                  == 
                                                  (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                   >> 0x0000001eU))))))))) {
            if ((0U != (((1U == (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                 >> 0x0000001eU)) << 1U) 
                        | (0U == (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                  >> 0x0000001eU))))) {
                if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                    VL_WRITEF_NX("[%0t] %%Error: rv_core.sv:213: Assertion failed in %m: unique case, but multiple matches found for '2'h%X'\n",4, 'M',vlSymsp->name(),"rv_core", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1000)
                                 , '#',2,(vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                          >> 0x0000001eU));
                    VL_STOP_MT("/design/rtl/rv_core.sv", 213, "");
                }
            }
        }
    }
    vlSelfRef.rv_core__DOT__flow_change = (((IData)(vlSelfRef.rv_core__DOT__branch_taken) 
                                            & (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                               >> 0x0000001cU)) 
                                           | (0U != 
                                              (3U & 
                                               (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                >> 0x00000017U))));
    vlSelfRef.rv_core__DOT__target_misaligned = ((IData)(vlSelfRef.rv_core__DOT__flow_change) 
                                                 & (vlSelfRef.rv_core__DOT__target 
                                                    >> 1U));
    vlSelfRef.rv_core__DOT__exc_cause = 2U;
    vlSelfRef.rv_core__DOT__exc_val = 0U;
    if ((1U & (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
               | (IData)(vlSelfRef.rv_core__DOT__csr_illegal)))) {
        vlSelfRef.rv_core__DOT__exc_cause = 2U;
        vlSelfRef.rv_core__DOT__exc_val = vlSelfRef.rv_core__DOT__ex_instr_q;
    } else {
        if ((8U & vlSelfRef.rv_core__DOT__ex_dec_q[0U])) {
            vlSelfRef.rv_core__DOT__exc_cause = 0x0000000bU;
        } else if ((4U & vlSelfRef.rv_core__DOT__ex_dec_q[0U])) {
            vlSelfRef.rv_core__DOT__exc_cause = 3U;
        } else if (vlSelfRef.rv_core__DOT__target_misaligned) {
            vlSelfRef.rv_core__DOT__exc_cause = 0U;
        } else if (vlSelfRef.rv_core__DOT__ls_misaligned) {
            vlSelfRef.rv_core__DOT__exc_cause = ((2U 
                                                  & vlSelfRef.rv_core__DOT__ex_dec_q[1U])
                                                  ? 4U
                                                  : 6U);
        }
        if ((1U & (~ (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                      >> 3U)))) {
            if ((4U & vlSelfRef.rv_core__DOT__ex_dec_q[0U])) {
                vlSelfRef.rv_core__DOT__exc_val = vlSelfRef.rv_core__DOT__ex_pc_q;
            } else if (vlSelfRef.rv_core__DOT__target_misaligned) {
                vlSelfRef.rv_core__DOT__exc_val = vlSelfRef.rv_core__DOT__target;
            } else if (vlSelfRef.rv_core__DOT__ls_misaligned) {
                vlSelfRef.rv_core__DOT__exc_val = vlSelfRef.rv_core__DOT__alu_result;
            }
        }
    }
    vlSelfRef.rv_core__DOT__exc = 1U;
    if ((1U & (~ (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                  | (IData)(vlSelfRef.rv_core__DOT__csr_illegal))))) {
        if ((1U & (~ (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                      >> 3U)))) {
            if ((1U & (~ (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                          >> 2U)))) {
                if ((1U & (~ (IData)(vlSelfRef.rv_core__DOT__target_misaligned)))) {
                    if ((1U & (~ (IData)(vlSelfRef.rv_core__DOT__ls_misaligned)))) {
                        vlSelfRef.rv_core__DOT__exc = 0U;
                    }
                }
            }
        }
    }
    vlSelfRef.rv_core__DOT__u_csr__DOT__trap_cause 
        = ((IData)(vlSelfRef.rv_core__DOT__irq_pending)
            ? vlSelfRef.rv_core__DOT__irq_cause : vlSelfRef.rv_core__DOT__exc_cause);
    vlSelfRef.rv_core__DOT__u_csr__DOT__trap_val = 
        (vlSelfRef.rv_core__DOT__exc_val & (- (IData)(
                                                      (1U 
                                                       & (~ (IData)(vlSelfRef.rv_core__DOT__irq_pending))))));
    vlSelfRef.rv_core__DOT__trap = ((IData)(vlSelfRef.rv_core__DOT__ex_fire) 
                                    & ((IData)(vlSelfRef.rv_core__DOT__exc) 
                                       | (IData)(vlSelfRef.rv_core__DOT__irq_pending)));
    vlSelfRef.rv_core__DOT__u_csr__DOT__trap = vlSelfRef.rv_core__DOT__trap;
    rv_core__DOT____Vcellinp__u_csr__commit = ((~ (IData)(vlSelfRef.rv_core__DOT__trap)) 
                                               & (IData)(vlSelfRef.rv_core__DOT__ex_fire));
    vlSelfRef.rv_core__DOT__u_csr__DOT__retire = rv_core__DOT____Vcellinp__u_csr__commit;
    vlSelfRef.rv_core__DOT__u_csr__DOT__commit = rv_core__DOT____Vcellinp__u_csr__commit;
    vlSelfRef.rv_core__DOT__mret = ((vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                     >> 1U) & (IData)(rv_core__DOT____Vcellinp__u_csr__commit));
    vlSelfRef.rv_core__DOT__u_csr__DOT__do_write = 
        ((0U != (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op)) 
         & ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__commit) 
            & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__is_write)));
    vlSelfRef.rv_core__DOT__u_csr__DOT__mret = vlSelfRef.rv_core__DOT__mret;
    vlSelfRef.rv_core__DOT__redirect = 0U;
    vlSelfRef.rv_core__DOT__redirect_pc = 0U;
    if (vlSelfRef.rv_core__DOT__trap) {
        vlSelfRef.rv_core__DOT__redirect = 1U;
        vlSelfRef.rv_core__DOT__redirect_pc = vlSelfRef.rv_core__DOT__csr_mtvec;
    } else if (vlSelfRef.rv_core__DOT__mret) {
        vlSelfRef.rv_core__DOT__redirect = 1U;
        vlSelfRef.rv_core__DOT__redirect_pc = vlSelfRef.rv_core__DOT__csr_mepc;
    } else if (((IData)(vlSelfRef.rv_core__DOT__ex_fire) 
                & (IData)(vlSelfRef.rv_core__DOT__flow_change))) {
        vlSelfRef.rv_core__DOT__redirect = 1U;
        vlSelfRef.rv_core__DOT__redirect_pc = vlSelfRef.rv_core__DOT__target;
    }
    vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect 
        = vlSelfRef.rv_core__DOT__redirect;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect_pc 
        = vlSelfRef.rv_core__DOT__redirect_pc;
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
                                                    (((((~ (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__rst_n)) 
                                                        & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_muldiv__DOT__rst_n__0)) 
                                                       << 9U) 
                                                      | (((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__clk) 
                                                          & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_muldiv__DOT__clk__0))) 
                                                         << 8U)) 
                                                     | (((((((~ (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__rst_n)) 
                                                             & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_csr__DOT__rst_n__0)) 
                                                            << 3U) 
                                                           | (((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__clk) 
                                                               & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_csr__DOT__clk__0))) 
                                                              << 2U)) 
                                                          | ((((~ (IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__rst_n)) 
                                                               & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_regfile__DOT__rst_n__0)) 
                                                              << 1U) 
                                                             | ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__clk) 
                                                                & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_regfile__DOT__clk__0))))) 
                                                         << 4U) 
                                                        | (((((~ (IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__rst_n)) 
                                                              & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_fetch__DOT__rst_n__0)) 
                                                             << 3U) 
                                                            | (((IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__clk) 
                                                                & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_fetch__DOT__clk__0))) 
                                                               << 2U)) 
                                                           | ((((~ (IData)(vlSelfRef.rv_core__DOT__rst_n)) 
                                                                & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__rst_n__0)) 
                                                               << 1U) 
                                                              | ((IData)(vlSelfRef.rv_core__DOT__clk) 
                                                                 & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__clk__0)))))))));
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
    vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_muldiv__DOT__clk__0 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_muldiv__DOT__rst_n__0 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__rst_n;
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
    // Locals
    IData/*31:0*/ __Vdly__rv_core__DOT__u_fetch__DOT__pc_q;
    __Vdly__rv_core__DOT__u_fetch__DOT__pc_q = 0;
    CData/*0:0*/ __Vdly__rv_core__DOT__u_fetch__DOT__pend_q;
    __Vdly__rv_core__DOT__u_fetch__DOT__pend_q = 0;
    // Body
    __Vdly__rv_core__DOT__u_fetch__DOT__pend_q = vlSelfRef.rv_core__DOT__u_fetch__DOT__pend_q;
    __Vdly__rv_core__DOT__u_fetch__DOT__pc_q = vlSelfRef.rv_core__DOT__u_fetch__DOT__pc_q;
    if (vlSelfRef.rv_core__DOT__u_fetch__DOT__rst_n) {
        if (vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect) {
            if (vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_ready) {
                __Vdly__rv_core__DOT__u_fetch__DOT__pc_q 
                    = vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect_pc;
                __Vdly__rv_core__DOT__u_fetch__DOT__pend_q = 0U;
            } else {
                __Vdly__rv_core__DOT__u_fetch__DOT__pend_q = 1U;
            }
            if ((1U & (~ (IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_ready)))) {
                vlSelfRef.rv_core__DOT__u_fetch__DOT__pend_pc_q 
                    = vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect_pc;
            }
        } else if (vlSelfRef.rv_core__DOT__u_fetch__DOT__pend_q) {
            if (vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_ready) {
                __Vdly__rv_core__DOT__u_fetch__DOT__pc_q 
                    = vlSelfRef.rv_core__DOT__u_fetch__DOT__pend_pc_q;
                __Vdly__rv_core__DOT__u_fetch__DOT__pend_q = 0U;
            }
        } else if ((((IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__started_q) 
                     & (IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_ready)) 
                    & (~ (IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__stall)))) {
            __Vdly__rv_core__DOT__u_fetch__DOT__pc_q 
                = ((IData)(4U) + vlSelfRef.rv_core__DOT__u_fetch__DOT__pc_q);
        }
        if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect) 
                      | (IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__pend_q))))) {
            if ((1U & (~ (IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__stall)))) {
                if (((IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__started_q) 
                     & (IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_ready))) {
                    vlSelfRef.rv_core__DOT__u_fetch__DOT__id_pc_q 
                        = vlSelfRef.rv_core__DOT__u_fetch__DOT__pc_q;
                    vlSelfRef.rv_core__DOT__u_fetch__DOT__id_instr_q 
                        = vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_rdata;
                }
            }
        }
        if (((IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect) 
             | (IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__pend_q))) {
            vlSelfRef.rv_core__DOT__u_fetch__DOT__id_valid_q = 0U;
        } else if ((1U & (~ (IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__stall)))) {
            vlSelfRef.rv_core__DOT__u_fetch__DOT__id_valid_q 
                = ((IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__started_q) 
                   & (IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_ready));
        }
    } else {
        __Vdly__rv_core__DOT__u_fetch__DOT__pc_q = 0U;
        __Vdly__rv_core__DOT__u_fetch__DOT__pend_q = 0U;
        vlSelfRef.rv_core__DOT__u_fetch__DOT__id_pc_q = 0U;
        vlSelfRef.rv_core__DOT__u_fetch__DOT__id_valid_q = 0U;
        vlSelfRef.rv_core__DOT__u_fetch__DOT__id_instr_q = 0x00000013U;
        vlSelfRef.rv_core__DOT__u_fetch__DOT__pend_pc_q = 0U;
    }
    vlSelfRef.rv_core__DOT__u_fetch__DOT__pc_q = __Vdly__rv_core__DOT__u_fetch__DOT__pc_q;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__pend_q = __Vdly__rv_core__DOT__u_fetch__DOT__pend_q;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__id_pc = vlSelfRef.rv_core__DOT__u_fetch__DOT__id_pc_q;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_addr 
        = vlSelfRef.rv_core__DOT__u_fetch__DOT__pc_q;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__id_valid 
        = vlSelfRef.rv_core__DOT__u_fetch__DOT__id_valid_q;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__started_q 
        = vlSelfRef.rv_core__DOT__u_fetch__DOT__rst_n;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__id_instr 
        = vlSelfRef.rv_core__DOT__u_fetch__DOT__id_instr_q;
    vlSelfRef.rv_core__DOT__imem_addr = vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_addr;
    vlSelfRef.imem_addr = vlSelfRef.rv_core__DOT__imem_addr;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_valid 
        = vlSelfRef.rv_core__DOT__u_fetch__DOT__started_q;
    vlSelfRef.rv_core__DOT__imem_valid = vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_valid;
    vlSelfRef.imem_valid = vlSelfRef.rv_core__DOT__imem_valid;
}

void Vtop___024root___nba_sequent__TOP__1(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__1\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VdfgRegularize_h6e95ff9d_0_5;
    __VdfgRegularize_h6e95ff9d_0_5 = 0;
    CData/*0:0*/ __Vdly__rv_core__DOT__u_csr__DOT__mpie_q;
    __Vdly__rv_core__DOT__u_csr__DOT__mpie_q = 0;
    QData/*63:0*/ __Vdly__rv_core__DOT__u_csr__DOT__minstret_q;
    __Vdly__rv_core__DOT__u_csr__DOT__minstret_q = 0;
    QData/*63:0*/ __Vdly__rv_core__DOT__u_csr__DOT__mcycle_q;
    __Vdly__rv_core__DOT__u_csr__DOT__mcycle_q = 0;
    // Body
    __Vdly__rv_core__DOT__u_csr__DOT__mcycle_q = vlSelfRef.rv_core__DOT__u_csr__DOT__mcycle_q;
    __Vdly__rv_core__DOT__u_csr__DOT__minstret_q = vlSelfRef.rv_core__DOT__u_csr__DOT__minstret_q;
    __Vdly__rv_core__DOT__u_csr__DOT__mpie_q = vlSelfRef.rv_core__DOT__u_csr__DOT__mpie_q;
    if (vlSelfRef.rv_core__DOT__u_csr__DOT__rst_n) {
        if (((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__do_write) 
             & (0x0b00U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)))) {
            __Vdly__rv_core__DOT__u_csr__DOT__mcycle_q 
                = ((0xffffffff00000000ULL & __Vdly__rv_core__DOT__u_csr__DOT__mcycle_q) 
                   | (IData)((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__wr_val)));
        } else if (((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__do_write) 
                    & (0x0b80U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)))) {
            __Vdly__rv_core__DOT__u_csr__DOT__mcycle_q 
                = ((0x00000000ffffffffULL & __Vdly__rv_core__DOT__u_csr__DOT__mcycle_q) 
                   | ((QData)((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__wr_val)) 
                      << 0x00000020U));
        } else {
            __Vdly__rv_core__DOT__u_csr__DOT__mcycle_q 
                = (1ULL + vlSelfRef.rv_core__DOT__u_csr__DOT__mcycle_q);
        }
        if (((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__do_write) 
             & (0x0b02U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)))) {
            __Vdly__rv_core__DOT__u_csr__DOT__minstret_q 
                = ((0xffffffff00000000ULL & __Vdly__rv_core__DOT__u_csr__DOT__minstret_q) 
                   | (IData)((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__wr_val)));
        } else if (((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__do_write) 
                    & (0x0b82U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)))) {
            __Vdly__rv_core__DOT__u_csr__DOT__minstret_q 
                = ((0x00000000ffffffffULL & __Vdly__rv_core__DOT__u_csr__DOT__minstret_q) 
                   | ((QData)((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__wr_val)) 
                      << 0x00000020U));
        } else if (vlSelfRef.rv_core__DOT__u_csr__DOT__retire) {
            __Vdly__rv_core__DOT__u_csr__DOT__minstret_q 
                = (1ULL + vlSelfRef.rv_core__DOT__u_csr__DOT__minstret_q);
        }
        if (vlSelfRef.rv_core__DOT__u_csr__DOT__trap) {
            __Vdly__rv_core__DOT__u_csr__DOT__mpie_q 
                = vlSelfRef.rv_core__DOT__u_csr__DOT__mie_q;
            vlSelfRef.rv_core__DOT__u_csr__DOT__mepc_q 
                = (vlSelfRef.rv_core__DOT__u_csr__DOT__trap_pc 
                   >> 2U);
            vlSelfRef.rv_core__DOT__u_csr__DOT__mcause_q 
                = vlSelfRef.rv_core__DOT__u_csr__DOT__trap_cause;
            vlSelfRef.rv_core__DOT__u_csr__DOT__mtval_q 
                = vlSelfRef.rv_core__DOT__u_csr__DOT__trap_val;
            vlSelfRef.rv_core__DOT__u_csr__DOT__mie_q = 0U;
        } else if (vlSelfRef.rv_core__DOT__u_csr__DOT__mret) {
            vlSelfRef.rv_core__DOT__u_csr__DOT__mie_q 
                = vlSelfRef.rv_core__DOT__u_csr__DOT__mpie_q;
            __Vdly__rv_core__DOT__u_csr__DOT__mpie_q = 1U;
        } else if (vlSelfRef.rv_core__DOT__u_csr__DOT__do_write) {
            if ((1U & (~ VL_ONEHOT_I(((((((0x0343U 
                                           == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)) 
                                          << 3U) | 
                                         ((0x0342U 
                                           == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)) 
                                          << 2U)) | 
                                        (((0x0341U 
                                           == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)) 
                                          << 1U) | 
                                         (0x0340U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)))) 
                                       << 3U) | (((0x0305U 
                                                   == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)) 
                                                  << 2U) 
                                                 | (((0x0304U 
                                                      == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)) 
                                                     << 1U) 
                                                    | (0x0300U 
                                                       == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))))))))) {
                if ((0U != ((((((0x0343U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)) 
                                << 3U) | ((0x0342U 
                                           == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)) 
                                          << 2U)) | 
                              (((0x0341U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)) 
                                << 1U) | (0x0340U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)))) 
                             << 3U) | (((0x0305U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)) 
                                        << 2U) | ((
                                                   (0x0304U 
                                                    == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)) 
                                                   << 1U) 
                                                  | (0x0300U 
                                                     == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))))))) {
                    if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                        VL_WRITEF_NX("[%0t] %%Error: rv_csr.sv:112: Assertion failed in %m: unique case, but multiple matches found for '12'h%X'\n",4, 'M',vlSymsp->name(),"rv_core.u_csr", 'T',-9
                                     , '#',64,VL_TIME_UNITED_Q(1000)
                                     , '#',12,(IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr));
                        VL_STOP_MT("/design/rtl/rv_csr.sv", 112, "");
                    }
                }
            }
            if ((0x0300U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                vlSelfRef.rv_core__DOT__u_csr__DOT__mie_q 
                    = (1U & (vlSelfRef.rv_core__DOT__u_csr__DOT__wr_val 
                             >> 3U));
                __Vdly__rv_core__DOT__u_csr__DOT__mpie_q 
                    = (1U & (vlSelfRef.rv_core__DOT__u_csr__DOT__wr_val 
                             >> 7U));
            } else if ((0x0304U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                vlSelfRef.rv_core__DOT__u_csr__DOT__meie_q 
                    = (1U & (vlSelfRef.rv_core__DOT__u_csr__DOT__wr_val 
                             >> 0x0bU));
                vlSelfRef.rv_core__DOT__u_csr__DOT__mtie_q 
                    = (1U & (vlSelfRef.rv_core__DOT__u_csr__DOT__wr_val 
                             >> 7U));
            } else if ((0x0305U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                vlSelfRef.rv_core__DOT__u_csr__DOT__mtvec_q 
                    = (vlSelfRef.rv_core__DOT__u_csr__DOT__wr_val 
                       >> 2U);
            } else if ((0x0340U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                vlSelfRef.rv_core__DOT__u_csr__DOT__mscratch_q 
                    = vlSelfRef.rv_core__DOT__u_csr__DOT__wr_val;
            } else if ((0x0341U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                vlSelfRef.rv_core__DOT__u_csr__DOT__mepc_q 
                    = (vlSelfRef.rv_core__DOT__u_csr__DOT__wr_val 
                       >> 2U);
            } else if ((0x0342U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                vlSelfRef.rv_core__DOT__u_csr__DOT__mcause_q 
                    = vlSelfRef.rv_core__DOT__u_csr__DOT__wr_val;
            } else if ((0x0343U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                vlSelfRef.rv_core__DOT__u_csr__DOT__mtval_q 
                    = vlSelfRef.rv_core__DOT__u_csr__DOT__wr_val;
            }
        }
    } else {
        __Vdly__rv_core__DOT__u_csr__DOT__mcycle_q = 0ULL;
        __Vdly__rv_core__DOT__u_csr__DOT__minstret_q = 0ULL;
        vlSelfRef.rv_core__DOT__u_csr__DOT__mie_q = 0U;
        __Vdly__rv_core__DOT__u_csr__DOT__mpie_q = 0U;
        vlSelfRef.rv_core__DOT__u_csr__DOT__meie_q = 0U;
        vlSelfRef.rv_core__DOT__u_csr__DOT__mtie_q = 0U;
        vlSelfRef.rv_core__DOT__u_csr__DOT__mtvec_q = 0U;
        vlSelfRef.rv_core__DOT__u_csr__DOT__mepc_q = 0U;
        vlSelfRef.rv_core__DOT__u_csr__DOT__mscratch_q = 0U;
        vlSelfRef.rv_core__DOT__u_csr__DOT__mcause_q = 0U;
        vlSelfRef.rv_core__DOT__u_csr__DOT__mtval_q = 0U;
    }
    vlSelfRef.rv_core__DOT__u_csr__DOT__mcycle_q = __Vdly__rv_core__DOT__u_csr__DOT__mcycle_q;
    vlSelfRef.rv_core__DOT__u_csr__DOT__minstret_q 
        = __Vdly__rv_core__DOT__u_csr__DOT__minstret_q;
    vlSelfRef.rv_core__DOT__u_csr__DOT__mpie_q = __Vdly__rv_core__DOT__u_csr__DOT__mpie_q;
    vlSelfRef.rv_core__DOT__u_csr__DOT__mepc = (vlSelfRef.rv_core__DOT__u_csr__DOT__mepc_q 
                                                << 2U);
    vlSelfRef.rv_core__DOT__u_csr__DOT__mtvec = (vlSelfRef.rv_core__DOT__u_csr__DOT__mtvec_q 
                                                 << 2U);
    vlSelfRef.rv_core__DOT__u_csr__DOT__mstatus_rd 
        = (0x00001800U | (((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__mpie_q) 
                           << 7U) | ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__mie_q) 
                                     << 3U)));
    vlSelfRef.rv_core__DOT__u_csr__DOT__mie_rd = (((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__meie_q) 
                                                   << 0x0000000bU) 
                                                  | ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__mtie_q) 
                                                     << 7U));
    __VdfgRegularize_h6e95ff9d_0_5 = ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__irq_ext) 
                                      & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__meie_q));
    vlSelfRef.rv_core__DOT__csr_mepc = vlSelfRef.rv_core__DOT__u_csr__DOT__mepc;
    vlSelfRef.rv_core__DOT__csr_mtvec = vlSelfRef.rv_core__DOT__u_csr__DOT__mtvec;
    vlSelfRef.rv_core__DOT__u_csr__DOT__irq_cause = 
        ((IData)(__VdfgRegularize_h6e95ff9d_0_5) ? 0x8000000bU
          : 0x80000007U);
    vlSelfRef.rv_core__DOT__u_csr__DOT__irq_pending 
        = ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__mie_q) 
           & (((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__irq_timer) 
               & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__mtie_q)) 
              | (IData)(__VdfgRegularize_h6e95ff9d_0_5)));
    vlSelfRef.rv_core__DOT__irq_cause = vlSelfRef.rv_core__DOT__u_csr__DOT__irq_cause;
    vlSelfRef.rv_core__DOT__irq_pending = vlSelfRef.rv_core__DOT__u_csr__DOT__irq_pending;
}

void Vtop___024root___nba_sequent__TOP__2(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__2\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*1:0*/ __Vdly__rv_core__DOT__u_muldiv__DOT__state_q;
    __Vdly__rv_core__DOT__u_muldiv__DOT__state_q = 0;
    CData/*5:0*/ __Vdly__rv_core__DOT__u_muldiv__DOT__count_q;
    __Vdly__rv_core__DOT__u_muldiv__DOT__count_q = 0;
    QData/*63:0*/ __Vdly__rv_core__DOT__u_muldiv__DOT__acc_q;
    __Vdly__rv_core__DOT__u_muldiv__DOT__acc_q = 0;
    // Body
    __Vdly__rv_core__DOT__u_muldiv__DOT__count_q = vlSelfRef.rv_core__DOT__u_muldiv__DOT__count_q;
    __Vdly__rv_core__DOT__u_muldiv__DOT__acc_q = vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q;
    __Vdly__rv_core__DOT__u_muldiv__DOT__state_q = vlSelfRef.rv_core__DOT__u_muldiv__DOT__state_q;
    if (vlSelfRef.rv_core__DOT__u_muldiv__DOT__rst_n) {
        if ((0U == (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__state_q))) {
            if (vlSelfRef.rv_core__DOT__u_muldiv__DOT__start) {
                __Vdly__rv_core__DOT__u_muldiv__DOT__state_q = 1U;
                __Vdly__rv_core__DOT__u_muldiv__DOT__count_q = 0x20U;
                vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_q 
                    = vlSelfRef.rv_core__DOT__u_muldiv__DOT__op;
                vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_b_q 
                    = vlSelfRef.rv_core__DOT__u_muldiv__DOT__b_abs;
                vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_a_q 
                    = vlSelfRef.rv_core__DOT__u_muldiv__DOT__a;
                __Vdly__rv_core__DOT__u_muldiv__DOT__acc_q 
                    = (QData)((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__a_abs));
                vlSelfRef.rv_core__DOT__u_muldiv__DOT__res_neg_q 
                    = (((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__a_neg) 
                        ^ (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__b_neg)) 
                       & (0U != vlSelfRef.rv_core__DOT__u_muldiv__DOT__b));
                vlSelfRef.rv_core__DOT__u_muldiv__DOT__rem_neg_q 
                    = vlSelfRef.rv_core__DOT__u_muldiv__DOT__a_neg;
            }
        } else if ((1U == (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__state_q))) {
            __Vdly__rv_core__DOT__u_muldiv__DOT__count_q 
                = (0x0000003fU & ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__count_q) 
                                  - (IData)(1U)));
            __Vdly__rv_core__DOT__u_muldiv__DOT__acc_q 
                = ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__run_div)
                    ? ((1U & (IData)((vlSelfRef.rv_core__DOT__u_muldiv__DOT__trial 
                                      >> 0x20U))) ? vlSelfRef.rv_core__DOT__u_muldiv__DOT__shifted
                        : (((QData)((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__trial)) 
                            << 0x00000020U) | (QData)((IData)(
                                                              (1U 
                                                               | ((IData)(
                                                                          (vlSelfRef.rv_core__DOT__u_muldiv__DOT__shifted 
                                                                           >> 1U)) 
                                                                  << 1U))))))
                    : ((vlSelfRef.rv_core__DOT__u_muldiv__DOT__mul_sum 
                        << 0x0000001fU) | (QData)((IData)(
                                                          (0x7fffffffU 
                                                           & (IData)(
                                                                     (vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q 
                                                                      >> 1U)))))));
            if ((1U == (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__count_q))) {
                __Vdly__rv_core__DOT__u_muldiv__DOT__state_q = 2U;
            }
        } else {
            __Vdly__rv_core__DOT__u_muldiv__DOT__state_q = 0U;
        }
        if ((1U & (~ VL_ONEHOT_I((((2U == (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__state_q)) 
                                   << 2U) | (((1U == (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__state_q)) 
                                              << 1U) 
                                             | (0U 
                                                == (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__state_q)))))))) {
            if ((0U != (((2U == (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__state_q)) 
                         << 2U) | (((1U == (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__state_q)) 
                                    << 1U) | (0U == (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__state_q)))))) {
                if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                    VL_WRITEF_NX("[%0t] %%Error: rv_muldiv.sv:81: Assertion failed in %m: unique case, but multiple matches found for '2'h%X'\n",4, 'M',vlSymsp->name(),"rv_core.u_muldiv", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1000)
                                 , '#',2,(IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__state_q));
                    VL_STOP_MT("/design/rtl/rv_muldiv.sv", 81, "");
                }
            }
        }
    } else {
        __Vdly__rv_core__DOT__u_muldiv__DOT__state_q = 0U;
        __Vdly__rv_core__DOT__u_muldiv__DOT__count_q = 0U;
        __Vdly__rv_core__DOT__u_muldiv__DOT__acc_q = 0ULL;
        vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_b_q = 0U;
        vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_a_q = 0U;
        vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_q = 0U;
        vlSelfRef.rv_core__DOT__u_muldiv__DOT__res_neg_q = 0U;
        vlSelfRef.rv_core__DOT__u_muldiv__DOT__rem_neg_q = 0U;
    }
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__count_q 
        = __Vdly__rv_core__DOT__u_muldiv__DOT__count_q;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q = __Vdly__rv_core__DOT__u_muldiv__DOT__acc_q;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__state_q 
        = __Vdly__rv_core__DOT__u_muldiv__DOT__state_q;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__run_div 
        = (1U & ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_q) 
                 >> 2U));
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__mul_sum 
        = (0x00000001ffffffffULL & ((QData)((IData)(
                                                    (vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q 
                                                     >> 0x00000020U))) 
                                    + ((- (QData)((IData)(
                                                          (1U 
                                                           & (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q))))) 
                                       & (QData)((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_b_q)))));
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__shifted 
        = VL_SHIFTL_QQI(64,64,32, vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q, 1U);
    if (vlSelfRef.rv_core__DOT__u_muldiv__DOT__res_neg_q) {
        vlSelfRef.rv_core__DOT__u_muldiv__DOT__mul_out 
            = (1ULL + (~ vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q));
        vlSelfRef.rv_core__DOT__u_muldiv__DOT__quo_out 
            = ((IData)(1U) + (~ (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q)));
    } else {
        vlSelfRef.rv_core__DOT__u_muldiv__DOT__mul_out 
            = vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q;
        vlSelfRef.rv_core__DOT__u_muldiv__DOT__quo_out 
            = (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q);
    }
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__rem_out 
        = ((0U == vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_b_q)
            ? vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_a_q
            : ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__rem_neg_q)
                ? ((IData)(1U) + (~ (IData)((vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q 
                                             >> 0x00000020U))))
                : (IData)((vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q 
                           >> 0x00000020U))));
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__done = (2U 
                                                   == (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__state_q));
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__trial = 
        (0x00000001ffffffffULL & ((QData)((IData)((vlSelfRef.rv_core__DOT__u_muldiv__DOT__shifted 
                                                   >> 0x00000020U))) 
                                  - (QData)((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_b_q))));
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__result = 
        ((4U & (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_q))
          ? ((2U & (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_q))
              ? vlSelfRef.rv_core__DOT__u_muldiv__DOT__rem_out
              : vlSelfRef.rv_core__DOT__u_muldiv__DOT__quo_out)
          : ((2U & (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_q))
              ? (IData)((vlSelfRef.rv_core__DOT__u_muldiv__DOT__mul_out 
                         >> 0x00000020U)) : ((1U & (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_q))
                                              ? (IData)(
                                                        (vlSelfRef.rv_core__DOT__u_muldiv__DOT__mul_out 
                                                         >> 0x00000020U))
                                              : (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__mul_out))));
    vlSelfRef.rv_core__DOT__mdu_result = vlSelfRef.rv_core__DOT__u_muldiv__DOT__result;
}

void Vtop___024root___nba_sequent__TOP__3(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__3\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v0;
    __VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v0 = 0;
    CData/*4:0*/ __VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v0;
    __VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v0 = 0;
    IData/*31:0*/ __VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v1;
    __VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v1 = 0;
    CData/*4:0*/ __VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v1;
    __VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v1 = 0;
    // Body
    if (vlSelfRef.rv_core__DOT__u_regfile__DOT__rst_n) {
        if (((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__we) 
             & (0U != (IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__waddr)))) {
            vlSelfRef.rv_core__DOT__u_regfile__DOT____Vlvbound_hf0d5df5e__0 
                = vlSelfRef.rv_core__DOT__u_regfile__DOT__wdata;
            if ((0x1eU >= (0x0000001fU & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__waddr) 
                                          - (IData)(1U))))) {
                __VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v0 
                    = vlSelfRef.rv_core__DOT__u_regfile__DOT____Vlvbound_hf0d5df5e__0;
                __VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v0 
                    = (0x0000001fU & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__waddr) 
                                      - (IData)(1U)));
                vlSelfRef.__VdlyCommitQueuerv_core__DOT__u_regfile__DOT__regs_q.enqueue(__VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v0, (IData)(__VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v0));
            }
        }
    } else {
        vlSelfRef.rv_core__DOT__u_regfile__DOT__unnamedblk1__DOT__i = 1U;
        while (VL_GTS_III(32, 0x00000020U, vlSelfRef.rv_core__DOT__u_regfile__DOT__unnamedblk1__DOT__i)) {
            vlSelfRef.rv_core__DOT__u_regfile__DOT____Vlvbound_he35c63e1__0 = 0U;
            if (VL_LIKELY(((0x1eU >= (0x0000001fU & 
                                      (vlSelfRef.rv_core__DOT__u_regfile__DOT__unnamedblk1__DOT__i 
                                       - (IData)(1U))))))) {
                __VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v1 
                    = vlSelfRef.rv_core__DOT__u_regfile__DOT____Vlvbound_he35c63e1__0;
                __VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v1 
                    = (0x0000001fU & (vlSelfRef.rv_core__DOT__u_regfile__DOT__unnamedblk1__DOT__i 
                                      - (IData)(1U)));
                vlSelfRef.__VdlyCommitQueuerv_core__DOT__u_regfile__DOT__regs_q.enqueue(__VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v1, (IData)(__VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v1));
            }
            vlSelfRef.rv_core__DOT__u_regfile__DOT__unnamedblk1__DOT__i 
                = ((IData)(1U) + vlSelfRef.rv_core__DOT__u_regfile__DOT__unnamedblk1__DOT__i);
        }
    }
    vlSelfRef.__VdlyCommitQueuerv_core__DOT__u_regfile__DOT__regs_q.commit(vlSelfRef.rv_core__DOT__u_regfile__DOT__regs_q);
}

void Vtop___024root___nba_sequent__TOP__4(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__4\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ rv_core__DOT____Vcellinp__u_regfile__we;
    rv_core__DOT____Vcellinp__u_regfile__we = 0;
    // Body
    vlSelfRef.rv_core__DOT__wb_valid_q = ((IData)(vlSelfRef.rv_core__DOT__rst_n) 
                                          && ((IData)(vlSelfRef.rv_core__DOT__mem_valid_q) 
                                              & (~ (IData)(vlSelfRef.rv_core__DOT__mem_stall))));
    if (vlSelfRef.rv_core__DOT__rst_n) {
        if (((IData)(vlSelfRef.rv_core__DOT__mdu_done) 
             & (~ (IData)(vlSelfRef.rv_core__DOT__mem_stall)))) {
            vlSelfRef.rv_core__DOT__mdu_active_q = 0U;
        } else if (vlSelfRef.rv_core__DOT__mdu_start) {
            vlSelfRef.rv_core__DOT__mdu_active_q = 1U;
        }
        if (((IData)(vlSelfRef.rv_core__DOT__mem_stall) 
             | (IData)(vlSelfRef.rv_core__DOT__mdu_busy))) {
            vlSelfRef.rv_core__DOT__ex_rs2_q = vlSelfRef.rv_core__DOT__fwd_rs2;
            vlSelfRef.rv_core__DOT__ex_rs1_q = vlSelfRef.rv_core__DOT__fwd_rs1;
        } else if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__redirect) 
                             | (IData)(vlSelfRef.rv_core__DOT__load_use))))) {
            vlSelfRef.rv_core__DOT__ex_rs2_q = vlSelfRef.rv_core__DOT__id_rs2_val;
            vlSelfRef.rv_core__DOT__ex_rs1_q = vlSelfRef.rv_core__DOT__id_rs1_val;
        }
        if (((IData)(vlSelfRef.rv_core__DOT__mem_valid_q) 
             & (~ (IData)(vlSelfRef.rv_core__DOT__mem_stall)))) {
            vlSelfRef.rv_core__DOT__wb_data_q = vlSelfRef.rv_core__DOT__mem_final;
            vlSelfRef.rv_core__DOT__wb_rd_q = vlSelfRef.rv_core__DOT__mem_rd_q;
            vlSelfRef.rv_core__DOT__wb_en_q = vlSelfRef.rv_core__DOT__mem_wb_en_q;
        }
        if ((1U & (~ (IData)(vlSelfRef.rv_core__DOT__mem_stall)))) {
            if (((IData)(vlSelfRef.rv_core__DOT__ex_fire) 
                 & (~ (IData)(vlSelfRef.rv_core__DOT__trap)))) {
                vlSelfRef.rv_core__DOT__mem_store_q 
                    = vlSelfRef.rv_core__DOT__fwd_rs2;
                vlSelfRef.rv_core__DOT__mem_result_q 
                    = vlSelfRef.rv_core__DOT__ex_result;
                vlSelfRef.rv_core__DOT__mem_unsigned_q 
                    = (1U & (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                             >> 0x0000001dU));
                vlSelfRef.rv_core__DOT__mem_size_q 
                    = (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                       >> 0x0000001eU);
                vlSelfRef.rv_core__DOT__mem_is_load_q 
                    = (1U & (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                             >> 1U));
                vlSelfRef.rv_core__DOT__mem_is_store_q 
                    = (1U & vlSelfRef.rv_core__DOT__ex_dec_q[1U]);
                vlSelfRef.rv_core__DOT__mem_rd_q = 
                    (0x0000001fU & (vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                                    >> 0x0000000dU));
                vlSelfRef.rv_core__DOT__mem_wb_en_q 
                    = (1U & (vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                             >> 0x0000000aU));
                vlSelfRef.rv_core__DOT__mem_valid_q = 1U;
            } else {
                vlSelfRef.rv_core__DOT__mem_valid_q = 0U;
            }
        }
        if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__mem_stall) 
                      | (IData)(vlSelfRef.rv_core__DOT__mdu_busy))))) {
            vlSelfRef.rv_core__DOT__ex_valid_q = ((1U 
                                                   & (~ 
                                                      ((IData)(vlSelfRef.rv_core__DOT__redirect) 
                                                       | (IData)(vlSelfRef.rv_core__DOT__load_use)))) 
                                                  && (IData)(vlSelfRef.rv_core__DOT__id_valid));
            if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__redirect) 
                          | (IData)(vlSelfRef.rv_core__DOT__load_use))))) {
                vlSelfRef.rv_core__DOT__ex_instr_q 
                    = vlSelfRef.rv_core__DOT__id_instr;
                vlSelfRef.rv_core__DOT__ex_pc_q = vlSelfRef.rv_core__DOT__id_pc;
            }
            if (((IData)(vlSelfRef.rv_core__DOT__redirect) 
                 | (IData)(vlSelfRef.rv_core__DOT__load_use))) {
                vlSelfRef.rv_core__DOT__ex_dec_q[0U] = 0U;
                vlSelfRef.rv_core__DOT__ex_dec_q[1U] = 0U;
                vlSelfRef.rv_core__DOT__ex_dec_q[2U] = 0U;
            } else if (vlSelfRef.rv_core__DOT__id_valid) {
                vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                    = vlSelfRef.rv_core__DOT__id_dec[0U];
                vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                    = vlSelfRef.rv_core__DOT__id_dec[1U];
                vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                    = vlSelfRef.rv_core__DOT__id_dec[2U];
            } else {
                vlSelfRef.rv_core__DOT__ex_dec_q[0U] = 0U;
                vlSelfRef.rv_core__DOT__ex_dec_q[1U] = 0U;
                vlSelfRef.rv_core__DOT__ex_dec_q[2U] = 0U;
            }
        }
    } else {
        vlSelfRef.rv_core__DOT__mdu_active_q = 0U;
        vlSelfRef.rv_core__DOT__ex_valid_q = 0U;
        vlSelfRef.rv_core__DOT__ex_instr_q = 0U;
        vlSelfRef.rv_core__DOT__mem_store_q = 0U;
        vlSelfRef.rv_core__DOT__ex_pc_q = 0U;
        vlSelfRef.rv_core__DOT__ex_rs2_q = 0U;
        vlSelfRef.rv_core__DOT__ex_rs1_q = 0U;
        vlSelfRef.rv_core__DOT__mem_result_q = 0U;
        vlSelfRef.rv_core__DOT__wb_data_q = 0U;
        vlSelfRef.rv_core__DOT__mem_unsigned_q = 0U;
        vlSelfRef.rv_core__DOT__mem_size_q = 0U;
        vlSelfRef.rv_core__DOT__mem_is_load_q = 0U;
        vlSelfRef.rv_core__DOT__mem_is_store_q = 0U;
        vlSelfRef.rv_core__DOT__wb_rd_q = 0U;
        vlSelfRef.rv_core__DOT__wb_en_q = 0U;
        vlSelfRef.rv_core__DOT__mem_rd_q = 0U;
        vlSelfRef.rv_core__DOT__mem_valid_q = 0U;
        vlSelfRef.rv_core__DOT__mem_wb_en_q = 0U;
        vlSelfRef.rv_core__DOT__ex_dec_q[0U] = 0U;
        vlSelfRef.rv_core__DOT__ex_dec_q[1U] = 0U;
        vlSelfRef.rv_core__DOT__ex_dec_q[2U] = 0U;
    }
    vlSelfRef.rv_core__DOT__u_lsu__DOT__store_data 
        = vlSelfRef.rv_core__DOT__mem_store_q;
    vlSelfRef.rv_core__DOT__u_csr__DOT__trap_pc = vlSelfRef.rv_core__DOT__ex_pc_q;
    vlSelfRef.rv_core__DOT__ex_link = ((IData)(4U) 
                                       + vlSelfRef.rv_core__DOT__ex_pc_q);
    vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs2_val 
        = vlSelfRef.rv_core__DOT__ex_rs2_q;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs1_val 
        = vlSelfRef.rv_core__DOT__ex_rs1_q;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__addr = vlSelfRef.rv_core__DOT__mem_result_q;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__mem_result 
        = vlSelfRef.rv_core__DOT__mem_result_q;
    vlSelfRef.rv_core__DOT__u_regfile__DOT__wdata = vlSelfRef.rv_core__DOT__wb_data_q;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__wb_data 
        = vlSelfRef.rv_core__DOT__wb_data_q;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__is_unsigned 
        = vlSelfRef.rv_core__DOT__mem_unsigned_q;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__size = vlSelfRef.rv_core__DOT__mem_size_q;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__is_load = vlSelfRef.rv_core__DOT__mem_is_load_q;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__is_store = vlSelfRef.rv_core__DOT__mem_is_store_q;
    vlSelfRef.rv_core__DOT__u_regfile__DOT__waddr = vlSelfRef.rv_core__DOT__wb_rd_q;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__wb_rd = vlSelfRef.rv_core__DOT__wb_rd_q;
    rv_core__DOT____Vcellinp__u_regfile__we = ((IData)(vlSelfRef.rv_core__DOT__wb_valid_q) 
                                               & (IData)(vlSelfRef.rv_core__DOT__wb_en_q));
    vlSelfRef.rv_core__DOT__u_lsu__DOT__offset = (3U 
                                                  & vlSelfRef.rv_core__DOT__u_lsu__DOT__addr);
    if ((1U & (~ VL_ONEHOT_I((((1U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size)) 
                               << 1U) | (0U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size))))))) {
        if ((0U != (((1U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size)) 
                     << 1U) | (0U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_lsu.sv:62: Assertion failed in %m: unique case, but multiple matches found for '2'h%X'\n",4, 'M',vlSymsp->name(),"rv_core.u_lsu", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000)
                             , '#',2,(IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size));
                VL_STOP_MT("/design/rtl/rv_lsu.sv", 62, "");
            }
        }
    }
    vlSelfRef.rv_core__DOT__u_regfile__DOT__we = rv_core__DOT____Vcellinp__u_regfile__we;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__wb_en = rv_core__DOT____Vcellinp__u_regfile__we;
    if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__offset))) {
        vlSelfRef.rv_core__DOT__u_lsu__DOT__half = 
            (0x0000ffffU & (vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_rdata 
                            >> 0x00000010U));
        vlSelfRef.rv_core__DOT__u_lsu__DOT__byte_val 
            = (0x000000ffU & ((1U & (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__offset))
                               ? (vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_rdata 
                                  >> 0x00000018U) : 
                              (vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_rdata 
                               >> 0x00000010U)));
    } else {
        vlSelfRef.rv_core__DOT__u_lsu__DOT__half = 
            (0x0000ffffU & vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_rdata);
        vlSelfRef.rv_core__DOT__u_lsu__DOT__byte_val 
            = (0x000000ffU & ((1U & (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__offset))
                               ? (vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_rdata 
                                  >> 8U) : vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_rdata));
    }
    vlSelfRef.rv_core__DOT__u_hazard__DOT__mem_rd = vlSelfRef.rv_core__DOT__mem_rd_q;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__mem_valid = vlSelfRef.rv_core__DOT__mem_valid_q;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__mem_wb_en 
        = ((IData)(vlSelfRef.rv_core__DOT__mem_valid_q) 
           & (IData)(vlSelfRef.rv_core__DOT__mem_wb_en_q));
    vlSelfRef.rv_core__DOT__u_lsu__DOT__load_data = 
        ((0U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size))
          ? (((- (IData)(((~ (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__is_unsigned)) 
                          & ((IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__byte_val) 
                             >> 7U)))) << 8U) | (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__byte_val))
          : ((1U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size))
              ? (((- (IData)(((~ (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__is_unsigned)) 
                              & ((IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__half) 
                                 >> 0x0000000fU)))) 
                  << 0x00000010U) | (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__half))
              : vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_rdata));
    if (((IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__mem_valid) 
         & (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__is_store))) {
        vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_write = 1U;
        vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_wstrb = 0U;
        vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_wdata = 0U;
        if ((0U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size))) {
            vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_wstrb 
                = (0x0000000fU & ((IData)(1U) << (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__offset)));
            vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_wdata 
                = ((vlSelfRef.rv_core__DOT__u_lsu__DOT__store_data 
                    << 0x00000018U) | ((0x00ff0000U 
                                        & (vlSelfRef.rv_core__DOT__u_lsu__DOT__store_data 
                                           << 0x00000010U)) 
                                       | ((0x0000ff00U 
                                           & (vlSelfRef.rv_core__DOT__u_lsu__DOT__store_data 
                                              << 8U)) 
                                          | (0x000000ffU 
                                             & vlSelfRef.rv_core__DOT__u_lsu__DOT__store_data))));
        } else if ((1U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size))) {
            vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_wstrb 
                = ((2U & (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__offset))
                    ? 0x0cU : 3U);
            vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_wdata 
                = ((vlSelfRef.rv_core__DOT__u_lsu__DOT__store_data 
                    << 0x00000010U) | (0x0000ffffU 
                                       & vlSelfRef.rv_core__DOT__u_lsu__DOT__store_data));
        } else {
            vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_wstrb = 0x0fU;
            vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_wdata 
                = vlSelfRef.rv_core__DOT__u_lsu__DOT__store_data;
        }
        if ((1U & (~ VL_ONEHOT_I((((1U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size)) 
                                   << 1U) | (0U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size))))))) {
            if ((0U != (((1U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size)) 
                         << 1U) | (0U == (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size))))) {
                if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                    VL_WRITEF_NX("[%0t] %%Error: rv_lsu.sv:33: Assertion failed in %m: unique case, but multiple matches found for '2'h%X'\n",4, 'M',vlSymsp->name(),"rv_core.u_lsu", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1000)
                                 , '#',2,(IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__size));
                    VL_STOP_MT("/design/rtl/rv_lsu.sv", 33, "");
                }
            }
        }
    } else {
        vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_write = 0U;
        vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_wstrb = 0U;
        vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_wdata = 0U;
    }
    vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_valid 
        = ((IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__mem_valid) 
           & ((IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__is_load) 
              | (IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__is_store)));
    vlSelfRef.rv_core__DOT__load_data = vlSelfRef.rv_core__DOT__u_lsu__DOT__load_data;
    vlSelfRef.rv_core__DOT__dmem_write = vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_write;
    vlSelfRef.rv_core__DOT__dmem_wstrb = vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_wstrb;
    vlSelfRef.rv_core__DOT__dmem_wdata = vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_wdata;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_addr = 
        (0xfffffffcU & (vlSelfRef.rv_core__DOT__u_lsu__DOT__addr 
                        & (- (IData)((IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_valid)))));
    vlSelfRef.rv_core__DOT__dmem_valid = vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_valid;
    if ((1U & (~ VL_ONEHOT_I((((2U == (3U & (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                             >> 3U))) 
                               << 1U) | (1U == (3U 
                                                & (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                                   >> 3U)))))))) {
        if ((0U != (((2U == (3U & (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                   >> 3U))) << 1U) 
                    | (1U == (3U & (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                    >> 3U)))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core.sv:168: Assertion failed in %m: unique case, but multiple matches found for '2'h%X'\n",4, 'M',vlSymsp->name(),"rv_core", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000)
                             , '#',2,(3U & (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                            >> 3U)));
                VL_STOP_MT("/design/rtl/rv_core.sv", 168, "");
            }
        }
    }
    vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rd = 
        (0x0000001fU & (vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                        >> 0x0000000dU));
    vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_is_load 
        = ((IData)(vlSelfRef.rv_core__DOT__ex_valid_q) 
           & (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
              >> 1U));
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__op = (7U 
                                                 & (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                    >> 4U));
    vlSelfRef.rv_core__DOT__csr_masked_zero = (0U == 
                                               (0x0000001fU 
                                                & (vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                                                   >> 0x00000017U)));
    vlSelfRef.rv_core__DOT__u_alu__DOT__op = (0x0000001fU 
                                              & (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                                 >> 5U));
    vlSelfRef.__VdfgRegularize_h6e95ff9d_0_1 = ((IData)(vlSelfRef.rv_core__DOT__ex_valid_q) 
                                                & (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                   >> 7U));
    vlSelfRef.rv_core__DOT__u_csr__DOT__op = (3U & 
                                              (((vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                 << 0x0000000bU) 
                                                | (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                   >> 0x00000015U)) 
                                               & (- (IData)((IData)(vlSelfRef.rv_core__DOT__ex_valid_q)))));
    vlSelfRef.rv_core__DOT__u_csr__DOT__addr = (0x00000fffU 
                                                & (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                   >> 8U));
    vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs2 = 
        (0x0000001fU & (vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                        >> 0x00000012U));
    vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs1 = 
        (0x0000001fU & (vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                        >> 0x00000017U));
    vlSelfRef.rv_core__DOT__mem_final = ((IData)(vlSelfRef.rv_core__DOT__mem_is_load_q)
                                          ? vlSelfRef.rv_core__DOT__load_data
                                          : vlSelfRef.rv_core__DOT__mem_result_q);
    vlSelfRef.dmem_write = vlSelfRef.rv_core__DOT__dmem_write;
    vlSelfRef.dmem_wstrb = vlSelfRef.rv_core__DOT__dmem_wstrb;
    vlSelfRef.dmem_wdata = vlSelfRef.rv_core__DOT__dmem_wdata;
    vlSelfRef.rv_core__DOT__dmem_addr = vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_addr;
    vlSelfRef.dmem_valid = vlSelfRef.rv_core__DOT__dmem_valid;
    vlSelfRef.rv_core__DOT__mem_stall = ((~ (IData)(vlSelfRef.rv_core__DOT__dmem_ready)) 
                                         & (IData)(vlSelfRef.rv_core__DOT__dmem_valid));
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__is_div = 
        (1U & ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op) 
               >> 2U));
    vlSelfRef.rv_core__DOT__u_csr__DOT__write_masked_zero 
        = vlSelfRef.rv_core__DOT__csr_masked_zero;
    if ((1U & (~ VL_ONEHOT_I((((3U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op)) 
                               << 2U) | (((2U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op)) 
                                          << 1U) | 
                                         (1U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op)))))))) {
        if ((0U != (((3U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op)) 
                     << 2U) | (((2U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op)) 
                                << 1U) | (1U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op)))))) {
            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_csr.sv:83: Assertion failed in %m: unique case, but multiple matches found for '2'h%X'\n",4, 'M',vlSymsp->name(),"rv_core.u_csr", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000)
                             , '#',2,(IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op));
                VL_STOP_MT("/design/rtl/rv_csr.sv", 83, "");
            }
        }
    }
    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 1U;
    if ((0x00000800U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
        if ((0x00000400U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
            if ((0x00000200U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                if ((0x00000100U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    if ((0x00000080U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    } else if ((0x00000040U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    } else if ((0x00000020U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    } else if ((0x00000010U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        if ((8U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                            vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                        } else if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                            if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                                vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                            } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                                vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                            }
                        } else if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                             >> 1U)))) {
                            if ((1U & (~ (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)))) {
                                vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                            }
                        }
                    } else {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    }
                } else {
                    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                }
            } else {
                vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
            }
        } else if ((0x00000200U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
            if ((0x00000100U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                if ((0x00000080U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    if ((0x00000040U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    } else if ((0x00000020U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    } else if ((0x00000010U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    } else if ((8U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    } else if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    } else if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                            vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                        }
                    } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    }
                } else if ((0x00000040U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                } else if ((0x00000020U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                } else if ((0x00000010U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                } else if ((8U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                } else if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                } else if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    }
                } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                }
            } else {
                vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
            }
        } else {
            vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
        }
    } else if ((0x00000400U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
    } else if ((0x00000200U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
        if ((0x00000100U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
            if ((0x00000080U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
            } else if ((0x00000040U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                if ((0x00000020U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                } else if ((0x00000010U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                } else if ((8U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                } else if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                    }
                }
            } else if ((0x00000020U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
            } else if ((0x00000010U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
            } else if ((8U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
            } else if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
                }
            } else if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
            }
        } else {
            vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
        }
    } else {
        vlSelfRef.rv_core__DOT__u_csr__DOT__known = 0U;
    }
    vlSelfRef.rv_core__DOT__u_csr__DOT__ro = (3U == 
                                              (3U & 
                                               ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                                >> 0x0000000aU)));
    vlSelfRef.rv_core__DOT__u_hazard__DOT__fwd_rs2 
        = (((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__mem_wb_en) 
            & ((0U != (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs2)) 
               & ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs2) 
                  == (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__mem_rd))))
            ? vlSelfRef.rv_core__DOT__u_hazard__DOT__mem_result
            : (((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__wb_en) 
                & ((0U != (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs2)) 
                   & ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs2) 
                      == (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__wb_rd))))
                ? vlSelfRef.rv_core__DOT__u_hazard__DOT__wb_data
                : vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs2_val));
    vlSelfRef.rv_core__DOT__u_hazard__DOT__fwd_rs1 
        = (((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__mem_wb_en) 
            & ((0U != (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs1)) 
               & ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs1) 
                  == (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__mem_rd))))
            ? vlSelfRef.rv_core__DOT__u_hazard__DOT__mem_result
            : (((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__wb_en) 
                & ((0U != (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs1)) 
                   & ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs1) 
                      == (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__wb_rd))))
                ? vlSelfRef.rv_core__DOT__u_hazard__DOT__wb_data
                : vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs1_val));
    vlSelfRef.dmem_addr = vlSelfRef.rv_core__DOT__dmem_addr;
    vlSelfRef.rv_core__DOT__mdu_start = ((~ ((IData)(vlSelfRef.rv_core__DOT__mem_stall) 
                                             | (IData)(vlSelfRef.rv_core__DOT__mdu_active_q))) 
                                         & (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_1));
    if (vlSelfRef.rv_core__DOT__u_muldiv__DOT__is_div) {
        vlSelfRef.rv_core__DOT__u_muldiv__DOT__a_signed 
            = (1U & (~ (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op)));
        vlSelfRef.rv_core__DOT__u_muldiv__DOT__b_signed 
            = (1U & (~ (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op)));
    } else {
        vlSelfRef.rv_core__DOT__u_muldiv__DOT__a_signed 
            = (1U & (3U != (3U & (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op))));
        vlSelfRef.rv_core__DOT__u_muldiv__DOT__b_signed 
            = (1U & ((1U == (3U & (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op))) 
                     | (0U == (3U & (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op)))));
    }
    vlSelfRef.rv_core__DOT__u_csr__DOT__is_write = 
        (1U & ((~ (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__write_masked_zero)) 
               | (1U == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op))));
    vlSelfRef.rv_core__DOT__fwd_rs2 = vlSelfRef.rv_core__DOT__u_hazard__DOT__fwd_rs2;
    vlSelfRef.rv_core__DOT__fwd_rs1 = vlSelfRef.rv_core__DOT__u_hazard__DOT__fwd_rs1;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__start = vlSelfRef.rv_core__DOT__mdu_start;
    vlSelfRef.rv_core__DOT__u_csr__DOT__illegal = (
                                                   (0U 
                                                    != (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op)) 
                                                   & ((~ (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__known)) 
                                                      | ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__ro) 
                                                         & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__is_write))));
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__b = vlSelfRef.rv_core__DOT__fwd_rs2;
    vlSelfRef.rv_core__DOT__alu_b = ((4U & vlSelfRef.rv_core__DOT__ex_dec_q[1U])
                                      ? ((vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                                          << 0x00000016U) 
                                         | (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                            >> 0x0000000aU))
                                      : vlSelfRef.rv_core__DOT__fwd_rs2);
    vlSelfRef.rv_core__DOT__csr_wdata = ((0x00100000U 
                                          & vlSelfRef.rv_core__DOT__ex_dec_q[0U])
                                          ? (0x0000001fU 
                                             & (vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                                                >> 0x00000017U))
                                          : vlSelfRef.rv_core__DOT__fwd_rs1);
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__a = vlSelfRef.rv_core__DOT__fwd_rs1;
    vlSelfRef.rv_core__DOT__target = ((0x00800000U 
                                       & vlSelfRef.rv_core__DOT__ex_dec_q[0U])
                                       ? (0xfffffffeU 
                                          & (vlSelfRef.rv_core__DOT__fwd_rs1 
                                             + ((vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                                                 << 0x00000016U) 
                                                | (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                                   >> 0x0000000aU))))
                                       : (vlSelfRef.rv_core__DOT__ex_pc_q 
                                          + ((vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                                              << 0x00000016U) 
                                             | (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                                >> 0x0000000aU))));
    vlSelfRef.rv_core__DOT__alu_a = ((1U == (3U & (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                                   >> 3U)))
                                      ? vlSelfRef.rv_core__DOT__ex_pc_q
                                      : (vlSelfRef.rv_core__DOT__fwd_rs1 
                                         & (- (IData)(
                                                      (2U 
                                                       != 
                                                       (3U 
                                                        & (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                                           >> 3U)))))));
    vlSelfRef.rv_core__DOT__csr_illegal = vlSelfRef.rv_core__DOT__u_csr__DOT__illegal;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__b_neg = 
        ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__b_signed) 
         & (vlSelfRef.rv_core__DOT__u_muldiv__DOT__b 
            >> 0x0000001fU));
    vlSelfRef.rv_core__DOT__u_alu__DOT__b = vlSelfRef.rv_core__DOT__alu_b;
    vlSelfRef.rv_core__DOT__u_csr__DOT__wdata = vlSelfRef.rv_core__DOT__csr_wdata;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__a_neg = 
        ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__a_signed) 
         & (vlSelfRef.rv_core__DOT__u_muldiv__DOT__a 
            >> 0x0000001fU));
    vlSelfRef.rv_core__DOT__u_alu__DOT__a = vlSelfRef.rv_core__DOT__alu_a;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__b_abs = 
        ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__b_neg)
          ? ((IData)(1U) + (~ vlSelfRef.rv_core__DOT__u_muldiv__DOT__b))
          : vlSelfRef.rv_core__DOT__u_muldiv__DOT__b);
    vlSelfRef.rv_core__DOT__u_alu__DOT__shamt = (0x0000001fU 
                                                 & vlSelfRef.rv_core__DOT__u_alu__DOT__b);
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__a_abs = 
        ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__a_neg)
          ? ((IData)(1U) + (~ vlSelfRef.rv_core__DOT__u_muldiv__DOT__a))
          : vlSelfRef.rv_core__DOT__u_muldiv__DOT__a);
    vlSelfRef.rv_core__DOT__u_alu__DOT__lt_signed = 
        VL_LTS_III(32, vlSelfRef.rv_core__DOT__u_alu__DOT__a, vlSelfRef.rv_core__DOT__u_alu__DOT__b);
    vlSelfRef.rv_core__DOT__u_alu__DOT__lt_unsigned 
        = (vlSelfRef.rv_core__DOT__u_alu__DOT__a < vlSelfRef.rv_core__DOT__u_alu__DOT__b);
    vlSelfRef.rv_core__DOT__u_alu__DOT__lt_s = vlSelfRef.rv_core__DOT__u_alu__DOT__lt_signed;
    vlSelfRef.rv_core__DOT__u_alu__DOT__lt_s = vlSelfRef.rv_core__DOT__u_alu__DOT__lt_signed;
    vlSelfRef.rv_core__DOT__u_alu__DOT__lt_s = vlSelfRef.rv_core__DOT__u_alu__DOT__lt_signed;
    vlSelfRef.rv_core__DOT__u_alu__DOT__result = ((0x00000010U 
                                                   & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                   ? 
                                                  ((- (IData)(
                                                              (1U 
                                                               & (~ 
                                                                  ((IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op) 
                                                                   >> 2U))))) 
                                                   & ((- (IData)(
                                                                 (1U 
                                                                  & (~ 
                                                                     ((IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op) 
                                                                      >> 1U))))) 
                                                      & ((- (IData)(
                                                                    (1U 
                                                                     & (~ (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))))) 
                                                         & (((IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__lt_unsigned)
                                                              ? vlSelfRef.rv_core__DOT__u_alu__DOT__b
                                                              : vlSelfRef.rv_core__DOT__u_alu__DOT__a) 
                                                            & (- (IData)(
                                                                         (1U 
                                                                          & (~ 
                                                                             ((IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op) 
                                                                              >> 3U)))))))))
                                                   : 
                                                  ((8U 
                                                    & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                    ? 
                                                   ((4U 
                                                     & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                     ? 
                                                    ((2U 
                                                      & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                      ? 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                       ? 
                                                      ((IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__lt_signed)
                                                        ? vlSelfRef.rv_core__DOT__u_alu__DOT__b
                                                        : vlSelfRef.rv_core__DOT__u_alu__DOT__a)
                                                       : 
                                                      ((IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__lt_unsigned)
                                                        ? vlSelfRef.rv_core__DOT__u_alu__DOT__a
                                                        : vlSelfRef.rv_core__DOT__u_alu__DOT__b))
                                                      : 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                       ? 
                                                      ((IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__lt_signed)
                                                        ? vlSelfRef.rv_core__DOT__u_alu__DOT__a
                                                        : vlSelfRef.rv_core__DOT__u_alu__DOT__b)
                                                       : 
                                                      (vlSelfRef.rv_core__DOT__u_alu__DOT__b 
                                                       + 
                                                       VL_SHIFTL_III(32,32,32, vlSelfRef.rv_core__DOT__u_alu__DOT__a, 3U))))
                                                     : 
                                                    ((2U 
                                                      & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                      ? 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                       ? 
                                                      (vlSelfRef.rv_core__DOT__u_alu__DOT__b 
                                                       + 
                                                       VL_SHIFTL_III(32,32,32, vlSelfRef.rv_core__DOT__u_alu__DOT__a, 2U))
                                                       : 
                                                      (vlSelfRef.rv_core__DOT__u_alu__DOT__b 
                                                       + 
                                                       VL_SHIFTL_III(32,32,32, vlSelfRef.rv_core__DOT__u_alu__DOT__a, 1U)))
                                                      : 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                       ? 
                                                      (vlSelfRef.rv_core__DOT__u_alu__DOT__a 
                                                       & vlSelfRef.rv_core__DOT__u_alu__DOT__b)
                                                       : 
                                                      (vlSelfRef.rv_core__DOT__u_alu__DOT__a 
                                                       | vlSelfRef.rv_core__DOT__u_alu__DOT__b))))
                                                    : 
                                                   ((4U 
                                                     & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                     ? 
                                                    ((2U 
                                                      & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                      ? 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                       ? 
                                                      VL_SHIFTRS_III(32,32,5, vlSelfRef.rv_core__DOT__u_alu__DOT__a, (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__shamt))
                                                       : 
                                                      (vlSelfRef.rv_core__DOT__u_alu__DOT__a 
                                                       >> (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__shamt)))
                                                      : 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                       ? 
                                                      (vlSelfRef.rv_core__DOT__u_alu__DOT__a 
                                                       ^ vlSelfRef.rv_core__DOT__u_alu__DOT__b)
                                                       : (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__lt_unsigned)))
                                                     : 
                                                    ((2U 
                                                      & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                      ? 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                       ? (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__lt_signed)
                                                       : 
                                                      (vlSelfRef.rv_core__DOT__u_alu__DOT__a 
                                                       << (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__shamt)))
                                                      : 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.rv_core__DOT__u_alu__DOT__op))
                                                       ? 
                                                      (vlSelfRef.rv_core__DOT__u_alu__DOT__a 
                                                       - vlSelfRef.rv_core__DOT__u_alu__DOT__b)
                                                       : 
                                                      (vlSelfRef.rv_core__DOT__u_alu__DOT__a 
                                                       + vlSelfRef.rv_core__DOT__u_alu__DOT__b))))));
    vlSelfRef.rv_core__DOT__u_alu__DOT__lt_u = vlSelfRef.rv_core__DOT__u_alu__DOT__lt_unsigned;
    vlSelfRef.rv_core__DOT__u_alu__DOT__lt_u = vlSelfRef.rv_core__DOT__u_alu__DOT__lt_unsigned;
    vlSelfRef.rv_core__DOT__u_alu__DOT__lt_u = vlSelfRef.rv_core__DOT__u_alu__DOT__lt_unsigned;
    vlSelfRef.rv_core__DOT__alu_lt_s = vlSelfRef.rv_core__DOT__u_alu__DOT__lt_s;
    vlSelfRef.rv_core__DOT__alu_result = vlSelfRef.rv_core__DOT__u_alu__DOT__result;
    vlSelfRef.rv_core__DOT__alu_lt_u = vlSelfRef.rv_core__DOT__u_alu__DOT__lt_u;
    vlSelfRef.rv_core__DOT__ex_offset = (3U & vlSelfRef.rv_core__DOT__alu_result);
    vlSelfRef.rv_core__DOT__branch_taken = (1U & ((0x08000000U 
                                                   & vlSelfRef.rv_core__DOT__ex_dec_q[0U])
                                                   ? 
                                                  ((0x04000000U 
                                                    & vlSelfRef.rv_core__DOT__ex_dec_q[0U])
                                                    ? 
                                                   ((0x02000000U 
                                                     & vlSelfRef.rv_core__DOT__ex_dec_q[0U])
                                                     ? 
                                                    (~ (IData)(vlSelfRef.rv_core__DOT__alu_lt_u))
                                                     : (IData)(vlSelfRef.rv_core__DOT__alu_lt_u))
                                                    : 
                                                   ((0x02000000U 
                                                     & vlSelfRef.rv_core__DOT__ex_dec_q[0U])
                                                     ? 
                                                    (~ (IData)(vlSelfRef.rv_core__DOT__alu_lt_s))
                                                     : (IData)(vlSelfRef.rv_core__DOT__alu_lt_s)))
                                                   : 
                                                  ((~ 
                                                    (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                     >> 0x0000001aU)) 
                                                   & ((0x02000000U 
                                                       & vlSelfRef.rv_core__DOT__ex_dec_q[0U])
                                                       ? 
                                                      (vlSelfRef.rv_core__DOT__fwd_rs1 
                                                       != vlSelfRef.rv_core__DOT__fwd_rs2)
                                                       : 
                                                      (vlSelfRef.rv_core__DOT__fwd_rs1 
                                                       == vlSelfRef.rv_core__DOT__fwd_rs2)))));
    vlSelfRef.rv_core__DOT__ls_misaligned = 0U;
    if ((IData)((0U != (3U & vlSelfRef.rv_core__DOT__ex_dec_q[1U])))) {
        vlSelfRef.rv_core__DOT__ls_misaligned = ((0U 
                                                  != 
                                                  (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                   >> 0x0000001eU)) 
                                                 && (1U 
                                                     & ((1U 
                                                         == 
                                                         (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                          >> 0x0000001eU))
                                                         ? (IData)(vlSelfRef.rv_core__DOT__ex_offset)
                                                         : 
                                                        (0U 
                                                         != (IData)(vlSelfRef.rv_core__DOT__ex_offset)))));
        if (VL_UNLIKELY(((1U & (~ VL_ONEHOT_I((((1U 
                                                 == 
                                                 (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                  >> 0x0000001eU)) 
                                                << 1U) 
                                               | (0U 
                                                  == 
                                                  (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                   >> 0x0000001eU))))))))) {
            if ((0U != (((1U == (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                 >> 0x0000001eU)) << 1U) 
                        | (0U == (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                  >> 0x0000001eU))))) {
                if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                    VL_WRITEF_NX("[%0t] %%Error: rv_core.sv:213: Assertion failed in %m: unique case, but multiple matches found for '2'h%X'\n",4, 'M',vlSymsp->name(),"rv_core", 'T',-9
                                 , '#',64,VL_TIME_UNITED_Q(1000)
                                 , '#',2,(vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                          >> 0x0000001eU));
                    VL_STOP_MT("/design/rtl/rv_core.sv", 213, "");
                }
            }
        }
    }
    vlSelfRef.rv_core__DOT__flow_change = (((IData)(vlSelfRef.rv_core__DOT__branch_taken) 
                                            & (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                               >> 0x0000001cU)) 
                                           | (0U != 
                                              (3U & 
                                               (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                >> 0x00000017U))));
    vlSelfRef.rv_core__DOT__target_misaligned = ((IData)(vlSelfRef.rv_core__DOT__flow_change) 
                                                 & (vlSelfRef.rv_core__DOT__target 
                                                    >> 1U));
    vlSelfRef.rv_core__DOT__exc_cause = 2U;
    vlSelfRef.rv_core__DOT__exc_val = 0U;
    if ((1U & (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
               | (IData)(vlSelfRef.rv_core__DOT__csr_illegal)))) {
        vlSelfRef.rv_core__DOT__exc_cause = 2U;
        vlSelfRef.rv_core__DOT__exc_val = vlSelfRef.rv_core__DOT__ex_instr_q;
    } else {
        if ((8U & vlSelfRef.rv_core__DOT__ex_dec_q[0U])) {
            vlSelfRef.rv_core__DOT__exc_cause = 0x0000000bU;
        } else if ((4U & vlSelfRef.rv_core__DOT__ex_dec_q[0U])) {
            vlSelfRef.rv_core__DOT__exc_cause = 3U;
        } else if (vlSelfRef.rv_core__DOT__target_misaligned) {
            vlSelfRef.rv_core__DOT__exc_cause = 0U;
        } else if (vlSelfRef.rv_core__DOT__ls_misaligned) {
            vlSelfRef.rv_core__DOT__exc_cause = ((2U 
                                                  & vlSelfRef.rv_core__DOT__ex_dec_q[1U])
                                                  ? 4U
                                                  : 6U);
        }
        if ((1U & (~ (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                      >> 3U)))) {
            if ((4U & vlSelfRef.rv_core__DOT__ex_dec_q[0U])) {
                vlSelfRef.rv_core__DOT__exc_val = vlSelfRef.rv_core__DOT__ex_pc_q;
            } else if (vlSelfRef.rv_core__DOT__target_misaligned) {
                vlSelfRef.rv_core__DOT__exc_val = vlSelfRef.rv_core__DOT__target;
            } else if (vlSelfRef.rv_core__DOT__ls_misaligned) {
                vlSelfRef.rv_core__DOT__exc_val = vlSelfRef.rv_core__DOT__alu_result;
            }
        }
    }
    vlSelfRef.rv_core__DOT__exc = 1U;
    if ((1U & (~ (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                  | (IData)(vlSelfRef.rv_core__DOT__csr_illegal))))) {
        if ((1U & (~ (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                      >> 3U)))) {
            if ((1U & (~ (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                          >> 2U)))) {
                if ((1U & (~ (IData)(vlSelfRef.rv_core__DOT__target_misaligned)))) {
                    if ((1U & (~ (IData)(vlSelfRef.rv_core__DOT__ls_misaligned)))) {
                        vlSelfRef.rv_core__DOT__exc = 0U;
                    }
                }
            }
        }
    }
}

void Vtop___024root___nba_sequent__TOP__5(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__5\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.rv_core__DOT__mdu_done = vlSelfRef.rv_core__DOT__u_muldiv__DOT__done;
}

void Vtop___024root___nba_sequent__TOP__6(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__6\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.rv_core__DOT__id_instr = vlSelfRef.rv_core__DOT__u_fetch__DOT__id_instr;
    vlSelfRef.rv_core__DOT__id_pc = vlSelfRef.rv_core__DOT__u_fetch__DOT__id_pc;
    vlSelfRef.rv_core__DOT__id_valid = vlSelfRef.rv_core__DOT__u_fetch__DOT__id_valid;
    vlSelfRef.rv_core__DOT__u_decode__DOT__instr = vlSelfRef.rv_core__DOT__id_instr;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__id_valid 
        = vlSelfRef.rv_core__DOT__id_valid;
    vlSelfRef.rv_core__DOT__u_decode__DOT__opcode = 
        (0x0000001fU & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                        >> 2U));
    vlSelfRef.rv_core__DOT__u_decode__DOT__f3 = (7U 
                                                 & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                    >> 0x0000000cU));
    vlSelfRef.rv_core__DOT__u_decode__DOT__f7 = (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                 >> 0x00000019U);
    vlSelfRef.rv_core__DOT__u_decode__DOT__imm_i = 
        (((- (IData)((vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                      >> 0x0000001fU))) << 0x0000000cU) 
         | (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
            >> 0x00000014U));
    vlSelfRef.rv_core__DOT__u_decode__DOT__imm_s = 
        (((- (IData)((vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                      >> 0x0000001fU))) << 0x0000000cU) 
         | ((0x00000fe0U & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                            >> 0x00000014U)) | (0x0000001fU 
                                                & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                   >> 7U))));
    vlSelfRef.rv_core__DOT__u_decode__DOT__imm_b = 
        (((- (IData)((vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                      >> 0x0000001fU))) << 0x0000000dU) 
         | ((((2U & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                     >> 0x0000001eU)) | (1U & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                               >> 7U))) 
             << 0x0000000bU) | ((0x000007e0U & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                >> 0x00000014U)) 
                                | (0x0000001eU & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                  >> 7U)))));
    vlSelfRef.rv_core__DOT__u_decode__DOT__imm_u = 
        (0xfffff000U & vlSelfRef.rv_core__DOT__u_decode__DOT__instr);
    vlSelfRef.rv_core__DOT__u_decode__DOT__imm_j = 
        ((((0x00000ffeU & ((- (IData)((vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                       >> 0x0000001fU))) 
                           << 1U)) | (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                      >> 0x0000001fU)) 
          << 0x00000014U) | ((((0x000001feU & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                               >> 0x0000000bU)) 
                               | (1U & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                        >> 0x00000014U))) 
                              << 0x0000000bU) | (0x000007feU 
                                                 & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                    >> 0x00000014U))));
    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] = 0U;
    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] = 0U;
    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] = 0U;
    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
        = ((0x00001fffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]) 
           | (0x0fffffffU & (((0x00007c00U & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                              >> 5U)) 
                              | ((0x000003e0U & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                 >> 0x0000000fU)) 
                                 | (0x0000001fU & (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                   >> 7U)))) 
                             << 0x0000000dU)));
    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
        = (0xfffffc07U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]);
    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
        = ((0x1fffffffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
           | (((6U & ((IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3) 
                      << 1U)) | (1U & ((IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3) 
                                       >> 2U))) << 0x0000001dU));
    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
        = ((0xf1ffffffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
           | ((IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3) 
              << 0x00000019U));
    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
        = ((0xff8000ffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
           | (((0x00001000U & ((IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3) 
                               << 0x0000000aU)) | (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                   >> 0x14U)) 
              << 8U));
    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
        = ((0xffffff8fU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
           | ((IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3) 
              << 4U));
    if ((3U != (3U & vlSelfRef.rv_core__DOT__u_decode__DOT__instr))) {
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
    } else if ((0x00000010U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
        if ((8U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
            if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
                if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                        = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                        = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                } else if ((0U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                    if ((0U == (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                >> 7U))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (8U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((0x00002000U == (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                >> 7U))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (4U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((0x00604000U == (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                >> 7U))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (2U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((0x0020a000U != (vlSelfRef.rv_core__DOT__u_decode__DOT__instr 
                                                >> 7U))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    }
                } else if ((4U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                        = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                } else {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                        = (0x00000400U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
                    if ((1U & (~ VL_ONEHOT_I((((2U 
                                                == 
                                                (3U 
                                                 & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) 
                                               << 1U) 
                                              | (1U 
                                                 == 
                                                 (3U 
                                                  & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)))))))) {
                        if ((0U != (((2U == (3U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) 
                                     << 1U) | (1U == 
                                               (3U 
                                                & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)))))) {
                            if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                                VL_WRITEF_NX("[%0t] %%Error: rv_decode.sv:182: Assertion failed in %m: unique case, but multiple matches found for '2'h%X'\n",4, 'M',vlSymsp->name(),"rv_core.u_decode", 'T',-9
                                             , '#',64,VL_TIME_UNITED_Q(1000)
                                             , '#',2,
                                             (3U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)));
                                VL_STOP_MT("/design/rtl/rv_decode.sv", 182, "");
                            }
                        }
                    }
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                        = ((0x0fffefffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]) 
                           | (0x00001000U & ((~ ((IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3) 
                                                 >> 2U)) 
                                             << 0x0000000cU)));
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                        = ((0xff9fffffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
                           | (((1U == (3U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)))
                                ? 1U : ((2U == (3U 
                                                & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)))
                                         ? 2U : 3U)) 
                              << 0x00000015U));
                }
            } else if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
                if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                        = ((0x000003ffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                           | (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_j 
                              << 0x0000000aU));
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                        = ((0x0ffffc00U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]) 
                           | (0x0fffffffU & (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_j 
                                             >> 0x00000016U)));
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                        = (0x01000000U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                        = (0x00000400U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
                } else {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                        = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                }
            } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                    = ((0x000003ffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                       | (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_i 
                          << 0x0000000aU));
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                    = ((0x0ffffc00U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]) 
                       | (0x0fffffffU & (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_i 
                                         >> 0x00000016U)));
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                    = (0x00800000U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                    = (0x00001000U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                    = (0x00000400U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                    = ((0xfffffffeU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
                       | (0U != (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)));
            } else {
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                    = ((0x000003ffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                       | (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_b 
                          << 0x0000000aU));
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                    = ((0x0ffffc00U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]) 
                       | (0x0fffffffU & (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_b 
                                         >> 0x00000016U)));
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                    = (0x10000000U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                    = (0x00001800U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                    = ((0xfffffffeU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
                       | ((2U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)) 
                          | (3U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))));
            }
        } else {
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
        }
    } else if ((8U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
        if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
            if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                    = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
            } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                    = ((0x000003ffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                       | (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_u 
                          << 0x0000000aU));
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                    = ((0x0ffffc00U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]) 
                       | (0x0fffffffU & (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_u 
                                         >> 0x00000016U)));
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                    = (0x00000014U | (0xffffffe3U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]));
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                    = (0x00000400U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
            } else {
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                    = (0x00001c00U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
                if ((0x00000040U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                        = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                } else if ((0x00000020U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                    if ((0x00000010U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((8U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((0U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                            = (0x00000020U | (0xfffffc1fU 
                                              & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]));
                    } else if ((5U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                            = (0x000000e0U | (0xfffffc1fU 
                                              & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]));
                    } else {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    }
                } else if ((0x00000010U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                    if ((8U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else {
                        if ((2U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                                = (0x00000140U | (0xfffffc1fU 
                                                  & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]));
                        } else if ((4U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                                = (0x00000160U | (0xfffffc1fU 
                                                  & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]));
                        } else if ((6U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                                = (0x00000180U | (0xfffffc1fU 
                                                  & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]));
                        } else {
                            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                                = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                        }
                        if ((1U & (~ VL_ONEHOT_I(((
                                                   (6U 
                                                    == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)) 
                                                   << 2U) 
                                                  | (((4U 
                                                       == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)) 
                                                      << 1U) 
                                                     | (2U 
                                                        == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)))))))) {
                            if ((0U != (((6U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)) 
                                         << 2U) | (
                                                   ((4U 
                                                     == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)) 
                                                    << 1U) 
                                                   | (2U 
                                                      == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)))))) {
                                if (VL_UNLIKELY((vlSymsp->_vm_contextp__->assertOn()))) {
                                    VL_WRITEF_NX("[%0t] %%Error: rv_decode.sv:141: Assertion failed in %m: unique case, but multiple matches found for '3'h%X'\n",4, 'M',vlSymsp->name(),"rv_core.u_decode", 'T',-9
                                                 , '#',64,VL_TIME_UNITED_Q(1000)
                                                 , '#',3,(IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3));
                                    VL_STOP_MT("/design/rtl/rv_decode.sv", 141, "");
                                }
                            }
                        }
                    }
                } else if ((8U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                        = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                } else if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                    if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                        if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                                = ((0xfffffc1fU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                                   | (((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                                        ? ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                                            ? 0x10U
                                            : 0x0fU)
                                        : ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                                            ? 0x0eU
                                            : 0x0dU)) 
                                      << 5U));
                        } else {
                            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                                = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                        }
                    } else {
                        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                    }
                } else if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                        = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))) {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                        = (0x00000080U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
                } else {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                        = ((0xfffffc1fU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                           | (((4U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                                ? ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                                    ? ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                                        ? 9U : 8U) : 
                                   ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                                     ? 6U : 5U)) : 
                               ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                                 ? ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                                     ? 4U : 3U) : (
                                                   (1U 
                                                    & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                                                    ? 2U
                                                    : 0U))) 
                              << 5U));
                }
            }
        } else if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
        } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
        } else {
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                = ((0x000003ffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                   | (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_s 
                      << 0x0000000aU));
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                = ((0x0ffffc00U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]) 
                   | (0x0fffffffU & (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_s 
                                     >> 0x00000016U)));
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]);
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                = (4U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]);
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                = (0x00001800U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                = ((0xfffffffeU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
                   | (2U < (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)));
        }
    } else if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
        if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
        } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                = ((0x000003ffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                   | (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_u 
                      << 0x0000000aU));
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                = ((0x0ffffc00U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]) 
                   | (0x0fffffffU & (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_u 
                                     >> 0x00000016U)));
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                = (0x0000000cU | (0xffffffe3U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]));
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                = (0x00000400U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
        } else {
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                = ((0x000003ffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                   | (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_i 
                      << 0x0000000aU));
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                = ((0x0ffffc00U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]) 
                   | (0x0fffffffU & (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_i 
                                     >> 0x00000016U)));
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                = (4U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]);
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                = (0x00001000U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
            vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
                = (0x00000400U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
            if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                        = ((0xfffffc1fU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                           | (((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                                ? 9U : 8U) << 5U));
                } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                        = ((0xfffffc1fU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                           | (((0x00000020U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))
                                ? 7U : 6U) << 5U));
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                        = ((0xfffffffeU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
                           | ((0U != (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7)) 
                              & (0x20U != (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7))));
                } else {
                    vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                        = (0x000000a0U | (0xfffffc1fU 
                                          & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]));
                }
            } else if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                    = ((0xfffffc1fU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
                       | (((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))
                            ? 4U : 3U) << 5U));
            } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) {
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                    = (0x00000040U | (0xfffffc1fU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]));
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
                    = ((0xfffffffeU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
                       | (0U != (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f7)));
            } else {
                vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
                    = (0xfffffc1fU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]);
            }
        }
    } else if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
            = ((0xfffffffeU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
               | ((1U & (~ (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) 
                  || ((0U != (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)) 
                      & (1U != (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)))));
    } else if ((1U & (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__opcode))) {
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
            = (1U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
    } else {
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
            = ((0x000003ffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]) 
               | (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_i 
                  << 0x0000000aU));
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
            = ((0x0ffffc00U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]) 
               | (0x0fffffffU & (vlSelfRef.rv_core__DOT__u_decode__DOT__imm_i 
                                 >> 0x00000016U)));
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
            = (6U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]);
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
            = (0x00001000U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
            = (0x00000400U | vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
            = ((0xfffffffeU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]) 
               | (((3U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3)) 
                   | (6U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))) 
                  | (7U == (IData)(vlSelfRef.rv_core__DOT__u_decode__DOT__f3))));
    }
    if ((1U & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U])) {
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U] 
            = (0x0ffffbffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U]);
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U] 
            = (0xfffffffcU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U]);
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
            = (0xefffffffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
            = (0xfe7fffffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
            = (0xffffff7fU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
        vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U] 
            = (0xff9fffffU & vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U]);
    }
    vlSelfRef.rv_core__DOT__id_dec[0U] = vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U];
    vlSelfRef.rv_core__DOT__id_dec[1U] = vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U];
    vlSelfRef.rv_core__DOT__id_dec[2U] = vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U];
    vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr1 
        = (0x0000001fU & (vlSelfRef.rv_core__DOT__id_dec[2U] 
                          >> 0x00000017U));
    vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr2 
        = (0x0000001fU & (vlSelfRef.rv_core__DOT__id_dec[2U] 
                          >> 0x00000012U));
    vlSelfRef.rv_core__DOT__u_hazard__DOT__id_uses_rs1 
        = (1U & (vlSelfRef.rv_core__DOT__id_dec[2U] 
                 >> 0x0000000cU));
    vlSelfRef.rv_core__DOT__u_hazard__DOT__id_uses_rs2 
        = (1U & (vlSelfRef.rv_core__DOT__id_dec[2U] 
                 >> 0x0000000bU));
    vlSelfRef.rv_core__DOT__u_hazard__DOT__id_rs1 = 
        (0x0000001fU & (vlSelfRef.rv_core__DOT__id_dec[2U] 
                        >> 0x00000017U));
    vlSelfRef.rv_core__DOT__u_hazard__DOT__id_rs2 = 
        (0x0000001fU & (vlSelfRef.rv_core__DOT__id_dec[2U] 
                        >> 0x00000012U));
}

void Vtop___024root___nba_comb__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.rv_core__DOT__u_csr__DOT__rdata = 0U;
    if ((0x00000800U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
        if ((0x00000400U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
            if ((0x00000200U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                if ((0x00000100U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                  >> 7U)))) {
                        if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                      >> 6U)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                          >> 5U)))) {
                                if ((0x00000010U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                                    if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                                  >> 3U)))) {
                                        if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                                            if ((1U 
                                                 & (~ 
                                                    ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                                     >> 1U)))) {
                                                if (
                                                    (1U 
                                                     & (~ (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)))) {
                                                    vlSelfRef.rv_core__DOT__u_csr__DOT__rdata = 0U;
                                                }
                                            }
                                        } else if (
                                                   (2U 
                                                    & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                                            vlSelfRef.rv_core__DOT__u_csr__DOT__rdata = 0U;
                                        } else if (
                                                   (1U 
                                                    & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                                            vlSelfRef.rv_core__DOT__u_csr__DOT__rdata = 0U;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } else if ((0x00000200U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
            if ((0x00000100U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                if ((0x00000080U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                    if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                  >> 6U)))) {
                        if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                      >> 5U)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                          >> 4U)))) {
                                if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                              >> 3U)))) {
                                    if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                                  >> 2U)))) {
                                        if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                                            if ((1U 
                                                 & (~ (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)))) {
                                                vlSelfRef.rv_core__DOT__u_csr__DOT__rdata 
                                                    = (IData)(
                                                              (vlSelfRef.rv_core__DOT__u_csr__DOT__minstret_q 
                                                               >> 0x20U));
                                            }
                                        } else if (
                                                   (1U 
                                                    & (~ (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)))) {
                                            vlSelfRef.rv_core__DOT__u_csr__DOT__rdata 
                                                = (IData)(
                                                          (vlSelfRef.rv_core__DOT__u_csr__DOT__mcycle_q 
                                                           >> 0x20U));
                                        }
                                    }
                                }
                            }
                        }
                    }
                } else if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                     >> 6U)))) {
                    if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                  >> 5U)))) {
                        if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                      >> 4U)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                          >> 3U)))) {
                                if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                              >> 2U)))) {
                                    if ((2U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                                        if ((1U & (~ (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)))) {
                                            vlSelfRef.rv_core__DOT__u_csr__DOT__rdata 
                                                = (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__minstret_q);
                                        }
                                    } else if ((1U 
                                                & (~ (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)))) {
                                        vlSelfRef.rv_core__DOT__u_csr__DOT__rdata 
                                            = (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__mcycle_q);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } else if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                         >> 0x0000000aU)))) {
        if ((0x00000200U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
            if ((0x00000100U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                              >> 7U)))) {
                    if ((0x00000040U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                        if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                      >> 5U)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                          >> 4U)))) {
                                if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                              >> 3U)))) {
                                    if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                                        if ((1U & (~ 
                                                   ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                                    >> 1U)))) {
                                            if ((1U 
                                                 & (~ (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr)))) {
                                                vlSelfRef.rv_core__DOT__u_csr__DOT__rdata 
                                                    = vlSelfRef.rv_core__DOT__u_csr__DOT__mip_rd;
                                            }
                                        }
                                    } else {
                                        vlSelfRef.rv_core__DOT__u_csr__DOT__rdata 
                                            = ((2U 
                                                & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))
                                                ? (
                                                   (1U 
                                                    & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))
                                                    ? vlSelfRef.rv_core__DOT__u_csr__DOT__mtval_q
                                                    : vlSelfRef.rv_core__DOT__u_csr__DOT__mcause_q)
                                                : (
                                                   (1U 
                                                    & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))
                                                    ? vlSelfRef.rv_core__DOT__u_csr__DOT__mepc
                                                    : vlSelfRef.rv_core__DOT__u_csr__DOT__mscratch_q));
                                    }
                                }
                            }
                        }
                    } else if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                         >> 5U)))) {
                        if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                      >> 4U)))) {
                            if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                          >> 3U)))) {
                                if ((4U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))) {
                                    if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                                  >> 1U)))) {
                                        vlSelfRef.rv_core__DOT__u_csr__DOT__rdata 
                                            = ((1U 
                                                & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))
                                                ? vlSelfRef.rv_core__DOT__u_csr__DOT__mtvec
                                                : vlSelfRef.rv_core__DOT__u_csr__DOT__mie_rd);
                                    }
                                } else if ((1U & (~ 
                                                  ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                                   >> 1U)))) {
                                    vlSelfRef.rv_core__DOT__u_csr__DOT__rdata 
                                        = ((1U & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr))
                                            ? 0x40001100U
                                            : vlSelfRef.rv_core__DOT__u_csr__DOT__mstatus_rd);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.rv_core__DOT__u_csr__DOT__trap_cause 
        = ((IData)(vlSelfRef.rv_core__DOT__irq_pending)
            ? vlSelfRef.rv_core__DOT__irq_cause : vlSelfRef.rv_core__DOT__exc_cause);
    vlSelfRef.rv_core__DOT__u_csr__DOT__trap_val = 
        (vlSelfRef.rv_core__DOT__exc_val & (- (IData)(
                                                      (1U 
                                                       & (~ (IData)(vlSelfRef.rv_core__DOT__irq_pending))))));
    vlSelfRef.rv_core__DOT__csr_rdata = vlSelfRef.rv_core__DOT__u_csr__DOT__rdata;
    vlSelfRef.rv_core__DOT__u_csr__DOT__wr_val = ((1U 
                                                   == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op))
                                                   ? vlSelfRef.rv_core__DOT__u_csr__DOT__wdata
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op))
                                                    ? 
                                                   (vlSelfRef.rv_core__DOT__u_csr__DOT__wdata 
                                                    | vlSelfRef.rv_core__DOT__u_csr__DOT__rdata)
                                                    : 
                                                   ((3U 
                                                     == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op))
                                                     ? 
                                                    ((~ vlSelfRef.rv_core__DOT__u_csr__DOT__wdata) 
                                                     & vlSelfRef.rv_core__DOT__u_csr__DOT__rdata)
                                                     : vlSelfRef.rv_core__DOT__u_csr__DOT__rdata)));
}

void Vtop___024root___nba_comb__TOP__1(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__1\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.rv_core__DOT__mdu_busy = ((~ (IData)(vlSelfRef.rv_core__DOT__mdu_done)) 
                                        & (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_1));
    vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2 = ((IData)(vlSelfRef.rv_core__DOT__mem_stall) 
                                                | (IData)(vlSelfRef.rv_core__DOT__mdu_busy));
    vlSelfRef.rv_core__DOT__ex_fire = ((~ (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2)) 
                                       & (IData)(vlSelfRef.rv_core__DOT__ex_valid_q));
}

void Vtop___024root___nba_comb__TOP__2(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__2\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.rv_core__DOT__u_regfile__DOT__rdata1 
        = ((0U == (IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr1))
            ? 0U : (((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__we) 
                     & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__waddr) 
                        == (IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr1)))
                     ? vlSelfRef.rv_core__DOT__u_regfile__DOT__wdata
                     : ((0x1eU >= (0x0000001fU & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr1) 
                                                  - (IData)(1U))))
                         ? vlSelfRef.rv_core__DOT__u_regfile__DOT__regs_q
                        [(0x0000001fU & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr1) 
                                         - (IData)(1U)))]
                         : vlSelfRef.rv_core__DOT__u_regfile__DOT____Vxrand___0)));
    vlSelfRef.rv_core__DOT__u_regfile__DOT__rdata2 
        = ((0U == (IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr2))
            ? 0U : (((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__we) 
                     & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__waddr) 
                        == (IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr2)))
                     ? vlSelfRef.rv_core__DOT__u_regfile__DOT__wdata
                     : ((0x1eU >= (0x0000001fU & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr2) 
                                                  - (IData)(1U))))
                         ? vlSelfRef.rv_core__DOT__u_regfile__DOT__regs_q
                        [(0x0000001fU & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr2) 
                                         - (IData)(1U)))]
                         : vlSelfRef.rv_core__DOT__u_regfile__DOT____Vxrand___1)));
    vlSelfRef.rv_core__DOT__id_rs1_val = vlSelfRef.rv_core__DOT__u_regfile__DOT__rdata1;
    vlSelfRef.rv_core__DOT__id_rs2_val = vlSelfRef.rv_core__DOT__u_regfile__DOT__rdata2;
}

void Vtop___024root___nba_comb__TOP__3(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__3\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.rv_core__DOT__u_hazard__DOT__load_use 
        = ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__id_valid) 
           & ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_is_load) 
              & ((0U != (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rd)) 
                 & (((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__id_uses_rs1) 
                     & ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__id_rs1) 
                        == (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rd))) 
                    | ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__id_uses_rs2) 
                       & ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__id_rs2) 
                          == (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rd)))))));
    vlSelfRef.rv_core__DOT__load_use = vlSelfRef.rv_core__DOT__u_hazard__DOT__load_use;
}

void Vtop___024root___nba_comb__TOP__4(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__4\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ rv_core__DOT____Vcellinp__u_csr__commit;
    rv_core__DOT____Vcellinp__u_csr__commit = 0;
    // Body
    vlSelfRef.rv_core__DOT__ex_result = ((0U != (3U 
                                                 & (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                    >> 0x00000017U)))
                                          ? vlSelfRef.rv_core__DOT__ex_link
                                          : ((0U == 
                                              (3U & 
                                               (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                >> 0x00000015U)))
                                              ? ((0x00000080U 
                                                  & vlSelfRef.rv_core__DOT__ex_dec_q[0U])
                                                  ? vlSelfRef.rv_core__DOT__mdu_result
                                                  : vlSelfRef.rv_core__DOT__alu_result)
                                              : vlSelfRef.rv_core__DOT__csr_rdata));
    vlSelfRef.rv_core__DOT__trap = ((IData)(vlSelfRef.rv_core__DOT__ex_fire) 
                                    & ((IData)(vlSelfRef.rv_core__DOT__exc) 
                                       | (IData)(vlSelfRef.rv_core__DOT__irq_pending)));
    vlSelfRef.rv_core__DOT__u_csr__DOT__trap = vlSelfRef.rv_core__DOT__trap;
    rv_core__DOT____Vcellinp__u_csr__commit = ((~ (IData)(vlSelfRef.rv_core__DOT__trap)) 
                                               & (IData)(vlSelfRef.rv_core__DOT__ex_fire));
    vlSelfRef.rv_core__DOT__u_csr__DOT__retire = rv_core__DOT____Vcellinp__u_csr__commit;
    vlSelfRef.rv_core__DOT__u_csr__DOT__commit = rv_core__DOT____Vcellinp__u_csr__commit;
    vlSelfRef.rv_core__DOT__mret = ((vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                     >> 1U) & (IData)(rv_core__DOT____Vcellinp__u_csr__commit));
    vlSelfRef.rv_core__DOT__u_csr__DOT__do_write = 
        ((0U != (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op)) 
         & ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__commit) 
            & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__is_write)));
    vlSelfRef.rv_core__DOT__u_csr__DOT__mret = vlSelfRef.rv_core__DOT__mret;
    vlSelfRef.rv_core__DOT__redirect = 0U;
    vlSelfRef.rv_core__DOT__redirect_pc = 0U;
    if (vlSelfRef.rv_core__DOT__trap) {
        vlSelfRef.rv_core__DOT__redirect = 1U;
        vlSelfRef.rv_core__DOT__redirect_pc = vlSelfRef.rv_core__DOT__csr_mtvec;
    } else if (vlSelfRef.rv_core__DOT__mret) {
        vlSelfRef.rv_core__DOT__redirect = 1U;
        vlSelfRef.rv_core__DOT__redirect_pc = vlSelfRef.rv_core__DOT__csr_mepc;
    } else if (((IData)(vlSelfRef.rv_core__DOT__ex_fire) 
                & (IData)(vlSelfRef.rv_core__DOT__flow_change))) {
        vlSelfRef.rv_core__DOT__redirect = 1U;
        vlSelfRef.rv_core__DOT__redirect_pc = vlSelfRef.rv_core__DOT__target;
    }
    vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect 
        = vlSelfRef.rv_core__DOT__redirect;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect_pc 
        = vlSelfRef.rv_core__DOT__redirect_pc;
}

void Vtop___024root___nba_comb__TOP__5(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__5\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.rv_core__DOT__stall_front = ((IData)(vlSelfRef.rv_core__DOT__load_use) 
                                           | (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2));
    vlSelfRef.rv_core__DOT__u_fetch__DOT__stall = vlSelfRef.rv_core__DOT__stall_front;
}

void Vtop___024root___eval_nba(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_nba\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __Vinline__nba_sequent__TOP__0___Vdly__rv_core__DOT__u_fetch__DOT__pc_q;
    __Vinline__nba_sequent__TOP__0___Vdly__rv_core__DOT__u_fetch__DOT__pc_q = 0;
    CData/*0:0*/ __Vinline__nba_sequent__TOP__0___Vdly__rv_core__DOT__u_fetch__DOT__pend_q;
    __Vinline__nba_sequent__TOP__0___Vdly__rv_core__DOT__u_fetch__DOT__pend_q = 0;
    IData/*31:0*/ __Vinline__nba_sequent__TOP__3___VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v0;
    __Vinline__nba_sequent__TOP__3___VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v0 = 0;
    CData/*4:0*/ __Vinline__nba_sequent__TOP__3___VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v0;
    __Vinline__nba_sequent__TOP__3___VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v0 = 0;
    IData/*31:0*/ __Vinline__nba_sequent__TOP__3___VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v1;
    __Vinline__nba_sequent__TOP__3___VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v1 = 0;
    CData/*4:0*/ __Vinline__nba_sequent__TOP__3___VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v1;
    __Vinline__nba_sequent__TOP__3___VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v1 = 0;
    CData/*0:0*/ __Vinline__nba_comb__TOP__4_rv_core__DOT____Vcellinp__u_csr__commit;
    __Vinline__nba_comb__TOP__4_rv_core__DOT____Vcellinp__u_csr__commit = 0;
    // Body
    if ((0x000000000000000cULL & vlSelfRef.__VnbaTriggered[0U])) {
        __Vinline__nba_sequent__TOP__0___Vdly__rv_core__DOT__u_fetch__DOT__pend_q 
            = vlSelfRef.rv_core__DOT__u_fetch__DOT__pend_q;
        __Vinline__nba_sequent__TOP__0___Vdly__rv_core__DOT__u_fetch__DOT__pc_q 
            = vlSelfRef.rv_core__DOT__u_fetch__DOT__pc_q;
        if (vlSelfRef.rv_core__DOT__u_fetch__DOT__rst_n) {
            if (vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect) {
                if (vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_ready) {
                    __Vinline__nba_sequent__TOP__0___Vdly__rv_core__DOT__u_fetch__DOT__pc_q 
                        = vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect_pc;
                    __Vinline__nba_sequent__TOP__0___Vdly__rv_core__DOT__u_fetch__DOT__pend_q = 0U;
                } else {
                    __Vinline__nba_sequent__TOP__0___Vdly__rv_core__DOT__u_fetch__DOT__pend_q = 1U;
                }
                if ((1U & (~ (IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_ready)))) {
                    vlSelfRef.rv_core__DOT__u_fetch__DOT__pend_pc_q 
                        = vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect_pc;
                }
            } else if (vlSelfRef.rv_core__DOT__u_fetch__DOT__pend_q) {
                if (vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_ready) {
                    __Vinline__nba_sequent__TOP__0___Vdly__rv_core__DOT__u_fetch__DOT__pc_q 
                        = vlSelfRef.rv_core__DOT__u_fetch__DOT__pend_pc_q;
                    __Vinline__nba_sequent__TOP__0___Vdly__rv_core__DOT__u_fetch__DOT__pend_q = 0U;
                }
            } else if ((((IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__started_q) 
                         & (IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_ready)) 
                        & (~ (IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__stall)))) {
                __Vinline__nba_sequent__TOP__0___Vdly__rv_core__DOT__u_fetch__DOT__pc_q 
                    = ((IData)(4U) + vlSelfRef.rv_core__DOT__u_fetch__DOT__pc_q);
            }
            if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect) 
                          | (IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__pend_q))))) {
                if ((1U & (~ (IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__stall)))) {
                    if (((IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__started_q) 
                         & (IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_ready))) {
                        vlSelfRef.rv_core__DOT__u_fetch__DOT__id_pc_q 
                            = vlSelfRef.rv_core__DOT__u_fetch__DOT__pc_q;
                        vlSelfRef.rv_core__DOT__u_fetch__DOT__id_instr_q 
                            = vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_rdata;
                    }
                }
            }
            if (((IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect) 
                 | (IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__pend_q))) {
                vlSelfRef.rv_core__DOT__u_fetch__DOT__id_valid_q = 0U;
            } else if ((1U & (~ (IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__stall)))) {
                vlSelfRef.rv_core__DOT__u_fetch__DOT__id_valid_q 
                    = ((IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__started_q) 
                       & (IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_ready));
            }
        } else {
            __Vinline__nba_sequent__TOP__0___Vdly__rv_core__DOT__u_fetch__DOT__pc_q = 0U;
            __Vinline__nba_sequent__TOP__0___Vdly__rv_core__DOT__u_fetch__DOT__pend_q = 0U;
            vlSelfRef.rv_core__DOT__u_fetch__DOT__id_pc_q = 0U;
            vlSelfRef.rv_core__DOT__u_fetch__DOT__id_valid_q = 0U;
            vlSelfRef.rv_core__DOT__u_fetch__DOT__id_instr_q = 0x00000013U;
            vlSelfRef.rv_core__DOT__u_fetch__DOT__pend_pc_q = 0U;
        }
        vlSelfRef.rv_core__DOT__u_fetch__DOT__pc_q 
            = __Vinline__nba_sequent__TOP__0___Vdly__rv_core__DOT__u_fetch__DOT__pc_q;
        vlSelfRef.rv_core__DOT__u_fetch__DOT__pend_q 
            = __Vinline__nba_sequent__TOP__0___Vdly__rv_core__DOT__u_fetch__DOT__pend_q;
        vlSelfRef.rv_core__DOT__u_fetch__DOT__id_pc 
            = vlSelfRef.rv_core__DOT__u_fetch__DOT__id_pc_q;
        vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_addr 
            = vlSelfRef.rv_core__DOT__u_fetch__DOT__pc_q;
        vlSelfRef.rv_core__DOT__u_fetch__DOT__id_valid 
            = vlSelfRef.rv_core__DOT__u_fetch__DOT__id_valid_q;
        vlSelfRef.rv_core__DOT__u_fetch__DOT__started_q 
            = vlSelfRef.rv_core__DOT__u_fetch__DOT__rst_n;
        vlSelfRef.rv_core__DOT__u_fetch__DOT__id_instr 
            = vlSelfRef.rv_core__DOT__u_fetch__DOT__id_instr_q;
        vlSelfRef.rv_core__DOT__imem_addr = vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_addr;
        vlSelfRef.imem_addr = vlSelfRef.rv_core__DOT__imem_addr;
        vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_valid 
            = vlSelfRef.rv_core__DOT__u_fetch__DOT__started_q;
        vlSelfRef.rv_core__DOT__imem_valid = vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_valid;
        vlSelfRef.imem_valid = vlSelfRef.rv_core__DOT__imem_valid;
    }
    if ((0x00000000000000c0ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__1(vlSelf);
    }
    if ((0x0000000000000300ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__2(vlSelf);
    }
    if ((0x0000000000000030ULL & vlSelfRef.__VnbaTriggered[0U])) {
        if (vlSelfRef.rv_core__DOT__u_regfile__DOT__rst_n) {
            if (((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__we) 
                 & (0U != (IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__waddr)))) {
                vlSelfRef.rv_core__DOT__u_regfile__DOT____Vlvbound_hf0d5df5e__0 
                    = vlSelfRef.rv_core__DOT__u_regfile__DOT__wdata;
                if ((0x1eU >= (0x0000001fU & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__waddr) 
                                              - (IData)(1U))))) {
                    __Vinline__nba_sequent__TOP__3___VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v0 
                        = vlSelfRef.rv_core__DOT__u_regfile__DOT____Vlvbound_hf0d5df5e__0;
                    __Vinline__nba_sequent__TOP__3___VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v0 
                        = (0x0000001fU & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__waddr) 
                                          - (IData)(1U)));
                    vlSelfRef.__VdlyCommitQueuerv_core__DOT__u_regfile__DOT__regs_q.enqueue(__Vinline__nba_sequent__TOP__3___VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v0, __Vinline__nba_sequent__TOP__3___VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v0);
                }
            }
        } else {
            vlSelfRef.rv_core__DOT__u_regfile__DOT__unnamedblk1__DOT__i = 1U;
            while (VL_GTS_III(32, 0x00000020U, vlSelfRef.rv_core__DOT__u_regfile__DOT__unnamedblk1__DOT__i)) {
                vlSelfRef.rv_core__DOT__u_regfile__DOT____Vlvbound_he35c63e1__0 = 0U;
                if (VL_LIKELY(((0x1eU >= (0x0000001fU 
                                          & (vlSelfRef.rv_core__DOT__u_regfile__DOT__unnamedblk1__DOT__i 
                                             - (IData)(1U))))))) {
                    __Vinline__nba_sequent__TOP__3___VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v1 
                        = vlSelfRef.rv_core__DOT__u_regfile__DOT____Vlvbound_he35c63e1__0;
                    __Vinline__nba_sequent__TOP__3___VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v1 
                        = (0x0000001fU & (vlSelfRef.rv_core__DOT__u_regfile__DOT__unnamedblk1__DOT__i 
                                          - (IData)(1U)));
                    vlSelfRef.__VdlyCommitQueuerv_core__DOT__u_regfile__DOT__regs_q.enqueue(__Vinline__nba_sequent__TOP__3___VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v1, __Vinline__nba_sequent__TOP__3___VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v1);
                }
                vlSelfRef.rv_core__DOT__u_regfile__DOT__unnamedblk1__DOT__i 
                    = ((IData)(1U) + vlSelfRef.rv_core__DOT__u_regfile__DOT__unnamedblk1__DOT__i);
            }
        }
        vlSelfRef.__VdlyCommitQueuerv_core__DOT__u_regfile__DOT__regs_q.commit(vlSelfRef.rv_core__DOT__u_regfile__DOT__regs_q);
    }
    if ((3ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__4(vlSelf);
    }
    if ((0x0000000000000300ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.rv_core__DOT__mdu_done = vlSelfRef.rv_core__DOT__u_muldiv__DOT__done;
    }
    if ((0x000000000000000cULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__6(vlSelf);
    }
    if ((0x00000000000000c3ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_comb__TOP__0(vlSelf);
    }
    if ((0x0000000000000303ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.rv_core__DOT__mdu_busy = ((~ (IData)(vlSelfRef.rv_core__DOT__mdu_done)) 
                                            & (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_1));
        vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2 = 
            ((IData)(vlSelfRef.rv_core__DOT__mem_stall) 
             | (IData)(vlSelfRef.rv_core__DOT__mdu_busy));
        vlSelfRef.rv_core__DOT__ex_fire = ((~ (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2)) 
                                           & (IData)(vlSelfRef.rv_core__DOT__ex_valid_q));
    }
    if ((0x000000000000003fULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.rv_core__DOT__u_regfile__DOT__rdata1 
            = ((0U == (IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr1))
                ? 0U : (((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__we) 
                         & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__waddr) 
                            == (IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr1)))
                         ? vlSelfRef.rv_core__DOT__u_regfile__DOT__wdata
                         : ((0x1eU >= (0x0000001fU 
                                       & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr1) 
                                          - (IData)(1U))))
                             ? vlSelfRef.rv_core__DOT__u_regfile__DOT__regs_q
                            [(0x0000001fU & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr1) 
                                             - (IData)(1U)))]
                             : vlSelfRef.rv_core__DOT__u_regfile__DOT____Vxrand___0)));
        vlSelfRef.rv_core__DOT__u_regfile__DOT__rdata2 
            = ((0U == (IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr2))
                ? 0U : (((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__we) 
                         & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__waddr) 
                            == (IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr2)))
                         ? vlSelfRef.rv_core__DOT__u_regfile__DOT__wdata
                         : ((0x1eU >= (0x0000001fU 
                                       & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr2) 
                                          - (IData)(1U))))
                             ? vlSelfRef.rv_core__DOT__u_regfile__DOT__regs_q
                            [(0x0000001fU & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__raddr2) 
                                             - (IData)(1U)))]
                             : vlSelfRef.rv_core__DOT__u_regfile__DOT____Vxrand___1)));
        vlSelfRef.rv_core__DOT__id_rs1_val = vlSelfRef.rv_core__DOT__u_regfile__DOT__rdata1;
        vlSelfRef.rv_core__DOT__id_rs2_val = vlSelfRef.rv_core__DOT__u_regfile__DOT__rdata2;
    }
    if ((0x000000000000000fULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.rv_core__DOT__u_hazard__DOT__load_use 
            = ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__id_valid) 
               & ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_is_load) 
                  & ((0U != (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rd)) 
                     & (((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__id_uses_rs1) 
                         & ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__id_rs1) 
                            == (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rd))) 
                        | ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__id_uses_rs2) 
                           & ((IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__id_rs2) 
                              == (IData)(vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rd)))))));
        vlSelfRef.rv_core__DOT__load_use = vlSelfRef.rv_core__DOT__u_hazard__DOT__load_use;
    }
    if ((0x00000000000003c3ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.rv_core__DOT__ex_result = ((0U != 
                                              (3U & 
                                               (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                >> 0x00000017U)))
                                              ? vlSelfRef.rv_core__DOT__ex_link
                                              : ((0U 
                                                  == 
                                                  (3U 
                                                   & (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                      >> 0x00000015U)))
                                                  ? 
                                                 ((0x00000080U 
                                                   & vlSelfRef.rv_core__DOT__ex_dec_q[0U])
                                                   ? vlSelfRef.rv_core__DOT__mdu_result
                                                   : vlSelfRef.rv_core__DOT__alu_result)
                                                  : vlSelfRef.rv_core__DOT__csr_rdata));
        vlSelfRef.rv_core__DOT__trap = ((IData)(vlSelfRef.rv_core__DOT__ex_fire) 
                                        & ((IData)(vlSelfRef.rv_core__DOT__exc) 
                                           | (IData)(vlSelfRef.rv_core__DOT__irq_pending)));
        vlSelfRef.rv_core__DOT__u_csr__DOT__trap = vlSelfRef.rv_core__DOT__trap;
        __Vinline__nba_comb__TOP__4_rv_core__DOT____Vcellinp__u_csr__commit 
            = ((~ (IData)(vlSelfRef.rv_core__DOT__trap)) 
               & (IData)(vlSelfRef.rv_core__DOT__ex_fire));
        vlSelfRef.rv_core__DOT__u_csr__DOT__retire 
            = __Vinline__nba_comb__TOP__4_rv_core__DOT____Vcellinp__u_csr__commit;
        vlSelfRef.rv_core__DOT__u_csr__DOT__commit 
            = __Vinline__nba_comb__TOP__4_rv_core__DOT____Vcellinp__u_csr__commit;
        vlSelfRef.rv_core__DOT__mret = ((vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                         >> 1U) & __Vinline__nba_comb__TOP__4_rv_core__DOT____Vcellinp__u_csr__commit);
        vlSelfRef.rv_core__DOT__u_csr__DOT__do_write 
            = ((0U != (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op)) 
               & ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__commit) 
                  & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__is_write)));
        vlSelfRef.rv_core__DOT__u_csr__DOT__mret = vlSelfRef.rv_core__DOT__mret;
        vlSelfRef.rv_core__DOT__redirect = 0U;
        vlSelfRef.rv_core__DOT__redirect_pc = 0U;
        if (vlSelfRef.rv_core__DOT__trap) {
            vlSelfRef.rv_core__DOT__redirect = 1U;
            vlSelfRef.rv_core__DOT__redirect_pc = vlSelfRef.rv_core__DOT__csr_mtvec;
        } else if (vlSelfRef.rv_core__DOT__mret) {
            vlSelfRef.rv_core__DOT__redirect = 1U;
            vlSelfRef.rv_core__DOT__redirect_pc = vlSelfRef.rv_core__DOT__csr_mepc;
        } else if (((IData)(vlSelfRef.rv_core__DOT__ex_fire) 
                    & (IData)(vlSelfRef.rv_core__DOT__flow_change))) {
            vlSelfRef.rv_core__DOT__redirect = 1U;
            vlSelfRef.rv_core__DOT__redirect_pc = vlSelfRef.rv_core__DOT__target;
        }
        vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect 
            = vlSelfRef.rv_core__DOT__redirect;
        vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect_pc 
            = vlSelfRef.rv_core__DOT__redirect_pc;
    }
    if ((0x000000000000030fULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.rv_core__DOT__stall_front = ((IData)(vlSelfRef.rv_core__DOT__load_use) 
                                               | (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2));
        vlSelfRef.rv_core__DOT__u_fetch__DOT__stall 
            = vlSelfRef.rv_core__DOT__stall_front;
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
            VL_FATAL_MT("/design/rtl/rv_core.sv", 3, "", "DIDNOTCONVERGE: Input combinational region did not converge after '--converge-limit' of 10000 tries");
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
            VL_FATAL_MT("/design/rtl/rv_core.sv", 3, "", "DIDNOTCONVERGE: NBA region did not converge after '--converge-limit' of 10000 tries");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        vlSelfRef.__VactIterCount = 0U;
        do {
            if (VL_UNLIKELY(((0x00002710U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vtop___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
                VL_FATAL_MT("/design/rtl/rv_core.sv", 3, "", "DIDNOTCONVERGE: Active region did not converge after '--converge-limit' of 10000 tries");
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
    if (VL_UNLIKELY(((vlSelfRef.imem_ready & 0xfeU)))) {
        Verilated::overWidthError("imem_ready");
    }
    if (VL_UNLIKELY(((vlSelfRef.dmem_ready & 0xfeU)))) {
        Verilated::overWidthError("dmem_ready");
    }
    if (VL_UNLIKELY(((vlSelfRef.irq_ext & 0xfeU)))) {
        Verilated::overWidthError("irq_ext");
    }
    if (VL_UNLIKELY(((vlSelfRef.irq_timer & 0xfeU)))) {
        Verilated::overWidthError("irq_timer");
    }
}
#endif  // VL_DEBUG
