#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"

make >/dev/null

pass_count=0
fail_count=0

run_valid() {
    local src="$1"
    local expected="$2"
    local base
    base="$(basename "$src" .halal)"

    if ./halalc "$src" "tests/${base}.c" >/dev/null 2>&1; then
        gcc "tests/${base}.c" -o "tests/${base}.out"
        local got
        got="$("tests/${base}.out" | tr -d '\r')"
        if [[ "$got" == "$expected" ]]; then
            echo "PASS valid: $src"
            pass_count=$((pass_count + 1))
        else
            echo "FAIL valid: $src (expected '$expected', got '$got')"
            fail_count=$((fail_count + 1))
        fi
    else
        echo "FAIL valid: $src (compiler failed)"
        fail_count=$((fail_count + 1))
    fi
}

run_invalid() {
    local src="$1"
    if ./halalc "$src" "tests/tmp.c" >/dev/null 2>&1; then
        echo "FAIL invalid: $src (unexpected success)"
        fail_count=$((fail_count + 1))
    else
        echo "PASS invalid: $src"
        pass_count=$((pass_count + 1))
    fi
}

run_valid "tests/valid_loops.halal" "18"
run_valid "tests/valid_complex_expr.halal" "999"
run_valid "tests/valid_function_args.halal" ""
run_valid "tests/valid_call_expr_assign.halal" "49"
run_invalid "tests/invalid_undeclared.halal"
run_invalid "tests/invalid_break_outside.halal"
run_invalid "tests/invalid_lexical.halal"
run_invalid "tests/invalid_syntax.halal"
run_invalid "tests/invalid_arg_count.halal"
run_invalid "tests/invalid_arg_type.halal"

echo "---"
echo "Passed: $pass_count"
echo "Failed: $fail_count"

if [[ $fail_count -ne 0 ]]; then
    exit 1
fi
