#!/bin/bash

file_name=program.cpp
build_dir=dump

mkdir -p $build_dir

# Detect LLVM tools
CLANG=$(command -v clang++)
LLI=$(command -v lli)
GDB=$(command -v gdb)
VALGRIND=$(command -v valgrind)

echo "Using clang++: $CLANG"
echo "Using lli:    $LLI"
echo "Using gdb:    $GDB"
echo "Using valgrind: $VALGRIND"

# --------------------------------------------------------
# 1. Generate LLVM IR (.ll) with debug info
# --------------------------------------------------------
$CLANG -std=c++23 -g -S -emit-llvm -pthread $file_name -o $build_dir/program.ll
echo "[OK] Generated IR: $build_dir/program.ll"

# --------------------------------------------------------
# 2. Produce binary executable with debug info
# --------------------------------------------------------
$CLANG -std=c++23 -g -pthread $build_dir/program.ll -o $build_dir/program
echo "[OK] Built executable: $build_dir/program"

# --------------------------------------------------------
# 3. Generate assembly (.s) with debug info
# --------------------------------------------------------
$CLANG -std=c++23 -g -S -pthread $build_dir/program.ll -o $build_dir/program.s
echo "[OK] Generated assembly: $build_dir/program.s"

# --------------------------------------------------------
# 4. Run program normally
# --------------------------------------------------------
echo "[Running] Executing program normally..."
$build_dir/program
exit_code=$?
echo "[Program exited with code: $exit_code]"

# # --------------------------------------------------------
# # 5. Run with Valgrind (memory check)
# # --------------------------------------------------------
# if [ -x "$VALGRIND" ]; then
#     echo "[Valgrind] Running memory check..."
#     $VALGRIND --leak-check=full --show-leak-kinds=all $build_dir/program
# else
#     echo "[Valgrind] Not found. Skipping memory check."
# fi

# # --------------------------------------------------------
# # 6. Run with GDB (interactive debugging)
# # --------------------------------------------------------
# if [ -x "$GDB" ]; then
#     echo "[GDB] Launching debugger..."
#     $GDB $build_dir/program
# else
#     echo "[GDB] Not found. Skipping debugger."
# fi

exit 0
