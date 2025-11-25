#!/bin/bash

# -----------------------------
# Haskell build and debug script
# -----------------------------

file_name=program.hs
build_dir=dump

mkdir -p $build_dir

# Detect GHC (Glasgow Haskell Compiler)
GHC=$(command -v ghc)

if [ -z "$GHC" ]; then
    echo "[ERROR] GHC not found! Install Haskell Platform or GHC first."
    exit 1
fi

echo "Using GHC: $GHC"

# -----------------------------
# 1. Compile Haskell file into dump folder
# -----------------------------
exe_file=$build_dir/program
$GHC -o $exe_file $file_name
if [ $? -ne 0 ]; then
    echo "[ERROR] Compilation failed!"
    exit 1
fi
echo "[OK] Built executable: $exe_file"

# -----------------------------
# 2. Run program normally
# -----------------------------
echo "[Running] Program output:"
$exe_file
exit_code=$?
echo "[Program exited with code: $exit_code]"

# -----------------------------
# 3. Optional debugging
# -----------------------------
# You can use GHCi for interactive debugging:
echo "[INFO] For interactive debugging, run:"
echo "ghci $file_name"

exit 0
