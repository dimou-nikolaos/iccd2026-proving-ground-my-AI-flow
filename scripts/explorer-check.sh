#!/usr/bin/env bash
# GREEN-queue gate for Explorer variants: prove a variant is equivalent to the
# banked core and has a DISTINCT post-synthesis netlist fingerprint.
#
#   ./explorer-check.sh <core-dir> <variant-dir> [submitted-hashes-file]
#
# <core-dir> / <variant-dir>: directories containing rtl/filelist.f (or pass
# the filelist via CORE_FL / VAR_FL). Runs inside proving-ground/flow:public.
#
# Exit 0  = EQUIVALENT + distinct  (safe to submit for Explorer)
# Exit 1  = not equivalent, build failure, or duplicate fingerprint
#
# The equivalence check uses the same yosys commands the flow's mutator uses
# (equiv_make / equiv_simple -seq 5 / equiv_induct -seq 5), so "proven" here
# means the same notion of equivalence the flow applies to mutants. It is a
# bounded/inductive proof on the RTL: treat inconclusive results as NOT proven
# (we never submit unproven GREEN candidates).
set -euo pipefail

core=$(cd "${1:?core dir}" && pwd); var=$(cd "${2:?variant dir}" && pwd)
hashes=${3:-runs/explorer-hashes.txt}
here=$(cd "$(dirname "$0")" && pwd); repo=$(cd "$here/.." && pwd)
mkdir -p "$(dirname "$hashes")"

sudo -n docker run --rm -u "$(id -u):$(id -g)" -e HOME=/tmp \
    -v "$core":/core:ro -v "$var":/var:ro -v "$repo":/repo \
    --entrypoint bash proving-ground/flow:public -c '
set -e
top=$(grep -m1 "^top:" /core/design.yaml 2>/dev/null | awk "{print \$2}")
[ -n "$top" ] || { echo "ERROR: /core/design.yaml missing or has no 'top:' line — copy the design.yaml in (fallback from filelist head is unsafe: pkgs come first)"; exit 1; }
corefl=$(grep -v "^[#[:space:]]" /core/rtl/filelist.f | grep -v "^[[:space:]]*$" | sed "s/\r//g; s/[[:space:]]*$//g; s|^/||; s|^|/core/|" | xargs)
varfl=$(grep -v "^[#[:space:]]" /var/rtl/filelist.f | grep -v "^[[:space:]]*$" | sed "s/\r//g; s/[[:space:]]*$//g; s|^/||; s|^|/var/|" | xargs)
prep_cmd="hierarchy -top $top; proc; flatten; memory -nomap; memory_map; opt_clean; async2sync; dffunmap"
yosys -Q -l /tmp/e.log <<EOF
read_slang --top $top $corefl
$prep_cmd
rename $top gold
design -stash gold
read_slang --top $top $varfl
$prep_cmd
rename $top gate
design -stash gate
design -copy-from gold -as gold gold
design -copy-from gate -as gate gate
equiv_make gold gate eq
hierarchy -top eq
equiv_simple -seq 5
equiv_induct -seq 5
equiv_status -assert
EOF
echo "EQUIV: PROVEN"
yosys -Q -p "read_slang --top $top $varfl; synth -top $top; write_verilog -noattr /tmp/n.v; stat" > /tmp/stat.txt 2>&1
f=$(sha256sum /tmp/n.v | cut -d" " -f1)
echo "FINGERPRINT: $f"
grep -E "^ *[0-9]+ cells|Number of cells|wire " /tmp/stat.txt | head -3
' > /tmp/explorer-check.log 2>&1 && rc=0 || rc=1
grep -E "EQUIV|FINGERPRINT|ERROR|Warning|Status" /tmp/explorer-check.log | tail -12
[ $rc -eq 0 ] || { echo "NOT EQUIVALENT (see /tmp/explorer-check.log) — RED queue at best"; exit 1; }

fp=$(grep '^FINGERPRINT:' /tmp/explorer-check.log | awk '{print $2}')
if grep -q "$fp" "$hashes" 2>/dev/null; then
    echo "DUPLICATE NETLIST — would not count for Explorer; skip this variant"; exit 1
fi
echo "$fp  $var  $(date -Is)" >> "$hashes"
echo "GREEN: equivalent + distinct (recorded in $hashes)"
