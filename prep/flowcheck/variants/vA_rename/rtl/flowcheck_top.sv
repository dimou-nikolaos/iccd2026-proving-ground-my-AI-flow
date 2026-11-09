// Synthetic top: FIFO plus a saturating write counter, so the flow sees
// arithmetic, relational, constant and conditional mutation sites.
module flowcheck_top (
    input  logic         clk,
    input  logic         rst_n,
    input  logic         wr_en,
    input  logic         rd_en,
    input  logic [31:0]  wdata,
    output logic [31:0]  rdata,
    output logic [3:0]   count,
    output logic         full,
    output logic         empty,
    output logic         overflow,
    output logic         underflow,
    output logic [15:0]  total_written
);

    logic [15:0] written_q;

    fifo u_fifo (
        .clk       (clk),
        .rst_n     (rst_n),
        .wr_en     (wr_en),
        .rd_en     (rd_en),
        .wdata     (wdata),
        .rdata     (rdata),
        .count     (count),
        .full      (full),
        .empty     (empty),
        .overflow  (overflow),
        .underflow (underflow)
    );

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            written_q <= '0;
        end else if (wr_en && !full) begin
            if (written_q != 16'hFFFF) begin
                written_q <= written_q + 16'h0001;
            end
        end
    end

    assign total_written = written_q;

endmodule
