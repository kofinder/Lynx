#!/bin/bash

# -----------------------------
# TypeScript build and run script
# -----------------------------

file_name=program.ts
build_dir=dump

mkdir -p $build_dir

# Detect tools
TSC=$(command -v tsc)
NODE=$(command -v node)

if [ -z "$TSC" ]; then
    echo "[ERROR] TypeScript compiler (tsc) not found! Install TypeScript first."
    exit 1
fi

if [ -z "$NODE" ]; then
    echo "[ERROR] Node.js not found! Install Node.js first."
    exit 1
fi

echo "Using tsc:  $TSC"
echo "Using node: $NODE"

# -----------------------------
# 1. Compile TypeScript to JavaScript
# -----------------------------
js_file=$build_dir/$(basename "$file_name" .ts).js
$TSC $file_name --outDir $build_dir
if [ $? -ne 0 ]; then
    echo "[ERROR] Compilation failed!"
    exit 1
fi
echo "[OK] Compiled to JavaScript: $js_file"

# -----------------------------
# 2. Run JavaScript with Node.js
# -----------------------------
echo "[Running] Program output:"
$NODE $js_file
exit_code=$?
echo "[Program exited with code: $exit_code]"

# -----------------------------
# 3. Optional debugging
# -----------------------------
echo "[INFO] For debugging, you can run:"
echo "node --inspect-brk $js_file"

exit 0
