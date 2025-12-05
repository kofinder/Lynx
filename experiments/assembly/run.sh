#!/bin/bash

asm_file=program.asm
build_dir=dump

mkdir -p $build_dir

# Output object and executable names
obj_file=$build_dir/program.o
exe_file=$build_dir/program

# --------------------------------------------------------
# 1. Assemble the .asm file into an object file
# --------------------------------------------------------
# If your .asm is NASM (Intel syntax)
nasm -f elf64 $asm_file -o $obj_file

# If your .asm is in GAS (AT&T) syntax, use:
# as --64 $asm_file -o $obj_file

echo "[OK] Assembled object file: $obj_file"

# --------------------------------------------------------
# 2. Link the object file into an executable
# --------------------------------------------------------
ld $obj_file -o $exe_file
echo "[OK] Linked executable: $exe_file"

# --------------------------------------------------------
# 3. Run the executable
# --------------------------------------------------------
echo "[Running] Executable output:"
$exe_file
exit_code=$?

echo "[Program exited with code: $exit_code]"
exit $exit_code
