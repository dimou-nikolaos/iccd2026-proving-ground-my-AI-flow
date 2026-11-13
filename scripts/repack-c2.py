#!/usr/bin/env python3
"""Repack challenge-2 submission tars to the competition layout:

    rtl/          same files as rtl/filelist.f
    tests/        test_*.py + helpers (+ Makefile used by the flow)
    assertions/   *.sv with bind
    report.md

Only build junk is removed (sim_build*/, __pycache__/, *.pyc, results.xml).
All other members are copied byte-identical with metadata preserved.
Originals are kept under runs/dirty-backup/.
"""
import glob
import os
import re
import shutil
import sys
import tarfile

JUNK = re.compile(r'(?:^|/)(sim_build[^/]*|__pycache__)(?:/|$)|results\.xml$|\.pyc$')
TOP_ORDER = ['rtl', 'tests', 'assertions', 'report.md']

def top_of(name):
    return name.split('/')[0]

def repack(path, backup_dir):
    out = path + '.new'
    removed = 0
    with tarfile.open(path, 'r:gz') as src:
        members = [m for m in src.getmembers() if not JUNK.search(m.name)]
        removed = len(src.getmembers()) - len(members)
        # deterministic top-level order, preserve intra-dir original order
        order = {t: i for i, t in enumerate(TOP_ORDER)}
        members.sort(key=lambda m: (order.get(top_of(m.name), 99), m.name))
        with tarfile.open(out, 'w:gz') as dst:
            for m in members:
                f = src.extractfile(m)
                dst.addfile(m, f)
    if removed:
        bkp = os.path.join(backup_dir, os.path.basename(path))
        if os.path.exists(bkp):
            os.remove(path)  # true original already banked in backup dir
        else:
            shutil.move(path, bkp)
        shutil.move(out, path)
    else:
        os.remove(out)
    return removed

def main():
    backup_dir = 'runs/dirty-backup'
    os.makedirs(backup_dir, exist_ok=True)
    targets = sorted(glob.glob('runs/c2-sub-*.tar.gz')) + ['runs/sub-best.tar.gz']
    for t in targets:
        r = repack(t, backup_dir)
        if r:
            print(f"{t}: removed {r} junk entries")
    return 0

if __name__ == '__main__':
    sys.exit(main())
