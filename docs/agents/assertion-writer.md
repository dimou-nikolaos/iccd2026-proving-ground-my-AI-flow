# Agent: assertion-writer

## INDEX

- Role
- Inputs
- PROACTIVE STRATEGY: enumerate before writing
- Assertion Categories (READ BEFORE WRITING)
- Bind Template
- Verilator SVA Subset (supported)
- Critical Rules
- Systematic Coverage Checklist (DO THIS FOR EVERY DESIGN)
- When to Accept Survivors: Semantically Equivalent Mutants
- Output Format
- Mutation kill effectiveness by category


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

## Assertion Categories (READ BEFORE WRITING)

**Pattern library with code examples: `docs/skills/sva-patterns.md` § "Assertion Categories and Templates" (11 categories).** Read that section before writing. Category names by mutation-kill effectiveness:

1. Signal-definition (HIGHEST kill value — pin every important computation)
2. Next-state
3. Protocol invariants
4. Reset (separate module, no `disable iff`)
5. Boundary / range
6. Persistence / write-scope (register interfaces — `$past()` guard)
7. Decode-definition (write decode lines, one assert per register)
8. Strobe check (wstrb gating)
9. Signal mux definition (kills negate on selectors)
10. Interface (MANDATORY top-level; defensive submodule)
11. Comparison-in-if guard — THREE-ASSERTION pattern

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

### Per-pattern checklists

- **Comparison-in-if guards**: write the THREE-ASSERTION pattern (category 11 in `docs/skills/sva-patterns.md`) — one assert per branch outcome (true / comparison-false / guard-false).
- **Signal muxes (ternary selects)**: write a definition assertion `mux_out == (sel ? a : b)` (category 9 in `docs/skills/sva-patterns.md`) — pins select condition and both branch values.

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
