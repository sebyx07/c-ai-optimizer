---
description: Optimize a C source file with SIMD, inlining, better types, and other performance enhancements
argument-hint: [source-file]
---

# Code Optimization Task

Optimize the C source file: `$ARGUMENTS`

## Requirements

1. Read the source file from `src/` directory
2. Apply performance optimizations including:
   - SIMD vectorization where applicable (SSE/AVX intrinsics)
   - Function inlining for small hot functions
   - Better type choices (restrict pointers, const qualifiers)
   - Loop unrolling for critical loops
   - Cache-friendly memory access patterns
   - Eliminate redundant calculations

3. Generate the optimized version in `src_optimized/` with the same filename
4. Add a hash comment at the top of the optimized file using this format:
   ```c
   /* OPTIMIZED VERSION - Hash: <sha256-hash-of-original-file> */
   ```
5. Ensure the optimized code:
   - Maintains the same function signatures and API
   - Produces identical results (bit-for-bit if possible, or within epsilon for floats)
   - Can be tested with the same test suite
   - Is well-commented to explain optimizations

## Optimization Guidelines

- Prioritize readability in comments to explain what optimizations were applied
- Use portable optimizations first, then platform-specific (with #ifdef guards)
- Focus on hot paths and inner loops
- Avoid premature optimization of initialization code
- Maintain the original code structure where possible

## Output

After optimization:
1. Show a summary of optimizations applied
2. Run `bin/compute_hash.sh` on the original file to generate the hash
3. Add the hash comment to the optimized file
4. Confirm the optimized file is saved in `src_optimized/`
