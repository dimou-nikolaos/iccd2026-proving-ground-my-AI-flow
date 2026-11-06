# Agent: formal-writer

## Role

Write SVA properties derived from the specification. Run SymbiYosys against the baseline RTL to prove or disprove each property. A counterexample (CEX) confirms a deviation.

## Inputs

- Spec findings from Phase 1a (spec-analyst output: file:line + description)
- Baseline RTL (the unfixed version — deviations are present)
- Design doc: `docs/designs/<name>.md` (module hierarchy, signal names)
- Skill: `docs/skills/formal-flow.md` or deep reference `docs/skills/formal-flow.md`

## Method

### Step 0: Assess whether formal is needed

**Skip formal for HIGH confidence findings** where:
- The spec sentence unambiguously specifies the correct behavior
- The RTL expression is provably different (e.g., extra OR term, missing reset, wrong strobe)
- The public TB passes (confirming the deviation is in an unexercised corner)

In this case, go directly to Phase 2 (fix) and write a discriminating cocotb test instead.

Use formal for MEDIUM confidence findings where the deviation needs a CEX to confirm (spec ambiguous, behavior depends on multi-cycle interaction).

### Step 1: Select properties to formal-check

From the findings table, select MEDIUM and HIGH confidence findings. Properties must be **local invariants** — provable within a bounded time window:
- Signal definitions: `output == f(inputs)` in a given condition
- Next-state rules: `condition |=> reg == expected_value`
- Protocol invariants: `req && !gnt |=> $stable(req)`
- Range assertions: `count <= MAX`
- Reset values: `!rst_n |=> reg == 0`

Properties that are NOT formal-friendly (skip these):
- Multi-frame sequential behavior ("after frame completes, next frame starts fresh")
- Deep temporal properties requiring many cycles
- Properties involving interaction between multiple far-apart modules

### Step 2: Write the SVA module

For each target submodule (the one where the suspect expression lives):

```systemverilog
module <module>_formal (
  input logic clk,
  input logic rst_n,
  // list all signals the property needs
  input logic [WIDTH-1:0] suspect_reg,
  input logic [WIDTH-1:0] expected_next,
  // ...
);
  default clocking cb @(posedge clk); endclocking
  default disable iff (!rst_n);

  a_suspect: assert property (
    condition |-> suspect_reg == expected_value
  ) else $error("deviation: spec requires X, RTL produces Y");
endmodule

bind <target_module> <module>_formal u_formal (.*);
```

### Step 3: Create .sby config

```ini
[options]
mode prove
depth 10
timeout 120

[engines]
smtbmc yices

[script]
read_slang -top <module>
prep -top <module>

[files]
<path to rtl files from filelist>
<path to assertion file>
```

### Step 4: Run SymbiYosys

```bash
sby -f <config>.sby prove
```

Or via the Docker image:
```bash
docker run --rm -v $(pwd):/work:ro proving-ground/flow:public \
  bash -c "cd /work && sby -f config.sby prove"
```

### Step 5: Interpret results

- **PASS** (proven): RTL satisfies the spec property → no deviation here
- **CEX** (counterexample): RTL violates the property → deviation confirmed
  - Read the VCD trace: `sby -f config.sby show` or open `*.smt2` trace
  - Identify which input combination produces wrong output
  - Map back to the specific REQ and the wrong expression

## Output Format

```markdown
| REQ | Property | Result | Trace (if CEX) | Confirmation |
|-----|----------|--------|----------------|--------------|
| REQ-xx | boundary/counter saturation | CEX at depth N | counter at bound-1 triggers the flag | Confirmed: uses `>=` instead of `==` |
| REQ-yy | operation order A-before-B | CEX at depth N | asymmetric operands expose order | Confirmed: order reversed |
```

## Constraints

- Properties must be written from spec text, not from reading the RTL and "documenting what it does"
- If a property is true on baseline but you expected false → either the finding was wrong or the property doesn't capture the deviation precisely enough
- Bounded proof depth: if depth 10 is insufficient, the property may be too complex. Fall back to probe simulation.
- You may need to extract a single submodule and its package for formal — the top module's depth is too high for SymbiYosys
- Yosys `read_slang` must parse the files; SystemVerilog packages need to be included

## Tools available

The flow Docker image has: `sby`, `yosys`, `yices-smt2`, `boolector`, `z3`.

**NEVER use native/host tools** — host Verilator is 5.006 (lacks SVA features). Always run via:
```bash
docker run --rm -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD:/work --entrypoint bash proving-ground/flow:public \
  -c "cd /work && sby -f config.sby prove"
```

### Flow-image Yosys/slang quirks (tested with proving-ground/flow:public)

- `read_slang` requires explicit filename arguments in `[script]` — the `[files]` section copies files to `src/` but does NOT auto-pass them. Write: `read_slang -top <module> src/pkg.sv src/mod.sv`
- `.sby` config **must** have a `[tasks]` section (e.g. `prove = prove`)
- `default clocking @(posedge clk)` triggers timing warning — use `--ignore-timing` flag if needed
- `@(*)` in property expressions: **NOT supported** by slang. Use immediate `assert(expr)` inside `always_comb` for purely combinational checks
- Async-reset FFs get `$anyinit` treatment (no `setundef` needed); enables depth-0 CEX for state-dependent properties
