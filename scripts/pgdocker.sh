#!/usr/bin/env bash
# Wrapper: host docker.sock is group `docker` but agent sessions don't carry the group.
# `sg docker -c` runs docker with the right group without sudo (sudo is password-gated).
# Usage: scripts/pgdocker.sh <docker args...>   (equivalent to `docker <args>`)
set -euo pipefail
args=("$@")
# rebuild a single-quoted command string for sg
cmd="docker"
for a in "${args[@]}"; do
  cmd+=" $(printf '%q' "$a")"
done
exec sg docker -c "$cmd"
