#!/bin/bash

# Check which source files need to be re-optimized
# Compares hashes of original files with hashes stored in optimized versions

echo "Checking for files that need re-optimization..."
echo "==============================================="

NEEDS_UPDATE=0

for src_file in src/*.c; do
    # Skip if file doesn't exist
    [ -e "$src_file" ] || continue

    # Get basename
    filename=$(basename "$src_file")
    opt_file="src_optimized/$filename"

    # Compute current hash of source file
    current_hash=$(./bin/compute_hash.sh "$src_file" 2>/dev/null)

    if [ ! -f "$opt_file" ]; then
        echo "[ MISSING ] $filename - No optimized version exists"
        NEEDS_UPDATE=1
    else
        # Extract hash from optimized file
        stored_hash=$(head -n 1 "$opt_file" | grep -oP 'Hash: \K[a-f0-9]+' || echo "")

        if [ -z "$stored_hash" ]; then
            echo "[ NO HASH ] $filename - Optimized version has no hash"
            NEEDS_UPDATE=1
        elif [ "$current_hash" != "$stored_hash" ]; then
            echo "[ CHANGED ] $filename"
            echo "            Original hash: $current_hash"
            echo "            Stored hash:   $stored_hash"
            NEEDS_UPDATE=1
        else
            echo "[   OK    ] $filename"
        fi
    fi
done

echo ""
echo "==============================================="

if [ $NEEDS_UPDATE -eq 1 ]; then
    echo "Some files need to be re-optimized."
    echo "Run: /optimize <filename> to update individual files"
    exit 1
else
    echo "All files are up to date!"
    exit 0
fi
