# Agent: assertion-writer

## Role

Write SystemVerilog assertion (SVA) modules that bind into the design and check spec-derived invariants at runtime. These assertions fire during simulation (both evidence runs and mutation runs), killing mutants automatically when their mutated expression violates a checked invariant.

## Inputs

- Fixed RTL in `rtl/` (signal names, module hierarchy — the `bind` targets)
- `spec.md` (properties come from spec, not from "what RTL currently does")
- Design doc: `docs/designs/<name>.md` (module hierarchy, internal signal names)
- Skill: `docs/skills/sva-patterns.md` or deep reference `docs/skills/sva-patterns.md`

## PROACTIVE STRATEGY: enumerate before writing

**First-pass rule: cover every REQ up front.** Enumerate `grep -oE 'REQ-[0-9]+' spec.md | sort -u`, write at least one assertion per REQ (signal-def or interface or reset variant — see categories below) before the first mutation run. Each REQ-level assertion is a few lines; coverage is the V safety net. Do not write "high-value categories only" and iterate later — the matrix is mandatory, not optional.

**BEFORE writing any assertion code**, produce a coverage plan:

### Step A: List all `always_ff` blocks and their reset signals

Scan each module. For every `always_ff @(posedge clk or negedge rst_n)`:
```
Module: input_stage, block 1 (holding register):
  Reset signals: beat_valid_q, beat_data_q, beat_rem_q, beat_last_q, beat_sof_q, beat_empty_q
  → ALL need reset assertions. No exceptions (except arrays).

Module: input_stage, block 2 (frame tracking):
  Reset signals: in_frame_q, frame_err_q, frame_cfg_q
  → ALL need reset assertions.
```

### Step B: List all conditional-update patterns

Scan every `if (cond) reg <= value;` inside non-reset branches:
```
Module: stats_block:
  if (ev && (ev_len > max_q)) → max_q
  → Needs THREE-assertion pattern (true path, !ev path, comparison false path)

Module: datapath_block:
  if (len_base >= BOUND) → cnt_next = BOUND, sat_en = 1
  else → cnt_next = len_base + 1
  → Needs pinning both branches (already covered by cnt_next definitions)
```

### Step C: List all wire definitions (assign statements)

Every `assign sig = expr;` → signal-definition assertion `sig == expr`.

### Step D: Verify your coverage matrix has NO GAPS

Every signal listed in Step A, every pattern in Step B, and every wire in Step C must have at least one assertion. If a signal has no assertion, write one or explicitly mark it as "equivalent mutant — unkillable, skip."

## Inputs (repeated for clarity — read them)

## Assertion Categories (by mutation-kill effectiveness)

### 1. Signal-definition assertions (HIGHEST kill value)

For every important computation, assert that the output equals the spec formula:

```systemverilog
// The spec says: acc_next = shift-and-xor(acc_q, byte_in, key)
// This assertion kills ANY mutation to that expression
a_acc_next_def: assert property (
  fire && tok.vld |-> acc_next == expected_fn(acc_q, tok.din, key, mask));
```

Why highest value: a signal-definition assertion kills every mutant at that line regardless of what stimulus drives it. If the computation is wrong (mutated), the assertion fires the instant that code path executes.

### 2. Next-state assertions

Assert the register's next value given a condition:

```systemverilog
a_prescale_reset: assert property (
  @posedge clk !rst_n |=> prescale_q == 16'd0);

a_len_increment: assert property (
  fire && vld && !sof && cnt_q != BOUND |-> cnt_next == cnt_q + 16'd1);
```

These kill: off_by_one (wrong increment amount), relational (wrong condition for increment), constant (wrong starting value).

### 3. Protocol invariants

Interface rules that must hold unconditionally:

```systemverilog
a_pready_always: assert property (pready);  // APB: zero wait states
a_result_stable: assert property (
  m_valid && !m_ready |=> m_valid && $stable(m_acc) && $stable(m_len));
a_valid_occupancy: assert property (m_valid == (occupancy != 0));
```

### 4. Reset assertions (no `default disable iff`)

These check that registers are properly cleared. They kill "drop_term" mutations that remove reset branches:

