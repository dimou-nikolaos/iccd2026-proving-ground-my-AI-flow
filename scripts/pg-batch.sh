#!/bin/bash
# scripts/pg-batch.sh — run N full-flow submissions in parallel
# Usage: pg-batch.sh [--jobs-per N] [--cpus-per N] --baseline <path> <sub1.tar.gz> [sub2.tar.gz] ...
# Defaults: cpus-per = floor(12/N), jobs-per = cpus-per (matched, validated by 1c)
# Output: "$LOGDIR"/pgbatch_<subname>.log per variant + summary table on stdout
#
# Example:
#   pg-batch.sh --baseline challenge-1/baseline runs/sub-a.tar.gz runs/sub-b.tar.gz
set -e
BASE="challenge-1/baseline"
CPUS=""
JOBS=""
SKIP=""
EXTRA=""
DOCKER=${DOCKER:-docker}
LOGDIR=${LOGDIR:-runs/logs}
mkdir -p "$LOGDIR"
SUBS=()
while [ $# -gt 0 ]; do
  case "$1" in
    --baseline) BASE="$2"; shift 2 ;;
    --cpus-per) CPUS="$2"; shift 2 ;;
    --jobs-per) JOBS="$2"; shift 2 ;;
    --skip) SKIP="$SKIP --skip $2"; shift 2 ;;
    --extra) EXTRA="$EXTRA $2"; shift 2 ;;
    *) SUBS+=("$1"); shift ;;
  esac
done
N=${#SUBS[@]}
[ "$N" = "0" ] && { echo "Usage: $0 [--baseline P] [--cpus-per N] [--jobs-per N] sub.tar.gz [...]"; exit 1; }
TOTAL_CORES=$(nproc)
[ -z "$CPUS" ] && CPUS=$((TOTAL_CORES / N))
[ -z "$JOBS" ] && JOBS=$CPUS
echo "Running $N variants × $CPUS cores (jobs $JOBS). Total $((N*CPUS)) cores (have $TOTAL_CORES)"
START=$(date +%s)
for s in "${SUBS[@]}"; do
  name=$(basename "$s" .tar.gz)
  ( $DOCKER run --rm --cpus "$CPUS" -u $(id -u):$(id -g) -e HOME=/tmp \
      -v "$PWD/$BASE":/design:ro -v "$PWD/runs:/runs" \
      proving-ground/flow:public \
      run --design /design --submission "/runs/$name.tar.gz" --out "/runs/out-$name" --jobs "$JOBS" --cache /runs/cache-$name $SKIP \
      > "$LOGDIR/pgbatch_$name.log" 2>&1 ) &
done
wait
END=$(date +%s); echo "Total wall-clock: $((END-START))s"
echo ""
echo "variant | status | V | P"
for s in "${SUBS[@]}"; do
  name=$(basename "$s" .tar.gz)
  st=$(grep -E '"status"' "$LOGDIR"/pgbatch_$name.log | head -1 | sed 's/.*: "//;s/".*//')
  vv=$(grep -E '"V":' "$LOGDIR"/pgbatch_$name.log | head -1 | sed 's/.*: //;s/,//')
  pp=$(grep -E '"P":' "$LOGDIR"/pgbatch_$name.log | head -1 | sed 's/.*: //;s/,//')
  printf "%-12s | %-9s | %-6s | %s\n" "$name" "$st" "$vv" "$pp"
done
