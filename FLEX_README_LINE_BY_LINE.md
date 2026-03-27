# Flex Line-by-Line Explanation

Source file explained: lexer.l

| Line | Source | Explanation |
|---:|---|---|
| 1 | %option noyywrap nodefault yylineno | Flex options: disables yywrap, enables default-rule safety, and tracks current input line number. |
| 2 |  | Blank line for readability and section separation. |
| 3 | %{ | Start of C prologue copied verbatim into generated lexer C code. |
| 4 | #include <stdio.h> | C include required by lexer helper code. |
| 5 | #include <stdlib.h> | C include required by lexer helper code. |
| 6 | #include <string.h> | C include required by lexer helper code. |
| 7 | #include "parser.tab.h" | C include required by lexer helper code. |
| 8 |  | Blank line for readability and section separation. |
| 9 | #define YY_DECL int next_token(void) | Macro definition customizing lexer behavior. |
| 10 |  | Blank line for readability and section separation. |
| 11 | static int indent_stack[256] = {0}; | Static variable/helper function declaration scoped to lexer file. |
| 12 | static int indent_top = 0; | Static variable/helper function declaration scoped to lexer file. |
| 13 | static int pending_dedents = 0; | Static variable/helper function declaration scoped to lexer file. |
| 14 | static int current_indent = 0; | Static variable/helper function declaration scoped to lexer file. |
| 15 |  | Blank line for readability and section separation. |
| 16 | static char* dup_text(const char* s) { | Static variable/helper function declaration scoped to lexer file. |
| 17 |     size_t n = strlen(s) + 1; | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 18 |     char* out = (char*)malloc(n); | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 19 |     if (!out) { | Conditional logic controlling lexer state/actions. |
| 20 |         fprintf(stderr, "Out of memory\n"); | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 21 |         exit(1); | Terminates on unrecoverable lexer error. |
| 22 |     } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 23 |     memcpy(out, s, n); | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 24 |     return out; | Returns token/value to parser. |
| 25 | } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 26 |  | Blank line for readability and section separation. |
| 27 | #define YY_USER_INIT do { BEGIN(BOL); } while (0) | Macro definition customizing lexer behavior. |
| 28 |  | Blank line for readability and section separation. |
| 29 | static int process_indent(int indent) { | Static variable/helper function declaration scoped to lexer file. |
| 30 |     int current = indent_stack[indent_top]; | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 31 |     if (indent > current) { | Conditional logic controlling lexer state/actions. |
| 32 |         indent_stack[++indent_top] = indent; | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 33 |         return INDENT; | Returns token/value to parser. |
| 34 |     } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 35 |     if (indent < current) { | Conditional logic controlling lexer state/actions. |
| 36 |         while (indent_top > 0 && indent < indent_stack[indent_top]) { | Loop used for indentation-stack processing. |
| 37 |             indent_top--; | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 38 |             pending_dedents++; | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 39 |         } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 40 |         if (indent != indent_stack[indent_top]) { | Conditional logic controlling lexer state/actions. |
| 41 |             fprintf(stderr, "Indentation error at line %d\n", yylineno); | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 42 |             exit(1); | Terminates on unrecoverable lexer error. |
| 43 |         } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 44 |         if (pending_dedents > 0) { | Conditional logic controlling lexer state/actions. |
| 45 |             pending_dedents--; | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 46 |             return DEDENT; | Returns token/value to parser. |
| 47 |         } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 48 |     } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 49 |     return 0; | Returns token/value to parser. |
| 50 | } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 51 | %} | End of C prologue block. |
| 52 |  | Blank line for readability and section separation. |
| 53 | %x BOL | Declares an exclusive start condition (state) used for context-sensitive tokenization. |
| 54 |  | Blank line for readability and section separation. |
| 55 | %% | Section delimiter in Flex file (definitions/rules/user code boundary). |
| 56 |  | Blank line for readability and section separation. |
| 57 | <BOL>[ ]+                 { current_indent += yyleng; } | Regex <BOL>[ ]+ : In BOL state, [ ] is literal space class and + means one-or-more; this counts leading spaces with yyleng. |
| 58 | <BOL>\t+                 { current_indent += 4 * yyleng; } | Regex <BOL>\t+ : In BOL state, \t is tab and + repeats; action converts tabs to indentation width. |
| 59 | <BOL>\r                  { } | Regex <BOL>\r : Matches carriage return at line start; ignored for CRLF compatibility. |
| 60 | <BOL>\n                  { current_indent = 0; return NEWLINE; } | Regex <BOL>\n : Matches blank line at line start; resets current_indent and emits NEWLINE. |
| 61 | <BOL>[#][^\n]*            { current_indent = 0; } | Regex <BOL>[#][^\n]* : Matches full # comment line from # to before newline. |
| 62 | <BOL>[\/][\/][^\n]*      { current_indent = 0; } | Regex <BOL>[\/][\/][^\n]* : Matches full // comment line in BOL state. |
| 63 | <BOL>.                    { | Regex <BOL>. : Catches first real character, pushes it back with unput, then emits INDENT/DEDENT transitions. |
| 64 |                             unput(yytext[0]); | Pushes one character back so it can be re-read by next rule. |
| 65 |                             int t = process_indent(current_indent); | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 66 |                             current_indent = 0; | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 67 |                             BEGIN(INITIAL); | Switches Flex start condition (state). |
| 68 |                             if (t) return t; | Conditional logic controlling lexer state/actions. |
| 69 |                          } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 70 |  | Blank line for readability and section separation. |
| 71 | <INITIAL>[ ]+             { } | Regex <INITIAL>[ ]+ : Matches spaces in normal state and ignores them. |
| 72 | <INITIAL>\t+             { } | Regex <INITIAL>\t+ : Matches tabs in normal state and ignores them. |
| 73 | <INITIAL>\r              { } | Regex <INITIAL>\r : Ignores carriage returns in normal state. |
| 74 | <INITIAL>\n              { BEGIN(BOL); return NEWLINE; } | Regex <INITIAL>\n : Emits NEWLINE and switches to BOL for indentation logic. |
| 75 | <INITIAL>[#][^\n]*        { } | Regex <INITIAL>[#][^\n]* : Matches inline # comments up to line end. |
| 76 | <INITIAL>[\/][\/][^\n]*  { } | Regex <INITIAL>[\/][\/][^\n]* : Matches inline // comments up to line end. |
| 77 |  | Blank line for readability and section separation. |
| 78 | <INITIAL>":"            { return COLON; } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 79 | <INITIAL>"("            { return LPAREN; } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 80 | <INITIAL>")"            { return RPAREN; } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 81 | <INITIAL>","            { return COMMA; } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 82 | <INITIAL>";"            { return SEMI; } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 83 |  | Blank line for readability and section separation. |
| 84 | <INITIAL>"=="           { return EQ; } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 85 | <INITIAL>"!="           { return NE; } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 86 | <INITIAL>"&&"           { return AND; } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 87 | <INITIAL>"\|\|"           { return OR; } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 88 | <INITIAL>">="           { return GE; } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 89 | <INITIAL>"<="           { return LE; } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 90 | <INITIAL>"="            { return ASSIGN; } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 91 | <INITIAL>"!"            { return NOT; } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 92 | <INITIAL>">"            { return GT; } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 93 | <INITIAL>"<"            { return LT; } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 94 | <INITIAL>"+"            { return PLUS; } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 95 | <INITIAL>"-"            { return MINUS; } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 96 | <INITIAL>"*"            { return STAR; } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 97 | <INITIAL>"%"            { return MOD; } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 98 | <INITIAL>"/"            { return SLASH; } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 99 |  | Blank line for readability and section separation. |
| 100 | <INITIAL>"\""([^\\\n]\|(\\.))*"\""  { | Regex for string literal: "([^\\\n]\|(\\.))*". Breakdown: opening quote, then zero-or-more of either non-backslash/non-newline chars OR escaped chars, then closing quote. |
| 101 |                                      yylval.str = dup_text(yytext); | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 102 |                             return STRING; | Returns token/value to parser. |
| 103 |                          } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 104 |  | Blank line for readability and section separation. |
| 105 | <INITIAL>[0-9]+(\.[0-9]+)? { | Regex for number: [0-9]+(\.[0-9]+)?. Breakdown: mandatory integer part, optional decimal part in ( ... ) with ? optional quantifier. |
| 106 |                                      yylval.str = dup_text(yytext); | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 107 |                             return NUMBER; | Returns token/value to parser. |
| 108 |                          } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 109 |  | Blank line for readability and section separation. |
| 110 | <INITIAL>[A-Za-z_][A-Za-z0-9_]* { | Regex for identifiers: first char letter/_ then zero-or-more alnum/_. After matching, code maps it to keyword/type/IDENT. |
| 111 |                             if (strcmp(yytext, "if") == 0 \|\| strcmp(yytext, "iza") == 0) return IF; | Conditional logic controlling lexer state/actions. |
| 112 |                             if (strcmp(yytext, "elif") == 0 \|\| strcmp(yytext, "iza_ila") == 0) return ELIF; | Conditional logic controlling lexer state/actions. |
| 113 |                             if (strcmp(yytext, "else") == 0 \|\| strcmp(yytext, "wailla") == 0) return ELSE; | Conditional logic controlling lexer state/actions. |
| 114 |                             if (strcmp(yytext, "while") == 0 \|\| strcmp(yytext, "ma_dama") == 0) return WHILE; | Conditional logic controlling lexer state/actions. |
| 115 |                             if (strcmp(yytext, "for") == 0 \|\| strcmp(yytext, "likulli") == 0) return FOR; | Conditional logic controlling lexer state/actions. |
| 116 |                             if (strcmp(yytext, "break") == 0 \|\| strcmp(yytext, "qif") == 0) return BREAK; | Conditional logic controlling lexer state/actions. |
| 117 |                             if (strcmp(yytext, "continue") == 0 \|\| strcmp(yytext, "wasil") == 0) return CONTINUE; | Conditional logic controlling lexer state/actions. |
| 118 |                             if (strcmp(yytext, "return") == 0 \|\| strcmp(yytext, "irji") == 0) return RETURN; | Conditional logic controlling lexer state/actions. |
| 119 |                             if (strcmp(yytext, "print") == 0 \|\| strcmp(yytext, "qul") == 0) return PRINT; | Conditional logic controlling lexer state/actions. |
| 120 |                             if (strcmp(yytext, "satr") == 0) return SATR; | Conditional logic controlling lexer state/actions. |
| 121 |                             if (strcmp(yytext, "def") == 0) return DEF; | Conditional logic controlling lexer state/actions. |
| 122 |  | Blank line for readability and section separation. |
| 123 |                             if (strcmp(yytext, "int") == 0 \|\| strcmp(yytext, "adad") == 0) { | Conditional logic controlling lexer state/actions. |
| 124 |                                 yylval.str = dup_text("int"); | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 125 |                                 return TYPE; | Returns token/value to parser. |
| 126 |                             } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 127 |                             if (strcmp(yytext, "double") == 0 \|\| strcmp(yytext, "haqiqi") == 0) { | Conditional logic controlling lexer state/actions. |
| 128 |                                 yylval.str = dup_text("double"); | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 129 |                                 return TYPE; | Returns token/value to parser. |
| 130 |                             } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 131 |                             if (strcmp(yytext, "char") == 0 \|\| strcmp(yytext, "harf") == 0) { | Conditional logic controlling lexer state/actions. |
| 132 |                                 yylval.str = dup_text("char"); | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 133 |                                 return TYPE; | Returns token/value to parser. |
| 134 |                             } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 135 |                             if (strcmp(yytext, "void") == 0 \|\| strcmp(yytext, "fadi") == 0) { | Conditional logic controlling lexer state/actions. |
| 136 |                                 yylval.str = dup_text("void"); | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 137 |                                 return TYPE; | Returns token/value to parser. |
| 138 |                             } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 139 |  | Blank line for readability and section separation. |
| 140 |                             yylval.str = dup_text(yytext); | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 141 |                             return IDENT; | Returns token/value to parser. |
| 142 |                          } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 143 |  | Blank line for readability and section separation. |
| 144 | <INITIAL>.                { | Regex catch-all <INITIAL>. : matches any remaining single char and reports invalid token. |
| 145 |                             fprintf(stderr, "Invalid token '%s' at line %d\n", yytext, yylineno); | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 146 |                             exit(1); | Terminates on unrecoverable lexer error. |
| 147 |                          } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 148 |  | Blank line for readability and section separation. |
| 149 | %% | Section delimiter in Flex file (definitions/rules/user code boundary). |
| 150 |  | Blank line for readability and section separation. |
| 151 | int yylex(void) { | Custom yylex wrapper: emits pending DEDENT tokens and final dedents at EOF. |
| 152 |     if (pending_dedents > 0) { | Conditional logic controlling lexer state/actions. |
| 153 |         pending_dedents--; | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 154 |         return DEDENT; | Returns token/value to parser. |
| 155 |     } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 156 |  | Blank line for readability and section separation. |
| 157 |     int tok = next_token(); | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 158 |  | Blank line for readability and section separation. |
| 159 |     if (tok == 0 && indent_top > 0) { | Conditional logic controlling lexer state/actions. |
| 160 |         indent_top--; | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 161 |         return DEDENT; | Returns token/value to parser. |
| 162 |     } | Implementation line participating in lexer state handling, token emission, or helper logic. |
| 163 |  | Blank line for readability and section separation. |
| 164 |     return tok; | Returns token/value to parser. |
| 165 | } | Implementation line participating in lexer state handling, token emission, or helper logic. |
