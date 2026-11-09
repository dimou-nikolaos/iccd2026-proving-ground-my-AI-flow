"""End-to-end tests for the program shrinker + the ISS-vs-fake-core oracle.

Run on the host:  python3 -m unittest test_shrink -v

Scenario (the release-day one, rehearsed): a large random program diverges
between the golden ISS and a core that is wrong in a *known* way. The shrinker
must return a much smaller program that

  1. still diverges,
  2. has the *same structural divergence signature* as the original (same bug,
     not a new one that appeared once the noise was removed),
  3. still contains the ISA feature the bug needs, and
  4. uses few distinct registers (readable dataflow).

The fake cores here are `fake_cores.*` - the same catalogue the detection matrix
(test_detect_matrix) uses - so shrinking is exercised on deviations that are
known to be reachable.
"""

import re
import unittest

from asm import REGS, assemble
from diff import compare_run
from fake_cores import (DivByZeroReturnsZero, EbreakNoTrap, MinMaxSwapped,
                        NoSignExtendLoads, SraIsLogical, StoreByteClobbersWord,
                        TrapMepcOffByOne)
from randprog import GenConfig, generate, trap_config
from rv32_iss import RV32Config, RV32ISS
from shrink import ShrinkConfig, make_keep, shrink_program, split_source

MEM_SIZE = 1 << 16
MAX_STEPS = 1200
PASS = ShrinkConfig(max_passes=2, max_calls=600)


def cfg_iss(**cfg_kwargs):
    """ISS factory with a policy override (legitimate-ambiguity cases)."""
    def factory(mem_size: int) -> RV32ISS:
        return RV32ISS(mem_size=mem_size, config=RV32Config(**cfg_kwargs))
    return factory


def make_pair(ref_factory=RV32ISS, dut_factory=RV32ISS, max_steps: int = MAX_STEPS):
    """run_pair(source) -> Optional[Divergence] for golden vs "dut".

    Uses diff.compare_run (lockstep, early exit at the divergence) instead of
    running both models to completion: the shrinker calls this thousands of times.
    """
    def rp(src: str):
        prog = assemble(src, base=0)
        ref = ref_factory(mem_size=MEM_SIZE)
        dut = dut_factory(mem_size=MEM_SIZE)
        prog.load_into(ref)
        prog.load_into(dut)
        return compare_run(ref, dut, max_steps=max_steps)
    return rp


def distinct_regs(src: str) -> int:
    return len({t for t in re.findall(r"[A-Za-z_][A-Za-z_0-9']*", src) if t in REGS})


