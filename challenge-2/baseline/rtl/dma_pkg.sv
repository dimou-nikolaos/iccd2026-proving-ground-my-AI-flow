// dma_pkg: shared constants and helpers for the dma_ctrl controller.
package dma_pkg;

  parameter int NUM_CH = 4;

  parameter logic [31:0] UNIT_ID = 32'h444D_0100;

  // Global register word offsets (paddr[11:2]).
  parameter logic [9:0] W_ID      = 10'h000;
  parameter logic [9:0] W_GCTRL   = 10'h001;
  parameter logic [9:0] W_IRQ_ST  = 10'h002;
  parameter logic [9:0] W_IRQ_EN  = 10'h003;
  parameter logic [9:0] W_ACTIVE  = 10'h004;
  parameter logic [9:0] W_GRANT   = 10'h005;

  // Channel register indices (paddr[4:2]) in the window 0x100 + 0x20 * n.
  parameter logic [2:0] R_CTRL = 3'd0;
  parameter logic [2:0] R_SRC  = 3'd1;
  parameter logic [2:0] R_DST  = 3'd2;
  parameter logic [2:0] R_LEN  = 3'd3;
  parameter logic [2:0] R_NEXT = 3'd4;
  parameter logic [2:0] R_STAT = 3'd5;

  // Implemented bits of the interrupt registers.
  parameter logic [31:0] IRQ_MASK = 32'h0000_00FF;

  // AXI response code for a successful access.
  parameter logic [1:0] RESP_OKAY = 2'b00;

  function automatic logic [31:0] strb_mask(input logic [3:0] strb);
    strb_mask = {{8{strb[3]}}, {8{strb[2]}}, {8{strb[1]}}, {8{strb[0]}}};
  endfunction

endpackage
