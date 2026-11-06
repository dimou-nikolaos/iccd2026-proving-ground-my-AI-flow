# Submission report.md Template

Keep it under one page.

```markdown
# Submission report: <design-id> (<top_module>)

## Deviations found

| # | Requirement | Location in the baseline | Symptom | Found by |
|---|---|---|---|---|
| 1 | REQ-XX (short title) | `rtl/file.sv`, expression | Human-readable consequence: "wrong output when..." or "interrupt lost when..." | `test_name`, assertion `assertion_name` |
| 2 | REQ-YY (short title) | ... | ... | ... |
| 3 | REQ-ZZ (short title) | ... | ... | ... |

## Changes made

1. `file.sv`: description of fix (minimal, one sentence per change).
2. `other.sv`: description...
3. Change request CR-N: what was added (brief).

Ports, the filelist and the clock constraint are unchanged.

## How it was verified

- `tests/test_submission.py`: N cocotb tests run by `tests/run.sh`, with a Python model in `tests/helper.py`. X tests target the deviations and fail on the baseline; Y tests target the CR; Z cover the rest of the specification.
- `assertions/file.sva`, bound into modules A and B: brief list of what's checked. Assertion `name` fires on the baseline during test X.
- Results on this submission: N of N tests pass with assertions enabled. On the baseline (tests alone): M of N fail.

Not covered by these tests: <be honest: throughput, counter wrap, rare corner cases>.
```

## Notes on format

- **Symptom column**: explains what goes WRONG in observable behavior (helps hidden-check evaluators understand you understand the bug)
- **Found by column**: links each deviation to specific evidence (test name or assertion name)
- **"Not covered" section**: shows you know the limits. If hidden checks test something you missed, this section at least demonstrates awareness. Don't leave it empty — even say "all spec requirements are covered."
- **Compliance line**: always state "Ports, the filelist and the clock constraint are unchanged" — proves you know the rules
- **Keep it factual**: no marketing, no "we believe" — state what was done and what the results are
