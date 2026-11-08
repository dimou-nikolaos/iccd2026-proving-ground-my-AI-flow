# Phase 4e: Wildcard Exploration

Optional phase, triggered **after** the Phase 4 exit criteria are met (C=1.0 locked, P≥0.50, V at ceiling). Runs during operator cooldown time. Budget: 2-4 hours local per design. Only surviving variants cost submissions.

## Why this phase exists

Score = 100 × C × (0.5V + 0.5P). A well-explored design scores >80. The default Phase 4b/4c flow (Tier A/B/C structural PPA + mutation hunting) tends to plateau at P≈0.55-0.60 because it optimizes **within** the baseline's algorithmic structure. To break 0.70 you typically need **algorithmic** moves (CSD multipliers, carry-save, distributed arithmetic, transposed filters, memory inference) or **aggressive pipelining** that pushes WNS way past baseline slack.

These moves are cheap to try locally (RTL edits + full flow run) but each has real C-risk (they change internals hidden tests might pin). Running them during cooldown time costs nothing except compute; a survivor is a new sub-best or Explorer candidate. A loser feeds the next challenge's PPA plan.

## Trigger

Enter Phase 4e when:
- C=1.0 is banked (server-confirmed) AND
- Phase 4b Tier A/B/C have been applied and their results recorded in `<design>-ppa-plan.md` AND
- Cooldown time is available (either waiting for a submission result, or between server submissions)

Do NOT enter Phase 4e before C=1.0 is locked — Phase 4e changes risk C regression, and you can't afford that uncertainty before C is banked.

## Protocol

### Step 0 — Decide serial vs batched execution

**Agent wall-clock = bash-call wall-clock**. Each tool invocation my agent makes is one stall. N separate Docker runs = N stalls. Batch into ONE bash call with `parallel` = ONE stall, results parsed afterward.

**Batch when**:
- 3+ variants to validate at the same step
- Each Docker call is long (PPA full-flow ~6-8 min each)
- Variants are independent (no result of variant 1 informs variant 2)

**Serial when**:
- Each result informs the next (exploratory debug, mutant hunting, RTL editing iterations)
- Fewer than 3 variants
- Variants share mutable state (baseline PPA cache recompute, cache miss on first run)

**Batched pattern (template)** — see `pg-flow-commands` skill for exact recipe:
```bash
# Pre-build variant dirs at /tmp/opencode/wild-variants/w<N>/rtl,tests,assertions,report.md
for v in w1 w2 w3 w4 w5; do tar czf runs/sub-$v.tar.gz -C /tmp/opencode/wild-variants/$v rtl tests assertions report.md; done

# Launch parallel via bash background jobs + wait (GNU parallel NOT installed)
for w in w1 w2 w3 w4 w5; do
  ( docker run --rm --cpus 2 ... --out /runs/out-$w ... > /tmp/opencode/flow_$w.log 2>&1 ) &
done
wait

# Parse results in a follow-up call:
for w in w1 w2 w3 w4 w5; do
  echo "=== $w ==="
  tail -15 /tmp/opencode/flow_$w.log
done
```

**Note**: GNU `parallel` is NOT available on this machine. Use bash `&` + `wait` (works correctly for backgrounding Docker invocations).

**Two-stage batch evaluation** (recommended):
- **Stage 1** (parallel submodule synth or whole-design synth, 1-2 min per variant, 10 in parallel = 10 min): pre-filter obvious area losers.
- **Stage 2** (parallel full-flow, ~12-15 min for 5 variants in parallel with `--cpus 2` each): validate survivors with real P breakdown.

This beats 5 × 6-8 min serial = 30-40 min, AND catches synthesis-only losers cheaply before spending full-flow time on them.

### Step 1 — Brainstorm 3-5 candidates (30 min)

Load `sky130hd-ppa` skill (Step 0 spec-freedom cross-check + algorithmic patterns section). List candidates ranked by:

| Rank by | Weight |
|---------|--------|
| Estimated P delta | primary |
| Time to implement + validate (see `running-the-flow.md` timing table) | secondary |
| Algorithmic distinctness (for Explorer award) | tiebreaker |

For each candidate, write a 3-line entry in `<design>-ppa-plan.md`:
```
| Name | Hypothesis | Est. ΔP | Est. time | Tier | Risk |
```

### Step 2 — Implement and validate each (1-3 hours each)

For each candidate, in order of expected ROI:

