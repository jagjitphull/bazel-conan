#!/usr/bin/env bash
set -euo pipefail

# If you set workspace(name="...") in WORKSPACE (legacy), put it here.
# With Bzlmod, main repo runfiles are commonly under "__main__".
WORKSPACE_CANDIDATES=()
if [[ -n "${WORKSPACE_NAME:-}" ]]; then
  WORKSPACE_CANDIDATES+=("${WORKSPACE_NAME}")
fi
# Fallback to the module name you used in MODULE.bazel (harmless to try)
WORKSPACE_CANDIDATES+=("bazel_rules_tutorial")
# Always try __main__ last (Bzlmod default for main repo)
WORKSPACE_CANDIDATES+=("__main__")

resolve_rf() {
  # $1: repo-relative path, e.g. "examples/joined.txt"
  local rel="$1"

  # Try each candidate workspace root
  for ws in "${WORKSPACE_CANDIDATES[@]}"; do
    # 1) RUNFILES_DIR
    if [[ -n "${RUNFILES_DIR:-}" && -f "${RUNFILES_DIR}/${ws}/${rel}" ]]; then
      printf '%s\n' "${RUNFILES_DIR}/${ws}/${rel}"
      return 0
    fi
    # 2) RUNFILES
    if [[ -n "${RUNFILES:-}" && -f "${RUNFILES}/${ws}/${rel}" ]]; then
      printf '%s\n' "${RUNFILES}/${ws}/${rel}"
      return 0
    fi
    # 3) $0.runfiles tree
    if [[ -e "$0.runfiles" && -f "$0.runfiles/${ws}/${rel}" ]]; then
      printf '%s\n' "$0.runfiles/${ws}/${rel}"
      return 0
    fi
    # 4) Manifest (Windows-style)
    if [[ -n "${RUNFILES_MANIFEST_FILE:-}" && -f "${RUNFILES_MANIFEST_FILE}" ]]; then
      local mapped
      mapped="$(grep -m1 "^${ws}/${rel} " "${RUNFILES_MANIFEST_FILE}" | cut -d' ' -f2- || true)"
      if [[ -n "${mapped}" && -f "${mapped}" ]]; then
        printf '%s\n' "${mapped}"
        return 0
      fi
    fi
  done

  # Not found anywhere
  printf '%s\n' ""
  return 1
}

show_rf() {
  local rel="$1"
  local p
  p="$(resolve_rf "${rel}")"
  if [[ -n "$p" ]]; then
    cat "$p"
  else
    echo "(missing runfile: ${rel})"
  fi
}

if [[ "${1:-}" == "show" ]]; then
  echo "== hello_out =="
  show_rf "examples/hello_out.txt"
  echo

  echo "== joined =="
  show_rf "examples/joined.txt"
  echo

  echo "== upper_a =="
  show_rf "examples/upper_a.txt"
  exit 0
fi

# Test-helper behavior (no args): assert 'alpha' exists in joined.txt
JOINED="$(resolve_rf "examples/joined.txt")"
if [[ -z "$JOINED" || ! -f "$JOINED" || ! grep -qx "alpha" "$JOINED"; then
  echo "joined.txt does not contain 'alpha'"
  exit 1
fi
echo "OK"

