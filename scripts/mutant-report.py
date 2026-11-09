#!/usr/bin/env python3
"""Render a pgflow mutation stage as an actionable table.

V is computed locally, and `stages/mutation.json` contains every selected
mutant with its *exact* mutated expression and the reason it was (or was not)
killed. This turns "V=0.875" into "these 5 expressions survived, here is what
they became", which is what you need to write the next assertion.

Usage:
  scripts/mutant-report.py runs/challenge-2-out              # survivors + stats
  scripts/mutant-report.py runs/challenge-2-out --all        # every mutant
  scripts/mutant-report.py runs/challenge-2-out --md         # markdown (paste in docs)
  scripts/mutant-report.py runs/challenge-2-out --class arith

Exit status: 0 if every counted mutant was killed, 1 if any survived.
"""

from __future__ import annotations

import argparse
import collections
import json
import sys
from pathlib import Path


def load(path: Path) -> dict:
    if path.is_dir():
        path = path / "stages" / "mutation.json"
    data = json.loads(path.read_text())
    # stages/*.json wraps the stage result; accept both shapes
    return data.get("details", data)


def reason_kind(reason: str) -> str:
    if not reason:
        return "survived"
    if reason.startswith("test failed"):
        return "test"
    if reason == "assertion":
        return "assertion"
    if reason == "timeout":
        return "timeout"
    if reason.startswith("simulation error"):
        return "sim-error"
    if reason.startswith("mutant did not build"):
        return "invalid"
    return reason


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("out", type=Path, help="flow --out directory (or a mutation.json)")
    ap.add_argument("--all", action="store_true", help="list every mutant, not just survivors")
    ap.add_argument("--md", action="store_true", help="markdown output")
    ap.add_argument("--class", dest="klass", action="append", help="filter by operator class")
    args = ap.parse_args(argv)

    det = load(args.out)
    mutants = det.get("mutants") or []
    counted = [m for m in mutants if m.get("counted", True)]
    killed = [m for m in counted if m.get("killed")]
    survivors = [m for m in counted if not m.get("killed")]
    if args.klass:
        keep = set(args.klass)
        mutants = [m for m in mutants if m.get("class") in keep]
        counted = [m for m in mutants if m.get("counted", True)]
        killed = [m for m in counted if m.get("killed")]
        survivors = [m for m in counted if not m.get("killed")]

    sel = det.get("selection") or {}
    method = det.get("method", "?")
    v = det.get("V")
    if v is None and counted:
        v = len(killed) / len(counted)

    if args.md:
        print(f"**V = {v:.3f}** ({len(killed)}/{len(counted)} killed, method `{method}`)\n")
        print("| class | sites | selected | killed | survived |")
        print("|---|---|---|---|---|")
        sites = sel.get("sites_by_class") or {}
        for cls in ("arith", "relational", "constant", "negate", "drop_term", "off_by_one"):
            sel_n = sum(1 for m in mutants if m.get("class") == cls)
            if not sel_n:
                continue
            k = sum(1 for m in mutants if m.get("class") == cls and m.get("killed"))
            print(f"| {cls} | {sites.get(cls, '?')} | {sel_n} | {k} | {sel_n - k} |")
    else:
        print(f"V = {v:.4f}  ({len(killed)}/{len(counted)} counted mutants killed)")
        print(f"method={method}  n_mutants={det.get('n_mutants')}  "
              f"per_mutant_timeout={det.get('per_mutant_timeout')}s")
        if det.get("selection_seconds") is not None:
            print(f"selection={det['selection_seconds']}s  "
                  f"schemata_build={det.get('schemata_build_seconds')}s  "
                  f"run={det.get('run_seconds')}s  "
                  f"sanity={det.get('schemata_sanity')}")
        print()
        sites = sel.get("sites_by_class") or {}
        print(f"{'class':<12}{'sites':>6}{'sel':>5}{'killed':>7}{'asser':>6}{'test':>5}{'other':>6}{'surv':>5}")
        for cls in ("arith", "relational", "constant", "negate", "drop_term", "off_by_one"):
            sel_m = [m for m in mutants if m.get("class") == cls]
            if not sel_m:
                continue
            kinds = collections.Counter(reason_kind(m.get("reason", "")) for m in sel_m
                                        if m.get("counted", True))
            other = kinds["timeout"] + kinds["sim-error"] + kinds["invalid"]
            print(f"{cls:<12}{sites.get(cls, '?'):>6}{len(sel_m):>5}{len(sel_m) - kinds['survived']:>7}"
                  f"{kinds['assertion']:>6}{kinds['test']:>5}{other:>6}{kinds['survived']:>5}")

    rows = mutants if args.all else survivors
    if rows:
        print(("\n## " if args.md else "\n") + ("mutants" if args.all else "survivors"))
        if args.md:
            print("\n| id | class | location | original | mutated | verdict |")
            print("|---|---|---|---|---|---|")
            for m in rows:
                print(f"| {m['id']} | {m['class']} | `{m['file']}:{m['line']}` | "
                      f"`{m['original']}` | `{m['mutated']}` | {reason_kind(m.get('reason', ''))} |")
        else:
            for m in rows:
                verdict = reason_kind(m.get("reason", ""))
                print(f"  #{m['id']:<3} {m['class']:<11} {m['file']}:{m['line']}  "
                      f"{m['description']}  [{verdict}]")
                print(f"        {m['original']}")
                print(f"     -> {m['mutated']}")
    elif not args.all:
        print("\nno survivors")
    return 0 if not survivors else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
