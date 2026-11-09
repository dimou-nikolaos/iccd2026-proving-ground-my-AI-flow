// APB4 protocol assertions (bind-style, all ports are `input`).
//
// Bind it to any module that carries an APB4 interface with these exact names:
//
//     bind dma_ctrl apb4_protocol_sva u_apb_sva (.*);
//
// All ports MUST be `input` (flow-lessons L25): `bind ... (.*)` matches by name, and an
// `output` port would make the observer drive the DUT wire, silently resolving to X.
//
// These assertions are about the *master's* behaviour (driving PSEL/PENABLE), so they are
// equally useful bound to the DUT's APB master and to our own BFM in a VIP self-test.
//
// Reset: `default disable iff (!rst_n)` — the checks do not apply while reset is asserted.

module apb4_protocol_sva (
    input logic        clk,
    input logic        rst_n,
    input logic        psel,
    input logic        penable,
    input logic        pwrite,
    input logic [31:0] paddr,
    input logic [31:0] pwdata,
    input logic [3:0]  pstrb,
    input logic [3:0]  pprot,
    input logic        pready,
    input logic [31:0] prdata,
    input logic        pslverr
);
    default disable iff (!rst_n);

    // A1: PENABLE is only legal inside a selected transfer.
    property p_penable_implies_psel;
        @(posedge clk) penable |-> psel;
    endproperty
    assert property (p_penable_implies_psel)
        else $error("APB4: PENABLE asserted while PSEL is low");

    // A2: SETUP must be followed by ACCESS (the transfer cannot be aborted).
    property p_setup_then_access;
        @(posedge clk) (psel && !penable) |=> (psel && penable);
    endproperty
    assert property (p_setup_then_access)
        else $error("APB4: SETUP cycle not followed by an ACCESS cycle");

    // A3: control/address/data/strobes are held stable across wait states.
    property p_hold_in_wait_state;
        @(posedge clk) (psel && penable && !pready) |=>
            (psel && penable && pwrite == $past(pwrite) &&
             paddr == $past(paddr) && pwdata == $past(pwdata) &&
             pstrb == $past(pstrb) && pprot == $past(pprot));
    endproperty
    assert property (p_hold_in_wait_state)
        else $error("APB4: control/address/data changed during a wait state");

    // A4: after the completing ACCESS cycle the master either idles or starts a new
    // SETUP — it must never hold PENABLE high into another ACCESS cycle.
    property p_no_extra_access;
        @(posedge clk) (psel && penable && pready) |=> (!psel) || (psel && !penable);
    endproperty
    assert property (p_no_extra_access)
        else $error("APB4: ACCESS continued after PREADY without a new SETUP");

    // A5: PREADY and PSLVERR are only meaningful inside a transfer.
    property p_pready_implies_psel;
        @(posedge clk) pready |-> psel;
    endproperty
    assert property (p_pready_implies_psel)
        else $error("APB4: PREADY asserted while PSEL is low");

    property p_pslverr_implies_completion;
        @(posedge clk) pslverr |-> (psel && penable && pready);
    endproperty
    assert property (p_pslverr_implies_completion)
        else $error("APB4: PSLVERR asserted outside a completing ACCESS cycle");

    // A6: PRDATA must not change while the access is being held with wait states
    //     (a read slave that re-drives PRDATA from a changing source would violate this).
    property p_prdata_stable_in_access;
        @(posedge clk) (psel && penable && !pready && !pwrite) |=> prdata == $past(prdata);
    endproperty
    assert property (p_prdata_stable_in_access)
        else $error("APB4: PRDATA changed during a held read access");
endmodule
