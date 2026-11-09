#!/usr/bin/env bash
# Release-day bootstrap for one challenge design.
#
#   scripts/design-init.sh 2 /path/to/released/challenge-2
#   scripts/design-init.sh 3 /path/to/released/challenge-3
#   scripts/design-init.sh 2                 # bundle already copied to ./challenge-2
#
# Does, in order (idempotent — never overwrites existing docs):
#   1. copies the released bundle to ./challenge-N
#   2. verifies the layout the flow depends on
#   3. creates the pristine baseline/ mirror for the PPA cache
#   4. creates docs/designs/challenge-N.md, ...-req-matrix.md, docs/state/...-progress.md
#   5. prints design.yaml facts, filelist, REQ count, public test modules,
#      and the exact next commands (public tests, baseline PPA cache)
#
# Does NOT run any Docker command; run those yourself from the printed list.
set -euo pipefail

if [ $# -lt 1 ]; then
    sed -n '2,16p' "$0" | sed 's/^# \{0,1\}//'
    exit 2
fi

N=$1
SRC=${2:-}
repo=$(cd "$(dirname "$0")/.." && pwd)
cd "$repo"
D="challenge-$N"

say()  { printf '%s\n' "$*"; }
warn() { printf 'WARN  %s\n' "$*" >&2; }
die()  { printf 'ERROR %s\n' "$*" >&2; exit 1; }

# ---------------------------------------------------------------- 1. copy
if [ -n "$SRC" ]; then
    [ -d "$SRC" ] || die "source bundle not found: $SRC"
    if [ -e "$D" ]; then
        die "$D already exists; remove it or run without the source argument"
    fi
    cp -r "$SRC" "$D"
    say "copied $SRC -> $D"
elif [ -d "$D" ]; then
    say "using existing $D"
else
    die "no source argument and ./$D does not exist"
fi

# ---------------------------------------------------------------- 2. verify
missing=0
for f in spec.md change_request.md design.yaml rtl/filelist.f flow/constraint.sdc; do
    if [ -e "$D/$f" ]; then
        say "  ok  $D/$f"
    else
        warn "missing $D/$f"
        missing=1
    fi
done
[ -d "$D/tb/public" ] || warn "missing $D/tb/public (public testbench)"
[ "$missing" -eq 0 ] || die "released bundle is incomplete — do not start Phase 1"

# ---------------------------------------------------------------- 3. baseline mirror
mkdir -p "$D/baseline"
for f in rtl tb flow design.yaml; do
    [ -e "$D/$f" ] || continue
    if [ -e "$D/baseline/$f" ]; then
        warn "$D/baseline/$f exists — leaving it alone (must stay pristine)"
    else
        cp -r "$D/$f" "$D/baseline/$f"
    fi
done
say "baseline mirror: $D/baseline (used as --design for honest PPA)"

# ---------------------------------------------------------------- 4. docs
mkdir -p docs/designs docs/state
meta=$(python3 - "$D" <<'PY'
import json, sys, pathlib, yaml
d = pathlib.Path(sys.argv[1])
y = yaml.safe_load((d / "design.yaml").read_text()) or {}
fl = d / str(y.get("filelist", "rtl/filelist.f"))
files = [l.split("//", 1)[0].split("#", 1)[0].strip() for l in fl.read_text().splitlines()]
files = [f for f in files if f]
print(json.dumps({
    "top": y.get("top"), "clock": y.get("clock_port", "clk"),
    "reset": y.get("reset_port", "rst_n"),
    "period": y.get("clock_period_ns", 10.0),
    "id": y.get("id", d.name), "lines": y.get("lines", "?"),
    "filelist": files,
}))
PY
)
top=$(printf '%s' "$meta" | python3 -c 'import json,sys;print(json.load(sys.stdin)["top"])')
period=$(printf '%s' "$meta" | python3 -c 'import json,sys;print(json.load(sys.stdin)["period"])')
flist=$(printf '%s' "$meta" | python3 -c 'import json,sys;print("\n".join("  "+f for f in json.load(sys.stdin)["filelist"]))')
reqs=$(grep -ohE 'REQ-[0-9]+' "$D/spec.md" 2>/dev/null | sort -uV || true)
nreq=$(printf '%s\n' "$reqs" | sed '/^$/d' | wc -l | tr -d ' ')

if [ ! -e "docs/designs/challenge-$N.md" ]; then
    cat > "docs/designs/challenge-$N.md" <<EOF
# challenge-$N — design knowledge ($top)

> Created by \`scripts/design-init.sh\`. Fill in during Phase 1; keep signal names
> accurate, they are the bind targets for every assertion.

## Top / timing
- Top module: \`$top\`
- Clock / reset: \`$(printf '%s' "$meta" | python3 -c 'import json,sys;d=json.load(sys.stdin);print(d["clock"]+" / "+d["reset"])')\`
- Clock period: ${period} ns (sky130hd)
- RTL size: $(printf '%s' "$meta" | python3 -c 'import json,sys;print(json.load(sys.stdin)["lines"])') lines

## Filelist
\`\`\`
$flist
\`\`\`

## Hierarchy
<!-- module tree; produced by @spec-analyst in Phase 1 -->

## Signal map (assertion bind targets)
| Module | Signal | Width | Meaning |
|---|---|---|---|

## Protocol / register map
<!-- bus protocol, register offsets, control/status bit fields, FSM states -->

## Change request
<!-- summary of change_request.md; REQ-ids it adds -->

## Confirmed deviations
| REQ | Location | Spec says | RTL does | Confidence |
|---|---|---|---|---|

## PPA notes
- Baseline (from \`runs/challenge-$N-baseline-out/stages/ppa.json\`):
- Best submission:
- Applied wins:

## V ceiling
- Observed:
- Unkillable mutants:
EOF
    say "created docs/designs/challenge-$N.md"
else
    say "kept existing docs/designs/challenge-$N.md"
fi

if [ ! -e "docs/designs/challenge-$N-req-matrix.md" ]; then
    {
        echo "# challenge-$N — REQ coverage matrix"
        echo
        echo "One row per requirement. Exit gate: every REQ has >= 1 test and >= 1 assertion."
        echo "Status values: \`todo\`, \`test\`, \`assert\`, \`done\`, \`n/a\` (with reason)."
        echo
        echo "| REQ | Summary | RTL location | Test | Assertion | Status |"
        echo "|---|---|---|---|---|---|"
        if [ "$nreq" -gt 0 ]; then
            printf '%s\n' "$reqs" | while read -r r; do
                [ -n "$r" ] || continue
                echo "| $r | | | | | todo |"
            done
        else
            echo "| REQ-? | (no REQ ids extracted — check spec.md format) | | | | todo |"
        fi
        echo
        echo "## Notes"
        echo "- Extract/refresh REQ list: \`grep -ohE 'REQ-[0-9]+' $D/spec.md | sort -uV\`"
    } > "docs/designs/challenge-$N-req-matrix.md"
    say "created docs/designs/challenge-$N-req-matrix.md ($nreq REQs)"
else
    say "kept existing docs/designs/challenge-$N-req-matrix.md"
fi

if [ ! -e "docs/state/challenge-$N-progress.md" ]; then
    cat > "docs/state/challenge-$N-progress.md" <<EOF
# challenge-$N — progress (started $(date -u +%Y-%m-%dT%H:%MZ))

## challenge-$N — Phase 1 (analyze) — in_progress

### Resume recipe
1. Read \`Next action for operator\` (bottom) and the score history.
2. \`md5sum runs/sub-best.tar.gz\` and compare with **Best submission**.
3. Read \`docs/designs/challenge-$N.md\` and \`docs/designs/challenge-$N-req-matrix.md\`.

### Phase status
| Phase | Status |
|-------|--------|
| 1 ANALYZE | in_progress |
| 2 FIX | pending |
| 3 VERIFY | pending |
| 4 ITERATE | pending |

### Score history
| Sub | Time | C | V | P | Notes |
|-----|------|---|---|---|-------|
| — | — | — | — | — | baseline not measured yet |

### Deviations found
| REQ | Location | Spec says | RTL does | Pattern | Confidence |
|-----|----------|-----------|----------|---------|------------|

### CR implementation notes
-

### V ceiling
- Observed:
- Equivalent (unkillable) mutants:

### PPA
- Baseline: (fill from \`runs/challenge-$N-baseline-out/stages/ppa.json\`)
- Applied optimizations:
  | Sub | Change | C | V | P | delta-P |
  |-----|--------|---|---|---|---------|

### Activity log (newest first)
#### $(date -u +%Y-%m-%dT%H:%MZ) / bootstrap
- Did: \`scripts/design-init.sh\` created workspace, baseline mirror, docs.
- Found: top=\`$top\`, period=${period} ns, $nreq REQ ids.
- Next: public test smoke run, then baseline PPA cache, then Phase 1.

### Operator feedback log
-

### Next action for operator
- Run the public testbench smoke test (command in docs/state/prep-plan.md §7).
- Then run the baseline PPA cache command (docs/design-init.md step 4).
- Then tell the agent to start Phase 1a (spec-analyst).

### Best submission
- (none yet)
EOF
    say "created docs/state/challenge-$N-progress.md"
else
    say "kept existing docs/state/challenge-$N-progress.md"
fi

# ---------------------------------------------------------------- 5. summary
pubmods=$(ls "$D"/tb/public/test_*.py 2>/dev/null | xargs -r -n1 basename | sed 's/^/  /' || true)
say ""
say "design facts"
say "  top        : $top"
say "  clock      : $(printf '%s' "$meta" | python3 -c 'import json,sys;print(json.load(sys.stdin)["clock"])') @ ${period} ns"
say "  reset      : $(printf '%s' "$meta" | python3 -c 'import json,sys;print(json.load(sys.stdin)["reset"])')"
say "  filelist   : $(printf '%s' "$meta" | python3 -c 'import json,sys;print(len(json.load(sys.stdin)["filelist"]))') files"
say "  REQ ids    : $nreq"
say "  public tb  :"
[ -n "$pubmods" ] && printf '%s\n' "$pubmods" || say "    (none found)"

say ""
say "next commands (sudo -n docker; 12 cores)"
say "  1) public tests"
say "     sudo -n docker run --rm --cpus 12 -u \$(id -u):\$(id -g) -e HOME=/tmp \\"
say "       -v \$PWD/$D:/design --entrypoint bash proving-ground/flow:public \\"
say "       -c 'cd /design/tb/public && make'"
say "  2) cache baseline PPA (slow, once)"
say "     tar czf runs/$D-baseline.tar.gz -C $D/baseline rtl design.yaml tb flow"
say "     sudo -n docker run --rm --cpus 12 -u \$(id -u):\$(id -g) -e HOME=/tmp \\"
say "       -v \$PWD/$D/baseline:/design:ro -v \$PWD/runs:/runs proving-ground/flow:public \\"
say "       run --design /design --submission /runs/$D-baseline.tar.gz \\"
say "           --out /runs/$D-baseline-out --jobs 12 --cache /runs/$D-cache"
say "  3) fast V/evidence iteration after RTL changes"
say "     sudo -n docker run --rm --cpus 12 -u \$(id -u):\$(id -g) -e HOME=/tmp \\"
say "       -v \$PWD/$D:/design:ro -v \$PWD/runs:/runs proving-ground/flow:public \\"
say "       run --design /design --submission /runs/sub.tar.gz \\"
say "           --out /runs/out --jobs 12 --cache /runs/$D-cache --skip ppa --keep-work"
say "     scripts/mutant-report.py runs/out"
