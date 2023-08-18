#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 [filename]"
    exit 1
fi

filename="$1"

if [ ! -f "$filename" ]; then
    echo "File not found: $filename"
    exit 1
fi

# Read each line from the file and execute non-comment lines
while IFS= read -r line; do
    # Remove leading and trailing whitespace from the line
    trimmed_line="$(echo "$line" | sed -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//')"

    # Skip empty lines and lines starting with #
    if [ -n "$trimmed_line" ] && [ "${trimmed_line:0:1}" != "#" ]; then
        eval "$trimmed_line"
    fi
done < "$filename"
