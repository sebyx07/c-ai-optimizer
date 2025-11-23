#!/bin/bash

# Compute SHA256 hash of a file
# Usage: ./compute_hash.sh <file>

if [ $# -ne 1 ]; then
    echo "Usage: $0 <file>"
    exit 1
fi

FILE="$1"

if [ ! -f "$FILE" ]; then
    echo "Error: File '$FILE' not found"
    exit 1
fi

# Compute SHA256 hash (works on Linux and macOS)
if command -v sha256sum &> /dev/null; then
    # Linux
    sha256sum "$FILE" | awk '{print $1}'
elif command -v shasum &> /dev/null; then
    # macOS
    shasum -a 256 "$FILE" | awk '{print $1}'
else
    echo "Error: Neither sha256sum nor shasum found"
    exit 1
fi
