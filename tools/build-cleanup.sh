#!/bin/sh

echo "Cleaning build directory, excluding external libraries..."
if [ -d build ]; then
  find build -mindepth 1 \
    -not -path "build/external" \
    -not -path "build/external/*" \
    -exec rm -rf {} +
else
  echo "No build directory found."
fi
