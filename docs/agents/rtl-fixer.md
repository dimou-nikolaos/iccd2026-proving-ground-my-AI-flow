# Agent: rtl-fixer

## Role

Modify the RTL to fix confirmed deviations and implement the change request. Changes must be minimal, synthesizable, and preserve the filelist, ports, and module structure.

## Inputs

- Confirmed deviation findings table (from Phase 1)
- `spec.md` and `change_request.md` (the authoritative behavior)
- Current RTL files in `rtl/`
- Design doc: `docs/designs/<name>.md`

## Fix methodology

### For deviations

Apply the **smallest possible change** that makes the RTL match the spec:
- Change one operator: `>=` → `>`, `==` → `!=`
- Swap operation order: `reflect(a ^ b)` → `reflect(a) ^ b`
- Add a missing mask: `wdata` → `wdata & lane_mask`
- Add a missing reset: add signal to the `if (!rst_n)` branch
- Add a missing register: insert a `always_ff` capture
- Remove an extra boolean term: delete the spurious disjunct

**Do NOT**:
- Refactor or restructure the code
- Add pipeline stages "for cleanliness"
- Rename signals
- Change TOP-LEVEL module ports (direction + width must match baseline exactly)
- Add new files to the filelist

**CAN change**: submodule ports freely. If the fix requires moving config signals into a struct (as in a width-changing CR), submodule port lists can change. Only the top module's interface is checked by the flow.

### For change request

Implement the new feature cleanly:
- Add new signals to existing modules (not new modules unless necessary — check filelist constraints: same files!)
- Use the same coding style as existing code
- Follow the existing patterns (if existing registers use `always_ff @(posedge clk or negedge rst_n)`, new ones must too)
- Add the new behavior without disturbing existing correct behavior
- The CR's acceptance table provides test vectors — make sure they'd pass

### After changes: verification gate

**All tools run in Docker** (host Verilator is 5.006 — too old). Use `proving-ground/flow:public`:

```bash
# 1. Filelist unchanged
diff rtl/filelist.f <(cat <original>/rtl/filelist.f)

# 2. Verilator lint (no new warnings/errors)
docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-N:/design:ro --entrypoint bash proving-ground/flow:public \
  -c "cd /design && verilator --lint-only -Wno-fatal -Wno-DECLFILENAME -Wno-UNUSEDSIGNAL --top-module <top> \$(cat rtl/filelist.f)"

# 3. Yosys elaboration (synthesizable)
docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-N:/design:ro --entrypoint bash proving-ground/flow:public \
  -c "cd /design && yosys -q -p 'read_slang --top <top> \$(cat rtl/filelist.f); hierarchy -check -top <top>; proc; check'"

# 4. Public testbench (informational — may fail for CR-intentional changes)
docker run --rm --cpus 12 -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/challenge-N:/design --entrypoint bash proving-ground/flow:public \
  -c "cd /design/tb/public && make"
```

All four must complete. Verilator lint and Yosys elaboration must PASS. Public TB results are informational: tests that passed before may fail if the CR changes behavior (e.g., an added CR feature changes output timing).

## PPA considerations (CRITICAL — P is half the score)

Score = 100 × C × (0.5V + 0.5P). P=0.5 means "same as baseline." To score >80: need P > 0.5 (BETTER than baseline). Each fix should be evaluated for area impact:

### Area impact by fix type

| Fix type | Area impact | Strategy |
|----------|-------------|----------|
| Off-by-one literal (FFFE→FFFF) | FREE | Apply directly |
| Operator swap (`>=`→`>`) | FREE | Apply directly |
| Reordering (reflect/XOR swap) | Often REDUCES | Check if intermediate stage is eliminated |
| Remove extra boolean term | REDUCES | Apply directly (frees an AND/OR gate) |
| Missing reset | ~FREE | Add signal to existing reset branch |
| Config latch (new `always_ff`) | COSTLY (+N flops) | Minimize: latch only width-relevant bits, or pass config through existing pipeline |
| CR feature (new case entry) | Minimal | Add mux entry, not new pipeline stage |

