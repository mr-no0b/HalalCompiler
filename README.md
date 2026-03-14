# Halal Compiler (Postfix + Indentation)

This project is a Flex/Bison compiler front-end that transpiles a custom source language into C.

Key syntax choices:
- Postfix (RPN) expressions for assignment and conditions.
- Python-like indentation blocks instead of `{}`.
- Supports both English and your Halal keyword style for major constructs.

## Simple project structure
- `lexer.l` - Flex lexer with indentation (`INDENT`/`DEDENT`) handling.
- `parser.y` - Bison grammar and C code generation.
- `main.c` - Compiler driver (`halalc`).
- `Makefile` - Build commands.
- `sample.halal` - Example source program.

## Build
```bash
make
```

## Run compiler
```bash
./halalc sample.halal out.c
```

Then compile generated C:
```bash
gcc out.c -o out
./out
```

## Syntax examples

### Function with indentation block
```txt
def int main():
    return 0
```

### Postfix assignment
```txt
int a b c + =
```
Equivalent C:
```c
int a = (b + c);
```

### If/else with postfix condition
```txt
if a 10 >:
    print a
else:
    print 0
```

## Notes
- Statements end by newline.
- Expressions are postfix; assignment operator `=` is written at the end of assignment statements.
- Current scaffold is intentionally compact and easy to extend for semantic analysis, symbol tables, and optimization.
