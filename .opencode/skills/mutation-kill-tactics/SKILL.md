---
name: mutation-kill-tactics
description: Decision tree and recipes for killing specific surviving mutants from the flow report by operator class
---

## Decision tree

Given a surviving mutant at `file.sv:LINE`:

1. **Read the line at that location** in the RTL
2. **Identify the expression** being mutated
3. **Infer the operator class** from what's on that line:
   - `+` or `-` → arith (swap)
   - `<`, `<=`, `>`, `>=`, `==`, `!=`, `&&`, `||` → relational (neighbor)
   - Integer literal → constant (±1)
   - `if (cond)` or ternary condition → negate
   - `&&` or `||` with 2 operands → drop_term
   - Comparison against a non-literal bound → off_by_one
4. **Choose kill method** (see recipes below)

## Per-operator recipes

### arith (`+`↔`-`, `*`→`+`, `&`↔`|`, `^`→`|`, `<<`↔`>>`)
**Kill**: signal-definition assertion
```
a_<signal>_def: assert property (<output> == <correct expression>);
```

### relational (`<`→`<=`, `==`→`!=`, `&&`→`||`)
**Kill**: exercise the boundary where old/new disagree
- `<` → `<=`: drive `x == y` (correct: false, mutated: true)
- `==` → `!=`: drive the case where `x` equals `y`
- `&&` → `||`: drive `A=1, B=0` (correct: false, mutated: true)

### constant (literal N → N±1)
**Kill**: pin the computation
```
a_increment: assert property (next == cur + <correct_constant>);
```

### negate (`if (cond)` → `if (!cond)`)
**Kill**: any test exercising `cond=true` → next-state assertion or model comparison

### drop_term (`A && B` → `A`; reset → `1'b0`)
**Kill**: 
- For `&&`/`||`: test with one operand 0, one operand 1; check correct output
- For reset: `assert property (!rst_n |=> reg == 0)` in reset-check module

### off_by_one (bound R → R±1)
**Kill**: test at exactly R (boundary). Correct behavior differs from mutated at the boundary value.

### strobe_lane (wrong byte lane checked: `wstrb[0]` → `wstrb[1]`, or condition removed)
**Kill**: write to the register with WRONG byte lane strobe (e.g., `strb=0x2` when register uses `wstrb[0]`). Assert register unchanged. Then write with correct strobe and assert updated.
- Test pattern: `write(REG, value, strb=0x2); assert(read(REG) == old_value)`
- Assertion pattern: `a_wr_strobe: assert property(wr_reg[n] == (acc_wr && sel && (creg == K)))` — pins the entire decode including strobe check

### address_boundary (`paddr range check` off-by-one)
**Kill**: read/write at the edge addresses. For `paddr[4:2] <= REG_MAX`: test at offset `REG_MAX+1` (unmapped, expect PSLVERR=1).
- Test pattern: `read(BASE + REG_OFFSET_INVALID); expect PSLVERR=1`

## Assertion vs test: which to use?

| Situation | Prefer |
|-----------|--------|
| Mutated expression feeds output directly | Assertion (signal-definition) |
| Mutation in rarely-hit branch | Test (drives the condition) |
| Mutation in `if` condition | Next-state assertion |
| Mutation in counter increment | Assertion pinning increment value |
| Mutation in reset branch | Reset assertion |

## Multiple kills from one assertion

A signal-definition assertion like `a_raw == en_q && (count_q < compare_act_q)` kills ALL mutations on that expression (operator swap, operand change, negate). Write assertions for major computations and you kill many mutants at once.

## Comparison-in-if: the three-assertion pattern (PROACTIVE — write these FIRST)

For every `if (A && (x > y)) reg <= value;` in the design, write THREE next-state assertions:

```systemverilog
// True path: kills mutations that prevent update when they shouldn't
a_true: assert property (A && (x > y) && !clr |=> reg == $past(value));
// False path (condition met but A false): kills && → ||
a_A_guard: assert property (!A && (x > y) && !clr |=> reg == $past(reg));
// False path (A true but comparison false): kills > → >=, > → ==, etc
a_cmp_guard: assert property (A && (x <= y) && !clr |=> reg == $past(reg));
```

This is the #1 missed pattern. Write these DURING Phase 3b, not reactively after seeing survivors.

## Semantically Equivalent Mutants (UNKillable — accept these)

Some mutations produce identical observable behavior. You CANNOT kill them. Recognize and accept:

