#!/bin/bash

# Test runner - runs tests for normal and/or optimized versions
# Usage: ./test.sh [normal|optimized|both]

set -e

MODE="${1:-both}"

run_tests() {
    local version=$1
    local test_binary=$2

    echo ""
    echo "=== Running $version tests ==="
    echo "========================================"

    if [ ! -f "$test_binary" ]; then
        echo "Error: Test binary not found: $test_binary"
        echo "Run ./bin/build.sh first"
        return 1
    fi

    "$test_binary"
    local result=$?

    if [ $result -eq 0 ]; then
        echo ""
        echo "✓ $version tests PASSED"
    else
        echo ""
        echo "✗ $version tests FAILED"
    fi

    return $result
}

case "$MODE" in
    normal)
        run_tests "NORMAL" "build/normal/test_runner"
        ;;

    optimized)
        run_tests "OPTIMIZED" "build/optimized/test_optimized"
        ;;

    both)
        NORMAL_RESULT=0
        OPTIMIZED_RESULT=0

        run_tests "NORMAL" "build/normal/test_runner" || NORMAL_RESULT=$?
        run_tests "OPTIMIZED" "build/optimized/test_optimized" || OPTIMIZED_RESULT=$?

        echo ""
        echo "========================================"
        echo "Test Summary:"

        if [ $NORMAL_RESULT -eq 0 ]; then
            echo "  Normal:    ✓ PASSED"
        else
            echo "  Normal:    ✗ FAILED"
        fi

        if [ $OPTIMIZED_RESULT -eq 0 ]; then
            echo "  Optimized: ✓ PASSED"
        else
            echo "  Optimized: ✗ FAILED"
        fi

        echo "========================================"

        if [ $NORMAL_RESULT -ne 0 ] || [ $OPTIMIZED_RESULT -ne 0 ]; then
            exit 1
        fi
        ;;

    *)
        echo "Usage: $0 [normal|optimized|both]"
        exit 1
        ;;
esac
