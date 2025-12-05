#!/bin/bash

file_name=program.go
build_dir=dump

mkdir -p $build_dir

# Detect tools
GO=$(command -v go)
DLV=$(command -v dlv)

if [ -z "$GO" ]; then
    echo "[ERROR] Go compiler not found in PATH!"
    exit 1
fi

echo "Using Go: $GO"
echo "Using Delve: $DLV"

# Compile Go file
exe_file=$build_dir/program
$GO build -o $exe_file $file_name
if [ $? -ne 0 ]; then
    echo "[ERROR] Compilation failed!"
    exit 1
fi
echo "[OK] Built executable: $exe_file"

# Run program normally
echo "[Running] Program output:"
$exe_file
exit_code=$?
echo "[Program exited with code: $exit_code]"

# Debug with Delve if available
if [ -x "$DLV" ]; then
    echo "[Delve] Launching debugger..."
    $DLV exec $exe_file
else
    echo "[Delve] Not found. Skipping debugger."
fi

exit 0
