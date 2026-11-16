"""Self-tests for the RV32IM+Zicsr ISS and assembler (run on the host, no cocotb)."""

import unittest

from asm import assemble
from rv32_iss import (
    RV32ISS, RV32Config,
    MSTATUS_MIE, CAUSE_ILLEGAL_INSTR, CAUSE_ECALL_M, CAUSE_LOAD_MISALIGNED,
)

MASK32 = 0xFFFF_FFFF

END = "\nend: j end\n"


def run_prog(src, steps=200, cfg=None, preload=None):
    iss = RV32ISS(mem_size=1 << 16, config=cfg)
    prog = assemble(src, base=0)
    prog.load_into(iss)
    if preload:
        for addr, data in preload.items():
            iss.mem[addr:addr + len(data)] = data
    iss.run(max_steps=steps)
    return iss


class TestAssembler(unittest.TestCase):
    def test_li_roundtrip(self):
        iss = run_prog("""
        li t0, 0x12345678
        li t1, -1
        li t2, 0
        li t3, 2048
        li t4, 2047
        li t5, -2048
        li t6, 0x80000000
        """ + END, steps=20)
        self.assertEqual(iss.regs[5], 0x1234_5678)
        self.assertEqual(iss.regs[6], MASK32)
        self.assertEqual(iss.regs[7], 0)
        self.assertEqual(iss.regs[28], 2048)
        self.assertEqual(iss.regs[29], 2047)
        self.assertEqual(iss.regs[30], 0xFFFF_F800)
        self.assertEqual(iss.regs[31], 0x8000_0000)

    def test_labels_and_org(self):
        iss = run_prog("""
        .org 0
        j start
        .org 0x40
        handler: mret
        start:  li t0, 0x40
                csrw mtvec, t0
                csrr a0, mtvec
        """ + END, steps=30)
        self.assertEqual(iss.regs[10], 0x40)

    def test_program_image(self):
        prog = assemble("""
        .org 0x10
        nop
        .word 0xDEADBEEF
        """)
        img = prog.image()
        self.assertEqual(len(img), 0x18)          # .org pads with NOPs
        self.assertEqual(img[0x14:0x18], (0xDEADBEEF).to_bytes(4, "little"))

class TestBaseISA(unittest.TestCase):
    def test_alu(self):
        iss = run_prog("""
        li t0, 5
        li t1, 7
        add a0, t0, t1
        sub a1, t1, t0
        slt a2, t0, t1
        sltu a3, t0, t1
        slli a4, t1, 4
        srli a5, t1, 1
        srai a6, t1, 1
        xori a7, t1, 0xFF
        """ + END, steps=40)
        self.assertEqual(iss.regs[10], 12)
        self.assertEqual(iss.regs[11], 2)
        self.assertEqual(iss.regs[12], 1)
        self.assertEqual(iss.regs[13], 1)
        self.assertEqual(iss.regs[14], 112)
        self.assertEqual(iss.regs[15], 3)
        self.assertEqual(iss.regs[16], 3)
        self.assertEqual(iss.regs[17], 7 ^ 0xFF)

    def test_shift_masking(self):
        # RV32 uses shamt[4:0]: shift by 33 == shift by 1.
        iss = run_prog("""
        li t0, 1
        li t1, 33
        sll a0, t0, t1
        sra a1, t0, t1
        li t2, -8
        sra a2, t2, t1
        """ + END, steps=30)
        self.assertEqual(iss.regs[10], 2)
        self.assertEqual(iss.regs[11], 0)
        self.assertEqual(iss.regs[12], MASK32 - 3)   # -8 >> 1 arithmetic = -4

    def test_loads_sign_extension(self):
        iss = run_prog("""
        li t0, 0x100
        lb a0, 0(t0)
        lbu a1, 0(t0)
        lh a2, 0(t0)
        lhu a3, 0(t0)
        lw a4, 0(t0)
        """ + END, steps=30, preload={0x100: bytes([0x80, 0xFF, 0x7F, 0x01])})
        self.assertEqual(iss.regs[10], 0xFFFF_FF80)
        self.assertEqual(iss.regs[11], 0x80)
        self.assertEqual(iss.regs[12], 0xFFFF_FF80)
        self.assertEqual(iss.regs[13], 0xFF80)
        self.assertEqual(iss.regs[14], 0x017F_FF80)

    def test_stores(self):
        iss = run_prog("""
        li t0, 0x100
        li t1, 0x200
        sb t0, 0(t1)
        sh t0, 4(t1)
        sw t0, 8(t1)
        """ + END, steps=30)
        self.assertEqual(iss.mem[0x200:0x201], bytes([0x00]))
        self.assertEqual(iss.mem[0x204:0x206], bytes([0x00, 0x01]))
        self.assertEqual(iss.mem[0x208:0x20C], (0x100).to_bytes(4, "little"))

    def test_negative_offset(self):
        iss = run_prog("""
        li t0, 0x104
        lb a0, -4(t0)
        """ + END, steps=20, preload={0x100: bytes([0x80, 0, 0, 0])})
        self.assertEqual(iss.regs[10], 0xFFFF_FF80)

    def test_branch_signedness(self):
        iss = run_prog("""
        li t0, -1
        li t1, 1
        li a0, 0
        li a1, 0
        blt t0, t1, L1
        li a0, 1
        L1:
        bltu t0, t1, L2
        li a1, 1
        L2:
        """ + END, steps=40)
        self.assertEqual(iss.regs[10], 0)   # signed: taken, a0 untouched
        self.assertEqual(iss.regs[11], 1)   # unsigned: not taken, a1 set

    def test_jal_jalr_link_and_bit0(self):
        iss = run_prog("""
        .org 0
        li a0, 0
        jal ra, func
        back: li a1, 7
        j end
        .org 0x40
        func: li a0, 42
        ori ra, ra, 1       # force bit0 of the return address
        jalr x0, ra, 0
        .org 0x80
        end: j end
        """, steps=40)
        self.assertEqual(iss.regs[10], 42)
        self.assertEqual(iss.regs[11], 7)
        self.assertEqual(iss.regs[1] & 1, 1)

    def test_x0_writes_discarded(self):
        iss = run_prog("""
        li x0, 12345
        add x0, x0, x0
        lw x0, 0(x0)
        """ + END, steps=20)
        self.assertEqual(iss.regs[0], 0)