| Mutation | Why equivalent | How to spot |
|----------|---------------|-------------|
| `&&` → `&` (1-bit operands) | Identical for single-bit | Handshake gates: `valid && ready` |
| `\|\|` → `\|` (1-bit operands) | Same | Status bit OR |
| `!x` → constant `1'b0` in reset of FIFO memory | Write-before-read means stale values never read | `for(i) mem[i] <= '0` loops |
| `>=` → `>` where both paths write same value | Saturating counter with `reg = MAX` | `if (x >= MAX) reg = MAX` |

**If an assertion you wrote SHOULD kill a mutant but it survives**: the mutation is likely equivalent. Check the mutated expression — if it only changes logical operators to bitwise (or vice versa) on 1-bit operands, STOP. Report as inherent survivor.

**Before iterating**: compute expected max V = (total - equivalent_count) / total. If you've killed all killable mutants, stop.

## Verification loop

```
1. Read surviving mutant list from report
2. Read the RTL at each survivor line — infer operator class
3. Check: is it semantically equivalent? (see table above). If YES → skip, accept.
4. For killable survivors: write assertion/test (see recipes)
5. Verify new evidence passes on fixed RTL (Docker) — **`rm -rf sim_build` first** (Verilator caches and reuses `Vtop` otherwise):
   docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
     -v $PWD/challenge-N:/design --entrypoint bash proving-ground/flow:public \
     -c "cd /design/tests && rm -rf sim_build && make RTL_ROOT=/design ASSERTIONS=1"
6. Repackage: tar czf runs/sub.tar.gz -C <dir> rtl tests assertions report.md
7. Run: docker ... --skip ppa
8. Check: V improved? Survivor gone?
9. If not killed and NOT equivalent: try different approach (test vs assertion)
10. Escalate after 3 failed attempts on same mutant
```

**Stopping criterion**: when all killable mutants are dead (remaining are equivalent), STOP. Expected V ceiling is documented in `docs/designs/<name>.md`.

## V ceiling assessment (DO THIS EARLY)

**The mutation set is DETERMINISTIC per submission archive**, seed = `f(sub_hash)`:
- The **same tar** → the **same 40 mutants + same survivors** on every run. Re-running an unchanged archive is wasted compute (no averaging; no run 3-5 times).
- Any **RTL change** changes the hash → a **different 40-mutant set**. So write **behavior-based** evidence (never target a specific line number — it moves), and **re-measure V after every RTL change** (a PPA refactor can move or drop mutants; a set may even be 40/40 while a later one is not).
- **Test/assertion changes ALSO re-roll the seed** (the hash covers the entire archive including tests/, assertions/, report.md). A test-only change can draw a set with MORE equivalent mutants than the previous set, dropping V from 1.0 to 0.975. **Do NOT assume that adding tests can only help V** — it's a coin flip. Only add tests for C-confidence (hidden test pass rate), not to chase V.
- A survivor row is a unique `file:line`; several operator mutants can collapse onto one line, so `surviving` rows ≠ mutant count. Use the `mutants`/`killed` counts for V, not the survivor-row list length.
- Document specific equivalent locations in `docs/designs/<name>.md`. Focus effort on **breadth of behavior coverage**, not on one seed.

**Common equivalent mutant patterns (unkillable)**:
| Pattern | Why | Location |
|---------|-----|----------|
| Masked register bits (e.g., IRQ_BITS & 0xF0F slice at [7:4]) | Always 0 by design mask | Any masked register update expression |
| `default` case value in unreachable mux branch | Branch never taken in any legal access | Read mux default, case default |
| Signal driven only during non-checked phase | TB never samples during that phase | `prdata` outside access phase |
| Dead-code default / write overwritten before any read (`q<=X` where a later always-loaded path sets `q` before `q` is read) | Value never observed | Final-entry branches; init-before-overwrite |
| Operand guarded by a design invariant (`x && !full` where the producer never asserts `x` while `full`) | Mutating the guarded operand is unreachable | Write-enable / handshake gating |
| Async reset on a register that is functionally cleared on the first cycle after reset (e.g., `pcount_q` cleared by `!en_q || update` rule, and `en_q` resets to 0 via its own async reset) | Removing the async reset branch still yields correct value by the next clock edge | `if (!rst_n) reg <= 0` where a next-state path also forces 0 |

## PPA routing failure (blocks scoring — not a V issue)

If a full flow fails at `place and route` with DRC violations: reduce cell count (narrow masked registers, remove dead logic, share comparators). The fix list and diagnosis are in the `sky130hd-ppa` skill — that is the canonical PPA reference. This skill is for V only.

## For deeper context
Read `docs/skills/mutation-strategies.md` (~10KB: full operator tables, schemata build details, equivalence checking, timeout formula).
