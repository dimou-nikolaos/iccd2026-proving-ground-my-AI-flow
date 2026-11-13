// rv_lsu: data memory interface in the memory stage. Aligns store data and strobes,
// and extracts and extends load data.
module rv_lsu (
  input  logic        mem_valid,
  input  logic        is_load,
  input  logic        is_store,
  input  logic [1:0]  size,
  input  logic        is_unsigned,
  input  logic [31:0] addr,
  input  logic [31:0] store_data,
  output logic        dmem_valid,
  output logic        dmem_write,
  output logic [31:0] dmem_addr,
  output logic [3:0]  dmem_wstrb,
  output logic [31:0] dmem_wdata,
  input  logic [31:0] dmem_rdata,
  output logic [31:0] load_data
);

  logic [1:0]  offset;
  logic [15:0] half;
  logic [7:0]  byte_val;

  assign offset     = addr[1:0];
  assign dmem_valid = mem_valid && (is_load || is_store);
  assign dmem_write = mem_valid && is_store;
  assign dmem_addr  = dmem_valid ? {addr[31:2], 2'b00} : 32'd0;

  always_comb begin
    dmem_wstrb = 4'b0000;
    dmem_wdata = 32'd0;
    if (mem_valid && is_store) begin
      unique case (size)
        2'b00: begin
          dmem_wstrb = 4'b0001 << offset;
          dmem_wdata = {4{store_data[7:0]}};
        end
        2'b01: begin
          dmem_wstrb = offset[1] ? 4'b1100 : 4'b0011;
          dmem_wdata = {2{store_data[15:0]}};
        end
        default: begin
          dmem_wstrb = 4'b1111;
          dmem_wdata = store_data;
        end
      endcase
    end
  end

  assign half = offset[1] ? dmem_rdata[31:16] : dmem_rdata[15:0];

  always_comb begin
    unique case (offset)
      2'b00: byte_val = dmem_rdata[7:0];
      2'b01: byte_val = dmem_rdata[15:8];
      2'b10: byte_val = dmem_rdata[23:16];
      2'b11: byte_val = dmem_rdata[31:24];
    endcase
  end

  always_comb begin
    unique case (size)
      2'b00: load_data = {{24{!is_unsigned && byte_val[7]}}, byte_val};
      2'b01: load_data = {{16{!is_unsigned && half[15]}}, half};
      default: load_data = dmem_rdata;
    endcase
  end

endmodule
