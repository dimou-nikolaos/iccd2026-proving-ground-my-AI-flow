"""Tests for the commit-stream diff + instruction classifier used by lockstep tests.

Run on the host:  python3 -m unittest test_diff -v
"""

import unittest

from asm import NOP, assemble, encode
from diff import (Ev, commit_events, first_divergence, instr_class,
                  steps_from_commits)
from fake_cores import NoSignExtendLoads
from rv32_iss import Commit, RV32Config, RV32ISS


def run_steps(src, cls=RV32ISS, steps=200):
    iss = cls(mem_size=1 << 12)
    assemble(src, base=0).load_into(iss)
    iss.run(max_steps=steps)
    return steps_from_commits(iss.trace)


class TestInstrClass(unittest.TestCase):
    def test_coarse_classes(self):
        cases = [
            ("add a0,a1,a2", "R:add"),
            ("sub a0,a1,a2", "R:add"),          # funct7 variant: same coarse class
            ("sll a0,a1,a2", "R:sll"),
            ("srai a0,a1,3", "I:srli:srai"),
            ("div a0,a1,a2", "M:div"),
            ("mulhu a0,a1,a2", "M:mulhu"),
            ("sh2add a0,a1,a2", "Z:sh2add"),
            ("minu a0,a1,a2", "Z:minu"),
            ("maxu a0,a1,a2", "Z:maxu"),
            ("lw a0,4(s0)", "load:lw"),
            ("lbu a0,4(s0)", "load:lbu"),
            ("sh a0,4(s0)", "store:sh"),
            ("bltu a0,a1,L1", "branch:bltu"),
            ("jalr a0,s0,4", "jalr"),
            ("csrrw a0,mscratch,a1", "csr:csrrw"),
            ("mret", "mret"),
            ("ecall", "ecall"),
            ("fence", "fence"),
        ]
        for text, want in cases:
            with self.subTest(text=text):
                self.assertEqual(instr_class(encode(text)[0]), want)

    def test_illegal_encodings(self):
        for word in (0x0000_0000, 0xFFFF_FFFF, 0x0000_004B):
            with self.subTest(word=hex(word)):
                self.assertTrue(instr_class(word).startswith(("illegal", "system")),
                                instr_class(word))

    def test_class_is_value_independent(self):
        """Register / immediate changes must not change the class.

        The shrinker rewrites operands constantly; if the class moved, signature
        pinning would reject every candidate.
        """
        self.assertEqual(instr_class(encode("add t0, t1, t2")[0]), "R:add")
        self.assertEqual(instr_class(encode("add s9, s8, s7")[0]), "R:add")
        self.assertEqual(instr_class(encode("addi a0, a0, 5")[0]), "I:addi")
        self.assertNotEqual(instr_class(encode("add t0,t1,t2")[0]),
                            instr_class(encode("addi t0,t1,5")[0]))


class TestEvents(unittest.TestCase):
    def test_rd_and_mem(self):
        c = Commit(pc=0x10, instr=encode("addi a0,x0,7")[0], rd=10, wdata=7)
        c.mem_writes.append((0x8000, 4, 0xDEAD))
        ev = commit_events(c)
        self.assertEqual(ev, (Ev("rd", 10, 7), Ev("mem", 0x8000, 4, 0xDEAD)))

    def test_trap_csrs_folded_away(self):
        c = Commit(pc=0x20, instr=encode("lw a0,3(s0)")[0],
                   csr_writes=[(0x342, 4), (0x341, 0x20), (0x343, 0x8003), (0x300, 8)])
        c.trap_cause = 4
        c.trap_epc = 0x20
        ev = commit_events(c)
        self.assertEqual(ev, (Ev("trap", 4, 0x20),))
        full = commit_events(c, csr_trap_writes=True)
        self.assertEqual(len(full), 5)

    def test_interrupt_pseudo_commit_has_no_instruction(self):
        """A 0x0000_0000 fetch is illegal; an interrupt pseudo-commit is not an instruction."""
        irq = Commit(pc=0x40, instr=0, trap_cause=0x8000_0000 | 11, trap_epc=0x40)
        bad = Commit(pc=0x44, instr=0, trap_cause=2, trap_epc=0x44)
        steps = steps_from_commits([irq, bad])
        self.assertIsNone(steps[0].instr)
        self.assertEqual(steps[1].instr, 0)


