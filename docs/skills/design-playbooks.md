# Design-Archetype Playbooks (per competition design type)

Pick the playbook for the released design in Phase 1 and load the matching tactic sections into the test-writer and assertion-writer task cards. Three designs ship: **datapath** (Nov 7), **control + bus** (Nov 10), **pipelined core** (Nov 13). This is a *routing* doc — it maps each archetype to the mutation classes to expect, the highest-yield assertions/tests, and the PPA levers that pay off. Technique mechanics live in the focused skills (`spec-deviation-hunting`, `sva-assertion-writing`, `cocotb-test-writing`, `mutation-kill-tactics`, `sky130hd-ppa`); this doc says which to pull and in what order.

The six flow mutation classes are fixed (`arith`, `relational`, `constant`, `negate`, `drop_term`, `off_by_one` — see `docs/skills/mutation-strategies.md`). "Expect these mutations" below maps the archetype's RTL features onto those classes.

---

## 1. Datapath block (arithmetic / streaming / transformation)

**RTL signature:** wide combinational math, variable shifts, saturators, deep operator chains, streaming handshakes; little state.

**Mutations to expect (operator class → where):** `arith` (+/-/&/|/^/<</>>) in every math node; `constant` in shift amounts, saturation limits, bit-slice bounds; `off_by_one` at saturation/overflow/length edges; `relational` in overflow/underflow compares; `drop_term` in sign-extension / mask terms.

