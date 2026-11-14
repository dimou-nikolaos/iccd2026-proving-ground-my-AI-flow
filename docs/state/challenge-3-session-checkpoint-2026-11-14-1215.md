# challenge-3 session checkpoint — 2026-11-14 12:15 (compaction survival pin)

## BANKED GOLDEN (md5 pins)
- `runs/sub.tar.gz` == `runs/sub-c3-drop7v3.tar.gz` md5 **092ca181e03abce1647667c1895e5d32**
  FULL-FLOW verified same-roll: **V=1.0 (0 survivors), P=0.484** (area 144312 µm², WNS 4.469 ns,
  17.117 mW; baseline 136024.0/4.69101/17.2019 — cache verified) → `runs/out-c3-drop7v3-full/`.
  Projected score **74.2 × C**. Public TB pass, 95/95 suite, 0 assertion fires.
- Same-netlist seed hedges (only report.md comment differs): drop7a a759cfb8 (V=0.95),
  drop7v4 c9b7211f (0.975), drop7v2 e37d4e58 (0.925), drop5a 9f478b66 (V=1.0, 91-test era,
  report says 85 — cosmetic). RTL of ALL drop-5/6/7 archives byte-identical to banked v1 text.

## SERVER-RETURN ACTION (operator uploads)
sub-1 = runs/sub.tar.gz (drop7v3). C=1.0 → bank. C<1.0 → failed hidden-test names localize;
drop2 c1e18428 (pre-PPA RTL) bisects the v1 delta (only netlist change vs drop2).

## STATE OF PLAY (evening plan 2026-11-14 — all phases done)
1. V classification DONE: fetch:67 drop_terms equiv; decode:178 was SYSTEM f3=100 illegal arm →
   kill test test_req34_illegal_system_f3_100 (word 0x00004073!); muldiv:37 b_signed drop_term
   EQUIV (MUL low word congruent); store uses_rs1 killable only via mcycle delta==6 pin →
   test_req13_store_base_interlock_cycle (hand-verified). Suite 95/95. All roll survivors map to
   documented equivalent families.
2. PPA CLOSED at 0.484: text lottery 0/5 (lot4 = yosys SYNTH FAIL on dup-assign — never do;
   lot1/2/3/5 losses), reset-strip line 0/4 (v6 0.4774 best, v6a rerun 0.4313 — lottery noise,
   non-monotone), v7t JALR speculative target adders 0.4585 (cone NOT mux-depth-limited →
   Tier-C speculation abandoned), v7 cross-module sharing analyzed-skipped.
3. Explorer staging list in evening-plan file (distinct-netlist archives; submit only after main
   C banked, one per cooldown window; lot4 EXCLUDED — synth-fail).
4. ISS 5th-deviation backstop closed earlier (26/26 lockstep). Deviation hunt closed at 4.

## OPERATING RULES RE-LEARNED THIS SESSION (do not repeat mistakes)
- Appending to a RUNNING bash queue script does NOT reliably execute the append → start a fresh queue.
- Overwriting a submission tar while its flow run is in flight can corrupt the run (v6a route-fail).
- Variant stacker base MUST be the unmodified live tree (base=variant silently reverts edits).
- Duplicate continuous assigns on `logic` vars: Verilator OK, yosys SYNTH FAIL — never ship.
- cocotb programs need finish() sentinel else run_checked times out at 12000 cycles.
- SYSTEM f3 field = bits[14:12]: f3=100 ⇒ bit14 set ⇒ word 0x…4073 (0x1073 is f3=001).

## FILES
- Plan+tracker: docs/state/challenge-3-evening-plan-2026-11-14.md (all TODO rows resolved except
  server-dependent C-checks) | longhorizon pointer: updated to drop7v3
- PPA ledger: docs/designs/challenge-3-ppa-plan.md | REQ matrix: 95 tests mapped, complete
- Tooling: scripts/stack-variants.py (base caveat noted), scripts/pgdocker.sh, scripts/mutant-report.py,
  /tmp/repack-any.py, scripts/repack-c3.py; PPA pattern: pristine mount + --cache + baseline==136024.0 check.
