# Bug Hunting: Seeded Deviation Patterns

Reference for `@spec-analyst`. The baseline RTLs contain seeded deviations that the public testbench does not catch. This document catalogs the patterns found and the detection heuristics.

## Pattern Catalog

### P1: Off-by-one at boundary comparisons

**What**: A comparison uses the wrong relational operator or wrong bound, shifting the trigger threshold by one.

**How it appears in RTL**:
```systemverilog
// Spec: "saturation when length exceeds BOUND" (i.e., when cnt_q == BOUND AND another byte arrives)
// BUG: uses >= where == is correct
assign sat_en = fire && tok.vld && cnt_q >= BOUND-1;  // wrong: triggers one byte early

// Correct:
assign sat_en = fire && tok.vld && cnt_q == BOUND;  // saturates only AFTER BOUND reached
```

Another example:
```systemverilog
// Spec: "tick when counter >= prescale"
// BUG: uses > instead of >=  (misses tick when counter == prescale)
assign tick = (cnt > prescale_q);  // should be >=
```

**Detection heuristic**:
- Find every integer comparison (`<`, `<=`, `>`, `>=`, `==`) in RTL
- Trace the bound to a spec REQ sentence
- Check: "at most X" → `<= X`; "exceeds X" → `> X`; "when X is reached" → `>= X`; "when X is exactly" → `== X`

### P2: Missing qualification / mask not applied

**What**: A signal path passes a value through without masking it with an enable/strobe that the spec requires.

**How it appears in RTL**:
```systemverilog
// Spec: "write clears bits where written bit is 1 AND byte lane strobe is 1"
// BUG: clear vector uses wdata directly, ignoring wstrb
assign clr_vec = wr_status ? wdata : 32'h0;  // should mask by wstrb

// Correct:
assign clr_vec = wr_status ? (wdata & {{8{wstrb[3]}}, {8{wstrb[2]}}, {8{wstrb[1]}}, {8{wstrb[0]}}}) : 32'h0;
```

**How to detect**:
- Grep for `wdata` without `wstrb`/`strobe`/`byte_en` in the same expression
- Spec says "in enabled lanes" or "byte strobes qualify" → mask must be present

### P3: Missing configuration latch (combinational pass-through)

**What**: Config inputs are used directly in computation instead of being registered at the start of a frame.

**How it appears in RTL**:
```systemverilog
// Spec: "configuration values used are those present at the first beat of the frame"
// BUG: config wires feed directly into the core's next-state logic
acc_next = transform_fn(acc_q, byte_in, cfg_key, cfg_w, ...);  // uses live config!

// Correct:
// Register cfg at first beat, use registered copy throughout the frame
cfg_reg <= cfg;  // only when sof
acc_next = transform_fn(acc_q, byte_in, cfg_reg.key, cfg_reg.width, ...);
```

**Detection heuristic**:
- Find spec REQs about "configuration sampling", "latched at first beat", "captured at start"
- Grep for config input names (`cfg_*`) in the computation blocks
- If they appear directly (not via a registered copy) → deviation

### P4: Missing reset branch / register not in reset list

**What**: An `always_ff` block has a reset branch but omits a signal that the spec says resets to 0.

**How it appears in RTL**:
```systemverilog
// BUG: prescale_q is assigned in the else branch but NOT in the reset branch
always_ff @(posedge clk or negedge rst_n)
  if (!rst_n) begin
    gen_q <= 1'b0;
    // prescale_q MISSING from reset list
  end else begin
    if (tick) prescale_q <= 16'd0;
    else prescale_q <= prescale_q + 16'd1;
  end
```

**Detection heuristic**:
- For each `always_ff @(posedge clk or negedge rst_n)` block:
  - List all signals assigned in the `else` branch
  - List all signals assigned in the `if (!rst_n)` branch
  - Any signal in the else branch but NOT in the reset branch → potential missing reset
- Cross-check with spec REQ about reset values

### P5: Wrong operation ordering

**What**: Two operations that should apply in a specific order (reflect then XOR, encode then modulate) are reversed.

**How it appears in RTL**:
```systemverilog
// Spec: "if reflect_out: acc = bit_reverse(acc); result = acc XOR out_mask"
// BUG: applies XOR before reflect
assign result = reflect(acc_q ^ cfg_mask, width);  // XOR then reflect (wrong)

// Correct:
assign result = reflect(acc_q, width) ^ cfg_mask;  // reflect then XOR
```

**Detection heuristic**:
- Find combinational assignment chains with multiple operations
- Trace against the spec's procedure (often numbered steps: "first do X, then do Y")
- Discriminating case: choose inputs where op(A,B) ≠ op(B,A) — e.g., asymmetric XOR mask

### P6: Extra disjunct / incorrect boolean expression

**What**: An output or condition has a boolean term that the spec does not justify.

**How it appears in RTL**:
```systemverilog
// Spec: "raw output is EN AND (COUNT < COMPARE_A)"
// BUG: adds shortcut for "always on" case as a separate OR term
assign raw = en_q && ((compare_act_q >= period_act_q) || (count_q < compare_act_q));

// Correct:
assign raw = en_q && (count_q < compare_act_q);
// (the "always on" case works naturally: if compare > period, count never reaches compare)
```

**Detection heuristic**:
- Find all `||` (OR) terms in boolean expressions that compute outputs/conditions
- Ask: "can I trace every term to a specific spec sentence?"
- If a term is an optimization/shortcut that changes boundary behavior → deviation
- Check especially: `COMPARE >= PERIOD` style "shortcut" conditions that alter the `==` case behavior

## Seeded Deviation Pattern Distribution (hints only — UPDATED with measured counts)

**Measured reality: NOT exactly 3.** challenge-1 seeded **4** (the 4th hid in plain sight — L1), challenge-2 seeded **5** (plus a 6th deviation judged not-worth-fixing and documented). Expect **3–6**; budget for 4–5. The pattern catalog below is a floor, not a ceiling: challenge-2's outstanding-commit deviation (REQ-27 class) did not fit P1-P6 cleanly — trust the per-REQ probe sweep (L1) over pattern-matching for completeness.

Hint distribution (still roughly true):
- Deviations use DIFFERENT patterns (no repeats observed)
- P1 and P5 appeared in at least one design so far
- Locations are always in one of: the datapath computation module, the input/capture stage, or the configuration module
- The REQ number gives a hint: low numbers (< 10) often involve reset/init behavior; mid-range (10-20) involve config latching; high (> 20) involve output/finalization order

## Key Principle

All seeded deviations are:
- **Single-line** (one operator, one expression, one missing branch entry)
- **Local** (don't require architectural restructuring to fix)
- **In visible runtime logic** (assign, always_comb, always_ff) — not in declarations or generate conditions
- **Not caught by public testbench** because the TB doesn't exercise the discriminating scenario

## Detection Strategy for Real Competition Designs

The seeded deviations in real designs will follow these same patterns. The agent should:
1. Enumerate all REQ-xx
2. For each REQ, find the implementing code
3. Apply the P1-P6 heuristics against each implementation
4. Flag HIGH-confidence findings
5. Confirm via formal (Phase 1b) or probe (Phase 1c)
