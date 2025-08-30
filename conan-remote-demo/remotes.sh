#!/usr/bin/env bash
set -euo pipefail

# Change these to your URLs
COMPANY_REMOTE_NAME="company_remote"
COMPANY_REMOTE_URL="https://artifactory.example.com/artifactory/api/conan/conan-local"

# Ensure conancenter exists (ConanCenter)
if ! conan remote list | grep -q '^conancenter'; then
  conan remote add conancenter https://center.conan.io
fi

# Add or update your private remote
if conan remote list | grep -q "^${COMPANY_REMOTE_NAME}"; then
  conan remote set-url "${COMPANY_REMOTE_NAME}" "${COMPANY_REMOTE_URL}"
else
  conan remote add "${COMPANY_REMOTE_NAME}" "${COMPANY_REMOTE_URL}"
fi

echo "Remotes:"
conan remote list
