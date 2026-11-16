// rv_csr next-state identities for MIE/MPIE/MEPC (REQ-35/41; kills D2 MPIE-latch omission)
/* verilator lint_off SYNCASYNCNET */
module rv_csr_defs (
    input  logic        clk,
    input  logic        rst_n,
    input  logic        trap,
    input  logic [31:0] trap_pc,
    input  logic        mret,
    input  logic        do_write,
    input  logic [11:0] addr,
    input  logic [31:0] wr_val,
    input  logic        mie_q,
    input  logic        mpie_q,
    input  logic [31:2] mepc_q
);
    import rv_pkg::*;

    default clocking cb @(posedge clk); endclocking
    default disable iff (!rst_n);

    A_CSR_MIE:  assert property (mie_q == ($past(trap) ? 1'b0 :
                    ($past(mret) ? $past(mpie_q) :
                    (($past(do_write) && ($past(addr) == CSR_MSTATUS)) ? $past(wr_val[3]) : $past(mie_q)))));

    A_CSR_MPIE: assert property (mpie_q == ($past(trap) ? $past(mie_q) :
                    ($past(mret) ? 1'b1 :
                    (($past(do_write) && ($past(addr) == CSR_MSTATUS)) ? $past(wr_val[7]) : $past(mpie_q)))));

    A_CSR_MEPC: assert property (mepc_q == ($past(trap) ? $past(trap_pc)[31:2] :
                    (($past(do_write) && ($past(addr) == CSR_MEPC)) ? $past(wr_val)[31:2] : $past(mepc_q))));

endmodule
/* verilator lint_on SYNCASYNCNET */

bind rv_csr rv_csr_defs u_csr_defs (
    .clk     (clk),
    .rst_n   (rst_n),
    .trap    (trap),
    .trap_pc (trap_pc),
    .mret    (mret),
    .do_write(do_write),
    .addr    (addr),
    .wr_val  (wr_val),
    .mie_q   (mie_q),
    .mpie_q  (mpie_q),
    .mepc_q  (mepc_q)
);
