#!/bin/bash

# Copy Makeifle template to directories
find . -type f -name "*.tex" | while read -r tex_file; do
    dir=$(dirname "$tex_file")
    echo "include ../Makefile" > "$dir/Makefile"
done

# Find all .tex files in subdirectories
find . -type f -name "*.tex" | while read -r tex_file; do
    dir=$(dirname "$tex_file")
    base=$(basename "$tex_file" .tex)
    pdf_file="$dir/$base.pdf"

    # Check if PDF exists
    if [ -f "$pdf_file" ]; then
        echo "✅ Skipping '$tex_file' — PDF already exists."
    else
        echo "🔧 Compiling '$tex_file' in '$dir'..."
        (cd "$dir" && make)
    fi
done
