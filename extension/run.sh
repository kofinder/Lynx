#!/bin/bash
# run.sh - Build/Package/Reinstall Lynx VS Code extension
set -e

# ==========================
# 1. Clean previous builds
# ==========================
echo "Cleaning previous builds..."
rm -f *.vsix

# ==========================
# 2. Build Lanuges Json via Makefile
# ==========================
echo "Building TextMeta Json..."
# python3 grammar.py

# ==========================
# 3. Package VS Code extension
# ==========================
echo "Packaging VS Code extension..."
if [ ! -d build ]; then
    mkdir build
fi
vsce package --out build/lynx-1.0.0.vsix

# ==========================
# 4. Reinstall VS Code extension
# ==========================
echo "Reinstalling VS Code extension..."
code --uninstall-extension lynx || true
code --install-extension build/lynx-1.0.0.vsix

# ==========================
# 5. Run LSP server manually (for testing)
# ==========================
LSP_BINARY="build/LynxLSPServer"
echo "Starting Lynx LSP Server at $LSP_BINARY..."
if [ ! -f "$LSP_BINARY" ]; then
    echo "Error: LynxLSPServer binary not found at $LSP_BINARY"
    exit 1
fi

"$LSP_BINARY"

echo "Build, installation, and server run complete."
