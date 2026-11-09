# RV32 core prep: per-stage contract SVA + retire-stream lockstep

Challenge-3 leverage. Two independent checking layers that can be written **before** the
core exists, because both are derived from the ISA rather than from a microarchitecture:

| Layer | Where | What it proves | Cost |
|---|---|---|---|
| **Per-stage contract SVA** | `rv_stage_sva.sv` (bound by `rv_stage_bind_selftest.sv`) | Every datapath value in every pipeline stage is the *only* value the ISA allows for the instruction word sitting in that stage, on every cycle | 0 extra cycles, catches a deviation the moment it is produced |
| **Retire-stream lockstep** | `test_stage.py` | The pipeline as a whole retires exactly the instruction stream a separately written ISS (`../rviss`) commits, in order, with the right writeback values | one commit-granular compare per retired instruction |

Why two layers and not one: a stage-local value check cannot see a pipeline that drops,
duplicates or speculatively retires an instruction (every stage is locally consistent while
the *sequence* is wrong) — the retire stream catches that. A retire-level check cannot see
a wrong value in a stage whose result is overwritten before commit, or a value that is
correct at writeback but wrong for three cycles in EX (which is what a mutation does) —
the stage assertions catch that. **Together they are what kills "silent" mutants**: the
class of mutant that keeps the interface and the final architectural state plausible.

## Files

| File | Role |
|---|---|
| `rv_ref_pkg.sv` | Pure ISA reference functions (ALU incl. M + CR-D3, immediates for all 5 formats, branch compare, load extension, legality, `writes_rd`, access size). No state, no timing, no microarchitecture. |
| `rv_stage_sva.sv` | The observer: all-`input` module, 20 concurrent assertions over a canonical *pipeline tap*. Parameters: `IALIGN32`, `CHECK_ILLEGAL`, `HAZARD_MODE`, `CR_D3`. |
| `rv_stage_bind_selftest.sv` | The bind for the self-test only. On release day this file is replaced by a 20-line map to the real core's signal names; the observer never changes. |
| `stage_harness.sv` | A synthetic 5-stage in-order RV32I(+CR-D3) core with `BUG=` injection. **A test fixture, not a prediction** of the released core: its job is to prove the observer detects deviations and to give the tap map a shape to work from. Its ALU/load logic is written independently of `rv_ref_pkg` so the check is not circular. |
| `test_stage.py` | Positive suite: retire-stream lockstep vs the ISS on random hazard-dense programs (incl. CR-D3), plus a fetch-address/content check. |
| `test_negative.py` | One program that reaches every unit, run against 9 deliberately broken harness builds. |
| `dbg_operand_diff.py` | Diagnostic (`make dbg RVSEED=n`): prints ISS vs DUT operands at the first divergent commit, so you learn whether an *operand* or an *operation* is wrong. |

## Run (flow image only)

```bash
sudo -n docker run --rm -u $(id -u):$(id -g) -e HOME=/tmp \
  -v $PWD/prep:/prep -w /prep/rvcore --entrypoint bash proving-ground/flow:public -c "make"
```

| Target | Result (2026-11-08) |
|---|---|
| `make` | `TESTS=3 PASS=3 FAIL=0`, **no assertion fired**, 0.74 s wall (112 µs sim) |
| `make negative` | 9/9 injected deviations trip an assertion (`ALL NEGATIVE CASES PASSED`) |
| `make dbg RVSEED=7` | `no mismatch` |

The negative matrix — each row is one injected deviation and the assertion that caught it:

| `BUG=` | Injected deviation | Caught by |
|---|---|---|
| 1 | `addi`: `+` → `-` | `A_EX_ALU` |
| 2 | load immediate zero-extended instead of sign-extended | `A_DEC_IMM` |
| 3 | `bltu` compared as signed | `A_EX_BRANCH` |
| 4 | `min` computes `max` (CR-D3) | `A_EX_ALU` |
| 5 | write-enable not suppressed for `rd == x0` | `A_WB_WE` |
| 6 | `lh` zero-extends (loses the sign) | `A_MEM_LOAD_DATA` |
| 7 | fetch PC skips an instruction on redirect | `A_IF_PC_FLOW` |
| 8 | `rd` taken from the wrong instruction bits | `A_DEC_FIELDS` |
| 10 | `sub`: `-` → `+` | `A_EX_ALU` |

