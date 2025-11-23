# C AI Optimizer

A demonstration project showing how AI can optimize C code while maintaining correctness through automated testing and hash-based change tracking.

## Concept

This project implements a unique workflow:

```
┌─────────────┐         ┌─────────────┐         ┌─────────────┐
│   Human     │         │     AI      │         │     GCC     │
│  Developer  │────────>│  Optimizer  │────────>│  Compiler   │
└─────────────┘         └─────────────┘         └─────────────┘
      │                       │                       │
   Writes                 Applies                 Produces
   Clean,                 SIMD, inlining,         Fast
   Readable               better types,           Binary
   Code                   optimizations

                    ┌──────────────┐
                    │  Test Suite  │
                    │  (Validates) │
                    └──────────────┘
                           │
                     Both versions
                     must pass!
```

### The Workflow

1. **Human writes clean, readable C code** (`src/*.c`)
   - Focus on correctness and maintainability
   - No premature optimization
   - Good variable names, clear logic

2. **AI optimizes the code** (`src_optimized/*.c`)
   - Applies SIMD vectorization (SSE/AVX)
   - Adds function inlining hints
   - Uses restrict pointers and const qualifiers
   - Unrolls critical loops
   - Optimizes memory access patterns
   - Makes code faster but less readable

3. **Hash tracking prevents drift**
   - Each optimized file contains a hash of the original
   - `bin/check_changes.sh` detects when source files change
   - Ensures optimized versions stay in sync

4. **Shared test suite validates both versions**
   - Same tests run against normal and optimized code
   - Guarantees identical behavior
   - Builds confidence in optimizations

## Project Structure

```
c-ai-optimizer/
├── src/                    # Human-written, readable source code
│   ├── main.c
│   ├── matrix.c           # Matrix operations
│   ├── vector.c           # Vector math
│   ├── stats.c            # Statistical functions
│   └── utils.c            # Utilities and timing
│
├── src_optimized/         # AI-optimized versions (generated)
│   ├── matrix.c           # Optimized with SIMD, inlining, etc.
│   ├── vector.c           # Each file contains hash of original
│   └── ...
│
├── include/               # Header files (shared by both versions)
│   ├── matrix.h
│   ├── vector.h
│   ├── stats.h
│   └── utils.h
│
├── tests/                 # Test suite (validates both versions)
│   ├── test_runner.c
│   ├── test_matrix.c
│   ├── test_vector.c
│   └── test_stats.c
│
├── bin/                   # Automation scripts
│   ├── build.sh           # Build normal/optimized/both
│   ├── test.sh            # Run tests
│   ├── benchmark.sh       # Performance comparison
│   ├── compute_hash.sh    # Calculate file hashes
│   └── check_changes.sh   # Detect files needing re-optimization
│
├── .claude/commands/      # Claude Code slash commands
│   └── optimize.md        # /optimize command for AI optimization
│
├── CMakeLists.txt         # CMake build configuration
├── Makefile               # Convenient build targets
└── README.md              # This file
```

## Getting Started

### Prerequisites

- GCC or Clang compiler
- CMake 3.10+
- Make
- sha256sum or shasum (for hash computation)

### Building

```bash
# Build both versions
make build

# Or build individually
make normal      # Build only normal version
make optimized   # Build only optimized version
```

### Running Tests

```bash
# Run all tests
make test

# Run tests for specific version
make test-normal
make test-optimized
```

### Running Demos

```bash
# Run the normal version
make run-normal

# Run the optimized version
make run-optimized

# Run both and compare
make run-both
```

### Performance Benchmarking

```bash
# Compare performance of both versions
make benchmark
```

## AI Optimization Workflow

### Step 1: Write Normal Code

Write clean, readable C code in `src/`. Focus on correctness, not performance.

Example (`src/matrix.c`):
```c
Matrix* matrix_multiply(const Matrix *a, const Matrix *b) {
    Matrix *result = matrix_create(a->rows, b->cols);

    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            result->data[i * result->cols + j] = sum;
        }
    }

    return result;
}
```

### Step 2: AI Optimizes

Use the `/optimize` command with Claude Code:

```bash
/optimize matrix.c
```

