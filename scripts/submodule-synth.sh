#!/bin/bash
# scripts/submodule-synth.sh — fast yosys generic synth on a single submodule
# Usage: submodule-synth.sh <design_root> <module_name> [deps_file1.sv ...]
# design_root has rtl/<file>.sv files. If deps not given, infers from filelist
# and reads only the named module + its direct instantiations.
# Outputs total cell count.
set -e
DESIGN_ROOT="$1"
MODULE="$2"
shift 2
[ -z "$DESIGN_ROOT" ] || [ -z "$MODULE" ] && { echo "Usage: $0 <design_root> <module> [dep.sv ...]"; exit 1; }
if [ $# = 0 ]; then
  # Default: read dsp_pkg + the named module + dsp_rndsat (common dep)
  DEPS="rtl/dsp_pkg.sv rtl/dsp_rndsat.sv rtl/$MODULE.sv"
else
  DEPS=""
  for d in "$@"; do DEPS="$DEPS rtl/$d"; done
fi
docker run --rm -u $(id -u):$(id -g) -e HOME=/tmp \
  -v "$DESIGN_ROOT":/design:ro --entrypoint bash proving-ground/flow:public \
  -c "cd /design && yosys -p 'read_slang $DEPS; hierarchy -top $MODULE; proc; opt; memory; opt; fsm; opt; techmap; opt; stat' 2>&1" \
  | awk -v mod="$MODULE" '/^=== /{in_mod=($0 ~ "=== " mod " ===")}; in_mod && /cells/{print $1; exit}'
