#!/bin/bash

# -----------------------------
# Elixir run script
# -----------------------------

file_name=program.ex
build_dir=dump

mkdir -p $build_dir

# Detect Elixir
ELIXIR=$(command -v elixir)
MIX=$(command -v mix)

if [ -z "$ELIXIR" ]; then
    echo "[ERROR] Elixir not found! Install Elixir first."
    exit 1
fi

echo "Using Elixir: $ELIXIR"

# -----------------------------
# 1. Run the Elixir program
# -----------------------------
echo "[Running] Program output:"
$ELIXIR $file_name
exit_code=$?
echo "[Program exited with code: $exit_code]"

# -----------------------------
# 2. Optional: Compile to bytecode (.beam)
# -----------------------------
# Elixir compiles to .beam files, usually managed with Mix projects.
echo "[INFO] To compile to .beam files or create a Mix project, see:"
echo "https://elixir-lang.org/getting-started/mix-otp/introduction-to-mix.html"

exit 0
