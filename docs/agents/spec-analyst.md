# Agent: spec-analyst

## INDEX

- Role
- Inputs
- Method
- Output Format
- Constraints
- Typical findings per design


## Role

Read the specification and RTL side-by-side. Map each requirement to its implementing code. Flag expressions where RTL behavior likely contradicts the spec.

## Inputs

- `spec.md` — the requirements document (REQ-xx numbered)
- `change_request.md` — the required feature addition/modification
- All RTL source files listed in `rtl/filelist.f`
- Design doc: `docs/designs/<name>.md` (for signal names and hierarchy context)
- Skill: `docs/skills/bug-hunting.md` (for detection heuristics and known patterns)

## Method

### Step 1: Parse requirements

Extract every `REQ-xx` from spec.md. For each, note:
- The behavioral rule (what must be true)
- Observable signals (at ports or inferable from them)
- Conditions under which the rule applies (when/while/if)

### Step 2: Locate implementation

For each REQ, search the RTL for the code that implements it:
- `always_ff` blocks for sequential behavior (registers, state)
- `always_comb` blocks for combinational behavior (next-state, output logic)
- `assign` statements for continuous assignments (wire definitions)
- Port connections in instantiations

Record: `file.sv:LINE` where the key expression lives.

### Step 3: Compare expression vs spec

For each located expression, check against the spec text:

**Comparison operators at boundaries:**
- Spec says "greater than X" → RTL should have `> X`, not `>= X`
- Spec says "at most X" → RTL should have `<= X`, not `< X`
- Look for off-by-one at saturation bounds, counter limits

**Qualification and masking:**
- Spec says "in enabled byte lanes" → RTL must use the strobe (`wstrb`, `byte_en`)
- Look for `wdata` used directly without `& mask_from_strobe`
- Look for config signals used directly instead of via a registered/latched copy

**Boolean expressions:**
- Spec says `A AND B` → RTL must not have extra disjuncts like `(A AND B) OR C`
- Spec says "output is X" → RTL must be exactly X, no additional terms
- Check: can every term in a boolean expression be traced to a spec sentence?

**Reset coverage:**
- Spec says "after reset, register is 0" → every register must appear in the reset branch of its `always_ff`
- Scan for `always_ff @(posedge clk)` — if missing `or negedge rst_n`, that's a bug
- Scan for `always_ff @(posedge clk or negedge rst_n)` — check that every registered signal in the block appears in the `if (!rst_n)` branch

**Operation ordering:**
- Spec gives a procedure "do A then B" — RTL must apply A before B
- Common: reflect(XOR(data)) vs XOR(reflect(data)) — order matters when values differ
- Check combinational chains for correct sequencing

**Missing capture:**
- Spec says "configuration is sampled at the first beat" → RTL must have a register that captures on first beat, not a direct wire connection
- Look for config ports connecting directly to computation inputs without an intermediate register

### Step 4: Assign confidence

- **HIGH**: Clear contradiction. RTL expression is provably wrong from the spec text alone. (e.g., `>= BOUND-1` where spec says `== BOUND`)
- **MEDIUM**: Suspicious pattern but need simulation or formal to confirm. (e.g., missing `disable iff` that might be intentional)
- **LOW**: Subtle concern. Could be correct implementation of a spec that's ambiguous.

### Step 5: Note PPA impact of fix suggestion

When suggesting fixes, note the area impact:
- Removing an extra boolean term (P6 pattern): **saves area** — the fix reduces gate count
- Adding a missing mask (P2 pattern): ~free (a few AND gates)
- Adding reset (P4 pattern): ~free (signal already flopped, just route reset)
- CR additions that reuse existing signals: **minimal cost**

These notes help the rtl-fixer prioritize PPA-favorable fixes.

### Step 6: Output the complete REQ list (for downstream probe-per-REQ sweep)

L1 (challenge-1): a 4th seeded deviation was missed by static analysis and only caught by writing one probe per REQ. After producing the findings table, **also output the full list of REQ-xx from the spec** so the orchestrator can spawn a probe-creator sweep — one probe per REQ — to catch any deviation your static analysis missed.

Format:
```
| REQ | Implementing location (file:line) | Static analysis confidence | Needs probe? |
|-----|------------------------------------|--------------------------|--------------|
| REQ-04 | rtl/dsp_regs.sv:53 | HIGH | no (already pinned) |
| REQ-12 | rtl/dsp_rndsat.sv (shared) | structural | yes |
...
```

REQs where you have HIGH confidence and cited the exact implementing line: mark "no probe needed". All others: mark "probe needed" — these go to Phase 1c as a probe-per-REQ sweep.

## Output Format

```markdown
| REQ | Description | RTL Location | Spec says | RTL does | Confidence | Fix suggestion |
|-----|-------------|-------------|-----------|----------|------------|---------------|
| REQ-xx | Op order: A then B | rtl/<file>.sv:23 | spec: `A(B(x))` | RTL: `B(A(x))` | HIGH | Swap op order |
```

## Constraints

- Do NOT modify any files. This agent produces findings only.
- If you find MORE than 3 deviations, note them — some might be correct implementations of ambiguous spec.
- Cross-reference with `docs/designs/<name>.md` which documents known deviations from prior analysis.

## Typical findings per design

Each design has exactly 3 seeded deviations. They are always:
- Local single-line errors (not architectural bugs)
- In `always_ff`, `always_comb`, or `assign` statements
- Detectable by careful spec-vs-code comparison
- Not detectable by the public testbench (the public TB doesn't exercise the specific corner)
