// Synthetic design used only to exercise the Proving Ground flow end-to-end
// before the real challenge designs are released. Not part of any submission.
module fifo #(
    parameter int DEPTH = 8,
    parameter int WIDTH = 32,
    parameter int AW    = 3
) (
    input  logic             clk,
    input  logic             rst_n,
    input  logic             wr_en,
    input  logic             rd_en,
    input  logic [WIDTH-1:0] wdata,
    output logic [WIDTH-1:0] rdata,
    output logic [AW:0]      count,
    output logic             full,
    output logic             empty,
    output logic             overflow,
    output logic             underflow
);

    logic [WIDTH-1:0] mem [0:DEPTH-1];
    logic [AW-1:0]    wr_ptr;
    logic [AW-1:0]    rd_ptr;

    wire wfire = wr_en && !full;
    wire rdfe = rd_en && !empty;

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            wr_ptr <= '0;
            rd_ptr <= '0;
            count  <= '0;
        end else begin
            if (wfire) begin
                mem[wr_ptr] <= wdata;
                wr_ptr      <= wr_ptr + 1'b1;
                count       <= count + 1'b1;
            end
            if (rdfe) begin
                rd_ptr <= rd_ptr + 1'b1;
                count  <= count - 1'b1;
            end
        end
    end

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            rdata <= '0;
        end else if (rdfe) begin
            rdata <= mem[rd_ptr];
        end
    end

    always_comb begin
        full      = (count == DEPTH[AW:0]);
        empty     = (count == '0);
        overflow  = wr_en && full;
        underflow = rd_en && empty;
    end

endmodule
