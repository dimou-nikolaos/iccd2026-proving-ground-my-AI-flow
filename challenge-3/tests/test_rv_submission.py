"""Submission test suite for rv_core (Phase 3a): directed corpus.

Port of the 44 Phase-1c probes (same test names) plus directed tests for
reset-mid-operation (REQ-01/02), muldiv/load mixtures under wait states
(REQ-10/27), the CR-D3 forwarding matrix (REQ-20/12, change_request.md
acceptance + x0/alias corners), and counter semantics (spec section 8,
REQ-33). Expectations are derived from spec.md, never from the RTL.

REQ-05 and REQ-42 are structural (lint/synth stage), not observable by
running programs at the ports, so they have no test here.
"""

import cocotb
from cocotb.triggers import FallingEdge

from rv_asm import (DEVICE_IRQ, MCYCLE, MCYCLEH, MCAUSE, MIE, MIP, MINSTRET, MINSTRETH,
                    MISA, MSCRATCH, MSTATUS, MEPC, MTVEC, MTVAL, Asm, _b, _i, _j, _r)
from rv_core_env import Env

M32 = 0xFFFFFFFF
TOHOST = 0xFFFF0000


def s32(v):
    v &= M32
    return v - (1 << 32) if v >> 31 else v


async def runp(dut, a, regs=(), data=None, timeout=12000, **kw):
    """Assemble, load, run; REQ-03/04 hold + REQ-07/08 checks are always on in
    Env.step. Returns the Env."""
    env = Env(dut, **kw)
    env.load(a.base, a.build())
    for base, words in (data or {}).items():
        env.load(base, words)
    await env.start()
    await env.run_checked(timeout)
    env.values = env.sig(len(regs))
    return env


def finish(a, regs):
    a.dump(regs)
    a.halt()


def hdl_install(a):
    """Point MTVEC at label 'hdl', then jump over the handler body that the
    hdl_* helpers emit right behind it (the main program resumes at 'main').
    Uses x25/x28/x29."""
    a.la(29, "hdl")
    a.csrrw(28, MTVEC, 29)
    a.jal(25, "main")


def hdl_pack(a, cc=20, tc=21):
    """Handler: packs MCAUSE into 4-bit fields of cc and MTVAL[1:0] into 4-bit
    fields of tc (first event in the top field of the sequence), then skips the
    trapping instruction (mepc+4). Uses x26/x27."""
    a.label("hdl")
    a.csrrs(26, MCAUSE, 0)
    a.slli(cc, cc, 4)
    a.or_(cc, cc, 26)
    a.csrrs(26, MTVAL, 0)
    a.andi(26, 26, 3)
    a.slli(tc, tc, 4)
    a.or_(tc, tc, 26)
    a.csrrs(27, MEPC, 0)
    a.addi(27, 27, 4)
    a.csrrw(27, MEPC, 27)
    a.mret()
    a.label("main")


def hdl_precise(a, rc=20, rt=21, re=22, rs=None, ret=None):
    """Handler: MCAUSE->rc, MTVAL->rt, MEPC->re, MSTATUS->rs; then mrets either
    past the trapping instruction (default) or to label `ret`."""
    a.label("hdl")
    a.csrrs(rc, MCAUSE, 0)
    a.csrrs(rt, MTVAL, 0)
    a.csrrs(re, MEPC, 0)
    if rs is not None:
        a.csrrs(rs, MSTATUS, 0)
    a.csrrs(27, MEPC, 0)
    if ret is None:
        a.addi(27, 27, 4)
    else:
        a.la(27, ret)
    a.csrrw(27, MEPC, 27)
    a.mret()
    a.label("main")


def hdl_irq(a, ret, rc=20, re=21):
    """Interrupt/trap handler: records MCAUSE/MEPC, clears both interrupt lines
    and MSTATUS.MIE|MPIE, then mrets to `ret`."""
    a.label("hdl")
    a.csrrs(rc, MCAUSE, 0)
    a.csrrs(re, MEPC, 0)
    a.li(31, DEVICE_IRQ)
    a.addi(26, 0, 0)
    a.sw(26, 31, 0)
    a.li(27, 0x88)
    a.csrrc(26, MSTATUS, 27)
    a.la(27, ret)
    a.csrrw(27, MEPC, 27)
    a.mret()
    a.label("main")


def irq_enable(a, mie_bits, tmp=1, tmp2=28):
    """Set MIE CSR bits (0x304) then set MSTATUS.MIE (bit 3)."""
    a.li(tmp, mie_bits)
    a.csrrw(tmp2, MIE, tmp)
    a.csrrs(tmp, MSTATUS, 0)
    a.addi(tmp, tmp, 8)
    a.csrrw(tmp2, MSTATUS, tmp)


# ---------------------------------------------------------------- reset / bus

@cocotb.test()
async def test_req01_reset_zero(dut):
    """REQ-01: general registers are 0 after reset."""
    a = Asm()
    a.addi(1, 3, 5)
    a.addi(2, 4, 7)
    a.slt(3, 5, 6)
    finish(a, [1, 2, 3])
    tb = await runp(dut, a, regs=[1, 2, 3])
    assert tb.values == [5, 7, 0], tb.values


@cocotb.test()
async def test_req02_first_fetch(dut):
    """REQ-02: dmem idle after reset, first fetch from address 0."""
    a = Asm()
    a.halt()
    tb = await runp(dut, a)
    assert tb.fetch_log[0] == 0, tb.fetch_log[:4]
    assert tb.load_log == [] and len(tb.store_log) == 1, (tb.load_log, tb.store_log)
    assert tb.store_log[0][0] == TOHOST, tb.store_log


@cocotb.test()
async def test_req03_04_08_hold(dut):
    """REQ-03/04: requests hold while not ready; REQ-08: addresses aligned."""
    for seed in (1, 7, 23):
        a = Asm()
        a.li(1, 0x8000)
        a.li(2, 0x1234)
        a.sw(2, 1, 0)
        a.lw(3, 1, 0)
        a.lw(4, 1, 4)
        a.beq(3, 2, "ok")
        a.addi(5, 5, 1)
        a.label("ok")
        a.mul(6, 2, 2)
        a.addi(6, 6, 1)
        a.jal(8, "f")
        a.addi(5, 5, 1)
        a.label("f")
        finish(a, [3, 5, 6])
        tb = await runp(dut, a, regs=[3, 5, 6], data={0x8000: [0x1234, 1]},
                        istall=0.7, dstall=0.7, seed=seed)
        assert tb.values == [0x1234, 0, ((0x1234 * 0x1234) + 1) & M32], (seed, tb.values)


@cocotb.test()
async def test_req01_reset_mid_operation(dut):
    """REQ-01/02: reset asserted mid-operation; re-run starts from scratch."""
    def prog():
        a = Asm()
        a.li(1, 0x8000)
        a.li(2, 20)
        a.label("loop")
        a.addi(3, 3, 1)
        a.mul(5, 3, 3)
        a.bne(3, 2, "loop")
        a.sw(5, 1, 0)
        finish(a, [3, 5])
        return a

    env = Env(dut, seed=4)
    env.load(0, prog().build())
    env.load(0x8000, [0])
    await env.start()
    for _ in range(40):
        await env.step()
    assert not env.done, "program finished before the reset point"
    await env.reset_pulse(3)
    for _ in range(2):
        await FallingEdge(dut.clk)
        assert int(dut.dmem_valid.value) == 0, "REQ-02: dmem_valid set after reset"

    env2 = Env(dut, seed=5)
    env2.mem = env.mem
    await env2.start()
    await env2.run_checked()
    assert env2.fetch_log[0] == 0, env2.fetch_log[:4]
    vals = env2.sig(2)
    assert vals == [20, 400], ("REQ-01: re-run after mid-op reset gave %s "
                               "(registers/pipeline state leaked)" % vals)


@cocotb.test()
async def test_req01_reset_consecutive(dut):
    """REQ-01: consecutive reset pulses leave all-zero state."""
    def prog():
        a = Asm()
        a.addi(1, 0, 6)
        a.slli(2, 1, 3)
        a.add(3, 2, 1)
        finish(a, [1, 2, 3])
        return a

    env = Env(dut, seed=6)
    env.load(0, prog().build())
    await env.start()
    await env.reset_pulse(2)
    await env.reset_pulse(2)
    await env.reset_pulse(1)
    env2 = Env(dut, seed=7)
    env2.mem = env.mem
    await env2.start()
    await env2.run_checked()
    assert env2.fetch_log[0] == 0, env2.fetch_log[:4]
    vals = env2.sig(3)
    assert vals == [6, 48, 54], vals


# ------------------------------------------------------ execution / pipelines

@cocotb.test()
async def test_req06_access_count(dut):
    """REQ-06: one access per executed load/store; none for traps or wrong path."""
    a = Asm()
    hdl_install(a)
    hdl_pack(a)
    a.li(1, 0x8000)
    a.li(2, 7)
    a.sw(2, 1, 0)
    a.lw(3, 1, 0)
    a.word(0xFFFFFFFF)
    a.lw(5, 1, 1)
    a.jal(29, "skip")
    a.sw(2, 1, 8)
    a.label("skip")
    a.lw(4, 1, 0)
    finish(a, [3, 4, 20])
    tb = await runp(dut, a, regs=[3, 4, 20], data={0x8000: [7, 0, 0, 0]})
    assert tb.values == [7, 7, 0x24], [hex(v) for v in tb.values]
    assert tb.load_log == [0x8000, 0x8000], tb.load_log
    st = [s for s in tb.store_log if s[0] == 0x8000]
    assert len(st) == 1 and st[0][2] & 0xFF == 7, tb.store_log