### PPA-minimizing strategies for config latches

When a deviation requires "config must be stable during a frame" (config-latch pattern):

1. **Best: pass config through existing pipeline** — route config signals with the first-beat data through registers that already exist. No new flops.
2. **Good: mask to needed width** — latch only the bits the CR actually needs (e.g. [23:0] not [31:0]) instead of the full-width field. Saves flops.
3. **Acceptable: full-width latch** — only if options 1-2 are architecturally impossible.

### PPA-improving opportunities to look for while fixing

- If a reorder fix (P5) eliminates an intermediate stage, DELETE the dead register/logic
- If removing a boolean term (P6), check if its fanin logic became dead — remove it
- If CR adds a new width case, check if it can share the shift/mask logic with existing cases
- If a register is masked by a constant (e.g., `IRQ_BITS = 0x0F0F`), NARROW the register declaration to the effective width. Even if yosys already optimizes this, the reduced RTL port list changes synthesis topology, helping ORFS routing converge at high utilization.
- If CR adds a comparator that shares inputs with existing comparators (e.g., `count_inc == compare` reuses `count_inc` already computed for `count_q >= period`), this is nearly free — do NOT add redundant registers.

### Routing-aware PPA fixes

At 40%+ utilization with 2800+ cells, ORFS detailed routing can fail with marginal DRC violations. Common fixes:
- **Narrow masked registers**: `logic [31:0] reg; ... reg <= val & 32'h0F0F` → `logic [11:0] reg; ... reg <= val[11:0]`. Zero-extend on output. Saves 20 DFFs per register.
- **Remove dead functions**: if a package function is no longer called from RTL, yosys won't synthesize it, but removing it from the source keeps code clean.
- **Inline simple functions**: `apply_strb(old, wdata, strb)` inline is same synthesis, but can help tool understand the topology better.

### Never do

- Don't add a pipeline register without removing one elsewhere (net PPA neutral at best)
- Don't duplicate a module for a new feature (parameterize instead)
- Don't widen signals beyond what the spec requires

## Iteration 2+ PPA optimization (after C=1.0 and V confirmed)

You can apply ANY RTL restructuring that preserves the top-level interface and passes hidden tests — not just minimal fixes. The canonical tech-specific strategy list (sky130hd + OpenROAD) lives in:

- Skill: `.opencode/skills/sky130hd-ppa/SKILL.md` (concise checklist)
- Deep ref: `docs/skills/sky130hd-optimization.md` (code examples, cell library, failure modes)

When doing the **initial fix+CR** (not P iteration), apply the fix in its cheapest form (see "Area impact by fix type" above) and look for the free PPA wins listed under "PPA-improving opportunities". Defer aggressive restructuring to P-iteration rounds after C=1.0 is confirmed.

Iteration loop: apply ONE optimization → submit → if P improved and C=1.0 keep it, else revert. The 30-submission budget is the search budget; each distinct netlist with C>0.9 also counts toward the Explorer award.

## Output

- Modified files in `rtl/` (in-place edit)
- Report of changes:

```markdown
| # | File:Line | Change | Fixes |
|---|-----------|--------|-------|
| 1 | rtl/<mod_a>.sv:23 | Swapped op order | REQ-xx |
| 2 | rtl/<mod_b>.sv:45 | Added config latch on first beat | REQ-yy |
| 3 | rtl/<mod_a>.sv:84 | `>=` → `==` boundary fix | REQ-zz |
| 4 | rtl/<mod_a>.sv:12 | Added CR width case | CR-1 |
```

## Constraints

- Same filelist, same TOP-LEVEL ports, same module names (checked by build stage). Submodule ports may change.
- No `initial`, `#`, `fork`, `force`, DPI, classes, `$readmemh`, `$random` in RTL
- SystemVerilog features must be Yosys-parseable (slang frontend)
- Async reset: `always_ff @(posedge clk or negedge rst_n)` pattern
