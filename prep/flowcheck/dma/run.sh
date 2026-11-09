#!/usr/bin/env bash
# End-to-end rehearsal of the pgflow scoring pipeline on the DMA-shaped design.
#
#   ./run.sh --skip ppa     # fast: build + public/evidence + mutation
#   ./run.sh                # full: also baseline + submission PPA
#
# Artifacts: runs/dmacheck-sub.tar.gz, runs/dmacheck-out/report.{md,json}
set -euo pipefail

here=$(cd "$(dirname "$0")" && pwd)
repo=$(cd "$here/../../.." && pwd)
mkdir -p "$repo/runs"

# sub/rtl is our OWN fixed copy (bug=0) - deliberately NOT re-copied from
# design/rtl (which carries the seeded deviation), unlike the fifo run.sh.
# The flow puts only tests/ on pythonpath, so the VIPs + env are vendored in:
cp "$here/../../bus_vip/apb4.py" "$here/../../bus_vip/axi4lite.py" \
   "$here/../../dma_vip/tests/dma_env.py" "$here/../../dma_vip/tests/test_dma.py" \
   "$here/sub/tests/"
cp "$here/../../dma_vip/tests/dma_assertions.sv" "$here/../../dma_vip/tests/dma_bind.sv" \
   "$here/sub/assertions/"
cp "$here/../../sva/apb4_protocol_sva.sv" "$here/../../sva/axi4lite_protocol_sva.sv" \
   "$here/sub/assertions/"

tar czf "$repo/runs/dmacheck-sub.tar.gz" -C "$here/sub" \
    rtl tests assertions report.md

sudo -n docker run --rm --cpus 12 -u "$(id -u):$(id -g)" -e HOME=/tmp \
    -v "$here/design":/design:ro \
    -v "$repo/runs":/runs \
    proving-ground/flow:public \
    run --design /design \
        --submission /runs/dmacheck-sub.tar.gz \
        --out /runs/dmacheck-out \
        --jobs 12 --cache /runs/dmacheck-cache \
        --keep-work "$@"
