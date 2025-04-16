#!/bin/bash

set -e  # Exit on error

BUILD_DIR="build"

echo "[+] Creating build directory: $BUILD_DIR"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "[+] Running CMake..."
cmake ..

echo "[+] Building the project..."
make -j$(nproc)

echo "[✓] Build completed successfully."

# Optional: symlink binary to /usr/local/bin for CLI use
BIN_PATH="/usr/local/bin/note-db"
if [ -f "note_db" ]; then
    echo "[+] Installing binary to $BIN_PATH (requires sudo)"
    sudo ln -sf "$(pwd)/note_db" "$BIN_PATH"
    echo "[✓] You can now run 'note-db' from anywhere!"
fi
