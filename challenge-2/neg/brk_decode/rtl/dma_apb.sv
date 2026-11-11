// dma_apb: APB4 completer decode with zero wait states.
module dma_apb (
  input  logic        psel,
  input  logic        penable,
  input  logic        pwrite,
  input  logic [11:0] paddr,
  input  logic [3:0]  pstrb,
  output logic        pready,
  output logic        pslverr,
  // Decoded access
  output logic        reg_wr,
  output logic [31:0] reg_wmask,
  output logic        glob_sel,
  output logic [9:0]  glob_word,
  output logic        chan_sel,
  output logic [1:0]  chan_idx,
  output logic [2:0]  chan_reg
);

  logic       access;
  logic [9:0] word;
  logic       is_glob, is_chan, mapped;
  logic       unused_bits;

  assign unused_bits = &{1'b0, paddr[1:0]};

  assign access = psel && penable;
  assign word   = paddr[11:2];

  assign is_glob = (word <= dma_pkg::W_GRANT);
  // Channel windows start at 0x100 and hold six registers each.
  assign is_chan = (word[9:5] == 5'b00011) && (word[2:0] <= dma_pkg::R_STAT);
  assign mapped  = is_glob || is_chan;

  assign pready    = 1'b1;
  assign pslverr   = access && !mapped;
  assign reg_wr    = access && pwrite && mapped;
  assign reg_wmask = dma_pkg::strb_mask(pstrb);
  assign glob_sel  = is_glob;
  assign glob_word = word;
  assign chan_sel  = is_chan;
  assign chan_idx  = word[4:3];
  assign chan_reg  = word[2:0];

endmodule
