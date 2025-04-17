#!/bin/bash

set -e  # Exit on error

BUILD_DIR="build"

echo "[+] Ensuring build directory exists: $BUILD_DIR"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "[+] Configuring CMake..."
cmake .. -DBUILD_TESTING=ON > /dev/null

echo "[+] Building tests..."
make -j$(nproc) > /dev/null

echo "[+] Running tests..."
ctest --output-on-failure
