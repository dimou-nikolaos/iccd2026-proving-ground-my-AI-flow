module dsp_fifo_sva #(
  parameter int WIDTH = 18,
  parameter int DEPTH = 8
) (
  input  logic             clk,
  input  logic             rst_n,
  input  logic             push_valid,
  input  logic             push_ready,
  input  logic [WIDTH-1:0] push_data,
  input  logic             pop_valid,
  input  logic             pop_ready,
  input  logic [WIDTH-1:0] pop_data,
  input  logic [WIDTH-1:0] mem_q [DEPTH],
  input  logic [$clog2(DEPTH)-1:0] wptr_q,
  input  logic [$clog2(DEPTH)-1:0] rptr_q,
  input  logic [$clog2(DEPTH):0]   count_q,
  input  logic             do_push,
  input  logic             do_pop
);

  localparam int AW = $clog2(DEPTH);

  logic        p_do_push, p_do_pop, p_rst = 1'b0;
  logic [AW-1:0] p_wptr, p_rptr;
  logic [AW:0] p_count;
  logic [WIDTH-1:0] p_push_data;
  logic [WIDTH-1:0] p_mem [DEPTH];

  always_comb begin
    if (!$isunknown({push_valid, push_ready, push_data, pop_valid, pop_ready, pop_data,
                     wptr_q, rptr_q, count_q, do_push, do_pop})) begin
      a_pushready_def: assert (push_ready == (count_q != (AW+1)'(DEPTH)))
        else $error("fifo push_ready def wrong");
      a_popvalid_def: assert (pop_valid == (count_q != '0))
        else $error("fifo pop_valid def wrong");
      a_popdata_def: assert (pop_data == mem_q[rptr_q])
        else $error("fifo pop_data def wrong");
      a_dopush_def: assert (do_push == (push_valid && push_ready))
        else $error("fifo do_push def wrong");
      a_dopop_def: assert (do_pop == (pop_valid && pop_ready))
        else $error("fifo do_pop def wrong");
    end
  end

  always @(posedge clk) begin
    if (p_rst && rst_n) begin
      a_wptr_ns: assert (wptr_q == (p_do_push
                    ? ((p_wptr == AW'(DEPTH-1)) ? '0 : (p_wptr + 1'b1))
                    : p_wptr))
        else $error("fifo wptr_q next-state wrong");
      a_rptr_ns: assert (rptr_q == (p_do_pop
                    ? ((p_rptr == AW'(DEPTH-1)) ? '0 : (p_rptr + 1'b1))
                    : p_rptr))
        else $error("fifo rptr_q next-state wrong");
      a_count_ns: assert (count_q == ((p_do_push && !p_do_pop) ? (p_count + 1'b1)
                           : ((p_do_pop && !p_do_push) ? (p_count - 1'b1)
                           : p_count)))
        else $error("fifo count_q next-state wrong");
      for (int g = 0; g < DEPTH; g++) begin
        a_mem_ns: assert (mem_q[g] == ((p_do_push && (p_wptr == AW'(g))) ? p_push_data : p_mem[g]))
          else $error("fifo mem next-state wrong");
      end
    end
    p_do_push   <= do_push;
    p_do_pop    <= do_pop;
    p_wptr      <= wptr_q;
    p_rptr      <= rptr_q;
    p_count     <= count_q;
    p_push_data <= push_data;
    for (int i = 0; i < DEPTH; i++) begin
      p_mem[i] <= mem_q[i];
    end
    p_rst       <= rst_n;
  end

endmodule

bind dsp_fifo dsp_fifo_sva #(.WIDTH(WIDTH), .DEPTH(DEPTH)) u_dsp_fifo_sva (.*);
