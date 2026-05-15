#!/bin/bash
echo "=== Building Lab 7 ==="
mkdir -p build
cd build
cmake ..
make -j$(nproc)
echo "=== Build complete ==="
