#!/usr/bin/env bash
set -euo pipefail

# Ensure Conan 2.x is installed:  pip install -U conan
conan --version

# Detect a default profile if not present
conan profile detect --force || true

# Clean previous vendor drop
rm -rf third_party/eigen_vendor third_party/eigen_deploy
mkdir -p third_party/eigen_vendor/include
mkdir -p third_party/eigen_deploy

# Use a DEPLOYER (Conan 2) to copy package files to a local folder
# This avoids having to know the Conan cache internals.
conan install . \
  -s build_type=Release \
  --deployer=full_deploy \
  --deployer-folder=third_party/eigen_deploy

# Find the include folder that contains Eigen/...
INC_DIR="$(find third_party/eigen_deploy -type d -name Eigen -print -quit || true)"
if [[ -z "${INC_DIR}" ]]; then
  echo "ERROR: Could not locate Eigen headers after deploy."
  exit 1
fi
# The parent of '.../Eigen' is the include directory
INC_DIR="$(dirname "$INC_DIR")"

# Copy headers into a stable path Bazel can reference
rsync -a --delete "$INC_DIR"/ third_party/eigen_vendor/include/

echo "Eigen headers installed at third_party/eigen_vendor/include"
echo "Now run:  bazel build //:adas_demo"
