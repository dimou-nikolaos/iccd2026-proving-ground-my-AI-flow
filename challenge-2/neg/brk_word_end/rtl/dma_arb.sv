// dma_arb: highest priority first, round robin among channels of equal priority.
module dma_arb (
  input  logic [dma_pkg::NUM_CH-1:0] eligible,
  input  logic [1:0]                 prio [dma_pkg::NUM_CH],
  input  logic [1:0]                 last,
  output logic                       grant_valid,
  output logic [1:0]                 grant_idx
);

  logic [1:0] start;
  logic [2:0] best_prio;

  assign start = last + 2'd1;

  always_comb begin
    logic [1:0] cand;
    grant_valid = 1'b0;
    grant_idx   = 2'd0;
    best_prio   = 3'd0;
    for (int i = 0; i < dma_pkg::NUM_CH; i++) begin
      cand = start + 2'(i);
      if (eligible[cand] && (!grant_valid || ({1'b0, prio[cand]} > best_prio))) begin
        grant_valid = 1'b1;
        grant_idx   = cand;
        best_prio   = {1'b0, prio[cand]};
      end
    end
  end

endmodule
