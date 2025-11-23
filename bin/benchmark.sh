#!/bin/bash

# Benchmark runner - compares performance of normal vs optimized versions
# Usage: ./benchmark.sh

set -e

echo "=========================================="
echo "  Performance Benchmark"
echo "=========================================="

if [ ! -f "build/normal/demo" ]; then
    echo "Error: Normal build not found. Run ./bin/build.sh first"
    exit 1
fi

if [ ! -f "build/optimized/demo_optimized" ]; then
    echo "Warning: Optimized build not found."
    echo "Only benchmarking normal version."
    echo ""

    echo "=== Normal Version ==="
    ./build/normal/demo
    exit 0
fi

echo ""
echo "=== Normal Version ==="
echo "Running..."
./build/normal/demo > /tmp/benchmark_normal.txt 2>&1
cat /tmp/benchmark_normal.txt | grep -A 10 "Matrix Multiply Benchmark"

echo ""
echo "=== Optimized Version ==="
echo "Running..."
./build/optimized/demo_optimized > /tmp/benchmark_optimized.txt 2>&1
cat /tmp/benchmark_optimized.txt | grep -A 10 "Matrix Multiply Benchmark"

echo ""
echo "=========================================="
echo "  Comparison"
echo "=========================================="
echo ""
echo "Normal version:"
grep "Matrix" /tmp/benchmark_normal.txt | grep "multiply:"
echo ""
echo "Optimized version:"
grep "Matrix" /tmp/benchmark_optimized.txt | grep "multiply:"
echo ""
echo "=========================================="
