#!/bin/bash

# -----------------------------
# C# build and debug script (single file using Mono)
# -----------------------------

file_name=program.cs
build_dir=dump

mkdir -p $build_dir

# Detect tools
CSC=$(command -v mcs)
MONO=$(command -v mono)

if [ -z "$CSC" ]; then
    echo "[ERROR] mcs (C# compiler) not found! Install mono-complete first."
    exit 1
fi

if [ -z "$MONO" ]; then
    echo "[ERROR] mono runtime not found! Install mono-complete first."
    exit 1
fi

echo "Using mcs: $CSC"
echo "Using mono: $MONO"

# -----------------------------
# 1. Compile C# file into dump folder
# -----------------------------
exe_file=$build_dir/program.exe
$CSC -out:$exe_file $file_name
if [ $? -ne 0 ]; then
    echo "[ERROR] Compilation failed!"
    exit 1
fi
echo "[OK] Built executable: $exe_file"

# -----------------------------
# 2. Run program normally
# -----------------------------
echo "[Running] Program output:"
$MONO $exe_file
exit_code=$?
echo "[Program exited with code: $exit_code]"

# -----------------------------
# 3. Optional: Debugging
# -----------------------------
# Mono comes with `mdb` for debugging, or you can use VS Code
echo "[INFO] For debugging, you can run:"
echo "mono --debug $exe_file"

exit 0
