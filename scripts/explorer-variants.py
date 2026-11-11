#!/usr/bin/env python3
"""explorer-variants.py — mechanical Explorer-variant generator (design-agnostic).

Copies a banked-core directory to a variant directory and applies ONE conservative
edit class. Every output is expected to be gated by scripts/explorer-check.sh
(yosys equivalence + netlist fingerprint) — this script never claims safety itself.

Usage (usually via scripts/explorer-variants.sh, not directly):
  explorer-variants.py <core-dir> <variant-dir> --class CLASS [--name IDENT] [--seed N]

Classes:
  rename      rename one internal signal across rtl/*.sv + assertions/*.sv
              (consistency keeps binds working; ports are never renamed)
  buffer      add (* keep *) wire <net>_bx = <net>; and redirect consumers to it
  keep-reg    (* keep = "true" *) on a register declaration (_q-suffix preferred)
  fsm-onehot  (* fsm_encoding = "one_hot" *) on an FSM state register declaration
  reroll      comment-only change (V-seed reroll; the fingerprint gate will
              report DUPLICATE — that is expected, it is not a GREEN candidate)

Host-side: pure python stdlib only. All real checking happens in the flow image.
"""
import argparse
import re
import shutil
import sys
from pathlib import Path

SV_KEYWORDS = set(
    "module endmodule input output inout logic wire reg always_ff always_comb always "
    "assign if else case endcase unique priority for begin end function endfunction "
    "typedef enum struct packed signed unsigned parameter localparam int bit byte "
    "posedge negedge or and not xor initial return void".split()
)


def sv_files(variant: Path):
    out = []
    for sub in ("rtl", "assertions"):
        d = variant / sub
        if d.is_dir():
            out += sorted(d.rglob("*.sv"))
    if not out:  # bare rtl dir layout
        out += sorted((variant / "rtl").rglob("*.sv"))
    return out


def port_names(text: str):
    """Identifiers declared input/output/inout anywhere (never rename these)."""
    return set(re.findall(r"\b(?:input|output|inout)\b\s+(?:var\s+)?(?:reg|logic|wire)?[^\s,;()]*\s+(\w+)", text))


def declarations(text: str):
    """(name, kind) for internal nets/regs declared in this file.
    kind: 'net' (wire/logic with continuous assign), 'reg' (_q regs)."""
    decls = []
    for m in re.finditer(
        r"^\s*(localparam\b|(?:wire|logic|reg)\s+(?:signed\s+)?(?:\[[^\]]*\]\s*)*)(\w+)",
        text, re.M,
    ):
        kind, name = m.group(1), m.group(2)
        if kind == "localparam\b" or name in SV_KEYWORDS:
            continue
        decls.append((name, kind.strip()))
    return decls


def count_word(text: str, name: str) -> int:
    return len(re.findall(rf"\b{re.escape(name)}\b", text))


def pick_rename_candidate(files, want: str | None, pick: int = 0):
    """Internal signal safe-ish to rename: single declaration, no port collision,
    3..40 whole-word occurrences total. Deterministic order (file, name).
    pick = index among equally-preferred candidates (for multi-variant batches)."""
    all_text = "".join(f.read_text() for f in files)
    pnames = port_names(all_text)
    scored = []
    for f in files:
        t = f.read_text()
        declared = [d for d in set(n for n, _ in declarations(t))]
        for name in declared:
            if want and name != want:
                continue
            if name in pnames:
                continue
            total = count_word(all_text, name)
            if not (3 <= total <= 40):
                continue
            # skip params-ish names (ALLCAPS) — width params are touched everywhere
            if name.isupper():
                continue
            n_decl = len(re.findall(rf"^\s*(?:wire|logic|reg)\b[^\n=;]*\b{re.escape(name)}\s*(?:=|;|\[)", t, re.M))
            scored.append((f.name, name, total, n_decl))
    scored.sort()
    ok = [s for s in scored if s[3] == 1]
    return ok[pick][1] if pick < len(ok) else None


def apply_rename(variant: Path, ident: str, pick: int = 0) -> str:
    files = sv_files(variant)
    all_text = "".join(f.read_text() for f in files)
    name = pick_rename_candidate(files, ident, pick)
    if not name:
        raise SystemExit("rename: no candidate found (use --name to force)")
    new = f"{name}_ex"
    for f in files:
        t = f.read_text()
        if re.search(rf"\b{re.escape(name)}\b", t):
            f.write_text(re.sub(rf"\b{re.escape(name)}\b", new, t))
    return f"rename {name} -> {new}"


def pick_buffer_net(files, want: str | None, pick: int = 0):
    """Combinational net driven by exactly one `assign net = ...;` and read elsewhere."""
    cands = []
    for f in files:
        t = f.read_text()
        for name, _ in declarations(t):
            if want and name != want:
                continue
            if name in port_names(t) or name.isupper():
                continue
            if re.search(rf"\b{re.escape(name)}\b\s*(?:<=|<='|<$)", t):  # reg-driven: skip
                continue
            n_assign = len(re.findall(rf"^\s*assign\s+{re.escape(name)}\s*=", t, re.M))
            if n_assign != 1:
                continue
            total = count_word(t, name)
            if total < 3:
                continue
            cands.append((f.name, name, total, f))
    cands.sort()
    return cands[pick] if pick < len(cands) else None


