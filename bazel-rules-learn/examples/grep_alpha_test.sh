#!/usr/bin/env bash
set -euo pipefail

JOINED_PATH=""

# Parse --joined=<path>
for arg in "$@"; do
  case "$arg" in
    --joined=*)
      JOINED_PATH="${arg#--joined=}"
      ;;
  esac
done

if [[ -z "${JOINED_PATH}" ]]; then
  echo "ERROR: --joined=<path> not provided"
  exit 2
fi

if [[ ! -f "${JOINED_PATH}" ]]; then
  echo "ERROR: joined file not found at: ${JOINED_PATH}"
  echo "Args were: $*"
  exit 2
fi

# Assert 'alpha' exists as a full line
if ! grep -qx "alpha" "${JOINED_PATH}"; then
  echo "joined.txt does not contain 'alpha' as a line"
  echo "Contents were:"
  cat "${JOINED_PATH}" || true
  exit 1
fi

echo "OK"
