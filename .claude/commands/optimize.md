---
description: Optimize a C source file with SIMD, OpenMP, cache-friendly patterns, and performance enhancements
argument-hint: [source-file]
---

# Code Optimization Task

Optimize the C source file: `$ARGUMENTS`

## Critical Rules - API Compatibility

**NEVER CHANGE FUNCTION SIGNATURES**
- **NO restrict pointers** - Original API must support aliasing (e.g., `matrix_add(m, m)`)
- **NO changing parameter types** - Keep exact same types as headers
- **Maintain const-correctness** as defined in headers
- Tests must validate identical behavior, including aliasing cases

## Optimization Strategy (in priority order)

### 1. **OpenMP Parallelization** (Highest Impact)
   - Add `#ifdef _OPENMP` guards with fallback
   - Use `#pragma omp parallel for` on outer loops of matrix operations
   - Use `schedule(dynamic, chunk_size)` for load balancing
   - **This is often the biggest win** - single-threaded "optimizations" miss the point

### 2. **Cache-Friendly Memory Access**
   - **Loop ordering matters**: Access memory in stride-1 patterns when possible
     - For matrix multiply: i-k-j order allows streaming through B's rows
     - For transpose: Use cache blocking to fit working set in L1/L2
   - **Cache blocking**: Use block sizes (32-64) that fit in cache
   - **Sequential access**: Process contiguous memory when possible

### 3. **SIMD Vectorization (AVX/SSE)**
   - Use `#ifdef __AVX__` with scalar fallback
   - **Vectorize contiguous memory accesses** - not scattered lookups!
   - Example: For matrix multiply, broadcast scalar and multiply vector
   - Always provide scalar cleanup loop for remainder elements
   - **Test with large inputs** - small matrices don't exercise SIMD paths

### 4. **Compiler Hints**
   - Add `const` qualifiers where appropriate (helps aliasing analysis)
   - Use `_mm256_loadu_pd` for unaligned, `_mm256_load_pd` for aligned (if guaranteed)
   - Consider aligned_alloc for data buffers (with proper alignment)

### 5. **What NOT to Do**
   - ❌ **NO restrict pointers** (breaks API compatibility with aliasing)
   - ❌ **NO scattered memory access in SIMD** (kills cache, destroys performance)
   - ❌ **NO "optimizations" that only work on tiny matrices** (tests must use 100k+ elements)
   - ❌ **NO changing API** (different semantics from original = broken code)
   - ❌ **NO premature micro-optimizations** (OpenMP + good cache behavior >> clever tricks)

## Example: Matrix Multiply Anti-Pattern

**WRONG** (scattered B matrix access):
```c
for each i, j, k:
    __m256d b_vec = _mm256_set_pd(
        b[(k+3) * cols + j],  // Different rows - cache miss!
        b[(k+2) * cols + j],  // Jumping megabytes for large matrices
        b[(k+1) * cols + j],
        b[k * cols + j]);
```

**RIGHT** (streaming access):
```c
for i, k:
    a_scalar = a[i][k]
    broadcast a_scalar to vector
    for j in steps of 4:
        load b[k][j:j+4]      // Contiguous in memory
        fmadd into result[i][j:j+4]
```

## Verification Requirements

After optimization, you **MUST**:
1. Compute hash: `bin/compute_hash.sh src/<filename>`
2. Add hash comment: `/* OPTIMIZED VERSION - Hash: <hash> */`
3. Write to `src_optimized/<filename>`
4. **Verify tests pass** - especially comprehensive tests with:
   - Large matrices (500x500+, 1M elements)
   - Aliasing cases (same pointer multiple times)
   - Non-square matrices
5. **Document optimizations** with comments explaining:
   - Why this approach (e.g., "i-k-j order for cache locality")
   - What SIMD instructions are used
   - Expected performance characteristics

## Output Summary

Provide:
1. List of optimizations applied (OpenMP, cache strategy, SIMD approach)
2. Confirm API compatibility maintained
3. Hash value and file location
4. Test results showing correctness preserved
