#!/bin/bash

# -----------------------------
# Java build and debug script
# -----------------------------

file_name=Program.java
build_dir=dump

mkdir -p $build_dir

# Detect Java tools
JAVAC=$(command -v javac)
JAVA=$(command -v java)
JDB=$(command -v jdb)

if [ -z "$JAVAC" ] || [ -z "$JAVA" ]; then
    echo "[ERROR] Java JDK not found! Install OpenJDK first."
    exit 1
fi

echo "Using javac: $JAVAC"
echo "Using java:  $JAVA"
echo "Using jdb:   $JDB"

# -----------------------------
# 1. Compile Java file into dump folder
# -----------------------------
$JAVAC -d $build_dir $file_name
if [ $? -ne 0 ]; then
    echo "[ERROR] Compilation failed!"
    exit 1
fi
echo "[OK] Compiled Java class files to: $build_dir"

# -----------------------------
# 2. Run program normally
# -----------------------------
# Extract class name without .java extension
class_name=$(basename "$file_name" .java)

echo "[Running] Program output:"
$JAVA -cp $build_dir $class_name
exit_code=$?
echo "[Program exited with code: $exit_code]"

# -----------------------------
# 3. Optional debugging
# -----------------------------
if [ -x "$JDB" ]; then
    echo "[JDB] To debug, run:"
    echo "jdb -classpath $build_dir $class_name"
else
    echo "[INFO] jdb not found. Skipping debugger."
fi

exit 0