class TestShrink(unittest.TestCase):
    def check(self, src, dut_factory, cfg=PASS, features=(), max_lines=None,
              max_steps=MAX_STEPS):
        """Shrink and assert the four properties. Returns the ShrinkResult.

        `src` may be a list of candidate programs: whether a random program
        reaches a given deviation is seed-dependent (a sign-extension bug needs a
        *negative* byte in the window), so the test offers a few seeds and fails
        only if none of them reproduces.
        """
        srcs = [src] if isinstance(src, str) else list(src)
        rp = make_pair(dut_factory=dut_factory, max_steps=max_steps)
        orig = None
        for cand in srcs:
            orig = rp(cand)
            if orig is not None:
                src = cand
                break
        self.assertIsNotNone(orig, "no candidate program reproduces")
        keep = make_keep(rp)
        if max_lines:
            cfg = ShrinkConfig(max_passes=cfg.max_passes, max_calls=cfg.max_calls,
                               max_lines=max_lines)
        res = shrink_program(src, keep, cfg, base=0)
        self.assertTrue(keep(res.source), "shrunk program no longer reproduces")
        self.assertEqual(keep.signature, orig.signature,
                         "the repro drifted to a different divergence")
        self.assertLess(len(res.lines), len(split_source(src)))
        for feat in features:
            self.assertTrue(any(f in res.source for f in feat),
                            f"feature {feat} lost:\n{res.source}")
        return res

    # --- ISA-feature deviations -----------------------------------------
    def test_cr_missing_shrinks_to_one_z_instruction(self):
        src = generate(GenConfig(seed=5, n_instr=120, include_cr=True))
        res = self.check(src, cfg_iss(zba_zbb=False),
                         features=[("sh1add", "sh2add", "sh3add", "min", "minu", "max", "maxu")])
        self.assertLess(len(res.lines), 10)

    def test_minmax_swapped(self):
        src = generate(GenConfig(seed=10, n_instr=120, include_cr=True))
        self.check(src, MinMaxSwapped, features=[("min", "max")])

    def test_div_by_zero(self):
        src = generate(GenConfig(seed=6, n_instr=100, m_rate=0.35))
        self.check(src, DivByZeroReturnsZero, features=[("div", "rem")])

    def test_sra_is_logical(self):
        src = generate(GenConfig(seed=9, n_instr=120, shift_rate=0.3))
        self.check(src, SraIsLogical, features=[("sra", "srai")])

    # --- trap / memory / policy deviations (handler present) -------------
    def test_store_byte_clobbers_word(self):
        src = generate(trap_config(seed=13, n_instr=40, data_words=8, call_rate=0.0))
        self.check(src, StoreByteClobbersWord, features=[("sb", "sh")])

    def test_ebreak_no_trap(self):
        src = generate(trap_config(seed=16, n_instr=40, data_words=8, misalign_rate=0.0,
                                   illegal_rate=0.0, call_rate=0.0))
        self.check(src, EbreakNoTrap, features=[("ebreak",)])

    def test_mepc_off_by_one_needs_a_trap_and_a_mepc_read(self):
        src = generate(trap_config(seed=17, n_instr=40, data_words=8, misalign_rate=0.05,
                                   illegal_rate=0.0, call_rate=0.0))
        res = self.check(src, TrapMepcOffByOne, features=[("mepc",)])
        self.assertTrue(any(op in res.source for op in ("ecall", "ebreak", "lw", "sw", "csrr",
                                                        "mret", "handler")),
                        f"no trap path in repro:\n{res.source}")

    def test_signed_load_no_sign_extension(self):
        """Seed-sensitive: needs a negative byte in the scratch window."""
        srcs = [
            generate(trap_config(seed=s, n_instr=n, data_words=dw, csr_rate=0.0,
                                 illegal_rate=0.0, ebreak_rate=0.0, call_rate=0.0,
                                 misalign_rate=0.2))
            for s, n, dw in ((55, 100, 64), (23, 100, 8), (77, 100, 64))
        ]
        self.check(srcs, NoSignExtendLoads, features=[("lb", "lh")], max_steps=3000)

    def test_misalignment_policy_ambiguity(self):
        src = generate(trap_config(seed=14, n_instr=40, data_words=8, csr_rate=0.0,
                                   illegal_rate=0.0, ebreak_rate=0.0, call_rate=0.0,
                                   misalign_rate=0.3))
        res = self.check(src, cfg_iss(misaligned="support"),
                         features=[("lw", "sw", "lh", "sh", "lb", "sb")])
        self.assertLess(len(res.lines), 5)

    # --- shrinker properties --------------------------------------------
    def test_registers_are_canonicalized(self):
        src = generate(GenConfig(seed=10, n_instr=120, include_cr=True))
        res = self.check(src, MinMaxSwapped,
                         cfg=ShrinkConfig(max_passes=2, max_calls=600, reg_pools=(2, 3)))
        self.assertLessEqual(distinct_regs(res.source), 4,
                             f"register canonicalization failed:\n{res.source}")

    def test_deterministic(self):
        src = generate(GenConfig(seed=5, n_instr=80, include_cr=True))
        outs = []
        for _ in range(2):
            keep = make_keep(make_pair(dut_factory=cfg_iss(zba_zbb=False)))
            outs.append(shrink_program(src, keep, ShrinkConfig(max_passes=2), base=0).source)
        self.assertEqual(outs[0], outs[1])

    def test_healthy_program_is_left_alone(self):
        """No divergence -> nothing to shrink, and nothing is dropped."""
        src = generate(GenConfig(seed=3, n_instr=40))
        keep = make_keep(make_pair())
        res = shrink_program(src, keep, ShrinkConfig(max_passes=2), base=0)
        self.assertEqual(len(res.lines), len(split_source(src)))
        self.assertFalse(keep(res.source))

    def test_report_is_informative(self):
        src = generate(GenConfig(seed=6, n_instr=60, m_rate=0.35))
        res = self.check(src, DivByZeroReturnsZero)
        text = res.report()
        self.assertIn("oracle calls", text)
        self.assertIn("->", text)


if __name__ == "__main__":
    unittest.main(verbosity=2)