class TestMExtension(unittest.TestCase):
    def test_div_rem_zero_and_overflow(self):
        iss = run_prog("""
        li t0, 0x80000000
        li t1, -1
        div a0, t0, t1
        rem a1, t0, t1
        li t2, 0
        div a2, t0, t2
        rem a3, t0, t2
        mul a4, t0, t1
        mulh a5, t0, t1
        divu a6, t0, t2
        remu a7, t0, t2
        """ + END, steps=40)
        self.assertEqual(iss.regs[10], 0x8000_0000)  # INT_MIN / -1
        self.assertEqual(iss.regs[11], 0)            # remainder is 0
        self.assertEqual(iss.regs[12], MASK32)       # div by zero -> -1
        self.assertEqual(iss.regs[13], 0x8000_0000)  # rem by zero -> dividend
        self.assertEqual(iss.regs[14], 0x8000_0000)  # MUL low word
        self.assertEqual(iss.regs[15], 0)            # MULH high word
        self.assertEqual(iss.regs[16], MASK32)       # divu by zero -> 2^32-1
        self.assertEqual(iss.regs[17], 0x8000_0000)  # remu by zero -> dividend

    def test_div_truncates_toward_zero(self):
        iss = run_prog("""
        li t0, -7
        li t1, 2
        div a0, t0, t1
        rem a1, t0, t1
        """ + END, steps=20)
        self.assertEqual(iss.regs[10], MASK32 - 2)   # -3
        self.assertEqual(iss.regs[11], MASK32)       # -1

    def test_mulhsu(self):
        iss = run_prog("""
        li t0, -1
        li t1, 2
        mulhsu a0, t0, t1      # (-1 * 2) >> 32 = -1
        mulhu a1, t0, t1       # (2^32-1)*2 >> 32 = 1
        """ + END, steps=20)
        self.assertEqual(iss.regs[10], MASK32)
        self.assertEqual(iss.regs[11], 1)


