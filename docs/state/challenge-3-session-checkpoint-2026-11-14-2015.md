# checkpoint 2026-11-14 20:15 — FINAL PPA PASS CLOSED (post-operator-notes triage)

## Pinned
- GOLDEN UNTOUCHED: runs/sub.tar.gz == runs/sub-c3-drop7v3.tar.gz md5 **092ca181e03abce1647667c1895e5d32**
  (V=1.0 + P=0.484, runs/out-c3-drop7v3-full/). Upload FIRST on server return.
- Operator v2 notes triaged → `docs/state/challenge-3-final-ppa-pass-2026-11-14.md` (tracker ALL rows
  ticked; that file = full s1..s13 results + verdict). This checkpoint supersedes 12:15 file.

## Final-pass results (all local, no uploads — server still down)
- s1 JALR-via-ALU P=0.4485 | s2 enable-strip P=0.4674 (p=+0.194 best power 13.87mW) | s4 branch-SUB P=0.4326 DEAD
- s3 REJECTED by counterexample (op_a_q not dead: fwd recapture during mdu_busy) | s8 valid-bits P=0.4454
  (best area 138660 but power backfire +22%: reset-less flops X-toggle vectorless → B1-full dead)
- s12 = s2+s8 stack: **P=0.4771 = best non-golden ever** (a−0.0201, p+0.0951, WNS 3.229 vs 4.47 needed)
  V=0.925; s12b same netlist P identical V=0.975 → PPA deterministic per NETLIST (seed rolls V only).
- s13 = s12+s1 P=0.4665. Campaign 24/24 distinct-netlist draws < 0.484 → **P=0.484 FINAL, NO more draws.**
- e1 (regfile clock gate) flow-REJECTED at build (derived clock) + 1 sim fail → E1 dead.
- Cycle-changers (D2/D3/F*/B3/E2/E3/A6) dead-on-arrival: our deterministic mcycle/minstret pins = hidden proxy.

## Explorer pool (upload one per 30-min cooldown AFTER drop7v3 C=1.0 banks; all Tier-C = C-unverified,
## drop7v3 is fallback if any tanks C): original 19 + s1 56efc312, s2 6cf72e7b, s4 1be2ed47,
## s8 59a120df, s12 f4189944, s12b cd00f267, s13 7d8a6b5f (list in ppa-plan). e1/lot4 EXCLUDED (invalid archives).

## Next action for operator
1. Server return → upload sub.tar.gz (drop7v3). C=1.0 → bank ~74.2.
2. Then drip explorer singles (26 candidates now; budget 30 subs incl. main = fits).
3. If C<1.0: failed-test names → v13 c8b50c69 (ALU-interface hedge) / drop2 c1e18428 (pre-PPA) / V-hedges.
4. NO new PPA/V/squeeze fronts. 24/24 + s12b determinism = closed. Nothing left but C + explorer drip.
