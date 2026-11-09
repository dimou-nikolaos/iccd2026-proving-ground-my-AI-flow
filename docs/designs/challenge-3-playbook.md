# Challenge-3 playbook — `rv_core` (five-stage RV32IM core)

**Updated 2026-11-08 from the organizers' public design description.**
All facts below are from the public blurb, not from the spec/release zip (not yet available).
On release, re-derive; the spec text is the only authority.

## Confirmed design shape

| Fact | Value |
|---|---|
| Top module | `rv_core` |
| Size | ~1,200 lines of SystemVerilog |
| Clock | **15 ns** on sky130hd (50 % looser than ch1/ch2) |
| Pipeline | 5 stages: **IF / ID / EX / MEM / WB** |
| ISA | **RV32I** + **M** extension + **Zicsr** |
| Privilege | **machine mode only**, traps and interrupts |
| Memory | **separate instruction and data interfaces** |
| Testbench | ships a **small Python assembler** for writing test programs |
| Change request | **CR-D3: seven address-generation and min/max instructions from Zba and Zbb** |

Implications:

- The verification surface is the largest of the three designs. Half of it (traps, interrupts,
  CSRs) is **hard to drive pin-by-pin** — it needs assembly test programs. The public assembler
  helps; our own assembler + ISS removes the dependency on its feature coverage.
- **Differential random-program testing is the primary deviation finder.** A 1,200-line pipeline
  has a small state space per instruction and a large instruction space; lockstep comparison
  against a Python ISS catches almost every seeded deviation without reading RTL line by line.
- **15 ns is generous.** A 32×32 array multiplier and a single-cycle 32-bit divider both close
  at 15 ns on sky130hd, so baseline timing is probably positive-slack. P becomes an
  **area/power** game (multiplier + divider + 32×32 register file dominate).
- The core likely uses **multi-cycle divide** (or a serial divider) — that FSM is a rich
  seeded-deviation site (stall propagation into forwarding/flush, reissue, CSR interaction).

## Predicted REQ categories (map to spec on release)

1. **RV32I base**: every opcode class — LUI/AUIPC/JAL/JALR/branches/loads/stores/ALU/immediates,
   `x0` hard-wired, sign/zero extension, shamt masking.
2. **M extension**: MUL/MULH/MULHSU/MULHU, DIV/DIVU/REM/REMU (+ chosen latency/encoding).
3. **Pipeline**: one instruction retires per cycle when no hazard; bubbles on flush/stall;
   no writeback from invalid stages; PC semantics.
4. **Hazards**: EX/MEM and MEM/WB forwarding with the correct priority; load-use stall
   (exactly one cycle + bubble); forwarding into branch comparison and store data.
5. **Control flow**: taken/not-taken branch flush width; JAL/JALR link values; misaligned
   target behavior (spec-defined: fault vs mask).
6. **CSRs (Zicsr)**: CSRRW/CSRRS/CSRRC + immediate forms, read/write side effects, `rs1=x0`
   no-write rule, illegal-CSR behavior, read-only fields.
7. **Traps**: illegal instruction, ECALL/EBREAK, misaligned access (if implemented),
   `mepc`/`mcause`/`mtval`/`mtvec` semantics, `mstatus.MIE/MPIE` push/pop, MRET.
8. **Interrupts**: source(s), enable conditions (`mip & mie & mstatus.MIE`), sampling point,
   latency, the post-MRET shadow, interaction with multi-cycle instructions.
9. **Memory interfaces**: request/response protocol, no spurious accesses, correct address/
   size/sign for LB/LH/LW/SB/SH/SW.
10. **CR-D3**: seven new instructions (below).

## CR-D3 — exact instruction set (verified against the ratified bit-manipulation spec)

"Seven address-generation and min/max instructions from Zba and Zbb" resolves uniquely in RV32:

| Instr | opcode | funct7 | funct3 | Semantics (RV32) |
|---|---|---|---|---|
| SH1ADD | 0x33 | `0010000` | `010` | `rd = (rs1 << 1) + rs2` |
| SH2ADD | 0x33 | `0010000` | `100` | `rd = (rs1 << 2) + rs2` |
| SH3ADD | 0x33 | `0010000` | `110` | `rd = (rs1 << 3) + rs2` |
| MIN    | 0x33 | `0000101` | `100` | signed min(rs1, rs2) |
| MINU   | 0x33 | `0000101` | `101` | unsigned min |
| MAX    | 0x33 | `0000101` | `110` | signed max |
| MAXU   | 0x33 | `0000101` | `111` | unsigned max |

All are R-type in the OP major opcode, results mod 2^32, no exceptions, no CSR interaction.

Implementation sketch (once the decoder is seen):