class TestZbaZbb(unittest.TestCase):
    def test_min_max_signed_unsigned(self):
        iss = run_prog("""
        li t0, 0x7FFFFFFF
        li t1, 0x80000000
        max a0, t0, t1
        maxu a1, t0, t1
        min a2, t0, t1
        minu a3, t0, t1
        """ + END, steps=30)
        self.assertEqual(iss.regs[10], 0x7FFF_FFFF)
        self.assertEqual(iss.regs[11], 0x8000_0000)
        self.assertEqual(iss.regs[12], 0x8000_0000)
        self.assertEqual(iss.regs[13], 0x7FFF_FFFF)

    def test_min_max_equal(self):
        iss = run_prog("""
        li t0, 0xDEADBEEF
        min a0, t0, t0
        max a1, t0, t0
        minu a2, t0, t0
        maxu a3, t0, t0
        """ + END, steps=20)
        for r in (10, 11, 12, 13):
            self.assertEqual(iss.regs[r], 0xDEAD_BEEF)

    def test_shadd(self):
        iss = run_prog("""
        li t0, 3
        li t1, 5
        sh1add a0, t0, t1
        sh2add a1, t0, t1
        sh3add a2, t0, t1
        li t2, 0x20000000
        sh3add a3, t2, x0
        """ + END, steps=30)
        self.assertEqual(iss.regs[10], 11)
        self.assertEqual(iss.regs[11], 17)
        self.assertEqual(iss.regs[12], 29)
        self.assertEqual(iss.regs[13], 0)            # 0x20000000<<3 wraps to 0

    def test_disabled_traps_illegal(self):
        iss = run_prog("""
        .org 0
        j start
        .org 0x40
        handler:
          csrr t0, mcause
          csrr t1, mepc
          addi t1, t1, 4
          csrw mepc, t1
          mret
        start:
          li t0, 0x40
          csrw mtvec, t0
          sh1add a0, x0, x0
          li a1, 9
        end: j end
        """, steps=40, cfg=RV32Config(zba_zbb=False))
        self.assertEqual(iss.csrs["mcause"], CAUSE_ILLEGAL_INSTR)
        self.assertEqual(iss.regs[11], 9)


class TestTrapsCSRs(unittest.TestCase):
    def test_ecall_mret(self):
        iss = run_prog("""
        .org 0
        j start
        .org 0x40
        handler:
          csrr t0, mcause
          csrr t1, mepc
          addi t1, t1, 4
          csrw mepc, t1
          mret
        start:
          li t0, 0x40
          csrw mtvec, t0
          li a0, 1
          ecall
          li a1, 2
        end: j end
        """, steps=40)
        self.assertEqual(iss.csrs["mcause"], CAUSE_ECALL_M)
        self.assertEqual(iss.regs[10], 1)
        self.assertEqual(iss.regs[11], 2)

    def test_illegal_instruction(self):
        iss = run_prog("""
        .org 0
        j start
        .org 0x40
        handler:
          csrr t0, mcause
          csrr t1, mtval
          csrr t2, mepc
          addi t2, t2, 4
          csrw mepc, t2
          mret
        start:
          li t0, 0x40
          csrw mtvec, t0
          .word 0x00000000
          li a0, 5
        end: j end
        """, steps=40)
        self.assertEqual(iss.csrs["mcause"], CAUSE_ILLEGAL_INSTR)
        self.assertEqual(iss.csrs["mtval"], 0)
        self.assertEqual(iss.regs[10], 5)
        self.assertEqual(iss.regs[5], CAUSE_ILLEGAL_INSTR)  # mcause read by handler

    def test_ro_csr_write_illegal(self):
        # spec REQ-32: write to a CSR with addr[11:10]=3 raises illegal
        # (mcause is RW per spec; mhartid 0xF14 is in the read-only range).
        iss = run_prog("""
        .org 0
        j start
        .org 0x40
        handler:
          csrr t0, mcause
          csrr t1, mepc
          addi t1, t1, 4
          csrw mepc, t1
          mret
        start:
          li t0, 0x40
          csrw mtvec, t0
          csrw mhartid, x0
          li a0, 1
        end: j end
        """, steps=40)
        self.assertEqual(iss.regs[5], CAUSE_ILLEGAL_INSTR)  # handler read mcause=2
        self.assertEqual(iss.regs[10], 1)

    def test_csrrs_zero_source_no_write(self):
        iss = run_prog("""
        li t0, 0x40
        csrw mtvec, t0
        csrrs x0, mtvec, x0
        csrrc x0, mtvec, x0
        csrr a0, mtvec
        li t1, 1
        csrrs x0, mscratch, t1
        csrr a1, mscratch
        """ + END, steps=30)
        self.assertEqual(iss.regs[10], 0x40)
        self.assertEqual(iss.regs[11], 1)

    def test_misaligned_load_policies(self):
        src = """
        li t0, 0x102
        lw a0, 0(t0)
        """ + END
        iss = run_prog(src, steps=20, preload={0x100: b"\x11\x22\x33\x44"})
        self.assertEqual(iss.csrs["mcause"], CAUSE_LOAD_MISALIGNED)
        self.assertEqual(iss.csrs["mtval"], 0x102)
        iss = run_prog(src, steps=20, preload={0x100: b"\x11\x22\x33\x44"},
                       cfg=RV32Config(misaligned="ignore"))
        self.assertEqual(iss.regs[10], 0x4433_2211)

    def test_interrupt_rearm_after_mret(self):
        # Spec REQ-37/38: level-sensitive line, taken before the next
        # instruction takes effect; no shadow window after MRET.
        iss = RV32ISS(mem_size=1 << 16)
        prog = assemble("""
        .org 0
        j start
        .org 0x40
        handler:
          addi s0, s0, 1
          mret
        start:
          li t0, 0x40
          csrw mtvec, t0
          li t0, 0x800
          csrrs x0, mie, t0
          li t0, 8
          csrrs x0, mstatus, t0
        loop: addi s1, s1, 1
          j loop
        """, base=0)
        prog.load_into(iss)
        iss.set_external_irq(11, True)
        iss.run(max_steps=30)

        self.assertGreaterEqual(iss.regs[8], 2)        # handler re-entered
        irq_commits = [c for c in iss.trace if (c.trap_cause or 0) & 0x8000_0000]
        self.assertTrue(irq_commits)
        self.assertEqual(irq_commits[0].trap_cause, 0x8000_000B)

        # Line stays asserted: the instruction right after each MRET is interrupted again.
        for i, c in enumerate(iss.trace):
            if c.instr == 0x3020_0073 and i + 1 < len(iss.trace):
                self.assertEqual(iss.trace[i + 1].trap_cause, 0x8000_000B,
                                 "interrupt not retaken immediately after MRET")

    def test_determinism(self):
        src = """
        li t0, 1234
        li t1, 5678
        mul a0, t0, t1
        div a1, t0, t1
        sw a0, 0x300(x0)
        """ + END
        a = run_prog(src, steps=30)
        b = run_prog(src, steps=30)
        self.assertEqual(a.regs, b.regs)
        self.assertEqual(bytes(a.mem[0x300:0x304]), bytes(b.mem[0x300:0x304]))


