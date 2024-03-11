#!/bin/sh
# Exclude build/ dir
# Exclude .bf (Brainfuck) files, because I can't get them working properly with cloc right now
cloc_output="$(cloc --exclude-dir=build --exclude-ext=bf --read-lang-def=cloc_definitions.txt .)"

# Replace newlines with \n literals
# Then, skip the first and the last line
sanitized_cloc_output=$(echo "$cloc_output" | sed '1d;$d' | awk '{printf "%s\\n", $0}')

# Perform substitution in new README.md file
cp README.md.mk README.md
sed -i "s|@CLOC_OUTPUT@|${sanitized_cloc_output}|g" "README.md"
