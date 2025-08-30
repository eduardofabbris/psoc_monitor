# @disclaimer: created with chatgpt
#!/bin/bash

# Loop through all .txt files in the current directory
for file in *.txt; do
    # Skip if no .txt files found
    [ -e "$file" ] || continue

    echo "🔍 Running report_gen.py on $file"
    python3 report_gen.py "$file"
done