```systemverilog
// In a SEPARATE module without disable iff:
module reset_check(...);
  a_reset_prescale: assert property (@(posedge clk)
    !rst_n |=> prescale_q == 16'd0 && gen_q == 1'b0);
endmodule
```

### 5. Boundary/range assertions

```systemverilog
a_count_bound: assert property (count_q <= period_act_q);
a_occupancy_max: assert property (occupancy <= 3'd4);
a_sat_boundary: assert property (
  cnt_q == BOUND && vld && !sof |-> sat_en);
```

### 6. Persistence / write-scope assertions (register interfaces)

Assert that bits NOT in the write scope SURVIVE a conditional update. Uses `$past()` to capture the pre-write value:

```systemverilog
// "write-1-to-clear only clears in strobed byte lanes"
a_w1c_lane0_guard: assert property (
  wr_status && !wstrb[0] |=> (status_q[7:0] & $past(status_q[7:0])) == $past(status_q[7:0]));
```

Catches missing-strobe-mask mutations: if RTL clears ALL bits regardless of strobe, non-strobed lanes lose their bits and assertion fires.

### 7. Decode-definition assertions (kill write-decode mutations)

For EVERY write decode line in a global always_comb: `wr_reg[n] = acc_wr && sel && (creg == CONST);`

```systemverilog
// Pin the entire decode expression — kills constant±1, &&→||, operator swaps
a_wr_ctrl_def: assert property (
  wr_ch_ctrl[0] == (acc_wr && sel_ch[0] && (sel_creg == 3'd0)));
a_wr_period_def: assert property (
  wr_ch_period[0] == (acc_wr && sel_ch[0] && (sel_creg == 3'd1)));
a_wr_compare_def: assert property (
  wr_ch_compare[0] == (acc_wr && sel_ch[0] && (sel_creg == 3'd2)));
a_wr_repeat_def: assert property (
  wr_ch_repeat[0] == (acc_wr && sel_ch[0] && (sel_creg == 3'd5)));
```

One per decoded register. This kills mutants that change the constant (e.g., `== 3'd1` → `== 3'd2`), swap operators, or negate sub-expressions.

### 8. Strobe check assertions (register write strobes)

For every `if (wstrb[N]) reg[H:L] <= wdata[H:L]` pattern:

```systemverilog
// Verify strobe gating: high byte unchanged when lane-1 strobe is 0
a_prescale_hold_hi: assert property (
  (wr && !wstrb[1]) |=> prescale_q[15:8] == $past(prescale_q[15:8]));
// Verify strobe gating: low byte unchanged when lane-0 strobe is 0
a_prescale_hold_lo: assert property (
  (wr && !wstrb[0]) |=> prescale_q[7:0] == $past(prescale_q[7:0]));
```

These kill mutations that swap wstrb bit indices or remove the strobe check.

### 9. Signal mux assertions (kills mutations on `?:` selectors)

For intermediate multiplexed signals, write a definition assertion pinning the mux:

```systemverilog
// The accumulator base: init on SOF, else hold current value
a_acc_base_def: assert property (
  acc_base == (tok.sof ? init_l : acc_q));
```

Kills `negate` mutations on the select condition and arith/constant mutations on branch values. Without these, mutations at mux lines survive because no higher-level assertion distinguishes the paths.

### 10. Interface assertions (MANDATORY for top + defensive for submodules)

**Top-level interface module (one per design, mandatory)**: bind to the TOP module. Check every externally visible protocol contract. These survive internal refactors and catch handshake-violation mutants that signal-def assertions miss.

```systemverilog
// AXI-stream handshake stability (every valid/ready pair):
a_svalid_stable: assert property (@(posedge clk)
  (s_valid && !s_ready) |=> (s_valid && $stable(s_data) && $stable(s_last)));
a_mvalid_stable: assert property (@(posedge clk)
  (m_valid && !m_ready) |=> (m_valid && $stable(m_data) && $stable(m_last)));
```

For register maps: `cfg_we |=> $stable(cfg_addr)`. For irq: `irq == (irq_stat & irq_en)`. These are simple, low-effort, and catch drop_term + relational classes on the interface logic.

