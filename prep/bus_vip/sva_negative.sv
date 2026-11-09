// Deliberately non-compliant APB4 master, used to prove that the bound protocol
// assertions are actually live (a green run must mean "no violation", not "assertions
// were dropped by the simulator").
//
// Violation: PENABLE stays high for extra ACCESS cycles after PREADY, which A4
// ("ACCESS continued after PREADY without a new SETUP") must catch.

module apb_bad_master (
    input  logic        clk,
    input  logic        rst_n,
    output logic        psel,
    output logic        penable,
    output logic        pwrite,
    output logic [31:0] paddr,
    output logic [31:0] pwdata,
    output logic [3:0]  pstrb,
    output logic [3:0]  pprot,
    input  logic        pready,
    input  logic [31:0] prdata,
    input  logic        pslverr
);
    logic [3:0] cyc;

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            cyc     <= 4'd0;
            psel    <= 1'b0;
            penable <= 1'b0;
            pwrite  <= 1'b0;
            paddr   <= 32'd0;
            pwdata  <= 32'd0;
            pstrb   <= 4'd0;
            pprot   <= 4'd0;
        end else begin
            if (cyc != 4'd15) cyc <= cyc + 4'd1;
            psel    <= (cyc != 4'd0);
            penable <= (cyc >= 4'd2);      // stays high forever: illegal
            pwrite  <= 1'b1;
            paddr   <= 32'h0;
            pwdata  <= 32'hCAFE_BABE;
            pstrb   <= 4'hF;
            pprot   <= 4'h0;
        end
    end
endmodule


module apb_negative_harness (
    input logic clk,
    input logic rst_n
);
    logic        psel, penable, pwrite, pready, pslverr;
    logic [31:0] paddr, pwdata, prdata;
    logic [3:0]  pstrb, pprot;

    apb_bad_master u_bad (
        .clk, .rst_n, .psel, .penable, .pwrite, .paddr, .pwdata, .pstrb, .pprot,
        .pready, .prdata, .pslverr
    );

    apb_regs_dut u_slv (
        .clk, .rst_n, .psel, .penable, .pwrite, .paddr, .pwdata, .pstrb, .pprot,
        .pready, .prdata, .pslverr
    );

    bind apb_regs_dut apb4_protocol_sva u_apb_sva (.*);
endmodule
