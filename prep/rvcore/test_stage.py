"""Self-test for the per-stage SVA observer + the retire-stream lockstep method.

Two independent checks run in the same simulation:

  1. SVA (in rv_stage_sva.sv, bound by rv_stage_bind_selftest.sv). Every stage's
     datapath value is compared against the ISA reference on every cycle. If any
     assertion fires, Verilator prints "Assertion failed" and the Makefile target
     fails.

  2. Retire-stream lockstep, from cocotb: every writeback event is compared, in
     order, with the commit produced by prep/rviss/rv32_iss.py (a separately
     written, host-tested ISA simulator). This is the check that catches
     wrong-path execution, dropped instructions and duplicated instructions -
     things a per-stage value check cannot see.

Why both: the SVA proves *each stage computes the ISA value*; the lockstep proves
*the pipeline as a whole retires the right program*. A silent mutation that keeps
every stage locally consistent but reorders/drops work is invisible to (1) and
caught by (2); a mutation inside a stage is caught by (1) even on a cycle where
the final architectural state happens to be unchanged.
"""

import random

import cocotb
from cocotb.clock import Clock
from cocotb.triggers import FallingEdge, RisingEdge

from asm import assemble
from randprog import GenConfig, generate
from rv32_iss import RV32ISS

CLK_NS = 10
NOP = 0x0000_0013
N_COMMITS = 600
DATA_BASE = 0x8000
DATA_WORDS = 64


def _writes_rd(instr: int) -> bool:
    op = instr & 0x7F
    return op in (0x33, 0x13, 0x03, 0x37, 0x17, 0x6F, 0x67)


async def _load_imem(dut, words, fill_to=None):
    """Write the program through the harness load port (reset held low).

    imem is filled with NOPs up to `fill_to` first. Wrong-path fetches are a
    normal part of a pipelined core, and an uninitialized memory would return X
    for them: that X can propagate into a stage tap and make the stage assertions
    meaningless. Filling makes every fetched word a real (legal) instruction.
    """
    dut.rst_n.value = 0
    dut.imem_we.value = 0
    await RisingEdge(dut.clk)
    last = max(addr >> 2 for addr, _ in words)
    for idx in range(0, fill_to if fill_to is not None else last + 32):
        await FallingEdge(dut.clk)
        dut.imem_waddr.value = idx & 0x3FF
        dut.imem_wdata.value = NOP
        dut.imem_we.value = 1
        await RisingEdge(dut.clk)
    for addr, word in words:
        await FallingEdge(dut.clk)
        dut.imem_waddr.value = (addr >> 2) & 0x3FF
        dut.imem_wdata.value = word & 0xFFFF_FFFF
        dut.imem_we.value = 1
        await RisingEdge(dut.clk)
    await FallingEdge(dut.clk)
    dut.imem_we.value = 0

async def _clear_dmem(dut, words=1024):
    """Zero the harness data memory.

    dmem is not in the reset domain, and all cocotb tests in a module share one
    simulation: without this a later program would read the previous program's
    stores and the ISS comparison would be meaningless. Clears the whole memory,
    not just the scratch window - a stray offset can land anywhere.
    """
    dut.dm_we.value = 0
    await RisingEdge(dut.clk)
    for idx in range(words):
        await FallingEdge(dut.clk)
        dut.dm_waddr.value = idx
        dut.dm_wdata.value = 0
        dut.dm_we.value = 1
        await RisingEdge(dut.clk)
    await FallingEdge(dut.clk)
    dut.dm_we.value = 0


async def _reset(dut, cycles=3):
    dut.rst_n.value = 0
    for _ in range(cycles):
        await RisingEdge(dut.clk)
    await FallingEdge(dut.clk)
    dut.rst_n.value = 1


def _iss_commits(src, n):
    iss = RV32ISS(mem_size=1 << 16)
    assemble(src, base=0).load_into(iss)
    commits = []
    for _ in range(n):
        c = iss.step()
        if c is None:
            break
        commits.append(c)
    assert iss.csrs["mcause"] == 0, f"program trapped: cause={iss.csrs['mcause']}"
    return iss, commits


async def _run_and_collect(dut, n):
    got = []
    for _ in range(20000):
        await FallingEdge(dut.clk)
        if dut.obs_wb_valid.value:
            got.append(dict(
                pc=int(dut.obs_wb_pc.value),
                instr=int(dut.obs_wb_instr.value),
                we=bool(dut.obs_wb_we.value),
                rd=int(dut.obs_wb_rd.value),
                wdata=int(dut.obs_wb_data.value),
            ))
            if len(got) >= n:
                break
    return got


async def _read_reg(dut, idx):
    dut.obs_reg_addr.value = idx
    await FallingEdge(dut.clk)
    return int(dut.obs_reg_data.value)


async def _read_mem(dut, words):
    out = []
    for i in range(words):
        dut.dm_obs_addr.value = i
        await FallingEdge(dut.clk)
        out.append(int(dut.dm_obs_data.value))
    return out


@cocotb.test()
async def stage_contracts_and_retire_order(dut):
    cocotb.start_soon(Clock(dut.clk, CLK_NS, unit="ns").start())

    cfg = GenConfig(seed=3, n_instr=250, include_cr=True, hazard_rate=0.7,
                    branch_rate=0.10, loadstore_rate=0.28)
    src = generate(cfg)
    prog = assemble(src, base=0)
    iss, commits = _iss_commits(src, N_COMMITS)
    assert len(commits) >= 200, "program too short for a meaningful check"

    await _load_imem(dut, prog.words)
    await _clear_dmem(dut)
    await _reset(dut)

    got = await _run_and_collect(dut, len(commits))
    assert len(got) == len(commits), \
        f"retired {len(got)} instructions, ISS committed {len(commits)}"

    for i, (g, c) in enumerate(zip(got, commits)):
        assert g["pc"] == c.pc, f"#{i}: pc {g['pc']:#x} != ISS {c.pc:#x}"
        assert g["instr"] == c.instr, \
            f"#{i} @{c.pc:#x}: instr {g['instr']:#010x} != ISS {c.instr:#010x}"
        if _writes_rd(c.instr) and (c.instr & 0xF80) != 0:
            assert g["we"], f"#{i} @{c.pc:#x}: expected a register write"
            assert g["rd"] == ((c.instr >> 7) & 0x1F), \
                f"#{i} @{c.pc:#x}: rd {g['rd']} != {(c.instr >> 7) & 0x1F}"
            assert g["wdata"] == c.wdata, \
                (f"#{i} @{c.pc:#x}: instr {c.instr:#010x} "
                 f"wdata {g['wdata']:#x} != ISS {c.wdata:#x}")
        else:
            assert not g["we"], f"#{i} @{c.pc:#x}: unexpected register write"

    # Final architectural state: registers and the data region.
    for r in range(1, 32):
        assert await _read_reg(dut, r) == iss.regs[r], \
            f"x{r} = {await _read_reg(dut, r):#x}, ISS {iss.regs[r]:#x}"

    dut_words = await _read_mem(dut, DATA_WORDS)
    for i in range(DATA_WORDS):
        want = int.from_bytes(iss.mem[DATA_BASE + 4 * i: DATA_BASE + 4 * i + 4], "little")
        assert dut_words[i] == want, \
            f"mem[{DATA_BASE + 4 * i:#x}] = {dut_words[i]:#x}, ISS {want:#x}"


@cocotb.test()
async def fetch_stream_is_from_the_program(dut):
    """A cheap, directed check that the fetch tap tracks the PC sequence.

    Catches a fetch unit that returns data for an address it did not present
    (a class of bug the value-only stage assertions cannot see, because they do
    not have the program image). This is the SVA complement promised in the
    README: memory *contents* belong to cocotb, not to SVA.
    """
    cocotb.start_soon(Clock(dut.clk, CLK_NS, unit="ns").start())

    src = """
        .org 0
        li   t0, 0x11
        li   t1, 0x22
        add  t2, t0, t1
        sw   t2, 0(s0)
        lw   t3, 0(s0)
        beq  t3, t2, ok
        li   t4, 0xBAD
    ok: addi t5, t3, 1
        j    ok
    """
    prog = assemble(src, base=0)
    word_at = {a >> 2: w for a, w in prog.words}
    # wrong-path fetches land on the NOP fill, not on X
    # data base s0 = 0 (the harness maps dmem independently of imem)

    await _load_imem(dut, prog.words)
    await _clear_dmem(dut)
    await _reset(dut)

    for _ in range(40):
        await FallingEdge(dut.clk)
        if not dut.if_valid.value:
            continue
        idx = int(dut.if_pc.value) >> 2
        assert int(dut.if_instr.value) == word_at.get(idx, NOP), (
            f"if_pc={int(dut.if_pc.value):#x} returned {int(dut.if_instr.value):#010x}, "
            f"program has {word_at.get(idx, NOP):#010x}")


@cocotb.test()
async def random_seeds_retire_in_order(dut):
    """Hazard-dense random programs, several seeds, same two-way check."""
    cocotb.start_soon(Clock(dut.clk, CLK_NS, unit="ns").start())

    for seed in (11, 12, 13):
        cfg = GenConfig(seed=seed, n_instr=180, include_cr=bool(seed % 2),
                        hazard_rate=0.8, branch_rate=0.09)
        src = generate(cfg)
        prog = assemble(src, base=0)
        iss, commits = _iss_commits(src, 400)
        await _load_imem(dut, prog.words)
        await _clear_dmem(dut)
        await _reset(dut)
        got = await _run_and_collect(dut, len(commits))
        assert len(got) == len(commits), f"seed {seed}: retire count mismatch"
        for i, (g, c) in enumerate(zip(got, commits)):
            assert g["pc"] == c.pc and g["instr"] == c.instr, \
                f"seed {seed} #{i}: {g['pc']:#x}/{g['instr']:#010x} != " \
                f"{c.pc:#x}/{c.instr:#010x}"
            if _writes_rd(c.instr) and (c.instr & 0xF80) != 0:
                assert g["wdata"] == c.wdata, \
                    (f"seed {seed} #{i} @{c.pc:#x}: instr {c.instr:#010x} "
                     f"wdata {g['wdata']:#x} != {c.wdata:#x}")
