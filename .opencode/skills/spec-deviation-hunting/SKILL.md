---
name: spec-deviation-hunting
description: Checklist and heuristics for finding RTL deviations from specification by static analysis
---

## What I do

Scan RTL code against numbered specification requirements to find places where the implementation contradicts the spec.

## Detection patterns (scan for each)

### P1: Off-by-one at boundaries
- [ ] Find all `<`, `<=`, `>`, `>=`, `==` comparisons with constants
- [ ] Trace each to a spec REQ sentence
- [ ] Check: "exceeds X" → `> X`; "at most X" → `<= X`; "when X reached" → `>= X`; "at exactly X" → `== X`
- [ ] Look for saturation, counter limits, length checks

### P2: Missing qualification / mask
- [ ] Find uses of `wdata`/`rdata`/write-data in register logic
- [ ] Check: is a byte strobe (`wstrb`, `pstrb`) applied?
- [ ] Spec says "byte lanes", "strobe", "in enabled lanes" → mask MUST be present
- [ ] Look for: `data & mask` vs bare `data`

### P3: Missing configuration latch
- [ ] Find config inputs (`cfg_*`) used in computation blocks
- [ ] Spec says "sampled at first beat", "latched at frame start", "captured at start"
- [ ] Config should flow: `cfg_port → register (on trigger) → computation`
- [ ] If config port feeds computation DIRECTLY → deviation

### P4: Missing reset branch
- [ ] Find every `always_ff @(posedge clk ...)` block
- [ ] List all signals assigned in the `else` branch
- [ ] List all signals in the `if (!rst_n)` (or reset) branch
- [ ] Signal in else but NOT in reset branch → missing reset
- [ ] Cross-check with spec reset section (the spec's reset requirements typically)

### P5: Wrong operation ordering
- [ ] Find multi-step combinational chains (reflect + XOR, encode + mask, etc.)
- [ ] Spec gives numbered procedure: "do A, then do B"
- [ ] RTL must apply A before B (or produce equivalent result)
- [ ] Discriminating: find values where `A(B(x)) ≠ B(A(x))`

### P6: Extra boolean term
- [ ] Find `||` (OR) and `&&` (AND) in output/condition expressions
- [ ] Ask: "can every term be traced to a specific spec sentence?"
- [ ] Look for "shortcut" or "optimization" conditions not justified by spec
- [ ] `x >= y || count < x` — is `x >= y` justified? What does it change at the boundary?

## Output format

```markdown
| REQ | RTL Location | Spec says | RTL does | Pattern | Confidence |
|-----|-------------|-----------|----------|---------|------------|
```

Confidence: HIGH (clear contradiction), MEDIUM (suspect, need probe), LOW (subtle, might be correct)

## Key principles
- Spec is truth. RTL is the suspect.
- Seeded deviations are always single-line, local, in runtime logic (assign, always_ff, always_comb)
- The public testbench doesn't catch them — it doesn't exercise the discriminating scenario
- Each design has exactly 3 deviations (the design README usually states the count — use it as the exit target: if you find 3 HIGH that each map to a REQ, you likely have them all; more findings are probably correct readings of ambiguous spec).

## Confirm a deviation before fixing it (two-sided probe)
For each HIGH deviation, write ONE discriminating probe and check BOTH directions before committing the RTL fix:
- **FAILS on baseline** (confirms the deviation is observable), AND
- **PASSES on the proposed fix** (confirms the fix is correct, not just different).
If a probe passes on baseline it isn't discriminating; if it fails on the fix the fix is wrong. This two-sided check caught nothing-wrong fixes and validated all three deviations on challenge-1 (probe set 0/5 on baseline, 5/5 on fix). Keep probes throwaway (delete before packaging) — real coverage goes in `tests/` in Phase 3.

## Confidence → next step decision

| Confidence | Action |
|-----------|--------|
| HIGH (clear contradiction, can trace exact spec sentence vs code) | Skip formal/probe. Go directly to Phase 2 fix. Write deviation test. |
| MEDIUM (suspect, might be correct per spec ambiguity) | Write probe test to confirm observable behavior. |
| LOW (subtle, might be correct) | Use formal to prove equivalence or find CEX. |

**Key insight**: If the public TB passes on baseline AND the discriminating condition is never exercised (e.g., COMPARE >= PERIOD), confidence is HIGH without running formal. The public TB passing confirms the deviation is not in a commonly-hit path.

## Phase 1 closing: probe-per-REQ sweep (L1 in flow-lessons)

After the static analysis above, the count "exactly 3 deviations" can still miss one. **challenge-1's 4th seeded deviation (REQ-16 HALF_EVEN rounding) was caught only by writing one probe test per REQ** from spec text — not by hunting RTL line patterns.

**Procedure** (orchestrator handles this, but spec-analyst should output the REQ list to enable it):
1. Enumerate every REQ-xx from spec (`grep -oE 'REQ-[0-9]+' spec.md | sort -u`)
2. For each, write a ~10-line cocotb probe that exercises the spec's stated behavior with one specific input combination
3. Run all probes on baseline RTL
4. Any FAIL = new deviation found (even if static analysis missed it)

Time cost: ~1 hour for 30-50 REQs. ROI: catches the hidden 4th deviation. Also seeds Phase 3a's REQ coverage matrix.

## For deeper context
Read `docs/skills/bug-hunting.md` (12KB, full pattern examples with code, pattern catalog + detection heuristics).
