#!/bin/bash

PROJECT_ROOT="$(pwd)/../../.."  # Adjust this according to where you start the script

# ===============================
# Build Release
# ===============================
mkdir -p build/linux/release
cd build/linux/release || exit

cmake ../../../ -DCMAKE_BUILD_TYPE=Release
echo
echo "===== Building Release ====="
cmake --build . -- -j$(nproc)

# ===============================
# Build Debug
# ===============================
mkdir -p ../debug
cd ../debug || exit

cmake ../../../ -DCMAKE_BUILD_TYPE=Debug
echo
echo "===== Building Debug ====="
cmake --build . -- -j$(nproc)

echo
echo "Build complete!"
