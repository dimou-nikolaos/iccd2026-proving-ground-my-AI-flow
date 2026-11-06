# SVA Assertion Patterns: Bind, Properties, and Verilator Subset

Reference for `@assertion-writer`. Covers the template patterns, bind mechanism, SVA operators available in Verilator 5.048, and the assertion strategies that maximize mutation kills.

## The Bind Mechanism

Assertions are standalone modules bound into the design via `bind`:

```systemverilog
module my_checks (
  input logic clk,
  input logic rst_n,
  // every signal referenced in the properties must be an input port
  input logic [7:0] counter,
  input logic wrap
);
  default clocking cb @(posedge clk); endclocking
  default disable iff (!rst_n);
  
  a_wrap_zero: assert property (
    wrap |-> counter == 8'd0) else $error("wrap without zero");
endmodule

bind my_counter my_checks u_checks (.*);
```

### How `(.*)` works
- Connects each port of `my_checks` to the signal of the same name in `my_counter`
- Port names MUST exactly match signal names in the target module
- Can reference internal signals (not just ports): if `my_counter` has a `wire cnt_next`, you can add `input logic [15:0] cnt_next` to the SVA module and it binds automatically
- Struct types work: `input design_pkg::token_t tok` binds to a signal declared as that type

### Binding to different module levels
One `.sv` file can have multiple assertion modules bound to different modules:
```systemverilog
module stream_top_sva (...); // top-level properties
module datapath_block_sva (...);   // internal datapath properties

bind stream_top stream_top_sva u_stream_top_sva (.*);
bind datapath_block   datapath_block_sva   u_datapath_block_sva   (.*);
```

### Binding to generate instances
```systemverilog
module channel_sva (...);
bind channel channel_sva u_ch_sva (.*);
// This binds to ALL instances of channel (generate for-loop creates 4)
```

## Assertion Categories and Templates

### 1. Signal-Definition Assertions (HIGHEST mutation kill value)

Assert that an output wire exactly matches the spec formula:
```systemverilog
// "raw output is EN AND (COUNT < COMPARE_A)"
a_raw_def: assert property (
  raw == en_q && (count_q < compare_act_q));
```

Why it kills mutants: any operator/operand change to the expression on that line breaks the equality. Checked every cycle, regardless of stimulus. This is the single most powerful assertion type.

### 2. Next-State Assertions

```systemverilog
// "if tick and COUNT >= PERIOD_A: COUNT becomes 0, load shadows"
a_wrapped_count: assert property (
  tick && en_q && count_q >= period_act_q |-> count_next == 16'd0);

// "if tick and COUNT < PERIOD_A: COUNT increments"
a_count_inc: assert property (
  tick && en_q && count_q < period_act_q |-> count_next == count_q + 16'd1);
```

Where `count_next` is the combinational next-value signal (from the always_comb block). If only a register exists (no separate next signal), use:
```systemverilog
a_count_inc_r: assert property (
  tick && en_q && count_q < period_act_q |=> count_q == $past(count_q) + 16'd1);
```

### 3. Protocol Invariants

```systemverilog
// APB: zero wait states
a_pready: assert property (pready);

// APB: pready is combinational (independent of psel/penable)
// Streaming: result holds while not accepted
a_stable: assert property (
  m_valid && !m_ready |=> m_valid && $stable(m_acc) && $stable(m_len) && $stable(m_status));

// Occupancy ↔ valid equivalence
a_valid_eq: assert property (m_valid == (occupancy != 3'd0));

// No write into full queue
a_no_overflow: assert property (res_wr |-> !res_full);
```

### 4. Reset Assertions (separate module, NO `disable iff`)

```systemverilog
module reset_sva (
  input logic clk,
  input logic rst_n,
  input logic [15:0] prescale_q,
  input logic gen_q
);
  // NO default disable iff — we WANT to check during/after reset
  a_reset_prescale: assert property (
    @(posedge clk) !rst_n |=> prescale_q == 16'd0 && gen_q == 1'b0);
endmodule
```

### 5. Boundary and Range Assertions

```systemverilog
// Counter never exceeds period
a_count_max: assert property (count_q <= period_act_q);

// Saturation triggers at correct value
a_sat_trigger: assert property (
  fire && tok.vld && !tok.sof && !sat_q && cnt_q == BOUND |-> sat_en);

// Status reserved bits
a_status_rsv: assert property (m_valid |-> m_status[3:2] == 2'b00);
```

## Verilator 5.048 SVA Subset