- **Decoder**: two new funct7 matches inside the existing OP (R-type) case. Do **not** let them
  fall through to the illegal-instruction default; also check the existing decoder does not
  already treat `funct7=0010000` as MUL (MUL is `0000001`).
- **Datapath**: SHxADD = shift-and-add. If the core has a barrel shifter before the adder,
  tap `rs1` through a 1/2/3-bit left shift into one adder input and `rs2` into the other.
  MIN/MAX = reuse the branch comparator's signed/unsigned compare result to select between
  `rs1` and `rs2` (two muxes). Cheap for area; check the added mux depth on the EX critical
  path.
- **Hazards/forwarding**: new instructions read rs1/rs2 and write rd — no new hazard logic, but
  the random test must cover forwarding from EX/MEM and MEM/WB into each new instruction.
- **Illegal-instruction decode**: ensure `funct7=0000101` other funct3 values (and
  `0010000` other funct3) still trap as illegal (spec-dependent; check what the baseline does
  for unknown OP funct7s).

### CR-D3 test matrix (directed, per instruction)

- Equal operands → min == max == operand
- `0x7FFFFFFF` vs `0x80000000` → signed vs unsigned disagree (kills signed/unsigned swap)
- `0xFFFFFFFF` (−1) vs `0` and vs `1` → signed negatives
- `x0` as rs1 / rs2 / rd
- `rd == rs1` and `rd == rs2` aliasing
- Shift-overflow encodings: `sh3add` with operand `0x20000000` (result wraps)
- Forwarding: producer in EX, then in MEM, then in WB; back-to-back dependencies
- Interaction with a load-use stall immediately before a new instruction
- Serialized traps after a new instruction (mepc/mcause correctness)

## Deviation catalog (hunt list)

Ordered by probability × detection cost. The random-program diff catches most of these; the
directed tests pin them to a REQ.

### ISA semantics

| # | Suspect | Note |
|---|---|---|
| R1 | DIV/REM by zero: DIV→−1, REM→dividend | Classic seed |
| R2 | Signed overflow: `0x80000000 / −1` → quotient `0x80000000`, remainder 0 | Classic seed |
| R3 | Shift amount masking: RV32 uses `shamt[4:0]` only | `sll/srl/sra` by 32+ |
| R4 | LB/LH sign-extend vs LBU/LHU zero-extend; negative offsets | Defn assertion possible |
| R5 | Branch signedness: BLT/BGE signed, BLTU/BGEU unsigned | Swap is a 1-line mutation |
| R6 | JALR target `& ~1`; `rd = pc+4` for JAL/JALR | Off-by-one seeds |
| R7 | Immediate sign-extension for JAL (20-bit), branches (13-bit), AUIPC | Bit-slicing bug |
| R8 | `x0` write discarded (all writes, including load to x0) | Reset-drop-like mutant |
| R9 | SLT/SLTU signedness; `sra` vs `srl` (funct7) | Comparator/shifter decode |

### Pipeline / hazards

| # | Suspect | Note |
|---|---|---|
| R10 | Forwarding priority EX/MEM over MEM/WB when both match | Common seed |
| R11 | Missing forwarding into branch comparator or store data | Directed test |
| R12 | Load-use stall: too short (0) or too long (2 cycles), or bubble not inserted | Cycle-count test |
| R13 | Flush width: IF+ID only vs IF+ID+EX (wrong on taken branch/JAL) | Inject instruction right behind branch |
| R14 | Invalid/bubble stage writes the register file or memory | x0/writeback assertions |
| R15 | PC select: `pc+4` vs `alu` vs `mepc` wrong for JALR/branch/trap | Directed |
| R16 | Double-advance of PC on trap or MRET (mepc ±4) | Directed trap test |

### Traps / CSRs / interrupts

| # | Suspect | Note |
|---|---|---|
| R17 | `mepc` = PC+4 instead of PC of trapping instruction | Classic seed |
| R18 | `mcause` code wrong (illegal=2, ecall-M=11, misaligned=4/6, breakpoint=3) | Per spec |
| R19 | Trap entry: `MPIE←MIE`, `MIE←0`; MRET: `MIE←MPIE`, `MPIE←1` | mstatus dance |
| R20 | MRET does not redirect to `mepc`, or re-executes the trapping instruction | Directed |
| R21 | Interrupt taken right after MRET (must be shadowed until the next instruction retires) | Classic off-by-one |
| R22 | Interrupt enable test wrong: `mip & mie & MIE` (one term missing/inverted) | Defn assertion |
| R23 | Interrupt sampling point differs between spec and RTL (taken mid-multi-cycle op) | Directed with slow memory |
| R24 | CSR write masks: read-only fields writable; `csrrs/csrrc` with `rs1=x0` writes | Directed |
| R25 | Write to a read-only CSR: illegal-instruction vs ignore (spec-defined) | Per spec |
| R26 | Illegal instruction leaves the pipeline running / does not trap | Directed |