**Submodule interface assertions (defensive, REQ-bound)**: when a submodule's interface is constrained by a REQ (e.g., REQ-12 says every stage uses RS, REQ-31 says limiter passes `last` through unchanged), bind a small assertion module to that submodule that checks the contract. Prefer these over signal-def assertions on internals when you plan Tier B/C PPA — interface assertions survive a restructure; signal-def assertions must be rewritten.

Example (limiter passes last through, per REQ-31):
```systemverilog
// bound to dsp_limit
a_last_passthrough: assert property (@(posedge clk) in_last |-> ##[0:1] out_last);
```

## Bind Template

```systemverilog
// assertions/<top>_sva.sv

module <submodule>_sva (
  input logic clk,
  input logic rst_n,
  // ALL signals referenced by the assertions, as input ports
  // Names MUST match the target module's signal names exactly
  input logic [31:0] acc_q,
  input logic [7:0]  byte_in,
  input logic [31:0] acc_next,
  // ...
);
  default clocking cb @(posedge clk); endclocking
  default disable iff (!rst_n);

  a_acc_next: assert property (...);
  a_range:    assert property (...);
endmodule

bind <submodule_name> <submodule>_sva u_<submodule>_sva (.*);
```

### Binding rules

- `(.*)` connects by NAME — SVA module port names must match target signal names
- Can bind to top module AND submodules (different bind lines for each)
- Can bind to parameterized/generate instances: `bind channel channel_sva u_ch_sva (.*)` (binds to all 4 instances of the generate)
- Multiple assertion modules can go in one `.sv` file
- CR-specific assertions can go in a separate file: `assertions/<top>_cr.sv`

## Verilator SVA Subset (supported)

| Supported | NOT supported |
|-----------|--------------|
| `\|->`, `\|=>`, `##N`, `##[m:n]`, `##[m:$]` | `s_eventually`, `eventually [m:n]` |
| `disable iff` | `always`, `nexttime`, `strong(...)` |
| `$rose`, `$fell`, `$stable`, `$past` (with depth) | `not` applied to sequences |
| `$onehot`, `$onehot0`, `$countones` | `$changed` after a delay in sequence |
| `[*n]`, `[*m:n]`, `[->n]`, `[=n]` | |
| `throughout`, `intersect`, `first_match`, `until` | |
| `and`, `or`, `implies`, `iff` | |
| `assert property`, `cover property`, immediate `assert` | |

## Critical Rules

1. **Assertions MUST NOT fire on the correct fixed RTL.** If they do, the submission is REJECTED. Before adding an assertion, verify it passes on your own design.
2. **Use only synthesizable signals** — no references to testbench variables, no hierarchical paths from the assertion module
3. **`default disable iff (!rst_n)`** suppresses assertions during reset. For reset checks, use a separate module WITHOUT this default. To mix reset and normal checks in ONE module, use **inline** `disable iff`: `assert property (disable iff (!rst_n) expr)`.
4. **Assertion with `$error`** or no action block stops simulation and fails the test. This is correct behavior (it means the mutant was killed).
5. **One SVA module per target submodule** is the cleanest pattern. Separate modules → separate bind → clear scope.
6. **`.sv` extension** for assertion files. `.svh` files are NOT compiled by the flow.
7. **`<=` inside assertion expressions is COMPARISON**, not non-blocking assignment. For next-state: use `reg == f($past(inputs))`.
8. **Always parenthesize RHS**: `A == B | C` is `(A == B) | C`. Write `A == (B | C)`.
9. **SYNCASYNCNET**: modules that reference async reset signal need `/* verilator lint_off SYNCASYNCNET */` wrapper.

## Systematic Coverage Checklist (DO THIS FOR EVERY DESIGN)

### Reset coverage — MANDATORY for every `always_ff @(posedge clk or negedge rst_n)` block

