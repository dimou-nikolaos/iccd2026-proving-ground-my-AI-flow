---
name: sva-assertion-writing
description: Templates and patterns for writing SystemVerilog assertions with bind for Verilator-compatible runtime checking
---

## Checklist: write assertions for a design

### 1. Identify bind targets
- Read design hierarchy: `docs/designs/<name>.md`
- Choose modules where important computations live
- Top module: protocol invariants, interface rules
- Submodules: signal definitions, next-state rules

### 2. Write assertion modules
One module per target. Port names MUST match signal names exactly (for `(.*)` binding).

### 3. Assertion priorities (highest mutation-kill value first)
- [ ] **Signal-definition**: `assert property (out == <spec formula>)` — kills arith, relational, constant mutations
- [ ] **Decode-definition**: `assert property (wr_reg[n] == (acc_wr && sel_ch[n] && creg == K))` — kills write-decode mutations at if-else chains
- [ ] **Signal mux**: `assert property (mux_out == (sel ? a : b))` — kills negate/relational on selectors
- [ ] **Next-state**: `condition |=> reg == expected` — kills off_by_one, relational, drop_term
- [ ] **Comparison-in-if (THREE paths)**: for every `if (A && (x > y)) reg <= val`:
      `A && (x > y) |=> reg == val` (true path)
      `!A |=> reg == $past(reg)` (kills `&&`→`||`)
      `A && (x <= y) |=> reg == $past(reg)` (kills relational mutation)
- [ ] **Persistence**: `wr && !strobe |=> (bits & $past(bits)) == $past(bits)` — kills missing-mask mutations
- [ ] **Protocol**: handshake, valid/ready, stability rules
- [ ] **Reset** (separate module, NO `disable iff`): EVERY signal in EVERY reset branch gets `!rst_n |-> reg == 0`
- [ ] **Boundary/range**: value limits, saturation trigger point

### 4. Template
```systemverilog
module <target>_sva (
  input logic clk,
  input logic rst_n,
  // signals referenced in properties — names must match target exactly
  input logic [W-1:0] signal_a,
  input logic [W-1:0] signal_b
);
  default clocking cb @(posedge clk); endclocking
  default disable iff (!rst_n);

  a_signal_def: assert property (signal_a == expected_fn(signal_b));
  a_next_state: assert property (cond |=> signal_a == correct_next);
endmodule

bind <target_module> <target>_sva u_<target>_sva (.*);
```

### 5. Reset check (separate module) — COMPLETENESS IS CRITICAL

For EVERY `always_ff @(posedge clk or negedge rst_n)` block, list ALL signals in the `if (!rst_n)` branch. Create an assertion for EACH. Missing any signal costs a mutation kill.

```systemverilog
module <target>_reset_sva (
  input logic clk,
  input logic rst_n,
  // EVERY signal from the reset branch goes here
  input logic sig_a,
  input logic [31:0] sig_b,
  input logic sig_c,   // ← the one you almost forgot to list
  input design_pkg::frame_cfg_t cfg_q  // struct: check one field
);
  // NO default disable iff
  a_reset_a: assert property (@(posedge clk) !rst_n |-> sig_a == '0);
  a_reset_b: assert property (@(posedge clk) !rst_n |-> sig_b == '0);
  a_reset_c: assert property (@(posedge clk) !rst_n |-> sig_c == '0);
  a_reset_cfg: assert property (@(posedge clk) !rst_n |-> cfg_q.width == '0);
  // SKIP memory arrays (write-before-read → equivalent)
endmodule
bind <target_module> <target>_reset_sva u_<target>_reset (.*);
```

**Tip**: grep for `<= '0` and `<= 1'b0` inside reset branches to enumerate every reset signal.

### 6. CR-specific assertions (optional separate file)
```systemverilog
// assertions/<top>_cr_sva.sv — new signals from change request
module <target>_cr_sva (...);
  // references new CR signals (won't compile if CR not implemented)
  a_cr_def: assert property (cr_signal == cr_formula);
endmodule
```

### 7. Verify
- [ ] Assertions compile with design (use Docker — never host tools):
```bash
docker run --rm -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-N:/design:ro --entrypoint bash proving-ground/flow:public \
  -c "cd /design && verilator --lint-only --assert -Wno-fatal --top-module <top> \$(cat rtl/filelist.f) assertions/*.sv"
```
- [ ] No assertion fires on fixed RTL (else: submission REJECTED)
- [ ] Run tests with assertions enabled → tests still pass
- [ ] Use `.sv` or `.v` extension (NOT `.svh` — flow ignores those)

## Verilator 5.048 SVA subset
Supported: `|->`, `|=>`, `##N`, `##[m:n]`, `##[m:$]`, `disable iff` (inline or `default`), named sequences/properties, `$rose`, `$fell`, `$stable`, `$past`, `$onehot`, `$onehot0`, `$countones`, `[*n]`, `[->n]`, `[=n]`, `throughout`, `intersect`, `first_match`, `until`, `and`, `or`, `implies`, `iff`, `assert property`, `cover property`, immediate `assert`

NOT supported: `s_eventually`, `always`, `nexttime`, `strong(...)`, `not` on sequences, `$changed` after delay (use `sig != $past(sig)`)

## Critical pitfalls

