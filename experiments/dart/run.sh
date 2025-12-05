#!/bin/bash

# -----------------------------
# Dart build and run script
# -----------------------------

file_name=program.dart
build_dir=dump

mkdir -p $build_dir

# Detect Dart SDK
DART=$(command -v dart)

if [ -z "$DART" ]; then
    echo "[ERROR] Dart SDK not found! Install Dart first."
    exit 1
fi

echo "Using Dart: $DART"

# -----------------------------
# 1. Compile Dart file (optional, Dart can run scripts directly)
# -----------------------------
# Dart can compile to a snapshot or AOT binary if desired
exe_file=$build_dir/program
$DART compile exe $file_name -o $exe_file
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
echo "[INFO] For interactive debugging, you can run:"
echo "dart run $file_name"

exit 0
