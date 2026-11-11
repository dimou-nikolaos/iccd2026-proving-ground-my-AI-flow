#!/usr/bin/env python3
"""Generate docs/designs/challenge-2-sub-catalog.md.

Matches every runs/c2-sub-*.tar.gz (md5+sha256) to a flow report under runs/out-*
via report.json submission_sha256; joins with the human verdict table kept in
runs/c2-best.md (kept as source of truth during the campaign; copied into this
doc as the Notes column by gen-catalog-c2.py when a row matches on archive name).
Usage: python3 scripts/gen-sub-catalog-c2.py
"""
import json, hashlib
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
RUNS = ROOT / "runs"
OUT = ROOT / "docs" / "designs" / "challenge-2-sub-catalog.md"

rep_by_sha = {}
for rj in list(RUNS.glob("out*/report.json")) + list(RUNS.glob("out*/**/report.json")):
    try:
        j = json.load(open(rj))
        sha = j.get("submission_sha256")
        if sha and sha not in rep_by_sha:
            rep_by_sha[sha] = (j, rj)
    except Exception:
        pass

rows = []
for tp in sorted(RUNS.glob("c2-sub-*.tar.gz"), key=lambda p: p.stat().st_mtime):
    b = tp.read_bytes()
    sha = hashlib.sha256(b).hexdigest()
    md5 = hashlib.md5(b).hexdigest()
    hit = rep_by_sha.get(sha)
    if hit:
        j, rj = hit
        status = j.get("status") or "unknown"
        if j.get("rejected_at"):
            status = f"rejected@{j['rejected_at']}"
        v = j.get("V")
        p = j.get("P")
        rows.append((tp.name, status, v, p, md5, str(rj.relative_to(RUNS))))
    else:
        rows.append((tp.name, "no-report", None, None, md5, "—"))

lines = []
lines.append("# challenge-2 submission catalog\n")
lines.append("All `runs/c2-sub-*.tar.gz` archives (gitignored, regenerable via "
             "`scripts/` + `docs/designs/challenge-2-sub-diffs.md` + `docs/state/challenge-2-progress.md` rebuild logs). "
             "Matched to flow reports by `submission_sha256`. Human verdicts live in `runs/c2-best.md` "
             "(this table's Notes column is curated there first, then copied here).\n")
lines.append("Legend: `scored` = full flow passed (V,P from report); `no-report` = archived only; "
             "`contract-change` (Notes) = ships MODIFIED assertions vs family-A goldens -> hidden-assert C-risk, "
             "late-slot/Explorer only (L60).\n")
lines.append(f"Archives: {len(rows)}. Distinct scored netlists: see progress-file Explorer counts.\n")
lines.append("| archive | status | V | P | tar md5 | flow out |")
lines.append("|---|---|---|---|---|---|")
for name, status, v, p, md5, rout in rows:
    vv = f"{v:.3f}" if isinstance(v, (int, float)) else "—"
    pp = f"{p:.4f}" if isinstance(p, (int, float)) else "—"
    lines.append(f"| `{name}` | {status} | {vv} | {pp} | `{md5[:12]}…` | {rout} |")

OUT.write_text("\n".join(lines) + "\n")
print(f"wrote {OUT}: {len(rows)} archives, "
      f"{sum(1 for r in rows if r[1]=='scored')} matched to reports")
