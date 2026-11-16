// port-level interface protocol invariants for rv_core (REQ-02/03/04/07/08)
/* verilator lint_off SYNCASYNCNET */
module rv_top_if (
    input logic        clk,
    input logic        rst_n,
    input logic        imem_valid,
    input logic        imem_ready,
    input logic [31:0] imem_addr,
    input logic        dmem_valid,
    input logic        dmem_ready,
    input logic        dmem_write,
    input logic [31:0] dmem_addr,
    input logic [3:0]  dmem_wstrb,
    input logic [31:0] dmem_wdata
);
    default clocking cb @(posedge clk); endclocking

    A_IMEM_HOLD: assert property (
        @(posedge clk) disable iff (!rst_n)
        (imem_valid && !imem_ready) |=> (imem_valid && $stable(imem_addr)));

    A_DMEM_HOLD: assert property (
        @(posedge clk) disable iff (!rst_n)
        (dmem_valid && !dmem_ready) |=>
            (dmem_valid && $stable(dmem_write) && $stable(dmem_addr) && $stable(dmem_wstrb)));

    A_DMEM_WDATA_HOLD: assert property (
        @(posedge clk) disable iff (!rst_n)
        (dmem_valid && !dmem_ready && dmem_write) |=> $stable(dmem_wdata));

    A_LOAD_WSTRB: assert property (
        @(posedge clk) disable iff (!rst_n)
        (dmem_valid && !dmem_write) |-> (dmem_wstrb == 4'b0000));

    A_IMEM_ALIGN: assert property (
        @(posedge clk) disable iff (!rst_n)
        imem_valid |-> (imem_addr[1:0] == 2'b00));

    A_DMEM_ALIGN: assert property (
        @(posedge clk) disable iff (!rst_n)
        dmem_valid |-> (dmem_addr[1:0] == 2'b00));

    A_RST_DMEM_FIRST: assert property (
        @(posedge clk)
        (rst_n && $past(rst_n) === 1'b0) |-> !dmem_valid);

    A_FIRST_FETCH_ZERO: assert property (
        @(posedge clk) disable iff (!rst_n)
        $rose(imem_valid) |-> (imem_addr == 32'd0));

endmodule
/* verilator lint_on SYNCASYNCNET */

bind rv_core rv_top_if u_top_if (
    .clk        (clk),
    .rst_n      (rst_n),
    .imem_valid (imem_valid),
    .imem_ready (imem_ready),
    .imem_addr  (imem_addr),
    .dmem_valid (dmem_valid),
    .dmem_ready (dmem_ready),
    .dmem_write (dmem_write),
    .dmem_addr  (dmem_addr),
    .dmem_wstrb (dmem_wstrb),
    .dmem_wdata (dmem_wdata)
);