| Supported | NOT Supported |
|-----------|--------------|
| `\|->` (overlapping) | `s_eventually` |
| `\|=>` (non-overlapping) | `eventually [m:n]` |
| `##N` (delay) | `always [expr]` |
| `##[m:n]`, `##[m:$]` (range delay) | `nexttime` |
| `disable iff (expr)` (inline or `default`) | `strong(...)` |
| Named sequences and properties | `not` applied to a sequence |
| `$rose(signal)` | `$changed` after a delay in a sequence |
| `$fell(signal)` | |
| `$stable(signal)` | |
| `$past(signal)`, `$past(signal, expr)` | |
| `$onehot(expr)`, `$onehot0(expr)` | |
| `$countones(expr)` | |
| `[*n]` (consecutive repeat) | |
| `[*m:n]` (range repeat) | |
| `[->n]` (goto repeat) | |
| `[=n]` (non-consecutive repeat) | |
| `throughout` | |
| `intersect` | |
| `first_match` | |
| `until` | |
| `and`, `or`, `implies`, `iff` | |
| `assert property`, `cover property` | |
| Immediate `assert` (inside always blocks) | |

**Workaround for `$changed`**: write `sig != $past(sig)` instead.

**Inline `disable iff`** (alternative to `default disable iff`):
```systemverilog
a_check: assert property (@(posedge clk) disable iff (!rst_n)
  condition |-> expected) else $error("message");
```
Both forms work in Verilator 5.048.

## Reset Handling Strategy

| Goal | Method |
|------|--------|
| Most assertions: ignore during reset | `default disable iff (!rst_n)` |
| Check that registers ARE reset | Separate module without `disable iff`, explicit `@(posedge clk)` in property |
| Verify reset doesn't affect normal logic | Use `disable iff` + a separate probe test in cocotb |

## Critical Rules

1. **Assertion must NOT fire on correct RTL** — if it fires during the evidence stage, submission is rejected.
2. **Assertion fires → test fails → mutant killed** — this is the mechanism by which assertions kill mutants. `$error` is fine.
3. **Only reference signals available in the bound module's scope** — no hierarchical paths.
4. **`.sv` or `.v` extension** — the flow compiles `assertions/*.sv` and `assertions/*.v` but NOT `.svh`.
5. **One assertion module per target submodule** — clean scoping, easy `(.*)` binding.
6. **Name properties after the requirement**: `a_sat_only_beyond_max`, `a_valid_eq_occupancy`.
7. **CR-specific assertions referencing new signals go in a separate file**. They won't compile on baseline (signal doesn't exist), but the flow only compiles assertions against the SUBMISSION's RTL (which has the CR). This is safe and correct.

## Assertion File Layout

```
assertions/
├── <top>_sva.sv           # main file: top-level + submodule bindings
├── <top>_reset_sva.sv     # (optional) reset checks, no disable iff
├── <top>_cr_sva.sv        # (optional) change-request-specific properties
└── ...
```

Separate CR assertions into their own file:
```systemverilog
// assertions/cr_sva.sv — only works after CR is implemented
module channel_cr_sva (
  input logic clk, input logic presetn,
  input logic cmp_evt,  // new signal from CR
  input logic [15:0] count_q,
  input logic [15:0] compare_act_q,
  input logic tick,
  input logic [15:0] period_act_q,
  input logic en_q
);
  default clocking cb @(posedge clk); endclocking
  default disable iff (!presetn);

  a_cmp_evt_def: assert property (
    cmp_evt == (tick && en_q && count_q < period_act_q && count_q + 16'd1 == compare_act_q));
endmodule

bind channel channel_cr_sva u_ch_cr (.*);
```

## Mutation Kill Coverage by Assertion Type

For a single `assign out = a + b;`:
- Signal-definition assertion `out == a + b` kills: `+`→`-` (arith), `a+b` operands swapped (constant), `a-b` mutation
- Does NOT kill: mutations to the assign condition (if there's a conditional select), mutations to how `a` or `b` are computed (those need assertions at their source)

For `always_ff`:
- `if (en) q <= d;` — negate mutation (`if (!en)`) is killed by: `assert property (en |=> q == $past(d))`
- `q <= q + 1;` — constant mutation (`+1` → `+2`) is killed by: `assert property (en |=> q == $past(q) + 1)`
- Reset drop: `if (!rst_n) q <= 0;` → mutant `if (1'b0) q <= 0;` is killed by: reset assertion checking `!rst_n |=> q == 0`

## The "Every Cycle" Advantage

Unlike tests (which only check at specific moments), assertions with `default clocking @(posedge clk)` are evaluated EVERY cycle. A mutated expression that only executes once every 100 cycles will still be caught by an assertion if any test exercises that path. But an assertion that fires requires:
1. The antecedent of the implication must be true (the condition leading to that code path)
2. The mutated RTL must produce a different result than the spec formula

So: **assertions catch mutations on exercised paths; tests must exercise the right paths**. Good tests create diverse exercise patterns; good assertions define correctness for those patterns.
