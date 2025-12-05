#!/bin/bash

# -----------------------------
# Kotlin build and debug script (Snap Kotlin compatible)
# -----------------------------

file_name=program.kt
build_dir=dump

mkdir -p $build_dir

# Detect Kotlin tools
KOTLINC=$(command -v kotlinc)
KOTLIN=$(command -v kotlin)

if [ -z "$KOTLINC" ] || [ -z "$KOTLIN" ]; then
    echo "[ERROR] Kotlin compiler/runtime not found! Install Kotlin first."
    exit 1
fi

echo "Using kotlinc: $KOTLINC"
echo "Using kotlin: $KOTLIN"

# -----------------------------
# 1. Compile Kotlin file into dump folder
# -----------------------------
$KOTLINC $file_name -d $build_dir
if [ $? -ne 0 ]; then
    echo "[ERROR] Compilation failed!"
    exit 1
fi
echo "[OK] Compiled classes to: $build_dir"

# -----------------------------
# 2. Run program normally
# -----------------------------
# Top-level main() generates <FileName>Kt
class_name=$(basename "$file_name" .kt)Kt

echo "[Running] Program output:"
$KOTLIN -cp $build_dir $class_name
exit_code=$?
echo "[Program exited with code: $exit_code]"

# -----------------------------
# 3. Optional debugging
# -----------------------------
echo "[INFO] For interactive debugging, you can use IntelliJ IDEA or add logging to your code."

exit 0