def apply_buffer(variant: Path, want: str | None, pick: int = 0) -> str:
    best = pick_buffer_net(sv_files(variant), want, pick)
    if not best:
        raise SystemExit("buffer: no candidate net found")
    _, name, _, f = best
    t = f.read_text()
    new = f"{name}_bx"
    decl_pat = rf"^\s*(wire|logic|reg)\b[^\n]*\b{re.escape(name)}\s*(=|;|\[|,)"
    driv_pat = rf"^\s*assign\s+{re.escape(name)}\s*="
    out = []
    for ln in t.splitlines(keepends=True):
        if re.match(driv_pat, ln):
            out.append(ln)
            indent = ln[: len(ln) - len(ln.lstrip())]
            out.append(f"{indent}(* keep *) wire {new} = {name};  // explorer-buffer-marker\n")
        elif re.match(decl_pat, ln) or "explorer-buffer-marker" in ln:
            out.append(ln)
        elif re.search(rf"\b{re.escape(name)}\b", ln):
            out.append(re.sub(rf"\b{re.escape(name)}\b", new, ln))
        else:
            out.append(ln)
    f.write_text("".join(out))
    return f"buffer: {name} -> kept copy {new} in {f.name}"


def pick_q_reg(files, want: str | None, fsm: bool, pick: int = 0):
    """Register declaration to receive an attribute. fsm: prefer state-like names."""
    cands = []
    for f in files:
        t = f.read_text()
        for name, _ in declarations(t):
            if want and name != want:
                continue
            if name in port_names(t) or name.isupper():
                continue
            if not re.search(rf"{re.escape(name)}\s*<?=", t):  # must be driven by a flop
                continue
            is_state = bool(re.search(r"(state|st_q|fsm|stage)", name, re.I))
            looks_q = name.endswith("_q") or name.endswith("_ff") or is_state
            if fsm and not is_state:
                continue
            if not looks_q:
                continue
            cands.append((f.name, name, f))
    cands.sort()
    return cands[pick] if pick < len(cands) else None


def apply_attr(variant: Path, want, attr: str, fsm: bool, pick: int = 0) -> str:
    hit = pick_q_reg(sv_files(variant), want, fsm, pick)
    if not hit:
        raise SystemExit(f"attr {attr}: no candidate register found")
    _, name, f = hit
    t = f.read_text()
    pat = rf"^(\s*)((?:wire|logic|reg)\s+(?:signed\s+)?(?:\[[^\]]*\]\s*)*{re.escape(name)}\s*(?:=|;|\[|,))"
    m = re.search(pat, t, re.M)
    assert m, f"declaration line for {name} not found"
    def _attr_ins(mm):
        indent, decl = mm.group(1), mm.group(2)
        m2 = re.match(rf"^(\s*(?:wire|logic|reg)\s+(?:signed\s+)?(?:\[[^\]]*\]\s*)?)({re.escape(name)})\s*(.*)$", decl, re.S)
        kw, rest = m2.group(1), m2.group(3)
        if not rest.startswith(','):
            return f'{indent}(* {attr} *) {decl}'
        rest = rest.lstrip()[1:].lstrip()  # drop the comma that split the list
        return f'{indent}(* {attr} *) {kw}{name};\n{indent}{kw}{rest}'
    t = re.sub(pat, _attr_ins, t, count=1, flags=re.M)
    f.write_text(t)
    return f"attr (* {attr} *) on {name} in {f.name}"


def apply_reroll(variant: Path, seed: int) -> str:
    files = sv_files(variant)
    f = files[0]
    t = f.read_text()
    marker = t.rstrip().splitlines()[-1]
    if "explorer-seed-reroll" in t:
        t = re.sub(r"// explorer-seed-reroll \d+", f"// explorer-seed-reroll {seed}", t)
    else:
        t += f"\n// explorer-seed-reroll {seed}\n"
    f.write_text(t)
    return f"reroll comment appended to {f.name}"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("core")
    ap.add_argument("variant")
    ap.add_argument("--class", dest="cls", required=True,
                    choices=["rename", "buffer", "keep-reg", "fsm-onehot", "reroll"])
    ap.add_argument("--name", help="force the candidate identifier")
    ap.add_argument("--pick", type=int, default=0, help="rotate to Nth candidate (multi-variant batches)")
    ap.add_argument("--seed", type=int, default=0)
    a = ap.parse_args()

    core, variant = Path(a.core).resolve(), Path(a.variant).resolve()
    if variant.exists():
        shutil.rmtree(variant)
    shutil.copytree(core, variant)

    if a.cls == "rename":
        desc = apply_rename(variant, a.name, a.pick)
    elif a.cls == "buffer":
        desc = apply_buffer(variant, a.name, a.pick)
    elif a.cls == "keep-reg":
        desc = apply_attr(variant, a.name, 'keep = "true"', fsm=False, pick=a.pick)
    elif a.cls == "fsm-onehot":
        desc = apply_attr(variant, a.name, 'fsm_encoding = "one_hot"', fsm=True, pick=a.pick)
    else:
        desc = apply_reroll(variant, a.seed)
    print(desc)


if __name__ == "__main__":
    main()
