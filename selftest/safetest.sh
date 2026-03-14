#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"

INPUT="${1:-${PROJECT_DIR}/selftest/demo.halal}"
OUTPUT_C="${2:-${PROJECT_DIR}/selftest/demo.c}"
OUTPUT_BIN="${3:-${PROJECT_DIR}/selftest/demo.out}"

if [[ ! -f "${INPUT}" ]]; then
    echo "Input file not found: ${INPUT}" >&2
    exit 1
fi

cd "${PROJECT_DIR}"

echo "[1/4] Building compiler..."
make

echo "[2/4] Transpiling: ${INPUT} -> ${OUTPUT_C}"
./halalc "${INPUT}" "${OUTPUT_C}"

echo "[3/4] Compiling generated C..."
gcc "${OUTPUT_C}" -o "${OUTPUT_BIN}"

echo "[4/4] Running executable..."
"${OUTPUT_BIN}"
