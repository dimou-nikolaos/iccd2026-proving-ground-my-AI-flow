#!/usr/bin/env bash
# scripts/explorer-variants.sh — batch-generate Explorer variant candidates from the
# banked core and gate every one through scripts/explorer-check.sh.
#
#   ./explorer-variants.sh --core <core-dir> [--class C] [--n N] [--ident NAME]
#                          [--hashfile F] [--outdir DIR]
#
# Default classes cycled when --class is omitted: rename buffer keep-reg fsm-onehot reroll
# (--n = variants per class; default 1). --ident forces a specific signal/register.
#
# Outputs:
#   runs/explorer/<id>/           variant dirs (copy of core + one edit)
#   runs/explorer-green.txt       id + fingerprint of gate-PASSED variants (submit queue)
#   runs/explorer-seedreroll.txt  comment-only archives (V seed rerolls, NOT Explorer)
#   runs/explorer-hashes.txt      every accepted fingerprint (dedup ledger, gate-owned)
#
# GREEN list entries still need the usual submission pipeline (tar + full local flow
# incl. PPA) before submitting — the gate proves equivalence+distinctness, nothing else.
set -uo pipefail
here=$(cd "$(dirname "$0")" && pwd)

CORE="" CLASSES="" N=1 IDENT="" HASHES="runs/explorer-hashes.txt" OUT="runs/explorer"
while [ $# -gt 0 ]; do
  case "$1" in
    --core) CORE="$2"; shift 2 ;;
    --class) CLASSES="$CLASSES $2"; shift 2 ;;
    --n) N="$2"; shift 2 ;;
    --ident) IDENT="$2"; shift 2 ;;
    --hashfile) HASHES="$2"; shift 2 ;;
    --outdir) OUT="$2"; shift 2 ;;
    *) echo "unknown arg: $1"; exit 1 ;;
  esac
done
[ -n "$CORE" ] || { echo "need --core <dir with rtl/filelist.f and design.yaml>"; exit 1; }
[ -f "$CORE/rtl/filelist.f" ] || { echo "$CORE has no rtl/filelist.f"; exit 1; }
mkdir -p "$OUT"

[ -n "$CLASSES" ] || CLASSES="rename buffer keep-reg fsm-onehot reroll"
seq_id=0; green=0; dup=0; bad=0
printf "%-22s %-10s %s\n" ID CLASS RESULT
for cls in $CLASSES; do
  for i in $(seq 1 "$N"); do
    seq_id=$((seq_id+1))
    id="e$(printf '%02d' $seq_id)_${cls}"
    dir="$OUT/$id"
    args=(--class "$cls" --seed "$seq_id" --pick "$((i-1))")
    [ -n "$IDENT" ] && args+=(--name "$IDENT")
    desc=$(python3 "$here/explorer-variants.py" "$CORE" "$dir" "${args[@]}" 2>&1) || {
      printf "%-22s %-10s GEN-FAIL (%s)\n" "$id" "$cls" "$desc"; bad=$((bad+1)); continue; }
    if [ "$cls" = "reroll" ]; then
      tar -czf "$dir.tar.gz" -C "$dir" rtl 2>/dev/null
      printf "%-22s %-10s %s\n" "$id" "$cls" "SEEDREROLL ($desc)"
      echo "$id  $desc" >> runs/explorer-seedreroll.txt
      continue
    fi
    res=$("$here/explorer-check.sh" "$CORE" "$dir" "$HASHES" 2>&1); rc=$?
    fp=$(grep -o '[0-9a-f]\{64\}' <<<"$res" | head -1)
    case $rc in
      0) printf "%-22s %-10s GREEN  (%s | %s)\n" "$id" "$cls" "$desc" "${fp:0:12}"
         echo "$id  $fp  $dir  $desc" >> runs/explorer-green.txt; green=$((green+1)) ;;
      *) if grep -q DUPLICATE <<<"$res"; then
           printf "%-22s %-10s COLLAPSED (equiv ok, netlist identical: %s)\n" "$id" "$cls" "$desc"
           dup=$((dup+1))
         else
           printf "%-22s %-10s REJECTED (%s)\n" "$id" "$cls" "$desc"
           mv "$dir" "$dir.rejected" 2>/dev/null; bad=$((bad+1))
         fi ;;
    esac
  done
done
echo "---"
echo "GREEN=$green collapsed=$dup rejected/genfail=$bad  (green queue: runs/explorer-green.txt)"
echo "next: tar GREEN dirs + run full local flow (pg-flow-commands skill) before any submission"
