// Feature probe (NOT shipped): Verilator 5.048 support checks for the constructs
// we plan to use in assertions/:
// - bind to a submodule referencing internal signals (incl. enum-typed state_q)
// - unpacked-array ports in an observer, bound by explicit expression map
// - golden always_comb replica inside an observer module
// - $past on complex expressions, inside {}, $onehot0

module fp_engine_sva (
  input logic        clk,
  input logic        rst_n,
  input logic [3:0]  state_q,
  input logic [3:0]  eligible,
  input logic        grant_valid,
  input logic [1:0]  grant_idx,
  input logic [1:0]  last,
  input logic [1:0]  prio [4],
  input logic [31:0] ch_dst [4],
  input logic [15:0] ch_len [4],
  input logic [1:0]  own_q
);
  default clocking cb @(posedge clk); endclocking
  default disable iff (!rst_n);

  a_state_legal: assert property (state_q inside {4'd0,4'd1,4'd2,4'd3,4'd4,4'd5,4'd6,4'd7,4'd8,4'd9,4'd10})
    else $error("FP: illegal state encoding");

  // golden replica of the arbiter selection
  logic        gv_ref;
  logic [1:0]  gi_ref;
  logic [2:0]  bp_ref;
  logic [1:0]  start_ref;
  always_comb begin
    logic [1:0] cand;
    gv_ref = 1'b0;
    gi_ref = 2'd0;
    bp_ref = 3'd0;
    start_ref = last + 2'd1;
    for (int i = 0; i < 4; i++) begin
      cand = start_ref + 2'(i);
      if (eligible[cand] && (!gv_ref || ({1'b0, prio[cand]} > bp_ref))) begin
        gv_ref = 1'b1;
        gi_ref = cand;
        bp_ref = {1'b0, prio[cand]};
      end
    end
  end
  a_grant_golden: assert property (grant_valid == gv_ref && grant_idx == gi_ref)
    else $error("FP: grant != golden rotating-priority model");

  a_len_past: assert property ($past(ch_len[grant_idx]) == $past(ch_len[grant_idx]))
    else $error("FP: $past on indexed array broken");

  a_onehot: assert property ($onehot0({grant_valid, own_q[0]}))
    else $error("FP: onehot0 broken");
endmodule

bind dma_engine fp_engine_sva u_fp_engine_sva (
  .clk(clk), .rst_n(rst_n), .state_q(state_q), .eligible(eligible),
  .grant_valid(grant_valid), .grant_idx(grant_idx), .last(last_q), .prio(ch_prio),
  .ch_dst(ch_dst), .ch_len(ch_len), .own_q(own_q)
);
