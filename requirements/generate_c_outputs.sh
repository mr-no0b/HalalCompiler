#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")/.."

# Build compiler first
bison -d parser.y
flex lexer.l
gcc -std=c11 -Wall -Wextra -Wpedantic -D_POSIX_C_SOURCE=200809L -o halalc parser.tab.c lex.yy.c main.c -lfl

mkdir -p requirements/output

count=0
for in_file in requirements/input/*.input.halal; do
  base="$(basename "$in_file" .input.halal)"
  out_c="requirements/output/${base}.c"
  ./halalc "$in_file" "$out_c" >/dev/null
  echo "generated: $out_c"
  count=$((count + 1))
done

echo "done: generated $count C file(s) in requirements/output"
