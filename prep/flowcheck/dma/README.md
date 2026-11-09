# dmacheck — DMA-shaped rehearsal of the scoring pipeline (prep, never submitted)

Stands in for a released challenge-2 bundle plus our submission, to rehearse the
*real* pgflow pipeline on a design shaped like dma_ctrl (APB4 slave + AXI4-Lite
master) before Nov 10:

- baseline (design/rtl): `dma_top` ties `bug=8'h01` — a seeded LEN off-by-one
  that the lenient public tb does NOT catch (same failure mode as a real
  seeded deviation);
- submission (sub/rtl): `bug=8'd0` (the Phase-2 "fix");
- sub/tests: the day-1 evidence skeleton (dma_env + test_dma) with the bus VIPs
  vendored in (the flow only puts `tests/` on pythonpath);
- sub/assertions: dma_irq/dma_fsm SVA + the generic APB4/AXI4-Lite protocol
  observers, bound with explicit all-input port maps.

## Run

    ./run.sh --skip ppa     # build + public + evidence + mutation
    ./run.sh                # full, incl. baseline+submission PPA

Expected on `--skip ppa`: public tb passes (it is lenient by design), evidence
passes, and V reflects which of the 40 mutants the skeleton kills.