```
For candidate X:
  1. Snapshot current RTL to /tmp/opencode/<design>-pre-<X>/
  2. Implement RTL change (Edit / Write tools)
  3. Lint + elaborate (Docker, ~1 min)
  4. Run submission tests with ASSERTIONS=1 on fresh sim_build (~1-3 min)
     - If any test fails: debug or abandon
     - If any assertion fires: debug or abandon
  5. Update / rewrite interface assertions if internals changed (L14)
  6. Package runs/sub-<X>.tar.gz
  7. Run FULL PPA flow (Docker, ~6-8 min with cache) — L14 requires this
     - If status=rejected or P=null: abandon (L14 CTS-fail rule)
     - If P improves AND local tests pass: queue for C-check submission
     - If P same or worse: abandon (L5: don't reroll V for P-neutral change)
  8. On abandon: restore RTL from /tmp/opencode snapshot
```

Time each step (`date +%s` before and after, or just note wall-clock in activity log). Update `running-the-flow.md` timing table with the measured durations for future reference.

### Step 3 — Submit survivors during cooldown

Each survivor: 1 C-check submission, Tier C rules apply. Keep highest-projective-score archive.

### Step 4 — Record outcomes for future challenges

Every candidate (winner OR loser) gets an outcome paragraph in `<design>-ppa-plan.md` under a "Wildcards tried" section:

```
### Wildcard: <name>
Hypothesis: <one-line>
Time: <total wall-clock including iteration>
Result: PROMOTED / REVERTED / P-neutral
Measurements: P=..., area=..., WNS=..., power=..., V=...
Verdict for future challenges: <1-2 sentences reusable knowledge>
```

These paragraphs become the seed list for Phase 4e on the next challenge.

## Wildcard catalog (seed ideas, extend per design)

**Aggressive pipelining**:
- Split every combinational stage ≥3 LUT depth. Each split buys ~+1 ns WNS (timing weight ≈ 1.2 in P).
- Push pipeline to within REQ-latency budget. Challenge-1 REQ-09 gives 16 cycles; typical design uses 8-10. Slack = free pipeline stages.
- Pipeline the FIFO write/read handshake.

**Arithmetic**:
- **CSD (Canonical Signed Digit)** for constant-coefficient multipliers: replaces `x * C` with shift-add tree; average 50% fewer add ops than binary encoding.
- **Carry-save accumulation**: FIR/boxcar sums accumulated as (sum, carry) pair, collapsed to a single value only at output. Cuts accumulator critical path.
- **Distributed arithmetic FIR**: eliminates multipliers entirely, uses ROM + shift-add. Big area win for fixed coeffs.
- **Transposed FIR form**: shorter critical path, more registers, better for aggressive pipelining.
- **Polyphase decomposition**: for FIR followed by decimator, only compute partial sums at decimation positions.

**Storage**:
- **Memory inference**: FIFO/tap-delay ≥64 bits → infer SRAM macro. Sky130hd has sram macros but flow tech lib may or may not expose them — verify.
- **Multi-bit flop packing**: adjacent flops with same enable/clock bundled into 2x/4x wide cells. Yosys `opt_dff` sometimes does this; `(* iopad *)` or `(* keep *)` attributes guide it.

**Clock/power**:
- **Explicit ICG cells**: instantiate `sky130hd_dfm` (master-slave flop with enable) instead of relying on Yosys inference.
- **Operand isolation on idle datapaths**: AND inputs with enable so they don't toggle in hold cycles.

**Sharing**:
- **Shared multiplier across stages** (FIR + gain): one multiplier MUX-fed between stages. Breaks REQ if REQ pins separate processing per stage — check spec.

**Yosys hints** (RTL attributes, kept through synthesis):
- `(* keep *)` on signal to prevent optimization (sometimes the opposite of what you want)
- `(* dont_touch *)` on cell instances
- `(* fsm_encoding = "one_hot" *)` / `"auto"` on FSM state reg

## Budget rules

- **Never** submit a Phase 4e variant to the server without a green local full-flow run (L14).
- **Never** stack two Phase 4e changes before C-checking each separately (they can oppose — L6).
- **Always** keep the pre-experiment RTL snapshot restorable; a losing variant shouldn't poison the working tree.
- If P improves AND local tests pass, you must bank 1 C-check submission before banking the change as sub-best.

## What this phase is NOT

- Not a substitute for Tier A/B/C (those are done first, they're safe).
- Not a place to experiment with RTL structure without REQ re-audit (Tier C rules still apply).
- Not for new test/assertion work — Phase 3 already covers all REQs (L12).
- Not for chasing V (seed rerolls, see L5 — only P wins justify re-banking).