### Memory interfaces

| # | Suspect | Note |
|---|---|---|
| R27 | Spurious memory access for a bubble/flushed instruction | Interface assertion |
| R28 | Size/address wrong for SB/SH/SW (byte lanes) | Directed + random |
| R29 | Misaligned access: fault vs ignore (spec-defined) | Per spec |
| R30 | Data-interface response sampled too early (before valid) | Slow-memory test |

## Assertion plan

Top-level, bind-style (all inputs; `default disable iff`):

1. **No writeback from an invalid stage**: `!wb_valid |-> !regfile_we`.
2. **x0 is never written**: `!regfile_we || wb_rd != 0` (also kills write-to-x0 mutants).
3. **PC legality**: `pc[1:0] == 0` at instruction-fetch boundaries.
4. **One-commit-per-cycle** when no stall/flush; `commit |-> !stall`.
5. **Memory request protocol**: valid stable until ready, no request on bubble.
6. **CSR definitions** once internal names are known: `mepc_next == (trap ? trap_pc : (mret ? mepc : ...))`,
   `mstatus.MIE` next-state from trap/mret/write, `mcause` decode.
7. **IRQ definition**: `irq_taken == mstatus.MIE && |(mip & mie)` at the spec's sampling point.
8. **CR-D3 (separate file)**: `ex_result == (sh1add ? (a<<1)+b : ...)` definition assertion +
   decimal/edge-value tests.

Because internal signal names are unknown until release, items 1–4 and 8 are **built and
verified** as `prep/rvcore/rv_stage_sva.sv`: 20 concurrent assertions over a canonical
pipeline tap (all-`input`, bind-safe), plus `rv_ref_pkg.sv` (pure ISA reference: ALU incl.
M + CR-D3, all five immediate formats, branch compare, load extension, legality). They were
proved live by injecting nine deviations into a synthetic 5-stage harness — each tripped an
assertion, and the clean build fires none. On release day the only work is a 20-line `bind`
map; see `prep/rvcore/README.md` for the port-by-port guidance and the
`IALIGN32 / CHECK_ILLEGAL / HAZARD_MODE / CR_D3` parameters.

The single highest-yield assertion is the EX-stage equivalence
`ex_alu == alu_ref(opcode, funct3, funct7, ex_a, ex_b_ref)`: for every encoding in the
subset it pins the whole ALU at every cycle, so an `arith`, `relational`, `negate`,
`constant`, `drop_term` or `off_by_one` mutant in that logic is caught the moment it is
produced, not only when a test happens to observe it.

## Verification infrastructure (built in `prep/`, reuse on day 1)

| Artifact | Use |
|---|---|
| `prep/rviss/rv32_iss.py` | Golden ISS: RV32IM + Zicsr + M-mode traps/interrupts; configurable misalign policy, CSR map, IRQ map |
| `prep/rviss/asm.py` | Mini assembler/encoder incl. CR-D3 instructions, labels, pseudo-ops |
| `prep/rviss/randprog.py` | Constrained-random program generator (ALU/shift/branch/load-store, hazard-dense) |
| `prep/rviss/test_iss.py` | ISS self-tests (hand-computed vectors + corner cases) |
| `prep/rvcore/rv_ref_pkg.sv` | Pure ISA reference functions consumed by the SVA observer |
| `prep/rvcore/rv_stage_sva.sv` | 20 per-stage contract assertions over a canonical pipeline tap (bind-safe, parameterized) |
| `prep/rvcore/test_stage.py` | Working retire-stream lockstep driver: NOP-fill imem, zero dmem, per-retire compare vs the ISS, final register + memory-image compare |
| `prep/rvcore/dbg_operand_diff.py` | Tells an *operand* divergence (forwarding/hazard) from an *operation* divergence (ALU) at the first wrong commit |
| `prep/rvcore/stage_harness.sv` | Scratch 5-stage fixture used to prove the assertions fire; **not** a model of the released core |

**Lockstep pattern** (driver exists in `prep/rvcore/test_stage.py`; on release day only the
memory/tap wiring changes):

1. Golden ISS runs the program, records (pc, instr, rd, value) per cycle-agnostic commit.
2. DUT commits are observed from the register-file write port / retire signal (or by
   dumping the register file and a hash of data memory at the end if no commit port is
   visible).
3. On mismatch, replay the last N instructions with the ISS to produce a minimal repro and
   a directed test.

If no commit port exists, fall back to **final-state comparison**: run the program, compare
all 31 registers + data memory + the PC/trap CSRs against the ISS. This is nearly as strong
for seeded deviations and works with only the architectural state exposed.

