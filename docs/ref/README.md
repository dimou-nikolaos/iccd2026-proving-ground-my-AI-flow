# Reference specifications (offline copies)

Fetched 2026-11-08 so Challenge-3 work does not depend on network access or on browsing
the RISC-V manual during the competition window.

| File | Source |
|---|---|
| `riscv-spec-riscv-isa-release-717aeca-2026-11-07.pdf` | https://github.com/riscv/riscv-isa-manual/releases/download/riscv-isa-release-717aeca-2026-11-07/riscv-spec.pdf |
| `norm-rules-riscv-isa-release-717aeca-2026-11-07.json` | same release, `norm-rules.json` |

Release tag: `riscv-isa-release-717aeca-2026-11-07`. The PDF is the combined
unprivileged + privileged + bit-manipulation volume (909 pages).

SHA-256:

```
20ef6e3fe7578a50e8777b9d6c7ef89df3dc63a7d5b7d62114f5073a5d3b14c7  riscv-spec-*.pdf
f79ca313f0a5834e26cc842a4f09ba38bcae7c29fb6ee9d38741d57f322eb8cf  norm-rules-*.json
```

## What to use it for on release day

- **CR-D3 encodings** (already verified, re-check against the spec text if the released
  spec is ambiguous): `SH1ADD/SH2ADD/SH3ADD` = opcode `0x33`, `funct7=0010000`,
  `funct3=010/100/110`; `MIN/MINU/MAX/MAXU` = opcode `0x33`, `funct7=0000101`,
  `funct3=100/101/110/111`.
- **Trap and CSR behavior** that the released spec may or may not spell out: `mstatus`
  `MPP`/`MPIE` update on trap and `MRET`, `mtvec` direct vs vectored, `mepc` bit-0
  clearing, `mcause` codes, `mtval` on illegal instruction / misaligned access, WARL
  fields, read-only CSR write behavior.
- **Misaligned access policy** — the spec allows either trapping or handling; the
  released spec wins. `prep/rviss/rv32_iss.py` has a policy knob for each case.
- **Normative rules** (`norm-rules-*.json`) for wording-level checks when the released
  spec is terse.

## Caveats

- `releases/latest` moves; the filename carries the tag, so this copy stays reproducible.
- The flow's assembler/testbench may implement a subset; the *released spec.md* is the
  authority for grading, this PDF only resolves "what does the ISA say".