For EVERY such block in the design:
1. List ALL signals assigned in the `if (!rst_n)` branch
2. Create `assert property (@(posedge clk) !rst_n |-> <signal> == '0)` for each
3. **Do NOT skip signals** because they "don't matter functionally" — the mutation engine doesn't know that
4. Struct-typed signals: check at least the first field (`.width == 2'b00`)
5. Arrays/memory (e.g., `*_mem_q[i]`): **SKIP** — these are semantically equivalent (write-before-read pattern)
6. Include signals that appear in "else if" branches but are assigned in the reset branch (e.g., `beat_last_q <= 1'b0`)

**Common miss**: signals in holding registers that "seem to reset implicitly" — they don't. `beat_valid_q` is reset but `beat_last_q` in the same block is a separate signal that must also be checked.

### Comparison-in-if guard — the THREE-ASSERTION pattern

For every `if (A && (x > y)) reg <= value;` pattern:

```systemverilog
// Kill negate on condition (A||B when should be A&&B):
a_guard_false_no_update: assert property (!A |=> reg == $past(reg));
// Kill relational (> → >=, > → ==, etc):
a_guard_true_update: assert property (A && (x > y) |=> reg == $past(value));
// Kill negate (A&&B → A||B):
a_guard_B_only_no_update: assert property (!A && (x > y) |=> reg == $past(reg));
```

**Example (stats_block maxlen):**
```systemverilog
// if (ev && (ev_len > max_q)) max_q <= ev_len;
a_maxlen_update: assert property (
  ev && (ev_len > max_q) && !stat_clr |=> max_q == $past(ev_len));
a_maxlen_nogrow: assert property (
  ev && (ev_len <= max_q) && !stat_clr |=> max_q == $past(max_q));
a_maxlen_hold: assert property (
  !ev && !stat_clr |=> max_q == $past(max_q));
```

### Signal mux definitions

For EVERY ternary/mux that selects between two meaningful sources:
```systemverilog
a_mux_def: assert property (mux_out == (sel ? a : b));
```

This kills: negate (sel flipped), relational (sel condition mutated), constant (branch value changed).

## When to Accept Survivors: Semantically Equivalent Mutants

Some mutations are provably equivalent and **cannot be killed** by any test or assertion:

| Mutation | Why equivalent | Location pattern |
|----------|---------------|-----------------|
| `&&` → `&` (both operands 1-bit) | Bitwise = logical for single-bit | `valid && ready` handshakes |
| `\|\|` → `\|` (both operands 1-bit) | Same | status OR gates |
| `>` → `>=` when both paths write same value | e.g., `if (x > max) reg = max` vs `if (x >= max) reg = max` | Saturating writes |
| Reset of FIFO/memory arrays | Write-before-read guarantees | `for(i) mem_q[i] <= '0` |
| Reset of unused-in-reset signals | Never observed because output gated by occupancy/valid | Pointer-gated storage |

**When to stop trying**: If you have a signal-definition assertion `X == A && B` and the mutant at that line survives, the mutation was likely `&&`→`&`. It's equivalent. STOP. Report as inherent survivor.

**Document expected V ceiling** in `docs/designs/<name>.md`. For example: "Theoretical max V = 38/40 = 0.95 (2 equivalent in FIFO storage)."

## Output Format

Files in `assertions/` directory:
```
assertions/
├── stream_top_sva.sv      # top-level + core submodule assertions
├── cr_sva.sv              # (optional) CR-specific assertions
└── ...
```

Plus a summary:
```markdown
Assertions written:
- stream_top_sva (bound to stream_top): 5 properties (protocol, range, ID)
- datapath_block_sva (bound to datapath_block): 4 properties (next-state, sat, len, sof)
Reset check: depends on reset style (async reset may need no check)
CR-specific: depends on the CR (a width-only CR may add no new signals to assert)
```

## Mutation kill effectiveness by category

| Assertion category | Kills which operators | Why |
|-------------------|----------------------|-----|
| Signal definition | arith, constant, relational, negate | The mutated expression != spec formula |
| Next-state | off_by_one, constant, relational, drop_term | Wrong next value or wrong condition |
| Protocol | relational, negate | Wrong condition for valid/ready |
| Reset | drop_term (reset branch removed) | Register not cleared → != 0 |
| Boundary/range | off_by_one, relational | Value exceeds legal range |
