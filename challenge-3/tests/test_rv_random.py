"""Random ISS-lockstep differential suite (Phase 3a backstop).

Each test generates a randprog program with a FIXED seed, runs it to
completion on the configured ISS (rv_core_env.SPEC_CFG, derived from
spec.md) and on the DUT, then compares:
  * the retire stream (wb_valid&&wb_en writes, rd!=0) via diff.py,
  * final registers (SIG-window dump),
  * final data memory (ISS sparse mem vs TB-observed stores).

Two wait patterns across the seed groups: no memory stalls (0,0) and
30% imem/dmem stalls (0.3,0.3) (REQ-03/04/10 stress). Three trap_mode
seeds (randprog.trap_config) additionally exercise misaligned /
sub-word / illegal / ebreak / CSR-corner / divide-corner classes with
the generator's self-emitted handler: traps are counted into data
memory, so they are ISS-lockstep comparable; a mismatch here is the
backstop for any residual seeded deviation.
"""

import cocotb

from randprog import GenConfig, trap_config
from rv_core_env import run_lockstep

BODY = dict(n_instr=150, include_cr=True, hazard_rate=0.5, m_rate=0.15,
            branch_rate=0.08, loadstore_rate=0.2)

# (seed, istall, dstall, tb_seed): group A stall-free, group B 30% waits.
RAND_CASES = [(s, 0.0, 0.0, 17) for s in (101, 103, 107, 113, 127)] + \
             [(s, 0.3, 0.3, 23) for s in (201, 211, 223, 227, 233)]

TRAP_CASES = [(301, 0.0, 0.0, 17), (302, 0.3, 0.3, 23), (303, 0.3, 0.3, 29)]


def _plain(seed, istall, dstall, tseed):
    return GenConfig(seed=seed, **BODY)


def _trap(seed, istall, dstall, tseed):
    return trap_config(seed=seed, n_instr=150)


def _mk(tag, seed, istall, dstall, tseed, mkcfg):
    async def test(dut):
        await run_lockstep(dut, mkcfg(seed, istall, dstall, tseed),
                           istall, dstall, tseed)
    test.__name__ = "test_rv_rand_%s%03d" % (tag, seed)
    test.__qualname__ = test.__name__
    return cocotb.test()(test)


for _s, _i, _d, _t in RAND_CASES:
    globals()["test_rv_rand_n%03d" % _s] = _mk("n", _s, _i, _d, _t, _plain)
    del _s, _i, _d, _t

for _s, _i, _d, _t in TRAP_CASES:
    globals()["test_rv_rand_t%03d" % _s] = _mk("t", _s, _i, _d, _t, _trap)
    del _s, _i, _d, _t
