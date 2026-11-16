"""Probe-per-REQ sweep for rv_core (Phase 1c completeness gate).

One short directed test per spec requirement (REQ-01..41) plus dedicated
deviation tests test_dev_d1..d4 and the change-request acceptance tests
test_cr1..3 (REQ-20). Expectations are derived from spec.md, never from RTL.

REQ-05 and REQ-42 are structural (no combinational dependence on ready/rdata;
synchronous/latch-free/sdc) -> not observable at ports by running programs;
they are covered by the flow's lint/synth stages, so no probe here.
"""

import cocotb

from rv_asm import (DEVICE_IRQ, MCAUSE, MIE, MISA, MSTATUS, MEPC, MSCRATCH,
                    MINSTRET, MTVEC, MTVAL, Asm, _b, _i, _j, _r)
from rv_tb import RvTB

M32 = 0xFFFFFFFF
TOHOST = 0xFFFF0000


def s32(v):
    v &= M32
    return v - (1 << 32) if v >> 31 else v


async def runp(dut, a, regs=(), data=None, timeout=12000, **kw):
    """Assemble, load, run; check REQ-03/04 hold (in RvTB), REQ-08 alignment and
    REQ-07 (load => wstrb 0) while stepping. Returns the RvTB."""
    tb = RvTB(dut, **kw)
    tb.load(a.base, a.build())
    for base, words in (data or {}).items():
        tb.load(base, words)
    await tb.start()
    d = dut
    while not tb.done:
        await tb.step()
        assert tb.cycles < timeout, "program did not finish in %d cycles" % timeout
        if int(d.imem_valid.value):
            assert int(d.imem_addr.value) & 3 == 0, "REQ-08: imem_addr not word aligned"
        if int(d.dmem_valid.value):
            assert int(d.dmem_addr.value) & 3 == 0, "REQ-08: dmem_addr not word aligned"
            if not int(d.dmem_write.value):
                assert int(d.dmem_wstrb.value) == 0, "REQ-07: load with dmem_wstrb != 0"
    assert not tb.violations, tb.violations[:3]
    tb.values = tb.sig(len(regs))
    return tb


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
    assert tb.values[3] == 8, tb.values[3]


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
    assert tb.values[3] == 1, tb.values[3]


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
