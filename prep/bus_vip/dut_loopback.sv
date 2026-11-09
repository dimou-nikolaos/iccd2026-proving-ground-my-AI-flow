// Self-test DUTs for the prep bus VIPs. Not part of any competition design.
//
//  apb_regs_dut      : APB4 slave with 4 registers, one wait state, PSLVERR on unmapped.
//  axi_ram_dut       : AXI4-Lite slave backed by a 256-word RAM.
//  axi_seq_master    : fixed-sequence AXI4-Lite master (2 writes + partial write + 2 reads),
//                      used to exercise the cocotb AXI4-Lite *slave* model.
//
// They are bundled in `vip_harness` with all bus signals exposed as top-level ports.

module apb_regs_dut (
    input  logic        clk,
    input  logic        rst_n,
    input  logic        psel,
    input  logic        penable,
    input  logic        pwrite,
    input  logic [31:0] paddr,
    input  logic [31:0] pwdata,
    input  logic [3:0]  pstrb,
    input  logic [3:0]  pprot,
    output logic        pready,
    output logic [31:0] prdata,
    output logic        pslverr
);
    logic [31:0] regs[0:3];
    logic [1:0]  wait_cnt;      // remaining wait states for the current access
    logic        access;
    logic        mapped;

    assign access = psel && penable;
    assign mapped = (paddr[31:4] == 28'd0);

    // Wait-state counter: 1 wait state per access, configured in SETUP.
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            wait_cnt <= 2'd0;
        end else if (psel && !penable) begin
            wait_cnt <= 2'd1;
        end else if (access && (wait_cnt != 2'd0)) begin
            wait_cnt <= wait_cnt - 2'd1;
        end else if (!access) begin
            wait_cnt <= 2'd0;
        end
    end

    // Register file: commit on the completing ACCESS edge, honour byte strobes.
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            for (int i = 0; i < 4; i++) regs[i] <= 32'd0;
        end else if (access && (wait_cnt == 2'd0) && pwrite && mapped) begin
            for (int b = 0; b < 4; b++) begin
                if (pstrb[b]) regs[paddr[3:2]][8*b +: 8] <= pwdata[8*b +: 8];
            end
        end
    end

    assign pready  = access && (wait_cnt == 2'd0);
    assign prdata  = mapped ? regs[paddr[3:2]] : 32'd0;
    assign pslverr = pready && !mapped;
endmodule


module axi_ram_dut #(
    parameter int WORDS = 256
) (
    input  logic        clk,
    input  logic        rst_n,
    input  logic        awvalid,
    output logic        awready,
    input  logic [31:0] awaddr,
    input  logic [2:0]  awprot,
    input  logic        wvalid,
    output logic        wready,
    input  logic [31:0] wdata,
    input  logic [3:0]  wstrb,
    output logic        bvalid,
    input  logic        bready,
    output logic [1:0]  bresp,
    input  logic        arvalid,
    output logic        arready,
    input  logic [31:0] araddr,
    input  logic [2:0]  arprot,
    output logic        rvalid,
    input  logic        rready,
    output logic [31:0] rdata,
    output logic [1:0]  rresp
);
    logic [31:0] mem[0:WORDS-1];
    logic        aw_hold, w_hold;
    logic [31:0] awaddr_q, wdata_q;
    logic [3:0]  wstrb_q;
    logic        rvalid_q;
    logic [31:0] rdata_q;

    assign awready = !aw_hold;
    assign wready  = !w_hold;
    assign bvalid  = aw_hold && w_hold;
    assign bresp   = 2'b00;
    assign arready = !rvalid_q;
    assign rvalid  = rvalid_q;
    assign rdata   = rdata_q;
    assign rresp   = 2'b00;

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            aw_hold <= 1'b0;
            w_hold  <= 1'b0;
            awaddr_q <= 32'd0;
            wdata_q  <= 32'd0;
            wstrb_q  <= 4'd0;
        end else begin
            if (awvalid && awready) begin
                aw_hold  <= 1'b1;
                awaddr_q <= awaddr;
            end
            if (wvalid && wready) begin
                w_hold  <= 1'b1;
                wdata_q <= wdata;
                wstrb_q <= wstrb;
            end
            if (bvalid && bready) begin
                for (int b = 0; b < 4; b++) begin
                    if (wstrb_q[b]) mem[awaddr_q[9:2]][8*b +: 8] <= wdata_q[8*b +: 8];
                end
                aw_hold <= 1'b0;
                w_hold  <= 1'b0;
            end
        end
    end

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            rvalid_q <= 1'b0;
            rdata_q  <= 32'd0;
        end else if (arvalid && arready) begin
            rvalid_q <= 1'b1;
            rdata_q  <= mem[araddr[9:2]];
        end else if (rvalid_q && rready) begin
            rvalid_q <= 1'b0;
        end
    end
endmodule


// Fixed-sequence AXI4-Lite master: W 0x20=0xA5A51234, R 0x20, W 0x30=0xFFFFFFFF,
// W 0x30 (strb=0101) = 0, R 0x30, then done. AW and W are tracked independently so the
// sequence also works against a slave that accepts them on different cycles.
module axi_seq_master (
    input  logic        clk,
    input  logic        rst_n,
    output logic        awvalid,
    input  logic        awready,
    output logic [31:0] awaddr,
    input  logic [2:0]  awprot,
    output logic        wvalid,
    input  logic        wready,
    output logic [31:0] wdata,
    output logic [3:0]  wstrb,
    input  logic        bvalid,
    output logic        bready,
    input  logic [1:0]  bresp,
    output logic        arvalid,
    input  logic        arready,
    output logic [31:0] araddr,
    input  logic [2:0]  arprot,
    input  logic        rvalid,
    output logic        rready,
    input  logic [31:0] rdata,
    input  logic [1:0]  rresp,
    output logic        done,
    output logic [31:0] rd0,
    output logic [31:0] rd1
);
    typedef enum logic [3:0] {
        S_IDLE,
        S_W1, S_W1B, S_R1, S_R1W,
        S_W2, S_W2B, S_W3, S_W3B, S_R2, S_R2W,
        S_DONE
    } state_t;
    state_t state;
    logic [31:0] rd0_q, rd1_q;

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state   <= S_IDLE;
            awvalid <= 1'b0; awaddr <= 32'd0;
            wvalid  <= 1'b0; wdata  <= 32'd0; wstrb <= 4'd0;
            bready  <= 1'b1;
            arvalid <= 1'b0; araddr <= 32'd0;
            rready  <= 1'b1;
            done    <= 1'b0;
            rd0_q   <= 32'd0; rd1_q <= 32'd0;
        end else begin
            case (state)
                S_IDLE: begin
                    awaddr  <= 32'h20; wdata <= 32'hA5A51234; wstrb <= 4'hF;
                    awvalid <= 1'b1;   wvalid <= 1'b1;
                    state   <= S_W1;
                end
                S_W1: begin
                    if (awready) awvalid <= 1'b0;
                    if (wready)  wvalid  <= 1'b0;
                    if ((!awvalid || awready) && (!wvalid || wready))
                        state <= S_W1B;
                end
                S_W1B: if (bvalid) begin
                    araddr  <= 32'h20; arvalid <= 1'b1;
                    state   <= S_R1;
                end
                S_R1: begin
                    if (arready) arvalid <= 1'b0;
                    if (!arvalid || arready) state <= S_R1W;
                end
                S_R1W: if (rvalid) begin
                    rd0_q   <= rdata;
                    awaddr  <= 32'h30; wdata <= 32'hFFFFFFFF; wstrb <= 4'hF;
                    awvalid <= 1'b1;   wvalid <= 1'b1;
                    state   <= S_W2;
                end
                S_W2: begin
                    if (awready) awvalid <= 1'b0;
                    if (wready)  wvalid  <= 1'b0;
                    if ((!awvalid || awready) && (!wvalid || wready))
                        state <= S_W2B;
                end
                S_W2B: if (bvalid) begin
                    awaddr  <= 32'h30; wdata <= 32'h00000000; wstrb <= 4'b0101;
                    awvalid <= 1'b1;   wvalid <= 1'b1;
                    state   <= S_W3;
                end
                S_W3: begin
                    if (awready) awvalid <= 1'b0;
                    if (wready)  wvalid  <= 1'b0;
                    if ((!awvalid || awready) && (!wvalid || wready))
                        state <= S_W3B;
                end
                S_W3B: if (bvalid) begin
                    araddr  <= 32'h30; arvalid <= 1'b1;
                    state   <= S_R2;
                end
                S_R2: begin
                    if (arready) arvalid <= 1'b0;
                    if (!arvalid || arready) state <= S_R2W;
                end
                S_R2W: if (rvalid) begin
                    rd1_q <= rdata;
                    state <= S_DONE;
                end
                S_DONE: done <= 1'b1;
                default: state <= S_IDLE;
            endcase
        end
    end

    assign rd0  = rd0_q;
    assign rd1  = rd1_q;
endmodule


module vip_harness (
    input  logic        clk,
    input  logic        rst_n,

    // APB4 slave-under-test interface
    input  logic        apb_psel,
    input  logic        apb_penable,
    input  logic        apb_pwrite,
    input  logic [31:0] apb_paddr,
    input  logic [31:0] apb_pwdata,
    input  logic [3:0]  apb_pstrb,
    input  logic [3:0]  apb_pprot,
    output logic        apb_pready,
    output logic [31:0] apb_prdata,
    output logic        apb_pslverr,

    // AXI4-Lite slave-under-test interface (driven by cocotb master BFM)
    input  logic        ram_awvalid,
    output logic        ram_awready,
    input  logic [31:0] ram_awaddr,
    input  logic [2:0]  ram_awprot,
    input  logic        ram_wvalid,
    output logic        ram_wready,
    input  logic [31:0] ram_wdata,
    input  logic [3:0]  ram_wstrb,
    output logic        ram_bvalid,
    input  logic        ram_bready,
    output logic [1:0]  ram_bresp,
    input  logic        ram_arvalid,
    output logic        ram_arready,
    input  logic [31:0] ram_araddr,
    input  logic [2:0]  ram_arprot,
    output logic        ram_rvalid,
    input  logic        ram_rready,
    output logic [31:0] ram_rdata,
    output logic [1:0]  ram_rresp,

    // AXI4-Lite master-under-test interface (observed by cocotb slave BFM)
    output logic        seq_awvalid,
    input  logic        seq_awready,
    output logic [31:0] seq_awaddr,
    output logic [2:0]  seq_awprot,
    output logic        seq_wvalid,
    input  logic        seq_wready,
    output logic [31:0] seq_wdata,
    output logic [3:0]  seq_wstrb,
    input  logic        seq_bvalid,
    output logic        seq_bready,
    input  logic [1:0]  seq_bresp,
    output logic        seq_arvalid,
    input  logic        seq_arready,
    output logic [31:0] seq_araddr,
    output logic [2:0]  seq_arprot,
    input  logic        seq_rvalid,
    output logic        seq_rready,
    input  logic [31:0] seq_rdata,
    input  logic [1:0]  seq_rresp,
    output logic        seq_done,
    output logic [31:0] seq_rd0,
    output logic [31:0] seq_rd1
);
    apb_regs_dut u_apb (
        .clk, .rst_n, .psel(apb_psel), .penable(apb_penable), .pwrite(apb_pwrite),
        .paddr(apb_paddr), .pwdata(apb_pwdata), .pstrb(apb_pstrb), .pprot(apb_pprot),
        .pready(apb_pready), .prdata(apb_prdata), .pslverr(apb_pslverr)
    );

    axi_ram_dut u_ram (
        .clk, .rst_n,
        .awvalid(ram_awvalid), .awready(ram_awready), .awaddr(ram_awaddr), .awprot(ram_awprot),
        .wvalid(ram_wvalid), .wready(ram_wready), .wdata(ram_wdata), .wstrb(ram_wstrb),
        .bvalid(ram_bvalid), .bready(ram_bready), .bresp(ram_bresp),
        .arvalid(ram_arvalid), .arready(ram_arready), .araddr(ram_araddr), .arprot(ram_arprot),
        .rvalid(ram_rvalid), .rready(ram_rready), .rdata(ram_rdata), .rresp(ram_rresp)
    );

    axi_seq_master u_seq (
        .clk, .rst_n,
        .awvalid(seq_awvalid), .awready(seq_awready), .awaddr(seq_awaddr), .awprot(seq_awprot),
        .wvalid(seq_wvalid), .wready(seq_wready), .wdata(seq_wdata), .wstrb(seq_wstrb),
        .bvalid(seq_bvalid), .bready(seq_bready), .bresp(seq_bresp),
        .arvalid(seq_arvalid), .arready(seq_arready), .araddr(seq_araddr), .arprot(seq_arprot),
        .rvalid(seq_rvalid), .rready(seq_rready), .rdata(seq_rdata), .rresp(seq_rresp),
        .done(seq_done), .rd0(seq_rd0), .rd1(seq_rd1)
    );
endmodule
