// Structural invariants for the synthetic flowcheck design.
//
// Two binds:
//   * into every `fifo` instance, using internal signals (count/full/empty/
//     wr_fire/rd_fire/rdata) — this is how a real design's assertions reach
//     state that is not on the top-level port list;
//   * into the top module for the exported status flags.
module fifo_internal_sva #(
    parameter int DEPTH = 8,
    parameter int AW    = 3
) (
    input logic        clk,
    input logic        rst_n,
    input logic [AW:0] count,
    input logic        full,
    input logic        empty,
    input logic        wr_fire,
    input logic        rd_fire,
    input logic [31:0] rdata
);

    a_count_range: assert property (@(posedge clk) disable iff (!rst_n)
        count <= DEPTH[AW:0]);

    a_empty_def: assert property (@(posedge clk) disable iff (!rst_n)
        empty == (count == '0));

    a_full_def: assert property (@(posedge clk) disable iff (!rst_n)
        full == (count == DEPTH[AW:0]));

    // one credit per accepted write, one debit per accepted read.
    // SVA samples preponed (pre-edge) values: `count` here is the value *before*
    // the edge, so the write/read that produced it are last cycle's events.
    a_count_step: assert property (@(posedge clk) disable iff (!rst_n)
        count == ($past(count) + $past(wr_fire ? 1 : 0) - $past(rd_fire ? 1 : 0)));

    // rdata only changes when the FIFO was actually read at the previous edge
    // ($stable compares this cycle with the previous one, so the enabling read
    // is $past(rd_fire), not rd_fire).
    a_rdata_stable: assert property (@(posedge clk) disable iff (!rst_n)
        !$stable(rdata) |-> $past(rd_fire));

endmodule

module fifo_status_sva (
    input logic clk,
    input logic rst_n,
    input logic wr_en,
    input logic rd_en,
    input logic full,
    input logic empty,
    input logic overflow,
    input logic underflow
);

    a_overflow_def: assert property (@(posedge clk) disable iff (!rst_n)
        overflow == (wr_en && full));

    a_underflow_def: assert property (@(posedge clk) disable iff (!rst_n)
        underflow == (rd_en && empty));

    a_full_empty: assert property (@(posedge clk) disable iff (!rst_n)
        !(full && empty));

endmodule

bind fifo fifo_internal_sva #(.DEPTH(8), .AW(3)) u_internal_sva (
    .clk     (clk),
    .rst_n   (rst_n),
    .count   (count),
    .full    (full),
    .empty   (empty),
    .wr_fire (wr_fire),
    .rd_fire (rd_fire),
    .rdata   (rdata)
);

bind flowcheck_top fifo_status_sva u_status_sva (
    .clk       (clk),
    .rst_n     (rst_n),
    .wr_en     (wr_en),
    .rd_en     (rd_en),
    .full      (full),
    .empty     (empty),
    .overflow  (overflow),
    .underflow (underflow)
);
