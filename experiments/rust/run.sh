#!/bin/bash

# -----------------------------
# Rust build and debug script
# -----------------------------

file_name=program.rs
build_dir=dump

mkdir -p $build_dir

# Detect tools
RUSTC=$(command -v rustc)
GDB=$(command -v gdb)
VALGRIND=$(command -v valgrind)

echo "Using rustc: $RUSTC"
echo "Using gdb: $GDB"
echo "Using valgrind: $VALGRIND"

# -----------------------------
# 1. Compile Rust file with debug symbols (-g)
# -----------------------------
exe_file=$build_dir/program
$RUSTC -g $file_name -o $exe_file
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
# 3. Run memory check with Valgrind
# -----------------------------
if [ -x "$VALGRIND" ]; then
    echo "[Valgrind] Running memory check..."
    $VALGRIND --leak-check=full --show-leak-kinds=all $exe_file
else
    echo "[Valgrind] Not found. Skipping memory check."
fi

# -----------------------------
# 4. Run with GDB (interactive)
# -----------------------------
if [ -x "$GDB" ]; then
    echo "[GDB] Launching debugger..."
    $GDB $exe_file
else
    echo "[GDB] Not found. Skipping debugger."
fi

exit 0
