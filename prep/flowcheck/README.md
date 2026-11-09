# `prep/flowcheck/` — rehearsal of the real scoring pipeline

A synthetic FIFO design and a submission for it, used to exercise `pgflow` end to end
before the challenge designs are released. Nothing here is a competition artifact.

```
dma/                    # DMA-shaped second rehearsal (see dma/README.md) — richer fixture:
                        # APB+AXI VIPs vendored into sub/tests, L41 probe families,
                        # seeded LEN off-by-one missed by lenient public tb
variants/               # vA_rename (equiv GREEN) + vB_bad (semantic, must be REJECTED)
                        # used to test scripts/explorer-check.sh
design/                 # stands in for a released bundle
  design.yaml           # real schema (see docs/skills/flow-internals.md §1)
  rtl/filelist.f        # 2 files, top = flowcheck_top
  rtl/{fifo,flowcheck_top}.sv
  tb/public/            # informational public testbench
  flow/constraint.sdc
sub/                    # stands in for our submission
  rtl/                  # copied from design/rtl by run.sh
  tests/test_fifo.py    # evidence suite (drives V)
  assertions/fifo_sva.sv  # 9 assertions, two binds
  report.md
run.sh                  # copies rtl, tars, runs pgflow in the flow image
```

## Run

```bash
./run.sh --skip ppa    # build + public/evidence + mutation, ~17 s
./run.sh               # + baseline/submission PPA (OpenROAD), ~137 s
scripts/mutant-report.py runs/flowcheck-out
```

Expected: `V = 0.975` (`39/40`) and `P = 0.5` (submission RTL == baseline RTL, so parity is
the correct answer). The single survivor is the 16-bit write-counter saturation constant
(`16'hFFFF -> 16'hFFFE`), which needs ~65k accepted writes to observe — an example of the
V ceiling, not a missing test.

## What it validated (and broke)

- Archive layout: only `rtl/`, `tests/`, `assertions/`, `report.md` survive unpacking;
  `tests/test_*.py` is the discovery rule; helper modules beside the tests are importable.
- Assertions compile with RTL under `--assert`; a `bind` into a submodule reaches internal
  signals; `--assert` is mandatory or Verilator silently drops them.
- Mutant schemata (`+PGMUT=K`) build and run; every selected mutant has an exact
  `original -> mutated` expression in `stages/mutation.json`.
- Per-mutant timeout = `min(300, max(30, 2·evidence + 15))`; with a 0.5 s evidence suite
  every mutant gets the 30 s floor.
- Two sampling bugs in the *test* and *assertions* (not the DUT) were found and fixed:
  post-edge state reads are off by one, and SVA samples preponed values (`$past` pairs
  with the event that produced the current state).
- A mid-operation reset test is required to see reset-branch mutants, because the flow
  starts registers at zero.
- The PPA path is real OpenROAD/sky130hd; `remove_from_collection` in an SDC aborts
  synthesis because the timing engine is OpenSTA.
