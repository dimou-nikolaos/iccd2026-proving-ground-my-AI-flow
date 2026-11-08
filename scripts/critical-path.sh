#!/bin/bash
# scripts/critical-path.sh — extract worst-slack timing paths from a flow output
# Requires the flow was run with --keep-work.
# Usage: critical-path.sh <out-dir-containing-work>
# Example:
#   docker run --rm ... proving-ground/flow:public run ... --keep-work --out /runs/out-foo
#   critical-path.sh runs/out-foo
set -e
OUT="$1"
[ -z "$OUT" ] && { echo "Usage: $0 <out-dir-with-work>"; exit 1; }
OUT_ABS=$(realpath "$OUT")
ODB="$OUT_ABS/work/ppa/orfs/results/sky130hd/submission/base/6_final.odb"
SDC="$OUT_ABS/work/ppa/orfs/results/sky130hd/submission/base/6_final.sdc"
if [ ! -f "$ODB" ] || [ ! -f "$SDC" ]; then
  echo "ERROR: $ODB or $SDC not found."
  echo "Did the flow run include --keep-work?"
  exit 1
fi
PARENT=$(dirname "$OUT_ABS")
docker run --rm -u $(id -u):$(id -g) -e HOME=/tmp \
  -v "$PARENT":"$PARENT" --entrypoint bash proving-ground/flow:public -c "
cat > /tmp/checks.tcl <<TCL
read_lef /OpenROAD-flow-scripts/flow/platforms/sky130hd/lef/sky130_fd_sc_hd.tlef
read_lef /OpenROAD-flow-scripts/flow/platforms/sky130hd/lef/sky130_fd_sc_hd_merged.lef
read_liberty /OpenROAD-flow-scripts/flow/platforms/sky130hd/lib/sky130_fd_sc_hd__tt_025C_1v80.lib
read_db $ODB
read_sdc $SDC
report_checks -path_delay max -format summary -group_path_count 50 -sort_by_slack
TCL
openroad -exit /tmp/checks.tcl 2>&1" \
  | grep -E "^(cfg_|rst_|clk|Startpoint|Endpoint|----|.*input.*output.*[0-9])" | head -40
