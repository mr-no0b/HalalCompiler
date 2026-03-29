# Halal Compiler (Postfix + Indentation)

This project is a Flex/Bison compiler front-end that transpiles a custom source language into C.

Key syntax choices:
- Postfix (RPN) expressions for assignment and conditions.
- Python-like indentation blocks instead of `{}`.
- Halal keyword style is the default (English aliases are still accepted).
- Loop support includes `ma_dama` (while), `likulli` (for), `qif` (break), and `wasil` (continue).

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

## Run tests
```bash
make test
```

## Syntax examples

### Function with indentation block
```txt
adad add(adad x, adad y):
    irji x y +
```

### Postfix assignment
```txt
adad a b c + =
```
Equivalent C:
```c
int a = (b + c);
```

### If/else with postfix condition
```txt
iza a 10 >:
    qul a
wailla:
    qul 0
```

### While loop
```txt
ma_dama i 10 <:
    i i 1 + =
```

### For loop (postfix style)
```txt
likulli adad i 0 = ; i 10 < ; i i 1 + =:
    qul i
```

### Function call with arguments
```txt
add(3, 4)
```

### Newline control with satr
```txt
qul "My roll is"
satr
qul 41
```

### Input using iqra (scanf style)
```txt
adad roll
iqra("%d", roll)
```

### Supported postfix operators
- Arithmetic: `+`, `-`, `*`, `/`, `%`
- Relational: `>`, `<`, `>=`, `<=`, `==`, `!=`
- Logical: `&&`, `||`, `!`

## Notes
- Statements end by newline.
- Expressions are postfix; assignment operator `=` is written at the end of assignment statements.
- `qul` does not append newline automatically.
- `satr` emits a newline (`printf("\\n")`).
- `iqra` maps to `scanf` style input (`iqra("%d", var)` -> `scanf("%d", &var)`).
- Basic semantic checks are included: undeclared variable use, redeclaration, assignment type checks, return checks, and loop-control misuse.

## Known limitations (current scope)
- Symbol checking is function-level (no full nested block scope yet).
- Type conversion and type system are basic and numeric-focused.
