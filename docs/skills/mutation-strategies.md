# Mutation Analysis: Engine Details and Kill Strategies

Reference for `@mutant-hunter`. Explains how the flow's mutation engine works, what it mutates, and how to write targeted evidence that kills specific surviving mutants.

## How the Mutation Engine Works

### Selection process
1. Parse submitted RTL with `pyslang`
2. Walk syntax tree collecting mutation sites (runtime contexts only)
3. For each class, generate candidate mutants
4. Shuffle within classes using seeded RNG (seed = SHA-256 of submission archive)
5. Interleave classes round-robin for balance
6. Filter: slang semantic validity check + Yosys equivalence proof
7. Select first 40 non-equivalent mutants

**Implications of the archive-seeded RNG (seeded by step 4):**
- **Deterministic per archive**: the same `tar` always yields the identical 40 mutants and identical survivors. Re-running an unchanged submission is wasted compute — there is no run-to-run V fluctuation.
- **Re-rolls on any change**: editing RTL, tests, or assertions **or even comments** changes the archive hash → a different 40-mutant set. So write behavior-based evidence, never target specific line numbers, and re-measure V after every change (a PPA refactor can move/drop mutants; a prior 40/40 does not carry over).
- **Typical V distribution** (measured across 20 challenge-2 archives with the same base evidence): V=0.975 most common (1 equivalent mutant, 60%), V=1.0 lucky draw (15–20%), V=0.925–0.95 unlucky draw (20–25% with 2–3 equivalent mutants). **Strategy**: after each PPA improvement, submit once; keep the archive with highest projective score (`V × 0.5 + P × 0.5`). Don't assume new content always improves V.
- **`surviving` list ≠ mutant count**: survivors are reported as unique `file:line` rows; several operators on one line collapse to one row. Read `mutants`/`killed` for V, and the equivalent-mutator filter (step 6) may leave fewer than 40 actual survivors even though `mutants=40`.

### What IS mutated (valid sites)

Only these contexts:
- `ContinuousAssign` (assign statements)
- `AlwaysBlock`, `AlwaysCombBlock`, `AlwaysFFBlock`, `AlwaysLatchBlock` (always blocks)
- `NetDeclaration` (assignments in net declarations)
- `HierarchyInstantiation` (port connection expressions)

### What is NEVER mutated

- Parameters, localparams, genvars
- Function bodies, task bodies
- Package declarations
- Initial/final blocks
- Generate constructs (if/for/case generate)
- Assertions (concurrent, immediate, property/sequence declarations)
- Type declarations, casts, range selects
- Event controls, bind directives, modports, typedefs
- **Left-hand side of assignments** (only RHS expressions mutated)
- Constant expressions (all identifiers are parameters)
- Mutations in modules not instantiated under the top

### Mutant lifecycle during scoring
1. All 40 mutants compiled into ONE Verilator model (schemata build) using `((pg_mut_sel == N) ? (mutated_expr) : (original))` guards
2. Your tests + assertions run with `+PGMUT=0` (sanity: no mutant, must pass)
3. Then with `+PGMUT=1` through `+PGMUT=40` (one mutant active per run)
4. A mutant is **killed** when: test fails, assertion fires (log detected), simulation crashes, or timeout
5. **Per-mutant timeout**: `min(300, max(30, 2 × evidence_seconds + 15))`

## The Six Operator Classes

### arith: arithmetic and bitwise operators
| Original | Mutated to |
|----------|-----------|
| `a + b` | `a - b` |
| `a - b` | `a + b` |
| `a * b` | `a + b` |
| `a & b` | `a \| b` |
| `a \| b` | `a & b` |
| `a ^ b` | `a \| b` |
| `a << b` | `a >> b` |
| `a >> b` | `a << b` |

### relational: comparison and logical operators
| Original | Mutated to |
|----------|-----------|
| `a < b` | `a <= b` or `a > b` |
| `a <= b` | `a < b` or `a >= b` |
| `a > b` | `a >= b` or `a < b` |
| `a >= b` | `a > b` or `a <= b` |
| `a == b` | `a != b` |
| `a != b` | `a == b` |
| `a && b` | `a \|\| b` |
| `a \|\| b` | `a && b` |

