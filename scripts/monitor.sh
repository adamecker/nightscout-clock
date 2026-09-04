#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
PROJECT_DIR=$(dirname -- "$SCRIPT_DIR")

# shellcheck source=get_port.sh
source "$SCRIPT_DIR/get_port.sh"
load_platformio_config
MONITOR_PORT=$(get_port)

mkdir -p "$PROJECT_DIR/log"
LOG_FILE="$PROJECT_DIR/log/monitor_$(date +'%Y%m%d_%H%M%S').log"

echo "Monitoring $MONITOR_PORT; saving output to $LOG_FILE"
if ! platformio device monitor --port "$MONITOR_PORT" --project-dir "$PROJECT_DIR" \
    --environment ulanzi_debug | tee "$LOG_FILE"; then
    echo "ERROR: Serial monitor exited with an error." >&2
    exit 1
fi
