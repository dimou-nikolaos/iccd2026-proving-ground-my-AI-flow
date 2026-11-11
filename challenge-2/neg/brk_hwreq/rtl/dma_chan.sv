// dma_chan: register state of one channel.
module dma_chan (
  input  logic        clk,
  input  logic        rst_n,
  // Register writes
  input  logic        wr_ctrl,
  input  logic        wr_src,
  input  logic        wr_dst,
  input  logic        wr_len,
  input  logic        wr_next,
  input  logic [31:0] wdata,
  input  logic [31:0] wmask,
  // Engine updates
  input  logic        owned,
  input  logic        word_done,
  input  logic        load_src,
  input  logic        load_dst,
  input  logic        load_len,
  input  logic        load_next,
  input  logic [31:0] load_data,
  input  logic        stop,
  input  logic        set_err,
  input  logic        set_done,
  // State
  output logic        active,
  output logic        abort,
  output logic        hw_req,
  output logic [1:0]  prio,
  output logic        link,
  output logic [31:0] src,
  output logic [31:0] dst,
  output logic [15:0] len,
  output logic [31:0] next,
  output logic [31:0] ctrl_rd,
  output logic [31:0] stat_rd
);

  logic        en_q, abort_q, err_q, done_q;
  logic [5:0]  cfg_q;
  logic [29:0] src_q, dst_q, next_q;
  logic [15:0] len_q;
  logic [31:0] ctrl_wr;
  logic [31:0] src_m, dst_m, next_m;
  logic        en_wr_visible;
  logic        unused_bits;

  assign unused_bits = &{1'b0, ctrl_wr[31:7], ctrl_wr[0], src_m[1:0], dst_m[1:0], next_m[1:0]};

  assign src_m  = ({src_q, 2'b00} & ~wmask) | (wdata & wmask);
  assign dst_m  = ({dst_q, 2'b00} & ~wmask) | (wdata & wmask);
  assign next_m = ({next_q, 2'b00} & ~wmask) | (wdata & wmask);

  assign ctrl_wr       = (ctrl_rd & ~wmask) | (wdata & wmask);
  assign en_wr_visible = wr_ctrl && wmask[0];

  assign active  = en_q;
  assign abort   = abort_q;
  assign hw_req  = cfg_q[3];
  assign prio    = cfg_q[4:3];
  assign link    = cfg_q[5];
  assign src     = {src_q, 2'b00};
  assign dst     = {dst_q, 2'b00};
  assign len     = len_q;
  assign next    = {next_q, 2'b00};
  assign ctrl_rd = {25'h0, cfg_q, en_q};
  assign stat_rd = {29'h0, done_q, err_q, en_q};

  // Enable, abort and sticky status.
  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      en_q    <= 1'b0;
      abort_q <= 1'b0;
      err_q   <= 1'b0;
      done_q  <= 1'b0;
    end else begin
      if (stop) begin
        en_q    <= 1'b0;
        abort_q <= 1'b0;
      end else if (abort_q && !owned) begin
        en_q    <= 1'b0;
        abort_q <= 1'b0;
      end else if (en_wr_visible) begin
        if (wdata[0] && !en_q) begin
          en_q   <= 1'b1;
        end else if (!wdata[0] && en_q) begin
          abort_q <= 1'b1;
        end
      end
      if (en_wr_visible && wdata[0] && !en_q) begin
        err_q  <= 1'b0;
        done_q <= 1'b0;
      end else begin
        if (set_err) begin
          err_q <= 1'b1;
        end
        if (set_done) begin
          done_q <= 1'b1;
        end
      end
    end
  end

  // Configuration bits can only change while the channel is stopped.
  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      cfg_q <= 6'd0;
    end else if (wr_ctrl && !en_q) begin
      cfg_q <= ctrl_wr[6:1];
    end
  end

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      src_q  <= 30'd0;
      dst_q  <= 30'd0;
      len_q  <= 16'd0;
      next_q <= 30'd0;
    end else begin
      if (wr_src && !en_q) begin
        src_q <= src_m[31:2];
      end else if (load_src) begin
        src_q <= load_data[31:2];
      end else if (word_done && !cfg_q[0]) begin
        src_q <= src_q + 30'd1;
      end

      if (wr_dst && !en_q) begin
        dst_q <= dst_m[31:2];
      end else if (load_dst) begin
        dst_q <= load_data[31:2];
      end else if (word_done && !cfg_q[1]) begin
        dst_q <= dst_q + 30'd1;
      end

      if (wr_len && !en_q) begin
        len_q <= (len_q & ~wmask[15:0]) | (wdata[15:0] & wmask[15:0]);
      end else if (load_len) begin
        len_q <= load_data[15:0];
      end else if (word_done) begin
        len_q <= len_q - 16'd1;
      end

      if (wr_next && !en_q) begin
        next_q <= next_m[31:2];
      end else if (load_next) begin
        next_q <= load_data[31:2];
      end
    end
  end

endmodule
