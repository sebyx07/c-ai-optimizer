# Optimized Source Files

This directory contains AI-optimized versions of the source files from `../src/`.

## How It Works

Each optimized file:
- Contains performance-optimized C code (SIMD, inlining, etc.)
- Starts with a hash comment tracking the original source
- Must pass the same tests as the normal version

## Generating Optimized Files

Use Claude Code's `/optimize` command:

```bash
/optimize matrix.c
```

This will create `src_optimized/matrix.c` with optimizations applied.

## Hash Tracking

Each file starts with:
```c
/* OPTIMIZED VERSION - Hash: <sha256-hash-of-original> */
```

Use `bin/check_changes.sh` to see which files need re-optimization.

## Building

The optimized files are automatically used when building with:
```bash
make optimized
# or
cmake -DBUILD_OPTIMIZED=ON ..
```
