"""Negative self-test: run one short program and let a deliberately broken harness
build (BUG=n) trip the stage observer.

This test intentionally makes no assertions of its own. The Makefile target greps
the simulator log for "Assertion failed"; absence of that line means the observer
is dead code, which is the failure mode we care about (Verilator silently drops
SVA without --assert).
"""

import cocotb
from cocotb.clock import Clock
from cocotb.triggers import FallingEdge, RisingEdge

from asm import assemble

CLK_NS = 10


async def _load(dut, words):
    """Load the program, NOP-fill imem and zero dmem, then release reset.

    Same discipline as the positive suite: X in memory would let an assertion
    fire for a reason that has nothing to do with the injected deviation.
    """
    dut.rst_n.value = 0
    dut.imem_we.value = 0
    dut.dm_we.value = 0
    await RisingEdge(dut.clk)
    last = max(a >> 2 for a, _ in words)
    for idx in range(last + 32):
        await FallingEdge(dut.clk)
        dut.imem_waddr.value = idx
        dut.imem_wdata.value = 0x0000_0013
        dut.imem_we.value = 1
        await RisingEdge(dut.clk)
    for idx in range(1024):
        await FallingEdge(dut.clk)
        dut.dm_waddr.value = idx
        dut.dm_wdata.value = 0
        dut.dm_we.value = 1
        await RisingEdge(dut.clk)
    for addr, word in words:
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


@cocotb.test()
async def run_program(dut):
    cocotb.start_soon(Clock(dut.clk, CLK_NS, unit="ns").start())

    # One program that reaches every unit: immediate decode (including a NEGATIVE
    # load offset), ALU, CR-D3, signed vs unsigned compare, load extension of a
    # halfword whose sign bit is set, an x0 write attempt, fetch flow, writeback.
    src = """
        .org 0
        li   s0, 0x100
        li   t0, 0x8000_0001
        li   t1, 0x0000_0002
        li   s1, 0xFFFF_F000
        addi t2, t0, -1
        sub  t3, t1, t0
        blt  t3, t1, b1
        li   t4, 0xBAD
    b1: min  t5, t0, t1
        maxu t6, t0, t1
        sh1add a0, t1, t1
        bltu t0, t1, b2
        li   a7, 0xBAD
    b2: sw   t0, 0(s0)
        sw   s1, 4(s0)
        lh   a1, 4(s0)
        lbu  a2, 1(s0)
        lw   a3, -4(s0)
        beq  a3, t0, b3
        li   a4, 0xBAD
    b3: jal  ra, b4
        li   a5, 0xBAD
    b4: addi a6, a3, 1
        addi zero, t0, 7
        j    b4
    """
    prog = assemble(src, base=0)
    await _load(dut, prog.words)
    for _ in range(400):
        await FallingEdge(dut.clk)
