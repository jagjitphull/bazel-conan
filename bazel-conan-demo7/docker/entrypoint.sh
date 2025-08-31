#!/usr/bin/env bash
set -euo pipefail
cd /work

# Bootstrap Conan wrapper if missing
if [[ ! -f third_party/conan/BUILD.bazel ]]; then
  echo ">>> third_party/conan/BUILD.bazel missing — bootstrapping Conan..."
  chmod +x tools/conan/install.sh tools/conan/gen_build_wrapper_multi.py tools/conan/gen_build_wrapper.py || true
  sed -i 's/\r$//' tools/conan/install.sh tools/conan/gen_build_wrapper_multi.py tools/conan/gen_build_wrapper.py || true
  bash tools/conan/install.sh
fi

# Build your target(s)
bazel build //apps/hello:hello
echo "Built: bazel-bin/apps/hello/hello"

# If the user passed a command, run it; otherwise drop to a shell
if [ "$#" -gt 0 ]; then
  exec "$@"
else
  exec /bin/bash
fi
