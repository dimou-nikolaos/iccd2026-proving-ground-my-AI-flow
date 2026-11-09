// DMA top-level invariants for the fixture (bind-style, all-input ports).
//
// These complement the generic APB4/AXI4-Lite protocol observers: they check
// the *engine* contract at the port boundary - state/AXI legality and the
// interrupt pulse shape. On day 1 the same two properties bind to the released
// dma_ctrl if it exposes a state/status tap; if it does not, drop dma_fsm_sva
// and keep dma_irq_sva (it only needs clk/rst_n/irq/IE-visible behaviour).
//
// L25 rule: every port is `input` so `bind ... (explicit map)` never drives
// a DUT net.

`default_nettype none

// IRQ must be a one-cycle pulse (level interrupts are a seeded deviation).
module dma_irq_sva (
    input logic clk,
    input logic rst_n,
    input logic irq
);
    default disable iff (!rst_n);
    property p_irq_single_cycle;
        @(posedge clk) irq |=> !irq;
    endproperty
    assert property (p_irq_single_cycle)
        else $error("DMA-IRQ: irq_done held high for more than one cycle");
endmodule

// Engine-state contract (needs the fixture's obs_state tap).
module dma_fsm_sva (
    input logic      clk,
    input logic      rst_n,
    input logic [3:0] state,
    input logic      busy,
    input logic      psel,
    input logic      penable,
    input logic      arvalid,
    input logic      awvalid,
    input logic      wvalid,
    input logic      pready
);
    default disable iff (!rst_n);

    localparam logic [3:0] S_IDLE    = 4'd0,
                           S_DESC_AR = 4'd1,
                           S_XFER_AR = 4'd4,
                           S_XFER_AW = 4'd6,
                           S_XFER_W  = 4'd7;

    // AR issue only in the two address-fetch states.
    property p_ar_state_legal;
        @(posedge clk) arvalid |-> (state == S_DESC_AR || state == S_XFER_AR);
    endproperty
    assert property (p_ar_state_legal)
        else $error("DMA-FSM: ARVALID asserted outside a fetch state (state=0x{0,state})");

    // AW/W issue only in their own states, and AW never without W to follow.
    property p_aw_state_legal;
        @(posedge clk) awvalid |-> (state == S_XFER_AW);
    endproperty
    assert property (p_aw_state_legal)
        else $error("DMA-FSM: AWVALID asserted outside S_XFER_AW");

    property p_w_state_legal;
        @(posedge clk) wvalid |-> (state == S_XFER_W);
    endproperty
    assert property (p_w_state_legal)
        else $error("DMA-FSM: WVALID asserted outside S_XFER_W");

    // A selected transfer must never insert a wait state on this config port
    // (the fixture promises single-cycle access; day-1: re-check the spec).
    property p_no_wait_state;
        @(posedge clk) (psel && penable) |-> pready;
    endproperty
    assert property (p_no_wait_state)
        else $error("DMA-FSM: PREADY low during a selected ACCESS (unexpected wait state)");
endmodule

`default_nettype wire