class TestFirstDivergence(unittest.TestCase):
    def test_identical(self):
        a = run_steps("addi a0,x0,1\naddi a1,x0,2\nadd a2,a0,a1\nj 0")
        self.assertIsNone(first_divergence(a, list(a)))

    def test_different_instruction_at_same_commit(self):
        ref = run_steps("addi a0,x0,1\naddi a1,x0,2\nadd a2,a0,a1\nj 0")
        dut = run_steps("addi a0,x0,1\naddi a1,x0,2\nsub a2,a0,a1\nj 0")
        d = first_divergence(ref, dut)
        self.assertIsNotNone(d)
        self.assertEqual((d.index, d.kind), (2, "instr"))

    def test_extra_retired_instruction(self):
        """A pipeline symptom: dut retires one commit more than the reference."""
        ref = steps_from_commits([Commit(0x0, NOP), Commit(0x4, NOP)])
        dut = steps_from_commits([Commit(0x0, NOP), Commit(0x4, NOP),
                                  Commit(0x8, NOP)])
        d = first_divergence(ref, dut)
        self.assertIsNotNone(d)
        self.assertEqual((d.index, d.kind), (2, "length"))

    def test_missing_write_is_detected(self):
        """The important case: dut computes the right thing but never writes rd."""
        ref = [steps_from_commits([Commit(0x0, NOP, rd=1, wdata=5)])[0]]
        dut = [steps_from_commits([Commit(0x0, NOP)])[0]]
        d = first_divergence(ref, dut)
        self.assertEqual(d.kind, "event")
        self.assertEqual(d.signature, ("event", "I:addi", "-rd"))


class TestSignatureSurvivesShrinking(unittest.TestCase):
    """The property the shrinker depends on: same bug, different program text.

    Shrinking deletes instructions (PCs move) and minimizes immediates (values
    change). If the signature moved, every candidate would be rejected.
    """

    def _sig(self, src):
        ref = run_steps(src)
        dut = run_steps(src, NoSignExtendLoads)
        return first_divergence(ref, dut)

    def test_same_bug_different_text(self):
        big = ("        li s0, 0x100\n"
               "        li t0, 0x80\n"
               "        addi t1, t0, 1\n"
               "        sb t1, 0x40(s0)\n"
               "        sb t0, 0(s0)\n"
               "        lb a2, 0(s0)\n"
               "        j 0\n")
        small = ("        li s0, 0x200\n"
                 "        li t0, 0x81\n"
                 "        sb t0, 0(s0)\n"
                 "        lb a2, 0(s0)\n"
                 "        j 0\n")
        a, b = self._sig(big), self._sig(small)
        self.assertIsNotNone(a)
        self.assertIsNotNone(b)
        self.assertEqual(a.signature, ("event", "load:lb", "rd"))
        self.assertEqual(a.signature, b.signature)
        self.assertNotEqual(a.pc, b.pc)           # the address moved; the signature did not


class TestTrapDivergence(unittest.TestCase):
    def test_fault_vs_support_same_signature(self):
        src = ("li s0, 0x100\nli a1, 0x11\n"
               "lw a2, 0(s0)\nlw a3, 1(s0)\nj 0\n")
        fault = run_steps(src)                                   # default: misaligned faults
        support = run_steps(src, lambda **kw: RV32ISS(
            mem_size=1 << 12, config=RV32Config(misaligned="support")))
        d = first_divergence(fault, support)
        self.assertIsNotNone(d)
        self.assertEqual(d.signature[0], "event")
        self.assertIn(d.signature[2], ("-trap", "+trap", "rd"))
        # the trapping instruction is a misaligned load in both views
        self.assertEqual(d.signature[1], "load:lw")


if __name__ == "__main__":
    unittest.main(verbosity=2)
