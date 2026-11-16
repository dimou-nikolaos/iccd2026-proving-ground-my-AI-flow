"""Sanity tests for the random program generator: no traps, deterministic, CR coverage."""

import unittest

from asm import assemble
from randprog import GenConfig, generate
from rv32_iss import RV32ISS


class TestRandProg(unittest.TestCase):
    def _run(self, src, steps):
        iss = RV32ISS(mem_size=1 << 16)
        assemble(src, base=0).load_into(iss)
        iss.run(max_steps=steps)
        return iss

    def test_seeds_run_without_traps(self):
        for seed in range(10):
            with self.subTest(seed=seed):
                cfg = GenConfig(seed=seed, n_instr=80, include_cr=bool(seed % 2))
                src = generate(cfg)
                iss = self._run(src, steps=4000)
                self.assertEqual(iss.csrs["mcause"], 0,
                                 f"unexpected trap at pc={iss.csrs['mepc']:#x}")
                self.assertGreater(iss.instret, 80)

    def test_determinism(self):
        cfg = GenConfig(seed=42, n_instr=120)
        a = self._run(generate(cfg), steps=5000)
        b = self._run(generate(cfg), steps=5000)
        self.assertEqual(a.regs, b.regs)
        self.assertEqual(bytes(a.mem[0x8000:0x8100]), bytes(b.mem[0x8000:0x8100]))

    def test_cr_instructions_emitted(self):
        cfg = GenConfig(seed=7, n_instr=200, include_cr=True)
        src = generate(cfg)
        cr = ["sh1add", "sh2add", "sh3add", "min", "minu", "max", "maxu"]
        self.assertTrue(any(op in src for op in cr))
        iss = self._run(src, steps=6000)
        self.assertEqual(iss.csrs["mcause"], 0)

    def test_large_program(self):
        cfg = GenConfig(seed=99, n_instr=1000, include_cr=True)
        src = generate(cfg)
        iss = self._run(src, steps=30000)
        self.assertEqual(iss.csrs["mcause"], 0)
        self.assertGreater(iss.instret, 1000)


if __name__ == "__main__":
    unittest.main(verbosity=2)
