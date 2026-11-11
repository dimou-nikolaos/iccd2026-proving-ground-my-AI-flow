# challenge-2/neg — verification scaffold (NOT shipped)

Everything in this directory sits **outside** `challenge-2/assertions/`, which matters:
`pgflow.public.assertion_files()` uses `assertions.rglob("*")`, i.e. *every* `.sv`/`.v`
file anywhere under `assertions/` is compiled into the evidence build. Nothing here may
therefore move into `assertions/`.

## Contents

| Path | What |
|---|---|
| `Makefile.smoke` | Compiles `rtl/filelist.f` + `$(ASSERT_DIR)/*.sv` with `--assert` and runs a cocotb module — mirrors `pgflow.public.build_with_assertions` (same `-Wno-fatal --x-assign unique --x-initial unique` args plus the flow's `+verilator+seed+… +verilator+rand+reset+0` plusargs). |
| `micro/test_req14.py` | Scratch stimulus: 80 % AXI stall so the REQ-14 valid/payload-hold checks are exercised non-vacuously. |
| `obs/` | Copy of the assertion set + `zz_obs.sv`, which counts AR/AW/W stall cycles (proves the REQ-14 antecedents occur). |
| `brk_*/rtl/` | One-mutation copies of the **fixed** RTL used as negative self-checks (below). |

## Commands (always inside the flow image)

```bash
# lint the whole set the way the flow's build stage does
sudo -n docker run --rm --cpus 6 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-2:/design:ro --entrypoint bash proving-ground/flow:public -c \
  'cd /design && verilator --lint-only --assert -Wno-fatal -Wall -Wno-DECLFILENAME \
   -Wno-UNUSEDSIGNAL -Wno-UNUSEDPARAM --top-module dma_ctrl \
   $(grep -v "^\s*\(//\|#\|\$\)" rtl/filelist.f) assertions/*.sv'

# public tests + assertions  (0 assertion fires expected)
sudo -n docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-2:/design -v $PWD/runs/neglogs:/logs --entrypoint bash \
  proving-ground/flow:public -c \
  'cd /design/neg && make -f Makefile.smoke'

# probe suite + assertions
... -c 'cd /design/neg && make -f Makefile.smoke TEST_DIR=/design/probe TEST_MODULE=test_probe'

# one negative self-check
... -c 'cd /design/neg && make -f Makefile.smoke RTL_ROOT=/design/neg/brk_arb_ge'
```

## Negative self-checks (each = one mutation of the fixed RTL, one line)

| Fixture | Mutation (mutation-operator class) | Assertion that fires | Fires with |
|---|---|---|---|
| `brk_arb_ge` | `dma_arb.sv`: `prio > best_prio` → `>=` (relational) | `engine_sva.a_req24_grant_golden` | probes (p1) |
| `brk_word_end` | `dma_engine.sv`: `word_end` drops the `m_bresp == RESP_OKAY` term (drop_term) | `engine_sva.a_word_end_def` | probes (p3) |
| `brk_len0` | `dma_engine.sv`: grant always takes `S_RD_ADDR` (missing LEN=0 case) | `engine_sva.a_req18_len0_end` | probes (p2) |
| `brk_irq` | `dma_irq.sv`: `irq = \|status_q` (enable term dropped) | `irq_sva.a_req35_irq` | public + probes |
| `brk_hwreq` | `dma_chan.sv`: `hw_req = cfg_q[3]` (wrong bit) | `chan_sva.a_cfg_bits_def` | public + probes |
| `brk_decode` | `dma_apb.sv`: channel window constant `5'b00010` → `5'b00011` (constant) | `apb_sva.a_decode_chan` | public + probes |
| `brk_arhold` | `dma_engine.sv`: `S_RD_ADDR` advances without `m_arready` (drop_term on a handshake guard) | `axi_sva.a_req14_ar_hold` | `micro/test_req14.py` |
| `brk_wdata` | `dma_engine.sv`: `m_wdata = data_q + 1` (arith) | `engine_sva.a_wdata_def` | public + probes |
| `brk_decswap` | `dma_ctrl.sv`: `.wr_next` strobe generated from `chan_reg == R_CTRL` (swap in the per-channel write decode) | `ctrl_sva.a_wr_next_<n>` | probes |

All 9 fire; the same fixtures produce **0** fires on the unmutated RTL. Because
Verilator 5.048 aborts the simulation at the **first** failed assertion
(`%Error: ... Verilog $stop`), one fixture only shows its first fire — that is
expected, not a second bug.

## Clean-RTL evidence (final, `assertions/` = 5 files, 199 `assert property`)

| Run | Result | Assertion fires |
|---|---|---|
| lint (`--lint-only --assert -Wall`) | no `%Error` | – |
| public tb + assertions, seed 4242 / 123456 | `TESTS=8 PASS=8 FAIL=0` | **0** |
| probe suite + assertions, seed 4242 / 987654 | `TESTS=5 PASS=5 FAIL=0` | **0** |
| `tests/` suite + assertions (the flow's V-stage combination) | `TESTS=53 PASS=53 FAIL=0` | **0** |
| `neg/micro/test_req14.py` + unmutated RTL | passes | **0** |

Command used for all of the above:

```bash
sudo -n docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-2:/design --entrypoint bash proving-ground/flow:public \
  -c 'cd /design/neg && make -f Makefile.smoke -B SIM_BUILD=/tmp/x \
       [TEST_DIR=/design/tests TEST_MODULE=test_dma_reqs,test_bus_protocol,test_outstanding,test_cr_desc]'
```

## Verilator 5.048 gotchas found while building this set (keep out of `assertions/`)

1. **`x inside {CONST_A, CONST_B}` is fine, but `x inside {y, z}` with non-constant
   RHS is rejected**: `%Error-UNSUPPORTED: RHS of ==? or !=? is fourstate but not a
   constant`. State-set legality checks therefore use explicit `==`/`||` comparisons
   (or compare against a literal range like `state_q <= 4'd10`).
2. **Concurrent assertions inside a `generate` loop whose property differs only by the
   genvar index are mis-compiled** (their `$past`/sampled expressions get aliased), so
   the assertion fires on *correct* RTL. The per-channel write-decode checks in
   `ctrl_sva.sv` are consequently unrolled by hand with literal indices.
3. **`--lint-only` with `--top-module dma_ctrl` prunes a module whose bind is not
   elaborated**, so an unresolved name inside an observer module can pass lint and only
   break the smoke build. Always finish with the smoke build, not just lint.
4. **Reset-phase checks need care**: registers are 0 with `+verilator+rand+reset+0`, and
   the first posedge after `rst_n` is already a valid clock, so `!rst_n |-> <reset value>`
   can false-fire. The reset modules use `(!rst_n && $past(!rst_n))` plus an explicit
   post-release check (e.g. `a_req26_last_after_rst`).
