#!/bin/bash
# scripts/synth-filter.sh — Stage 1 wildcard filter: yosys whole-design synth on N tar.gz variants
# Usage: synth-filter.sh --top TOP <sub1.tar.gz> [sub2.tar.gz ...]
# Outputs cell count per variant.
set -e
WORK=/tmp/opencode/synth-filter
TOP=""
while [ "${1#--top}" != "$1" ]; do
  TOP="${1#--top}"
  [ -z "$TOP" ] && TOP="$2" && shift
  shift
done
[ -z "$TOP" ] && { echo "Usage: $0 --top <top-module> <sub.tar.gz> [...]"; exit 1; }
SUBS=("$@")
N=${#SUBS[@]}
[ "$N" = "0" ] && { echo "Usage: $0 --top <top-module> <sub.tar.gz> [...]"; exit 1; }
mkdir -p "$WORK"
for s in "${SUBS[@]}"; do
  name=$(basename "$s" .tar.gz)
  rm -rf "$WORK/$name"
  mkdir -p "$WORK/$name"
  tar xzf "$s" -C "$WORK/$name"
done
START=$(date +%s)
for s in "${SUBS[@]}"; do
  name=$(basename "$s" .tar.gz)
  ( FLIST=$(tr '\n' ' ' < "$WORK/$name/rtl/filelist.f")
    docker run --rm --cpus 1 -u $(id -u):$(id -g) -e HOME=/tmp \
      -v "$WORK/$name":/design:ro --entrypoint bash proving-ground/flow:public \
      -c "cd /design && yosys -p 'read_slang $FLIST; hierarchy -top $TOP; proc; opt; memory; opt; fsm; opt; techmap; opt; stat' 2>&1" \
      > "$WORK/$name.synth.log" ) &
done
wait
END=$(date +%s); echo "Wall-clock: $((END-START))s for $N variants"
echo ""
for s in "${SUBS[@]}"; do
  name=$(basename "$s" .tar.gz)
  cells=$(awk -v top="$TOP" '$0 ~ "^=== "top" ===$"{f=1;next} f && /cells/{print $1; exit}' "$WORK/$name.synth.log")
  echo "$name: $cells cells"
done
