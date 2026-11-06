# Formal Verification Flow: SymbiYosys Setup, Properties, and CEX Interpretation

Reference for `@formal-writer`. Covers how to run SymbiYosys for formal property checking against the RTL, what properties are amenable to formal proof, and how to interpret counterexamples as confirmed deviations.

## When to Use Formal Checking

Formal is most effective for **local invariants** — properties that constrain behavior within a few clock cycles:
- Signal definitions: `output == f(inputs)` in given condition
- Next-state rules: `condition |=> register == expected_next`
- Range assertions: `counter <= MAX`
- Reset values: `!rst_n |=> reg == 0`
- Protocol rules: `req && !ack |=> $stable(req)`

Formal is NOT practical for:
- Multi-frame sequential behavior (a transform accumulated over a whole message/frame)
- Deep temporal properties (needs > 20 cycle unrolling)
- Properties spanning multiple far-apart modules
- Designs with large state spaces (many registers)

## SymbiYosys (.sby) Configuration

### File format
```ini
[options]
mode prove
depth 10
timeout 120

[engines]
smtbmc yices

[script]
read_slang -top datapath_block
prep -top datapath_block

[files]
design_pkg.sv
datapath_block.sv
output_stage.sv
datapath_block_sva.sv
```

### Key options
| Option | Meaning |
|--------|---------|
| `mode prove` | Prove all assertions (can report PASS or CEX) |
| `mode cover` | Find traces reaching cover points |
| `depth N` | Unroll N clock cycles |
| `timeout S` | Per-engine timeout in seconds |

### Engines available in the flow image
- `smtbmc yices` (fastest for most cases)
- `smtbmc boolector` (alternative SMT solver)
- `smtbmc z3` (most powerful, slower)
- `abc pdr` (induction-based; good for invariants that hold at all depths)

## The Script Section

Must include:
1. `read_slang -top <module>` — parse RTL with slang SystemVerilog frontend
2. `prep -top <module>` — elaborate, flatten hierarchy to target, extract properties

If the target uses a package:
```
read_slang -top datapath_block
```
You must include the package file in `[files]` before the module.

If the target has a sub-dependency (e.g., `datapath_block` instantiates `output_stage`):
- Include all needed files in `[files]`
- `prep -top datapath_block` handles hierarchy flattening

## Writing Formal Properties

### Pattern 1: Signal definition (immediate-style, checked every cycle)
```systemverilog
module datapath_block_formal (...);
  default clocking cb @(posedge clk); endclocking
  default disable iff (!rst_n);
  
  // "length increments by 1 for each byte that is not start-of-frame"
  a_len_increment: assert property (
    fire && tok.vld && !tok.sof && cnt_q != BOUND
      |-> cnt_next == cnt_q + 16'd1);
endmodule
```

### Pattern 2: Reset value check (no disable iff)
```systemverilog
module presc_reset_formal (...);
  // Explicit clocking in property (no default disable iff)
  a_reset: assert property (
    @(posedge clk) !rst_n |=> prescale_q == 16'd0);
endmodule
```

### Pattern 3: Saturation behavior
```systemverilog
a_sat_boundary: assert property (
  fire && tok.vld && !tok.sof && cnt_q == BOUND-1
    |-> !sat_en);  // at BOUND-1, still not saturated
```

### Pattern 4: Output definition
```systemverilog
a_irq_eq: assert property (
  irq == |(status_q & enable_q));
```

## Running SymbiYosys

### From the flow Docker image
```bash
docker run --rm -v $(pwd):/work -u $(id -u):$(id -g) -e HOME=/tmp \
  --entrypoint bash \
  proving-ground/flow:public \
  -c "cd /work && mkdir -p sby_work && sby -f formal.sby"
```

### Directly (if sby installed locally)
```bash
sby -f formal.sby
```

### Interpreting output
- **PASSED**: all assertions proven correct at the given depth → RTL matches spec here
- **FAILED (CEX)**: a trace exists that violates an assertion → deviation confirmed
  - Look for `Trace` file (VCD or `trace.smt2`)
  - SBY reports which assertion failed: `a_sat_boundary: FAILED`
  - Read the trace: initial state + inputs at each cycle → identify the violating scenario

## Reading a Counterexample (CEX)

The CEX shows a concrete trace:
```
Cycle 0: rst_n=0, all regs = 0 (reset)
Cycle 1: rst_n=1, fire=1, tok.sof=1, cnt_next=1
...
Cycle 8: fire=1, tok.vld=1, tok.sof=0, cnt_q=BOUND-1, cnt_next=BOUND, sat_en=1
         ^^^ THIS IS THE BUG: at cnt_q=BOUND-1, saturation triggers (should trigger at BOUND)
```

**CEX interpretation for the findings table**:
- CEX cycle + signal state tells you the exact discriminator
- Map back to the REQ: "REQ-xx says a counter saturates only when it EXCEEDS its bound, but the RTL asserts the flag one increment early (at bound-1)"
- This confirms: HIGH confidence, specific fix needed (change `>=` to `==` or adjust bound)

## Depth Considerations

| Depth | What it catches |
|-------|-----------------|
| 1-3 | Combinational properties, immediate next-state, reset |
| 5-10 | Multi-step state sequences, register interactions within one module |
| 10-20 | Complex FSM transitions, counter wrap detection |
| 20+ | Multi-module interaction sequences (likely too slow) |

Start with `depth 10`. If a property is still unproven (timeout), increase depth or simplify the property.

## Bounded vs Unbounded Proof

- `smtbmc` with `depth N` is **bounded**: it proves the property holds for N cycles from all initial states. A property might fail at depth N+1.
- `abc pdr` is **unbounded** (induction): proves for all depths, but only works for inductive invariants.
- For deviation detection: bounded depth 10 is sufficient. The seeded bugs violate their property within the first few cycles of a triggering condition.

## Practical Workflow

1. Identify the submodule where the suspect expression lives
2. Write a minimal SVA module referencing signals available at that module's scope
3. Create `.sby` config listing only the files needed for that submodule + package
4. Set `mode prove`, `depth 10`
5. Run. If CEX → deviation confirmed, document the trace.
6. If PASS → the suspect expression might be correct. Move to probe test or re-examine.

## Limitations and Workarounds

| Issue | Workaround |
|-------|-----------|
| Module too large for formal (too many registers) | Bind assertions at submodule level, prove one submodule at a time |
| Package with complex types | Include package in `[files]`, slang parses it |
| `always_ff @(posedge clk or negedge rst_n)` | SBY handles async reset with `disable iff` or explicit model |
| Generate blocks | Bind to the generated instance; SBY treats each as separate |
| Properties that need > 20 cycles | Drop to probe simulation instead |
| Sby not installed locally | Use Docker: `docker run ... --entrypoint sby` |

## Yosys Formal Commands

If you need to run Yosys steps manually (debugging parse issues):
```bash
yosys -l yosys.log -p "read_slang -top datapath_block design_pkg.sv datapath_block.sv output_stage.sv; prep -top datapath_block; stat"
```

`stat` prints the elaborated design. If it errors, the file list or slang parse has issues — fix before running sby.
