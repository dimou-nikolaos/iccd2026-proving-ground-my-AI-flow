// AXI4-Lite protocol assertions (bind-style, all ports are `input`).
//
// Bind it to any module that carries a full AXI4-Lite interface with these exact names:
//
//     bind dma_ctrl axi4lite_protocol_sva u_axi_sva (.*);
//
// All ports MUST be `input` (flow-lessons L25) — see apb4_protocol_sva.sv.
//
// Focus: VALID/payload stability while waiting for READY. This is the single most common
// seeded protocol deviation (flow-lessons L3) and it is invisible to data-comparison
// tests when the slave happens to be fast; slow/backpressured slaves expose it.
//
// Reset: `default disable iff (!rst_n)`.

module axi4lite_protocol_sva (
    input logic        clk,
    input logic        rst_n,

    // write address channel
    input logic        awvalid,
    input logic        awready,
    input logic [31:0] awaddr,
    input logic [2:0]  awprot,
    // write data channel
    input logic        wvalid,
    input logic        wready,
    input logic [31:0] wdata,
    input logic [3:0]  wstrb,
    // write response channel
    input logic        bvalid,
    input logic        bready,
    input logic [1:0]  bresp,
    // read address channel
    input logic        arvalid,
    input logic        arready,
    input logic [31:0] araddr,
    input logic [2:0]  arprot,
    // read data channel
    input logic        rvalid,
    input logic        rready,
    input logic [31:0] rdata,
    input logic [1:0]  rresp
);
    default disable iff (!rst_n);

    // A1/A2: AW payload must not change while VALID waits for READY.
    property p_aw_stable;
        @(posedge clk) (awvalid && !awready) |=>
            (awvalid && awaddr == $past(awaddr) && awprot == $past(awprot));
    endproperty
    assert property (p_aw_stable)
        else $error("AXI4-Lite: AWVALID/AWADDR/AWPROT changed before AWREADY");

    // A3/A4: W payload must not change while VALID waits for READY. WSTRB is checked
    //        too: a strobe that follows WVALID violates the channel contract.
    property p_w_stable;
        @(posedge clk) (wvalid && !wready) |=>
            (wvalid && wdata == $past(wdata) && wstrb == $past(wstrb));
    endproperty
    assert property (p_w_stable)
        else $error("AXI4-Lite: WVALID/WDATA/WSTRB changed before WREADY");

    // A5: B response must be held until BREADY.
    property p_b_stable;
        @(posedge clk) (bvalid && !bready) |=>
            (bvalid && bresp == $past(bresp));
    endproperty
    assert property (p_b_stable)
        else $error("AXI4-Lite: BVALID/BRESP changed before BREADY");

    // A6/A7: AR payload must not change while VALID waits for READY.
    property p_ar_stable;
        @(posedge clk) (arvalid && !arready) |=>
            (arvalid && araddr == $past(araddr) && arprot == $past(arprot));
    endproperty
    assert property (p_ar_stable)
        else $error("AXI4-Lite: ARVALID/ARADDR/ARPROT changed before ARREADY");

    // A8/A9: R payload must be held until RREADY.
    property p_r_stable;
        @(posedge clk) (rvalid && !rready) |=>
            (rvalid && rdata == $past(rdata) && rresp == $past(rresp));
    endproperty
    assert property (p_r_stable)
        else $error("AXI4-Lite: RVALID/RDATA/RRESP changed before RREADY");

    // A10: BRESP/RRESP are 2-bit codes; an X on either is a modelling escape, not a value.
    property p_bresp_known;
        @(posedge clk) bvalid |-> !$isunknown(bresp);
    endproperty
    assert property (p_bresp_known)
        else $error("AXI4-Lite: unknown BRESP");

    property p_rresp_known;
        @(posedge clk) rvalid |-> !$isunknown(rresp);
    endproperty
    assert property (p_rresp_known)
        else $error("AXI4-Lite: unknown RRESP");
endmodule
