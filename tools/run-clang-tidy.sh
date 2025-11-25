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

# Only scan your main modules
HEADER_FILTER='^src/(types|logger|exceptions|analyzer)/'

# Use xargs for per-file parallel execution to ensure .clang-tidy is respected
if command -v clang-tidy >/dev/null 2>&1; then
  echo "Running clang-tidy via xargs with live logs and .clang-tidy support"
  find "${ROOT_DIR}/src/types" "${ROOT_DIR}/src/analyzer" "${ROOT_DIR}/src/logger" "${ROOT_DIR}/src/exceptions" \
    -type f \( -name '*.cpp' -o -name '*.cxx' -o -name '*.cc' -o -name '*.hpp' \) | \
    xargs -n1 -P"${THREADS}" -I{} sh -c 'echo "Checking {}"; clang-tidy "{}" \
      -p "'"${BUILD_DIR}"'" \
      --header-filter="'"${HEADER_FILTER}"'" \
      -system-headers \
      -extra-arg-before=-fno-delayed-template-parsing || true'
else
  echo "clang-tidy not found — cannot run tidy analysis"
  exit 1
fi
