#!/bin/bash

# Build script - builds both normal and optimized versions
# Usage: ./build.sh [normal|optimized|both|clean]

set -e

MODE="${1:-both}"

case "$MODE" in
    normal)
        echo "Building normal version..."
        mkdir -p build/normal
        cd build/normal
        cmake -DBUILD_OPTIMIZED=OFF ../..
        make
        echo "Normal build complete: build/normal/demo"
        ;;

    optimized)
        echo "Building optimized version..."
        mkdir -p build/optimized
        cd build/optimized
        cmake -DBUILD_OPTIMIZED=ON ../..
        make
        echo "Optimized build complete: build/optimized/demo_optimized"
        ;;

    both)
        echo "Building both versions..."

        echo ""
        echo "=== Building normal version ==="
        mkdir -p build/normal
        cd build/normal
        cmake -DBUILD_OPTIMIZED=OFF ../..
        make
        cd ../..

        echo ""
        echo "=== Building optimized version ==="
        mkdir -p build/optimized
        cd build/optimized
        cmake -DBUILD_OPTIMIZED=ON ../..
        make
        cd ../..

        echo ""
        echo "Build complete!"
        echo "  Normal:    build/normal/demo"
        echo "  Optimized: build/optimized/demo_optimized"
        ;;

    clean)
        echo "Cleaning build directories..."
        rm -rf build/
        echo "Clean complete"
        ;;

    *)
        echo "Usage: $0 [normal|optimized|both|clean]"
        exit 1
        ;;
esac
