# Makefile for C AI Optimizer Project
# Provides convenient targets for building, testing, and benchmarking

.PHONY: all clean build test benchmark check-changes help normal optimized both

# Default target
all: build

# Build targets
build:
	@./bin/build.sh both

normal:
	@./bin/build.sh normal

optimized:
	@./bin/build.sh optimized

both:
	@./bin/build.sh both

# Test targets
test:
	@./bin/test.sh both

test-normal:
	@./bin/test.sh normal

test-optimized:
	@./bin/test.sh optimized

# Benchmark
benchmark: build
	@./bin/benchmark.sh

# Check which files need re-optimization
check-changes:
	@./bin/check_changes.sh

# Clean build artifacts
clean:
	@./bin/build.sh clean

# Run the demo programs
run-normal: normal
	@echo "Running normal version:"
	@./build/normal/demo

run-optimized: optimized
	@echo "Running optimized version:"
	@./build/optimized/demo_optimized

run-both: both
	@echo "=== Running Normal Version ==="
	@./build/normal/demo
	@echo ""
	@echo "=== Running Optimized Version ==="
	@./build/optimized/demo_optimized

# Help target
help:
	@echo "C AI Optimizer - Makefile Targets"
	@echo "=================================="
	@echo ""
	@echo "Build targets:"
	@echo "  make build         - Build both normal and optimized versions (default)"
	@echo "  make normal        - Build only normal version"
	@echo "  make optimized     - Build only optimized version"
	@echo "  make both          - Build both versions"
	@echo ""
	@echo "Test targets:"
	@echo "  make test          - Run tests for both versions"
	@echo "  make test-normal   - Run tests for normal version only"
	@echo "  make test-optimized- Run tests for optimized version only"
	@echo ""
	@echo "Run targets:"
	@echo "  make run-normal    - Run the normal demo"
	@echo "  make run-optimized - Run the optimized demo"
	@echo "  make run-both      - Run both demos"
	@echo ""
	@echo "Benchmark:"
	@echo "  make benchmark     - Compare performance of both versions"
	@echo ""
	@echo "Utilities:"
	@echo "  make check-changes - Check which files need re-optimization"
	@echo "  make clean         - Remove all build artifacts"
	@echo "  make help          - Show this help message"
	@echo ""
