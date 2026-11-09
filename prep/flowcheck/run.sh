#!/usr/bin/env bash
# End-to-end dry run of the pgflow scoring pipeline on a synthetic design.
#
#   ./run.sh --skip ppa     # fast: build + public/evidence + mutation (~1 min)
#   ./run.sh                # full: also baseline + submission PPA (~10 min)
#
# Artifacts: runs/flowcheck-sub.tar.gz, runs/flowcheck-out/report.{md,json}
set -euo pipefail

here=$(cd "$(dirname "$0")" && pwd)
repo=$(cd "$here/../.." && pwd)
mkdir -p "$repo/runs"

rm -rf "$here/sub/rtl"
cp -r "$here/design/rtl" "$here/sub/rtl"

tar czf "$repo/runs/flowcheck-sub.tar.gz" -C "$here/sub" \
    rtl tests assertions report.md

sudo -n docker run --rm --cpus 12 -u "$(id -u):$(id -g)" -e HOME=/tmp \
    -v "$here/design":/design:ro \
    -v "$repo/runs":/runs \
    proving-ground/flow:public \
    run --design /design \
        --submission /runs/flowcheck-sub.tar.gz \
        --out /runs/flowcheck-out \
        --jobs 12 --cache /runs/flowcheck-cache \
        --keep-work "$@"
