#!/usr/bin/env python3
import os
import sys
from clang.cindex import Index, CursorKind, TranslationUnit

# Path to your project src folder
SRC_DIR = "src"

# Extensions to process
EXTENSIONS = (".cpp", ".cc", ".cxx", ".hpp", ".h")

# Initialize Clang Index
index = Index.create()

def mark_unused_params(file_path):
    # Parse the file
    tu = index.parse(file_path, args=['-std=c++23'])

    # Collect edits: list of (line, col, old_text, new_text)
    edits = []

    # Visit all functions
    for f in tu.cursor.get_children():
        if f.kind in (CursorKind.FUNCTION_DECL, CursorKind.CXX_METHOD):
            for p in f.get_arguments():
                # Check if parameter is unused (simple heuristic: not referenced)
                if not list(p.get_references()):
                    # Replace param with /*param*/
                    edits.append((p.extent.start.line, p.extent.start.column, p.spelling, f'/*{p.spelling}*/'))

    if not edits:
        return

    # Apply edits (line by line)
    with open(file_path, "r", encoding="utf-8") as f:
        lines = f.readlines()

    for line, col, old, new in reversed(edits):
        l = lines[line - 1]
        # Replace only the first occurrence of the parameter name on that line
        prefix = l[:col - 1]
        suffix = l[col - 1:]
        suffix = suffix.replace(old, new, 1)
        lines[line - 1] = prefix + suffix

    # Write back
    with open(file_path, "w", encoding="utf-8") as f:
        f.writelines(lines)

    print(f"Fixed unused params in: {file_path}")

def main():
    for root, _, files in os.walk(SRC_DIR):
        for file in files:
            if file.endswith(EXTENSIONS):
                path = os.path.join(root, file)
                try:
                    mark_unused_params(path)
                except Exception as e:
                    print(f"Failed to process {path}: {e}", file=sys.stderr)

if __name__ == "__main__":
    main()
