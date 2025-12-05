#!/bin/bash

file_name=program.cpp
build_dir=dump

mkdir -p $build_dir

# Detect tools
GCC=$(command -v g++)
GDB=$(command -v gdb)
VALGRIND=$(command -v valgrind)

echo "Using g++: $GCC"
echo "Using gdb: $GDB"
echo "Using valgrind: $VALGRIND"

# --------------------------------------------------------
# 1. Compile with debug info (-g) and pthread support
# --------------------------------------------------------
$GCC -std=c++23 -g -O0 -pthread $file_name -o $build_dir/program
echo "[OK] Built executable with debug symbols: $build_dir/program"

# --------------------------------------------------------
# 2. Generate assembly (optional)
# --------------------------------------------------------
$GCC -std=c++23 -S -g -O0 -pthread $file_name -o $build_dir/program.s
echo "[OK] Generated assembly: $build_dir/program.s"

# --------------------------------------------------------
# 3. Run with Valgrind (memory check)
# --------------------------------------------------------
if [ -x "$VALGRIND" ]; then
    echo "[Valgrind] Running memory check..."
    $VALGRIND --leak-check=full --show-leak-kinds=all $build_dir/program
else
    echo "[Valgrind] Not found. Skipping memory check."
fi

# --------------------------------------------------------
# 4. Run with GDB (interactive debugging)
# --------------------------------------------------------
if [ -x "$GDB" ]; then
    echo "[GDB] Launching debugger..."
    $GDB $build_dir/program
else
    echo "[GDB] Not found. Skipping debugger."
fi

exit 0
