// rv_muldiv divide-by-zero result pins (REQ-30; shadow dividend kills D4 rem-by-zero)
/* verilator lint_off SYNCASYNCNET */
module rv_muldiv_defs (
    input  logic        clk,
    input  logic        rst_n,
    input  logic        start,
    input  logic [31:0] a,
    input  logic        done,
    input  logic [2:0]  op_q,
    input  logic [31:0] op_b_q,
    input  logic [31:0] result
);
    default clocking cb @(posedge clk); endclocking
    default disable iff (!rst_n);

    logic [31:0] a_ref_q;
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) a_ref_q <= 32'd0;
        else if (start) a_ref_q <= a;
    end

    A_MDU_REM0: assert property (
        done && ((op_q == 3'b110) || (op_q == 3'b111)) && (op_b_q == 32'd0)
            |-> (result == a_ref_q));

    A_MDU_DIV0: assert property (
        done && ((op_q == 3'b100) || (op_q == 3'b101)) && (op_b_q == 32'd0)
            |-> (result == 32'hFFFF_FFFF));

endmodule
/* verilator lint_on SYNCASYNCNET */

bind rv_muldiv rv_muldiv_defs u_muldiv_defs (
    .clk    (clk),
    .rst_n  (rst_n),
    .start  (start),
    .a      (a),
    .done   (done),
    .op_q   (op_q),
    .op_b_q (op_b_q),
    .result (result)
);
