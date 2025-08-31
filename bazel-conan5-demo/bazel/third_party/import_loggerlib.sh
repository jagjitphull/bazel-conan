#!/usr/bin/env bash
set -euo pipefail

# Usage:
#   ./import_loggerlib.sh static   # default
#   ./import_loggerlib.sh shared
FLAVOR="${1:-static}"   # static | shared
PKG_REF="loggerlib/1.0.0"
DEST_DIR="$(cd "$(dirname "$0")" && pwd)/loggerlib"

TMP_JSON="$(mktemp)"; trap 'rm -f "$TMP_JSON"' EXIT

# 1) Query cache JSON
conan list "${PKG_REF}#*:*#*" --format=json >"$TMP_JSON"

# 2) Pick a package id (Release, shared True/False)
PYTHON_FILTER=$(cat <<'PY'
import json, os, sys
data=json.load(open(sys.argv[1]))
want_shared = (os.environ.get("WANT_SHARED","0") == "1")
recipes = data.get("Local Cache",{}).get("loggerlib/1.0.0",{}).get("revisions",{})
pkgid=""
rrev=""
for rrev_k, revdata in recipes.items():
    for pid, pinfo in revdata.get("packages",{}).items():
        info = pinfo.get("info",{})
        stg  = info.get("settings",{})
        opts = info.get("options",{})
        if stg.get("build_type") != "Release":
            continue
        if (opts.get("shared") == "True") == want_shared:
            pkgid = pid
            rrev  = rrev_k
            break
    if pkgid:
        break
print(pkgid + "|" + rrev if pkgid else "")
PY
)

export WANT_SHARED; if [ "${FLAVOR}" = "shared" ]; then WANT_SHARED=1; else WANT_SHARED=0; fi
SEL="$(python3 -c "$PYTHON_FILTER" "$TMP_JSON")"
if [ -z "$SEL" ]; then
  echo "ERROR: No package id found for ${PKG_REF} (Release, shared=${FLAVOR})."
  echo "Build it: conan create libs/loggerlib -s build_type=Release -o loggerlib/*:shared=$([ "$FLAVOR" = shared ] && echo True || echo False) --build=missing"
  exit 1
fi
PKG_ID="${SEL%%|*}"
RREV="${SEL##*|}"

# 3) Build a *valid* reference for cache path:
#    name/version#rrev:pkgid (prev optional)
FULL_REF="${PKG_REF}#${RREV}:${PKG_ID}"

# 4) Resolve paths
SRC_DIR="$(conan cache path "${PKG_REF}#${RREV}" --folder=source)"
BLD_DIR="$(conan cache path "${FULL_REF}"          --folder=build)"

if [ ! -d "$SRC_DIR" ] || [ ! -d "$BLD_DIR" ]; then
  echo "ERROR: Could not resolve source/build folders."
  echo " SRC_DIR: $SRC_DIR"
  echo " BLD_DIR: $BLD_DIR"
  exit 1
fi

# 5) Find built libs under the package build dir
mapfile -t LIBS < <(find "$BLD_DIR" -type f \( -name 'libloggerlib.a' -o -name 'libloggerlib.so' -o -name 'libloggerlib.dylib' -o -name 'loggerlib.lib' -o -name 'loggerlib.dll' \) 2>/dev/null | sort)
if [ ${#LIBS[@]} -eq 0 ]; then
  echo "ERROR: No loggerlib libraries found under: $BLD_DIR"
  exit 1
fi

# Prefer the requested flavor
select_lib=""
for f in "${LIBS[@]}"; do
  base="$(basename "$f")"
  case "$FLAVOR:$base" in
    static:libloggerlib.a|static:loggerlib.lib) select_lib="$f"; break;;
    shared:libloggerlib.so|shared:libloggerlib.dylib|shared:loggerlib.dll) select_lib="$f"; break;;
  esac
done
[ -z "$select_lib" ] && select_lib="${LIBS[0]}"

# 6) Stage into Bazel-friendly tree
rm -rf "${DEST_DIR}"
mkdir -p "${DEST_DIR}/include" "${DEST_DIR}/lib" "${DEST_DIR}/bin"

if [ -d "${SRC_DIR}/include" ]; then
  cp -a "${SRC_DIR}/include/." "${DEST_DIR}/include/"
fi

case "$(basename "$select_lib")" in
  *.a|*.so|*.dylib|*.lib) cp -a "$select_lib" "${DEST_DIR}/lib/";;
  *.dll)                   cp -a "$select_lib" "${DEST_DIR}/bin/";;
esac

echo "Imported ${FLAVOR} package:"
echo "  Ref:       ${FULL_REF}"
echo "  Build lib: ${select_lib}"
echo "  Headers:   ${SRC_DIR}/include -> ${DEST_DIR}/include"
ls -l "${DEST_DIR}/lib" || true
ls -l "${DEST_DIR}/bin" || true