## Assertion inventory

| Id | Stage | Contract | Mutation class it kills |
|---|---|---|---|
| `A_DEC_FIELDS` | ID | `rs1/rs2/rd/funct3/opcode` come from the instruction word | relational/constant inside field wiring |
| `A_DEC_IMM` | ID | immediate == ISA decode of that format (all 5 formats) | sign-extension, bit-select, off-by-one |
| `A_DEC_LEGAL` | ID | illegal flag == ISA legality | missing/extra encoding (feature mutants) |
| `A_CRD3_LEGAL` | ID | CR-D3 encodings are **not** illegal (when the core claims them) | "feature not implemented" mutants |
| `A_EX_ALU` | EX | ALU result == f(opcode, funct3, funct7, a, b) for every encoding the subset covers (base, M, CR-D3) | **arith, relational, negate, constant, drop_term, off_by_one** — the single highest-yield assertion |
| `A_EX_BRANCH` | EX | branch decision == ISA compare | relational |
| `A_EX_TARGET_BRANCH` / `A_EX_TARGET_JALR` | EX | target == pc+imm (word aligned) / (a+imm)&~1 | off_by_one, arith |
| `A_MEM_ADDR` | MEM | address == base + ISA immediate | arith, wrong imm format |
| `A_MEM_SIZE` | MEM | access size == ISA size for that funct3 | constant |
| `A_MEM_EXCLUSIVE` / `A_MEM_WE_IS_STORE` / `A_MEM_RE_IS_LOAD` | MEM | read/write qualifiers agree with the instruction | drop_term, relational |
| `A_MEM_LOAD_DATA` | MEM | loaded value == ISA sign/zero extension of the observed read word | arith/constant in extension logic |
| `A_WB_RD` / `A_WB_WE` / `A_WB_DATA_LUI` | WB | `rd == instr[11:7]`, `we == writes_rd(op) && rd != 0`, LUI value | field wiring, x0 discipline, constant |
| `A_IF_PC_FLOW` | IF | fetch PC is a sequential successor, a stall hold, or an announced redirect | off_by_one, drop/dup |
| `A_IF_PC_ALIGN` / `A_ID_PC_ALIGN` | IF/ID | PC word aligned (RV32I, no C) | off_by_one |
| `A_ID_TO_EX`, `A_EX_TO_MEM`, `A_MEM_TO_WB`, `A_EX_REDIRECT` | — | instruction identity preserved across stage registers (only with `HAZARD_MODE=1`) | drop_term, flush bugs |

## Release-day adoption

1. `bind` the observer to the released top (or to the pipeline module that holds the stage
   registers) and map its ports to the real names. The observer itself does not change:

   ```systemverilog
   bind rv_core rv_stage_sva #(.IALIGN32(1'b1), .CHECK_ILLEGAL(1'b1),
                               .HAZARD_MODE(1'b0), .CR_D3(1'b1)) u_stage_sva (
     .clk(clk), .rst_n(rst_n),
     .if_valid(<fetch_valid>), .if_pc(<pc_f>), .if_instr(<imem_rdata>),
     .id_valid(<id_valid>), .id_pc(<pc_d>), .id_instr(<instr_d>),
     .id_rs1(<rs1_d>), .id_rs2(<rs2_d>), .id_rd(<rd_d>),
     .id_imm(<imm_d>), .id_illegal(<illegal_d>),
     .ex_valid(<ex_valid>), .ex_pc(<pc_e>), .ex_instr(<instr_e>),
     .ex_a(<op_a_e>), .ex_b(<op_b_e>), .ex_alu(<alu_e>),
     .ex_taken(<br_taken_e>), .ex_target(<target_e>),
     .mem_valid(<mem_valid>), .mem_instr(<instr_m>),
     .mem_base(<rs1_m>), .mem_addr(<addr_m>), .mem_store_data(<store_data_m>),
     .mem_we(<store_en_m>), .mem_re(<load_en_m>), .mem_size(<size_m>),
     .mem_rdata(<dmem_rdata>), .mem_load_data(<load_ext_m>),
     .wb_valid(<wb_valid>), .wb_instr(<instr_w>), .wb_we(<reg_we_w>),
     .wb_rd(<rd_w>), .wb_data(<wdata_w>),
     .redirect_valid(<redirect_e>), .redirect_pc(<redirect_target_e>),
     .flush_ex(<flush_e>), .flush_mem(<flush_m>), .flush_wb(<flush_w>)
   );
   ```

