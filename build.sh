#!/bin/bash

# Detect and create default conan profile if it doesn't exist
echo "Checking conan profile..."
if [ ! -f ~/.conan2/profiles/default ]; then
    echo "Creating default conan profile..."
    conan profile detect --force
fi

# Install build essentials if not present
if ! command -v g++ &>/dev/null || ! command -v make &>/dev/null; then
    echo "Installing build essentials..."
    apt-get update && apt-get install -y build-essential cmake
fi

# Clean previous build
rm -rf build

# Create build directory
mkdir -p build
cd build

# Use conan to install dependencies
echo "Installing dependencies with conan..."
conan install .. --build=missing

# Configure project with cmake
echo "Configuring project with cmake..."
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake

# Build the project
echo "Building project..."
cmake --build .

echo "Build completed. Executable is in build/bin/"