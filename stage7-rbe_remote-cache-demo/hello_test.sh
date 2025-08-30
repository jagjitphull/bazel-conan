#!/usr/bin/env bash
set -euo pipefail

rel="$1"  # runfiles-relative path (e.g., hello or path/to/hello)
bin="${TEST_SRCDIR}/${TEST_WORKSPACE}/${rel}"

echo "Debug: TEST_SRCDIR=$TEST_SRCDIR"
echo "Debug: TEST_WORKSPACE=${TEST_WORKSPACE:-}"
echo "Debug: Expecting binary at: $bin"

if [[ ! -x "$bin" ]]; then
  echo "ERROR: Not executable or missing: $bin"
  ls -al "${TEST_SRCDIR}/${TEST_WORKSPACE}" || true
  exit 127
fi

out="$("$bin")"
[[ "$out" == "Hello, Bazel Remote Cache!" ]] || {
  echo "Unexpected output: $out"
  exit 1
}

echo "OK"
