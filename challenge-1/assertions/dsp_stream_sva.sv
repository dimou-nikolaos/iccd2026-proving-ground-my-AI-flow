module dsp_stream_sva (
  input  logic        clk,
  input  logic        rst_n,
  input  logic        s_valid,
  input  logic        s_ready,
  input  logic [15:0] s_data,
  input  logic        s_last,
  input  logic        m_valid,
  input  logic        m_ready,
  input  logic [15:0] m_data,
  input  logic        m_last,
  input  logic        enable,
  input  logic        stat_clear,
  input  logic        inf_ready,
  input  logic        inf_valid,
  input  logic        gain_valid,
  input  logic        fir_valid,
  input  logic        fir_active,
  input  logic        dec_ready,
  input  logic        dec_valid,
  input  logic        dec_clip,
  input  logic        lim_clipped,
  input  logic        ouf_ready,
  input  logic [16:0] ouf_data,
  input  logic        push_fire,
  input  logic        clip_event,
  input  logic        frame_event,
  input  logic        busy,
  input  logic        fifo_empty,
  input  logic [15:0] last_len_q,
  input  logic [15:0] frame_cnt_q
);

  logic        p_mvalid, p_mready, p_mlast, p_stat_clear, p_frame_event, p_rst = 1'b0;
  logic [15:0] p_mdata, p_lastlen, p_fcnt;
  logic        p_svalid, p_sready, p_slast = 1'b0;
  logic [15:0] p_sdata;

  always_comb begin
    if (!$isunknown({s_valid, s_ready, m_valid, m_ready, m_data, m_last, enable, inf_ready,
                     inf_valid, gain_valid, fir_valid, fir_active, dec_ready, dec_valid, dec_clip,
                     lim_clipped, ouf_ready, ouf_data, push_fire, clip_event, frame_event,
                     busy, fifo_empty})) begin
      a_sready_def: assert (s_ready == (enable && inf_ready))
        else $error("stream s_ready def wrong");
      a_mdata_def: assert (m_data == ouf_data[15:0])
        else $error("stream m_data def wrong");
      a_mlast_def: assert (m_last == ouf_data[16])
        else $error("stream m_last def wrong");
      a_pushfire_def: assert (push_fire == (dec_valid && ouf_ready))
        else $error("stream push_fire def wrong");
      a_clipevent_def: assert (clip_event == (push_fire && (dec_clip || lim_clipped)))
        else $error("stream clip_event def wrong");
      a_frameevent_def: assert (frame_event == (m_valid && m_ready && m_last))
        else $error("stream frame_event def wrong");
      a_fifoempty_def: assert (fifo_empty == !m_valid)
        else $error("stream fifo_empty def wrong");
      a_busy_def: assert (busy == (inf_valid || gain_valid || fir_active || dec_valid || m_valid))
        else $error("stream busy def wrong");
    end
  end

  always @(posedge clk) begin
    if (p_rst && rst_n) begin
      a_lastlen_ns: assert (last_len_q == (p_stat_clear ? 16'd0
                          : (p_frame_event ? (p_fcnt + 16'd1) : p_lastlen)))
        else $error("stream last_len_q next-state wrong");
      a_framecnt_ns: assert (frame_cnt_q == (p_stat_clear ? 16'd0
                           : (p_frame_event ? 16'd0
                           : ((p_mvalid && p_mready) ? (p_fcnt + 16'd1) : p_fcnt))))
        else $error("stream frame_cnt_q next-state wrong");
      if (p_mvalid && !p_mready) begin
        a_out_stable: assert (m_valid && (m_data == p_mdata) && (m_last == p_mlast))
          else $error("stream output not held during stall");
      end
      // Input handshake stability: valid/data/last held while ready is 0.
      if (p_svalid && !p_sready) begin
        a_in_stable: assert (s_valid && (s_data == p_sdata) && (s_last == p_slast))
          else $error("stream input not held during stall");
      end
    end
    p_svalid     <= s_valid;
    p_sready     <= s_ready;
    p_sdata      <= s_data;
    p_slast      <= s_last;
    p_mvalid     <= m_valid;
    p_mready     <= m_ready;
    p_mdata      <= m_data;
    p_mlast      <= m_last;
    p_stat_clear <= stat_clear;
    p_frame_event<= frame_event;
    p_lastlen    <= last_len_q;
    p_fcnt       <= frame_cnt_q;
    p_rst        <= rst_n;
  end

endmodule

bind dsp_stream dsp_stream_sva u_dsp_stream_sva (.*);
