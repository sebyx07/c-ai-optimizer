#!/bin/bash

# Benchmark runner - compares O2 baseline, O3 human, and O3 AI-optimized
# Usage: ./benchmark.sh

set -e

echo "=========================================="
echo "  Performance Benchmark Comparison"
echo "=========================================="
echo ""
echo "Comparing three versions:"
echo "  1. O2 Human Code (baseline)"
echo "  2. O3 Human Code (compiler optimized)"
echo "  3. O3 AI-Optimized (SIMD + cache + compiler)"
echo ""

# Build O2 baseline version
echo "Building O2 baseline..."
mkdir -p build/baseline
cd build/baseline
cmake -DBUILD_OPTIMIZED=OFF -DCMAKE_C_FLAGS="-O2 -Wall -Wextra" ../.. > /dev/null 2>&1
make > /dev/null 2>&1
cd ../..

# Build O3 normal version (already built)
if [ ! -f "build/normal/demo" ]; then
    echo "Error: O3 normal build not found. Run ./bin/build.sh first"
    exit 1
fi

# Build O3 optimized version (already built)
if [ ! -f "build/optimized/demo_optimized" ]; then
    echo "Error: O3 optimized build not found. Run ./bin/build.sh first"
    exit 1
fi

echo ""
echo "=========================================="
echo "  Running Benchmarks"
echo "=========================================="

echo ""
echo "=== 1. O2 Human Code (Baseline) ==="
echo "Running..."
./build/baseline/demo > /tmp/benchmark_o2.txt 2>&1
cat /tmp/benchmark_o2.txt | grep -A 10 "Matrix Multiply Benchmark"

echo ""
echo "=== 2. O3 Human Code ==="
echo "Running..."
./build/normal/demo > /tmp/benchmark_o3.txt 2>&1
cat /tmp/benchmark_o3.txt | grep -A 10 "Matrix Multiply Benchmark"

echo ""
echo "=== 3. O3 AI-Optimized (SIMD + Cache) ==="
echo "Running..."
./build/optimized/demo_optimized > /tmp/benchmark_ai.txt 2>&1
cat /tmp/benchmark_ai.txt | grep -A 10 "Matrix Multiply Benchmark"

echo ""
echo "=========================================="
echo "  Performance Summary"
echo "=========================================="
echo ""
echo "1. O2 Human Code (Baseline):"
grep "Matrix" /tmp/benchmark_o2.txt | grep "multiply:"
echo ""
echo "2. O3 Human Code (+Compiler Optimization):"
grep "Matrix" /tmp/benchmark_o3.txt | grep "multiply:"
echo ""
echo "3. O3 AI-Optimized (+SIMD +Cache +Compiler):"
grep "Matrix" /tmp/benchmark_ai.txt | grep "multiply:"
echo ""

# Calculate speedups
echo "=========================================="
echo "  Speedup Analysis"
echo "=========================================="
echo ""

# Extract times for 200x200 matrix (most representative)
o2_time=$(grep "Matrix 200x200 multiply:" /tmp/benchmark_o2.txt | grep -oP '\d+\.\d+' | head -1)
o3_time=$(grep "Matrix 200x200 multiply:" /tmp/benchmark_o3.txt | grep -oP '\d+\.\d+' | head -1)
ai_time=$(grep "Matrix 200x200 multiply:" /tmp/benchmark_ai.txt | grep -oP '\d+\.\d+' | head -1)

if [ ! -z "$o2_time" ] && [ ! -z "$o3_time" ] && [ ! -z "$ai_time" ]; then
    echo "200x200 Matrix Multiplication:"
    echo "  O2 Human:        ${o2_time} ms (baseline)"

    o3_speedup=$(echo "scale=2; $o2_time / $o3_time" | bc)
    echo "  O3 Human:        ${o3_time} ms (${o3_speedup}x faster)"

    ai_speedup=$(echo "scale=2; $o2_time / $ai_time" | bc)
    ai_vs_o3=$(echo "scale=2; $o3_time / $ai_time" | bc)
    echo "  O3 AI-Optimized: ${ai_time} ms (${ai_speedup}x faster than O2, ${ai_vs_o3}x faster than O3)"
    echo ""

    compiler_gain=$(echo "scale=1; ($o2_time - $o3_time) / $o2_time * 100" | bc)
    ai_gain=$(echo "scale=1; ($o2_time - $ai_time) / $o2_time * 100" | bc)

    echo "Performance Gains:"
    echo "  Compiler (O2→O3):      ${compiler_gain}% improvement"
    echo "  AI Optimizations:      ${ai_gain}% total improvement"
    echo ""
fi

echo "=========================================="
echo ""
echo "AI optimizations include:"
echo "  • AVX SIMD vectorization (4 doubles at once)"
echo "  • Cache-blocked matrix multiplication"
echo "  • FMA (fused multiply-add) instructions"
echo "  • Loop unrolling and multiple accumulators"
echo "  • Aligned memory allocation"
echo ""
