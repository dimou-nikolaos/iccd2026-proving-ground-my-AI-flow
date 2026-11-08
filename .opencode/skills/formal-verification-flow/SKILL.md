---
name: formal-verification-flow
description: Steps for running SymbiYosys formal property checking — .sby config, property writing, and CEX interpretation
---

## When to use formal

Prove/disprove LOCAL invariants within bounded cycles (1-20):
- Signal definitions, next-state rules, range checks, reset values, protocol invariants

NOT for: multi-frame sequential behavior, deep temporal (needs >20 cycles), multi-module properties.

**ROI caveat (L13 in flow-lessons)**: in our flow image, SBY formal has POOR ROI vs simulation next-state assertions for most properties. Flow Docker has SBY + slang, but slang's SVA support is limited (`|=>`, `|->`, `@(*)` rejected), and async-reset FFs use `$anyinit` so helper-reg patterns produce false-positive CEXs at step 1. A clean SBY run costs 1-2 hours of setup + iteration; the same property is pinned "for free" by a one-cycle-back identity assertion inside `always @(posedge clk)` that runs at every cycle of every cocotb test. **Default to simulation next-state assertions. Reserve formal for genuinely deep properties (>20 cycles) or unbounded induction.**

## When to SKIP formal (go directly to fix)

If spec-analyst found HIGH confidence deviations:
- Spec unambiguously states the correct behavior (direct text comparison sufficient)
- RTL expression is provably different (extra OR term, missing reset, wrong strobe index)
- Public TB passes (confirming the corner is unexercised)

In this case, formal is wasted time. Write a cocotb probe test instead (2 minutes) or go straight to Phase 2 fix. Use formal ONLY for MEDIUM confidence where you need a CEX to resolve ambiguity.

## Steps

### 1. Write SVA properties from spec
```systemverilog
module <target>_formal (...signals...);
  default clocking cb @(posedge clk); endclocking
  default disable iff (!rst_n);
  
  a_prop_name: assert property (
    condition |-> output == correct_value);
endmodule
bind <target> <target>_formal u_formal (.*);
```

### 2. Create .sby config
```ini
[options]
mode prove
depth 10
timeout 120

[engines]
smtbmc yices

[script]
read_slang -top <target_module>
prep -top <target_module>

[files]
<package>.sv          # needed if module uses package types
<target>.sv
<dependency>.sv       # submodules instantiated by target
<formal_assertion>.sv  # the file from step 1
```

### 3. Run SymbiYosys
```bash
# Native (if installed):
sby -f formal.sby prove

# Via Docker:
docker run --rm -v $(pwd):/work -u $(id -u):$(id -g) -e HOME=/tmp \
  --entrypoint bash proving-ground/flow:public \
  -c "cd /work && sby -f formal.sby prove"
```

### 4. Interpret results
- **PASSED**: property proven. RTL is correct at this invariant.
- **FAILED (CEX)**: deviation confirmed. Read the trace.

### 5. Read CEX trace
SBY outputs a trace file (VCD or SMT2). It shows:
- Which assertion failed
- The signal states at each cycle
- The exact input combination that violates the property

Map the CEX back to: "REQ-NN violated because RTL expression at file:line produces X when spec requires Y."

## Tips

- Start with `depth 10`. Most seeded bugs trigger within 5 cycles.
- If timeout: reduce scope (target smaller submodule) or increase depth.
- Package files must be listed BEFORE the module using them.
- `prep -top <mod>` flattens hierarchy to that module only — properties reference signals at that scope.
- Use `abc pdr` engine for induction (proves for all depths) when `smtbmc` times out.
- If a property PASSES on the buggy RTL, it doesn't capture the deviation. Refine or move to probe test.

## Engines available in flow image
| Engine | Best for |
|--------|----------|
| `smtbmc yices` | Default, fast for most |
| `smtbmc boolector` | Bitvector-heavy properties |
| `smtbmc z3` | Most powerful, slower |
| `abc pdr` | Induction (unbounded, but limited property classes) |

## For deeper context
Read `docs/skills/formal-flow.md` (10KB, full sby format, depth considerations, workaround patterns).
