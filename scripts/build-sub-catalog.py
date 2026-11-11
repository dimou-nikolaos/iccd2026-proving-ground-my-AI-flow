#!/usr/bin/env python3
"""v2: match by sha256 of tarball bytes to report.json's submission_sha256."""
import os, json, hashlib, subprocess, tarfile
from pathlib import Path

ROOT = Path("/home/ndimou/workspace/ICCD_2026/practice/iccd2026-proving-ground-my-AI-flow")
RUNS = ROOT / "runs"
EXTRACT = Path("/tmp/sub_extract")

subs = sorted([p.name for p in EXTRACT.iterdir() if p.is_dir()])

def tar_path(name):
    for cand in [RUNS / f"{name}.tar.gz", RUNS / "ppa-candidates" / f"{name}.tar.gz"]:
        if cand.exists(): return cand
    return None

# ---------- tar sha256 / md5 ----------
tar_info = {}
for s in subs:
    tp = tar_path(s)
    if tp:
        b = tp.read_bytes()
        tar_info[s] = {
            "path": tp,
            "md5": hashlib.md5(b).hexdigest(),
            "sha256": hashlib.sha256(b).hexdigest(),
            "mtime": tp.stat().st_mtime,
        }
    else:
        tar_info[s] = {"path": None, "md5": None, "sha256": None, "mtime": 0}

# ---------- report lookup by sha256 ----------
report_by_sha = {}
for rj in RUNS.glob("out*/report.json"):
    try:
        j = json.load(open(rj))
        sha = j.get("submission_sha256")
        if sha and sha not in report_by_sha:
            report_by_sha[sha] = j
    except:
        pass

# ---------- content diff for parent inference ----------
def content_diff_files(a, b):
    out = subprocess.run(
        ["diff", "-rq", "--exclude=sim_build*", "--exclude=__pycache__",
         "--exclude=results.xml", "--exclude=*.pyc",
         str(EXTRACT / a), str(EXTRACT / b)],
        capture_output=True, text=True
    )
    files = [l for l in out.stdout.splitlines() if l.startswith("Files ") or l.startswith("Only in ")]
    return len(files)

subs_chrono = sorted(subs, key=lambda s: (tar_info[s]["mtime"], s))
parent_map = {}
for s in subs_chrono:
    if s == "sub-0":
        parent_map[s] = None
        continue
    t_me = tar_info[s]["mtime"]
    cands = []
    for o in subs:
        if o == s: continue
        if tar_info[o]["mtime"] == 0: continue
        if tar_info[o]["mtime"] > t_me: continue
        if tar_info[o]["mtime"] == t_me and o >= s: continue
        n = content_diff_files(s, o)
        cands.append((n, o))
    cands.sort()
    parent_map[s] = cands[0][1] if cands and cands[0][0] < 30 else None  # threshold

# ---------- emit ----------
def parse_report(s):
    sha = tar_info[s]["sha256"]
    if sha and sha in report_by_sha:
        j = report_by_sha[sha]
        return {
            "status": j.get("status", "unknown"),
            "rejected_at": j.get("rejected_at"),
            "V": j.get("V"),
            "P": j.get("P"),
            "stage_msg": {k: v.get("message", "") for k, v in j.get("stages", {}).items()},
            "stage_status": {k: v.get("status", "") for k, v in j.get("stages", {}).items()},
        }
    return {"status": "no-report", "rejected_at": None, "V": None, "P": None,
            "stage_msg": {}, "stage_status": {}}

def make_diff(child, parent):
    if not parent: return ""
    out = subprocess.run(
        ["diff", "-ruN", "--label", f"a/{parent}", "--label", f"b/{child}",
         "--exclude=sim_build*", "--exclude=__pycache__",
         "--exclude=results.xml", "--exclude=*.pyc",
         str(EXTRACT / parent), str(EXTRACT / child)],
        capture_output=True, text=True
    )
    return out.stdout

records = []
for s in subs_chrono:
    r = parse_report(s)
    records.append({
        "sub": s, "parent": parent_map[s],
        "md5": tar_info[s]["md5"],
        "mtime": tar_info[s]["mtime"],
        **r,
    })

