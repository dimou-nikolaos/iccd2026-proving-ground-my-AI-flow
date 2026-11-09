"""Detection matrix: every catalogued core deviation must be *seen* by the harness.

This is the negative test of the differential method (the ISS-side analogue of
`make negative` in prep/sva and prep/rvcore). Run on the host:

    python3 -m unittest test_detect_matrix -v

For each entry in `fake_cores.CORE_DEVIATIONS`, at least one program profile must
produce a divergence against the golden ISS. A deviation that no profile reaches
means the generator is blind to that feature class, and a "no divergence" result
on the real core would be meaningless for it. Deviations listed in
`fake_cores.NOT_REACHABLE` are excluded *with their reason printed* - those need
a different instrument (irq driving, directed counter tests), not more programs.
"""

import unittest

from asm import assemble
from diff import first_divergence, steps_from_commits
from fake_cores import CORE_DEVIATIONS, NOT_REACHABLE
from randprog import GenConfig, generate, trap_config
from rv32_iss import RV32ISS

MEM_SIZE = 1 << 17
MAX_STEPS = 4000

# The program profiles a release-day campaign would actually run. Each catches a
# different feature class; the matrix proves the union covers the catalogue.
PROFILES = {
    "plain": GenConfig(seed=11, n_instr=120),
    "cr": GenConfig(seed=12, n_instr=120, include_cr=True, m_rate=0.15),
    "mext": GenConfig(seed=21, n_instr=120, m_rate=0.45, include_cr=True),
    "corners": trap_config(seed=13, n_instr=120),
}


def _run(src: str, factory=RV32ISS) -> RV32ISS:
    prog = assemble(src, base=0)
    iss = factory(mem_size=MEM_SIZE)
    prog.load_into(iss)
    iss.run(max_steps=MAX_STEPS)
    return iss


def detect(name: str, src: str):
    """Return the first divergence of fake `name` vs golden on `src`, or None."""
    ref = steps_from_commits(_run(src).trace)
    dut = steps_from_commits(_run(src, CORE_DEVIATIONS[name]).trace)
    return first_divergence(ref, dut)


class TestDetectMatrix(unittest.TestCase):
    def test_every_reachable_deviation_is_detected(self):
        for name in CORE_DEVIATIONS:
            if name in NOT_REACHABLE:
                continue
            with self.subTest(deviation=name):
                hits = {p: detect(name, generate(cfg)) for p, cfg in PROFILES.items()}
                caught = sorted(p for p, d in hits.items() if d is not None)
                self.assertTrue(caught, f"{name}: no profile produced a divergence")

    def test_matrix_covers_the_whole_catalogue(self):
        """Nothing may be silently dropped: reachable + not-reachable = all."""
        self.assertEqual(set(CORE_DEVIATIONS),
                         set(NOT_REACHABLE) | (set(CORE_DEVIATIONS) - set(NOT_REACHABLE)))
        self.assertTrue(set(NOT_REACHABLE) <= set(CORE_DEVIATIONS),
                        "NOT_REACHABLE lists an unknown deviation")

    def test_print_triage_map(self):
        """Which profile catches what, and what the method cannot reach at all."""
        rows = []
        for name in CORE_DEVIATIONS:
            if name in NOT_REACHABLE:
                rows.append(f"  {name:<34} {'-':<20} NOT REACHABLE: {NOT_REACHABLE[name]}")
                continue
            hits, sig = [], ""
            for p, cfg in PROFILES.items():
                d = detect(name, generate(cfg))
                if d is not None:
                    hits.append(p)
                    sig = sig or "/".join(str(x) for x in d.signature)
            rows.append(f"  {name:<34} {','.join(hits):<20} {sig}")
        print("\ncore-deviation detection matrix (profiles: " + ", ".join(PROFILES) + ")")
        print("\n".join(rows))
        self.assertGreaterEqual(len(rows), len(CORE_DEVIATIONS))


if __name__ == "__main__":
    unittest.main(verbosity=2)
