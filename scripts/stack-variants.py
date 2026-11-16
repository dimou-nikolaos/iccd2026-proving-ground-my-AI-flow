#!/usr/bin/env python3
"""Stack RTL file-variants from multiple trees onto a base tree.
usage: stack-variants.py <base_tree> <out_tree> <mod_tree1> [mod_tree2 ...]
Only files that DIFFER from base in ANY mod_tree are copied (last tree wins on
conflicts; conflicts are reported and abort unless --force)."""
import sys, shutil, filecmp, os
from pathlib import Path

def rel_files(tree):
    root = Path(tree) / 'rtl'
    return {str(p.relative_to(root)): p for p in root.glob('*.sv')}

def main():
    args = [a for a in sys.argv[1:] if a != '--force']
    # NOTE: base tree must be the UNMODIFIED baseline (live challenge-3);
    # using one variant as base silently reverts other variants' edits to it.
    force = '--force' in sys.argv
    base, out, mods = args[0], args[1], args[2:]
    shutil.rmtree(out, ignore_errors=True)
    shutil.copytree(base, out)
    bf = rel_files(base)
    picked = {}
    for m in mods:
        for name, p in rel_files(m).items():
            if not filecmp.cmp(p, bf[name], shallow=False):
                if name in picked and not force:
                    sys.exit(f"CONFLICT on {name}: {picked[name]} vs {m}")
                picked[name] = p
    for name, p in picked.items():
        shutil.copy(p, Path(out) / 'rtl' / name)
        print(f"stacked rtl/{name}  <- {p}")
    print("stack done ->", out)

main()
