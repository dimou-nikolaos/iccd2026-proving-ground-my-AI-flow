"""Diagnostic tool: find WHY a retired value diverges (wrong operand or wrong operation).

Records the DUT's EX-stage operands for every instruction (keyed by pc), and the
ISS's operand values for every commit, then reports the first commit whose
writeback value differs together with both sides' operands.

Env: RVSEED (default 3), RVNINSTR (default 250).
"""

import os
import sys

import cocotb
from cocotb.clock import Clock
from cocotb.triggers import FallingEdge, RisingEdge

from asm import assemble
from randprog import GenConfig, generate
from rv32_iss import RV32ISS

NOP = 0x0000_0013
SEED = int(os.environ.get("RVSEED", "3"))
NINSTR = int(os.environ.get("RVNINSTR", "250"))


def _decode(instr):
    op = instr & 0x7F
    f3 = (instr >> 12) & 7
    f7 = (instr >> 25) & 0x7F
    if op in (0x03, 0x13, 0x67):
        v = instr >> 20
        imm = v - (1 << 12) if v & 0x800 else v
    elif op == 0x23:
        v = ((instr >> 25) << 5) | ((instr >> 7) & 0x1F)
        imm = v - (1 << 12) if v & 0x800 else v
    else:
        imm = 0
    return op, f3, f7, imm


@cocotb.test()
async def dbg_operands(dut):
    cocotb.start_soon(Clock(dut.clk, 10, unit="ns").start())

    cfg = GenConfig(seed=SEED, n_instr=180, include_cr=bool(SEED % 2),
                    hazard_rate=0.8, branch_rate=0.09)
    src = generate(cfg)
    prog = assemble(src, base=0)

    iss = RV32ISS(mem_size=1 << 16)
    prog.load_into(iss)
    ref = []
    for _ in range(600):
        c = iss.step()
        if c is None:
            break
        op, f3, f7, imm = _decode(c.instr)
        rs1, rs2 = (c.instr >> 15) & 0x1F, (c.instr >> 20) & 0x1F
        # operand values as the ISA sees them (b = immediate for op-imm/loads)
        a = iss.regs[rs1] if rs1 else 0
        b = iss.regs[rs2] if rs2 else 0
        ref.append((c, op, f3, f7, imm, a, b))

    dut.rst_n.value = 0
    dut.imem_we.value = 0
    await RisingEdge(dut.clk)
    last = max(a >> 2 for a, _ in prog.words)
    for idx in range(last + 32):
        await FallingEdge(dut.clk)
        dut.imem_waddr.value = idx
        dut.imem_wdata.value = NOP
        dut.imem_we.value = 1
        await RisingEdge(dut.clk)
    for addr, word in prog.words:
        await FallingEdge(dut.clk)
        dut.imem_waddr.value = (addr >> 2) & 0x3FF
        dut.imem_wdata.value = word & 0xFFFF_FFFF
        dut.imem_we.value = 1
        await RisingEdge(dut.clk)
    await FallingEdge(dut.clk)
    dut.imem_we.value = 0
    for _ in range(3):
        await RisingEdge(dut.clk)
    await FallingEdge(dut.clk)
    dut.rst_n.value = 1

    ex_by_pc = {}
    wb = []
    for _ in range(len(ref) * 3 + 3000):
        await FallingEdge(dut.clk)
        try:
            if int(dut.ex_valid.value):
                ex_by_pc.setdefault(int(dut.ex_pc.value), []).append(
                    (int(dut.ex_instr.value), int(dut.ex_a.value), int(dut.ex_b.value)))
            if int(dut.obs_wb_valid.value):
                wb.append((int(dut.obs_wb_pc.value), int(dut.obs_wb_instr.value),
                           int(dut.obs_wb_we.value), int(dut.obs_wb_data.value)))
                if len(wb) >= len(ref):
                    break
        except ValueError:
            continue

    for i, ((pc, instr, we, wdata), (c, op, f3, f7, imm, a, b)) in enumerate(zip(wb, ref)):
        if c.instr != instr or c.pc != pc:
            print(f"#{i} DESYNC pc={pc:#x}/{c.pc:#x} instr={instr:#010x}/{c.instr:#010x}",
                  file=sys.stderr, flush=True)
            break
        if we and (instr & 0xF80) != 0 and wdata != c.wdata:
            print(f"#{i} MISMATCH pc={pc:#x} instr={instr:#010x} "
                  f"op={op:#02x} f3={f3:b} f7={f7:07b} imm={imm:#x} "
                  f"rs1={(instr>>15)&0x1F} rs2={(instr>>20)&0x1F}",
                  file=sys.stderr, flush=True)
            print(f"   ISS  a={a:#010x} b={b:#010x} -> {c.wdata:#x}",
                  file=sys.stderr, flush=True)
            for (ei, ea, eb) in ex_by_pc.get(pc, [])[:2]:
                print(f"   DUT  instr={ei:#010x} ex_a={ea:#010x} ex_b={eb:#010x} -> {wdata:#x}",
                      file=sys.stderr, flush=True)
            break
    else:
        print("no mismatch", file=sys.stderr, flush=True)
