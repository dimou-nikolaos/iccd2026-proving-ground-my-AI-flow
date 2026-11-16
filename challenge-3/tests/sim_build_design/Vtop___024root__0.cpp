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
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mie_q 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__mie_q;
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mpie_q 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__mpie_q;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__run_div 
        = (1U & ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_q) 
                 >> 2U));
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__op_q 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_q;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__op_b_q 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_b_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_valid_q 
        = vlSelfRef.rv_core__DOT__ex_valid_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_pc_q 
        = vlSelfRef.rv_core__DOT__ex_pc_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_rs1_q 
        = vlSelfRef.rv_core__DOT__ex_rs1_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_rs2_q 
        = vlSelfRef.rv_core__DOT__ex_rs2_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_dec_q[0U] 
        = vlSelfRef.rv_core__DOT__ex_dec_q[0U];
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_dec_q[1U] 
        = vlSelfRef.rv_core__DOT__ex_dec_q[1U];
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_dec_q[2U] 
        = vlSelfRef.rv_core__DOT__ex_dec_q[2U];
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_valid_q 
        = vlSelfRef.rv_core__DOT__mem_valid_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_rd_q 
        = vlSelfRef.rv_core__DOT__mem_rd_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_wb_en_q 
        = vlSelfRef.rv_core__DOT__mem_wb_en_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_is_load_q 
        = vlSelfRef.rv_core__DOT__mem_is_load_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__wb_valid_q 
        = vlSelfRef.rv_core__DOT__wb_valid_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__wb_en_q 
        = vlSelfRef.rv_core__DOT__wb_en_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__wb_rd_q 
        = vlSelfRef.rv_core__DOT__wb_rd_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__wb_data_q 
        = vlSelfRef.rv_core__DOT__wb_data_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mdu_active_q 
        = vlSelfRef.rv_core__DOT__mdu_active_q;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_pc 
        = vlSelfRef.rv_core__DOT__ex_pc_q;
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mepc_q 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__mepc_q;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__mul_sum 
        = (0x00000001ffffffffULL & ((QData)((IData)(
                                                    (vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q 
                                                     >> 0x00000020U))) 
                                    + ((- (QData)((IData)(
                                                          (1U 
                                                           & (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q))))) 
                                       & (QData)((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_b_q)))));
    vlSelfRef.rv_core__DOT__imem_rdata = vlSelfRef.imem_rdata;
    vlSelfRef.rv_core__DOT__u_csr__DOT__trap_pc = vlSelfRef.rv_core__DOT__ex_pc_q;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__shifted 
        = VL_SHIFTL_QQI(64,64,32, vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q, 1U);
    vlSelfRef.rv_core__DOT__imem_ready = vlSelfRef.imem_ready;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__id_pc = vlSelfRef.rv_core__DOT__u_fetch__DOT__id_pc_q;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_valid 
        = vlSelfRef.rv_core__DOT__u_fetch__DOT__started_q;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_addr 
        = vlSelfRef.rv_core__DOT__u_fetch__DOT__pc_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_result_q 
        = vlSelfRef.rv_core__DOT__mem_result_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_size_q 
        = vlSelfRef.rv_core__DOT__mem_size_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_unsigned_q 
        = vlSelfRef.rv_core__DOT__mem_unsigned_q;
    vlSelfRef.rv_core__DOT__ex_link = ((IData)(4U) 
                                       + vlSelfRef.rv_core__DOT__ex_pc_q);
    vlSelfRef.rv_core__DOT__u_lsu__DOT__is_unsigned 
        = vlSelfRef.rv_core__DOT__mem_unsigned_q;
    vlSelfRef.rv_core__DOT__u_regfile__DOT__waddr = vlSelfRef.rv_core__DOT__wb_rd_q;
    vlSelfRef.rv_core__DOT__u_regfile__DOT__wdata = vlSelfRef.rv_core__DOT__wb_data_q;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rd = 
        (0x0000001fU & (vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                        >> 0x0000000dU));
    vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_is_load 
        = ((IData)(vlSelfRef.rv_core__DOT__ex_valid_q) 
           & (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
              >> 1U));
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
    vlSelfRef.rv_core__DOT__clk = vlSelfRef.clk;
    vlSelfRef.rv_core__DOT__rst_n = vlSelfRef.rst_n;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__store_data 
        = vlSelfRef.rv_core__DOT__mem_store_q;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__rem_out 
        = ((0U == vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_b_q)
            ? vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_a_q
            : ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__rem_neg_q)
                ? ((IData)(1U) + (~ (IData)((vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q 
                                             >> 0x00000020U))))
                : (IData)((vlSelfRef.rv_core__DOT__u_muldiv__DOT__acc_q 
                           >> 0x00000020U))));
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
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__op = (7U 
                                                 & (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                    >> 4U));
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_instr 
        = vlSelfRef.rv_core__DOT__ex_instr_q;
    vlSelfRef.rv_core__DOT__u_csr__DOT__mepc = (vlSelfRef.rv_core__DOT__u_csr__DOT__mepc_q 
                                                << 2U);
    vlSelfRef.rv_core__DOT__u_csr__DOT__mtvec = (vlSelfRef.rv_core__DOT__u_csr__DOT__mtvec_q 
                                                 << 2U);
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
    vlSelfRef.__VdfgRegularize_h6e95ff9d_0_1 = ((IData)(vlSelfRef.rv_core__DOT__ex_valid_q) 
                                                & (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                   >> 7U));
    vlSelfRef.rv_core__DOT__dmem_ready = vlSelfRef.dmem_ready;
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
    vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs1 = 
        (0x0000001fU & (vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                        >> 0x00000017U));
    vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs1_val 
        = vlSelfRef.rv_core__DOT__ex_rs1_q;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs2 = 
        (0x0000001fU & (vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                        >> 0x00000012U));
    vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs2_val 
        = vlSelfRef.rv_core__DOT__ex_rs2_q;
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
    vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_rdata 
        = vlSelfRef.rv_core__DOT__imem_rdata;
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__trap_pc 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__trap_pc;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__trial = 
        (0x00000001ffffffffULL & ((QData)((IData)((vlSelfRef.rv_core__DOT__u_muldiv__DOT__shifted 
                                                   >> 0x00000020U))) 
                                  - (QData)((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_b_q))));
    vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_ready 
        = vlSelfRef.rv_core__DOT__imem_ready;
    vlSelfRef.rv_core__DOT__u_top_if__DOT__imem_ready 
        = vlSelfRef.rv_core__DOT__imem_ready;
    vlSelfRef.rv_core__DOT__id_pc = vlSelfRef.rv_core__DOT__u_fetch__DOT__id_pc;
    vlSelfRef.rv_core__DOT__imem_valid = vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_valid;
    vlSelfRef.rv_core__DOT__imem_addr = vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_addr;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_link 
        = vlSelfRef.rv_core__DOT__ex_link;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__clk = vlSelfRef.rv_core__DOT__clk;
    vlSelfRef.rv_core__DOT__u_regfile__DOT__clk = vlSelfRef.rv_core__DOT__clk;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__clk = vlSelfRef.rv_core__DOT__clk;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__clk = vlSelfRef.rv_core__DOT__clk;
    vlSelfRef.rv_core__DOT__u_top_if__DOT__clk = vlSelfRef.rv_core__DOT__clk;
    vlSelfRef.rv_core__DOT__u_csr__DOT__clk = vlSelfRef.rv_core__DOT__clk;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__clk = vlSelfRef.rv_core__DOT__clk;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__rst_n = vlSelfRef.rv_core__DOT__rst_n;
    vlSelfRef.rv_core__DOT__u_regfile__DOT__rst_n = vlSelfRef.rv_core__DOT__rst_n;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__rst_n 
        = vlSelfRef.rv_core__DOT__rst_n;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__rst_n 
        = vlSelfRef.rv_core__DOT__rst_n;
    vlSelfRef.rv_core__DOT__u_top_if__DOT__rst_n = vlSelfRef.rv_core__DOT__rst_n;
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
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__is_div = 
        (1U & ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__op) 
               >> 2U));
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_funct3 
        = (7U & (vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_instr 
                 >> 0x0000000cU));
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_funct7 
        = (vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_instr 
           >> 0x00000019U);
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_opcode 
        = (0x0000007fU & vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_instr);
    vlSelfRef.rv_core__DOT__csr_mepc = vlSelfRef.rv_core__DOT__u_csr__DOT__mepc;
    vlSelfRef.rv_core__DOT__csr_mtvec = vlSelfRef.rv_core__DOT__u_csr__DOT__mtvec;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__dmem_rdata 
        = vlSelfRef.rv_core__DOT__dmem_rdata;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_rdata 
        = vlSelfRef.rv_core__DOT__dmem_rdata;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__offset = (3U 
                                                  & vlSelfRef.rv_core__DOT__u_lsu__DOT__addr);
    vlSelfRef.rv_core__DOT__u_csr__DOT__write_masked_zero 
        = vlSelfRef.rv_core__DOT__csr_masked_zero;
    vlSelfRef.rv_core__DOT__u_csr__DOT__irq_timer = vlSelfRef.rv_core__DOT__irq_timer;
    vlSelfRef.rv_core__DOT__u_csr__DOT__irq_ext = vlSelfRef.rv_core__DOT__irq_ext;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__done 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__done;
    vlSelfRef.rv_core__DOT__mdu_done = vlSelfRef.rv_core__DOT__u_muldiv__DOT__done;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__dmem_ready 
        = vlSelfRef.rv_core__DOT__dmem_ready;
    vlSelfRef.rv_core__DOT__u_top_if__DOT__dmem_ready 
        = vlSelfRef.rv_core__DOT__dmem_ready;
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
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__addr 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__addr;
    vlSelfRef.rv_core__DOT__u_csr__DOT__known = 1U;
    vlSelfRef.rv_core__DOT__u_csr__DOT__ro = (3U == 
                                              (3U & 
                                               ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__addr) 
                                                >> 0x0000000aU)));
    vlSelfRef.rv_core__DOT__id_instr = vlSelfRef.rv_core__DOT__u_fetch__DOT__id_instr;
    vlSelfRef.rv_core__DOT__u_regfile__DOT__we = rv_core__DOT____Vcellinp__u_regfile__we;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__wb_en = rv_core__DOT____Vcellinp__u_regfile__we;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_pc 
        = vlSelfRef.rv_core__DOT__id_pc;
    vlSelfRef.imem_valid = vlSelfRef.rv_core__DOT__imem_valid;
    vlSelfRef.rv_core__DOT__u_top_if__DOT__imem_valid 
        = vlSelfRef.rv_core__DOT__imem_valid;
    vlSelfRef.imem_addr = vlSelfRef.rv_core__DOT__imem_addr;
    vlSelfRef.rv_core__DOT__u_top_if__DOT__imem_addr 
        = vlSelfRef.rv_core__DOT__imem_addr;
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__clk 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__clk;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__clk 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__clk;
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__rst_n 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__rst_n;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__rst_n 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__rst_n;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__result 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__result;
    vlSelfRef.rv_core__DOT__mdu_result = vlSelfRef.rv_core__DOT__u_muldiv__DOT__result;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__id_valid 
        = vlSelfRef.rv_core__DOT__id_valid;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_valid 
        = vlSelfRef.rv_core__DOT__id_valid;
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__csr_mepc 
        = vlSelfRef.rv_core__DOT__csr_mepc;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__csr_mtvec 
        = vlSelfRef.rv_core__DOT__csr_mtvec;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_off 
        = (3U & vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_result_q);
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_byte 
        = (0x000000ffU & ((0U == (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_off))
                           ? vlSelfRef.rv_core__DOT__u_core_defs__DOT__dmem_rdata
                           : ((1U == (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_off))
                               ? (vlSelfRef.rv_core__DOT__u_core_defs__DOT__dmem_rdata 
                                  >> 8U) : ((2U == (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_off))
                                             ? (vlSelfRef.rv_core__DOT__u_core_defs__DOT__dmem_rdata 
                                                >> 0x10U)
                                             : (vlSelfRef.rv_core__DOT__u_core_defs__DOT__dmem_rdata 
                                                >> 0x18U)))));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_half 
        = (0x0000ffffU & ((2U & (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_off))
                           ? (vlSelfRef.rv_core__DOT__u_core_defs__DOT__dmem_rdata 
                              >> 0x10U) : vlSelfRef.rv_core__DOT__u_core_defs__DOT__dmem_rdata));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_ref 
        = ((0U == (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_size_q))
            ? (((- (IData)(((~ (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_unsigned_q)) 
                            & ((IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_byte) 
                               >> 7U)))) << 8U) | (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_byte))
            : ((1U == (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_size_q))
                ? (((- (IData)(((~ (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_unsigned_q)) 
                                & ((IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_half) 
                                   >> 0x0000000fU)))) 
                    << 0x00000010U) | (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_half))
                : vlSelfRef.rv_core__DOT__u_core_defs__DOT__dmem_rdata));
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mdu_done 
        = vlSelfRef.rv_core__DOT__mdu_done;
    vlSelfRef.rv_core__DOT__mdu_busy = ((~ (IData)(vlSelfRef.rv_core__DOT__mdu_done)) 
                                        & (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_1));
    vlSelfRef.rv_core__DOT__dmem_write = vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_write;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_addr = 
        (0xfffffffcU & (vlSelfRef.rv_core__DOT__u_lsu__DOT__addr 
                        & (- (IData)((IData)(vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_valid)))));
    vlSelfRef.rv_core__DOT__dmem_valid = vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_valid;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__id_instr 
        = vlSelfRef.rv_core__DOT__id_instr;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_instr 
        = vlSelfRef.rv_core__DOT__id_instr;
    vlSelfRef.rv_core__DOT__u_decode__DOT__instr = vlSelfRef.rv_core__DOT__id_instr;
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mdu_result 
        = vlSelfRef.rv_core__DOT__mdu_result;
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mdu_busy 
        = vlSelfRef.rv_core__DOT__mdu_busy;
    vlSelfRef.dmem_write = vlSelfRef.rv_core__DOT__dmem_write;
    vlSelfRef.rv_core__DOT__u_top_if__DOT__dmem_write 
        = vlSelfRef.rv_core__DOT__dmem_write;
    vlSelfRef.rv_core__DOT__dmem_addr = vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_addr;
    vlSelfRef.dmem_valid = vlSelfRef.rv_core__DOT__dmem_valid;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__dmem_valid 
        = vlSelfRef.rv_core__DOT__dmem_valid;
    vlSelfRef.rv_core__DOT__u_top_if__DOT__dmem_valid 
        = vlSelfRef.rv_core__DOT__dmem_valid;
    vlSelfRef.rv_core__DOT__mem_stall = ((~ (IData)(vlSelfRef.rv_core__DOT__dmem_ready)) 
                                         & (IData)(vlSelfRef.rv_core__DOT__dmem_valid));
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_opcode 
        = (0x0000007fU & vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_instr);
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_funct3 
        = (7U & (vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_instr 
                 >> 0x0000000cU));
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_funct7 
        = (vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_instr 
           >> 0x00000019U);
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
    vlSelfRef.rv_core__DOT__fwd_rs1 = vlSelfRef.rv_core__DOT__u_hazard__DOT__fwd_rs1;
    vlSelfRef.rv_core__DOT__fwd_rs2 = vlSelfRef.rv_core__DOT__u_hazard__DOT__fwd_rs2;
    vlSelfRef.rv_core__DOT__load_data = vlSelfRef.rv_core__DOT__u_lsu__DOT__load_data;
    vlSelfRef.dmem_wstrb = vlSelfRef.rv_core__DOT__dmem_wstrb;
    vlSelfRef.rv_core__DOT__u_top_if__DOT__dmem_wstrb 
        = vlSelfRef.rv_core__DOT__dmem_wstrb;
    vlSelfRef.dmem_wdata = vlSelfRef.rv_core__DOT__dmem_wdata;
    vlSelfRef.rv_core__DOT__u_top_if__DOT__dmem_wdata 
        = vlSelfRef.rv_core__DOT__dmem_wdata;
    vlSelfRef.rv_core__DOT__csr_illegal = vlSelfRef.rv_core__DOT__u_csr__DOT__illegal;
    vlSelfRef.rv_core__DOT__csr_rdata = vlSelfRef.rv_core__DOT__u_csr__DOT__rdata;
    vlSelfRef.rv_core__DOT__irq_cause = vlSelfRef.rv_core__DOT__u_csr__DOT__irq_cause;
    vlSelfRef.rv_core__DOT__irq_pending = vlSelfRef.rv_core__DOT__u_csr__DOT__irq_pending;
    vlSelfRef.dmem_addr = vlSelfRef.rv_core__DOT__dmem_addr;
    vlSelfRef.rv_core__DOT__u_top_if__DOT__dmem_addr 
        = vlSelfRef.rv_core__DOT__dmem_addr;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_stall 
        = vlSelfRef.rv_core__DOT__mem_stall;
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__fwd_rs1 
        = vlSelfRef.rv_core__DOT__fwd_rs1;
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__fwd_rs2 
        = vlSelfRef.rv_core__DOT__fwd_rs2;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__b = vlSelfRef.rv_core__DOT__fwd_rs2;
    vlSelfRef.rv_core__DOT__alu_b = ((4U & vlSelfRef.rv_core__DOT__ex_dec_q[1U])
                                      ? ((vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                                          << 0x00000016U) 
                                         | (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                            >> 0x0000000aU))
                                      : vlSelfRef.rv_core__DOT__fwd_rs2);
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__load_data 
        = vlSelfRef.rv_core__DOT__load_data;
    vlSelfRef.rv_core__DOT__mem_final = ((IData)(vlSelfRef.rv_core__DOT__mem_is_load_q)
                                          ? vlSelfRef.rv_core__DOT__load_data
                                          : vlSelfRef.rv_core__DOT__mem_result_q);
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__csr_rdata 
        = vlSelfRef.rv_core__DOT__csr_rdata;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__irq_pending 
        = vlSelfRef.rv_core__DOT__irq_pending;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mdu_start 
        = vlSelfRef.rv_core__DOT__mdu_start;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__start = vlSelfRef.rv_core__DOT__mdu_start;
    vlSelfRef.rv_core__DOT__ex_fire = ((~ (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2)) 
                                       & (IData)(vlSelfRef.rv_core__DOT__ex_valid_q));
    vlSelfRef.rv_core__DOT__id_dec[0U] = vlSelfRef.rv_core__DOT__u_decode__DOT__dec[0U];
    vlSelfRef.rv_core__DOT__id_dec[1U] = vlSelfRef.rv_core__DOT__u_decode__DOT__dec[1U];
    vlSelfRef.rv_core__DOT__id_dec[2U] = vlSelfRef.rv_core__DOT__u_decode__DOT__dec[2U];
    vlSelfRef.rv_core__DOT__u_csr__DOT__wdata = vlSelfRef.rv_core__DOT__csr_wdata;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__a 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__a;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__a_neg = 
        ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__a_signed) 
         & (vlSelfRef.rv_core__DOT__u_muldiv__DOT__a 
            >> 0x0000001fU));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__target 
        = vlSelfRef.rv_core__DOT__target;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_target 
        = vlSelfRef.rv_core__DOT__target;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_valid 
        = ((~ ((vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                >> 7U) | ((vlSelfRef.rv_core__DOT__target 
                           >> 1U) & (IData)((0U != 
                                             (0x11000000U 
                                              & vlSelfRef.rv_core__DOT__ex_dec_q[0U])))))) 
           & (IData)(vlSelfRef.rv_core__DOT__ex_valid_q));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__alu_a 
        = vlSelfRef.rv_core__DOT__alu_a;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_a 
        = vlSelfRef.rv_core__DOT__alu_a;
    vlSelfRef.rv_core__DOT__u_alu__DOT__a = vlSelfRef.rv_core__DOT__alu_a;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__b_neg = 
        ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__b_signed) 
         & (vlSelfRef.rv_core__DOT__u_muldiv__DOT__b 
            >> 0x0000001fU));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__alu_b 
        = vlSelfRef.rv_core__DOT__alu_b;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_b 
        = vlSelfRef.rv_core__DOT__alu_b;
    vlSelfRef.rv_core__DOT__u_alu__DOT__b = vlSelfRef.rv_core__DOT__alu_b;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_final 
        = vlSelfRef.rv_core__DOT__mem_final;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__start 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__start;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_fire 
        = vlSelfRef.rv_core__DOT__ex_fire;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__id_dec[0U] 
        = vlSelfRef.rv_core__DOT__id_dec[0U];
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__id_dec[1U] 
        = vlSelfRef.rv_core__DOT__id_dec[1U];
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__id_dec[2U] 
        = vlSelfRef.rv_core__DOT__id_dec[2U];
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_rs1 
        = (0x0000001fU & (vlSelfRef.rv_core__DOT__id_dec[2U] 
                          >> 0x00000017U));
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_rs2 
        = (0x0000001fU & (vlSelfRef.rv_core__DOT__id_dec[2U] 
                          >> 0x00000012U));
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_rd 
        = (0x0000001fU & (vlSelfRef.rv_core__DOT__id_dec[2U] 
                          >> 0x0000000dU));
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_imm 
        = ((vlSelfRef.rv_core__DOT__id_dec[2U] << 0x00000016U) 
           | (vlSelfRef.rv_core__DOT__id_dec[1U] >> 0x0000000aU));
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_illegal 
        = (1U & vlSelfRef.rv_core__DOT__id_dec[0U]);
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
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__b_abs = 
        ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__b_neg)
          ? ((IData)(1U) + (~ vlSelfRef.rv_core__DOT__u_muldiv__DOT__b))
          : vlSelfRef.rv_core__DOT__u_muldiv__DOT__b);
    if ((0x13U == (IData)(vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_opcode))) {
        vlSelfRef.__Vfunc_imm_ref__0__opcode = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_opcode;
        vlSelfRef.__Vfunc_imm_ref__0__instr = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_instr;
        vlSelfRef.__Vfunc_imm_ref__0__i_imm = (((- (IData)(
                                                           (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                            >> 0x1fU))) 
                                                << 0x0000000cU) 
                                               | (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                  >> 0x14U));
        vlSelfRef.__Vfunc_imm_ref__0__s_imm = (((- (IData)(
                                                           (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                            >> 0x1fU))) 
                                                << 0x0000000cU) 
                                               | ((0x00000fe0U 
                                                   & (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                      >> 0x00000014U)) 
                                                  | (0x0000001fU 
                                                     & (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                        >> 7U))));
        vlSelfRef.__Vfunc_imm_ref__0__b_imm = (((- (IData)(
                                                           (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                            >> 0x1fU))) 
                                                << 0x0000000dU) 
                                               | ((((2U 
                                                     & (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                        >> 0x0000001eU)) 
                                                    | (1U 
                                                       & (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                          >> 7U))) 
                                                   << 0x0000000bU) 
                                                  | ((0x000007e0U 
                                                      & (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                         >> 0x00000014U)) 
                                                     | (0x0000001eU 
                                                        & (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                           >> 7U)))));
        vlSelfRef.__Vfunc_imm_ref__0__u_imm = (0xfffff000U 
                                               & vlSelfRef.__Vfunc_imm_ref__0__instr);
        vlSelfRef.__Vfunc_imm_ref__0__j_imm = ((((0x00000ffeU 
                                                  & ((- (IData)(
                                                                (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                                 >> 0x1fU))) 
                                                     << 1U)) 
                                                 | (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                    >> 0x1fU)) 
                                                << 0x00000014U) 
                                               | ((((0x000001feU 
                                                     & (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                        >> 0x0000000bU)) 
                                                    | (1U 
                                                       & (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                          >> 0x14U))) 
                                                   << 0x0000000bU) 
                                                  | (0x000007feU 
                                                     & (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                        >> 0x00000014U))));
        vlSelfRef.__Vfunc_imm_ref__0__Vfuncout = ((0x00000040U 
                                                   & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                   ? 
                                                  ((0x00000020U 
                                                    & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                    ? 
                                                   ((0x00000010U 
                                                     & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                     ? 0U
                                                     : 
                                                    ((8U 
                                                      & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                      ? 
                                                     ((4U 
                                                       & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                       ? 
                                                      ((2U 
                                                        & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                        ? 
                                                       ((1U 
                                                         & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                         ? vlSelfRef.__Vfunc_imm_ref__0__j_imm
                                                         : 0U)
                                                        : 0U)
                                                       : 0U)
                                                      : 
                                                     ((4U 
                                                       & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                       ? 
                                                      ((2U 
                                                        & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                        ? 
                                                       ((1U 
                                                         & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                         ? vlSelfRef.__Vfunc_imm_ref__0__i_imm
                                                         : 0U)
                                                        : 0U)
                                                       : 
                                                      ((2U 
                                                        & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                        ? 
                                                       ((1U 
                                                         & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                         ? vlSelfRef.__Vfunc_imm_ref__0__b_imm
                                                         : 0U)
                                                        : 0U))))
                                                    : 0U)
                                                   : 
                                                  ((0x00000020U 
                                                    & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                    ? 
                                                   ((0x00000010U 
                                                     & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                     ? 
                                                    ((8U 
                                                      & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                      ? 0U
                                                      : 
                                                     ((4U 
                                                       & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                       ? 
                                                      ((2U 
                                                        & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                        ? 
                                                       ((1U 
                                                         & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                         ? vlSelfRef.__Vfunc_imm_ref__0__u_imm
                                                         : 0U)
                                                        : 0U)
                                                       : 0U))
                                                     : 
                                                    ((8U 
                                                      & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                      ? 0U
                                                      : 
                                                     ((4U 
                                                       & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                       ? 0U
                                                       : 
                                                      ((2U 
                                                        & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                        ? 
                                                       ((1U 
                                                         & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                         ? vlSelfRef.__Vfunc_imm_ref__0__s_imm
                                                         : 0U)
                                                        : 0U))))
                                                    : 
                                                   ((0x00000010U 
                                                     & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                     ? 
                                                    ((8U 
                                                      & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                      ? 0U
                                                      : 
                                                     ((4U 
                                                       & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                       ? 
                                                      ((2U 
                                                        & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                        ? 
                                                       ((1U 
                                                         & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                         ? vlSelfRef.__Vfunc_imm_ref__0__u_imm
                                                         : 0U)
                                                        : 0U)
                                                       : 
                                                      ((2U 
                                                        & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                        ? 
                                                       ((1U 
                                                         & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                         ? vlSelfRef.__Vfunc_imm_ref__0__i_imm
                                                         : 0U)
                                                        : 0U)))
                                                     : 
                                                    ((8U 
                                                      & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                      ? 0U
                                                      : 
                                                     ((4U 
                                                       & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                       ? 0U
                                                       : 
                                                      ((2U 
                                                        & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                        ? 
                                                       ((1U 
                                                         & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                         ? vlSelfRef.__Vfunc_imm_ref__0__i_imm
                                                         : 0U)
                                                        : 0U))))));
        vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_b_ref 
            = vlSelfRef.__Vfunc_imm_ref__0__Vfuncout;
    } else {
        vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_b_ref 
            = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_b;
    }
    vlSelfRef.rv_core__DOT__u_alu__DOT__shamt = (0x0000001fU 
                                                 & vlSelfRef.rv_core__DOT__u_alu__DOT__b);
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
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__wr_val 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__wr_val;
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__load_use 
        = vlSelfRef.rv_core__DOT__load_use;
    vlSelfRef.rv_core__DOT__stall_front = ((IData)(vlSelfRef.rv_core__DOT__load_use) 
                                           | (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__alu_result 
        = vlSelfRef.rv_core__DOT__alu_result;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_alu 
        = vlSelfRef.rv_core__DOT__alu_result;
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__stall_front 
        = vlSelfRef.rv_core__DOT__stall_front;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_result 
        = vlSelfRef.rv_core__DOT__ex_result;
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__branch_taken 
        = vlSelfRef.rv_core__DOT__branch_taken;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_taken 
        = vlSelfRef.rv_core__DOT__branch_taken;
    vlSelfRef.rv_core__DOT__flow_change = (((IData)(vlSelfRef.rv_core__DOT__branch_taken) 
                                            & (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                               >> 0x0000001cU)) 
                                           | (0U != 
                                              (3U & 
                                               (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                >> 0x00000017U))));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ls_misaligned 
        = vlSelfRef.rv_core__DOT__ls_misaligned;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__flow_change 
        = vlSelfRef.rv_core__DOT__flow_change;
    vlSelfRef.rv_core__DOT__target_misaligned = ((IData)(vlSelfRef.rv_core__DOT__flow_change) 
                                                 & (vlSelfRef.rv_core__DOT__target 
                                                    >> 1U));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__target_misaligned 
        = vlSelfRef.rv_core__DOT__target_misaligned;
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__exc = vlSelfRef.rv_core__DOT__exc;
    vlSelfRef.rv_core__DOT__trap = ((IData)(vlSelfRef.rv_core__DOT__ex_fire) 
                                    & ((IData)(vlSelfRef.rv_core__DOT__exc) 
                                       | (IData)(vlSelfRef.rv_core__DOT__irq_pending)));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__trap 
        = vlSelfRef.rv_core__DOT__trap;
    vlSelfRef.rv_core__DOT__u_csr__DOT__trap = vlSelfRef.rv_core__DOT__trap;
    rv_core__DOT____Vcellinp__u_csr__commit = ((~ (IData)(vlSelfRef.rv_core__DOT__trap)) 
                                               & (IData)(vlSelfRef.rv_core__DOT__ex_fire));
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__trap 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__trap;
    vlSelfRef.rv_core__DOT__u_csr__DOT__retire = rv_core__DOT____Vcellinp__u_csr__commit;
    vlSelfRef.rv_core__DOT__u_csr__DOT__commit = rv_core__DOT____Vcellinp__u_csr__commit;
    vlSelfRef.rv_core__DOT__mret = ((vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                     >> 1U) & (IData)(rv_core__DOT____Vcellinp__u_csr__commit));
    vlSelfRef.rv_core__DOT__u_csr__DOT__do_write = 
        ((0U != (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op)) 
         & ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__commit) 
            & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__is_write)));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mret 
        = vlSelfRef.rv_core__DOT__mret;
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
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__do_write 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__do_write;
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mret 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__mret;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect 
        = vlSelfRef.rv_core__DOT__redirect;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__redirect 
        = vlSelfRef.rv_core__DOT__redirect;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect_pc 
        = vlSelfRef.rv_core__DOT__redirect_pc;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__redirect_pc 
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
                                                    ((((((((IData)(vlSelfRef.rv_core__DOT__u_top_if__DOT__clk) 
                                                           & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_top_if__DOT__clk__0))) 
                                                          << 3U) 
                                                         | (((IData)(vlSelfRef.rv_core__DOT__u_stage_sva__DOT__clk) 
                                                             & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_stage_sva__DOT__clk__0))) 
                                                            << 2U)) 
                                                        | ((((IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__clk) 
                                                             & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_core_defs__DOT__clk__0))) 
                                                            << 1U) 
                                                           | ((~ (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__rst_n)) 
                                                              & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__rst_n__0)))) 
                                                       << 0x0000000cU) 
                                                      | ((((((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__clk) 
                                                             & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__clk__0))) 
                                                            << 3U) 
                                                           | (((~ (IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__rst_n)) 
                                                               & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_muldiv__DOT__rst_n__0)) 
                                                              << 2U)) 
                                                          | ((((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__clk) 
                                                               & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_muldiv__DOT__clk__0))) 
                                                              << 1U) 
                                                             | ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__clk) 
                                                                & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__clk__0))))) 
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
    if (vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__rst_n) {
        if (vlSymsp->_vm_contextp__->assertOnGet(1, 1)) {
            if (VL_UNLIKELY(((1U & (~ ((~ (((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__done) 
                                            & ((4U 
                                                == (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__op_q)) 
                                               | (5U 
                                                  == (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__op_q)))) 
                                           & (0U == vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__op_b_q))) 
                                       | (0xffffffffU 
                                          == vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__result))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_muldiv_defs.sv:26: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_muldiv.u_muldiv_defs.A_MDU_DIV0", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_muldiv_defs.sv", 26, "");
            }
            if (VL_UNLIKELY(((1U & (~ ((~ (((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__done) 
                                            & ((6U 
                                                == (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__op_q)) 
                                               | (7U 
                                                  == (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__op_q)))) 
                                           & (0U == vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__op_b_q))) 
                                       | (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__result 
                                          == vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__a_ref_q))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_muldiv_defs.sv:22: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_muldiv.u_muldiv_defs.A_MDU_REM0", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_muldiv_defs.sv", 22, "");
            }
        }
    }
}

void Vtop___024root___nba_sequent__TOP__1(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__1\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (vlSelfRef.rv_core__DOT__u_top_if__DOT__rst_n) {
        if (vlSymsp->_vm_contextp__->assertOnGet(1, 1)) {
            if (VL_UNLIKELY(((1U & (~ ((~ (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__imem_valid)) 
                                       | (0U == (3U 
                                                 & vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__imem_addr)))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_top_if.sv:35: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_top_if.A_IMEM_ALIGN", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_top_if.sv", 35, "");
            }
            if (VL_UNLIKELY(((1U & (~ ((~ (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_valid)) 
                                       | (0U == (3U 
                                                 & vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_addr)))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_top_if.sv:39: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_top_if.A_DMEM_ALIGN", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_top_if.sv", 39, "");
            }
            if (VL_UNLIKELY(((1U & (~ ((~ ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_valid) 
                                           & (~ (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_write)))) 
                                       | (0U == (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_wstrb)))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_top_if.sv:31: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_top_if.A_LOAD_WSTRB", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_top_if.sv", 31, "");
            }
        }
    }
    if (vlSymsp->_vm_contextp__->assertOnGet(1, 1)) {
        if (VL_UNLIKELY(((1U & (~ ((~ ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__rst_n) 
                                       & (~ (IData)(vlSelfRef.rv_core__DOT__u_top_if__DOT___Vpast_15_1)))) 
                                   | (~ (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_valid)))))))) {
            VL_WRITEF_NX("[%0t] %%Error: rv_top_if.sv:43: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_top_if.A_RST_DMEM_FIRST", 'T',-9
                         , '#',64,VL_TIME_UNITED_Q(1000));
            VL_STOP_MT("/design/assertions/rv_top_if.sv", 43, "");
        }
    }
    if (vlSelfRef.rv_core__DOT__u_top_if__DOT__rst_n) {
        if (vlSymsp->_vm_contextp__->assertOnGet(1, 1)) {
            if (VL_UNLIKELY(((1U & (~ ((~ ((~ (IData)(vlSelfRef.rv_core__DOT__u_top_if__DOT___Vpast_17_1)) 
                                           & (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__imem_valid))) 
                                       | (0U == vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__imem_addr))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_top_if.sv:47: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_top_if.A_FIRST_FETCH_ZERO", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_top_if.sv", 47, "");
            }
            if (VL_UNLIKELY(((1U & (~ ((~ (IData)(vlSelfRef.rv_core__DOT__u_top_if__DOT___Vpast_9_1)) 
                                       | (vlSelfRef.rv_core__DOT__u_top_if__DOT___Vpast_10_1 
                                          == vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_wdata))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_top_if.sv:27: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_top_if.A_DMEM_WDATA_HOLD", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_top_if.sv", 27, "");
            }
            if (VL_UNLIKELY(((1U & (~ ((~ (IData)(vlSelfRef.rv_core__DOT__u_top_if__DOT___Vpast_1_1)) 
                                       | ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__imem_valid) 
                                          & (vlSelfRef.rv_core__DOT__u_top_if__DOT___Vpast_2_1 
                                             == vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__imem_addr)))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_top_if.sv:18: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_top_if.A_IMEM_HOLD", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_top_if.sv", 18, "");
            }
            if (VL_UNLIKELY(((1U & (~ ((~ (IData)(vlSelfRef.rv_core__DOT__u_top_if__DOT___Vpast_4_1)) 
                                       | ((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_valid) 
                                            & ((IData)(vlSelfRef.rv_core__DOT__u_top_if__DOT___Vpast_5_1) 
                                               == (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_write))) 
                                           & (vlSelfRef.rv_core__DOT__u_top_if__DOT___Vpast_6_1 
                                              == vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_addr)) 
                                          & ((IData)(vlSelfRef.rv_core__DOT__u_top_if__DOT___Vpast_7_1) 
                                             == (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_wstrb))))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_top_if.sv:22: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_top_if.A_DMEM_HOLD", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_top_if.sv", 22, "");
            }
        }
    }
    vlSelfRef.rv_core__DOT__u_top_if__DOT___Vpast_15_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__rst_n;
    vlSelfRef.rv_core__DOT__u_top_if__DOT___Vpast_17_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__imem_valid;
    vlSelfRef.rv_core__DOT__u_top_if__DOT___Vpast_10_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_wdata;
    vlSelfRef.rv_core__DOT__u_top_if__DOT___Vpast_9_1 
        = ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__rst_n) 
           & (((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_valid) 
               & (~ (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_ready))) 
              & (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_write)));
    vlSelfRef.rv_core__DOT__u_top_if__DOT___Vpast_2_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__imem_addr;
    vlSelfRef.rv_core__DOT__u_top_if__DOT___Vpast_1_1 
        = ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__rst_n) 
           & ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__imem_valid) 
              & (~ (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__imem_ready))));
    vlSelfRef.rv_core__DOT__u_top_if__DOT___Vpast_6_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_addr;
    vlSelfRef.rv_core__DOT__u_top_if__DOT___Vpast_7_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_wstrb;
    vlSelfRef.rv_core__DOT__u_top_if__DOT___Vpast_5_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_write;
    vlSelfRef.rv_core__DOT__u_top_if__DOT___Vpast_4_1 
        = ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__rst_n) 
           & ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_valid) 
              & (~ (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_ready))));
}

void Vtop___024root___nba_sequent__TOP__2(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__2\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*6:0*/ __Vfunc_imm_known__1__opcode;
    __Vfunc_imm_known__1__opcode = 0;
    IData/*31:0*/ __Vfunc_imm_ref__2__instr;
    __Vfunc_imm_ref__2__instr = 0;
    CData/*6:0*/ __Vfunc_imm_ref__2__opcode;
    __Vfunc_imm_ref__2__opcode = 0;
    IData/*31:0*/ __Vfunc_imm_ref__2__i_imm;
    __Vfunc_imm_ref__2__i_imm = 0;
    IData/*31:0*/ __Vfunc_imm_ref__2__s_imm;
    __Vfunc_imm_ref__2__s_imm = 0;
    IData/*31:0*/ __Vfunc_imm_ref__2__b_imm;
    __Vfunc_imm_ref__2__b_imm = 0;
    IData/*31:0*/ __Vfunc_imm_ref__2__u_imm;
    __Vfunc_imm_ref__2__u_imm = 0;
    IData/*31:0*/ __Vfunc_imm_ref__2__j_imm;
    __Vfunc_imm_ref__2__j_imm = 0;
    CData/*6:0*/ __Vfunc_alu_known__3__opcode;
    __Vfunc_alu_known__3__opcode = 0;
    CData/*2:0*/ __Vfunc_alu_known__3__funct3;
    __Vfunc_alu_known__3__funct3 = 0;
    CData/*6:0*/ __Vfunc_alu_known__3__funct7;
    __Vfunc_alu_known__3__funct7 = 0;
    CData/*6:0*/ __Vfunc_alu_ref__4__opcode;
    __Vfunc_alu_ref__4__opcode = 0;
    CData/*2:0*/ __Vfunc_alu_ref__4__funct3;
    __Vfunc_alu_ref__4__funct3 = 0;
    CData/*6:0*/ __Vfunc_alu_ref__4__funct7;
    __Vfunc_alu_ref__4__funct7 = 0;
    IData/*31:0*/ __Vfunc_alu_ref__4__a;
    __Vfunc_alu_ref__4__a = 0;
    IData/*31:0*/ __Vfunc_alu_ref__4__b;
    __Vfunc_alu_ref__4__b = 0;
    QData/*63:0*/ __Vfunc_alu_ref__4__sa;
    __Vfunc_alu_ref__4__sa = 0;
    QData/*63:0*/ __Vfunc_alu_ref__4__sb;
    __Vfunc_alu_ref__4__sb = 0;
    QData/*63:0*/ __Vfunc_alu_ref__4__sp;
    __Vfunc_alu_ref__4__sp = 0;
    QData/*63:0*/ __Vfunc_alu_ref__4__hs;
    __Vfunc_alu_ref__4__hs = 0;
    QData/*63:0*/ __Vfunc_alu_ref__4__ua;
    __Vfunc_alu_ref__4__ua = 0;
    QData/*63:0*/ __Vfunc_alu_ref__4__ub;
    __Vfunc_alu_ref__4__ub = 0;
    QData/*63:0*/ __Vfunc_alu_ref__4__up;
    __Vfunc_alu_ref__4__up = 0;
    IData/*31:0*/ __Vfunc_alu_ref__4__sdiv;
    __Vfunc_alu_ref__4__sdiv = 0;
    IData/*31:0*/ __Vfunc_alu_ref__4__smod;
    __Vfunc_alu_ref__4__smod = 0;
    IData/*31:0*/ __Vfunc_alu_ref__4__udiv;
    __Vfunc_alu_ref__4__udiv = 0;
    IData/*31:0*/ __Vfunc_alu_ref__4__umod;
    __Vfunc_alu_ref__4__umod = 0;
    CData/*6:0*/ __Vfunc_is_branch__5__opcode;
    __Vfunc_is_branch__5__opcode = 0;
    CData/*2:0*/ __Vfunc_branch_ref__6__funct3;
    __Vfunc_branch_ref__6__funct3 = 0;
    IData/*31:0*/ __Vfunc_branch_ref__6__a;
    __Vfunc_branch_ref__6__a = 0;
    IData/*31:0*/ __Vfunc_branch_ref__6__b;
    __Vfunc_branch_ref__6__b = 0;
    CData/*6:0*/ __Vfunc_is_branch__7__opcode;
    __Vfunc_is_branch__7__opcode = 0;
    CData/*6:0*/ __Vfunc_is_jal__8__opcode;
    __Vfunc_is_jal__8__opcode = 0;
    IData/*31:0*/ __Vfunc_imm_ref__9__instr;
    __Vfunc_imm_ref__9__instr = 0;
    CData/*6:0*/ __Vfunc_imm_ref__9__opcode;
    __Vfunc_imm_ref__9__opcode = 0;
    IData/*31:0*/ __Vfunc_imm_ref__9__i_imm;
    __Vfunc_imm_ref__9__i_imm = 0;
    IData/*31:0*/ __Vfunc_imm_ref__9__s_imm;
    __Vfunc_imm_ref__9__s_imm = 0;
    IData/*31:0*/ __Vfunc_imm_ref__9__b_imm;
    __Vfunc_imm_ref__9__b_imm = 0;
    IData/*31:0*/ __Vfunc_imm_ref__9__u_imm;
    __Vfunc_imm_ref__9__u_imm = 0;
    IData/*31:0*/ __Vfunc_imm_ref__9__j_imm;
    __Vfunc_imm_ref__9__j_imm = 0;
    CData/*6:0*/ __Vfunc_is_jalr__10__opcode;
    __Vfunc_is_jalr__10__opcode = 0;
    IData/*31:0*/ __Vfunc_imm_ref__11__instr;
    __Vfunc_imm_ref__11__instr = 0;
    CData/*6:0*/ __Vfunc_imm_ref__11__opcode;
    __Vfunc_imm_ref__11__opcode = 0;
    IData/*31:0*/ __Vfunc_imm_ref__11__i_imm;
    __Vfunc_imm_ref__11__i_imm = 0;
    IData/*31:0*/ __Vfunc_imm_ref__11__s_imm;
    __Vfunc_imm_ref__11__s_imm = 0;
    IData/*31:0*/ __Vfunc_imm_ref__11__b_imm;
    __Vfunc_imm_ref__11__b_imm = 0;
    IData/*31:0*/ __Vfunc_imm_ref__11__u_imm;
    __Vfunc_imm_ref__11__u_imm = 0;
    IData/*31:0*/ __Vfunc_imm_ref__11__j_imm;
    __Vfunc_imm_ref__11__j_imm = 0;
    CData/*1:0*/ __Vfunc_load_ref__21__addr;
    __Vfunc_load_ref__21__addr = 0;
    CData/*7:0*/ __Vfunc_load_ref__21__b;
    __Vfunc_load_ref__21__b = 0;
    // Body
    if (vlSelfRef.rv_core__DOT__u_stage_sva__DOT__rst_n) {
        if (vlSymsp->_vm_contextp__->assertOnGet(1, 1)) {
            if (VL_UNLIKELY(((1U & (~ ((~ (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_valid)) 
                                       | (0U == (3U 
                                                 & vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_pc)))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_stage_sva.sv:338: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_stage_sva.g_align.A_ID_PC_ALIGN", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_stage_sva.sv", 338, "");
            }
            if (VL_UNLIKELY(((1U & (~ ((~ (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_valid)) 
                                       | ((((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_rs1) 
                                              == (0x0000001fU 
                                                  & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_instr 
                                                     >> 0x0fU))) 
                                             & ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_rs2) 
                                                == 
                                                (0x0000001fU 
                                                 & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_instr 
                                                    >> 0x14U)))) 
                                            & ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_rd) 
                                               == (0x0000001fU 
                                                   & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_instr 
                                                      >> 7U)))) 
                                           & ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_funct3) 
                                              == (7U 
                                                  & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_instr 
                                                     >> 0x0cU)))) 
                                          & ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_opcode) 
                                             == (0x0000007fU 
                                                 & vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_instr))))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_stage_sva.sv:180: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_stage_sva.A_DEC_FIELDS", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_stage_sva.sv", 180, "");
            }
            __Vfunc_load_ref__21__addr = (3U & vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__mem_addr_isa);
            __Vfunc_load_ref__21__b = 0U;
            __Vfunc_is_branch__5__opcode = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_opcode;
            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_4__is_branch 
                = (0x63U == (IData)(__Vfunc_is_branch__5__opcode));
            __Vfunc_branch_ref__6__b = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_b;
            __Vfunc_branch_ref__6__a = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_a;
            __Vfunc_branch_ref__6__funct3 = (7U & (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_funct3));
            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_5__branch_ref 
                = ((4U & (IData)(__Vfunc_branch_ref__6__funct3))
                    ? ((2U & (IData)(__Vfunc_branch_ref__6__funct3))
                        ? ((1U & (IData)(__Vfunc_branch_ref__6__funct3))
                            ? (__Vfunc_branch_ref__6__a 
                               >= __Vfunc_branch_ref__6__b)
                            : (__Vfunc_branch_ref__6__a 
                               < __Vfunc_branch_ref__6__b))
                        : ((1U & (IData)(__Vfunc_branch_ref__6__funct3))
                            ? VL_GTES_III(32, __Vfunc_branch_ref__6__a, __Vfunc_branch_ref__6__b)
                            : VL_LTS_III(32, __Vfunc_branch_ref__6__a, __Vfunc_branch_ref__6__b)))
                    : ((1U & (~ ((IData)(__Vfunc_branch_ref__6__funct3) 
                                 >> 1U))) && ((1U & (IData)(__Vfunc_branch_ref__6__funct3))
                                               ? (__Vfunc_branch_ref__6__a 
                                                  != __Vfunc_branch_ref__6__b)
                                               : (__Vfunc_branch_ref__6__a 
                                                  == __Vfunc_branch_ref__6__b))));
            if (VL_UNLIKELY(((1U & (~ ((~ ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_valid) 
                                           & (IData)(vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_4__is_branch))) 
                                       | ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_taken) 
                                          == (IData)(vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_5__branch_ref)))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_stage_sva.sv:223: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_stage_sva.A_EX_BRANCH", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_stage_sva.sv", 223, "");
            }
            __Vfunc_imm_known__1__opcode = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_opcode;
            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_0__imm_known 
                = ((0x00000040U & (IData)(__Vfunc_imm_known__1__opcode))
                    ? ((1U & ((IData)(__Vfunc_imm_known__1__opcode) 
                              >> 5U)) && ((1U & (~ 
                                                 ((IData)(__Vfunc_imm_known__1__opcode) 
                                                  >> 4U))) 
                                          && ((8U & (IData)(__Vfunc_imm_known__1__opcode))
                                               ? ((1U 
                                                   & ((IData)(__Vfunc_imm_known__1__opcode) 
                                                      >> 2U)) 
                                                  && ((1U 
                                                       & ((IData)(__Vfunc_imm_known__1__opcode) 
                                                          >> 1U)) 
                                                      && (1U 
                                                          & (IData)(__Vfunc_imm_known__1__opcode))))
                                               : ((1U 
                                                   & ((IData)(__Vfunc_imm_known__1__opcode) 
                                                      >> 1U)) 
                                                  && (1U 
                                                      & (IData)(__Vfunc_imm_known__1__opcode))))))
                    : ((0x00000020U & (IData)(__Vfunc_imm_known__1__opcode))
                        ? ((0x00000010U & (IData)(__Vfunc_imm_known__1__opcode))
                            ? ((1U & (~ ((IData)(__Vfunc_imm_known__1__opcode) 
                                         >> 3U))) && 
                               ((1U & ((IData)(__Vfunc_imm_known__1__opcode) 
                                       >> 2U)) && (
                                                   (1U 
                                                    & ((IData)(__Vfunc_imm_known__1__opcode) 
                                                       >> 1U)) 
                                                   && (1U 
                                                       & (IData)(__Vfunc_imm_known__1__opcode)))))
                            : ((1U & (~ ((IData)(__Vfunc_imm_known__1__opcode) 
                                         >> 3U))) && 
                               ((1U & (~ ((IData)(__Vfunc_imm_known__1__opcode) 
                                          >> 2U))) 
                                && ((1U & ((IData)(__Vfunc_imm_known__1__opcode) 
                                           >> 1U)) 
                                    && (1U & (IData)(__Vfunc_imm_known__1__opcode))))))
                        : ((0x00000010U & (IData)(__Vfunc_imm_known__1__opcode))
                            ? ((1U & (~ ((IData)(__Vfunc_imm_known__1__opcode) 
                                         >> 3U))) && 
                               ((1U & ((IData)(__Vfunc_imm_known__1__opcode) 
                                       >> 1U)) && (1U 
                                                   & (IData)(__Vfunc_imm_known__1__opcode))))
                            : ((1U & (~ ((IData)(__Vfunc_imm_known__1__opcode) 
                                         >> 3U))) && 
                               ((1U & (~ ((IData)(__Vfunc_imm_known__1__opcode) 
                                          >> 2U))) 
                                && ((1U & ((IData)(__Vfunc_imm_known__1__opcode) 
                                           >> 1U)) 
                                    && (1U & (IData)(__Vfunc_imm_known__1__opcode))))))));
            __Vfunc_imm_ref__2__opcode = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_opcode;
            __Vfunc_imm_ref__2__instr = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_instr;
            __Vfunc_imm_ref__2__i_imm = (((- (IData)(
                                                     (__Vfunc_imm_ref__2__instr 
                                                      >> 0x1fU))) 
                                          << 0x0000000cU) 
                                         | (__Vfunc_imm_ref__2__instr 
                                            >> 0x14U));
            __Vfunc_imm_ref__2__s_imm = (((- (IData)(
                                                     (__Vfunc_imm_ref__2__instr 
                                                      >> 0x1fU))) 
                                          << 0x0000000cU) 
                                         | ((0x00000fe0U 
                                             & (__Vfunc_imm_ref__2__instr 
                                                >> 0x00000014U)) 
                                            | (0x0000001fU 
                                               & (__Vfunc_imm_ref__2__instr 
                                                  >> 7U))));
            __Vfunc_imm_ref__2__b_imm = (((- (IData)(
                                                     (__Vfunc_imm_ref__2__instr 
                                                      >> 0x1fU))) 
                                          << 0x0000000dU) 
                                         | ((((2U & 
                                               (__Vfunc_imm_ref__2__instr 
                                                >> 0x0000001eU)) 
                                              | (1U 
                                                 & (__Vfunc_imm_ref__2__instr 
                                                    >> 7U))) 
                                             << 0x0000000bU) 
                                            | ((0x000007e0U 
                                                & (__Vfunc_imm_ref__2__instr 
                                                   >> 0x00000014U)) 
                                               | (0x0000001eU 
                                                  & (__Vfunc_imm_ref__2__instr 
                                                     >> 7U)))));
            __Vfunc_imm_ref__2__u_imm = (0xfffff000U 
                                         & __Vfunc_imm_ref__2__instr);
            __Vfunc_imm_ref__2__j_imm = ((((0x00000ffeU 
                                            & ((- (IData)(
                                                          (__Vfunc_imm_ref__2__instr 
                                                           >> 0x1fU))) 
                                               << 1U)) 
                                           | (__Vfunc_imm_ref__2__instr 
                                              >> 0x1fU)) 
                                          << 0x00000014U) 
                                         | ((((0x000001feU 
                                               & (__Vfunc_imm_ref__2__instr 
                                                  >> 0x0000000bU)) 
                                              | (1U 
                                                 & (__Vfunc_imm_ref__2__instr 
                                                    >> 0x14U))) 
                                             << 0x0000000bU) 
                                            | (0x000007feU 
                                               & (__Vfunc_imm_ref__2__instr 
                                                  >> 0x00000014U))));
            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_1__imm_ref 
                = ((0x00000040U & (IData)(__Vfunc_imm_ref__2__opcode))
                    ? ((0x00000020U & (IData)(__Vfunc_imm_ref__2__opcode))
                        ? ((0x00000010U & (IData)(__Vfunc_imm_ref__2__opcode))
                            ? 0U : ((8U & (IData)(__Vfunc_imm_ref__2__opcode))
                                     ? ((4U & (IData)(__Vfunc_imm_ref__2__opcode))
                                         ? ((2U & (IData)(__Vfunc_imm_ref__2__opcode))
                                             ? ((1U 
                                                 & (IData)(__Vfunc_imm_ref__2__opcode))
                                                 ? __Vfunc_imm_ref__2__j_imm
                                                 : 0U)
                                             : 0U) : 0U)
                                     : ((4U & (IData)(__Vfunc_imm_ref__2__opcode))
                                         ? ((2U & (IData)(__Vfunc_imm_ref__2__opcode))
                                             ? ((1U 
                                                 & (IData)(__Vfunc_imm_ref__2__opcode))
                                                 ? __Vfunc_imm_ref__2__i_imm
                                                 : 0U)
                                             : 0U) : 
                                        ((2U & (IData)(__Vfunc_imm_ref__2__opcode))
                                          ? ((1U & (IData)(__Vfunc_imm_ref__2__opcode))
                                              ? __Vfunc_imm_ref__2__b_imm
                                              : 0U)
                                          : 0U)))) : 0U)
                    : ((0x00000020U & (IData)(__Vfunc_imm_ref__2__opcode))
                        ? ((0x00000010U & (IData)(__Vfunc_imm_ref__2__opcode))
                            ? ((8U & (IData)(__Vfunc_imm_ref__2__opcode))
                                ? 0U : ((4U & (IData)(__Vfunc_imm_ref__2__opcode))
                                         ? ((2U & (IData)(__Vfunc_imm_ref__2__opcode))
                                             ? ((1U 
                                                 & (IData)(__Vfunc_imm_ref__2__opcode))
                                                 ? __Vfunc_imm_ref__2__u_imm
                                                 : 0U)
                                             : 0U) : 0U))
                            : ((8U & (IData)(__Vfunc_imm_ref__2__opcode))
                                ? 0U : ((4U & (IData)(__Vfunc_imm_ref__2__opcode))
                                         ? 0U : ((2U 
                                                  & (IData)(__Vfunc_imm_ref__2__opcode))
                                                  ? 
                                                 ((1U 
                                                   & (IData)(__Vfunc_imm_ref__2__opcode))
                                                   ? __Vfunc_imm_ref__2__s_imm
                                                   : 0U)
                                                  : 0U))))
                        : ((0x00000010U & (IData)(__Vfunc_imm_ref__2__opcode))
                            ? ((8U & (IData)(__Vfunc_imm_ref__2__opcode))
                                ? 0U : ((4U & (IData)(__Vfunc_imm_ref__2__opcode))
                                         ? ((2U & (IData)(__Vfunc_imm_ref__2__opcode))
                                             ? ((1U 
                                                 & (IData)(__Vfunc_imm_ref__2__opcode))
                                                 ? __Vfunc_imm_ref__2__u_imm
                                                 : 0U)
                                             : 0U) : 
                                        ((2U & (IData)(__Vfunc_imm_ref__2__opcode))
                                          ? ((1U & (IData)(__Vfunc_imm_ref__2__opcode))
                                              ? __Vfunc_imm_ref__2__i_imm
                                              : 0U)
                                          : 0U))) : 
                           ((8U & (IData)(__Vfunc_imm_ref__2__opcode))
                             ? 0U : ((4U & (IData)(__Vfunc_imm_ref__2__opcode))
                                      ? 0U : ((2U & (IData)(__Vfunc_imm_ref__2__opcode))
                                               ? ((1U 
                                                   & (IData)(__Vfunc_imm_ref__2__opcode))
                                                   ? __Vfunc_imm_ref__2__i_imm
                                                   : 0U)
                                               : 0U))))));
            if (VL_UNLIKELY(((1U & (~ ((~ ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_valid) 
                                           & (IData)(vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_0__imm_known))) 
                                       | (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_imm 
                                          == vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_1__imm_ref))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_stage_sva.sv:189: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_stage_sva.A_DEC_IMM", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_stage_sva.sv", 189, "");
            }
            __Vfunc_is_jalr__10__opcode = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_opcode;
            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_9__is_jalr 
                = (0x67U == (IData)(__Vfunc_is_jalr__10__opcode));
            __Vfunc_imm_ref__11__opcode = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_opcode;
            __Vfunc_imm_ref__11__instr = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_instr;
            __Vfunc_imm_ref__11__i_imm = (((- (IData)(
                                                      (__Vfunc_imm_ref__11__instr 
                                                       >> 0x1fU))) 
                                           << 0x0000000cU) 
                                          | (__Vfunc_imm_ref__11__instr 
                                             >> 0x14U));
            __Vfunc_imm_ref__11__s_imm = (((- (IData)(
                                                      (__Vfunc_imm_ref__11__instr 
                                                       >> 0x1fU))) 
                                           << 0x0000000cU) 
                                          | ((0x00000fe0U 
                                              & (__Vfunc_imm_ref__11__instr 
                                                 >> 0x00000014U)) 
                                             | (0x0000001fU 
                                                & (__Vfunc_imm_ref__11__instr 
                                                   >> 7U))));
            __Vfunc_imm_ref__11__b_imm = (((- (IData)(
                                                      (__Vfunc_imm_ref__11__instr 
                                                       >> 0x1fU))) 
                                           << 0x0000000dU) 
                                          | ((((2U 
                                                & (__Vfunc_imm_ref__11__instr 
                                                   >> 0x0000001eU)) 
                                               | (1U 
                                                  & (__Vfunc_imm_ref__11__instr 
                                                     >> 7U))) 
                                              << 0x0000000bU) 
                                             | ((0x000007e0U 
                                                 & (__Vfunc_imm_ref__11__instr 
                                                    >> 0x00000014U)) 
                                                | (0x0000001eU 
                                                   & (__Vfunc_imm_ref__11__instr 
                                                      >> 7U)))));
            __Vfunc_imm_ref__11__u_imm = (0xfffff000U 
                                          & __Vfunc_imm_ref__11__instr);
            __Vfunc_imm_ref__11__j_imm = ((((0x00000ffeU 
                                             & ((- (IData)(
                                                           (__Vfunc_imm_ref__11__instr 
                                                            >> 0x1fU))) 
                                                << 1U)) 
                                            | (__Vfunc_imm_ref__11__instr 
                                               >> 0x1fU)) 
                                           << 0x00000014U) 
                                          | ((((0x000001feU 
                                                & (__Vfunc_imm_ref__11__instr 
                                                   >> 0x0000000bU)) 
                                               | (1U 
                                                  & (__Vfunc_imm_ref__11__instr 
                                                     >> 0x14U))) 
                                              << 0x0000000bU) 
                                             | (0x000007feU 
                                                & (__Vfunc_imm_ref__11__instr 
                                                   >> 0x00000014U))));
            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_10__imm_ref 
                = ((0x00000040U & (IData)(__Vfunc_imm_ref__11__opcode))
                    ? ((0x00000020U & (IData)(__Vfunc_imm_ref__11__opcode))
                        ? ((0x00000010U & (IData)(__Vfunc_imm_ref__11__opcode))
                            ? 0U : ((8U & (IData)(__Vfunc_imm_ref__11__opcode))
                                     ? ((4U & (IData)(__Vfunc_imm_ref__11__opcode))
                                         ? ((2U & (IData)(__Vfunc_imm_ref__11__opcode))
                                             ? ((1U 
                                                 & (IData)(__Vfunc_imm_ref__11__opcode))
                                                 ? __Vfunc_imm_ref__11__j_imm
                                                 : 0U)
                                             : 0U) : 0U)
                                     : ((4U & (IData)(__Vfunc_imm_ref__11__opcode))
                                         ? ((2U & (IData)(__Vfunc_imm_ref__11__opcode))
                                             ? ((1U 
                                                 & (IData)(__Vfunc_imm_ref__11__opcode))
                                                 ? __Vfunc_imm_ref__11__i_imm
                                                 : 0U)
                                             : 0U) : 
                                        ((2U & (IData)(__Vfunc_imm_ref__11__opcode))
                                          ? ((1U & (IData)(__Vfunc_imm_ref__11__opcode))
                                              ? __Vfunc_imm_ref__11__b_imm
                                              : 0U)
                                          : 0U)))) : 0U)
                    : ((0x00000020U & (IData)(__Vfunc_imm_ref__11__opcode))
                        ? ((0x00000010U & (IData)(__Vfunc_imm_ref__11__opcode))
                            ? ((8U & (IData)(__Vfunc_imm_ref__11__opcode))
                                ? 0U : ((4U & (IData)(__Vfunc_imm_ref__11__opcode))
                                         ? ((2U & (IData)(__Vfunc_imm_ref__11__opcode))
                                             ? ((1U 
                                                 & (IData)(__Vfunc_imm_ref__11__opcode))
                                                 ? __Vfunc_imm_ref__11__u_imm
                                                 : 0U)
                                             : 0U) : 0U))
                            : ((8U & (IData)(__Vfunc_imm_ref__11__opcode))
                                ? 0U : ((4U & (IData)(__Vfunc_imm_ref__11__opcode))
                                         ? 0U : ((2U 
                                                  & (IData)(__Vfunc_imm_ref__11__opcode))
                                                  ? 
                                                 ((1U 
                                                   & (IData)(__Vfunc_imm_ref__11__opcode))
                                                   ? __Vfunc_imm_ref__11__s_imm
                                                   : 0U)
                                                  : 0U))))
                        : ((0x00000010U & (IData)(__Vfunc_imm_ref__11__opcode))
                            ? ((8U & (IData)(__Vfunc_imm_ref__11__opcode))
                                ? 0U : ((4U & (IData)(__Vfunc_imm_ref__11__opcode))
                                         ? ((2U & (IData)(__Vfunc_imm_ref__11__opcode))
                                             ? ((1U 
                                                 & (IData)(__Vfunc_imm_ref__11__opcode))
                                                 ? __Vfunc_imm_ref__11__u_imm
                                                 : 0U)
                                             : 0U) : 
                                        ((2U & (IData)(__Vfunc_imm_ref__11__opcode))
                                          ? ((1U & (IData)(__Vfunc_imm_ref__11__opcode))
                                              ? __Vfunc_imm_ref__11__i_imm
                                              : 0U)
                                          : 0U))) : 
                           ((8U & (IData)(__Vfunc_imm_ref__11__opcode))
                             ? 0U : ((4U & (IData)(__Vfunc_imm_ref__11__opcode))
                                      ? 0U : ((2U & (IData)(__Vfunc_imm_ref__11__opcode))
                                               ? ((1U 
                                                   & (IData)(__Vfunc_imm_ref__11__opcode))
                                                   ? __Vfunc_imm_ref__11__i_imm
                                                   : 0U)
                                               : 0U))))));
            if (VL_UNLIKELY(((1U & (~ ((~ ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_valid) 
                                           & (IData)(vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_9__is_jalr))) 
                                       | (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_target 
                                          == (0xfffffffeU 
                                              & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_a 
                                                 + vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_10__imm_ref))))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_stage_sva.sv:241: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_stage_sva.A_EX_TARGET_JALR", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_stage_sva.sv", 241, "");
            }
            __Vfunc_is_branch__7__opcode = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_opcode;
            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_6__is_branch 
                = (0x63U == (IData)(__Vfunc_is_branch__7__opcode));
            __Vfunc_is_jal__8__opcode = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_opcode;
            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_7__is_jal 
                = (0x6fU == (IData)(__Vfunc_is_jal__8__opcode));
            __Vfunc_imm_ref__9__opcode = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_opcode;
            __Vfunc_imm_ref__9__instr = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_instr;
            __Vfunc_imm_ref__9__i_imm = (((- (IData)(
                                                     (__Vfunc_imm_ref__9__instr 
                                                      >> 0x1fU))) 
                                          << 0x0000000cU) 
                                         | (__Vfunc_imm_ref__9__instr 
                                            >> 0x14U));
            __Vfunc_imm_ref__9__s_imm = (((- (IData)(
                                                     (__Vfunc_imm_ref__9__instr 
                                                      >> 0x1fU))) 
                                          << 0x0000000cU) 
                                         | ((0x00000fe0U 
                                             & (__Vfunc_imm_ref__9__instr 
                                                >> 0x00000014U)) 
                                            | (0x0000001fU 
                                               & (__Vfunc_imm_ref__9__instr 
                                                  >> 7U))));
            __Vfunc_imm_ref__9__b_imm = (((- (IData)(
                                                     (__Vfunc_imm_ref__9__instr 
                                                      >> 0x1fU))) 
                                          << 0x0000000dU) 
                                         | ((((2U & 
                                               (__Vfunc_imm_ref__9__instr 
                                                >> 0x0000001eU)) 
                                              | (1U 
                                                 & (__Vfunc_imm_ref__9__instr 
                                                    >> 7U))) 
                                             << 0x0000000bU) 
                                            | ((0x000007e0U 
                                                & (__Vfunc_imm_ref__9__instr 
                                                   >> 0x00000014U)) 
                                               | (0x0000001eU 
                                                  & (__Vfunc_imm_ref__9__instr 
                                                     >> 7U)))));
            __Vfunc_imm_ref__9__u_imm = (0xfffff000U 
                                         & __Vfunc_imm_ref__9__instr);
            __Vfunc_imm_ref__9__j_imm = ((((0x00000ffeU 
                                            & ((- (IData)(
                                                          (__Vfunc_imm_ref__9__instr 
                                                           >> 0x1fU))) 
                                               << 1U)) 
                                           | (__Vfunc_imm_ref__9__instr 
                                              >> 0x1fU)) 
                                          << 0x00000014U) 
                                         | ((((0x000001feU 
                                               & (__Vfunc_imm_ref__9__instr 
                                                  >> 0x0000000bU)) 
                                              | (1U 
                                                 & (__Vfunc_imm_ref__9__instr 
                                                    >> 0x14U))) 
                                             << 0x0000000bU) 
                                            | (0x000007feU 
                                               & (__Vfunc_imm_ref__9__instr 
                                                  >> 0x00000014U))));
            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_8__imm_ref 
                = ((0x00000040U & (IData)(__Vfunc_imm_ref__9__opcode))
                    ? ((0x00000020U & (IData)(__Vfunc_imm_ref__9__opcode))
                        ? ((0x00000010U & (IData)(__Vfunc_imm_ref__9__opcode))
                            ? 0U : ((8U & (IData)(__Vfunc_imm_ref__9__opcode))
                                     ? ((4U & (IData)(__Vfunc_imm_ref__9__opcode))
                                         ? ((2U & (IData)(__Vfunc_imm_ref__9__opcode))
                                             ? ((1U 
                                                 & (IData)(__Vfunc_imm_ref__9__opcode))
                                                 ? __Vfunc_imm_ref__9__j_imm
                                                 : 0U)
                                             : 0U) : 0U)
                                     : ((4U & (IData)(__Vfunc_imm_ref__9__opcode))
                                         ? ((2U & (IData)(__Vfunc_imm_ref__9__opcode))
                                             ? ((1U 
                                                 & (IData)(__Vfunc_imm_ref__9__opcode))
                                                 ? __Vfunc_imm_ref__9__i_imm
                                                 : 0U)
                                             : 0U) : 
                                        ((2U & (IData)(__Vfunc_imm_ref__9__opcode))
                                          ? ((1U & (IData)(__Vfunc_imm_ref__9__opcode))
                                              ? __Vfunc_imm_ref__9__b_imm
                                              : 0U)
                                          : 0U)))) : 0U)
                    : ((0x00000020U & (IData)(__Vfunc_imm_ref__9__opcode))
                        ? ((0x00000010U & (IData)(__Vfunc_imm_ref__9__opcode))
                            ? ((8U & (IData)(__Vfunc_imm_ref__9__opcode))
                                ? 0U : ((4U & (IData)(__Vfunc_imm_ref__9__opcode))
                                         ? ((2U & (IData)(__Vfunc_imm_ref__9__opcode))
                                             ? ((1U 
                                                 & (IData)(__Vfunc_imm_ref__9__opcode))
                                                 ? __Vfunc_imm_ref__9__u_imm
                                                 : 0U)
                                             : 0U) : 0U))
                            : ((8U & (IData)(__Vfunc_imm_ref__9__opcode))
                                ? 0U : ((4U & (IData)(__Vfunc_imm_ref__9__opcode))
                                         ? 0U : ((2U 
                                                  & (IData)(__Vfunc_imm_ref__9__opcode))
                                                  ? 
                                                 ((1U 
                                                   & (IData)(__Vfunc_imm_ref__9__opcode))
                                                   ? __Vfunc_imm_ref__9__s_imm
                                                   : 0U)
                                                  : 0U))))
                        : ((0x00000010U & (IData)(__Vfunc_imm_ref__9__opcode))
                            ? ((8U & (IData)(__Vfunc_imm_ref__9__opcode))
                                ? 0U : ((4U & (IData)(__Vfunc_imm_ref__9__opcode))
                                         ? ((2U & (IData)(__Vfunc_imm_ref__9__opcode))
                                             ? ((1U 
                                                 & (IData)(__Vfunc_imm_ref__9__opcode))
                                                 ? __Vfunc_imm_ref__9__u_imm
                                                 : 0U)
                                             : 0U) : 
                                        ((2U & (IData)(__Vfunc_imm_ref__9__opcode))
                                          ? ((1U & (IData)(__Vfunc_imm_ref__9__opcode))
                                              ? __Vfunc_imm_ref__9__i_imm
                                              : 0U)
                                          : 0U))) : 
                           ((8U & (IData)(__Vfunc_imm_ref__9__opcode))
                             ? 0U : ((4U & (IData)(__Vfunc_imm_ref__9__opcode))
                                      ? 0U : ((2U & (IData)(__Vfunc_imm_ref__9__opcode))
                                               ? ((1U 
                                                   & (IData)(__Vfunc_imm_ref__9__opcode))
                                                   ? __Vfunc_imm_ref__9__i_imm
                                                   : 0U)
                                               : 0U))))));
            if (VL_UNLIKELY(((1U & (~ ((~ ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_valid) 
                                           & ((IData)(vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_6__is_branch) 
                                              | (IData)(vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_7__is_jal)))) 
                                       | ((vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_target 
                                           == (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_pc 
                                               + vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_8__imm_ref)) 
                                          & (0U == 
                                             (3U & vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_target))))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_stage_sva.sv:234: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_stage_sva.A_EX_TARGET_BRANCH", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_stage_sva.sv", 234, "");
            }
        }
    }
    if (vlSelfRef.rv_core__DOT__u_stage_sva__DOT__rst_n) {
        if (vlSymsp->_vm_contextp__->assertOnGet(1, 1)) {
            __Vfunc_alu_known__3__funct7 = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_funct7;
            __Vfunc_alu_known__3__funct3 = (7U & (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_funct3));
            __Vfunc_alu_known__3__opcode = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_opcode;
            {
                vlSelf->rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known = 0;
                if ((0x13U == (IData)(__Vfunc_alu_known__3__opcode))) {
                    if (((1U == (IData)(__Vfunc_alu_known__3__funct3)) 
                         | (5U == (IData)(__Vfunc_alu_known__3__funct3)))) {
                        vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known 
                            = ((0U == (IData)(__Vfunc_alu_known__3__funct7)) 
                               | (0x20U == (IData)(__Vfunc_alu_known__3__funct7)));
                        goto __Vlabel0;
                    }
                    vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known = 1U;
                    goto __Vlabel0;
                }
                if ((0x33U == (IData)(__Vfunc_alu_known__3__opcode))) {
                    if ((0x00000040U & (IData)(__Vfunc_alu_known__3__funct7))) {
                        vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known = 0U;
                        goto __Vlabel0;
                    } else if ((0x00000020U & (IData)(__Vfunc_alu_known__3__funct7))) {
                        if ((0x00000010U & (IData)(__Vfunc_alu_known__3__funct7))) {
                            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known = 0U;
                            goto __Vlabel0;
                        } else if ((8U & (IData)(__Vfunc_alu_known__3__funct7))) {
                            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known = 0U;
                            goto __Vlabel0;
                        } else if ((4U & (IData)(__Vfunc_alu_known__3__funct7))) {
                            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known = 0U;
                            goto __Vlabel0;
                        } else if ((2U & (IData)(__Vfunc_alu_known__3__funct7))) {
                            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known = 0U;
                            goto __Vlabel0;
                        } else if ((1U & (IData)(__Vfunc_alu_known__3__funct7))) {
                            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known = 0U;
                            goto __Vlabel0;
                        } else {
                            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known 
                                = ((0U == (IData)(__Vfunc_alu_known__3__funct3)) 
                                   | (5U == (IData)(__Vfunc_alu_known__3__funct3)));
                            goto __Vlabel0;
                        }
                    } else if ((0x00000010U & (IData)(__Vfunc_alu_known__3__funct7))) {
                        if ((8U & (IData)(__Vfunc_alu_known__3__funct7))) {
                            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known = 0U;
                            goto __Vlabel0;
                        } else if ((4U & (IData)(__Vfunc_alu_known__3__funct7))) {
                            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known = 0U;
                            goto __Vlabel0;
                        } else if ((2U & (IData)(__Vfunc_alu_known__3__funct7))) {
                            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known = 0U;
                            goto __Vlabel0;
                        } else if ((1U & (IData)(__Vfunc_alu_known__3__funct7))) {
                            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known = 0U;
                            goto __Vlabel0;
                        } else {
                            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known 
                                = (((2U == (IData)(__Vfunc_alu_known__3__funct3)) 
                                    | (4U == (IData)(__Vfunc_alu_known__3__funct3))) 
                                   | (6U == (IData)(__Vfunc_alu_known__3__funct3)));
                            goto __Vlabel0;
                        }
                    } else if ((8U & (IData)(__Vfunc_alu_known__3__funct7))) {
                        vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known = 0U;
                        goto __Vlabel0;
                    } else if ((4U & (IData)(__Vfunc_alu_known__3__funct7))) {
                        if ((2U & (IData)(__Vfunc_alu_known__3__funct7))) {
                            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known = 0U;
                            goto __Vlabel0;
                        } else if ((1U & (IData)(__Vfunc_alu_known__3__funct7))) {
                            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known 
                                = ((((4U == (IData)(__Vfunc_alu_known__3__funct3)) 
                                     | (5U == (IData)(__Vfunc_alu_known__3__funct3))) 
                                    | (6U == (IData)(__Vfunc_alu_known__3__funct3))) 
                                   | (7U == (IData)(__Vfunc_alu_known__3__funct3)));
                            goto __Vlabel0;
                        } else {
                            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known = 0U;
                            goto __Vlabel0;
                        }
                    } else if ((2U & (IData)(__Vfunc_alu_known__3__funct7))) {
                        vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known = 0U;
                        goto __Vlabel0;
                    } else if ((1U & (IData)(__Vfunc_alu_known__3__funct7))) {
                        vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known = 1U;
                        goto __Vlabel0;
                    } else {
                        vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known = 1U;
                        goto __Vlabel0;
                    }
                }
                vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known = 0U;
                __Vlabel0: ;
            }
            __Vfunc_alu_ref__4__b = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_b_ref;
            __Vfunc_alu_ref__4__a = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_a;
            __Vfunc_alu_ref__4__funct7 = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_funct7;
            __Vfunc_alu_ref__4__funct3 = (7U & (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_funct3));
            __Vfunc_alu_ref__4__opcode = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_opcode;
            {
                vlSelf->rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref = 0;
                __Vfunc_alu_ref__4__sa = 0;
                __Vfunc_alu_ref__4__sb = 0;
                __Vfunc_alu_ref__4__sp = 0;
                __Vfunc_alu_ref__4__hs = 0;
                __Vfunc_alu_ref__4__ua = 0;
                __Vfunc_alu_ref__4__ub = 0;
                __Vfunc_alu_ref__4__up = 0;
                __Vfunc_alu_ref__4__sdiv = 0;
                __Vfunc_alu_ref__4__smod = 0;
                __Vfunc_alu_ref__4__udiv = 0;
                __Vfunc_alu_ref__4__umod = 0;
                __Vfunc_alu_ref__4__sa = VL_EXTENDS_QI(64,32, __Vfunc_alu_ref__4__a);
                __Vfunc_alu_ref__4__sb = VL_EXTENDS_QI(64,32, __Vfunc_alu_ref__4__b);
                __Vfunc_alu_ref__4__ua = (QData)((IData)(__Vfunc_alu_ref__4__a));
                __Vfunc_alu_ref__4__ub = (QData)((IData)(__Vfunc_alu_ref__4__b));
                if ((0x13U == (IData)(__Vfunc_alu_ref__4__opcode))) {
                    if ((4U & (IData)(__Vfunc_alu_ref__4__funct3))) {
                        if ((2U & (IData)(__Vfunc_alu_ref__4__funct3))) {
                            if ((1U & (IData)(__Vfunc_alu_ref__4__funct3))) {
                                vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref 
                                    = (__Vfunc_alu_ref__4__a 
                                       & __Vfunc_alu_ref__4__b);
                                goto __Vlabel1;
                            } else {
                                vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref 
                                    = (__Vfunc_alu_ref__4__a 
                                       | __Vfunc_alu_ref__4__b);
                                goto __Vlabel1;
                            }
                        } else if ((1U & (IData)(__Vfunc_alu_ref__4__funct3))) {
                            if ((0x00000020U & (IData)(__Vfunc_alu_ref__4__funct7))) {
                                vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref 
                                    = VL_SHIFTRS_III(32,32,5, __Vfunc_alu_ref__4__a, 
                                                     (0x0000001fU 
                                                      & __Vfunc_alu_ref__4__b));
                                goto __Vlabel1;
                            } else {
                                vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref 
                                    = (__Vfunc_alu_ref__4__a 
                                       >> (0x0000001fU 
                                           & __Vfunc_alu_ref__4__b));
                                goto __Vlabel1;
                            }
                        } else {
                            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref 
                                = (__Vfunc_alu_ref__4__a 
                                   ^ __Vfunc_alu_ref__4__b);
                            goto __Vlabel1;
                        }
                    } else if ((2U & (IData)(__Vfunc_alu_ref__4__funct3))) {
                        if ((1U & (IData)(__Vfunc_alu_ref__4__funct3))) {
                            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref 
                                = ((__Vfunc_alu_ref__4__a 
                                    < __Vfunc_alu_ref__4__b)
                                    ? 1U : 0U);
                            goto __Vlabel1;
                        } else {
                            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref 
                                = (VL_LTS_III(32, __Vfunc_alu_ref__4__a, __Vfunc_alu_ref__4__b)
                                    ? 1U : 0U);
                            goto __Vlabel1;
                        }
                    } else if ((1U & (IData)(__Vfunc_alu_ref__4__funct3))) {
                        vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref 
                            = (__Vfunc_alu_ref__4__a 
                               << (0x0000001fU & __Vfunc_alu_ref__4__b));
                        goto __Vlabel1;
                    } else {
                        vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref 
                            = (__Vfunc_alu_ref__4__a 
                               + __Vfunc_alu_ref__4__b);
                        goto __Vlabel1;
                    }
                }
                if ((0x00000040U & (IData)(__Vfunc_alu_ref__4__funct7))) {
                    vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref = 0U;
                } else if ((0x00000020U & (IData)(__Vfunc_alu_ref__4__funct7))) {
                    vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref 
                        = ((0x00000010U & (IData)(__Vfunc_alu_ref__4__funct7))
                            ? 0U : ((8U & (IData)(__Vfunc_alu_ref__4__funct7))
                                     ? 0U : ((4U & (IData)(__Vfunc_alu_ref__4__funct7))
                                              ? 0U : 
                                             ((2U & (IData)(__Vfunc_alu_ref__4__funct7))
                                               ? 0U
                                               : ((1U 
                                                   & (IData)(__Vfunc_alu_ref__4__funct7))
                                                   ? 0U
                                                   : 
                                                  ((0U 
                                                    == (IData)(__Vfunc_alu_ref__4__funct3))
                                                    ? 
                                                   (__Vfunc_alu_ref__4__a 
                                                    - __Vfunc_alu_ref__4__b)
                                                    : 
                                                   ((5U 
                                                     == (IData)(__Vfunc_alu_ref__4__funct3))
                                                     ? 
                                                    VL_SHIFTRS_III(32,32,5, __Vfunc_alu_ref__4__a, 
                                                                   (0x0000001fU 
                                                                    & __Vfunc_alu_ref__4__b))
                                                     : 0U)))))));
                } else if ((0x00000010U & (IData)(__Vfunc_alu_ref__4__funct7))) {
                    vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref 
                        = ((8U & (IData)(__Vfunc_alu_ref__4__funct7))
                            ? 0U : ((4U & (IData)(__Vfunc_alu_ref__4__funct7))
                                     ? 0U : ((2U & (IData)(__Vfunc_alu_ref__4__funct7))
                                              ? 0U : 
                                             ((1U & (IData)(__Vfunc_alu_ref__4__funct7))
                                               ? 0U
                                               : ((2U 
                                                   == (IData)(__Vfunc_alu_ref__4__funct3))
                                                   ? 
                                                  (VL_SHIFTL_III(32,32,32, __Vfunc_alu_ref__4__a, 1U) 
                                                   + __Vfunc_alu_ref__4__b)
                                                   : 
                                                  ((4U 
                                                    == (IData)(__Vfunc_alu_ref__4__funct3))
                                                    ? 
                                                   (VL_SHIFTL_III(32,32,32, __Vfunc_alu_ref__4__a, 2U) 
                                                    + __Vfunc_alu_ref__4__b)
                                                    : 
                                                   ((6U 
                                                     == (IData)(__Vfunc_alu_ref__4__funct3))
                                                     ? 
                                                    (VL_SHIFTL_III(32,32,32, __Vfunc_alu_ref__4__a, 3U) 
                                                     + __Vfunc_alu_ref__4__b)
                                                     : 0U)))))));
                } else if ((8U & (IData)(__Vfunc_alu_ref__4__funct7))) {
                    vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref = 0U;
                } else if ((4U & (IData)(__Vfunc_alu_ref__4__funct7))) {
                    vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref 
                        = ((2U & (IData)(__Vfunc_alu_ref__4__funct7))
                            ? 0U : ((1U & (IData)(__Vfunc_alu_ref__4__funct7))
                                     ? ((4U & (IData)(__Vfunc_alu_ref__4__funct3))
                                         ? ((2U & (IData)(__Vfunc_alu_ref__4__funct3))
                                             ? ((1U 
                                                 & (IData)(__Vfunc_alu_ref__4__funct3))
                                                 ? 
                                                ((__Vfunc_alu_ref__4__a 
                                                  > __Vfunc_alu_ref__4__b)
                                                  ? __Vfunc_alu_ref__4__a
                                                  : __Vfunc_alu_ref__4__b)
                                                 : 
                                                (VL_GTS_III(32, __Vfunc_alu_ref__4__a, __Vfunc_alu_ref__4__b)
                                                  ? __Vfunc_alu_ref__4__a
                                                  : __Vfunc_alu_ref__4__b))
                                             : ((1U 
                                                 & (IData)(__Vfunc_alu_ref__4__funct3))
                                                 ? 
                                                ((__Vfunc_alu_ref__4__a 
                                                  < __Vfunc_alu_ref__4__b)
                                                  ? __Vfunc_alu_ref__4__a
                                                  : __Vfunc_alu_ref__4__b)
                                                 : 
                                                (VL_LTS_III(32, __Vfunc_alu_ref__4__a, __Vfunc_alu_ref__4__b)
                                                  ? __Vfunc_alu_ref__4__a
                                                  : __Vfunc_alu_ref__4__b)))
                                         : 0U) : 0U));
                } else if ((2U & (IData)(__Vfunc_alu_ref__4__funct7))) {
                    vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref = 0U;
                } else if ((1U & (IData)(__Vfunc_alu_ref__4__funct7))) {
                    __Vfunc_alu_ref__4__sp = VL_MULS_QQQ(64, __Vfunc_alu_ref__4__sa, __Vfunc_alu_ref__4__sb);
                    __Vfunc_alu_ref__4__hs = VL_MULS_QQQ(64, __Vfunc_alu_ref__4__sa, __Vfunc_alu_ref__4__ub);
                    __Vfunc_alu_ref__4__up = (__Vfunc_alu_ref__4__ua 
                                              * __Vfunc_alu_ref__4__ub);
                    if ((4U & (IData)(__Vfunc_alu_ref__4__funct3))) {
                        if ((2U & (IData)(__Vfunc_alu_ref__4__funct3))) {
                            if ((1U & (IData)(__Vfunc_alu_ref__4__funct3))) {
                                if ((0U == __Vfunc_alu_ref__4__b)) {
                                    vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref 
                                        = __Vfunc_alu_ref__4__a;
                                } else {
                                    __Vfunc_alu_ref__4__umod 
                                        = (IData)(VL_MODDIV_QQQ(64, __Vfunc_alu_ref__4__ua, __Vfunc_alu_ref__4__ub));
                                    vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref 
                                        = __Vfunc_alu_ref__4__umod;
                                }
                            } else if ((0U == __Vfunc_alu_ref__4__b)) {
                                vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref 
                                    = __Vfunc_alu_ref__4__a;
                            } else if (((0x80000000U 
                                         == __Vfunc_alu_ref__4__a) 
                                        & (0xffffffffU 
                                           == __Vfunc_alu_ref__4__b))) {
                                vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref = 0U;
                            } else {
                                __Vfunc_alu_ref__4__smod 
                                    = (IData)(VL_MODDIVS_QQQ(64, __Vfunc_alu_ref__4__sa, __Vfunc_alu_ref__4__sb));
                                vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref 
                                    = __Vfunc_alu_ref__4__smod;
                            }
                        } else if ((1U & (IData)(__Vfunc_alu_ref__4__funct3))) {
                            if ((0U == __Vfunc_alu_ref__4__b)) {
                                vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref = 0xffffffffU;
                            } else {
                                __Vfunc_alu_ref__4__udiv 
                                    = (IData)(VL_DIV_QQQ(64, __Vfunc_alu_ref__4__ua, __Vfunc_alu_ref__4__ub));
                                vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref 
                                    = __Vfunc_alu_ref__4__udiv;
                            }
                        } else if ((0U == __Vfunc_alu_ref__4__b)) {
                            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref = 0xffffffffU;
                        } else if (((0x80000000U == __Vfunc_alu_ref__4__a) 
                                    & (0xffffffffU 
                                       == __Vfunc_alu_ref__4__b))) {
                            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref 
                                = __Vfunc_alu_ref__4__a;
                        } else {
                            __Vfunc_alu_ref__4__sdiv 
                                = (IData)(VL_DIVS_QQQ(64, __Vfunc_alu_ref__4__sa, __Vfunc_alu_ref__4__sb));
                            vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref 
                                = __Vfunc_alu_ref__4__sdiv;
                        }
                    } else {
                        vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref 
                            = ((2U & (IData)(__Vfunc_alu_ref__4__funct3))
                                ? ((1U & (IData)(__Vfunc_alu_ref__4__funct3))
                                    ? (IData)((__Vfunc_alu_ref__4__up 
                                               >> 0x20U))
                                    : (IData)((__Vfunc_alu_ref__4__hs 
                                               >> 0x20U)))
                                : ((1U & (IData)(__Vfunc_alu_ref__4__funct3))
                                    ? (IData)((__Vfunc_alu_ref__4__sp 
                                               >> 0x20U))
                                    : (IData)(__Vfunc_alu_ref__4__sp)));
                    }
                } else {
                    vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref 
                        = ((4U & (IData)(__Vfunc_alu_ref__4__funct3))
                            ? ((2U & (IData)(__Vfunc_alu_ref__4__funct3))
                                ? ((1U & (IData)(__Vfunc_alu_ref__4__funct3))
                                    ? (__Vfunc_alu_ref__4__a 
                                       & __Vfunc_alu_ref__4__b)
                                    : (__Vfunc_alu_ref__4__a 
                                       | __Vfunc_alu_ref__4__b))
                                : ((1U & (IData)(__Vfunc_alu_ref__4__funct3))
                                    ? (__Vfunc_alu_ref__4__a 
                                       >> (0x0000001fU 
                                           & __Vfunc_alu_ref__4__b))
                                    : (__Vfunc_alu_ref__4__a 
                                       ^ __Vfunc_alu_ref__4__b)))
                            : ((2U & (IData)(__Vfunc_alu_ref__4__funct3))
                                ? ((1U & (IData)(__Vfunc_alu_ref__4__funct3))
                                    ? ((__Vfunc_alu_ref__4__a 
                                        < __Vfunc_alu_ref__4__b)
                                        ? 1U : 0U) : 
                                   (VL_LTS_III(32, __Vfunc_alu_ref__4__a, __Vfunc_alu_ref__4__b)
                                     ? 1U : 0U)) : 
                               ((1U & (IData)(__Vfunc_alu_ref__4__funct3))
                                 ? (__Vfunc_alu_ref__4__a 
                                    << (0x0000001fU 
                                        & __Vfunc_alu_ref__4__b))
                                 : (__Vfunc_alu_ref__4__a 
                                    + __Vfunc_alu_ref__4__b))));
                }
                __Vlabel1: ;
            }
            if (VL_UNLIKELY(((1U & (~ ((~ ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_valid) 
                                           & (IData)(vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_2__alu_known))) 
                                       | (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_alu 
                                          == vlSelfRef.rv_core__DOT__u_stage_sva__DOT____VlemCall_3__alu_ref))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_stage_sva.sv:213: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_stage_sva.A_EX_ALU", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_stage_sva.sv", 213, "");
            }
        }
    }
}

void Vtop___024root___nba_sequent__TOP__3(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__3\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (vlSelfRef.rv_core__DOT__u_core_defs__DOT__rst_n) {
        if (vlSymsp->_vm_contextp__->assertOnGet(1, 1)) {
            if (VL_UNLIKELY(((vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__load_data 
                              != vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ld_ref)))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:102: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_LOAD_DATA", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 102, "");
            }
            if (VL_UNLIKELY(((vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_link 
                              != ((IData)(4U) + vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_pc_q))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:89: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_EX_LINK", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 89, "");
            }
            if (VL_UNLIKELY(((1U & (~ ((~ ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_valid) 
                                           & ((IData)(
                                                      ((0x20000033U 
                                                        == 
                                                        (0xfe00007fU 
                                                         & vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_instr)) 
                                                       & (((2U 
                                                            == 
                                                            (7U 
                                                             & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_instr 
                                                                >> 0x0cU))) 
                                                           | (4U 
                                                              == 
                                                              (7U 
                                                               & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_instr 
                                                                  >> 0x0cU)))) 
                                                          | (6U 
                                                             == 
                                                             (7U 
                                                              & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_instr 
                                                                 >> 0x0cU)))))) 
                                              | (IData)(
                                                        ((0x0a000033U 
                                                          == 
                                                          (0xfe00007fU 
                                                           & vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_instr)) 
                                                         & ((((4U 
                                                               == 
                                                               (7U 
                                                                & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_instr 
                                                                   >> 0x0cU))) 
                                                              | (5U 
                                                                 == 
                                                                 (7U 
                                                                  & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_instr 
                                                                     >> 0x0cU)))) 
                                                             | (6U 
                                                                == 
                                                                (7U 
                                                                 & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_instr 
                                                                    >> 0x0cU)))) 
                                                            | (7U 
                                                               == 
                                                               (7U 
                                                                & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_instr 
                                                                   >> 0x0cU))))))))) 
                                       | (~ vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_dec[0U]))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:110: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_CRD3_LEGAL", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 110, "");
            }
            if (VL_UNLIKELY((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_stall) 
                              != ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__dmem_valid) 
                                  & (~ (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__dmem_ready))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:80: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_MEM_STALL", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 80, "");
            }
            if (VL_UNLIKELY((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__target_misaligned) 
                              != ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__flow_change) 
                                  & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__target 
                                     >> 1U)))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:91: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_TARGET_MISALIGNED", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 91, "");
            }
            if (VL_UNLIKELY(((vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_final 
                              != ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_is_load_q)
                                   ? vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__load_data
                                   : vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_result_q))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:95: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_MEM_FINAL", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 95, "");
            }
            if (VL_UNLIKELY((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__stall_front) 
                              != (((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_stall) 
                                   | (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mdu_busy)) 
                                  | (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__load_use)))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:82: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_STALL_FRONT", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 82, "");
            }
            if (VL_UNLIKELY((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__trap) 
                              != ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_fire) 
                                  & ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__irq_pending) 
                                     | (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__exc))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:85: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_TRAP", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 85, "");
            }
            if (VL_UNLIKELY(((vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__alu_b 
                              != ((4U & vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[1U])
                                   ? ((vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[2U] 
                                       << 0x00000016U) 
                                      | (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[1U] 
                                         >> 0x0000000aU))
                                   : vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__fwd_rs2))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:93: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_ALU_B", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 93, "");
            }
            if (VL_UNLIKELY((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ls_misaligned) 
                              != (((vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[1U] 
                                    >> 1U) | vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[1U]) 
                                  & ((0U != (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[0U] 
                                             >> 0x0000001eU)) 
                                     & ((1U == (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[0U] 
                                                >> 0x0000001eU))
                                         ? vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__alu_result
                                         : (0U != (3U 
                                                   & vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__alu_result))))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:96: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_LS_MIS", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 96, "");
            }
            if (VL_UNLIKELY((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__flow_change) 
                              != (1U & ((0U != (0x01800000U 
                                                & vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[0U])) 
                                        | ((vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[0U] 
                                            >> 0x0000001cU) 
                                           & (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__branch_taken)))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:90: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_FLOW_CHANGE", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 90, "");
            }
            if (VL_UNLIKELY(((vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__alu_a 
                              != ((1U == (3U & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[1U] 
                                                >> 3U)))
                                   ? vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_pc_q
                                   : ((2U == (3U & 
                                              (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[1U] 
                                               >> 3U)))
                                       ? 0U : vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__fwd_rs1)))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:92: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_ALU_A", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 92, "");
            }
            if (VL_UNLIKELY((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_fire) 
                              != (((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_valid_q) 
                                   & (~ (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_stall))) 
                                  & (~ (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mdu_busy))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:83: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_EX_FIRE", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 83, "");
            }
            if (VL_UNLIKELY(((vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_result 
                              != ((IData)((0U != (0x01800000U 
                                                  & vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[0U])))
                                   ? vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_link
                                   : ((0U != (3U & 
                                              (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[0U] 
                                               >> 0x00000015U)))
                                       ? vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__csr_rdata
                                       : ((0x00000080U 
                                           & vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[0U])
                                           ? vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mdu_result
                                           : vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__alu_result))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:94: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_EX_RESULT", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 94, "");
            }
            if (VL_UNLIKELY((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mdu_busy) 
                              != (((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_valid_q) 
                                   & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[0U] 
                                      >> 7U)) & (~ (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mdu_done))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:81: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_MDU_BUSY", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 81, "");
            }
            if (VL_UNLIKELY((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__redirect) 
                              != (((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__trap) 
                                   | (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mret)) 
                                  | ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_fire) 
                                     & (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__flow_change))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:87: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_REDIRECT", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 87, "");
            }
            if (VL_UNLIKELY((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__load_use) 
                              != ((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_valid) 
                                    & ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_valid_q) 
                                       & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[1U] 
                                          >> 1U))) 
                                   & (0U != (0x0000001fU 
                                             & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[2U] 
                                                >> 0x0000000dU)))) 
                                  & (((vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_dec[2U] 
                                       >> 0x0000000cU) 
                                      & ((0x0000001fU 
                                          & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_dec[2U] 
                                             >> 0x00000017U)) 
                                         == (0x0000001fU 
                                             & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[2U] 
                                                >> 0x0000000dU)))) 
                                     | ((vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_dec[2U] 
                                         >> 0x0000000bU) 
                                        & ((0x0000001fU 
                                            & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_dec[2U] 
                                               >> 0x00000012U)) 
                                           == (0x0000001fU 
                                               & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[2U] 
                                                  >> 0x0000000dU)))))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:100: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_LOAD_USE", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 100, "");
            }
            if (VL_UNLIKELY((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mdu_start) 
                              != ((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_valid_q) 
                                    & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[0U] 
                                       >> 7U)) & (~ (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mdu_active_q))) 
                                  & (~ (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_stall))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:84: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_MDU_START", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 84, "");
            }
            if (VL_UNLIKELY(((vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__redirect_pc 
                              != ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__trap)
                                   ? vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__csr_mtvec
                                   : ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mret)
                                       ? vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__csr_mepc
                                       : (((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_fire) 
                                           & (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__flow_change))
                                           ? vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__target
                                           : 0U))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:88: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_REDIRECT_PC", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 88, "");
            }
            if (VL_UNLIKELY((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mret) 
                              != (((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_fire) 
                                   & (~ (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__trap))) 
                                  & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[0U] 
                                     >> 1U)))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:86: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_MRET", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 86, "");
            }
            if (VL_UNLIKELY((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_valid_q) 
                              != ((IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_23_1) 
                                  & (~ (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_22_1))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:105: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_WBV_NXT", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 105, "");
            }
            if (VL_UNLIKELY(((vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__fwd_rs1 
                              != (((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_valid_q) 
                                     & (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_wb_en_q)) 
                                    & ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_rd_q) 
                                       == (0x0000001fU 
                                           & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[2U] 
                                              >> 0x00000017U)))) 
                                   & (0U != (0x0000001fU 
                                             & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[2U] 
                                                >> 0x00000017U))))
                                   ? vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_result_q
                                   : (((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_valid_q) 
                                         & (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_en_q)) 
                                        & ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_rd_q) 
                                           == (0x0000001fU 
                                               & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[2U] 
                                                  >> 0x00000017U)))) 
                                       & (0U != (0x0000001fU 
                                                 & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[2U] 
                                                    >> 0x00000017U))))
                                       ? vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_data_q
                                       : vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_rs1_q)))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:98: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_FWD_RS1", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 98, "");
            }
            if (VL_UNLIKELY(((vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__fwd_rs2 
                              != (((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_valid_q) 
                                     & (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_wb_en_q)) 
                                    & ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_rd_q) 
                                       == (0x0000001fU 
                                           & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[2U] 
                                              >> 0x00000012U)))) 
                                   & (0U != (0x0000001fU 
                                             & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[2U] 
                                                >> 0x00000012U))))
                                   ? vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_result_q
                                   : (((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_valid_q) 
                                         & (IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_en_q)) 
                                        & ((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_rd_q) 
                                           == (0x0000001fU 
                                               & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[2U] 
                                                  >> 0x00000012U)))) 
                                       & (0U != (0x0000001fU 
                                                 & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[2U] 
                                                    >> 0x00000012U))))
                                       ? vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_data_q
                                       : vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_rs2_q)))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:99: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_FWD_RS2", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 99, "");
            }
            if (VL_UNLIKELY(((vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_data_q 
                              != (((IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_23_1) 
                                   & (~ (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_22_1)))
                                   ? vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_34_1
                                   : vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_35_1))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:108: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_WBDATA_NXT", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 108, "");
            }
            if (VL_UNLIKELY((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_en_q) 
                              != (((IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_23_1) 
                                   & (~ (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_22_1)))
                                   ? (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_28_1)
                                   : (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_29_1)))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:106: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_WBE_NXT", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 106, "");
            }
            if (VL_UNLIKELY((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_rd_q) 
                              != (((IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_23_1) 
                                   & (~ (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_22_1)))
                                   ? (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_31_1)
                                   : (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_32_1)))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:107: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_WBRD_NXT", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 107, "");
            }
            if (VL_UNLIKELY((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_valid_q) 
                              != ((IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_22_1)
                                   ? (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_23_1)
                                   : ((IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_24_1) 
                                      & (~ (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_25_1)))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_core_defs.sv:104: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_core_defs.A_MEMV_NXT", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_core_defs.sv", 104, "");
            }
        }
    }
    vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_34_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_final;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_35_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_data_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_28_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_wb_en_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_29_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_en_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_31_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_rd_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_32_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_rd_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_25_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__trap;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_23_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_valid_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_24_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_fire;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT___Vpast_22_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_stall;
}

void Vtop___024root___nba_sequent__TOP__4(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__4\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__rst_n) {
        if (vlSymsp->_vm_contextp__->assertOnGet(1, 1)) {
            if (VL_UNLIKELY(((vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mepc_q 
                              != (0x3fffffffU & ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_1_1)
                                                  ? 
                                                 (vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_11_1 
                                                  >> 2U)
                                                  : 
                                                 (((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_4_1) 
                                                   & (0x0341U 
                                                      == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_5_1)))
                                                   ? 
                                                  (vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_12_1 
                                                   >> 2U)
                                                   : vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_13_1))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_csr_defs.sv:29: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_csr.u_csr_defs.A_CSR_MEPC", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_csr_defs.sv", 29, "");
            }
            if (VL_UNLIKELY((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mie_q) 
                              != ((~ (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_1_1)) 
                                  & ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_2_1)
                                      ? (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_3_1)
                                      : (((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_4_1) 
                                          & (0x0300U 
                                             == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_5_1)))
                                          ? (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_6_1)
                                          : (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_7_1)))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_csr_defs.sv:21: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_csr.u_csr_defs.A_CSR_MIE", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_csr_defs.sv", 21, "");
            }
            if (VL_UNLIKELY((((IData)(vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mpie_q) 
                              != ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_1_1)
                                   ? (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_7_1)
                                   : ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_2_1) 
                                      | (((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_4_1) 
                                          & (0x0300U 
                                             == (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_5_1)))
                                          ? (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_9_1)
                                          : (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_3_1)))))))) {
                VL_WRITEF_NX("[%0t] %%Error: rv_csr_defs.sv:25: Assertion failed in %m: 'assert' failed.\n",3, 'M',vlSymsp->name(),"rv_core.u_csr.u_csr_defs.A_CSR_MPIE", 'T',-9
                             , '#',64,VL_TIME_UNITED_Q(1000));
                VL_STOP_MT("/design/assertions/rv_csr_defs.sv", 25, "");
            }
        }
    }
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_11_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__trap_pc;
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_13_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mepc_q;
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_12_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__wr_val;
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_6_1 
        = (1U & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__wr_val 
                 >> 3U));
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_2_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mret;
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_1_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__trap;
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_4_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__do_write;
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_5_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__addr;
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_7_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mie_q;
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_3_1 
        = vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mpie_q;
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT___Vpast_9_1 
        = (1U & (vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__wr_val 
                 >> 7U));
}