**Golden model:** event-driven pure function `(inputs,config)->outputs` compared at the result/transaction boundary; add a **fixed-latency check** (a datapath pipe has constant latency N — assert output[k] corresponds to input[k-N], don't assume variable state).

**High-yield assertions (see sva skill):**
- Signal-definition pins on every major output math node (`out == <expression>`) — kills the most at once.
- Next-state identities on accumulators/saturators with the `$past` one-cycle-back pattern (pitfall 1a).
- Boundary pins for saturation/overflow (`(a OP b) outside range |-> sat==1`) — only if spec-guaranteed.

**PPA levers that pay:** move per-transaction config-derived shift/mask out of the hot path to load time (big power win); strip async reset from data-path-only flops/arrays (big area win — one at a time, can nudge timing); parenthesize adder trees; **split the longest combinational path with a register pipeline** (challenge-1: 2-stage gain pipeline gave +2ns WNS which became +0.06 P — the biggest single P delta in the design). (Challenge-1 example: P 0.45→0.57.)

**PPA levers that DIDN'T pay (challenge-1, measured):**
- Spec-minimum width narrowing: FIR accumulator 40→35 bits increased area +656 µm², degraded WNS and power (P −0.014). Synthesis maps odd-width logic worse than power-of-2 widths.
- Buffer elimination: removing input FIFO (DEPTH=2→0) degraded WNS by 1.65ns (s_ready combo path). P −0.005.
- Stage fusion: fusing decim rndsat + limiter (BOUNDED param) degraded WNS by 1.34ns (variable comparator in fusion path). P −0.023.
- Aggressive parallelism: 8-multiplier parallel FIR (replacing iterative MAC) crashed CTS at 2.5× baseline cell count. PPA-failed = not even a valid Explorer candidate.

**Conclusion for datapath designs**: timing slack is the dominant P contribution (t-weight ≈ 1.2 vs a-weight ≈ 0.27, p-weight ≈ 0.28). **Protect WNS aggressively.** P gains come from pipeline splits (creating slack), not from micro-optimizations that trade timing for area.

**V ceiling note:** datapaths max out cheaply — signal-def + next-state pins kill nearly all mutants; residual survivors are usually provably-equivalent (dead write, invariant-gated operand). See mutation skill.

---

## 2. Control + bus interface (FSM / arbiter / register map / APB-AHB-AXI)

**RTL signature:** dense state-transition graph, address decode + byte strobes, protocol handshakes (valid/ready, PSEL/PENABLE), status/IRQ regs; minimal math.

**Mutations to expect:** `negate` on state-transition / handshake conditions; `relational` + `off_by_one` on address-range and strobe-lane decode; `drop_term` on `valid&&ready` gating and on reset/clock-enable gating; `constant` on state encodings and register addresses; `arith` mostly absent. `strobe_lane` class (wrong byte-lane check, or strobe condition removed entirely) — challenge-2 confirmed this is a high-frequency class in APB register-file designs.

**Test strategy — permutation sweep, not long data streams:**
- Sweep the **control-signal permutation space**: back-to-back stalls, `valid` held across many `!ready`, consecutive/concurrent resets, invalid protocol bursts, reset mid-transaction.
- **Address boundary** tests: last-valid offset, first-invalid offset (expect error/PSLVERR). Kills decode `off_by_one`.
- **Strobe-lane discrimination**: write a register with the WRONG byte lane (assert unchanged), then the correct lane (assert updated). Kills `wstrb` mutations. **Challenge-2 pattern:** for each writable multi-byte register (PRESCALE, CH_PERIOD, CH_COMPARE) AND for control regs with single-strobe gating (CTRL gated by `wstrb[0]`, UPDATE gated by `wstrb[0]`), write with wrong strobe and read-back to confirm no change. One test covering 5–6 registers catches most strobe_lane mutants in one shot.
- Full register-map sweep: every address, W1C/W1S, reserved-bit behaviour, PSLVERR.
- **IRQ/interrupt coverage**: configure events, verify set/clear/enable/masked for both CR-original and CR-added bit groups. W1C with specific strobe lanes.

**High-yield assertions:**
- **Write-decode pins** (sva skill decode pattern) — one pin per register `wr_reg[n] == (sel && strobe && enable)` kills the whole decode line.
- **Strobe-gating 3-path pattern** (sva skill strobe pattern, challenge-2): for each `if (wr_reg && wstrb[K]) reg <= wdata`, write true-path (update), strobe-guard (hold when strobe inactive), and wr-guard (hold when write-enable inactive). These 3 assertions per register collectively kill the entire strobe_lane mutation class.
- **Handshake stability** `(valid && !ready) |-> $stable(payload)` and no-drop-while-valid — **ONLY if the spec guarantees the DUT holds data during a stall** (else self-fires; confirm first).
- **FSM safety**: `state inside {legal_set}` / `!(illegal)` — **ONLY if that encoding is truly unreachable**; otherwise make it a `cover`. Illegal-state self-fire = rejection.
- **Reset-check module** per `sva-assertion-writing` §5 (control blocks have many state regs to pin).
- **Signal-definition pins** for event generation: `set_vec == {cmp_evt, 4'h0, period_evt}`, `irq == |(status_q & enable_q)`, `clr_vec == (wr_status ? wdata & mask : 0)`.

**PPA levers:** narrow unused register/reserved fields to actual widths; homogeneous reset style (avoid mixed async/sync per module — fragments layout); clean `if(en)` for ICG inference; shared comparators in decode. Timing usually easy → focus area.

**Proven stacked PPA recipe (challenge-2, P 0.48→0.4965):**
1. **ICG-friendly 3-branch flop** on every counter with distinct load/step/hold conditions: `if (load) q<=init; else if (step) q<=next; else hold;`. Buys WNS slack (challenge-2: +0.36 ns). Apply FIRST.
2. **Shorten hot comparators** using invariants (e.g., `count_q >= period_q` → `count_q == period_q` when `count_q` never exceeds `period_q`). Saves cells AND shortens critical path.
3. **AND-OR read mux with one-hot decode** (replace `unique case` with `assign rd = (dec[0] & v0) | (dec[1] & v1) | ...`). Big area win (challenge-2: −241 cells) BUT WNS regresses — bundle with (2) to recover.
4. **Operand isolation on idle adders**: `in_iso = en ? in : 0; sum = in_iso + 1` when adder is enabled <50% of cycles. Area+power win.

Apply in this order; timing slack enables area wins. Never reverse.

**PPA ceiling (challenge-2 measured):** control+bus designs under ~3000 cells hit P ceiling near **0.50** (parity with baseline) with the stacked recipe above. CR-added feature logic (comparators, IRQ flops) is spec-mandated area cost that cannot be recovered. **Do not chase P > 0.50 on control+bus designs.** Budget 5–6 submissions on PPA, then move to C/V confidence and next design.

---

## 3. Small pipelined core (fetch/decode/execute/wb, regfile, hazard unit, cache)

**RTL signature:** both control and datapath; inter-stage pipeline registers, forwarding/hazard logic, memory, PC/branch.

**Mutations to expect:** `negate` on hazard/forward-detect and branch-condition terms; `relational`/`off_by_one` in dependency compares and branch/offset sign-extension; `constant` on opcodes/immediates/PC addends; `arith` in ALU + PC+imm; `drop_term` in stall/flush gating and writeback-enable.

**Test strategy — Random Instruction Stream (RIS) + trace compare:**
- Drive legal + semi-random instruction sequences (back-to-back loads/stores, RAW/WAW chains, taken/not-taken branches, branch-in-delay, dependencies across all stage distances) into the RTL **and** an ISA/reference-model; compare **architectural state** (full register file + PC + memory) at writeback, not just a value per cycle.
- Target the hard corners: RAW with/without forwarding, load-use stall, branch mispredict flush, `x0`/zero-reg never written, sign-extended immediates and branch offsets, memory alignment.

**High-yield assertions (bind to inter-stage pipeline registers):**
- **Hazard/forward pins**: RAW between stages `((dec_rs==exe_rd) && exe_wr && dec_rs!=0) |-> stall_or_forward` — pin the exact hazard expression.
- **PC invariant**: `(!stall && !branch_taken) |-> next_pc == pc + <stride>` (spec stride).
- **Zero-register**: never write to reg 0; **writeback gating** pins on the regfile write enable expression.
- Next-state identities on each pipeline register (the `$past` pattern) — a wrong flush/enable shows up immediately.

**PPA levers:** the ALU/forwarding muxes are the power/area hot spots — balanced mux trees for forwarding select, operand-isolate stages when stalled (register-with-enable, don't add bare flops), strip resets from stage *data* registers kept valid by stage-valid flags. Watch the forwarding mux depth on the critical path.

---

## Cross-archetype reminders (already in the core flow — restated because they bite hardest here)
- Assertions are `bind`-only in `assertions/`, **never synthesized → no P impact**; their cost is sim runtime → per-mutant timeout. Write what helps V, keep each property cheap.
- Only assert **spec-guaranteed** properties. Handshake-stability, no-illegal-state, and datapath-bound invariants all self-fire on correct RTL when the spec doesn't promise them → rejection.
- Every interface invariant/illegal-state assertion must pass on your fixed RTL with `ASSERTIONS=1` on a fresh `sim_build` before packaging (orchestrator gate).
- Expect the seeded deviations to be local single-line runtime edits (all archetypes); public TB won't exercise the discriminating case.