## PPA plan (15 ns, sky130hd)

Prior: 15 ns is loose; expect positive WNS at baseline. P is likely **area/power-dominated**.
Per L16, do not add pipelining unless the measured path demands it.

| Step | Action | Tier | Note |
|---|---|---|---|
| A | Fix deviations + CR; bank C=1.0 | A | Never delay |
| B | Measure: area breakdown by module (`scripts/submodule-synth.sh`), WNS, power | — | Find whether mult/div/regfile dominates |
| B | `(* keep *)` wildcard, one wire per module (L27–L29) | B | Cheap |
| B | Register-file storage simplification (one always_ff, no reset on data flops) | B | Data flops rarely need reset; saves area/power. Check spec's reset REQ |
| C | Multiplier/divider restructuring (iterative vs combinational) | **C** | **Only if the spec leaves latency unconstrained** — cycle counts are probably observable (interrupt latency, test wait loops). C-check mandatory (L4) |
| C | Multi-port register-file RAM inference | C | Yosys SRAM inference for 2R1W is unreliable; two 1R1W banks + mux is the fallback. Measure |
| — | ALU operand isolation | unlikely | Small widths; L17 says it loses |

**New spec-freedom question for ch3 (audit on release):** is the pipeline's **cycle count**
observable? If the spec says "an instruction retires in one cycle except on a load-use hazard"
then cycle behavior is frozen and all latency-changing optimizations are off the table. Read
this before planning any Tier C.

## Mutation notes (RV-specific mutant classes)

- **Arithmetic mutants** on ALU ops: kill with per-op directed tests using asymmetric operands
  (`+`→`-`, `<<`→`>>`, signed/unsigned). Asymmetric means **opposite signs and extreme values**:
  `srai` of a negative number, `0x8000_0000 / -1`, `rem` with a negative dividend. Positive-only
  operands make the whole signedness mutant class invisible (L37 — and note that `?:` with one
  unsigned arm silently makes the signed arm unsigned, so a "correct-looking" RTL line can be a
  seeded deviation).
- **Decode mutants** (funct3/funct7 bit errors): kill with one instruction per (opcode, funct3,
  funct7) combination. The decode-definition assertions **already exist** (`A_DEC_FIELDS`,
  `A_DEC_IMM`, `A_DEC_LEGAL`, `A_EX_ALU`) — they do not need internal decode signals to be named,
  only mapped.
- **Comparison mutants** in branch/CSR logic: signed/unsigned discipline (`A_EX_BRANCH`).
- **Silent mutants** — the ones that survive because nothing observes them. Two-layer answer:
  the stage-contract SVA pins every stage value every cycle (so a wrong value is caught even when
  it is overwritten before commit), and the retire-stream lockstep pins the instruction *sequence*
  (so a dropped/duplicated/speculatively-retired instruction is caught even when every stage is
  locally consistent). Neither alone is enough.
- **State-transition mutants** in the trap/MRET/interrupt FSM: directed tests + next-state
  assertions on `mstatus`/`mepc`/`mcause`.
- **Cycle-count mutants** (stall/flush width): tests that count cycles on a hazard/branch.
- Expect residual equivalent mutants in reserved/unused encodings — document as V ceiling.

## Day-1 checklist (Nov 13)

1. [ ] Unzip, `md5sum` baseline, bootstrap docs (`challenge-3.md`, `-req-matrix.md`,
   `-progress.md`).
2. [ ] Baseline flow run; note C/V/P, WNS, area breakdown.
3. [ ] Adapt the ISS CSR map + trap sources to the spec (config, not rewrite).
4. [ ] Build the cocotb memory model + lockstep (or final-state) harness around the public
   assembler — start from `prep/rvcore/test_stage.py`, which already does fetch-load,
   NOP-fill, dmem zero, per-retire compare and final-state compare.
4b. [ ] Write the `bind` map for `prep/rvcore/rv_stage_sva.sv` onto the released stage
   registers (`HAZARD_MODE=0` at first) and confirm zero firings on the baseline before
   using it as a deviation oracle.
5. [ ] Run 5–10 random programs + directed ISA tests → deviation list.
6. [ ] Implement CR-D3; per-REQ test + assertion matrix complete (L12).
7. [ ] First submission by end of day.

## What NOT to repeat from challenge-1

- Don't add pipeline stages because "it's a pipeline core" — check WNS first (L16).
- Don't operand-isolate narrow ALUs (L17).
- Don't stack `(* keep *)` in the same module (L28).
- Don't change externally observable cycle counts without a server C-check (L4).
- Don't chase V past the equivalent-mutant ceiling (L7, L8).
