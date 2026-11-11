#!/usr/bin/env python3
"""Update sub-diffs.md and sub-catalog.md by matching tarball SHA256 to flow reports."""
import os, json, hashlib, re
from pathlib import Path

ROOT = Path("/home/ndimou/workspace/ICCD_2026/practice/iccd2026-proving-ground-my-AI-flow")
RUNS = ROOT / "runs"

# Build map: tar_sha256 -> tarball path
tar_paths = list(RUNS.glob("sub-*.tar.gz")) + list(RUNS.glob("ppa-candidates/sub-*.tar.gz"))
tar_by_sha = {}
tar_by_name = {}
for tp in tar_paths:
    sha = hashlib.sha256(tp.read_bytes()).hexdigest()
    name = tp.stem
    tar_by_sha[sha] = tp
    tar_by_name[name] = sha

# Build map: sha256 -> report.json contents
report_by_sha = {}
for rj in RUNS.glob("out*/report.json"):
    try:
        j = json.load(open(rj))
        sha = j.get("submission_sha256")
        if sha:
            # Store first occurrence (multiple configs may match; we want the canonical one)
            if sha not in report_by_sha:
                report_by_sha[sha] = {"path": rj, "json": j}
    except:
        pass

print(f"Tarballs: {len(tar_paths)}")
print(f"Reports with sha256: {len(report_by_sha)}")

# Show which tarballs matched
matched = 0
unmatched_tar = []
for tp in tar_paths:
    sha = hashlib.sha256(tp.read_bytes()).hexdigest()
    if sha in report_by_sha:
        matched += 1
    else:
        unmatched_tar.append(tp.name)
print(f"Matched: {matched}/{len(tar_paths)}")
print(f"Unmatched tarballs (first 10): {unmatched_tar[:10]}")

# Show unmatched reports (out dirs without corresponding tarball — renamed/deleted)
matched_shas = {hashlib.sha256(tp.read_bytes()).hexdigest() for tp in tar_paths}
unmatched_reports = [rj for sha, r in report_by_sha.items() if sha not in matched_shas for rj in [r["path"]]]
print(f"Unmatched reports: {len(unmatched_reports)}")
for r in unmatched_reports[:10]:
    print(f"  {r}")