2. Rules for the tap, in priority order:
   * `ex_a`/`ex_b` must be the operands **the ALU actually used** (after forwarding), not
     the raw register outputs — otherwise `A_EX_ALU` compares against the wrong inputs.
   * `if_pc`/`if_instr` must be an address and the data returned *for that address*. If the
     core registers the imem request, tap the response side.
   * Tie `flush_*`/`redirect_*` to `1'b0` and leave `HAZARD_MODE=0` if you cannot map them.
     The value assertions do not need them.
   * Only enable `HAZARD_MODE` once the released microarchitecture confirms the squash
     schedule in its own words.
3. Adopt `test_stage.py`'s retire-stream method against the real core: observe writeback
   (a retire port, or `wb_valid/wb_pc/wb_instr/wb_data/wb_rd` via `--public-flat-rw` if the
   core is not flattened, or its own debug signals) and compare against `../rviss`.
   `stage_harness.sv` is a scratch fixture: do not ship it, and do not treat its hazard
   schedule as a specification.

## What is deliberately **not** asserted (and where it is checked instead)

| Not asserted | Why | Where it is checked |
|---|---|---|
| Fetched word == program[pc] | the observer cannot see memory contents; the program image lives in Python | `fetch_stream_is_from_the_program` in `test_stage.py` |
| Hazard/forwarding/stall/flush timing | microarchitecture, not ISA; asserting it would fire on legal designs | retire-stream lockstep (a wrong forward shows up as a wrong committed value) |
| Store byte-lane placement on internal wires | the ISA fixes the memory image, not the DUT's internal write-data layout | final memory-image compare against the ISS |
| AUIPC / JAL / JALR link values | need the instruction's own PC in WB, which the tap does not carry | retire-stream lockstep (`wb_pc` is compared) |
| CSR effects and traps | the released CSR map decides | ISS with `RV32Config` policies once the spec is read |

## Gotchas this exercise found (these are the reason to run it)

All three were *real* bugs in this directory's own code, each invisible to a naive test and
each a plausible seeded deviation in a real core. Verilator 5.048 reproduced them faithfully.

1. **A declaration initializer is not a continuous assignment.**
   `logic [6:0] id_opcode = id_instr[6:0];` inside a module (or a `f3 = instr[14:12];`
   inside an `always_comb`) takes effect **once**, so the value goes stale and assertions
   silently compare the wrong thing. Use `logic ...; assign x = ...;` at module level, or an
   explicit assignment as the first statement of the block.
2. **A `?:` conditional collapses signedness of both branches.** This is the silent-RV32
   bug generator:
   ```systemverilog
   alu = f7[5] ? (a >>> sh) : (a >> sh);        // WRONG: >>> becomes a LOGICAL shift
   alu = (b==0) ? -1 : ($signed(a) % $signed(b)); // WRONG: % becomes UNSIGNED remainder
   ```
   because the unsigned branch makes the whole conditional unsigned, and that context is
   pushed *into* the signed branch. Write separate statements (`if/else` or separate `case`
   arms) whenever signed and unsigned variants of the same operator appear in one expression.
   Also: `x >>> n` on a plain `logic [31:0]` is a *logical* shift — `sra`/`srai` need
   `$signed(a) >>> n`.
3. **Product widths**: `(a*b) >> 32` truncates before the shift (self-determined width); the
   product must be computed in a 64-bit assignment context. Same class as the seeded
   `MULH*` deviations in the playbook.

Memory hygiene, learned the hard way twice: **fill imem with NOPs and zero dmem before every
run**. A pipelined core fetches wrong-path words; an uninitialized memory returns X, which
either fires assertions for the wrong reason (a "green means red" negative test) or lets a
stale value from the previous program in the same simulation masquerade as a DUT deviation.
