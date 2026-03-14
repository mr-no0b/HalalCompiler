# Compiler Project Requirements (Clean Checklist)

## 1) Lexical Analysis (Flex)
- [ ] Define all required tokens for the language.
- [ ] Implement lexical analysis using Flex.
- [ ] Handle invalid/unknown tokens with clear error reporting.
- [ ] Integrate lexer output correctly with Bison parser.

## 2) Syntax Analysis (Bison)
- [ ] Implement syntax analysis using Bison.
- [ ] Write well-defined grammar rules aligned with the project proposal.
- [ ] Keep parsing logic organized and maintainable.
- [ ] Add syntax error handling and helpful parse error messages.

## 3) Semantic Checks / Language Correctness
- [ ] Support variable declaration rules.
- [ ] Support assignment validation.
- [ ] Implement type checking.
- [ ] Implement implicit type conversion where applicable.

## 4) Correctness of Execution Behavior
- [ ] Expression evaluation works correctly.
- [ ] Conditional statements (`if`/`else`) work correctly.
- [ ] Loops work correctly.
- [ ] Functions work correctly (including return behavior).

### Example correctness scenarios
- [ ] Program can declare variables.
- [ ] Program can assign values.
- [ ] Program can print variables/results.
- [ ] Loops execute correctly.
- [ ] Functions return correct results.

## 5) Unique / Advanced Feature (At Least One)
- [ ] Add at least one advanced feature.

### Example advanced features
- [ ] Intermediate code generation.
- [ ] Any code optimization technique.

## 6) Optional Validation Checklist (Recommended)
- [ ] Create sample test programs for each language feature.
- [ ] Include invalid-input test cases (lexical, syntax, semantic).
- [ ] Document known limitations of the current version.
