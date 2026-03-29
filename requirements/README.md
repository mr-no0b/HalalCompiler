# Requirements Test Pack

Folder layout:
- `input/*.input.halal`: source inputs for the compiler.
- `expected/*.expected.txt`: expected behavior/output for manual checking.
- `output/`: generated C files (kept empty until you run generation).

Case map to `requirements_clean_checklist.md`:
- 01: Lexical tokens/keywords (valid)
- 02: Lexical invalid token handling
- 03: Syntax error handling
- 04: Semantic undeclared variable
- 05: Assignment/type checking
- 06: Execution correctness (if/else, loops, function return)
- 07: iqra/scanf single input
- 08: iqra/scanf multi input
- 09: Advanced/unique feature (transpile + function call in expression)

Manual generation (all cases):
- `bash requirements/generate_c_outputs.sh`

Manual generation (single case example):
- `./halalc requirements/input/01_lexical_valid.input.halal requirements/output/01_lexical_valid.c`

Notes:
- For invalid cases, expected text is an error substring.
- For stdin cases, expected files document the intended input/output behavior.