### constant: literal values
| Original | Mutated to |
|----------|-----------|
| Integer literal N | N+1 or N-1 (width preserved) |
| `'0` | `'1` |
| `'1` | `'0` |

### negate: conditions
| Original | Mutated to |
|----------|-----------|
| `if (cond)` | `if (!(cond))` |
| `cond ? x : y` | `!(cond) ? x : y` |

### drop_term: operand removal
| Original | Mutated to |
|----------|-----------|
| `A && B` | `A` (drops B) or `B` (drops A) |
| `A \|\| B` | `A` or `B` |
| Reset condition in `always_ff` | `1'b0` (makes reset unreachable) |

### off_by_one: comparison bounds
| Original | Mutated to |
|----------|-----------|
| `x > R` | `x > (R + 1)` or `x > (R - 1)` |

Note: only when R is NOT a plain literal. The engine skips plain-literal comparisons for off_by_one (they're handled by `constant` class).

## Kill Strategies by Operator Class

### Killing `arith` mutants

**Best method: signal-definition assertion**
```systemverilog
// Mutated line: assign result = a + b;
// Assertion pins the definition:
a_result_def: assert property (result == a + b);
```
Any time `result` is computed (that path is exercised), the assertion fires if the operator is wrong.

**Alternative: model comparison in test**
If the Python model computes the correct result and the test asserts `dut_result == model_result`, any arith mutation that produces wrong output at the port will be caught.

**Discriminating values**: for `+` vs `-`, almost any non-zero operand discriminates. For `&` vs `|`, need operands that differ (one bit set in one but not other).

### Killing `relational` mutants

**Best method: boundary test + next-state assertion**

```systemverilog
// Mutated line: if (count_q >= period_act_q) ...
// The mutation changes >= to > (relational)
// Kill: exercise count_q == period_act_q (the boundary)
a_boundary_wrapped: assert property (
  tick && en_q && count_q == period_act_q |-> count_next == 16'd0);
```

If the mutation is `>` instead of `>=`, then at `count_q == period_act_q` the condition is false → count_next != 0 → assertion fires.

**Test-based**: drive the design to exactly hit the boundary. The cycle-accurate model predicts the behavior at the boundary; if mutated RTL disagrees, the model comparison fails.

### Killing `constant` mutants

**Best method: signal-definition assertion pinning the constant**

```systemverilog
// Mutated line: count_next = count_q + 16'd1 → + 16'd2
a_count_increment: assert property (
  tick && en_q && count_q < period_act_q |-> count_next == count_q + 16'd1);
```

The assertion explicitly uses `16'd1`. If the RTL uses `16'd2`, the assertion fires.

### Killing `negate` mutants

**Best method: conditional next-state assertion**

```systemverilog
// Mutated line: if (en_q) → if (!en_q)
a_enabled_loads: assert property (
  en_q |-> load_happens == expected_behavior_when_enabled);
```

OR simply: any test that drives `en_q = 1` and exercises the enabled path will diverge from the model.

### Killing `drop_term` mutants

**For `A && B` → `A` (drops B)**:
- Drive `A = 1, B = 0`. Correct: condition is FALSE (both required). Mutated: condition is TRUE.
- Test checks the divergence.

**For reset condition → `1'b0`**:
- Reset assertion: `!rst_n |=> reg == 0`. If reset is unreachable, register keeps its old nonzero value → assertion fires.

```systemverilog
// Reset check module (no disable iff):
a_reset_works: assert property (
  @(posedge clk) !rst_n |=> counter_q == 8'd0);
```

### Killing `off_by_one` mutants

**Best method: boundary test at exact value**

```systemverilog
// Mutated line: sat = (cnt_q >= BOUND) → (cnt_q >= BOUND-1) or (cnt_q >= BOUND+1)
// Test: drive exactly BOUND bytes. Check that saturation does NOT occur.
```

Or assertion:
```systemverilog
a_sat_boundary: assert property (
  fire && vld && !sat_q && cnt_q == BOUND-1 |-> !sat_en);
```

## Choosing Assertion vs Test

| Situation | Choose |
|-----------|--------|
| Mutated expression feeds an observable output | Signal-definition assertion (kills unconditionally) |
| Mutated expression is in a rarely-exercised branch | Test (drives the specific condition that reaches that branch) |
| Mutated operator is in an `if` condition | Next-state assertion (pins what happens when condition is true) |
| Mutated constant is a counter increment | Assertion `q_next == q + correct_const` |
| Mutated term is in reset logic | Reset assertion (check `!rst_n |=> q == 0`) |

## The "already killed" false negative

Sometimes a survivor appears in the report but is actually killed by existing evidence. Causes:
- The mutant is only exercised by a code path that no test triggers (need new test stimulus)
- The assertion covering that line doesn't fire because its antecedent is never true (need more diverse stimulus)
- The assertion exists but wasn't compiled (wrong file extension, not in `assertions/` dir)

## Common surviving-mutant patterns

| Location pattern | Why it survives | Kill strategy |
|-----------------|-----------------|---------------|
| Mux/select: `assign x = cond ? a : b` (e.g. a datapath mux) | No assertion pins the mux expression; tests exercise both paths but no check distinguishes the wrong branch at the intermediate signal | Add signal-definition assertion on the mux output: `a_base_def: assert property (acc_base == (tok.sof ? init_l : acc_q))` |
| Status assignment block (e.g. a status-assign block) | Intermediate signal, not directly visible at output; mutations to status bits may not fail model comparison if tests don't hit the specific condition | Assert the status assignment: `status[SAT_BIT] == sat_en` |
| Handshake condition (e.g. `s_ready = !valid_q \|\| (fire && last)`) | Both branches produce correct observable behavior under most stimulus; only specific timing scenarios distinguish | Test that back-to-back beats are accepted when the previous beat's last token is consumed in the same cycle |
| FIFO full condition (e.g. `full = (count_q == DEPTH)`) | The full signal gates writes via another condition; mutation to `full` alone may not cause divergence unless the exact boundary cycle is hit | Assertion: `assert property (full == (count_q == DEPTH))` or test filling to capacity + one more write |
| Stats counter max/min (e.g. `if (ev && (len > max_q))`) | Off-by-one in max-tracking only visible if a frame has length exactly at the current max | Test with increasing frame lengths (each new frame is the new max) |
| Stats read mux (e.g. a stats read mux) | Mutation in one mux arm only shows if that particular `stat_sel` value is exercised | Test every `stat_sel` value (0-7) at least once |

**Key insight**: signal-definition assertions at intermediate points kill mutants that higher-level output assertions miss. Write assertions on EVERY non-trivial intermediate signal (muxes, conditions, counters), not just final outputs.

## V seed uncertainty — robust V beats lucky V

The mutant seed is derived from the **submission tar's content hash**. Changing ANY file (even adding one test) rerolls the seed and changes which 40 mutants get generated. **A local V=1.0 is a single-seed sample** — it does NOT mean the design has complete mutation coverage; it just means you got lucky that the 40 mutants drawn from that seed happened to all be killed.

**Strategic implication**: optimize for **V robustness**, not for any single-seed V value:
- A submission with 63 tests covering all 40 REQs is more robust than a 61-test submission even if the 61-test one shows V=1.0 locally and the 63-test one shows V=0.975. The 63-test submission kills more mutants on average across all possible seeds.
- Don't chase V=1.0 on a local seed by tuning tests to that seed. That's overfitting.
- **REQ-matrix completeness** is the right proxy for expected V — the spec is fixed, the mutant generator targets spec-implementing code, so REQ coverage correlates with mutation coverage.

**V ceiling (per design)**: there's always a small residue of equivalent mutants (mutations producing identical behavior under observable I/O — e.g., removing an async reset on a register that's only used inside an already-reset context, or replacing `(x != 0)` with `(x > 0)` for an unsigned signal). Accept ~0.975-1.0 as the realistic ceiling.

## Reading the report

```json
{
  "stages": {
    "mutation": {
      "V": 0.725,
      "details": {
        "survivors": [
          {"file": "rtl/<file>.sv", "line": 84, "id": 7},
          {"file": "rtl/<file2>.sv", "line": 23, "id": 12}
        ],
        "counted": 40,
        "killed": 29
      }
    }
  }
}
```

The report gives `file:line` but NOT the operator class. Infer it from the source code at that line.