void Vtop___024root___nba_sequent__TOP__5(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__5\n"); );
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
    vlSelfRef.rv_core__DOT__u_top_if__DOT__imem_addr 
        = vlSelfRef.rv_core__DOT__imem_addr;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_valid 
        = vlSelfRef.rv_core__DOT__u_fetch__DOT__started_q;
    vlSelfRef.rv_core__DOT__imem_valid = vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_valid;
    vlSelfRef.imem_valid = vlSelfRef.rv_core__DOT__imem_valid;
    vlSelfRef.rv_core__DOT__u_top_if__DOT__imem_valid 
        = vlSelfRef.rv_core__DOT__imem_valid;
}

void Vtop___024root___nba_sequent__TOP__6(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__6\n"); );
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
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mpie_q 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__mpie_q;
    vlSelfRef.rv_core__DOT__u_csr__DOT__mtvec = (vlSelfRef.rv_core__DOT__u_csr__DOT__mtvec_q 
                                                 << 2U);
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mepc_q 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__mepc_q;
    vlSelfRef.rv_core__DOT__u_csr__DOT__mepc = (vlSelfRef.rv_core__DOT__u_csr__DOT__mepc_q 
                                                << 2U);
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mie_q 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__mie_q;
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
    vlSelfRef.rv_core__DOT__csr_mtvec = vlSelfRef.rv_core__DOT__u_csr__DOT__mtvec;
    vlSelfRef.rv_core__DOT__csr_mepc = vlSelfRef.rv_core__DOT__u_csr__DOT__mepc;
    vlSelfRef.rv_core__DOT__u_csr__DOT__irq_cause = 
        ((IData)(__VdfgRegularize_h6e95ff9d_0_5) ? 0x8000000bU
          : 0x80000007U);
    vlSelfRef.rv_core__DOT__u_csr__DOT__irq_pending 
        = ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__mie_q) 
           & (((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__irq_timer) 
               & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__mtie_q)) 
              | (IData)(__VdfgRegularize_h6e95ff9d_0_5)));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__csr_mtvec 
        = vlSelfRef.rv_core__DOT__csr_mtvec;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__csr_mepc 
        = vlSelfRef.rv_core__DOT__csr_mepc;
    vlSelfRef.rv_core__DOT__irq_cause = vlSelfRef.rv_core__DOT__u_csr__DOT__irq_cause;
    vlSelfRef.rv_core__DOT__irq_pending = vlSelfRef.rv_core__DOT__u_csr__DOT__irq_pending;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__irq_pending 
        = vlSelfRef.rv_core__DOT__irq_pending;
}