1. **`<=` inside assertion expressions is COMPARISON, not NBA.** For next-state checking, use `reg == f($past(inputs))` pattern. Example: to check `status_q <= f(input)` at posedge, write `status_q == f($past(input))` (post-edge value equals pre-edge function).

   1a. **Next-state identity (safest for counters/regs): align EVERYTHING one cycle back.** A reg's value sampled at posedge T was written at edge T-1 from signals sampled at T-1. So assert the unconditional identity `q == (enable@T-1 ? f($past(d)) : $past(q))` using `$past(enable)`, `$past(d)`, `$past(q)` — e.g. `frames_q == ($past(stat_clr) ? 0 : ($past(ev_frame) ? $past(frames_q)+1 : $past(frames_q)))`. The common self-fire bug: pairing `$past(ev)` with the CURRENT `clr`/`cfg` (mixed cycles) fires right after a clear or a config change. Use `$past(...)` on the enable/clear/config too, and on a value latched from a live input (`cfg_q.x == $past(cfg_x)`).

2. **`default disable iff (!rst_n)` applies to ALL assertions in the block, including reset-check assertions.** To have both normal and reset assertions in one module, use inline: `assert property (disable iff (!rst_n) expr)`. Omit `default disable iff` and add it per-assertion.

3. **SYNCASYNCNET warning**: assertion modules that use `rst_n` (async reset in design) trigger this Verilator warning. Wrap reset assertion modules in `/* verilator lint_off SYNCASYNCNET */ ... /* verilator lint_on SYNCASYNCNET */`.

4. **Width mismatches in replication**: `{4'h0, sig, 4'h0, sig}` = 16 bits, NOT 32. Always verify replication totals match target width. Verilator `-Wall` catches this.

5. **Parenthesize in assertion expressions**: `status_q == A | B` parses as `(status_q == A) | B` (comparison then OR), not `status_q == (A | B)`. Always parenthesize RHS.

6. **Interface/FSM invariants are high-yield BUT must be spec-guaranteed.** Patterns like `(valid && !ready) |-> $stable(data)` (data held during a stall) and `current_state != ILLEGAL_ENC` (no deadlock/unused state) kill control mutations cheaply, but they **self-fire on correct RTL if the spec doesn't actually promise them** (a DUT may legally change data mid-stall, or the "unused" encoding may be reachable) → automatic rejection. Before writing one, confirm the requirement in the spec; if unsure, make it a `cover` first, or gate on the precise protocol condition.

## Assertions and PPA — fact (don't over-constrain them)

Assertion files are compiled **separately with `verilator --assert`** for the public/evidence and mutation sims. The **PPA stage synthesizes `rtl/filelist.f` only — assertions are NEVER synthesized**, so they have **zero effect on area/timing/power (P)**. Do not weaken or avoid assertions "for PPA." Their only real cost is **simulation runtime**: heavier assertions inflate `evidence_seconds`, which sets the per-mutant timeout (`2·evidence+15`) — too-slow tests give each mutant less time and can mask subtle kills. So: write as many **bind-only** properties as help V, but keep them structurally cheap (one expression / short sequence per property, no big unrolled loops over internal signals).

## Decode assertion pattern (kills write-decode mutations)

For every `wr_reg[n] = cond_a && cond_b && (sel == CONST);` in always_comb write decode:
```systemverilog
a_wr_reg_def: assert property (
  wr_reg[0] == (cond_a && cond_b && (sel == CONST))
);
```
This kills ALL mutations to that decode line (constant ±1, `&&`→`||`, operator swaps).

## Strobe-gating assertion pattern (APB/register designs — high-yield, challenge-2)

For every `if (wr_reg && wstrb[K]) reg[byte] <= wdata[byte];` pattern, write THREE assertions:

```systemverilog
// 1. True path: write updates when strobe active
a_wr_hi: assert property (
  (wr_reg && wstrb[1]) |=> reg[15:8] == $past(wdata[15:8]))
  else $error("reg[15:8] not written");

// 2. Strobe guard: no update when strobe inactive (kills `wstrb[0]` → always-true)
a_hold_nostrb: assert property (
  wr_reg && !wstrb[0] |=> reg[7:0] == $past(reg[7:0]))
  else $error("reg[7:0] changed without strobe[0]");

// 3. Write-enable guard: no update when wr_reg inactive (kills `wr_reg` → always-true)
a_hold_noreg_wr: assert property (
  !wr_reg && wstrb[0] |=> reg[7:0] == $past(reg[7:0]))
  else $error("reg[7:0] changed on non-reg write");
```

**Challenge-2 result**: this 3-assertion pattern killed strobe_lane mutants at every strobe-gated write line in `pwm_channel`, `pwm_regs`, and `pwm_irq`. Combined with the strobe-gating **test** pattern (write with wrong strobe lane, assert unchanged; write with correct strobe, assert updated), these achieved 40/40 kill on one seed.

For strobe-masking in always_comb (e.g., `clr_vec = wr_status ? (wdata & strb_mask(wstrb)) : 0`):
```systemverilog
a_clr_vec_def: assert property (
  clr_vec == (wr_status ? (wdata & {{8{wstrb[3]}}, {8{wstrb[2]}}, {8{wstrb[1]}}, {8{wstrb[0]}}}) : 32'h0))
  else $error("clr_vec definition wrong");
```
Pins the entire strobe-mask expression in one shot.

## For deeper context
Read `docs/skills/sva-patterns.md` (12KB, full examples, bind to generate instances, struct ports, mutation kill mapping).
