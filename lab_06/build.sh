#!/bin/bash
echo "=== Building Lab 6 ==="
mkdir -p build
cd build
cmake ..
make -j$(nproc)
echo "=== Build complete ==="