void Vtop___024root___nba_sequent__TOP__7(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__7\n"); );
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
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__op_q 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_q;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__op_b_q 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__op_b_q;
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
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__done 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__done;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__result 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__result;
    vlSelfRef.rv_core__DOT__mdu_result = vlSelfRef.rv_core__DOT__u_muldiv__DOT__result;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mdu_result 
        = vlSelfRef.rv_core__DOT__mdu_result;
}

void Vtop___024root___nba_sequent__TOP__8(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__8\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__rst_n) {
        if (vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__start) {
            vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__a_ref_q 
                = vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__a;
        }
    } else {
        vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__a_ref_q = 0U;
    }
}

void Vtop___024root___nba_sequent__TOP__9(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__9\n"); );
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

void Vtop___024root___nba_sequent__TOP__10(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__10\n"); );
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
            vlSelfRef.rv_core__DOT__ex_rs1_q = vlSelfRef.rv_core__DOT__fwd_rs1;
            vlSelfRef.rv_core__DOT__ex_rs2_q = vlSelfRef.rv_core__DOT__fwd_rs2;
        } else if ((1U & (~ ((IData)(vlSelfRef.rv_core__DOT__redirect) 
                             | (IData)(vlSelfRef.rv_core__DOT__load_use))))) {
            vlSelfRef.rv_core__DOT__ex_rs1_q = vlSelfRef.rv_core__DOT__id_rs1_val;
            vlSelfRef.rv_core__DOT__ex_rs2_q = vlSelfRef.rv_core__DOT__id_rs2_val;
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
        vlSelfRef.rv_core__DOT__ex_rs1_q = 0U;
        vlSelfRef.rv_core__DOT__ex_rs2_q = 0U;
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mdu_active_q 
        = vlSelfRef.rv_core__DOT__mdu_active_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_valid_q 
        = vlSelfRef.rv_core__DOT__ex_valid_q;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_instr 
        = vlSelfRef.rv_core__DOT__ex_instr_q;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__store_data 
        = vlSelfRef.rv_core__DOT__mem_store_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_pc_q 
        = vlSelfRef.rv_core__DOT__ex_pc_q;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_pc 
        = vlSelfRef.rv_core__DOT__ex_pc_q;
    vlSelfRef.rv_core__DOT__u_csr__DOT__trap_pc = vlSelfRef.rv_core__DOT__ex_pc_q;
    vlSelfRef.rv_core__DOT__ex_link = ((IData)(4U) 
                                       + vlSelfRef.rv_core__DOT__ex_pc_q);
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_rs1_q 
        = vlSelfRef.rv_core__DOT__ex_rs1_q;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs1_val 
        = vlSelfRef.rv_core__DOT__ex_rs1_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_rs2_q 
        = vlSelfRef.rv_core__DOT__ex_rs2_q;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs2_val 
        = vlSelfRef.rv_core__DOT__ex_rs2_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_result_q 
        = vlSelfRef.rv_core__DOT__mem_result_q;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__addr = vlSelfRef.rv_core__DOT__mem_result_q;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__mem_result 
        = vlSelfRef.rv_core__DOT__mem_result_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__wb_valid_q 
        = vlSelfRef.rv_core__DOT__wb_valid_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__wb_data_q 
        = vlSelfRef.rv_core__DOT__wb_data_q;
    vlSelfRef.rv_core__DOT__u_regfile__DOT__wdata = vlSelfRef.rv_core__DOT__wb_data_q;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__wb_data 
        = vlSelfRef.rv_core__DOT__wb_data_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_unsigned_q 
        = vlSelfRef.rv_core__DOT__mem_unsigned_q;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__is_unsigned 
        = vlSelfRef.rv_core__DOT__mem_unsigned_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_size_q 
        = vlSelfRef.rv_core__DOT__mem_size_q;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__size = vlSelfRef.rv_core__DOT__mem_size_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_is_load_q 
        = vlSelfRef.rv_core__DOT__mem_is_load_q;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__is_load = vlSelfRef.rv_core__DOT__mem_is_load_q;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__is_store = vlSelfRef.rv_core__DOT__mem_is_store_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__wb_rd_q 
        = vlSelfRef.rv_core__DOT__wb_rd_q;
    vlSelfRef.rv_core__DOT__u_regfile__DOT__waddr = vlSelfRef.rv_core__DOT__wb_rd_q;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__wb_rd = vlSelfRef.rv_core__DOT__wb_rd_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__wb_en_q 
        = vlSelfRef.rv_core__DOT__wb_en_q;
    rv_core__DOT____Vcellinp__u_regfile__we = ((IData)(vlSelfRef.rv_core__DOT__wb_valid_q) 
                                               & (IData)(vlSelfRef.rv_core__DOT__wb_en_q));
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_funct3 
        = (7U & (vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_instr 
                 >> 0x0000000cU));
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_funct7 
        = (vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_instr 
           >> 0x00000019U);
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_opcode 
        = (0x0000007fU & vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_instr);
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__trap_pc 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__trap_pc;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_link 
        = vlSelfRef.rv_core__DOT__ex_link;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__offset = (3U 
                                                  & vlSelfRef.rv_core__DOT__u_lsu__DOT__addr);
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_off 
        = (3U & vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_result_q);
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_byte 
        = (0x000000ffU & ((0U == (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_off))
                           ? vlSelfRef.rv_core__DOT__u_core_defs__DOT__dmem_rdata
                           : ((1U == (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_off))
                               ? (vlSelfRef.rv_core__DOT__u_core_defs__DOT__dmem_rdata 
                                  >> 8U) : ((2U == (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_off))
                                             ? (vlSelfRef.rv_core__DOT__u_core_defs__DOT__dmem_rdata 
                                                >> 0x10U)
                                             : (vlSelfRef.rv_core__DOT__u_core_defs__DOT__dmem_rdata 
                                                >> 0x18U)))));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_half 
        = (0x0000ffffU & ((2U & (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_off))
                           ? (vlSelfRef.rv_core__DOT__u_core_defs__DOT__dmem_rdata 
                              >> 0x10U) : vlSelfRef.rv_core__DOT__u_core_defs__DOT__dmem_rdata));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_ref 
        = ((0U == (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_size_q))
            ? (((- (IData)(((~ (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_unsigned_q)) 
                            & ((IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_byte) 
                               >> 7U)))) << 8U) | (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_byte))
            : ((1U == (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_size_q))
                ? (((- (IData)(((~ (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_unsigned_q)) 
                                & ((IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_half) 
                                   >> 0x0000000fU)))) 
                    << 0x00000010U) | (IData)(vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_half))
                : vlSelfRef.rv_core__DOT__u_core_defs__DOT__dmem_rdata));
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_rd_q 
        = vlSelfRef.rv_core__DOT__mem_rd_q;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__mem_rd = vlSelfRef.rv_core__DOT__mem_rd_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_valid_q 
        = vlSelfRef.rv_core__DOT__mem_valid_q;
    vlSelfRef.rv_core__DOT__u_lsu__DOT__mem_valid = vlSelfRef.rv_core__DOT__mem_valid_q;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_wb_en_q 
        = vlSelfRef.rv_core__DOT__mem_wb_en_q;
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_dec_q[0U] 
        = vlSelfRef.rv_core__DOT__ex_dec_q[0U];
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_dec_q[1U] 
        = vlSelfRef.rv_core__DOT__ex_dec_q[1U];
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_dec_q[2U] 
        = vlSelfRef.rv_core__DOT__ex_dec_q[2U];
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
    vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs1 = 
        (0x0000001fU & (vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                        >> 0x00000017U));
    vlSelfRef.rv_core__DOT__u_hazard__DOT__ex_rs2 = 
        (0x0000001fU & (vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                        >> 0x00000012U));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__load_data 
        = vlSelfRef.rv_core__DOT__load_data;
    vlSelfRef.rv_core__DOT__mem_final = ((IData)(vlSelfRef.rv_core__DOT__mem_is_load_q)
                                          ? vlSelfRef.rv_core__DOT__load_data
                                          : vlSelfRef.rv_core__DOT__mem_result_q);
    vlSelfRef.dmem_write = vlSelfRef.rv_core__DOT__dmem_write;
    vlSelfRef.rv_core__DOT__u_top_if__DOT__dmem_write 
        = vlSelfRef.rv_core__DOT__dmem_write;
    vlSelfRef.dmem_wstrb = vlSelfRef.rv_core__DOT__dmem_wstrb;
    vlSelfRef.rv_core__DOT__u_top_if__DOT__dmem_wstrb 
        = vlSelfRef.rv_core__DOT__dmem_wstrb;
    vlSelfRef.dmem_wdata = vlSelfRef.rv_core__DOT__dmem_wdata;
    vlSelfRef.rv_core__DOT__u_top_if__DOT__dmem_wdata 
        = vlSelfRef.rv_core__DOT__dmem_wdata;
    vlSelfRef.rv_core__DOT__dmem_addr = vlSelfRef.rv_core__DOT__u_lsu__DOT__dmem_addr;
    vlSelfRef.dmem_valid = vlSelfRef.rv_core__DOT__dmem_valid;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__dmem_valid 
        = vlSelfRef.rv_core__DOT__dmem_valid;
    vlSelfRef.rv_core__DOT__u_top_if__DOT__dmem_valid 
        = vlSelfRef.rv_core__DOT__dmem_valid;
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
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__addr 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__addr;
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_final 
        = vlSelfRef.rv_core__DOT__mem_final;
    vlSelfRef.dmem_addr = vlSelfRef.rv_core__DOT__dmem_addr;
    vlSelfRef.rv_core__DOT__u_top_if__DOT__dmem_addr 
        = vlSelfRef.rv_core__DOT__dmem_addr;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_stall 
        = vlSelfRef.rv_core__DOT__mem_stall;
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
    vlSelfRef.rv_core__DOT__fwd_rs1 = vlSelfRef.rv_core__DOT__u_hazard__DOT__fwd_rs1;
    vlSelfRef.rv_core__DOT__fwd_rs2 = vlSelfRef.rv_core__DOT__u_hazard__DOT__fwd_rs2;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mdu_start 
        = vlSelfRef.rv_core__DOT__mdu_start;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__start = vlSelfRef.rv_core__DOT__mdu_start;
    vlSelfRef.rv_core__DOT__u_csr__DOT__illegal = (
                                                   (0U 
                                                    != (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op)) 
                                                   & ((~ (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__known)) 
                                                      | ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__ro) 
                                                         & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__is_write))));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__fwd_rs1 
        = vlSelfRef.rv_core__DOT__fwd_rs1;
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__fwd_rs2 
        = vlSelfRef.rv_core__DOT__fwd_rs2;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__b = vlSelfRef.rv_core__DOT__fwd_rs2;
    vlSelfRef.rv_core__DOT__alu_b = ((4U & vlSelfRef.rv_core__DOT__ex_dec_q[1U])
                                      ? ((vlSelfRef.rv_core__DOT__ex_dec_q[2U] 
                                          << 0x00000016U) 
                                         | (vlSelfRef.rv_core__DOT__ex_dec_q[1U] 
                                            >> 0x0000000aU))
                                      : vlSelfRef.rv_core__DOT__fwd_rs2);
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__start 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__start;
    vlSelfRef.rv_core__DOT__csr_illegal = vlSelfRef.rv_core__DOT__u_csr__DOT__illegal;
    vlSelfRef.rv_core__DOT__u_csr__DOT__wdata = vlSelfRef.rv_core__DOT__csr_wdata;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__a 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__a;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__a_neg = 
        ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__a_signed) 
         & (vlSelfRef.rv_core__DOT__u_muldiv__DOT__a 
            >> 0x0000001fU));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__target 
        = vlSelfRef.rv_core__DOT__target;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_target 
        = vlSelfRef.rv_core__DOT__target;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_valid 
        = ((~ ((vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                >> 7U) | ((vlSelfRef.rv_core__DOT__target 
                           >> 1U) & (IData)((0U != 
                                             (0x11000000U 
                                              & vlSelfRef.rv_core__DOT__ex_dec_q[0U])))))) 
           & (IData)(vlSelfRef.rv_core__DOT__ex_valid_q));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__alu_a 
        = vlSelfRef.rv_core__DOT__alu_a;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_a 
        = vlSelfRef.rv_core__DOT__alu_a;
    vlSelfRef.rv_core__DOT__u_alu__DOT__a = vlSelfRef.rv_core__DOT__alu_a;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__b_neg = 
        ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__b_signed) 
         & (vlSelfRef.rv_core__DOT__u_muldiv__DOT__b 
            >> 0x0000001fU));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__alu_b 
        = vlSelfRef.rv_core__DOT__alu_b;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_b 
        = vlSelfRef.rv_core__DOT__alu_b;
    vlSelfRef.rv_core__DOT__u_alu__DOT__b = vlSelfRef.rv_core__DOT__alu_b;
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__a_abs = 
        ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__a_neg)
          ? ((IData)(1U) + (~ vlSelfRef.rv_core__DOT__u_muldiv__DOT__a))
          : vlSelfRef.rv_core__DOT__u_muldiv__DOT__a);
    vlSelfRef.rv_core__DOT__u_muldiv__DOT__b_abs = 
        ((IData)(vlSelfRef.rv_core__DOT__u_muldiv__DOT__b_neg)
          ? ((IData)(1U) + (~ vlSelfRef.rv_core__DOT__u_muldiv__DOT__b))
          : vlSelfRef.rv_core__DOT__u_muldiv__DOT__b);
    if ((0x13U == (IData)(vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_opcode))) {
        vlSelfRef.__Vfunc_imm_ref__0__opcode = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_opcode;
        vlSelfRef.__Vfunc_imm_ref__0__instr = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_instr;
        vlSelfRef.__Vfunc_imm_ref__0__i_imm = (((- (IData)(
                                                           (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                            >> 0x1fU))) 
                                                << 0x0000000cU) 
                                               | (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                  >> 0x14U));
        vlSelfRef.__Vfunc_imm_ref__0__s_imm = (((- (IData)(
                                                           (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                            >> 0x1fU))) 
                                                << 0x0000000cU) 
                                               | ((0x00000fe0U 
                                                   & (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                      >> 0x00000014U)) 
                                                  | (0x0000001fU 
                                                     & (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                        >> 7U))));
        vlSelfRef.__Vfunc_imm_ref__0__b_imm = (((- (IData)(
                                                           (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                            >> 0x1fU))) 
                                                << 0x0000000dU) 
                                               | ((((2U 
                                                     & (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                        >> 0x0000001eU)) 
                                                    | (1U 
                                                       & (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                          >> 7U))) 
                                                   << 0x0000000bU) 
                                                  | ((0x000007e0U 
                                                      & (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                         >> 0x00000014U)) 
                                                     | (0x0000001eU 
                                                        & (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                           >> 7U)))));
        vlSelfRef.__Vfunc_imm_ref__0__u_imm = (0xfffff000U 
                                               & vlSelfRef.__Vfunc_imm_ref__0__instr);
        vlSelfRef.__Vfunc_imm_ref__0__j_imm = ((((0x00000ffeU 
                                                  & ((- (IData)(
                                                                (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                                 >> 0x1fU))) 
                                                     << 1U)) 
                                                 | (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                    >> 0x1fU)) 
                                                << 0x00000014U) 
                                               | ((((0x000001feU 
                                                     & (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                        >> 0x0000000bU)) 
                                                    | (1U 
                                                       & (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                          >> 0x14U))) 
                                                   << 0x0000000bU) 
                                                  | (0x000007feU 
                                                     & (vlSelfRef.__Vfunc_imm_ref__0__instr 
                                                        >> 0x00000014U))));
        vlSelfRef.__Vfunc_imm_ref__0__Vfuncout = ((0x00000040U 
                                                   & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                   ? 
                                                  ((0x00000020U 
                                                    & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                    ? 
                                                   ((0x00000010U 
                                                     & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                     ? 0U
                                                     : 
                                                    ((8U 
                                                      & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                      ? 
                                                     ((4U 
                                                       & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                       ? 
                                                      ((2U 
                                                        & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                        ? 
                                                       ((1U 
                                                         & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                         ? vlSelfRef.__Vfunc_imm_ref__0__j_imm
                                                         : 0U)
                                                        : 0U)
                                                       : 0U)
                                                      : 
                                                     ((4U 
                                                       & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                       ? 
                                                      ((2U 
                                                        & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                        ? 
                                                       ((1U 
                                                         & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                         ? vlSelfRef.__Vfunc_imm_ref__0__i_imm
                                                         : 0U)
                                                        : 0U)
                                                       : 
                                                      ((2U 
                                                        & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                        ? 
                                                       ((1U 
                                                         & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                         ? vlSelfRef.__Vfunc_imm_ref__0__b_imm
                                                         : 0U)
                                                        : 0U))))
                                                    : 0U)
                                                   : 
                                                  ((0x00000020U 
                                                    & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                    ? 
                                                   ((0x00000010U 
                                                     & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                     ? 
                                                    ((8U 
                                                      & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                      ? 0U
                                                      : 
                                                     ((4U 
                                                       & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                       ? 
                                                      ((2U 
                                                        & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                        ? 
                                                       ((1U 
                                                         & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                         ? vlSelfRef.__Vfunc_imm_ref__0__u_imm
                                                         : 0U)
                                                        : 0U)
                                                       : 0U))
                                                     : 
                                                    ((8U 
                                                      & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                      ? 0U
                                                      : 
                                                     ((4U 
                                                       & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                       ? 0U
                                                       : 
                                                      ((2U 
                                                        & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                        ? 
                                                       ((1U 
                                                         & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                         ? vlSelfRef.__Vfunc_imm_ref__0__s_imm
                                                         : 0U)
                                                        : 0U))))
                                                    : 
                                                   ((0x00000010U 
                                                     & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                     ? 
                                                    ((8U 
                                                      & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                      ? 0U
                                                      : 
                                                     ((4U 
                                                       & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                       ? 
                                                      ((2U 
                                                        & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                        ? 
                                                       ((1U 
                                                         & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                         ? vlSelfRef.__Vfunc_imm_ref__0__u_imm
                                                         : 0U)
                                                        : 0U)
                                                       : 
                                                      ((2U 
                                                        & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                        ? 
                                                       ((1U 
                                                         & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                         ? vlSelfRef.__Vfunc_imm_ref__0__i_imm
                                                         : 0U)
                                                        : 0U)))
                                                     : 
                                                    ((8U 
                                                      & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                      ? 0U
                                                      : 
                                                     ((4U 
                                                       & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                       ? 0U
                                                       : 
                                                      ((2U 
                                                        & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                        ? 
                                                       ((1U 
                                                         & (IData)(vlSelfRef.__Vfunc_imm_ref__0__opcode))
                                                         ? vlSelfRef.__Vfunc_imm_ref__0__i_imm
                                                         : 0U)
                                                        : 0U))))));
        vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_b_ref 
            = vlSelfRef.__Vfunc_imm_ref__0__Vfuncout;
    } else {
        vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_b_ref 
            = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_b;
    }
    vlSelfRef.rv_core__DOT__u_alu__DOT__shamt = (0x0000001fU 
                                                 & vlSelfRef.rv_core__DOT__u_alu__DOT__b);
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__alu_result 
        = vlSelfRef.rv_core__DOT__alu_result;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_alu 
        = vlSelfRef.rv_core__DOT__alu_result;
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__branch_taken 
        = vlSelfRef.rv_core__DOT__branch_taken;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_taken 
        = vlSelfRef.rv_core__DOT__branch_taken;
    vlSelfRef.rv_core__DOT__flow_change = (((IData)(vlSelfRef.rv_core__DOT__branch_taken) 
                                            & (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                               >> 0x0000001cU)) 
                                           | (0U != 
                                              (3U & 
                                               (vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                                >> 0x00000017U))));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ls_misaligned 
        = vlSelfRef.rv_core__DOT__ls_misaligned;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__flow_change 
        = vlSelfRef.rv_core__DOT__flow_change;
    vlSelfRef.rv_core__DOT__target_misaligned = ((IData)(vlSelfRef.rv_core__DOT__flow_change) 
                                                 & (vlSelfRef.rv_core__DOT__target 
                                                    >> 1U));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__target_misaligned 
        = vlSelfRef.rv_core__DOT__target_misaligned;
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__exc = vlSelfRef.rv_core__DOT__exc;
}

void Vtop___024root___nba_sequent__TOP__11(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__11\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.rv_core__DOT__mdu_done = vlSelfRef.rv_core__DOT__u_muldiv__DOT__done;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mdu_done 
        = vlSelfRef.rv_core__DOT__mdu_done;
}

void Vtop___024root___nba_sequent__TOP__12(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__12\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.rv_core__DOT__id_instr = vlSelfRef.rv_core__DOT__u_fetch__DOT__id_instr;
    vlSelfRef.rv_core__DOT__id_pc = vlSelfRef.rv_core__DOT__u_fetch__DOT__id_pc;
    vlSelfRef.rv_core__DOT__id_valid = vlSelfRef.rv_core__DOT__u_fetch__DOT__id_valid;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__id_instr 
        = vlSelfRef.rv_core__DOT__id_instr;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_instr 
        = vlSelfRef.rv_core__DOT__id_instr;
    vlSelfRef.rv_core__DOT__u_decode__DOT__instr = vlSelfRef.rv_core__DOT__id_instr;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_pc 
        = vlSelfRef.rv_core__DOT__id_pc;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__id_valid 
        = vlSelfRef.rv_core__DOT__id_valid;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_valid 
        = vlSelfRef.rv_core__DOT__id_valid;
    vlSelfRef.rv_core__DOT__u_hazard__DOT__id_valid 
        = vlSelfRef.rv_core__DOT__id_valid;
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_opcode 
        = (0x0000007fU & vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_instr);
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_funct3 
        = (7U & (vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_instr 
                 >> 0x0000000cU));
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_funct7 
        = (vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_instr 
           >> 0x00000019U);
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__id_dec[0U] 
        = vlSelfRef.rv_core__DOT__id_dec[0U];
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__id_dec[1U] 
        = vlSelfRef.rv_core__DOT__id_dec[1U];
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__id_dec[2U] 
        = vlSelfRef.rv_core__DOT__id_dec[2U];
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_rs1 
        = (0x0000001fU & (vlSelfRef.rv_core__DOT__id_dec[2U] 
                          >> 0x00000017U));
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_rs2 
        = (0x0000001fU & (vlSelfRef.rv_core__DOT__id_dec[2U] 
                          >> 0x00000012U));
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_rd 
        = (0x0000001fU & (vlSelfRef.rv_core__DOT__id_dec[2U] 
                          >> 0x0000000dU));
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_imm 
        = ((vlSelfRef.rv_core__DOT__id_dec[2U] << 0x00000016U) 
           | (vlSelfRef.rv_core__DOT__id_dec[1U] >> 0x0000000aU));
    vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_illegal 
        = (1U & vlSelfRef.rv_core__DOT__id_dec[0U]);
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__csr_rdata 
        = vlSelfRef.rv_core__DOT__csr_rdata;
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__wr_val 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__wr_val;
}

void Vtop___024root___nba_comb__TOP__1(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__1\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.rv_core__DOT__mdu_busy = ((~ (IData)(vlSelfRef.rv_core__DOT__mdu_done)) 
                                        & (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_1));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mdu_busy 
        = vlSelfRef.rv_core__DOT__mdu_busy;
    vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2 = ((IData)(vlSelfRef.rv_core__DOT__mem_stall) 
                                                | (IData)(vlSelfRef.rv_core__DOT__mdu_busy));
    vlSelfRef.rv_core__DOT__ex_fire = ((~ (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2)) 
                                       & (IData)(vlSelfRef.rv_core__DOT__ex_valid_q));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_fire 
        = vlSelfRef.rv_core__DOT__ex_fire;
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__load_use 
        = vlSelfRef.rv_core__DOT__load_use;
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_result 
        = vlSelfRef.rv_core__DOT__ex_result;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__trap 
        = vlSelfRef.rv_core__DOT__trap;
    vlSelfRef.rv_core__DOT__u_csr__DOT__trap = vlSelfRef.rv_core__DOT__trap;
    rv_core__DOT____Vcellinp__u_csr__commit = ((~ (IData)(vlSelfRef.rv_core__DOT__trap)) 
                                               & (IData)(vlSelfRef.rv_core__DOT__ex_fire));
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__trap 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__trap;
    vlSelfRef.rv_core__DOT__u_csr__DOT__retire = rv_core__DOT____Vcellinp__u_csr__commit;
    vlSelfRef.rv_core__DOT__u_csr__DOT__commit = rv_core__DOT____Vcellinp__u_csr__commit;
    vlSelfRef.rv_core__DOT__mret = ((vlSelfRef.rv_core__DOT__ex_dec_q[0U] 
                                     >> 1U) & (IData)(rv_core__DOT____Vcellinp__u_csr__commit));
    vlSelfRef.rv_core__DOT__u_csr__DOT__do_write = 
        ((0U != (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__op)) 
         & ((IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__commit) 
            & (IData)(vlSelfRef.rv_core__DOT__u_csr__DOT__is_write)));
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__mret 
        = vlSelfRef.rv_core__DOT__mret;
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
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__do_write 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__do_write;
    vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mret 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__mret;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect 
        = vlSelfRef.rv_core__DOT__redirect;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__redirect 
        = vlSelfRef.rv_core__DOT__redirect;
    vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect_pc 
        = vlSelfRef.rv_core__DOT__redirect_pc;
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__redirect_pc 
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
    vlSelfRef.rv_core__DOT__u_core_defs__DOT__stall_front 
        = vlSelfRef.rv_core__DOT__stall_front;
}

void Vtop___024root___eval_nba(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_nba\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __Vinline__nba_sequent__TOP__5___Vdly__rv_core__DOT__u_fetch__DOT__pc_q;
    __Vinline__nba_sequent__TOP__5___Vdly__rv_core__DOT__u_fetch__DOT__pc_q = 0;
    CData/*0:0*/ __Vinline__nba_sequent__TOP__5___Vdly__rv_core__DOT__u_fetch__DOT__pend_q;
    __Vinline__nba_sequent__TOP__5___Vdly__rv_core__DOT__u_fetch__DOT__pend_q = 0;
    IData/*31:0*/ __Vinline__nba_sequent__TOP__9___VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v0;
    __Vinline__nba_sequent__TOP__9___VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v0 = 0;
    CData/*4:0*/ __Vinline__nba_sequent__TOP__9___VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v0;
    __Vinline__nba_sequent__TOP__9___VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v0 = 0;
    IData/*31:0*/ __Vinline__nba_sequent__TOP__9___VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v1;
    __Vinline__nba_sequent__TOP__9___VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v1 = 0;
    CData/*4:0*/ __Vinline__nba_sequent__TOP__9___VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v1;
    __Vinline__nba_sequent__TOP__9___VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v1 = 0;
    CData/*0:0*/ __Vinline__nba_comb__TOP__4_rv_core__DOT____Vcellinp__u_csr__commit;
    __Vinline__nba_comb__TOP__4_rv_core__DOT____Vcellinp__u_csr__commit = 0;
    // Body
    if ((0x0000000000000800ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__0(vlSelf);
    }
    if ((0x0000000000008000ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__1(vlSelf);
    }
    if ((0x0000000000004000ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__2(vlSelf);
    }
    if ((0x0000000000002000ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__3(vlSelf);
    }
    if ((0x0000000000000100ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__4(vlSelf);
    }
    if ((0x000000000000000cULL & vlSelfRef.__VnbaTriggered[0U])) {
        __Vinline__nba_sequent__TOP__5___Vdly__rv_core__DOT__u_fetch__DOT__pend_q 
            = vlSelfRef.rv_core__DOT__u_fetch__DOT__pend_q;
        __Vinline__nba_sequent__TOP__5___Vdly__rv_core__DOT__u_fetch__DOT__pc_q 
            = vlSelfRef.rv_core__DOT__u_fetch__DOT__pc_q;
        if (vlSelfRef.rv_core__DOT__u_fetch__DOT__rst_n) {
            if (vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect) {
                if (vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_ready) {
                    __Vinline__nba_sequent__TOP__5___Vdly__rv_core__DOT__u_fetch__DOT__pc_q 
                        = vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect_pc;
                    __Vinline__nba_sequent__TOP__5___Vdly__rv_core__DOT__u_fetch__DOT__pend_q = 0U;
                } else {
                    __Vinline__nba_sequent__TOP__5___Vdly__rv_core__DOT__u_fetch__DOT__pend_q = 1U;
                }
                if ((1U & (~ (IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_ready)))) {
                    vlSelfRef.rv_core__DOT__u_fetch__DOT__pend_pc_q 
                        = vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect_pc;
                }
            } else if (vlSelfRef.rv_core__DOT__u_fetch__DOT__pend_q) {
                if (vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_ready) {
                    __Vinline__nba_sequent__TOP__5___Vdly__rv_core__DOT__u_fetch__DOT__pc_q 
                        = vlSelfRef.rv_core__DOT__u_fetch__DOT__pend_pc_q;
                    __Vinline__nba_sequent__TOP__5___Vdly__rv_core__DOT__u_fetch__DOT__pend_q = 0U;
                }
            } else if ((((IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__started_q) 
                         & (IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_ready)) 
                        & (~ (IData)(vlSelfRef.rv_core__DOT__u_fetch__DOT__stall)))) {
                __Vinline__nba_sequent__TOP__5___Vdly__rv_core__DOT__u_fetch__DOT__pc_q 
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
            __Vinline__nba_sequent__TOP__5___Vdly__rv_core__DOT__u_fetch__DOT__pc_q = 0U;
            __Vinline__nba_sequent__TOP__5___Vdly__rv_core__DOT__u_fetch__DOT__pend_q = 0U;
            vlSelfRef.rv_core__DOT__u_fetch__DOT__id_pc_q = 0U;
            vlSelfRef.rv_core__DOT__u_fetch__DOT__id_valid_q = 0U;
            vlSelfRef.rv_core__DOT__u_fetch__DOT__id_instr_q = 0x00000013U;
            vlSelfRef.rv_core__DOT__u_fetch__DOT__pend_pc_q = 0U;
        }
        vlSelfRef.rv_core__DOT__u_fetch__DOT__pc_q 
            = __Vinline__nba_sequent__TOP__5___Vdly__rv_core__DOT__u_fetch__DOT__pc_q;
        vlSelfRef.rv_core__DOT__u_fetch__DOT__pend_q 
            = __Vinline__nba_sequent__TOP__5___Vdly__rv_core__DOT__u_fetch__DOT__pend_q;
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
        vlSelfRef.rv_core__DOT__u_top_if__DOT__imem_addr 
            = vlSelfRef.rv_core__DOT__imem_addr;
        vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_valid 
            = vlSelfRef.rv_core__DOT__u_fetch__DOT__started_q;
        vlSelfRef.rv_core__DOT__imem_valid = vlSelfRef.rv_core__DOT__u_fetch__DOT__imem_valid;
        vlSelfRef.imem_valid = vlSelfRef.rv_core__DOT__imem_valid;
        vlSelfRef.rv_core__DOT__u_top_if__DOT__imem_valid 
            = vlSelfRef.rv_core__DOT__imem_valid;
    }
    if ((0x00000000000000c0ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__6(vlSelf);
    }
    if ((0x0000000000000600ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__7(vlSelf);
    }
    if ((0x0000000000001800ULL & vlSelfRef.__VnbaTriggered[0U])) {
        if (vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__rst_n) {
            if (vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__start) {
                vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__a_ref_q 
                    = vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__a;
            }
        } else {
            vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__a_ref_q = 0U;
        }
    }
    if ((0x0000000000000030ULL & vlSelfRef.__VnbaTriggered[0U])) {
        if (vlSelfRef.rv_core__DOT__u_regfile__DOT__rst_n) {
            if (((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__we) 
                 & (0U != (IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__waddr)))) {
                vlSelfRef.rv_core__DOT__u_regfile__DOT____Vlvbound_hf0d5df5e__0 
                    = vlSelfRef.rv_core__DOT__u_regfile__DOT__wdata;
                if ((0x1eU >= (0x0000001fU & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__waddr) 
                                              - (IData)(1U))))) {
                    __Vinline__nba_sequent__TOP__9___VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v0 
                        = vlSelfRef.rv_core__DOT__u_regfile__DOT____Vlvbound_hf0d5df5e__0;
                    __Vinline__nba_sequent__TOP__9___VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v0 
                        = (0x0000001fU & ((IData)(vlSelfRef.rv_core__DOT__u_regfile__DOT__waddr) 
                                          - (IData)(1U)));
                    vlSelfRef.__VdlyCommitQueuerv_core__DOT__u_regfile__DOT__regs_q.enqueue(__Vinline__nba_sequent__TOP__9___VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v0, __Vinline__nba_sequent__TOP__9___VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v0);
                }
            }
        } else {
            vlSelfRef.rv_core__DOT__u_regfile__DOT__unnamedblk1__DOT__i = 1U;
            while (VL_GTS_III(32, 0x00000020U, vlSelfRef.rv_core__DOT__u_regfile__DOT__unnamedblk1__DOT__i)) {
                vlSelfRef.rv_core__DOT__u_regfile__DOT____Vlvbound_he35c63e1__0 = 0U;
                if (VL_LIKELY(((0x1eU >= (0x0000001fU 
                                          & (vlSelfRef.rv_core__DOT__u_regfile__DOT__unnamedblk1__DOT__i 
                                             - (IData)(1U))))))) {
                    __Vinline__nba_sequent__TOP__9___VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v1 
                        = vlSelfRef.rv_core__DOT__u_regfile__DOT____Vlvbound_he35c63e1__0;
                    __Vinline__nba_sequent__TOP__9___VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v1 
                        = (0x0000001fU & (vlSelfRef.rv_core__DOT__u_regfile__DOT__unnamedblk1__DOT__i 
                                          - (IData)(1U)));
                    vlSelfRef.__VdlyCommitQueuerv_core__DOT__u_regfile__DOT__regs_q.enqueue(__Vinline__nba_sequent__TOP__9___VdlyVal__rv_core__DOT__u_regfile__DOT__regs_q__v1, __Vinline__nba_sequent__TOP__9___VdlyDim0__rv_core__DOT__u_regfile__DOT__regs_q__v1);
                }
                vlSelfRef.rv_core__DOT__u_regfile__DOT__unnamedblk1__DOT__i 
                    = ((IData)(1U) + vlSelfRef.rv_core__DOT__u_regfile__DOT__unnamedblk1__DOT__i);
            }
        }
        vlSelfRef.__VdlyCommitQueuerv_core__DOT__u_regfile__DOT__regs_q.commit(vlSelfRef.rv_core__DOT__u_regfile__DOT__regs_q);
    }
    if ((3ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__10(vlSelf);
    }
    if ((0x0000000000000600ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.rv_core__DOT__mdu_done = vlSelfRef.rv_core__DOT__u_muldiv__DOT__done;
        vlSelfRef.rv_core__DOT__u_core_defs__DOT__mdu_done 
            = vlSelfRef.rv_core__DOT__mdu_done;
    }
    if ((0x000000000000000cULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__12(vlSelf);
    }
    if ((0x00000000000000c3ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_comb__TOP__0(vlSelf);
    }
    if ((0x0000000000000603ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.rv_core__DOT__mdu_busy = ((~ (IData)(vlSelfRef.rv_core__DOT__mdu_done)) 
                                            & (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_1));
        vlSelfRef.rv_core__DOT__u_core_defs__DOT__mdu_busy 
            = vlSelfRef.rv_core__DOT__mdu_busy;
        vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2 = 
            ((IData)(vlSelfRef.rv_core__DOT__mem_stall) 
             | (IData)(vlSelfRef.rv_core__DOT__mdu_busy));
        vlSelfRef.rv_core__DOT__ex_fire = ((~ (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2)) 
                                           & (IData)(vlSelfRef.rv_core__DOT__ex_valid_q));
        vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_fire 
            = vlSelfRef.rv_core__DOT__ex_fire;
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
        vlSelfRef.rv_core__DOT__u_core_defs__DOT__load_use 
            = vlSelfRef.rv_core__DOT__load_use;
    }
    if ((0x00000000000006c3ULL & vlSelfRef.__VnbaTriggered[0U])) {
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
        vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_result 
            = vlSelfRef.rv_core__DOT__ex_result;
        vlSelfRef.rv_core__DOT__u_core_defs__DOT__trap 
            = vlSelfRef.rv_core__DOT__trap;
        vlSelfRef.rv_core__DOT__u_csr__DOT__trap = vlSelfRef.rv_core__DOT__trap;
        __Vinline__nba_comb__TOP__4_rv_core__DOT____Vcellinp__u_csr__commit 
            = ((~ (IData)(vlSelfRef.rv_core__DOT__trap)) 
               & (IData)(vlSelfRef.rv_core__DOT__ex_fire));
        vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__trap 
            = vlSelfRef.rv_core__DOT__u_csr__DOT__trap;
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
        vlSelfRef.rv_core__DOT__u_core_defs__DOT__mret 
            = vlSelfRef.rv_core__DOT__mret;
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
        vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__do_write 
            = vlSelfRef.rv_core__DOT__u_csr__DOT__do_write;
        vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mret 
            = vlSelfRef.rv_core__DOT__u_csr__DOT__mret;
        vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect 
            = vlSelfRef.rv_core__DOT__redirect;
        vlSelfRef.rv_core__DOT__u_core_defs__DOT__redirect 
            = vlSelfRef.rv_core__DOT__redirect;
        vlSelfRef.rv_core__DOT__u_fetch__DOT__redirect_pc 
            = vlSelfRef.rv_core__DOT__redirect_pc;
        vlSelfRef.rv_core__DOT__u_core_defs__DOT__redirect_pc 
            = vlSelfRef.rv_core__DOT__redirect_pc;
    }
    if ((0x000000000000060fULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.rv_core__DOT__stall_front = ((IData)(vlSelfRef.rv_core__DOT__load_use) 
                                               | (IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_2));
        vlSelfRef.rv_core__DOT__u_fetch__DOT__stall 
            = vlSelfRef.rv_core__DOT__stall_front;
        vlSelfRef.rv_core__DOT__u_core_defs__DOT__stall_front 
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

void Vtop___024root___sample(Vtop___024root* vlSelf);

void Vtop___024root___eval(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VicoIterCount;
    IData/*31:0*/ __VnbaIterCount;
    // Body
    Vtop___024root___sample(vlSelf);
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

void Vtop___024root___sample(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___sample\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__rst_n;
    __Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__rst_n = 0;
    // Body
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mie_q 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mie_q;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mpie_q 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mpie_q;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mepc_q 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mepc_q;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__trap 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__trap;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mret 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__mret;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__do_write 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__do_write;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__addr 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__addr;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__wr_val 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__wr_val;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__trap_pc 
        = vlSelfRef.rv_core__DOT__u_csr__DOT__u_csr_defs__DOT__trap_pc;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__done 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__done;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__op_q 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__op_q;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__op_b_q 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__op_b_q;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__result 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__result;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__a_ref_q 
        = vlSelfRef.rv_core__DOT__u_muldiv__DOT__u_muldiv_defs__DOT__a_ref_q;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_stall 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_stall;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__dmem_valid 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__dmem_valid;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__dmem_ready 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__dmem_ready;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mdu_busy 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__mdu_busy;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_valid_q 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_valid_q;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[0U] 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_dec_q[0U];
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[1U] 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_dec_q[1U];
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_dec_q[2U] 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_dec_q[2U];
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mdu_done 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__mdu_done;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__stall_front 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__stall_front;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__load_use 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__load_use;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_fire 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_fire;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mdu_start 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__mdu_start;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mdu_active_q 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__mdu_active_q;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__trap 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__trap;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__irq_pending 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__irq_pending;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__exc 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__exc;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mret 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__mret;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__redirect 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__redirect;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__flow_change 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__flow_change;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__redirect_pc 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__redirect_pc;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__csr_mtvec 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__csr_mtvec;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__csr_mepc 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__csr_mepc;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__target 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__target;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_link 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_link;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_pc_q 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_pc_q;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__branch_taken 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__branch_taken;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__target_misaligned 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__target_misaligned;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__alu_a 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__alu_a;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__fwd_rs1 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__fwd_rs1;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__alu_b 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__alu_b;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__fwd_rs2 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__fwd_rs2;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_result 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_result;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__csr_rdata 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__csr_rdata;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mdu_result 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__mdu_result;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__alu_result 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__alu_result;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_final 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_final;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_is_load_q 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_is_load_q;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__load_data 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__load_data;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_result_q 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_result_q;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ls_misaligned 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__ls_misaligned;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_valid_q 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_valid_q;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_wb_en_q 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_wb_en_q;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__mem_rd_q 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__mem_rd_q;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_valid_q 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__wb_valid_q;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_en_q 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__wb_en_q;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_rd_q 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__wb_rd_q;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__wb_data_q 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__wb_data_q;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_rs1_q 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_rs1_q;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ex_rs2_q 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__ex_rs2_q;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_valid 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__id_valid;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_dec[0U] 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__id_dec[0U];
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_dec[1U] 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__id_dec[1U];
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_dec[2U] 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__id_dec[2U];
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__ld_ref 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__ld_ref;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_core_defs__DOT__id_instr 
        = vlSelfRef.rv_core__DOT__u_core_defs__DOT__id_instr;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_valid 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_valid;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_rs1 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_rs1;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_instr 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_instr;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_rs2 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_rs2;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_rd 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_rd;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_funct3 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_funct3;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_opcode 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_opcode;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_imm 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_imm;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_funct7 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_funct7;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_funct3 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_funct3;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_opcode 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_opcode;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_b_ref 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_b_ref;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_a 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_a;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_valid 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_valid;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_alu 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_alu;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_b 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_b;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_taken 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_taken;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_instr 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_instr;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_target 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_target;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__ex_pc 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__ex_pc;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__mem_addr_isa 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__mem_addr_isa;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__id_pc 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__id_pc;
    __Vsampled_TOP__rv_core__DOT__u_stage_sva__DOT__rst_n 
        = vlSelfRef.rv_core__DOT__u_stage_sva__DOT__rst_n;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__imem_valid 
        = vlSelfRef.rv_core__DOT__u_top_if__DOT__imem_valid;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__imem_addr 
        = vlSelfRef.rv_core__DOT__u_top_if__DOT__imem_addr;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_valid 
        = vlSelfRef.rv_core__DOT__u_top_if__DOT__dmem_valid;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_write 
        = vlSelfRef.rv_core__DOT__u_top_if__DOT__dmem_write;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_addr 
        = vlSelfRef.rv_core__DOT__u_top_if__DOT__dmem_addr;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_wstrb 
        = vlSelfRef.rv_core__DOT__u_top_if__DOT__dmem_wstrb;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_wdata 
        = vlSelfRef.rv_core__DOT__u_top_if__DOT__dmem_wdata;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__rst_n 
        = vlSelfRef.rv_core__DOT__u_top_if__DOT__rst_n;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__imem_ready 
        = vlSelfRef.rv_core__DOT__u_top_if__DOT__imem_ready;
    vlSelfRef.__Vsampled_TOP__rv_core__DOT__u_top_if__DOT__dmem_ready 
        = vlSelfRef.rv_core__DOT__u_top_if__DOT__dmem_ready;
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
