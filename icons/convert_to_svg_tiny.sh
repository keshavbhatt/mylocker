#!/bin/bash

# This util converts svg files to SVG 1.2 specification.

#Qt's SVG rendering is based on SVG 1.2 Tiny,
#which is a subset of the full SVG specification. Some SVG features beyond SVG 1.2 Tiny might not be supported by Qt.

INPUT_DIR="./svg"
OUTPUT_DIR="./svg_tiny"

mkdir -p "$OUTPUT_DIR"

for file in "$INPUT_DIR"/*.svg; do
    filename=$(basename "$file")
    output_file="$OUTPUT_DIR/$filename"

    # Convert fill="rgba(r,g,b,1)" to hex color
    content=$(sed -E 's/fill="rgba\(([0-9]+), *([0-9]+), *([0-9]+), *1\)"/fill="#\1\2\3"/g' "$file")

    # Convert RGB values to proper hex
    content=$(echo "$content" | perl -pe '
        s/fill="#(\d{1,3})(\d{1,3})(\d{1,3})"/"fill=\"#" . sprintf("%02X%02X%02X", $1, $2, $3) . "\""/eg
    ')

    # Add version and baseProfile
    content=$(echo "$content" | sed -E '
        s/<svg([^>]*)>/<svg\1 version="1.2" baseProfile="tiny" xmlns:svg="http:\/\/www.w3.org\/2000\/svg">/
    ')

    echo "$content" > "$output_file"
    echo "Converted: $filename -> $output_file"
done

echo "All files processed. Output in: $OUTPUT_DIR"
