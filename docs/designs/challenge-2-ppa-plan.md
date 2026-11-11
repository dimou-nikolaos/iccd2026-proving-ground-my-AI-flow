# challenge-2 (dma_ctrl) — PPA plan & wildcard ledger

Score model: P has terms for area, timing (dominant), power. Baseline cached in
`runs/challenge-2-baseline-out` (area 45710.1 um2, WNS +2.367 → period_min 7.63,
power 6.63 mW). t-slope ≈ +0.0098 P per 1% period_min improvement (measured).

## Timing walls (setup, all families)
1. `dreq` port → eligibility → arbiter compare/rotate → `ch_len_zero[grant_idx]` mux →
   engine state D. (abar2 WNS 2.546.) Input-delay 2 ns at head (io_pct 0.2).
2. `paddr` port → dma_apb decode → prdata read mux → output pad. (Exposed by W2b at
   slack 2.13; present in every family.)
Cutting BOTH collapses period_min toward the internal floor (~5.8–6.2 ns) → W6.

## Tier classification (per docs/flow-ppa-and-v.md)
- Tier A (free, bundled): lzero flag restructure, barrel-rotate arbiter, strobe-slim,
  gate-form rk encoder, keep-hint pruning. Mostly NEUTRAL-to-negative here — the design
  has no arithmetic to mine; edit-class micro-reshuffles LOSE (abar5 −0.022, abar8
  −0.017: placer shuffle penalty).
- Tier B (timing-neutral): `(* keep *)` len_q hints (abar9: area −135 um2 but WNS
  −0.14, net −0.0015).
- Tier C (structural, 1/submission): registered arbiter grant + commit guard (W2b,
  0.4974 — cut wall 1 but exposed wall 2 + 1010 um2).

## Champion
- **W8** family (abar2 + shared writeback datapath): **P=0.5212** (area 41521.1,
  WNS 2.061, 5.90 mW). Bank #1: c2-sub-w8. Prev champion abar2: 0.5184 (45803.9).
- Winning lever: REQ-13 single-outstanding engine ⇒ compute +4/−1 walk arithmetic ONCE
  for owned channel; banks load. −4283 um2 (−9.4%)! (est was 2.5k — yosys also folded
  the four le1 comparators and per-bank mux trees). WNS −0.49 acceptable (still +2.0).

## Wildcards tried (Phase 4e; doctrine: spec-silenced freedoms per docs/ppa-freedom-strategy.md)
| Move | Result | Verdict |
|---|---|---|
| W1 one-hot grant chain (REQ-23 "onehot" freedom) | P=0.5072 | LOSS: +1270 um2 off worst path; Explorer entry |
| W2b registered grant + commit guard (spec §11 latency freedom) | P=0.4974 | cut wall 1, exposed wall 2; Explorer entry, W6 base |
| W3 registered LEN-zero flags | P=0.4999 | Explorer entry (equiv-proven) |
| W5 `(* fsm_encoding one_hot *)` | P=0.5184, PPA byte-identical | flow IGNORES fsm attrs (L54) → netlist duplicate, dropped |
| W6 = W2b + `word_d` SETUP-boundary decode register | P=0.5082, V=1.0 (1st seed) | both walls cut, final WNS still 2.567 (L58 route pin); area +1010, pwr +0.39 → Explorer entry, not bankable |
| **W8 = shared writeback datapath (wb_* buses, (len<=1)=NOR(dec==0))** | **P=0.5212, V=1.0 (1st seed)** | **WIN — NEW BANK c2-sub-w8**; −4283 um2; WNS −0.49 OK |
| R1 = abar2 + reset-less internal engine FFs (11 FF) | P=0.5102, V=1.0 (1st seed) | area −251 µm² (45552, UNDER baseline) but WNS 2.118 place-dice loss → net −0.008; Explorer entry. Reset-slim saves ~250 µm²: BIG idea for challenge-3 datapath/pipeline regs |
| FSM one-hot manual (11 states → state_q 11b) | REJECTED | bind `.*` width check: hidden SVA declaring `[3:0] state_q` fails ELAB → C=0. Same-width re-encode: zero gain. attrs: ignored (L54). DEAD |
| Reset removal incl. data_q (32b) | REJECTED for bank | data_q drives m_wdata top pin → x after reset (hidden all-outputs-low check could read x → ValueError). Not worth +0.002 |
| e05/e06 buffer-entry variants | 0.5058/0.5055 | Explorer entries |
| keep0-2, arb2, a3/arb3/abar3 micro-families | 0.49–0.52 spread | Explorer breadth only |

## Rejected ideas (do not retry)
- `dreq_q` input pipeline: stale-dreq 1-cycle shadow after dack → spurious second burst
  (REQ-27). dreq sampling must stay live into eligibility/commit guard. (L57)
- Delaying `irq`: REQ-32 "cycle the event occurs" + REQ-35 f(registers) only. DEAD.
- APB wait states: REQ-03 pins pready=1 every cycle. DEAD.
- Channel storage sharing / register file: REQ-11/32/06 need concurrent visibility. DEAD.
- Overlapping AXI read+write: REQ-13 one-outstanding. DEAD.
- Wide-comparator flags alone (lzero): power +15%. AVOID.
- ICG insertion: no wide clock-gated idleness in 4-channel interleaved engine; skipped.

## C-risk ledger for W6
- Hidden test driving psel&&penable with NO preceding SETUP (non-APB4) diverges from
  baseline (word_d holds previous address). Public TB + 58-suite drive compliant APB.
- Hidden asserts binding dma_apb internals: baseline names kept live where possible
  (`word` stays combinational); decode OUTPUTS lag outside access only.
- Containment: W6 is an exploratory slot AFTER bank #1 banks C=1.0; costs 1 slot on miss.
