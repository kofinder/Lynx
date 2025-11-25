#!/bin/bash

file_name=myprogram.cpp
build_dir=dump

mkdir -p $build_dir

# Detect llvm tools
CLANG=$(command -v clang++)
LLI=$(command -v lli)

echo "Using clang: $CLANG"
echo "Using lli:   $LLI"

# --------------------------------------------------------
# 1. Generate LLVM IR (.ll)
# --------------------------------------------------------
$CLANG -std=c++23 -S -emit-llvm $file_name -o $build_dir/myprogram.ll
echo "[OK] Generated IR: $build_dir/myprogram.ll"

# --------------------------------------------------------
# 2. Produce binary executable
# --------------------------------------------------------
$CLANG -std=c++23 -o $build_dir/myprogram $build_dir/myprogram.ll
echo "[OK] Built executable: $build_dir/myprogram"

# --------------------------------------------------------
# 3. Generate assembly (.s)
# --------------------------------------------------------
$CLANG -std=c++23 -S $build_dir/myprogram.ll -o $build_dir/myprogram.s
echo "[OK] Generated assembly: $build_dir/myprogram.s"

# --------------------------------------------------------
# 4. Execute LLVM IR with lli
# --------------------------------------------------------
$LLI $build_dir/myprogram.ll
exit_code=$?

echo "[Program exited with code: $exit_code]"
exit $exit_code