@cocotb.test()
async def test_req07_wstrb(dut):
    """REQ-07: byte-lane strobes and data placement for SB/SH/SW."""
    a = Asm()
    a.li(1, 0x8000)
    a.li(2, 0x11223344)
    a.sb(2, 1, 0)
    a.sb(2, 1, 1)
    a.sb(2, 1, 3)
    a.sh(2, 1, 0)
    a.sh(2, 1, 2)
    a.sw(2, 1, 4)
    a.lw(3, 1, 0)
    a.lw(4, 1, 4)
    finish(a, [3, 4])
    tb = await runp(dut, a, regs=[3, 4], data={0x8000: [0, 0]})
    assert tb.values == [0x33443344, 0x11223344], [hex(v) for v in tb.values]

    def lanes(sb):
        return sum(0xFF << (8 * i) for i in range(4) if sb >> i & 1)

    exp = [(0x8000, 0b0001, 0x44), (0x8000, 0b0010, 0x44 << 8), (0x8000, 0b1000, 0x44 << 24),
           (0x8000, 0b0011, 0x3344), (0x8000, 0b1100, 0x3344 << 16), (0x8004, 0b1111, 0x11223344)]
    got = [(ad, sb, wd & lanes(sb)) for (ad, sb, wd) in tb.store_log if ad in (0x8000, 0x8004)]
    want = [(ad, sb, wd & lanes(sb)) for (ad, sb, wd) in exp]
    assert got == want, (["%08x %b %08x" % g for g in got], ["%08x %b %08x" % w for w in want])


@cocotb.test()
async def test_req09_fence_wfi_noop(dut):
    """REQ-09: FENCE, FENCE.I and WFI run as no-ops."""
    a = Asm()
    a.li(1, 5)
    a.fence()
    a.word(0x0000100F)
    a.wfi()
    a.addi(2, 1, 1)
    finish(a, [2])
    tb = await runp(dut, a, regs=[2])
    assert tb.values == [6], tb.values


@cocotb.test()
async def test_req10_same_under_wait_states(dut):
    """REQ-10: results do not depend on memory wait states."""
    def prog():
        a = Asm()
        a.li(1, 0x8000)
        a.lw(2, 1, 0)
        a.addi(3, 2, 1)
        a.mul(4, 3, 3)
        a.sw(4, 1, 4)
        a.lw(5, 1, 4)
        a.bne(5, 4, "bad")
        a.addi(6, 5, -4)
        a.label("bad")
        a.div(7, 6, 2)
        a.rem(8, 6, 2)
        a.jal(9, "done")
        a.addi(6, 6, 1)
        a.label("done")
        finish(a, [4, 6, 7, 8])
        return a

    for istall, dstall, seed in ((0.0, 0.0, 5), (0.4, 0.5, 6), (0.85, 0.85, 9)):
        tb = await runp(dut, prog(), regs=[4, 6, 7, 8], data={0x8000: [20, 0]},
                        istall=istall, dstall=dstall, seed=seed)
        assert tb.values == [441, 437, 21, 17], (istall, dstall, seed, tb.values)


@cocotb.test()
async def test_dev_d5_muldiv_during_load_stall(dut):
    """D5 probe (REQ-10/27): muldiv finishing while an older load is stalled."""
    for seed in (3, 11, 42, 77):
        a = Asm()
        a.li(1, 0x8000)
        a.lw(2, 1, 0)
        a.li(4, 7)
        a.li(5, 6)
        a.mul(6, 4, 5)
        a.addi(7, 2, 1)
        a.lw(8, 1, 4)
        a.div(9, 6, 8)
        a.add(10, 9, 7)
        finish(a, [6, 7, 9, 10])
        tb = await runp(dut, a, regs=[6, 7, 9, 10], data={0x8000: [40, 5]},
                        istall=0.3, dstall=0.3, seed=seed)
        assert tb.values == [42, 41, 8, 49], (seed, tb.values)


