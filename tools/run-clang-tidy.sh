#!/usr/bin/env bash
# tools/run-clang-tidy.sh
# Usage: tools/run-clang-tidy.sh [threads]
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${ROOT_DIR}/build"
THREADS="${1:-4}"

if [[ ! -f "${BUILD_DIR}/compile_commands.json" ]]; then
  echo "compile_commands.json not found in ${BUILD_DIR}."
  echo "Run: cmake -S . -B ${BUILD_DIR} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
  exit 1
fi

# prefer the run-clang-tidy.py helper if available
if command -v run-clang-tidy.py >/dev/null 2>&1; then
  echo "Using run-clang-tidy.py (parallel)"
  run-clang-tidy.py -p "${BUILD_DIR}" -j "${THREADS}" --exclude "external/*"
else
  echo "run-clang-tidy.py not found — falling back to parallel clang-tidy via xargs"
  # Accept common source extensions; headers will be examined when referenced from TU
  find "${ROOT_DIR}/src" -name '*.cpp' -o -name '*.cxx' -o -name '*.cc' | \
    xargs -n1 -P"${THREADS}" -I{} clang-tidy {} -p "${BUILD_DIR}" --extra-arg=-std=c++23 --header-filter='^(src/)' || true
fi
