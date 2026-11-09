# Flow Internals (verified against `proving-ground/flow:public`)

Everything here was read from the installed `pgflow` package **inside the flow image**
(`/usr/local/lib/python3.10/dist-packages/pgflow/`) and confirmed with an end-to-end
dry run on a synthetic design (`prep/flowcheck/`, see that directory's README).
Unlike `flow-commands.md` (which is about *how to invoke* the flow), this file is about
*what the flow will do to us* — so we spend release-day time on RTL, not on surprises.

> These are public, local tools. Nothing here is hidden-test data: `C` remains
> server-only. Everything about `V` and `P` is computed on our own machine.

## 1. `design.yaml` schema

`pgflow.design.load_design` reads only these keys (unknown keys are ignored):

```yaml
id: challenge-2                # default: directory name; used in report.json and cache dir name
top: dma_ctrl                  # REQUIRED (KeyError if absent)
clock_port: clk                # default "clk"
reset_port: rst_n              # default "rst_n"  (passed to the mutator as a reset name)
clock_period_ns: 10.0          # default 10.0; used by the PPA score
filelist: rtl/filelist.f       # default "rtl/filelist.f"
sim: verilator                 # string, or dict {"simulator": ..., "build_args": [...]}
lines: 780                     # informational
ppa:                           # ORFS variables, uppercased; defaults below
  CORE_UTILIZATION: 40
  PLACE_DENSITY_LB_ADDON: 0.20
```

`sim.build_args` (or `sim_options.build_args`) default to
`-Wno-fatal --x-assign unique --x-initial unique`; the flow appends `--assert` when
compiling our assertions.

The PPA cache key is `sha256_tree(design_dir)` **excluding `hidden/`, `worked-example/`,
`__pycache__/` and `tb/`** (`pgflow.ppa.cache_dir`). Consequences:

- Editing `rtl/` in the `--design` directory invalidates the cached baseline → run PPA
  with `--design` pointing at a pristine mirror (already in `docs/design-init.md`).
- Editing `tb/` or `tests/` does **not** invalidate it — useful to know when a cache seems
  stale.
- `hidden/` is excluded from the hash, so the organizers can drop hidden tests in without
  changing our cache.

## 2. Submission archive rules (`pgflow/archive.py`)

- Accepted: `.tar.gz`, `.tgz`, `.zip`. Symlinks, hard links, devices, encrypted members
  and duplicate names (case-insensitive) are rejected.
- Top-level entries kept: **only** `rtl/`, `tests/`, `assertions/`, `report.md`,
  `trajectory/`. Anything else is silently dropped with a warning.
- **Required:** `rtl/` and `report.md`. `tests/` and `assertions/` are optional but
  without `tests/` V = 0 by definition.
- A single wrapping folder is stripped (`challenge-2-sub/rtl/...` → `rtl/...`).
- Limits: ≤ 2000 members, ≤ 20 MB per file, ≤ 100 MB uncompressed, ≤ 50 MB archive,
  compression ratio ≤ 200 (above 10 MB total). `sim_build/`, `__pycache__/`,
  `results.xml` are not excluded automatically — packaging them wastes the ratio budget.

## 3. Build stage (`pgflow/checks.py`)

Reject conditions, in evaluation order:

1. `rtl/filelist.f` missing, or its entry list differs **at all** from the released one.
   Comparison happens after stripping comments (`//`, `#`) and blanks, so comment changes
   are fine; order and filenames are not.
2. Any `.sdc` anywhere in the submission whose bytes differ from the released
   `flow/constraint.sdc` → rejected. Don't ship an SDC unless it is byte-identical.
3. Forbidden constructs anywhere in the filelist RTL: `initial`, `final`, `#` delays,
   `fork/join`, `wait`/`wait fork`, `force`/`release`, classes, DPI import/export,
   program blocks; system calls `$readmemh/$readmemb`, `$fopen/$fclose/$fread/$fscanf/
   $fgets/$fgetc/$sscanf`, `$system`, `$writememh/$writememb`, `$fwrite`, `$fdisplay`,
   `$value$plusargs`, `$test$plusargs`, `$random`, `$urandom`, `$urandom_range`,
   `$dist_uniform`; `\`include` with an absolute path or `..`.
   (`@(posedge clk)` is allowed; only `#`-delays are rejected.)
4. `verilator --lint-only -Wno-fatal -Wno-DECLFILENAME -Wno-UNUSEDSIGNAL -Wno-UNUSEDPARAM`.
5. Yosys `read_slang` + `hierarchy -check` + `proc` + `check`.
6. Top-level **port set, direction and width** must equal the baseline (computed once by
   elaborating the released RTL and cached as `baseline_ports.json`).

> `assertions/*.sv` are **not** scanned by the forbidden-construct pass; they are only
> compiled in the evidence stage. Keep them free of `initial` anyway — it costs nothing
> and avoids surprises if the checker is extended.

## 4. Public/evidence stage (`pgflow/public.py`)

- Build: every filelist source of **our** `rtl/` + every `assertions/*.{sv,v}` file, with
  `--assert` and the design's `build_args`.
- Public testbench (`tb/public/`) runs for information only; its failure does not reject.
- Our tests: modules are discovered by **filename** `tests/test_*.py` (basename stem is the
  module name; subdirectories are not searched). They run with `pythonpath = tests/`, so
  helper modules next to them (`vip.py`, `golden.py`, …) import normally.
- Rejection if any test fails, an assertion fires, the run times out (600 s), or no test
  result is produced.
- `evidence_seconds` = wall-clock seconds of that run. It sets the per-mutant timeout
  (`min(300, max(30, 2·evidence + 15))`), so keep the whole evidence suite in the low
  single-digit seconds. A 45 s suite gives each of 40 mutants 105 s — mostly wasted, and
  it makes the mutation stage slow.
- Simulation plusargs: `+verilator+seed+<n>` and `+verilator+rand+reset+0`, i.e.
  **registers and inputs start at zero**. Tests must not depend on X or on random startup.
- Assertion-failure detection is a regex on the log:
  `Assertion failed | %Error: .*assert | [N] %Error`. A `$finish`-style abort counts.

## 5. Mutation stage (`pgflow/mutate.py`, `mutation.py`)

**Operator classes (fixed set, exactly six):**

| class | edits |
|---|---|
| `arith` | `+↔-`, `*→+`, `&↔|`, `^→|`, `<<↔>>`, `>>>→>>`, `<<<→<<` |
| `relational` | `<→<=/>`, `<=→</>=`, `>→>=/<`, `>=→>/<=`, `==↔!=`, `&&↔||` |
| `constant` | integer literal ±1 (same size/base), `'0↔'1` |
| `negate` | `if`/ternary condition `c → !(c)` |
| `drop_term` | `A && B → A` or `B` (also `||`), reset condition → `1'b0` |
| `off_by_one` | comparison bound `R → R±1` when `R` is not a plain literal |

**Where mutants come from** — only run-time contexts: continuous assigns, `always_*`
blocks, net declaration assignments, instance port connections. Never: parameters,
localparams, dimensions, range selects, generate conditions, loop headers, case labels,
replication counts, casts, system-function arguments, functions/tasks, packages,
interfaces, assertion/property bodies, `initial`/`final`, bind directives, or the LHS of
an assignment. Sites whose module is not elaborated under `top` are dropped.

**Selection** — deterministic from `sha256(submission archive)`:
sites are deduped, filtered to instantiated modules, shuffled per class, interleaved
round-robin across the six classes, and accepted until 40, after:
- slang semantic check (invalid mutants rejected),
- Yosys equivalence check (`equiv_make`/`equiv_simple -seq 5`/`equiv_induct -seq 5`,
  10 s each, 90 s total budget). Proven-equivalent mutants are discarded; a timed-out
  check **keeps** the mutant; after the budget, candidates are accepted unchecked.

Then one Verilator build holds all mutants behind `+PGMUT=K` (mutant schemata; the tool
injects `$value$plusargs` itself). A sanity run with `+PGMUT=0` must pass our tests.
Each mutant then runs the same test modules once. Kill if **any** test fails, an
assertion fires, the sim errors, or the per-mutant timeout expires.

`V = killed / counted`; mutants that do not build are not counted.

### Consequences for our evidence suite

- A mutant is killed by *either* a test check or an assertion. Assertions are cheaper to
  write for internal invariants; tests are better for end-to-end behavior.
- Anything that only differs in a corner nobody stimulates is unkillable in 30 s.
  A 16-bit saturation constant, for example, needs ~65k cycles to observe. Accept the
  V ceiling; don't blow the evidence budget chasing one mutant.
- **Reset-branch mutants are invisible at time 0** because registers start at zero.
  Always include a mid-operation reset test (assert reset while the design holds nonzero
  state and check *every* state element returns to its reset value).
- `drop_term` on a handshake term (`ack && !busy → ack`) is killed by an assertion that
  the handshake requires the guard, not by a data check.
- Mutant IDs are re-rolled whenever the archive changes. Fix **classes and expressions**,
  never specific IDs.

## 6. The V iteration loop (no server submission needed)

```bash
# 1. package + run everything except PPA (~20 s for a small design)
./prep/flowcheck/run.sh --skip ppa      # or, for a real design, the pgflow command

# 2. read the verdict as an action list
scripts/mutant-report.py runs/<design>-out          # survivors with exact mutations
scripts/mutant-report.py runs/<design>-out --md     # table for the progress doc

# 3. add the assertion / test that distinguishes the mutant, re-run step 1-2
```

`stages/mutation.json` also carries `selection.sites_by_class` — if a class has very few
sites (e.g. `off_by_one`), V is dominated by the other classes; don't spend time on
assertions that only that class could trip.

## 7. PPA stage (`pgflow/ppa.py`, `score.py`)

- Fixed ORFS flow on `sky130hd`: `synth → floorplan → place → cts → route → finish`
  (`do-finish`, no GDS merge). Same SDC as released. `SYNTH_HDL_FRONTEND=slang`,
  `SYNTH_HIERARCHICAL=0`, `LEC_CHECK=0`.
- Baseline and submission run **in parallel** (two ORFS runs, each `--jobs 12`).
- Baseline is cached in `<cache>/baseline_ppa.json`, keyed by the design tree hash.
- Metrics come from ORFS JSON: `area_um2`, `cell_count`, `utilization`, `wns_ns`,
  `tns_ns`, `hold_wns_ns`, `power_w`, `wirelength_um`, `drc_errors`, die/core area,
  global-route overflow.
- Score:

```
a = clip((A_base - A_sub)/A_base, -1, 1)
t = clip((T_base - T_sub)/T_base, -1, 1)      T = clock_period_ns - WNS
p = clip((P_base - P_sub)/P_base, -1, 1)
P = clip( 0.5 + 0.5*(0.4a + 0.4t + 0.2p), 0, 1 )
if WNS_sub < 0 and P > 0.3:  P = 0.3          # timing fail cap
```

Notes that change PPA planning:

- Timing is 40 % of the *improvement*, so shortening the critical path pays even when the
  baseline already has positive slack (`T_base = 15 - WNS`, and a smaller achievable
  period is better).
- Power is only 20 %, and `power_w` at `finish` includes the clock tree — clock gating
  and fanout reduction move both area and power.
- A design that misses timing is capped at P = 0.3 regardless of area. Fix WNS first.
- `P = 0.5` means parity; the practical target range is 0.6–0.8.

## 8. Numbers from the synthetic dry run (this host, 16 cores)

| Stage | Time |
|---|---|
| build (lint + yosys + ports) | 0.2 s |
| public build + public tb + evidence (4 tests) | ~5 s (evidence 0.5 s) |
| mutation (40 mutants, schemata) | ~11–13 s (selection ~6.5 s, build 1.6 s, run 2.2 s) |
| PPA (baseline + submission in parallel, ~1k cells) | 136 s (route 110 s, place 10 s, others < 5 s each) |
| full run, `--skip ppa` | ~17 s |
| full run, with PPA | ~137 s |

Baseline-vs-identical-submission sanity check: `P = 0.5` exactly (`a = t = p = 0`), i.e.
fixed-vs-fixed parity, which confirms why the `--design` mirror must stay pristine.
Measured baseline: 14 574 µm², WNS +6.33 ns at a 10 ns period (achievable period
3.67 ns), 0.00397 W, 0 DRC. Positive slack does **not** mean timing is irrelevant: the
score uses `T = period − WNS`, so a shorter critical path still earns `t > 0`.

The evidence suite is the only stage whose runtime we control. On the real designs the
public tb and evidence runs will be longer, and the per-mutant timeout follows
`2·evidence + 15`. Routing dominates PPA wall time; use `--skip ppa` for every
evidence/V iteration and only run PPA when measuring P.

## 9. Known environment gotchas

- The timing engine is **OpenSTA**, not Design Compiler: `remove_from_collection` is not a
  command and aborts synthesis when it appears in an SDC. (Irrelevant for competition
  SDCs, which we must not edit.)
- `pgflow`'s ORFS runner does not pass `KLAYOUT`/GDS through (`do-finish`), so don't look
  for a `.gds` in the work dir.
- A failed PPA stage does not reject the submission; it produces `P` unavailable
  (locally `P` stays 0 in the report, and the warning goes to `report.json["warnings"]`).
- `pgflow run --keep-work` keeps `work/`, which is the only way to see the built
  schemata, ORFS work home, and per-mutant logs.
