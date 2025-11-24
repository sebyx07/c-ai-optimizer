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

**Source code (`src/`) is intentionally simple:**
- Written like a normal programmer would write it
- Prioritizes clarity, maintainability, and correctness over performance
- Uses basic const qualifiers but avoids premature optimization
- **This simplicity is by design** - not a flaw to be "fixed"

**The `/optimize` command transforms simple code into high-performance code:**

**Critical optimization priorities (in order):**
1. **OpenMP parallelization** - Biggest performance win for embarrassingly parallel operations
2. **Cache-friendly memory access** - Proper loop ordering and blocking (i-k-j for matmul, blocking for transpose)
3. **SIMD vectorization** - AVX/SSE for contiguous memory only (no scattered access!)
4. **Const qualifiers** - Helps compiler aliasing analysis (where it doesn't break API)

**NEVER do these (API-breaking or harmful):**
- ❌ **NO restrict pointers** - Breaks API compatibility, prevents valid aliasing like `matrix_add(m, m)`
- ❌ **NO scattered SIMD access** - Destroys cache locality, makes code slower not faster
- ❌ **NO signature changes** - Must maintain exact API from headers

**Maintain these guarantees:**
- Identical function signatures (strict API compatibility)
- Identical results including with aliased pointers
- Must pass comprehensive test suite (small + large + aliasing tests)
- Well-documented optimizations explaining why they work

**Process:**
1. Read source file from `src/filename.c`
2. Apply optimizations following priority order above
3. Compute hash: `bin/compute_hash.sh src/filename.c`
4. Add hash comment at top of optimized file
5. Write to `src_optimized/filename.c`
6. **Run comprehensive tests** to validate: `make test`
   - Tests must include large matrices (100k+ elements) to exercise SIMD/cache behavior
   - Tests must include aliasing cases (same pointer multiple times)

**Human review is essential:**
- AI-generated optimizations are suggestions, not guaranteed correct
- Always review generated code for correctness and actual performance
- Tests alone don't ensure safety - understand what the code does
- Benchmark with realistic workloads, not just micro-tests

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
- Basic tests: `tests/test_matrix.c`, `tests/test_vector.c`, `tests/test_stats.c`
- Comprehensive tests: `tests/test_matrix_comprehensive.c` (large matrices, aliasing, stress tests)
- Tests validate correctness including:
  - Small matrices (basic functionality)
  - Large matrices (100k+ elements to exercise SIMD and cache behavior)
  - Aliasing cases (e.g., `matrix_add(m, m)` must work correctly)
  - Non-square matrices
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
- All optimizations must preserve correctness **including edge cases**
- Comprehensive tests catch:
  - Broken SIMD implementations (small tests miss these)
  - Cache-hostile patterns (only visible with large data)
  - Aliasing bugs (if restrict was incorrectly used)
- Shared test suite is the contract between both versions
- Performance gains without correctness are worthless
- Tests are necessary but not sufficient - human review is critical

**Comparing against reality:**
- Benchmarks should compare against established libraries (e.g., OpenBLAS, Eigen)
- If "optimized" code is still slow vs. good libraries, the optimizations likely missed key techniques
- Use `make benchmark` and analyze performance characteristics, not just wall time
