# challenge-2 — design knowledge (dma_ctrl)

> Created by `scripts/design-init.sh`. Fill in during Phase 1; keep signal names
> accurate, they are the bind targets for every assertion.

## Top / timing
- Top module: `dma_ctrl`
- Clock / reset: `clk / rst_n`
- Clock period: 10.0 ns (sky130hd)
- RTL size: 777 lines

## Filelist
```
  rtl/dma_pkg.sv
  rtl/dma_apb.sv
  rtl/dma_chan.sv
  rtl/dma_arb.sv
  rtl/dma_engine.sv
  rtl/dma_irq.sv
  rtl/dma_ctrl.sv
```

## Hierarchy

| File | Module | Role |
|---|---|---|
| dma_pkg.sv | package | offsets, IRQ_MASK, strb_mask(), UNIT_ID |
| dma_apb.sv | dma_apb | APB4 decode: access, mapped, glob/chan select, wmask |
| dma_chan.sv | dma_chan ×4 | per-channel regs: en/abort/err/done, cfg, src/dst/len/next |
| dma_engine.sv | dma_engine | FSM S_IDLE..S_STOP, arbiter instance, AXI master, events, dack, last_grant |
| dma_arb.sv | dma_arb | priority + rotating selection (combinational) |
| dma_irq.sv | dma_irq | IRQ_STATUS(W1C)/IRQ_ENABLE regs, irq output |
| dma_ctrl.sv | dma_ctrl | top: glue + prdata mux + GCTRL |

## Signal map (assertion bind targets)

| Module | Signal | Width | Meaning |
|---|---|---|---|
| dma_engine | state_q | enum 4b | S_IDLE,S_RD_ADDR,S_RD_DATA,S_WR,S_WR_RESP,S_END,S_FD_ADDR,S_FD_DATA,S_RELEASE,S_ERR,S_STOP |
| dma_engine | own_q, last_q, words_q, moved_q, fd_idx_q | 2,2,4,1,2 | owner, LAST_GRANT, burst counter, ≥1 moved, fetch index |
| dma_engine | eligible, grant_valid, grant_idx | 4,1,2 | arbiter I/O |
| dma_engine | ev_done, ev_err (ev_desc after CR) | 4 | event pulses |
| dma_chan | en_q, abort_q, err_q, done_q, cfg_q[5:0], len_q | — | per-channel state; cfg bits SRC_FIX,DST_FIX,HW_REQ,PRIO[1:0],LINK |
| dma_irq | status_q, enable_q, set_vec, clr_vec | 32 | [3:0]DONE [7:4]ERR ([11:8]DESC after CR) |
| dma_apb | access, mapped, is_glob, is_chan | 1 | decode flags |

## Protocol / register map
<!-- bus protocol, register offsets, control/status bit fields, FSM states -->

## Change request
<!-- summary of change_request.md; REQ-ids it adds -->

## Confirmed deviations
| REQ | Location | Spec says | RTL does | Confidence |
|---|---|---|---|---|

## PPA notes
- Baseline (from `runs/challenge-2-baseline-out/stages/ppa.json`):
- Best submission:
- Applied wins:

## V ceiling
- Observed:
- Unkillable mutants:
