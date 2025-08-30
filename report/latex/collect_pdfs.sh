# @disclaimer: created with chatgpt
#!/bin/bash

# Destination folder
DEST_DIR="./pdfs"

# Create destination folder if it doesn't exist
mkdir -p "$DEST_DIR"

# Find and copy all .pdf files from subdirectories
find . -type f -name "*.pdf" | while read -r pdf_file; do
    # Get base name of the file (e.g., report.pdf)
    base_name=$(basename "$pdf_file")

    if [ "$pdf_file" != "$DEST_DIR/$base_name" ]; then
        echo "📄 Copying: $pdf_file → $DEST_DIR/$base_name"
        cp "$pdf_file" "$DEST_DIR/$base_name"
    fi
done
