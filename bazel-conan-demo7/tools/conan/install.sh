#!/usr/bin/env bash
set -euo pipefail

# --- robust path resolution even if invoked by /bin/sh
SELF="${BASH_SOURCE[0]:-${0}}"
ROOT="$(cd "$(dirname "${SELF}")/../.." && pwd)"
OUT="${ROOT}/third_party/conan"
RAW="${OUT}/_raw"

echo ">>> install.sh: ROOT=${ROOT}"
echo ">>> install.sh: OUT=${OUT}"
echo ">>> install.sh: RAW=${RAW}"

# Ensure Conan is on PATH (Dockerfile sets /opt/py/bin)
if ! command -v conan >/dev/null 2>&1; then
  echo "ERROR: 'conan' not found on PATH. Current PATH: ${PATH}"
  echo "HINT: In Dockerfile, ensure: ENV PATH=\"/opt/py/bin:\${PATH}\" BEFORE calling this script."
  exit 10
fi
conan --version || true

# Prepare folders
rm -rf "${OUT}"
mkdir -p "${RAW}" "${OUT}/include" "${OUT}/lib"

echo ">>> Detecting Conan profile"
conan profile detect --force

echo ">>> Running Conan install (full_deploy)"
# If you want shared libs explicitly, keep the -o line; otherwise remove it.
conan install "${ROOT}/tools/conan" \
  -s build_type=Release \
  -o zlib*:shared=False \
  --build=missing \
  --deployer=full_deploy \
  --deployer-folder="${RAW}" \
  --output-folder="${RAW}"

echo ">>> Post-deploy: showing first 40 entries from RAW:"
find "${RAW}" -maxdepth 5 -type f -o -type l | head -n 40 || true

# ---- Flatten: headers ----
ZLIB_H="$(find "${RAW}" -type f -name zlib.h | head -n1 || true)"
if [[ -n "${ZLIB_H}" ]]; then
  HDR_DIR="$(dirname "${ZLIB_H}")"
  echo ">>> Found zlib.h in: ${HDR_DIR}"
  cp -a "${HDR_DIR}/." "${OUT}/include/"
else
  echo "WARNING: zlib.h not found under ${RAW}"
fi

# ---- Flatten: libraries (files AND symlinks, any lib dir name) ----
echo ">>> Scanning for libraries in ${RAW} ..."
find "${RAW}" \( -type f -o -type l \) \
  \( -name '*.so' -o -name '*.so.*' -o -name '*.a' -o -name '*.dylib' -o -name '*.lib' \) \
  -print0 | xargs -0 -I{} bash -c 'echo ">>> Copy lib: {}"; cp -P "{}" "'"${OUT}"'/lib/"'

echo ">>> After flatten:"
find "${OUT}" -maxdepth 2 -type d -print
ls -al "${OUT}/include" || true
ls -al "${OUT}/lib" || true

# --- Always generate a Bazel package, even if include/lib empty (so entrypoint can re-run) ---
python3 "${ROOT}/tools/conan/gen_build_wrapper.py" "${OUT}"

# --- Soft sanity: at least one lib present; if not, show context then exit ---
if ! find "${OUT}/lib" -maxdepth 1 \( -type f -o -type l \) | grep -q . ; then
  echo "ERROR: No libraries were copied into ${OUT}/lib."
  echo "DEBUG: Show RAW tree (depth 5):"
  find "${RAW}" -maxdepth 5 -type d -print
  echo "DEBUG: Show RAW files (first 80):"
  find "${RAW}" -maxdepth 5 -type f -o -type l | head -n 80
  exit 3
fi

echo ">>> Conan deploy complete at ${OUT}"



