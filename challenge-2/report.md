# challenge-2 submission report — dma_ctrl

## Deviations found and fixed (baseline → submission)

| REQ | File | Baseline behavior | Spec-required fix |
|---|---|---|---|
| REQ-24 | dma_arb.sv | `>=` comparison made the LAST eligible channel win among equal PRIO | first channel in rotating order (after LAST_GRANT) wins |
| REQ-18 | dma_engine.sv | LEN=0 descriptor performed a full read+write and CH_LEN wrapped to 0xFFFF | granted LEN=0 descriptor goes straight to S_END: no data access, LEN stays 0 |
| REQ-28/29/30 | dma_engine.sv | `m_bresp` never inspected: failed write responses counted as success (addresses advanced, chain continued, DONE set) | non-OKAY `m_bresp` → S_ERR: stop with ERR, no DONE, SRC/DST/LEN keep values |
| REQ-31 | dma_engine.sv | abort path (S_STOP) asserted DONE | abort sets neither DONE nor ERR (a failing last word routes via S_ERR) |
| REQ-27 | dma_engine.sv | HW_REQ chain descriptor ends pulsed dack twice (S_END + S_RELEASE) | exactly one dack per burst that moved ≥1 word (chain-continuing ends pulse at S_RELEASE only) |

## Change request CR-D2 implemented

- `IRQ_STATUS[11:8]` DESC[n] (W1C, reset 0), `IRQ_ENABLE[11:8]` DESC_IE[n] (RW).
- Engine `ev_desc` pulses once per completed descriptor (programmed block, zero-length,
  chain-mid and chain-last); never on error-stopped or aborted descriptors.
- Fetch-failure acceptance case: DESC stays set from the completing descriptor's S_END
  cycle while S_ERR adds ERR without DONE.
- W1C / enable lane behavior for bits [11:8] follows pstrb[1] via the existing mask path;
  REQ-33/34/35 apply unchanged (`IRQ_MASK` widened to 0xFFF).

## Evidence

- `tests/`: 58 cocotb tests — per-REQ functional (REQ-01..36), APB4/AXI4-Lite interface
  legality, outstanding-transaction/reset-race coverage, CR-D2 acceptance, plus long
  randomized soak.
  58/58 pass on the submission RTL; 16 fail on the released baseline (every known
  deviation plus the CR-D2 additions — no spurious failures either direction).
- `assertions/`: 1264 lines of SVA — APB/AXI protocol legality (REQ-03/04/06/07/13/14),
  engine state entry/exit golden defs (REQ-12..28), grant golden model (REQ-24),
  dack definition (REQ-27), chan sticky/abort (REQ-31/32), irq W1C/set-beats-clear/irq
  equation (REQ-33/34/35), DESC events (REQ-36). All sampled per clock edge; 0 fires
  across public, deviation-probe, and full evidence runs. Nine one-line mutation
  fixtures (`neg/`, not shipped) each fire at least one assertion.
- REQ coverage matrix: every REQ has ≥1 test and ≥1 assertion.

## PPA

The shipped RTL additionally contains PPA restructurings that preserve behavior:

- `dma_arb.sv`: grant network rewritten as parallel max-of-4 priority +
  barrel-rotate + first-match encoder (functionally identical to the REQ-24 fixed
  rotate-and-compare; the golden grant SVA and rotation soak tests pass unchanged).
- `dma_engine.sv` / `dma_ctrl.sv`: unreachable `default:` arms removed from the
  register-read case (spec leaves unmapped-word reads unspecified; observable
  behavior of mapped reads unchanged).
- `dma_engine.sv`: precomputed per-channel `ch_len==0` / `<=1` flags replace repeated
  comparators.

Net effect locally scored (final route, vs baseline): area +0.2% (45804 vs 45710 µm²),
timing margin improved (final WNS +2.55 vs +2.37 ns), power −14% (5.69 vs 6.63 mW)
→ P ≈ 0.52 (timing is route-determined by the APB path in every variant). More
variants are tracked separately (all locally scored).