@cocotb.test()
async def test_req10_27_muldiv_load_mix(dut):
    """REQ-10/27: back-to-back muldiv interleaved with loads, under wait states."""
    dwords = [0xFFFFFFF9, 100000, 3]

    def prog():
        a = Asm()
        a.li(1, 0x8000)
        a.lw(2, 1, 0)
        a.lw(3, 1, 4)
        a.div(4, 3, 2)
        a.rem(5, 3, 2)
        a.mul(6, 4, 4)
        a.lw(7, 1, 8)
        a.div(8, 6, 7)
        a.mulhu(9, 4, 2)
        a.remu(10, 3, 2)
        a.mul(11, 2, 2)
        a.lw(12, 1, 0)
        a.add(13, 11, 12)
        a.lw(14, 1, 4)
        a.div(15, 14, 12)
        a.jal(29, "skip")
        a.addi(13, 13, 1)
        a.label("skip")
        a.rem(16, 12, 3)
        finish(a, [4, 5, 6, 8, 9, 10, 13, 15, 16])
        return a

    x2, x3, x7 = s32(dwords[0]), dwords[1], dwords[2]
    div_tr = lambda q, r: q
    q1 = -(abs(x3) // abs(x2))          # 100000 / -7, toward zero
    r1 = abs(x3) % abs(x2)              # dividend positive -> remainder positive
    x6 = (q1 * q1) & M32
    exp = [q1 & M32, r1, x6, (x6 // x7) & M32,
           ((q1 & M32) * dwords[0]) >> 32,
           x3 % dwords[0],
           ((x2 * x2) & M32 + x2) & M32 if False else ((49 + x2) & M32),
           q1 & M32,
           x2 & M32]
    for istall, dstall, seed in ((0.0, 0.0, 31), (0.4, 0.4, 32), (0.7, 0.3, 33)):
        tb = await runp(dut, prog(), regs=[4, 5, 6, 8, 9, 10, 13, 15, 16],
                        data={0x8000: dwords}, istall=istall, dstall=dstall, seed=seed)
        assert tb.values == exp, (istall, dstall, seed,
                                  [hex(v) for v in tb.values], [hex(e) for e in exp])


@cocotb.test()
async def test_dev_d1_x0_forward(dut):
    """REQ-11 / D1: x0 read right behind an instruction with rd=x0."""
    a = Asm()
    a.li(1, 5)
    a.li(2, 3)
    a.li(9, 0x8000)
    a.csrrw(27, MSCRATCH, 1)
    a.addi(0, 1, 5)
    a.addi(5, 0, 0)
    a.jal(0, "tgt")
    a.addi(6, 0, 0)
    a.label("tgt")
    a.addi(6, 0, 0)
    a.csrrw(0, MSCRATCH, 1)
    a.addi(7, 0, 0)
    a.mul(0, 1, 2)
    a.addi(8, 0, 0)
    a.lw(0, 9, 0)
    a.addi(10, 0, 0)
    finish(a, [5, 6, 7, 8, 10])
    tb = await runp(dut, a, regs=[5, 6, 7, 8, 10], data={0x8000: [42]})
    assert tb.values == [0, 0, 0, 0, 0], tb.values


@cocotb.test()
async def test_req11_x0_reads_zero(dut):
    """REQ-11: x0 reads 0 in ordinary operand positions."""
    a = Asm()
    a.addi(3, 0, -1)
    a.sltu(4, 0, 3)
    a.slt(5, 3, 0)
    a.xor(6, 0, 3)
    a.bne(0, 3, "ok")
    a.addi(7, 7, 1)
    a.label("ok")
    a.andi(8, 0, 1)
    finish(a, [3, 4, 5, 6, 7, 8])
    tb = await runp(dut, a, regs=[3, 4, 5, 6, 7, 8])
    assert tb.values == [0xFFFFFFFF, 1, 1, 0xFFFFFFFF, 0, 0], [hex(v) for v in tb.values]


@cocotb.test()
async def test_req12_forward_chain(dut):
    """REQ-12: results available to the next instruction (ALU/mul/load/store)."""
    a = Asm()
    a.li(1, 10)
    a.addi(2, 1, 1)
    a.add(3, 2, 1)
    a.add(4, 3, 2)
    a.mul(5, 4, 4)
    a.addi(6, 5, 1)
    a.li(7, 0x8000)
    a.lw(8, 7, 0)
    a.add(9, 8, 6)
    a.sw(9, 7, 4)
    a.lw(10, 7, 4)
    a.sll(11, 10, 6)
    finish(a, [3, 4, 6, 9, 10, 11])
    tb = await runp(dut, a, regs=[3, 4, 6, 9, 10, 11], data={0x8000: [100, 0]})
    assert tb.values == [21, 32, 1025, 1125, 1125, 2250], tb.values


@cocotb.test()
async def test_req13_wrong_path_no_effect(dut):
    """REQ-13: instructions fetched after a taken branch/jump have no effect."""
    a = Asm()
    a.li(1, 0x8000)
    a.li(2, 5)
    a.bne(1, 2, "skip")
    a.sw(2, 1, 0)
    a.label("skip")
    a.jal(29, "skip2")
    a.sw(2, 1, 4)
    a.label("skip2")
    a.lw(3, 1, 0)
    a.lw(4, 1, 4)
    finish(a, [3, 4])
    tb = await runp(dut, a, regs=[3, 4], data={0x8000: [0, 0]})
    assert tb.values == [0, 0], tb.values
    assert [s for s in tb.store_log if s[0] in (0x8000, 0x8004)] == [], tb.store_log


@cocotb.test()
async def test_req14_jalr_clears_bit0(dut):
    """REQ-14: JALR clears bit 0 of the computed target."""
    a = Asm()
    a.la(1, "tgt")
    a.addi(1, 1, 1)
    pc = a.here()
    a.jalr(2, 1, 0)
    a.addi(5, 5, 1)
    a.label("tgt")
    a.addi(9, 9, 1)
    a.jal(28, "end")
    a.label("end")
    finish(a, [9, 2, 5])
    tb = await runp(dut, a, regs=[9, 2, 5])
    assert tb.values[0] == 1, tb.values
    assert tb.values[1] == pc + 4, (hex(tb.values[1]), hex(pc + 4))
    assert tb.values[2] == 0, tb.values[2]


@cocotb.test()
async def test_req15_branch_misaligned(dut):
    """REQ-15/36: taken branch to a target with bit 1 set -> cause 0, mtval=target."""
    a = Asm()
    hdl_install(a)
    hdl_precise(a, rc=20, rt=21, re=22)
    a.li(1, 7)
    pc = a.here()
    a.items.append(lambda p: _b(6, 1, 1, 0))
    a.label("after")
    a.addi(1, 1, 1)
    finish(a, [20, 21, 22, 1])
    tb = await runp(dut, a, regs=[20, 21, 22, 1])
    assert tb.values[0] == 0, hex(tb.values[0])
    assert tb.values[1] == pc + 6, (hex(tb.values[1]), hex(pc + 6))
    assert tb.values[2] == pc, (hex(tb.values[2]), hex(pc))
    assert tb.values[3] == 8, tb.values


@cocotb.test()
async def test_req15_jal_misaligned(dut):
    """REQ-15/36: JAL to a target with bit 1 set -> cause 0, mtval=target."""
    a = Asm()
    hdl_install(a)
    hdl_precise(a, rc=20, rt=21, re=22)
    pc = a.here()
    a.items.append(lambda p: _j(6, 28))
    a.label("after")
    a.addi(1, 1, 1)
    finish(a, [20, 21, 22, 1])
    tb = await runp(dut, a, regs=[20, 21, 22, 1])
    assert tb.values[0] == 0, hex(tb.values[0])
    assert tb.values[1] == pc + 6, (hex(tb.values[1]), hex(pc + 6))
    assert tb.values[3] == 1, tb.values


@cocotb.test()
async def test_req16_shamt_uses_low_five(dut):
    """REQ-16: shift amount is the low five bits of the register operand."""
    a = Asm()
    a.li(1, 1)
    a.li(3, 33)
    a.sll(4, 1, 3)
    a.li(5, 0x80000000)
    a.srl(6, 5, 3)
    a.sra(7, 5, 3)
    a.li(8, 0xFFFFFFFF)
    a.sra(9, 8, 3)
    finish(a, [4, 6, 7, 9])
    tb = await runp(dut, a, regs=[4, 6, 7, 9])
    assert tb.values == [2, 0x40000000, 0xC0000000, 0xFFFFFFFF], [hex(v) for v in tb.values]


@cocotb.test()
async def test_req17_sltiu_unsigned_imm(dut):
    """REQ-17: SLTIU compares against the sign-extended immediate as unsigned."""
    a = Asm()
    a.li(1, 5)
    a.sltiu(2, 1, -1)
    a.sltiu(3, 1, 3)
    a.li(4, 0xFFFFFFFF)
    a.sltiu(5, 4, -1)
    a.li(6, 1)
    a.sltiu(7, 6, -2)
    a.sltiu(8, 0, -1)
    finish(a, [2, 3, 5, 7, 8])
    tb = await runp(dut, a, regs=[2, 3, 5, 7, 8])
    assert tb.values == [1, 0, 0, 1, 1], tb.values


# ------------------------------------------------------------ loads and stores

@cocotb.test()
async def test_req21_load_extension(dut):
    """REQ-21: LB/LH sign-extend, LBU/LHU zero-extend."""
    a = Asm()
    a.li(1, 0x8000)
    a.lb(2, 1, 0)
    a.lbu(3, 1, 0)
    a.lh(4, 1, 0)
    a.lhu(5, 1, 0)
    a.lb(6, 1, 2)
    a.lbu(7, 1, 2)
    finish(a, [2, 3, 4, 5, 6, 7])
    tb = await runp(dut, a, regs=[2, 3, 4, 5, 6, 7], data={0x8000: [0x1234FF80]})
    assert tb.values == [0xFFFFFF80, 0x80, 0xFFFFFF80, 0xFF80, 0x34, 0x34], \
        [hex(v) for v in tb.values]


@cocotb.test()
async def test_req22_misaligned_load_store(dut):
    """REQ-22/36: misaligned LW/LH/SW/SH trap (4/6, mtval=addr) with no access."""
    a = Asm()
    hdl_install(a)
    hdl_pack(a)
    a.li(1, 0x8000)
    a.li(2, 0x1234)
    a.lw(3, 1, 1)
    a.lh(4, 1, 3)
    a.sw(2, 1, 2)
    a.sh(2, 1, 1)
    a.lb(5, 1, 1)
    a.lhu(6, 1, 2)
    finish(a, [20, 21, 5, 6])
    tb = await runp(dut, a, regs=[20, 21, 5, 6], data={0x8000: [0x89ABCDEF]})
    assert tb.values[0] == 0x4466, hex(tb.values[0])
    assert tb.values[1] == 0x1321, hex(tb.values[1])
    assert tb.values[2] == 0xFFFFFFCD and tb.values[3] == 0x89AB, [hex(v) for v in tb.values]
    assert tb.load_log == [0x8000, 0x8000], tb.load_log
    assert [s for s in tb.store_log if s[0] == 0x8000] == [], tb.store_log


@cocotb.test()
async def test_req23_lb_offsets(dut):
    """REQ-23: LB/LBU at offset k return byte k of the word."""
    a = Asm()
    a.li(1, 0x8000)
    a.lb(2, 1, 0)
    a.lb(3, 1, 1)
    a.lb(4, 1, 2)
    a.lb(5, 1, 3)
    a.lbu(6, 1, 0)
    a.lbu(7, 1, 1)
    a.lbu(8, 1, 2)
    a.lbu(9, 1, 3)
    finish(a, [2, 3, 4, 5, 6, 7, 8, 9])
    tb = await runp(dut, a, regs=[2, 3, 4, 5, 6, 7, 8, 9], data={0x8000: [0x89ABCDEF]})
    assert tb.values == [0xFFFFFFEF, 0xFFFFFFCD, 0xFFFFFFAB, 0xFFFFFF89, 0xEF, 0xCD, 0xAB, 0x89], \
        [hex(v) for v in tb.values]


@cocotb.test()
async def test_req24_lhu_offset2(dut):
    """REQ-24: LHU at offset 2 returns bits [31:16] zero-extended."""
    a = Asm()
    a.li(1, 0x8000)
    a.lhu(3, 1, 2)
    a.lhu(4, 1, 0)
    finish(a, [3, 4])
    tb = await runp(dut, a, regs=[3, 4], data={0x8000: [0x89ABCDEF]})
    assert tb.values == [0x89AB, 0xCDEF], [hex(v) for v in tb.values]


@cocotb.test()
async def test_req25_lh_offset0(dut):
    """REQ-25: LH at offset 0 extends from bit 15."""
    a = Asm()
    a.li(1, 0x8000)
    a.lh(3, 1, 0)
    a.lh(4, 1, 2)
    a.lh(5, 1, 6)
    finish(a, [3, 4, 5])
    tb = await runp(dut, a, regs=[3, 4, 5], data={0x8000: [0x12348000, 0x7FFF0000]})
    assert tb.values == [0xFFFF8000, 0x00001234, 0x00007FFF], [hex(v) for v in tb.values]


@cocotb.test()
async def test_dev_d3_lh_offset2(dut):
    """REQ-25 / D3: LH at offset 2 returns bits [31:16] extended from bit 31."""
    a = Asm()
    a.li(1, 0x8000)
    a.lh(3, 1, 2)
    a.lh(4, 1, 6)
    a.lhu(5, 1, 2)
    finish(a, [3, 4, 5])
    tb = await runp(dut, a, regs=[3, 4, 5], data={0x8000: [0x80000000, 0xFFFF0000]})
    # bits[31:16]=0x8000 sign-extended from word bit31 -> 0xFFFF8000 (baseline: 0x00008000)
    assert tb.values == [0xFFFF8000, 0xFFFFFFFF, 0x8000], [hex(v) for v in tb.values]


# ------------------------------------------------------------- mul / divide

@cocotb.test()
async def test_req26_mul_variants(dut):
    """REQ-26: MUL/MULH/MULHSU/MULHU high/low bits, incl. sign combinations."""
    x1, x2, x7, x8 = 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000, 2
    a = Asm()
    a.li(1, x1)
    a.li(2, x2)
    a.li(7, x7)
    a.li(8, x8)
    a.mul(3, 1, 2)
    a.mulh(4, 1, 2)
    a.mulhu(5, 1, 2)
    a.mulhsu(6, 1, 2)
    a.mulhsu(9, 7, 8)
    a.mulh(10, 7, 8)
    a.mulhu(11, 7, 8)
    a.mulhsu(12, 8, 7)
    finish(a, [3, 4, 5, 6, 9, 10, 11, 12])
    tb = await runp(dut, a, regs=[3, 4, 5, 6, 9, 10, 11, 12])
    exp = [(x1 * x2) & M32,
           (s32(x1) * s32(x2)) >> 32 & M32,
           (x1 * x2) >> 32 & M32,
           (s32(x1) * x2) >> 32 & M32,
           (s32(x7) * x8) >> 32 & M32,
           (s32(x7) * s32(x8)) >> 32 & M32,
           (x7 * x8) >> 32 & M32,
           (s32(x8) * x7) >> 32 & M32]
    assert tb.values == exp, [[hex(v) for v in tb.values], [hex(e) for e in exp]]


@cocotb.test()
async def test_req27_muldiv_waits_pipeline(dut):
    """REQ-27: muldiv may take many cycles while the rest of the pipeline waits."""
    for seed in (2, 19):
        a = Asm()
        a.li(1, 0x8000)
        a.li(2, 100000)
        a.li(3, 37)
        a.div(4, 2, 3)
        a.rem(5, 2, 3)
        a.sw(4, 1, 0)
        a.sw(5, 1, 4)
        a.lw(6, 1, 0)
        a.mul(7, 4, 6)
        a.addi(8, 7, 1)
        finish(a, [4, 5, 8])
        tb = await runp(dut, a, regs=[4, 5, 8], data={0x8000: [0, 0]}, dstall=0.6, seed=seed)
        exp = [100000 // 37, 100000 % 37, ((100000 // 37) ** 2 + 1) & M32]
        assert tb.values == exp, (seed, tb.values, exp)
        st = [s for s in tb.store_log if s[0] in (0x8000, 0x8004)]
        assert [s[0] for s in st] == [0x8000, 0x8004], tb.store_log


@cocotb.test()
async def test_req28_div_rem_round_to_zero(dut):
    """REQ-28: DIV/REM round toward zero; REM sign follows the dividend."""
    a = Asm()
    pairs = [(7, 2), (-7, 2), (7, -2), (-7, -2)]
    regs = []
    for i, (x, y) in enumerate(pairs):
        a.li(2 * i + 1, x)
        a.li(2 * i + 2, y)
        a.div(10 + i, 2 * i + 1, 2 * i + 2)
        a.rem(18 + i, 2 * i + 1, 2 * i + 2)
        regs += [10 + i, 18 + i]
    finish(a, regs)
    tb = await runp(dut, a, regs=regs)
    exp = []
    for x, y in pairs:
        q = abs(x) // abs(y)
        q = -q if (x < 0) != (y < 0) else q
        r = abs(x) % abs(y)
        r = -r if x < 0 else r
        exp += [q & M32, r & M32]
    assert tb.values == exp, [[hex(v) for v in tb.values], [hex(e) for e in exp]]


@cocotb.test()
async def test_req29_div_int_min(dut):
    """REQ-29: DIV of -2^31 by -1 gives -2^31, REM gives 0."""
    a = Asm()
    a.li(1, 0x80000000)
    a.li(2, 0xFFFFFFFF)
    a.div(3, 1, 2)
    a.rem(4, 1, 2)
    a.divu(5, 1, 2)
    a.remu(6, 1, 2)
    finish(a, [3, 4, 5, 6])
    tb = await runp(dut, a, regs=[3, 4, 5, 6])
    assert tb.values == [0x80000000, 0, 0x80000000 // 0xFFFFFFFF, 0x80000000 % 0xFFFFFFFF], \
        [hex(v) for v in tb.values]


@cocotb.test()
async def test_dev_d4_div_by_zero(dut):
    """REQ-30 / D4: DIV=-1, DIVU=2^32-1, REM and REMU return the dividend."""
    a = Asm()
    a.li(1, 0xFFFFFFF9)
    a.li(2, 0)
    a.li(3, 9)
    a.div(4, 1, 2)
    a.divu(5, 1, 2)
    a.rem(6, 1, 2)
    a.remu(7, 1, 2)
    a.rem(8, 3, 2)
    a.remu(9, 3, 2)
    a.div(10, 3, 2)
    finish(a, [4, 5, 6, 7, 8, 9, 10])
    tb = await runp(dut, a, regs=[4, 5, 6, 7, 8, 9, 10])
    assert tb.values == [0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFF9, 0xFFFFFFF9, 9, 9, 0xFFFFFFFF], \
        [hex(v) for v in tb.values]


# ---------------------------------------------------------------------- CSRs

@cocotb.test()
async def test_req31_csr_semantics(dut):
    """REQ-31: CSRRW/CSRRS/CSRRC and imm forms: rd=old value, CSR updated."""
    a = Asm()
    a.li(1, 0x1234)
    a.csrrw(2, MSCRATCH, 1)
    a.li(1, 0x00F0)
    a.csrrs(3, MSCRATCH, 1)
    a.li(1, 0x0030)
    a.csrrc(4, MSCRATCH, 1)
    a.csrrwi(5, MSCRATCH, 5)
    a.csrrsi(6, MSCRATCH, 0xA)
    a.csrrci(7, MSCRATCH, 0xF)
    a.csrrs(8, MSCRATCH, 0)
    a.csrrs(9, MSCRATCH, 0)
    finish(a, [2, 3, 4, 5, 6, 7, 8, 9])
    tb = await runp(dut, a, regs=[2, 3, 4, 5, 6, 7, 8, 9])
    # csrrwi/csrrsi/csrrci use the zero-extended 5-bit imm: write / set / clear.
    assert tb.values == [0, 0x1234, 0x12F4, 0x12C4, 0x5, 0xF, 0, 0], \
        [hex(v) for v in tb.values]


@cocotb.test()
async def test_req31_32_read_only_csrs(dut):
    """REQ-31/32: read-only CSRs readable with rs1=x0/imm=0; writes trap (2)."""
    a = Asm()
    hdl_install(a)
    hdl_pack(a)
    a.csrrs(3, 0xF11, 0)
    a.csrrs(4, 0xF12, 0)
    a.csrrci(5, 0xF14, 0)
    a.li(1, 0x1234)
    a.csrrw(6, 0xF11, 1)
    a.csrrs(7, 0x000, 1)
    a.csrrw(8, MISA, 1)
    a.csrrs(9, MISA, 0)
    finish(a, [3, 4, 5, 9, 20])
    tb = await runp(dut, a, regs=[3, 4, 5, 9, 20])
    assert tb.values[0] == 0 and tb.values[1] == 0 and tb.values[2] == 0, tb.values[:3]
    assert tb.values[3] == 0x40001100, hex(tb.values[3])
    assert tb.values[4] == 0x22, hex(tb.values[4])


@cocotb.test()
async def test_req33_minstret(dut):
    """REQ-33: MINSTRET counts retired instrs; the read itself is not counted."""
    a = Asm()
    a.csrrs(1, MINSTRET, 0)
    a.addi(2, 1, 1)
    a.addi(3, 2, 1)
    a.addi(4, 3, 1)
    a.csrrs(5, MINSTRET, 0)
    finish(a, [1, 5])
    tb = await runp(dut, a, regs=[1, 5])
    delta = tb.values[1] - tb.values[0]
    assert delta == 4, ("REQ-33: MINSTRET delta %d, expected 4 "
                        "(3 instructions + the first read; the reading instruction "
                        "itself must not be counted)" % delta)


@cocotb.test()
async def test_req33_counter_deltas(dut):
    """REQ-33 + section 8: MINSTRET delta over a known block; MCYCLE monotonic."""
    a = Asm()
    a.csrrs(1, MINSTRET, 0)
    a.li(9, 0x8000)              # 2 instrs (lui+addi)
    a.lw(10, 9, 0)               # 3
    a.addi(11, 11, 1)            # 4..7
    a.addi(11, 11, 1)
    a.addi(11, 11, 1)
    a.addi(11, 11, 1)
    a.mul(12, 10, 10)            # 8
    a.mulhu(13, 10, 10)          # 9
    a.sltu(14, 10, 13)           # 10
    a.xor(15, 14, 12)            # 11
    a.sub(16, 16, 11)            # 12
    a.csrrs(2, MINSTRET, 0)
    a.csrrs(3, MCYCLEH, 0)
    a.csrrs(4, MINSTRETH, 0)
    a.csrrs(5, MCYCLE, 0)
    a.csrrs(6, MCYCLE, 0)
    finish(a, [1, 2, 3, 4, 5, 6])
    tb = await runp(dut, a, regs=[1, 2, 3, 4, 5, 6], data={0x8000: [0x9E3779B9]})
    delta = tb.values[1] - tb.values[0]
    assert delta == 13, ("REQ-33: MINSTRET delta %d, expected 13 "
                         "(the first read + a 12-instruction block; reads do not "
                         "count themselves)" % delta)
    assert tb.values[2] == 0 and tb.values[3] == 0, [hex(v) for v in tb.values]
    assert tb.values[5] > tb.values[4] > 0, (tb.values[4], tb.values[5])


@cocotb.test()
async def test_req33_minstret_across_trap(dut):
    """REQ-33: a trapping instruction is not counted; handler instrs are."""
    a = Asm()
    hdl_install(a)
    hdl_precise(a, rc=24, rt=25, re=23)     # handler body: 7 instructions
    a.csrrs(20, MINSTRET, 0)
    a.ecall()
    a.csrrs(21, MINSTRET, 0)
    finish(a, [20, 21])
    tb = await runp(dut, a, regs=[20, 21])
    delta = tb.values[1] - tb.values[0]
    assert delta == 8, ("REQ-33: MINSTRET delta %d, expected 8 "
                        "(first read + 7 handler instructions; the ECALL traps and "
                        "must not be counted)" % delta)


# --------------------------------------------------------------------- traps

@cocotb.test()
async def test_req34_illegal_encodings(dut):
    """REQ-34: undefined encodings all raise illegal instruction (cause 2)."""
    a = Asm()
    hdl_install(a)
    hdl_pack(a)
    bad = [0xFFFFFFFF, 0x00100072, _r(0b0000011, 2, 3, 0, 4, 0x33),
           _r(0b0010000, 2, 3, 0b000, 4, 0x33), _r(0b0000101, 2, 3, 0b001, 4, 0x33),
           0x00000473, _i(0, 1, 0b011, 2, 0x03)]
    for w in bad:
        a.word(w)
    finish(a, [20])
    tb = await runp(dut, a, regs=[20])
    assert tb.values[0] == 0x02222222, hex(tb.values[0])


@cocotb.test()
async def test_req34_35_mtval_no_other_effect(dut):
    """REQ-34/35: illegal instr -> mtval=instruction word, mepc=PC, no other effect."""
    a = Asm()
    hdl_install(a)
    hdl_precise(a, rc=20, rt=21, re=22)
    a.li(9, 7)
    pc = a.here()
    bad = _r(0b0000011, 2, 3, 0, 9, 0x33)
    a.word(bad)
    finish(a, [20, 21, 22, 9])
    tb = await runp(dut, a, regs=[20, 21, 22, 9])
    assert tb.values[0] == 2, hex(tb.values[0])
    assert tb.values[1] == bad, (hex(tb.values[1]), hex(bad))
    assert tb.values[2] == pc, (hex(tb.values[2]), hex(pc))
    assert tb.values[3] == 7, tb.values[3]


@cocotb.test()
async def test_req35_36_40_ecall_mret(dut):
    """REQ-35/36/40: ECALL -> cause 11, mtval 0, mepc=PC, MPIE<-MIE; MRET at MEPC."""
    a = Asm()
    hdl_install(a)
    hdl_precise(a, rc=20, rt=21, re=22, rs=23)
    irq_enable(a, 0)
    pc = a.here()
    a.ecall()
    a.addi(2, 2, 1)
    a.csrrs(3, MSTATUS, 0)
    finish(a, [20, 21, 22, 23, 2, 3])
    tb = await runp(dut, a, regs=[20, 21, 22, 23, 2, 3])
    assert tb.values[0] == 11, hex(tb.values[0])
    assert tb.values[1] == 0, hex(tb.values[1])
    assert tb.values[2] == pc, (hex(tb.values[2]), hex(pc))
    assert tb.values[3] == 0x1880, hex(tb.values[3])
    assert tb.values[4] == 1, tb.values[4]
    assert tb.values[5] == 0x1888, hex(tb.values[5])


@cocotb.test()
async def test_req40_mret_fetch_at_mepc(dut):
    """REQ-40: MRET continues fetch at MEPC."""
    a = Asm()
    hdl_install(a)
    hdl_precise(a, rc=20, rt=21, re=22, ret="land")
    a.ecall()
    a.addi(2, 2, 1)
    a.jal(28, "end")
    a.label("land")
    a.addi(3, 3, 9)
    a.label("end")
    finish(a, [2, 3])
    tb = await runp(dut, a, regs=[2, 3])
    assert tb.values == [0, 9], tb.values


@cocotb.test()
async def test_req36_ebreak(dut):
    """REQ-36: EBREAK -> cause 3, mtval = address of the EBREAK."""
    a = Asm()
    hdl_install(a)
    hdl_precise(a, rc=20, rt=21, re=22)
    a.nop()
    pc = a.here()
    a.ebreak()
    finish(a, [20, 21, 22])
    tb = await runp(dut, a, regs=[20, 21, 22])
    assert tb.values[0] == 3, hex(tb.values[0])
    assert tb.values[1] == pc, (hex(tb.values[1]), hex(pc))
    assert tb.values[2] == pc, (hex(tb.values[2]), hex(pc))


@cocotb.test()
async def test_dev_d2_mret_sets_mpie(dut):
    """REQ-41 / D2: MRET copies MPIE to MIE, then sets MPIE to 1."""
    a = Asm()
    hdl_install(a)
    hdl_precise(a, rc=24, rt=25, re=23)
    a.ecall()
    a.csrrs(2, MSTATUS, 0)
    finish(a, [2])
    tb = await runp(dut, a, regs=[2])
    assert tb.values[0] == 0x1880, ("REQ-41: mstatus after MRET is %08x, expected 00001880 "
                                    "(MPP=3, MPIE<-1, MIE<-MPIE=0)" % tb.values[0])


# ---------------------------------------------------------------- interrupts

@cocotb.test()
async def test_req37_irq_external_priority(dut):
    """REQ-37/35/36: external interrupt has priority; mepc = next unrun instr."""
    a = Asm()
    hdl_install(a)
    hdl_irq(a, "end")
    irq_enable(a, 0x880)
    a.set_irq(3)
    lo = a.here()
    for _ in range(6):
        a.nop()
    hi = a.here() - 4
    a.label("end")
    finish(a, [20, 21])
    tb = await runp(dut, a, regs=[20, 21])
    assert tb.values[0] == 0x8000000B, hex(tb.values[0])
    assert lo <= tb.values[1] <= hi and tb.values[1] % 4 == 0, \
        (hex(tb.values[1]), hex(lo), hex(hi))


@cocotb.test()
async def test_req37_irq_timer(dut):
    """REQ-37/36: timer interrupt -> cause 0x80000007, mtval 0."""
    a = Asm()
    hdl_install(a)
    hdl_irq(a, "end", rc=20, re=21)
    irq_enable(a, 0x080)
    a.set_irq(3)
    for _ in range(4):
        a.nop()
    a.label("end")
    a.csrrs(22, MTVAL, 0)
    finish(a, [20, 22])
    tb = await runp(dut, a, regs=[20, 22])
    assert tb.values[0] == 0x80000007, hex(tb.values[0])
    assert tb.values[1] == 0, hex(tb.values[1])


@cocotb.test()
async def test_req38_masked_line_not_recorded(dut):
    """REQ-38: a line asserted only while masked must not be recorded."""
    a = Asm()
    hdl_install(a)
    hdl_irq(a, "end")
    a.set_irq(3)
    for _ in range(10):
        a.nop()
    a.set_irq(0)
    irq_enable(a, 0x880)
    for _ in range(10):
        a.nop()
    a.label("end")
    finish(a, [20])
    tb = await runp(dut, a, regs=[20])
    assert tb.values[0] == 0, ("REQ-38: interrupt taken after the line was removed "
                               "(mcause=%08x)" % tb.values[0])


@cocotb.test()
async def test_req39_interrupt_taken_bounded(dut):
    """REQ-39: an interrupt is taken within a bounded time while instrs run."""
    a = Asm()
    hdl_install(a)
    hdl_irq(a, "end")
    irq_enable(a, 0x080)
    a.li(29, 0xFFFFFFFF)
    a.set_irq(2)
    a.label("loop")
    a.addi(2, 2, 1)
    a.bne(2, 29, "loop")
    a.label("end")
    finish(a, [20, 2])
    tb = await runp(dut, a, regs=[20, 2])
    assert tb.values[0] == 0x80000007, hex(tb.values[0])
    assert tb.cycles < 500, "REQ-39: interrupt not taken promptly (%d cycles)" % tb.cycles
    assert tb.values[1] <= 8, ("REQ-39: %d loop iterations before the interrupt "
                               "was taken" % tb.values[1])


# ---------------------------------------------------- change request (REQ-20)

def _shxadd(a, rd, rs1, rs2, f3):
    a.word(_r(0b0010000, rs2, rs1, f3, rd, 0x33))


def _minmax(a, rd, rs1, rs2, f3):
    a.word(_r(0b0000101, rs2, rs1, f3, rd, 0x33))


@cocotb.test()
async def test_cr1_shadd(dut):
    """REQ-20: SH1ADD/SH2ADD/SH3ADD acceptance + forwarding of their results."""
    a = Asm()
    a.li(1, 0x80000003)
    a.li(2, 100)
    _shxadd(a, 3, 1, 2, 0b010)
    _shxadd(a, 4, 1, 2, 0b100)
    _shxadd(a, 5, 1, 2, 0b110)
    a.addi(6, 5, 1)
    _shxadd(a, 7, 6, 1, 0b010)
    finish(a, [3, 4, 5, 6, 7])
    tb = await runp(dut, a, regs=[3, 4, 5, 6, 7])
    assert tb.values == [106, 112, 124, 125, ((125 << 1) + 0x80000003) & M32], \
        [hex(v) for v in tb.values]


@cocotb.test()
async def test_cr2_minmax(dut):
    """REQ-20: MIN/MINU/MAX/MAXU acceptance values and signed/unsigned corners."""
    a = Asm()
    a.li(1, 0xFFFFFFFF)
    a.li(2, 1)
    _minmax(a, 3, 1, 2, 0b100)
    _minmax(a, 4, 1, 2, 0b101)
    _minmax(a, 5, 1, 2, 0b110)
    _minmax(a, 6, 1, 2, 0b111)
    a.li(7, 0x7FFFFFFF)
    a.li(8, 0xFFFFFFFF)
    _minmax(a, 9, 7, 8, 0b100)
    _minmax(a, 10, 7, 8, 0b101)
    _minmax(a, 11, 7, 8, 0b110)
    _minmax(a, 12, 7, 8, 0b111)
    _minmax(a, 13, 2, 2, 0b100)
    _minmax(a, 14, 2, 2, 0b111)
    finish(a, [3, 4, 5, 6, 9, 10, 11, 12, 13, 14])
    tb = await runp(dut, a, regs=[3, 4, 5, 6, 9, 10, 11, 12, 13, 14])
    assert tb.values == [0xFFFFFFFF, 1, 1, 0xFFFFFFFF, 0xFFFFFFFF, 0x7FFFFFFF,
                         0x7FFFFFFF, 0xFFFFFFFF, 1, 1], [hex(v) for v in tb.values]


@cocotb.test()
async def test_cr3_reserved_f3_illegal(dut):
    """CR-D3 pt 3: reserved funct3 with the new funct7 values stays illegal (2)."""
    a = Asm()
    hdl_install(a)
    hdl_pack(a)
    a.word(_r(0b0010000, 2, 1, 0b000, 3, 0x33))
    a.word(_r(0b0010000, 2, 1, 0b001, 3, 0x33))
    a.word(_r(0b0010000, 2, 1, 0b011, 3, 0x33))
    a.word(_r(0b0010000, 2, 1, 0b101, 3, 0x33))
    a.word(_r(0b0010000, 2, 1, 0b111, 3, 0x33))
    a.word(_r(0b0000101, 2, 1, 0b000, 3, 0x33))
    a.word(_r(0b0000101, 2, 1, 0b010, 3, 0x33))
    a.word(_r(0b0000101, 2, 1, 0b011, 3, 0x33))
    finish(a, [20, 21])
    tb = await runp(dut, a, regs=[20, 21])
    assert tb.values[0] == 0x22222222, hex(tb.values[0])
    assert tb.values[1] == 0x33333333, hex(tb.values[1])


CR_OPS = [("sh1add", 0b0010000, 0b010), ("sh2add", 0b0010000, 0b100),
          ("sh3add", 0b0010000, 0b110), ("min", 0b0000101, 0b100),
          ("minu", 0b0000101, 0b101), ("max", 0b0000101, 0b110),
          ("maxu", 0b0000101, 0b111)]

CR_CONST_A = 0x80000003
CR_CONST_B = 100
CR_CONST_RS2 = 0xFFFF0FF0


def cr_eval(f7, f3, ra, rb):
    if f7 == 0b0010000:
        k = {0b010: 1, 0b100: 2, 0b110: 3}[f3]
        return ((ra << k) + rb) & M32
    # spec: MIN/MAX signed, MINU/MAXU unsigned; results are the operand values
    if f3 == 0b100:
        return ra if s32(ra) < s32(rb) else rb
    if f3 == 0b101:
        return ra if ra < rb else rb
    if f3 == 0b110:
        return ra if s32(ra) > s32(rb) else rb
    return ra if ra > rb else rb


@cocotb.test()
async def test_cr4_forwarding_matrix(dut):
    """REQ-20/12 + x0 (REQ-11): every CR-D3 op as consumer, with the value
    producer (ALU addi / load / mul / CR) at 0, 1 or 2 instructions distance;
    rd=x0 producers must not forward (x0-guard); results are folded into one
    xor accumulator so any wrong forwarding shows up as a mismatch."""
    prod_imm = lambda ci: ((ci * 7919) % 2048) - 1024
    lw_word = lambda ci: (0x5EED0000 + ci * 2654435761) & M32

    a = Asm()
    a.li(20, CR_CONST_A)
    a.li(22, CR_CONST_B)
    a.li(7, CR_CONST_RS2)
    a.li(21, 0x9000)
    a.addi(9, 0, 0)
    a.addi(10, 0, 0)
    a.addi(25, 0, 0)

    lw_ci = 0
    expected = 0
    ci = 0
    for _, f7, f3 in [(None, f7, f3) for _, f7, f3 in CR_OPS]:
        for prod in ("addi", "lw", "mul", "cr"):
            for gap in (0, 1, 2):
                if prod == "addi":
                    a.addi(6, 20, prod_imm(ci))
                    pv = (CR_CONST_A + prod_imm(ci)) & M32
                elif prod == "lw":
                    a.lw(6, 21, 4 * lw_ci)
                    pv = lw_word(lw_ci)
                    lw_ci += 1
                elif prod == "mul":
                    a.mul(6, 20, 22)
                    pv = (CR_CONST_A * CR_CONST_B) & M32
                else:
                    _shxadd(a, 6, 20, 22, 0b010)
                    pv = ((CR_CONST_A << 1) + CR_CONST_B) & M32
                for _ in range(gap):
                    a.addi(25, 25, 1)
                a.word(_r(f7, 7, 6, f3, 8, 0x33))
                a.xor(9, 9, 8)
                expected ^= cr_eval(f7, f3, pv, CR_CONST_RS2)
                ci += 1

    # x0-guard: producers with rd=x0 must never forward a value into x0 reads.
    for prod_kind in ("cr", "addi", "lw", "mul", "jal", "csr"):
        if prod_kind == "cr":
            _shxadd(a, 0, 20, 22, 0b110)
        elif prod_kind == "addi":
            a.addi(0, 20, -5)
        elif prod_kind == "lw":
            a.lw(0, 21, 4 * lw_ci)
            lw_ci += 1
        elif prod_kind == "mul":
            a.mul(0, 20, 22)
        elif prod_kind == "jal":
            a.jal(0, "x0tgt")
            a.label("x0tgt")
        else:
            a.csrrw(0, MSCRATCH, 20)
        a.addi(8, 0, 0)
        a.xor(9, 9, 8)
        a.sltu(13, 0, 8)
        a.add(10, 10, 13)
    a.lw(0, 21, 4 * lw_ci)
    lw_ci += 1
    for _ in range(2):
        a.addi(25, 25, 1)
    a.addi(8, 0, 0)
    a.xor(9, 9, 8)
    a.sltu(13, 0, 8)
    a.add(10, 10, 13)

    finish(a, [9, 10])
    tb = await runp(dut, a, regs=[9, 10],
                    data={0x9000: [lw_word(i) for i in range(lw_ci)]})
    assert tb.values[0] == expected, ("CR-D3 forwarding matrix: xor accumulator "
                                      "%08x != expected %08x" % (tb.values[0], expected))
    assert tb.values[1] == 0, ("REQ-11: x0 read after rd=x0 producer was "
                               "nonzero %d times" % tb.values[1])


@cocotb.test()
async def test_cr5_rd_rs_aliasing(dut):
    """CR-D3 acceptance + playbook corners: rd==rs aliasing, signed/unsigned
    extremes, shift wrap, and CR results feeding the next CR op."""
    a = Asm()
    a.li(6, 3)
    a.slli(6, 6, 1)                 # 6
    _shxadd(a, 6, 6, 6, 0b010)       # (6<<1)+6 = 18
    _shxadd(a, 6, 6, 6, 0b100)       # (18<<2)+18 = 90
    _shxadd(a, 6, 6, 6, 0b110)       # (90<<3)+90 = 810
    _minmax(a, 6, 6, 6, 0b100)       # min(x,x) = x
    _minmax(a, 6, 6, 6, 0b111)       # maxu(x,x) = x
    a.li(9, 0x7FFFFFFF)
    a.li(10, 0x80000000)
    _minmax(a, 11, 9, 10, 0b100)     # MIN signed
    _minmax(a, 12, 9, 10, 0b110)     # MAX signed
    _minmax(a, 13, 9, 10, 0b101)     # MINU unsigned
    _minmax(a, 14, 9, 10, 0b111)     # MAXU unsigned
    a.li(15, 0x20000000)
    _shxadd(a, 16, 15, 15, 0b110)    # (0x20000000<<3)+0x20000000 wraps mod 2^32
    a.li(17, 0xFFFFFFF6)             # -10 signed
    _minmax(a, 18, 17, 9, 0b100)
    _minmax(a, 19, 17, 9, 0b111)
    finish(a, [6, 11, 12, 13, 14, 16, 18, 19])
    tb = await runp(dut, a, regs=[6, 11, 12, 13, 14, 16, 18, 19])
    exp = [810, 0x80000000, 0x7FFFFFFF, 0x7FFFFFFF, 0x80000000,
           0x20000000, 0xFFFFFFF6, 0xFFFFFFF6]
    assert tb.values == exp, [[hex(v) for v in tb.values], [hex(e) for e in exp]]


# ------------------------------------------------ mutation-kill (Phase 4c seeds)

async def _illegal_load(dut, f3):
    a = Asm()
    hdl_install(a)
    hdl_precise(a, rc=20, rt=21, re=22)
    a.label("main")
    bad = _i(0, 0, f3, 1, 0x03)
    pc = a.here()
    a.word(bad)
    a.label("end")
    finish(a, [20, 21, 22, 1])
    tb = await runp(dut, a, regs=[20, 21, 22, 1])
    assert tb.values[0] == 2, "mcause=%08x (want illegal-instruction 2)" % tb.values[0]
    assert tb.values[1] == bad, (hex(tb.values[1]), hex(bad))
    assert tb.values[2] == pc, (hex(tb.values[2]), hex(pc))


@cocotb.test()
async def test_req23_illegal_load_f3(dut):
    """REQ-23/24: load funct3 in {011,110,111} are not RV32IM-legal ->
    illegal-instruction trap with mtval = instruction (kills decode drop_terms
    of each illegal-funct3 term)."""
    for f3 in (3, 6, 7):
        await _illegal_load(dut, f3)


@cocotb.test()
async def test_req33_mcycleh_pure_read_no_write(dut):
    """REQ-33: a CSR read with rs1=x0 / uimm=0 must not write; MCYCLEH write
    value survives repeated pure reads (kills csr drop_term of do_write)."""
    a = Asm()
    a.li(1, 0x4000)
    a.csrrw(0, MCYCLEH, 1)          # MCYCLEH = 0x4000 (real write)
    a.csrrs(5, MCYCLEH, 0)          # pure read, rs1=x0
    a.csrrsi(6, MCYCLEH, 0)         # pure read, uimm=0
    a.csrrs(7, MCYCLEH, 0)
    a.label("end")
    finish(a, [5, 6, 7])
    tb = await runp(dut, a, regs=[5, 6, 7])
    assert all(v in (0x4000, 0x4001) for v in tb.values), \
        [hex(v) for v in tb.values]


@cocotb.test()
async def test_req15_jalr_nonzero_imm(dut):
    """REQ-15: JALR target = (rs1 + sign-extended imm) & ~1, both signs of imm
    (kills core +imm -> -imm on the JALR target adder)."""
    a = Asm()
    a.label("main")
    a.la(5, "seqA")
    j1 = a.here()
    a.jalr(6, 5, 8)                 # fixed: lands at seqA+8
    a.label("seqA")                 # mutant lands 16 earlier (in main: hang/garbage)
    a.addi(1, 0, 31)
    a.addi(1, 0, 32)
    a.addi(1, 0, 33)                # fixed landing point
    a.addi(4, 0, 44)                # fixed fall-through marker
    a.la(5, "midB")
    j2 = a.here()
    a.jalr(7, 5, -8)                # fixed: lands at labelB
    a.label("labelB")
    a.addi(2, 0, 21)
    a.addi(2, 0, 22)
    a.label("midB")
    a.addi(3, 0, 23)                # fixed passes; mutant (lands at midB+8) skips all
    a.label("end")
    finish(a, [1, 2, 3, 4, 6, 7])
    tb = await runp(dut, a, regs=[1, 2, 3, 4, 6, 7])
    assert tb.values[0] == 33, tb.values[0]
    assert tb.values[1] == 22 and tb.values[2] == 23, tb.values[1:3]
    assert tb.values[3] == 44, tb.values[3]
    assert tb.values[4] == j1 + 4 and tb.values[5] == j2 + 4, tb.values[4:]


@cocotb.test()
async def test_req37_irq_ext_only_mie(dut):
    """REQ-36/37: with MEIE=1, MTIE=0 and both lines asserted, the external
    interrupt must still be taken (kills csr drop_term of meie&&irq_ext)."""
    a = Asm()
    hdl_install(a)
    hdl_irq(a, "end")
    irq_enable(a, 0x800)            # MEIE only
    a.set_irq(3)                    # both lines
    for _ in range(6):
        a.nop()
    a.label("end")
    finish(a, [20])
    tb = await runp(dut, a, regs=[20])
    assert tb.values[0] == 0x8000000B, hex(tb.values[0])


@cocotb.test()
async def test_req13_load_use_dependent_load(dut):
    """REQ-13: a load whose BASE register is produced by the immediately prior
    load must be interlocked (kills decode uses_rs1=0 on OP_LOAD)."""
    a = Asm()
    a.li(5, 0x8000)
    a.lw(1, 5, 0)                # x1 = mem[0x8000] = 0x8004
    a.lw(3, 1, 0)                # x3 = mem[x1] = mem[0x8004] = 0xBEEF
    a.label("end")
    finish(a, [3])
    tb = await runp(dut, a, regs=[3], data={0x8000: [0x8004, 0xBEEF]},
                    dstall=0.7)
    assert tb.values[0] == 0xBEEF, hex(tb.values[0])


@cocotb.test()
async def test_req33_mcycle_write_readback(dut):
    """REQ-33 / section 8: MCYCLE is writable; read-back reflects the write
    (kills csr MCYCLE compare off_by_one to MCYCLE+1)."""
    a = Asm()
    a.li(1, 0xFFFFFFF0)
    a.csrrw(0, MCYCLE, 1)
    a.csrrs(2, MCYCLE, 0)
    a.label("end")
    finish(a, [2])
    tb = await runp(dut, a, regs=[2])
    assert tb.values[0] >= 0xFFFFFFF0, hex(tb.values[0])


@cocotb.test()
async def test_req33_noncsr_instr_csr_addr_no_write(dut):
    """REQ-33: a retiring NON-CSR instruction whose immediate bits encode a CSR
    address must not touch that CSR (kills csr do_write drop_term of op!=NONE).
    lui x0,0xB02 encodes MINSTRET in [31:20]; its retire must still increment."""
    a = Asm()
    a.csrrs(1, MINSTRET, 0)
    a.word(0xB0280037)           # lui x0, 0xB02 with rs1-field=x1 (unmasked)
    a.addi(2, 0, 1)
    a.csrrs(3, MINSTRET, 0)
    a.label("end")
    finish(a, [1, 3])
    tb = await runp(dut, a, regs=[1, 3])
    delta = tb.values[1] - tb.values[0]
    assert delta == 3, ("MINSTRET delta %d, expected 3 (lui x0 + addi + first "
                        "read; mutant loses the lui retire)" % delta)


@cocotb.test()
async def test_req36_csr_reset_values(dut):
    """REQ-01/36: trap/interrupt CSRs read their spec reset values before any
    trap or write (kills csr reset-constant and mie_rd padding mutations)."""
    a = Asm()
    a.csrrs(1, MTVEC, 0)
    a.csrrs(2, MEPC, 0)
    a.csrrs(3, MCAUSE, 0)
    a.csrrs(4, MSCRATCH, 0)
    a.csrrs(5, MTVAL, 0)
    a.csrrs(6, MIE, 0)
    a.csrrs(7, MIP, 0)
    a.csrrs(8, MSTATUS, 0)
    a.label("end")
    finish(a, [1, 2, 3, 4, 5, 6, 7, 8])
    tb = await runp(dut, a, regs=[1, 2, 3, 4, 5, 6, 7, 8])
    assert tb.values[:7] == [0] * 7, [hex(v) for v in tb.values]
    assert tb.values[7] == 0x1800, hex(tb.values[7])   # MPP=3, MPIE=0, MIE=0


@cocotb.test()
async def test_req13_load_then_dependent_store(dut):
    """REQ-13: a store whose DATA register is produced by the immediately prior
    load must be interlocked (kills decode uses_rs2=0 on OP_STORE)."""
    a = Asm()
    a.li(5, 0x8000)
    a.lw(1, 5, 0)                # x1 = 0xABCD
    a.sw(1, 5, 8)                # mem[0x8008] = x1
    a.lw(2, 5, 8)                # readback
    a.label("end")
    finish(a, [2])
    tb = await runp(dut, a, regs=[2], data={0x8000: [0xABCD, 0, 0, 0]},
                    dstall=0.7)
    assert tb.values[0] == 0xABCD, hex(tb.values[0])


@cocotb.test()
async def test_req33_minstret_write_readback(dut):
    """REQ-33: MINSTRET is writable; read-back reflects the write
    (kills csr MINSTRET compare off_by_one to MINSTRET+1)."""
    a = Asm()
    a.li(1, 0xFFFFFF00)
    a.csrrw(0, MINSTRET, 1)
    a.csrrs(2, MINSTRET, 0)
    a.label("end")
    finish(a, [2])
    tb = await runp(dut, a, regs=[2])
    assert tb.values[0] >= 0xFFFFFF00, hex(tb.values[0])


@cocotb.test()
async def test_req33_minstreth_pure_read_no_write(dut):
    """REQ-33: a pure read of MINSTRETH must not suppress that instruction's
    retire count (kills csr drop_term of do_write on the MINSTRETH arm)."""
    a = Asm()
    a.csrrs(1, MINSTRET, 0)
    a.addi(2, 0, 1)
    a.csrrsi(3, MINSTRETH, 0)    # pure read (uimm=0)
    a.csrrs(4, MINSTRET, 0)
    a.label("end")
    finish(a, [1, 4])
    tb = await runp(dut, a, regs=[1, 4])
    delta = tb.values[1] - tb.values[0]
    assert delta == 3, "MINSTRET delta %d, expected 3" % delta


@cocotb.test()
async def test_req39_illegal_traps_promptly(dut):
    """REQ-39-class promptness (own design invariant, deterministic no-wait run):
    an illegal instruction traps within 20 mcycle ticks of the preceding CSR
    read (kills decode is_mdu=1 on the illegal-mask arm: muldiv would add 34+
    cycles of EX stall before the trap)."""
    a = Asm()
    hdl_install(a)
    a.label("hdl")
    a.csrrs(20, MCYCLE, 0)
    a.csrrs(27, MEPC, 0)
    a.addi(27, 27, 4)
    a.csrrw(27, MEPC, 27)
    a.mret()
    a.label("main")
    a.csrrs(1, MCYCLE, 0)
    a.word(_i(0, 0, 3, 1, 0x03))     # illegal load f3=011
    a.label("end")
    finish(a, [20, 1])
    tb = await runp(dut, a, regs=[20, 1])
    delta = tb.values[0] - tb.values[1]
    assert 0 < delta < 20, ("mcycle delta across illegal-instr trap = %d" % delta)


@cocotb.test()
async def test_req37_irq_cause_timer_when_ext_idle(dut):
    """REQ-37: with MEIE=1 but the ext line LOW and only the timer line high,
    mcause must be timer (kills irq_cause drop_term of irq_ext in the mux)."""
    a = Asm()
    hdl_install(a)
    hdl_irq(a, "end")
    irq_enable(a, 0x880)          # both enables
    a.set_irq(2)                  # timer line only
    for _ in range(6):
        a.nop()
    a.label("end")
    finish(a, [20])
    tb = await runp(dut, a, regs=[20])
    assert tb.values[0] == 0x80000007, hex(tb.values[0])


@cocotb.test()
async def test_req31_csrrc_forms_write(dut):
    """REQ-31: CSRRCC/CSRRCI with unmasked source must actually clear bits
    (kills csr op-compare that drops CSR_RC writes)."""
    a = Asm()
    a.li(1, 8)                    # MSTATUS.MIE bit
    a.csrrs(0, MSTATUS, 1)        # set MIE (RS, unmasked: rs1=x1)
    a.csrrs(2, MSTATUS, 0)        # pure read: expect MIE=1
    a.csrrc(3, MSTATUS, 1)        # clear via rs1 form
    a.csrrs(4, MSTATUS, 0)        # expect MIE=0
    a.csrrs(0, MSTATUS, 1)        # set again
    a.csrrci(5, MSTATUS, 8)       # clear via uimm form
    a.csrrs(6, MSTATUS, 0)        # expect MIE=0
    a.label("end")
    finish(a, [2, 3, 4, 5, 6])
    tb = await runp(dut, a, regs=[2, 3, 4, 5, 6])
    assert tb.values[0] & 8, hex(tb.values[0])
    assert not (tb.values[2] & 8), hex(tb.values[2])
    assert not (tb.values[4] & 8), hex(tb.values[4])


@cocotb.test()
async def test_req30_flushed_csr_instr_no_write(dut):
    """REQ-30/35: a CSR write instruction squashed by an interrupt must have
    NO side effect (kills csr commit drop_term of ex_fire: commit fires while
    the CSR instr sits stalled in EX, writing MCYCLE=x1 before the trap)."""
    a = Asm()
    hdl_install(a)
    a.label("hdl")
    a.csrrs(20, MCYCLE, 0)        # fixed: mcycle here is small (<200)
    a.li(31, DEVICE_IRQ)
    a.addi(26, 0, 0)
    a.sw(26, 31, 0)
    a.li(27, 0x88)
    a.csrrc(26, MSTATUS, 27)
    a.csrrs(27, MEPC, 0)
    a.addi(27, 27, 4)
    a.csrrw(27, MEPC, 27)
    a.mret()
    a.label("main")
    irq_enable(a, 0x080)
    a.set_irq(2)                  # timer line stays high
    a.li(5, 0x8000)
    a.lw(1, 5, 0)                 # x1 = 12345; MEM waits several cycles
    a.csrrw(0, MCYCLE, 1)         # stalls in EX behind it; then trapped
    a.label("end")
    finish(a, [20])
    tb = await runp(dut, a, regs=[20], data={0x8000: [12345]}, dstall=0.9)
    assert tb.values[20 - 20] < 200, "MCYCLE=%d: squashed CSR wrote anyway" % tb.values[0]


@cocotb.test()
async def test_req13_no_stall_on_unused_rs1_field(dut):
    """REQ-13: the load-use interlock must not fire for an ID instruction that
    carries a colliding rs1 FIELD but does not use rs1 (LUI). Pins the exact
    mcycle delta of a no-wait run (kills hazard drop_term of id_uses_rs1:
    an unwanted +1 stall bubble changes the delta)."""
    a = Asm()
    a.csrrs(1, MCYCLE, 0)
    a.li(5, 0x8000)
    a.lw(10, 5, 0)                        # producer rd=x10
    a.word((0x50150 << 12) | (9 << 7) | 0x37)   # lui x9: rs1-field bits[19:15]=0xA
    a.csrrs(2, MCYCLE, 0)
    a.label("end")
    finish(a, [1, 2])
    tb = await runp(dut, a, regs=[1, 2], data={0x8000: [7]})
    delta = tb.values[1] - tb.values[0]
    assert delta == 5, ("mcycle delta %d, expected 5 (no interlock bubble)" % delta)


@cocotb.test()
async def test_req35_trap_cycle_csr_write_suppressed(dut):
    """REQ-30/35: an instruction trapped AT its fire cycle must not commit its
    CSR write. The interrupt line rises right after the prior store retires,
    so the trap lands exactly on the CSRRW MCYCLE; the handler must then see
    MCYCLE untouched (kills csr commit drop_term: ex_fire||!trap commits the
    write on the trap cycle)."""
    a = Asm()
    hdl_install(a)
    a.label("hdl")
    a.csrrs(20, MCYCLE, 0)        # must be small (no 12345 write)
    a.csrrs(21, MEPC, 0)          # must equal the CSRRW pc
    a.li(31, DEVICE_IRQ)
    a.addi(26, 0, 0)
    a.sw(26, 31, 0)               # clear lines
    a.li(27, 0x88)
    a.csrrc(26, MSTATUS, 27)
    a.csrrs(27, MEPC, 0)
    a.addi(27, 27, 4)
    a.csrrw(27, MEPC, 27)
    a.mret()
    a.label("main")
    irq_enable(a, 0x080)
    a.li(1, 12345)
    a.set_irq(2)                  # line rises when this store retires
    pc_csrw = a.here()            # next instruction = the trapped CSRRW
    a.csrrw(0, MCYCLE, 1)         # fixed: trapped, no write; mutant: writes 12345
    a.label("end")
    finish(a, [20, 21])
    tb = await runp(dut, a, regs=[20, 21])
    assert tb.values[1] == pc_csrw, ("mepc=%08x expected %08x (trap not on CSRRW?)"
                                     % (tb.values[1], pc_csrw))
    assert tb.values[0] < 200, "MCYCLE=%d: trapped CSRRW still wrote" % tb.values[0]


@cocotb.test()
async def test_req01_counters_reset_zero(dut):
    """REQ-01/33: MCYCLE/MINSTRET reset to 0. First MINSTRET read (before any
    retire) must be exactly 0; first MCYCLE read is pinned to its deterministic
    value in a no-wait run (kills counter reset-constant mutations)."""
    a = Asm()
    a.csrrs(1, MINSTRET, 0)
    a.csrrs(2, MCYCLE, 0)
    a.label("end")
    finish(a, [1, 2])
    tb = await runp(dut, a, regs=[1, 2])
    print("FIRST_MINSTRET=%d FIRST_MCYCLE=%d" % (tb.values[0], tb.values[1]))
    assert tb.values[0] == 0, "MINSTRET first read = %d (reset not 0?)" % tb.values[0]
    assert tb.values[1] == 4, "MCYCLE first read = %d (deterministic no-wait run)" % tb.values[1]


@cocotb.test()
async def test_req33_minstreth_write_readback(dut):
    """REQ-33: MINSTRETH is writable; pure re-reads keep the value
    (kills csr MINSTRETH compare off_by_one)."""
    a = Asm()
    a.li(1, 0x4000)
    a.csrrw(0, MINSTRETH, 1)
    a.csrrs(5, MINSTRETH, 0)
    a.csrrsi(6, MINSTRETH, 0)
    a.csrrs(7, MINSTRETH, 0)
    a.label("end")
    finish(a, [5, 6, 7])
    tb = await runp(dut, a, regs=[5, 6, 7])
    assert all(v in (0x4000, 0x4001) for v in tb.values), [hex(v) for v in tb.values]
