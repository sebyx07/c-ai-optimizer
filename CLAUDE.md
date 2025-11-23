# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a C AI Optimizer project that demonstrates a unique workflow where humans write clean, readable C code and AI optimizes it for performance while maintaining correctness through automated testing and hash-based change tracking.

**Dual codebase architecture:**
- `src/` - Human-written, readable, maintainable source code
- `src_optimized/` - AI-generated, performance-optimized versions with SIMD, inlining, etc.
- `include/` - Shared headers for both versions (maintain API stability)
- `tests/` - Shared test suite that validates both versions produce identical results

## Common Commands

### Building
```bash
# Build both versions (normal + optimized)
make build

# Build individually
make normal      # Build from src/
make optimized   # Build from src_optimized/

# Clean all builds
make clean
```

### Testing
```bash
# Run tests on both versions
make test

# Test individually
make test-normal
make test-optimized
```

### Running Demos
```bash
make run-normal      # Run normal version
make run-optimized   # Run optimized version
make run-both        # Run and compare both
```

### Optimization Workflow
```bash
# Use the /optimize slash command to create optimized versions
/optimize matrix.c

# Check which files need re-optimization (hash mismatch)
make check-changes

# Benchmark performance comparison
make benchmark
```

## Build System Architecture

**CMake with dual-build mode:**
- `BUILD_OPTIMIZED=OFF` (default): Compiles from `src/`
- `BUILD_OPTIMIZED=ON`: Compiles from `src_optimized/`, falls back to `src/` if file missing
- Build scripts in `bin/` handle creating separate build directories for each version

**Key insight:** Both versions share the same headers and test suite, ensuring API compatibility and correctness validation.

## Hash-Based Change Tracking

Each optimized file begins with:
```c
/* OPTIMIZED VERSION - Hash: <sha256-of-original> */
```

This enables automatic detection of source changes:
- `bin/compute_hash.sh src/file.c` - Calculate hash of source file
- `bin/check_changes.sh` - Compare all source hashes with optimized versions
- Output: `[OK]`, `[CHANGED]` (needs re-optimization), or `[MISSING]` (not yet optimized)

## Optimization Philosophy

When using `/optimize` command on a source file:

**Apply these optimizations:**
- SIMD vectorization (SSE/AVX intrinsics) for data-parallel operations
- Function inlining hints (`__attribute__((hot, flatten))` or `inline`)
- Restrict pointers and const qualifiers for better aliasing analysis
- Loop unrolling for critical inner loops
- Cache-friendly memory access patterns (e.g., transpose matrices for better locality)
- Eliminate redundant calculations

**Maintain these guarantees:**
- Identical function signatures (API compatibility)
- Identical results (bit-for-bit for integers, within epsilon for floats)
- Must pass the same test suite
- Add comments explaining optimizations applied

**Process:**
1. Read source file from `src/filename.c`
2. Apply optimizations while preserving correctness
3. Compute hash: `bin/compute_hash.sh src/filename.c`
4. Add hash comment at top of optimized file
5. Write to `src_optimized/filename.c`
6. Run tests to validate: `make test`

## Project Structure

**Library modules** (in both src/ and src_optimized/):
- `matrix.c` - Matrix operations (multiply, add, transpose, scale)
- `vector.c` - Vector math operations
- `stats.c` - Statistical functions
- `utils.c` - Utilities and timing functions

**Entry points:**
- `src/main.c` - Demo program showcasing library functionality
- `tests/test_runner.c` - Test suite entry point

**Test organization:**
- Individual test files: `tests/test_matrix.c`, `tests/test_vector.c`, `tests/test_stats.c`
- Tests validate correctness, not performance
- Use exact comparison for integers, epsilon-based for floats

## Development Workflow

**When source files change:**
1. Edit code in `src/` focusing on clarity and correctness
2. Run `make check-changes` to identify which files need re-optimization
3. Use `/optimize filename.c` to regenerate optimized versions
4. Run `make test` to validate both versions pass
5. Run `make benchmark` to measure performance improvements

**When adding new files:**
1. Create implementation in `src/new_feature.c`
2. Create header in `include/new_feature.h`
3. Add tests in `tests/test_new_feature.c`
4. Update `CMakeLists.txt` to include new source and test files
5. Run `/optimize new_feature.c`
6. Validate with `make test`

## Key Principles

**Separation of concerns:**
- `src/` - Optimize for human understanding, not machine execution
- `src_optimized/` - Optimize for machine execution, document for human understanding
- Tests ensure both achieve the same functional outcome

**Hash tracking prevents drift:**
- Optimized versions can get out of sync when source changes
- Automated hash checking catches this immediately
- Re-optimization is explicit and intentional

**Test-driven correctness:**
- All optimizations must preserve correctness
- Shared test suite is the contract between both versions
- Performance gains without correctness are worthless
