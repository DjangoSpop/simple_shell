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

# Read each line from the file and execute the commands
while IFS= read -r command; do
    eval "$command"
done < "$filename"