class TestSpecPolicies(unittest.TestCase):
    def test_mstatus_reset_and_write_ignore(self):
        iss = run_prog("""
        csrr a0, mstatus
        li t0, 0xFFFFF800
        csrrw x0, mstatus, t0
        csrr a1, mstatus
        """ + END, steps=30)
        self.assertEqual(iss.regs[10], 0x1800)     # MPP reads 3 at reset
        self.assertEqual(iss.regs[11], 0x1800)     # MPP/other bits ignore writes

    def test_mie_bit_mask(self):
        iss = run_prog("""
        li t0, 0xFFF
        csrrw x0, mie, t0
        csrr a0, mie
        """ + END, steps=20)
        self.assertEqual(iss.regs[10], 0x880)      # only MTIE/MEIE exist

    def test_mcause_mtval_are_rw(self):
        iss = run_prog("""
        li t1, 0x1234
        csrw mcause, t1
        csrr a0, mcause
        li t2, 0xABCD0001
        csrw mtval, t2
        csrr a1, mtval
        csrr a2, mepc
        """ + END, steps=30)
        self.assertEqual(iss.regs[10], 0x1234)
        self.assertEqual(iss.regs[11], 0xABCD_0001)
        self.assertEqual(iss.regs[12], 0)          # no trap happened

    def test_counter_csrs_are_rw_64bit(self):
        iss = run_prog("""
        csrr a0, mcycle
        li t0, 100
        csrw mcycle, t0
        csrr a1, mcycle
        csrr a2, minstret
        li t1, 7
        csrw minstret, t1
        csrr a3, minstret
        csrr a4, minstreth
        csrr a5, mcycleh
        """ + END, steps=40)
        self.assertEqual(iss.regs[10], 0)          # counter starts at 0
        self.assertEqual(iss.regs[11], 100)        # write wins over this step's tick
        self.assertEqual(iss.regs[12], 4)          # 4 instructions retired before this read
        self.assertEqual(iss.regs[13], 7)
        self.assertEqual(iss.regs[14], 0)
        self.assertEqual(iss.regs[15], 0)

    def test_misaligned_branch_target(self):
        iss = run_prog("""
        .org 0
        j start
        start:
          li t0, 0x40
          csrw mtvec, t0
          li t3, 1
          bne t3, x0, 6
          addi t4, t4, 1
        .org 0x40
        handler:
          csrr t0, mcause
          csrr t1, mtval
          csrr t2, mepc
          addi t2, t2, 4
          csrw mepc, t2
          mret
        """ + END, steps=60)
        self.assertEqual(iss.regs[5], 0)           # mcause 0: instr misaligned
        self.assertEqual(iss.regs[6], 6)           # mtval = taken target (address 6)
        self.assertEqual(iss.regs[28], 1)          # trapping instr untouched
        self.assertEqual(iss.regs[29], 1)          # resumed at mepc+4


if __name__ == "__main__":
    unittest.main(verbosity=2)