The AI will:
- Apply SIMD vectorization to the inner loop
- Add inlining hints
- Use restrict pointers
- Optimize memory access patterns
- Add performance comments
- Calculate and embed the source hash

Example optimized output (`src_optimized/matrix.c`):
```c
/* OPTIMIZED VERSION - Hash: a3f2e9... */

#include <immintrin.h>  // For AVX intrinsics

// Matrix multiply with AVX2 SIMD vectorization
__attribute__((hot, flatten))
Matrix* matrix_multiply(const Matrix* restrict a,
                       const Matrix* restrict b) {
    // ... highly optimized implementation ...
}
```

### Step 3: Verify Correctness

```bash
# Run tests on both versions
make test
```

Both versions must pass all tests. If the optimized version fails, the optimization needs to be corrected.

### Step 4: Track Changes

```bash
# Check if source files have changed
make check-changes
```

This compares the hash in each optimized file with the current source hash. If they differ, you need to re-run `/optimize`.

## Automation Scripts

### `bin/build.sh`

Build script with multiple modes:
- `./bin/build.sh normal` - Build normal version
- `./bin/build.sh optimized` - Build optimized version
- `./bin/build.sh both` - Build both (default)
- `./bin/build.sh clean` - Clean all builds

### `bin/test.sh`

Test runner:
- `./bin/test.sh normal` - Test normal version
- `./bin/test.sh optimized` - Test optimized version
- `./bin/test.sh both` - Test both (default)

### `bin/benchmark.sh`

Runs performance benchmarks on both versions and compares results.

### `bin/compute_hash.sh`

Computes SHA256 hash of a file:
```bash
./bin/compute_hash.sh src/matrix.c
```

### `bin/check_changes.sh`

Checks all source files to see if they've changed since optimization:
```bash
./bin/check_changes.sh
```

Output:
```
[   OK    ] matrix.c
[ CHANGED ] vector.c
[ MISSING ] stats.c
```

## Development Tips

### When to Re-optimize

Run `make check-changes` regularly. Re-optimize files when:
- Source code changes
- New optimizations become available
- Target platform changes

### Maintaining Both Versions

- **Source (`src/`)**: Human edits here, focus on clarity
- **Optimized (`src_optimized/`)**: AI generates, focus on speed
- **Headers (`include/`)**: Shared by both, maintain API stability
- **Tests (`tests/`)**: Validate both versions produce same results

### Adding New Files

1. Write clean implementation in `src/new_feature.c`
2. Create header in `include/new_feature.h`
3. Add tests in `tests/test_new_feature.c`
4. Update CMakeLists.txt to include new files
5. Run `/optimize new_feature.c` to create optimized version
6. Run `make test` to validate

## Example: Optimizing a New File

```bash
# 1. Check current status
make check-changes

# 2. Use Claude Code to optimize
/optimize vector.c

# 3. Build both versions
make build

# 4. Run tests
make test

# 5. Benchmark performance
make benchmark

# 6. Verify hash tracking
make check-changes
```

## Implementation Details

### Build System

The project uses CMake with a `BUILD_OPTIMIZED` option:

- `BUILD_OPTIMIZED=OFF` (default): Builds from `src/`
- `BUILD_OPTIMIZED=ON`: Builds from `src_optimized/`, falling back to `src/` if file doesn't exist

### Hash Tracking

Each optimized file starts with:
```c
/* OPTIMIZED VERSION - Hash: <sha256-of-original> */
```

The `check_changes.sh` script:
1. Computes current hash of source file
2. Extracts stored hash from optimized file
3. Compares hashes
4. Reports status

### Test Suite

Tests are designed to validate correctness, not performance:
- Test exact output for integer operations
- Test floating-point results within epsilon
- Test edge cases and error handling

## Future Enhancements

- [ ] Auto-vectorization reports
- [ ] Performance regression detection
- [ ] Platform-specific optimizations (ARM NEON, etc.)
- [ ] Compiler flag exploration
- [ ] Integration with CI/CD
- [ ] Visual performance comparisons

## License

This is a demonstration project. Use freely for learning and experimentation.

## Contributing

This project demonstrates a workflow. Feel free to:
- Add new algorithms to optimize
- Improve the optimization strategies
- Enhance the testing framework
- Add platform-specific optimizations
