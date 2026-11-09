// Bind file used by the VIP self-test only (it targets the self-test DUTs by name).
//
// On a real design, write the equivalent three-line bind file against the released top
// module and the released port names — the observer modules themselves do not change.
//
//     bind dma_ctrl  apb4_protocol_sva     u_apb_sva  (.*);
//     bind dma_ctrl  axi4lite_protocol_sva u_axi_sva  (.*);

bind apb_regs_dut    apb4_protocol_sva     u_apb_sva (.*);
bind axi_ram_dut     axi4lite_protocol_sva u_axi_sva (.*);
bind axi_seq_master  axi4lite_protocol_sva u_seq_sva (.*);