# ---------- sub-diffs.md ----------
diff_md = ROOT / "docs" / "designs" / "challenge-1-sub-diffs.md"
with open(diff_md, "w") as f:
    f.write("# challenge-1 submission diffs\n\n")
    f.write("Per-submission unified diff against inferred parent variant. ")
    f.write("Generated 2026-11-11 by sha256-matching each tarball to its flow report.\n\n")
    f.write("## Rebuild recipe (apply in chain order)\n\n")
    f.write("```bash\n")
    f.write("# Walk the parent chain from BASELINE to target, applying each diff in order:\n")
    f.write("tar xzf runs/sub-0.tar.gz -C /tmp/work\n")
    f.write("cd /tmp/work\n")
    f.write("# apply diff for sub-N against parent (see each section below):\n")
    f.write("# manually edit rtl/<file>.sv as per diff hunks\n")
    f.write("# re-tar when done:\n")
    f.write("tar czf runs/sub-TARGET.tar.gz rtl tests assertions report.md\n")
    f.write("```\n\n")
    f.write("Or shortcut: tarballs themselves are in `runs/sub-*.tar.gz` on the working machine. ")
    f.write("The diffs here let you recreate them on a fresh checkout without needing the original tarballs.\n\n")
    f.write("## Index (chronological by tarball mtime)\n\n")
    f.write("| # | sub | parent | status | V | P | tar md5 |\n")
    f.write("|---|-----|--------|--------|---|---|--------|\n")
    for i, r in enumerate(records, 1):
        v = f"{r['V']:.3f}" if r['V'] is not None else "—"
        p = f"{r['P']:.4f}" if r['P'] is not None else "—"
        st = r['status'] if r['status'] != "rejected" else f"rejected@{r.get('rejected_at','?')}"
        md5 = (r['md5'] or "—")[:12]
        par = r['parent'] or "BASELINE"
        f.write(f"| {i} | [{r['sub']}](#{r['sub']}) | `{par}` | {st} | {v} | {p} | `{md5}` |\n")
    f.write("\n---\n\n")
    for r in records:
        s = r['sub']
        f.write(f"## {s}\n\n")
        f.write(f"- **parent**: `{r['parent'] or 'BASELINE (released)'}`\n")
        f.write(f"- **tar md5**: `{r['md5'] or '—'}`  tar sha256: `{tar_info[s]['sha256'][:24]}...` (full sha256 in flow report)\n")
        st = r['status']
        if st == "rejected":
            f.write(f"- **flow status**: rejected at `{r.get('rejected_at')}` — {r['stage_msg'].get(r.get('rejected_at',''),'')}\n")
        else:
            bits = [f"status: {st}"]
            if r['V'] is not None: bits.append(f"V={r['V']:.3f}")
            if r['P'] is not None: bits.append(f"P={r['P']:.4f}")
            # PPA breakdown if available
            for stage in r.get('stage_status', {}):
                if r['stage_status'][stage] and r['stage_msg'].get(stage):
                    pass
            f.write(f"- **flow**: {', '.join(bits)}\n")
            # PPA detail from stages message
            for k, m in r['stage_msg'].items():
                if m:
                    f.write(f"  - {k}: {m}\n")
        f.write("\n")
        if r['parent'] is None:
            f.write("_(baseline archive — start of chain)_\n\n---\n\n")
            continue
        d = make_diff(s, r['parent'])
        if not d.strip():
            f.write("_(diff vs parent is empty — identical content)_\n\n---\n\n")
            continue
        if len(d) > 80000:
            d = d[:80000] + "\n... [truncated] ...\n"
        f.write("```diff\n")
        f.write(d)
        f.write("\n```\n\n---\n\n")

print(f"Wrote {diff_md} ({os.path.getsize(diff_md)} bytes)")

# ---------- sub-catalog.md ----------
cat_md = ROOT / "docs" / "designs" / "challenge-1-sub-catalog.md"
with open(cat_md, "w") as f:
    f.write("# challenge-1 submission catalog\n\n")
    f.write("Every submission archive produced during the challenge-1 campaign. ")
    f.write("Parent inferred by closest content among earlier-mtime variants. ")
    f.write("V/P/status matched by sha256 of tarball bytes against flow `report.json` `submission_sha256` field. ")
    f.write("137/138 tarballs matched to a flow report (sub-0's report was the bare `runs/out/`, no sha256 stored).\n\n")
    f.write("Per-submission rebuild diffs: `challenge-1-sub-diffs.md`.\n\n")
    f.write("## Status legend\n\n")
    f.write("- `scored` — passed all flow stages, V and P reported\n")
    f.write("- `rejected@build` — Verilator lint/elab failed\n")
    f.write("- `rejected@public` — cocotb tests failed with assertions bound\n")
    f.write("- `rejected@mutation` or `@ppa` — failed at later stage\n")
    f.write("- `no-report` — tarball exists but no flow run output captured\n\n")
    f.write("## Catalog (chronological by tarball mtime)\n\n")
    f.write("| # | sub | parent | status | V | P | tar md5 |\n")
    f.write("|---|-----|--------|--------|---|---|----------|\n")
    for i, r in enumerate(records, 1):
        v = f"{r['V']:.3f}" if r['V'] is not None else "—"
        p = f"{r['P']:.4f}" if r['P'] is not None else "—"
        st = r['status'] if r['status'] != "rejected" else f"rejected@{r.get('rejected_at','?')}"
        md5 = (r['md5'] or "—")[:12]
        par = r['parent'] or "BASELINE"
        f.write(f"| {i} | `{r['sub']}` | `{par}` | {st} | {v} | {p} | `{md5}` |\n")
    f.write("\n## Rebuild notes\n\n")
    f.write("- Tarballs `sub-0`..`sub-16` form the **core promoted chain** (baseline + seeded fixes + CR + Tier A/B PPA wins).\n")
    f.write("- `sub-w*` `sub-g*` `sub-h*` `sub-e*` `sub-v*` are P-perturbation / Explorer variants layered on sub-16.\n")
    f.write("- `sub-acc-narrow`, `sub-fifo-rm`, `sub-fir-parallel`, `sub-fuse-lim`, `sub-gain3` are pre-16 PPA experiments in `runs/ppa-candidates/`.\n")
    f.write("- For exact per-variant diff, see matching section in `challenge-1-sub-diffs.md`.\n")

print(f"Wrote {cat_md} ({os.path.getsize(cat_md)} bytes)")
