// apb_sva.sv — APB4 completer contract for dma_ctrl (REQ-03, REQ-04, REQ-05, REQ-07, REQ-08).
//
// Bound to the TOP module (dma_ctrl) so every observed signal is either a top-level
// port or a top-scope net (the dma_apb instance outputs). No hierarchical references,
// every observer port is `input` (flow-lessons L25).
//
// The decode is re-derived here from the SPEC (address map of section 5) and compared
// against what dma_apb actually produces: a golden-model signal-definition check, which
// kills every mutation of the decode expressions in rtl/dma_apb.sv.

/* verilator lint_off SYNCASYNCNET */

module apb_sva (
  input logic        clk,
  input logic        rst_n,
  // APB4 completer ports of dma_ctrl
  input logic        psel,
  input logic        penable,
  input logic        pwrite,
  input logic [11:0] paddr,
  input logic [31:0] pwdata,
  input logic [3:0]  pstrb,
  input logic [31:0] prdata,
  input logic        pready,
  input logic        pslverr,
  // decode outputs of the dma_apb instance (nets in dma_ctrl scope)
  input logic        reg_wr,
  input logic [31:0] reg_wmask,
  input logic        glob_sel,
  input logic [9:0]  glob_word,
  input logic        chan_sel,
  input logic [1:0]  chan_idx,
  input logic [2:0]  chan_reg
);

  default clocking cb @(posedge clk); endclocking
  default disable iff (!rst_n);

  // ---- golden APB decode derived from the spec register map (section 5) ----
  logic        access;   // REQ-04: transfer happens in the access phase
  logic [9:0]  word;     // REQ-05: decode uses paddr[11:2], [1:0] ignored
  logic        is_glob;  // 0x000..0x014
  logic        is_chan;  // 0x100+0x20n, six registers per channel
  logic        mapped;   // REQ-07: everything else is unmapped

  assign access  = psel && penable;
  assign word    = paddr[11:2];
  assign is_glob = (word <= 10'h005);
  assign is_chan = (word[9:5] == 5'b00010) && (word[2:0] <= 3'd5);
  assign mapped  = is_glob || is_chan;

  // REQ-03: zero wait states — pready is constant 1 in every cycle.
  a_req03_pready_const: assert property (pready == 1'b1)
    else $error("REQ-03: pready is not constant 1 (pready=%b)", pready);

  // REQ-05: address decode uses paddr[11:2].
  a_decode_word: assert property (glob_word == word)
    else $error("APB decode: glob_word=%h != paddr[11:2]=%h", glob_word, word);

  // REQ-07: the global/chan window decode matches the spec map.
  a_decode_glob: assert property (glob_sel == is_glob)
    else $error("REQ-07: glob_sel=%b disagrees with spec decode (%b) at paddr=%h",
                glob_sel, is_glob, paddr);
  a_decode_chan: assert property (chan_sel == is_chan)
    else $error("REQ-07: chan_sel=%b disagrees with spec decode (%b) at paddr=%h",
                chan_sel, is_chan, paddr);
  a_decode_disjoint: assert property (!(glob_sel && chan_sel))
    else $error("APB decode: global and channel windows overlap at paddr=%h", paddr);

  // REQ-05 + register map: channel index and register index inside the window.
  a_decode_idx: assert property (chan_idx == word[4:3])
    else $error("APB decode: chan_idx=%0d != paddr[11:2][4:3]=%0d", chan_idx, word[4:3]);
  a_decode_reg: assert property (chan_reg == word[2:0])
    else $error("APB decode: chan_reg=%0d != paddr[11:2][2:0]=%0d", chan_reg, word[2:0]);

  // REQ-04: a write is performed only in the access phase of a mapped write.
  a_req04_reg_wr: assert property (reg_wr == (access && pwrite && mapped))
    else $error("REQ-04/07: reg_wr=%b but access=%b pwrite=%b mapped=%b",
                reg_wr, access, pwrite, mapped);

  // REQ-08: byte-lane strobes expand to a bit mask (wmask definition).
  a_req08_wmask: assert property (
      reg_wmask == {{8{pstrb[3]}}, {8{pstrb[2]}}, {8{pstrb[1]}}, {8{pstrb[0]}}})
    else $error("REQ-08: reg_wmask=%h wrong for pstrb=%b", reg_wmask, pstrb);

  // REQ-07: unmapped access completes with pslverr=1, mapped with pslverr=0.
  a_req07_pslverr: assert property (pslverr == (access && !mapped))
    else $error("REQ-07: pslverr=%b but access=%b mapped=%b at paddr=%h",
                pslverr, access, mapped, paddr);

  // REQ-06: outside the access phase of a read, prdata = 0.
  a_req06_prdata_zero: assert property (!(access && !pwrite) |-> prdata == 32'h0)
    else $error("REQ-06: prdata=%h driven outside a read access phase", prdata);

endmodule

/* verilator lint_on SYNCASYNCNET */

bind dma_ctrl apb_sva u_apb_sva (.*);
