# Bison Line-by-Line Explanation

Source file explained: parser.y

| Line | Source | Explanation |
|---:|---|---|
| 1 | %{ | Start of Bison C prologue copied into generated parser C source. |
| 2 | #include <stdio.h> | C header include required by parser helper implementation. |
| 3 | #include <stdlib.h> | C header include required by parser helper implementation. |
| 4 | #include <string.h> | C header include required by parser helper implementation. |
| 5 | #include <stdarg.h> | C header include required by parser helper implementation. |
| 6 | #include <ctype.h> | C header include required by parser helper implementation. |
| 7 |  | Blank line for readability and grouping of grammar/code sections. |
| 8 | static char* generated_program = NULL; | Static helper declaration or definition used by parser and semantic phase. |
| 9 | static int semantic_errors = 0; | Static helper declaration or definition used by parser and semantic phase. |
| 10 | extern int yylineno; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 11 |  | Blank line for readability and grouping of grammar/code sections. |
| 12 | typedef enum { | Enum declaration used by semantic type system. |
| 13 |   TY_INT, | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 14 |   TY_DOUBLE, | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 15 |   TY_CHAR, | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 16 |   TY_VOID, | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 17 |   TY_UNKNOWN | Grammar nonterminal label line introducing productions. |
| 18 | } ValueType; | Closes a semantic action or C code block. |
| 19 |  | Blank line for readability and grouping of grammar/code sections. |
| 20 | typedef struct { | Struct declaration for symbol/function tables. |
| 21 |   char name[64]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 22 |   ValueType type; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 23 | } Symbol; | Closes a semantic action or C code block. |
| 24 |  | Blank line for readability and grouping of grammar/code sections. |
| 25 | typedef struct { | Struct declaration for symbol/function tables. |
| 26 |   char name[64]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 27 |   ValueType ret_type; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 28 |   int arg_count; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 29 |   ValueType arg_types[64]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 30 | } FunctionInfo; | Closes a semantic action or C code block. |
| 31 |  | Blank line for readability and grouping of grammar/code sections. |
| 32 | static Symbol symbols[1024]; | Static helper declaration or definition used by parser and semantic phase. |
| 33 | static int symbol_count = 0; | Static helper declaration or definition used by parser and semantic phase. |
| 34 |  | Blank line for readability and grouping of grammar/code sections. |
| 35 | static FunctionInfo functions[256]; | Static helper declaration or definition used by parser and semantic phase. |
| 36 | static int function_count = 0; | Static helper declaration or definition used by parser and semantic phase. |
| 37 |  | Blank line for readability and grouping of grammar/code sections. |
| 38 | static ValueType current_function_type = TY_UNKNOWN; | Static helper declaration or definition used by parser and semantic phase. |
| 39 | static int loop_depth = 0; | Static helper declaration or definition used by parser and semantic phase. |
| 40 | extern int yylineno; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 41 |  | Blank line for readability and grouping of grammar/code sections. |
| 42 | static int pending_param_count = 0; | Static helper declaration or definition used by parser and semantic phase. |
| 43 | static ValueType pending_param_types[64]; | Static helper declaration or definition used by parser and semantic phase. |
| 44 | static char pending_param_names[64][64]; | Static helper declaration or definition used by parser and semantic phase. |
| 45 |  | Blank line for readability and grouping of grammar/code sections. |
| 46 | static int pending_call_arg_count = 0; | Static helper declaration or definition used by parser and semantic phase. |
| 47 | static ValueType pending_call_arg_types[64]; | Static helper declaration or definition used by parser and semantic phase. |
| 48 | static ValueType last_call_expr_type = TY_UNKNOWN; | Static helper declaration or definition used by parser and semantic phase. |
| 49 |  | Blank line for readability and grouping of grammar/code sections. |
| 50 | static char* xstrdup(const char* s) { | Static helper declaration or definition used by parser and semantic phase. |
| 51 |     size_t n = strlen(s) + 1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 52 |     char* out = (char*)malloc(n); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 53 |     if (!out) { | Opens a semantic action or C code block. |
| 54 |         fprintf(stderr, "Out of memory\n"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 55 |         exit(1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 56 |     } | Closes a semantic action or C code block. |
| 57 |     memcpy(out, s, n); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 58 |     return out; | Returns computed value/token/type from helper code. |
| 59 | } | Closes a semantic action or C code block. |
| 60 |  | Blank line for readability and grouping of grammar/code sections. |
| 61 | static char* str_printf(const char* fmt, ...) { | Static helper declaration or definition used by parser and semantic phase. |
| 62 |     va_list ap; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 63 |     va_start(ap, fmt); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 64 |     int n = vsnprintf(NULL, 0, fmt, ap); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 65 |     va_end(ap); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 66 |  | Blank line for readability and grouping of grammar/code sections. |
| 67 |     if (n < 0) { | Opens a semantic action or C code block. |
| 68 |         fprintf(stderr, "Formatting error\n"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 69 |         exit(1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 70 |     } | Closes a semantic action or C code block. |
| 71 |  | Blank line for readability and grouping of grammar/code sections. |
| 72 |     char* out = (char*)malloc((size_t)n + 1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 73 |     if (!out) { | Opens a semantic action or C code block. |
| 74 |         fprintf(stderr, "Out of memory\n"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 75 |         exit(1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 76 |     } | Closes a semantic action or C code block. |
| 77 |  | Blank line for readability and grouping of grammar/code sections. |
| 78 |     va_start(ap, fmt); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 79 |     vsnprintf(out, (size_t)n + 1, fmt, ap); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 80 |     va_end(ap); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 81 |     return out; | Returns computed value/token/type from helper code. |
| 82 | } | Closes a semantic action or C code block. |
| 83 |  | Blank line for readability and grouping of grammar/code sections. |
| 84 | static char* concat2(const char* a, const char* b) { | Static helper declaration or definition used by parser and semantic phase. |
| 85 |     size_t na = strlen(a); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 86 |     size_t nb = strlen(b); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 87 |     char* out = (char*)malloc(na + nb + 1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 88 |     if (!out) { | Opens a semantic action or C code block. |
| 89 |         fprintf(stderr, "Out of memory\n"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 90 |         exit(1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 91 |     } | Closes a semantic action or C code block. |
| 92 |     memcpy(out, a, na); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 93 |     memcpy(out + na, b, nb); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 94 |     out[na + nb] = '\0'; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 95 |     return out; | Returns computed value/token/type from helper code. |
| 96 | } | Closes a semantic action or C code block. |
| 97 |  | Blank line for readability and grouping of grammar/code sections. |
| 98 | static char* concat_space(const char* a, const char* b) { | Static helper declaration or definition used by parser and semantic phase. |
| 99 |     if (a[0] == '\0') { | Opens a semantic action or C code block. |
| 100 |         return xstrdup(b); | Returns computed value/token/type from helper code. |
| 101 |     } | Closes a semantic action or C code block. |
| 102 |   return str_printf("%s\t%s", a, b); | Returns computed value/token/type from helper code. |
| 103 | } | Closes a semantic action or C code block. |
| 104 |  | Blank line for readability and grouping of grammar/code sections. |
| 105 | static char* indent_code(const char* code) { | Static helper declaration or definition used by parser and semantic phase. |
| 106 |     size_t n = strlen(code); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 107 |     char* out = (char*)malloc(n * 2 + 64); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 108 |     if (!out) { | Opens a semantic action or C code block. |
| 109 |         fprintf(stderr, "Out of memory\n"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 110 |         exit(1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 111 |     } | Closes a semantic action or C code block. |
| 112 |  | Blank line for readability and grouping of grammar/code sections. |
| 113 |     size_t j = 0; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 114 |     int at_line_start = 1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 115 |     for (size_t i = 0; i < n; i++) { | Opens a semantic action or C code block. |
| 116 |         if (at_line_start && code[i] != '\n') { | Opens a semantic action or C code block. |
| 117 |             out[j++] = ' '; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 118 |             out[j++] = ' '; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 119 |             out[j++] = ' '; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 120 |             out[j++] = ' '; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 121 |             at_line_start = 0; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 122 |         } | Closes a semantic action or C code block. |
| 123 |         out[j++] = code[i]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 124 |         if (code[i] == '\n') { | Opens a semantic action or C code block. |
| 125 |             at_line_start = 1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 126 |         } | Closes a semantic action or C code block. |
| 127 |     } | Closes a semantic action or C code block. |
| 128 |     out[j] = '\0'; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 129 |     return out; | Returns computed value/token/type from helper code. |
| 130 | } | Closes a semantic action or C code block. |
| 131 |  | Blank line for readability and grouping of grammar/code sections. |
| 132 | static void semantic_error(const char* fmt, ...) { | Static helper declaration or definition used by parser and semantic phase. |
| 133 |   semantic_errors++; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 134 |   fprintf(stderr, "Semantic error (line %d): ", yylineno); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 135 |   va_list ap; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 136 |   va_start(ap, fmt); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 137 |   vfprintf(stderr, fmt, ap); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 138 |   va_end(ap); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 139 |   fprintf(stderr, "\n"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 140 | } | Closes a semantic action or C code block. |
| 141 |  | Blank line for readability and grouping of grammar/code sections. |
| 142 | static ValueType parse_type_name(const char* t) { | Static helper declaration or definition used by parser and semantic phase. |
| 143 |   if (strcmp(t, "int") == 0) return TY_INT; | Conditional semantic logic or helper control flow. |
| 144 |   if (strcmp(t, "double") == 0) return TY_DOUBLE; | Conditional semantic logic or helper control flow. |
| 145 |   if (strcmp(t, "char") == 0) return TY_CHAR; | Conditional semantic logic or helper control flow. |
| 146 |   if (strcmp(t, "void") == 0) return TY_VOID; | Conditional semantic logic or helper control flow. |
| 147 |   return TY_UNKNOWN; | Returns computed value/token/type from helper code. |
| 148 | } | Closes a semantic action or C code block. |
| 149 |  | Blank line for readability and grouping of grammar/code sections. |
| 150 | static void clear_symbols(void) { | Static helper declaration or definition used by parser and semantic phase. |
| 151 |   symbol_count = 0; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 152 | } | Closes a semantic action or C code block. |
| 153 |  | Blank line for readability and grouping of grammar/code sections. |
| 154 | static int symbol_index(const char* name) { | Static helper declaration or definition used by parser and semantic phase. |
| 155 |   for (int i = symbol_count - 1; i >= 0; i--) { | Opens a semantic action or C code block. |
| 156 |     if (strcmp(symbols[i].name, name) == 0) { | Opens a semantic action or C code block. |
| 157 |       return i; | Returns computed value/token/type from helper code. |
| 158 |     } | Closes a semantic action or C code block. |
| 159 |   } | Closes a semantic action or C code block. |
| 160 |   return -1; | Returns computed value/token/type from helper code. |
| 161 | } | Closes a semantic action or C code block. |
| 162 |  | Blank line for readability and grouping of grammar/code sections. |
| 163 | static ValueType symbol_type(const char* name) { | Static helper declaration or definition used by parser and semantic phase. |
| 164 |   int i = symbol_index(name); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 165 |   if (i < 0) return TY_UNKNOWN; | Conditional semantic logic or helper control flow. |
| 166 |   return symbols[i].type; | Returns computed value/token/type from helper code. |
| 167 | } | Closes a semantic action or C code block. |
| 168 |  | Blank line for readability and grouping of grammar/code sections. |
| 169 | static int declare_symbol(const char* name, ValueType type) { | Static helper declaration or definition used by parser and semantic phase. |
| 170 |   if (symbol_index(name) >= 0) { | Opens a semantic action or C code block. |
| 171 |     semantic_error("redeclaration of variable '%s'", name); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 172 |     return 0; | Returns computed value/token/type from helper code. |
| 173 |   } | Closes a semantic action or C code block. |
| 174 |   if (symbol_count >= 1024) { | Opens a semantic action or C code block. |
| 175 |     semantic_error("too many symbols"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 176 |     return 0; | Returns computed value/token/type from helper code. |
| 177 |   } | Closes a semantic action or C code block. |
| 178 |   strncpy(symbols[symbol_count].name, name, sizeof(symbols[symbol_count].name) - 1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 179 |   symbols[symbol_count].name[sizeof(symbols[symbol_count].name) - 1] = '\0'; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 180 |   symbols[symbol_count].type = type; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 181 |   symbol_count++; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 182 |   return 1; | Returns computed value/token/type from helper code. |
| 183 | } | Closes a semantic action or C code block. |
| 184 |  | Blank line for readability and grouping of grammar/code sections. |
| 185 | static int function_index(const char* name) { | Static helper declaration or definition used by parser and semantic phase. |
| 186 |   for (int i = 0; i < function_count; i++) { | Opens a semantic action or C code block. |
| 187 |     if (strcmp(functions[i].name, name) == 0) return i; | Conditional semantic logic or helper control flow. |
| 188 |   } | Closes a semantic action or C code block. |
| 189 |   return -1; | Returns computed value/token/type from helper code. |
| 190 | } | Closes a semantic action or C code block. |
| 191 |  | Blank line for readability and grouping of grammar/code sections. |
| 192 | static int function_exists(const char* name) { | Static helper declaration or definition used by parser and semantic phase. |
| 193 |   return function_index(name) >= 0; | Returns computed value/token/type from helper code. |
| 194 | } | Closes a semantic action or C code block. |
| 195 |  | Blank line for readability and grouping of grammar/code sections. |
| 196 | static void reset_pending_params(void) { | Static helper declaration or definition used by parser and semantic phase. |
| 197 |   pending_param_count = 0; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 198 | } | Closes a semantic action or C code block. |
| 199 |  | Blank line for readability and grouping of grammar/code sections. |
| 200 | static void push_pending_param(const char* type_name, const char* param_name) { | Static helper declaration or definition used by parser and semantic phase. |
| 201 |   if (pending_param_count >= 64) { | Opens a semantic action or C code block. |
| 202 |     semantic_error("too many function parameters"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 203 |     return; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 204 |   } | Closes a semantic action or C code block. |
| 205 |   pending_param_types[pending_param_count] = parse_type_name(type_name); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 206 |   strncpy(pending_param_names[pending_param_count], param_name, sizeof(pending_param_names[pending_param_count]) - 1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 207 |   pending_param_names[pending_param_count][sizeof(pending_param_names[pending_param_count]) - 1] = '\0'; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 208 |   pending_param_count++; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 209 | } | Closes a semantic action or C code block. |
| 210 |  | Blank line for readability and grouping of grammar/code sections. |
| 211 | static void reset_pending_call_args(void) { | Static helper declaration or definition used by parser and semantic phase. |
| 212 |   pending_call_arg_count = 0; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 213 | } | Closes a semantic action or C code block. |
| 214 |  | Blank line for readability and grouping of grammar/code sections. |
| 215 | static void push_pending_call_arg(ValueType t) { | Static helper declaration or definition used by parser and semantic phase. |
| 216 |   if (pending_call_arg_count >= 64) { | Opens a semantic action or C code block. |
| 217 |     semantic_error("too many function call arguments"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 218 |     return; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 219 |   } | Closes a semantic action or C code block. |
| 220 |   pending_call_arg_types[pending_call_arg_count++] = t; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 221 | } | Closes a semantic action or C code block. |
| 222 |  | Blank line for readability and grouping of grammar/code sections. |
| 223 | static void register_function(const char* name, ValueType ret_type) { | Static helper declaration or definition used by parser and semantic phase. |
| 224 |   if (function_exists(name)) { | Opens a semantic action or C code block. |
| 225 |     semantic_error("redefinition of function '%s'", name); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 226 |     return; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 227 |   } | Closes a semantic action or C code block. |
| 228 |   if (function_count >= 256) { | Opens a semantic action or C code block. |
| 229 |     semantic_error("too many functions"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 230 |     return; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 231 |   } | Closes a semantic action or C code block. |
| 232 |   strncpy(functions[function_count].name, name, sizeof(functions[function_count].name) - 1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 233 |   functions[function_count].name[sizeof(functions[function_count].name) - 1] = '\0'; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 234 |   functions[function_count].ret_type = ret_type; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 235 |   functions[function_count].arg_count = pending_param_count; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 236 |   for (int i = 0; i < pending_param_count; i++) { | Opens a semantic action or C code block. |
| 237 |     functions[function_count].arg_types[i] = pending_param_types[i]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 238 |   } | Closes a semantic action or C code block. |
| 239 |   function_count++; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 240 | } | Closes a semantic action or C code block. |
| 241 |  | Blank line for readability and grouping of grammar/code sections. |
| 242 | static void begin_function(const char* type_name, const char* name) { | Static helper declaration or definition used by parser and semantic phase. |
| 243 |   current_function_type = parse_type_name(type_name); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 244 |   clear_symbols(); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 245 |   register_function(name, current_function_type); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 246 |   for (int i = 0; i < pending_param_count; i++) { | Opens a semantic action or C code block. |
| 247 |     declare_symbol(pending_param_names[i], pending_param_types[i]); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 248 |   } | Closes a semantic action or C code block. |
| 249 | } | Closes a semantic action or C code block. |
| 250 |  | Blank line for readability and grouping of grammar/code sections. |
| 251 | static int is_number_token(const char* tok) { | Static helper declaration or definition used by parser and semantic phase. |
| 252 |   if (!tok \|\| !*tok) return 0; | Conditional semantic logic or helper control flow. |
| 253 |   int has_dot = 0; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 254 |   int i = 0; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 255 |   if (tok[0] == '-') i = 1; | Conditional semantic logic or helper control flow. |
| 256 |   for (; tok[i]; i++) { | Opens a semantic action or C code block. |
| 257 |     if (tok[i] == '.') { | Opens a semantic action or C code block. |
| 258 |       if (has_dot) return 0; | Conditional semantic logic or helper control flow. |
| 259 |       has_dot = 1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 260 |       continue; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 261 |     } | Closes a semantic action or C code block. |
| 262 |     if (!isdigit((unsigned char)tok[i])) return 0; | Conditional semantic logic or helper control flow. |
| 263 |   } | Closes a semantic action or C code block. |
| 264 |   return i > 0; | Returns computed value/token/type from helper code. |
| 265 | } | Closes a semantic action or C code block. |
| 266 |  | Blank line for readability and grouping of grammar/code sections. |
| 267 | static int is_identifier_token(const char* tok) { | Static helper declaration or definition used by parser and semantic phase. |
| 268 |   if (!tok \|\| !*tok) return 0; | Conditional semantic logic or helper control flow. |
| 269 |   if (!(isalpha((unsigned char)tok[0]) \|\| tok[0] == '_')) return 0; | Conditional semantic logic or helper control flow. |
| 270 |   for (int i = 1; tok[i]; i++) { | Opens a semantic action or C code block. |
| 271 |     if (!(isalnum((unsigned char)tok[i]) \|\| tok[i] == '_')) return 0; | Conditional semantic logic or helper control flow. |
| 272 |   } | Closes a semantic action or C code block. |
| 273 |   return 1; | Returns computed value/token/type from helper code. |
| 274 | } | Closes a semantic action or C code block. |
| 275 |  | Blank line for readability and grouping of grammar/code sections. |
| 276 | static int is_operator(const char* tok) { | Static helper declaration or definition used by parser and semantic phase. |
| 277 |     return strcmp(tok, "+") == 0 \|\| strcmp(tok, "-") == 0 \|\| strcmp(tok, "*") == 0 \|\| | Returns computed value/token/type from helper code. |
| 278 |        strcmp(tok, "/") == 0 \|\| strcmp(tok, "%") == 0 \|\| strcmp(tok, ">") == 0 \|\| strcmp(tok, "<") == 0 \|\| | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 279 |            strcmp(tok, ">=") == 0 \|\| strcmp(tok, "<=") == 0 \|\| strcmp(tok, "==") == 0 \|\| | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 280 |        strcmp(tok, "!=") == 0 \|\| strcmp(tok, "&&") == 0 \|\| strcmp(tok, "\|\|") == 0 \|\| | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 281 |        strcmp(tok, "!") == 0; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 282 | } | Closes a semantic action or C code block. |
| 283 |  | Blank line for readability and grouping of grammar/code sections. |
| 284 | static int is_call_token(const char* tok) { | Static helper declaration or definition used by parser and semantic phase. |
| 285 |   const char* lp = strchr(tok, '('); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 286 |   size_t n = strlen(tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 287 |   return lp != NULL && n > 2 && tok[n - 1] == ')'; | Returns computed value/token/type from helper code. |
| 288 | } | Closes a semantic action or C code block. |
| 289 |  | Blank line for readability and grouping of grammar/code sections. |
| 290 | static ValueType call_return_type_from_token(const char* tok) { | Static helper declaration or definition used by parser and semantic phase. |
| 291 |   const char* lp = strchr(tok, '('); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 292 |   if (!lp) return TY_UNKNOWN; | Conditional semantic logic or helper control flow. |
| 293 |   size_t len = (size_t)(lp - tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 294 |   if (len == 0 \|\| len >= 64) return TY_UNKNOWN; | Conditional semantic logic or helper control flow. |
| 295 |   char name[64]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 296 |   memcpy(name, tok, len); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 297 |   name[len] = '\0'; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 298 |   int fi = function_index(name); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 299 |   if (fi < 0) { | Opens a semantic action or C code block. |
| 300 |     semantic_error("call to undefined function '%s'", name); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 301 |     return TY_UNKNOWN; | Returns computed value/token/type from helper code. |
| 302 |   } | Closes a semantic action or C code block. |
| 303 |   return functions[fi].ret_type; | Returns computed value/token/type from helper code. |
| 304 | } | Closes a semantic action or C code block. |
| 305 |  | Blank line for readability and grouping of grammar/code sections. |
| 306 | static int op_arity(const char* tok) { | Static helper declaration or definition used by parser and semantic phase. |
| 307 |   if (strcmp(tok, "!") == 0) return 1; | Conditional semantic logic or helper control flow. |
| 308 |   return 2; | Returns computed value/token/type from helper code. |
| 309 | } | Closes a semantic action or C code block. |
| 310 |  | Blank line for readability and grouping of grammar/code sections. |
| 311 | static int is_numeric_type(ValueType t) { | Static helper declaration or definition used by parser and semantic phase. |
| 312 |   return t == TY_INT \|\| t == TY_DOUBLE \|\| t == TY_CHAR; | Returns computed value/token/type from helper code. |
| 313 | } | Closes a semantic action or C code block. |
| 314 |  | Blank line for readability and grouping of grammar/code sections. |
| 315 | static int can_assign(ValueType lhs, ValueType rhs) { | Static helper declaration or definition used by parser and semantic phase. |
| 316 |   if (lhs == TY_UNKNOWN \|\| rhs == TY_UNKNOWN) return 0; | Conditional semantic logic or helper control flow. |
| 317 |   if (lhs == TY_VOID \|\| rhs == TY_VOID) return 0; | Conditional semantic logic or helper control flow. |
| 318 |   if (lhs == rhs) return 1; | Conditional semantic logic or helper control flow. |
| 319 |   return is_numeric_type(lhs) && is_numeric_type(rhs); | Returns computed value/token/type from helper code. |
| 320 | } | Closes a semantic action or C code block. |
| 321 |  | Blank line for readability and grouping of grammar/code sections. |
| 322 | static ValueType number_type(const char* tok) { | Static helper declaration or definition used by parser and semantic phase. |
| 323 |   return strchr(tok, '.') ? TY_DOUBLE : TY_INT; | Returns computed value/token/type from helper code. |
| 324 | } | Closes a semantic action or C code block. |
| 325 |  | Blank line for readability and grouping of grammar/code sections. |
| 326 | static ValueType infer_postfix_type(const char* expr) { | Static helper declaration or definition used by parser and semantic phase. |
| 327 |   char* copy = xstrdup(expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 328 |   ValueType stack[512]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 329 |   int top = 0; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 330 |  | Blank line for readability and grouping of grammar/code sections. |
| 331 |   char* tok = strtok(copy, "\t"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 332 |   while (tok) { | Opens a semantic action or C code block. |
| 333 |     if (!is_operator(tok)) { | Opens a semantic action or C code block. |
| 334 |       if (is_number_token(tok)) { | Opens a semantic action or C code block. |
| 335 |         stack[top++] = number_type(tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 336 |       } else if (tok[0] == '"') { | Opens a semantic action or C code block. |
| 337 |         stack[top++] = TY_CHAR; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 338 |       } else if (is_call_token(tok)) { | Opens a semantic action or C code block. |
| 339 |         ValueType rt = call_return_type_from_token(tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 340 |         if (rt == TY_UNKNOWN) { | Opens a semantic action or C code block. |
| 341 |           free(copy); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 342 |           return TY_UNKNOWN; | Returns computed value/token/type from helper code. |
| 343 |         } | Closes a semantic action or C code block. |
| 344 |         stack[top++] = rt; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 345 |       } else if (is_identifier_token(tok)) { | Opens a semantic action or C code block. |
| 346 |         ValueType t = symbol_type(tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 347 |         if (t == TY_UNKNOWN) { | Opens a semantic action or C code block. |
| 348 |           semantic_error("use of undeclared variable '%s'", tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 349 |           free(copy); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 350 |           return TY_UNKNOWN; | Returns computed value/token/type from helper code. |
| 351 |         } | Closes a semantic action or C code block. |
| 352 |         stack[top++] = t; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 353 |       } else { | Opens a semantic action or C code block. |
| 354 |         semantic_error("invalid token in expression '%s'", tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 355 |         free(copy); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 356 |         return TY_UNKNOWN; | Returns computed value/token/type from helper code. |
| 357 |       } | Closes a semantic action or C code block. |
| 358 |       tok = strtok(NULL, "\t"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 359 |       continue; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 360 |     } | Closes a semantic action or C code block. |
| 361 |  | Blank line for readability and grouping of grammar/code sections. |
| 362 |     int arity = op_arity(tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 363 |     if (top < arity) { | Opens a semantic action or C code block. |
| 364 |       semantic_error("invalid postfix expression '%s'", expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 365 |       free(copy); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 366 |       return TY_UNKNOWN; | Returns computed value/token/type from helper code. |
| 367 |     } | Closes a semantic action or C code block. |
| 368 |  | Blank line for readability and grouping of grammar/code sections. |
| 369 |     if (arity == 1) { | Opens a semantic action or C code block. |
| 370 |       ValueType a = stack[--top]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 371 |       if (!is_numeric_type(a)) { | Opens a semantic action or C code block. |
| 372 |         semantic_error("operator '%s' requires numeric operand", tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 373 |         free(copy); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 374 |         return TY_UNKNOWN; | Returns computed value/token/type from helper code. |
| 375 |       } | Closes a semantic action or C code block. |
| 376 |       stack[top++] = TY_INT; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 377 |       tok = strtok(NULL, "\t"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 378 |       continue; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 379 |     } | Closes a semantic action or C code block. |
| 380 |  | Blank line for readability and grouping of grammar/code sections. |
| 381 |     ValueType rhs = stack[--top]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 382 |     ValueType lhs = stack[--top]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 383 |     if (!is_numeric_type(lhs) \|\| !is_numeric_type(rhs)) { | Opens a semantic action or C code block. |
| 384 |       semantic_error("operator '%s' requires numeric operands", tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 385 |       free(copy); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 386 |       return TY_UNKNOWN; | Returns computed value/token/type from helper code. |
| 387 |     } | Closes a semantic action or C code block. |
| 388 |  | Blank line for readability and grouping of grammar/code sections. |
| 389 |     if (strcmp(tok, "%") == 0) { | Opens a semantic action or C code block. |
| 390 |       if (lhs == TY_DOUBLE \|\| rhs == TY_DOUBLE) { | Opens a semantic action or C code block. |
| 391 |         semantic_error("operator '%%' does not support double operands"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 392 |         free(copy); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 393 |         return TY_UNKNOWN; | Returns computed value/token/type from helper code. |
| 394 |       } | Closes a semantic action or C code block. |
| 395 |       stack[top++] = TY_INT; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 396 |     } else if (strcmp(tok, ">") == 0 \|\| strcmp(tok, "<") == 0 \|\| strcmp(tok, ">=") == 0 \|\| | Closes a semantic action or C code block. |
| 397 |            strcmp(tok, "<=") == 0 \|\| strcmp(tok, "==") == 0 \|\| strcmp(tok, "!=") == 0 \|\| | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 398 |            strcmp(tok, "&&") == 0 \|\| strcmp(tok, "\|\|") == 0) { | Opens a semantic action or C code block. |
| 399 |       stack[top++] = TY_INT; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 400 |     } else { | Opens a semantic action or C code block. |
| 401 |       stack[top++] = (lhs == TY_DOUBLE \|\| rhs == TY_DOUBLE) ? TY_DOUBLE : TY_INT; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 402 |     } | Closes a semantic action or C code block. |
| 403 |  | Blank line for readability and grouping of grammar/code sections. |
| 404 |     tok = strtok(NULL, "\t"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 405 |   } | Closes a semantic action or C code block. |
| 406 |  | Blank line for readability and grouping of grammar/code sections. |
| 407 |   if (top != 1) { | Opens a semantic action or C code block. |
| 408 |     semantic_error("invalid postfix expression '%s'", expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 409 |     free(copy); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 410 |     return TY_UNKNOWN; | Returns computed value/token/type from helper code. |
| 411 |   } | Closes a semantic action or C code block. |
| 412 |  | Blank line for readability and grouping of grammar/code sections. |
| 413 |   ValueType out = stack[0]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 414 |   free(copy); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 415 |   return out; | Returns computed value/token/type from helper code. |
| 416 | } | Closes a semantic action or C code block. |
| 417 |  | Blank line for readability and grouping of grammar/code sections. |
| 418 | static char* postfix_to_c(const char* expr) { | Static helper declaration or definition used by parser and semantic phase. |
| 419 |     char* copy = xstrdup(expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 420 |     char* stack[512]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 421 |     int top = 0; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 422 |  | Blank line for readability and grouping of grammar/code sections. |
| 423 |   char* tok = strtok(copy, "\t"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 424 |   while (tok) { | Opens a semantic action or C code block. |
| 425 |         if (!is_operator(tok)) { | Opens a semantic action or C code block. |
| 426 |             stack[top++] = xstrdup(tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 427 |       tok = strtok(NULL, "\t"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 428 |       continue; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 429 |         } | Closes a semantic action or C code block. |
| 430 |  | Blank line for readability and grouping of grammar/code sections. |
| 431 |         int arity = op_arity(tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 432 |         if (top < arity) { | Opens a semantic action or C code block. |
| 433 |             fprintf(stderr, "Invalid postfix expression: %s\n", expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 434 |             free(copy); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 435 |             return xstrdup("0"); | Returns computed value/token/type from helper code. |
| 436 |         } | Closes a semantic action or C code block. |
| 437 |  | Blank line for readability and grouping of grammar/code sections. |
| 438 |         if (arity == 1) { | Opens a semantic action or C code block. |
| 439 |           char* a = stack[--top]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 440 |           char* merged = str_printf("(%s%s)", tok, a); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 441 |           stack[top++] = merged; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 442 |           free(a); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 443 |           tok = strtok(NULL, "\t"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 444 |           continue; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 445 |         } | Closes a semantic action or C code block. |
| 446 |  | Blank line for readability and grouping of grammar/code sections. |
| 447 |         char* rhs = stack[--top]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 448 |         char* lhs = stack[--top]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 449 |         char* merged = str_printf("(%s %s %s)", lhs, tok, rhs); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 450 |         stack[top++] = merged; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 451 |         free(lhs); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 452 |         free(rhs); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 453 |         tok = strtok(NULL, "\t"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 454 |     } | Closes a semantic action or C code block. |
| 455 |  | Blank line for readability and grouping of grammar/code sections. |
| 456 |     char* out = (top == 1) ? xstrdup(stack[0]) : xstrdup("0"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 457 |     for (int i = 0; i < top; i++) { | Opens a semantic action or C code block. |
| 458 |         free(stack[i]); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 459 |     } | Closes a semantic action or C code block. |
| 460 |     free(copy); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 461 |     return out; | Returns computed value/token/type from helper code. |
| 462 | } | Closes a semantic action or C code block. |
| 463 |  | Blank line for readability and grouping of grammar/code sections. |
| 464 | int yylex(void); | Forward declaration of lexer function invoked by parser. |
| 465 | void yyerror(const char* s); | Parser error function declaration/definition. |
| 466 | const char* get_generated_program(void); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 467 | int has_semantic_errors(void); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 468 | %} | End of Bison C prologue block. |
| 469 |  | Blank line for readability and grouping of grammar/code sections. |
| 470 | %union { | Defines YYSTYPE union members for semantic values. |
| 471 |     char* str; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 472 | } | Closes a semantic action or C code block. |
| 473 |  | Blank line for readability and grouping of grammar/code sections. |
| 474 | %token <str> IDENT NUMBER STRING TYPE | Declares lexer tokens consumed by Bison grammar. |
| 475 | %token IF ELIF ELSE WHILE FOR RETURN PRINT SATR BREAK CONTINUE DEF | Declares lexer tokens consumed by Bison grammar. |
| 476 | %token NEWLINE INDENT DEDENT COLON ASSIGN SEMI | Declares lexer tokens consumed by Bison grammar. |
| 477 | %token PLUS MINUS STAR SLASH MOD GT LT GE LE EQ NE AND OR NOT | Declares lexer tokens consumed by Bison grammar. |
| 478 | %token LPAREN RPAREN COMMA | Declares lexer tokens consumed by Bison grammar. |
| 479 |  | Blank line for readability and grouping of grammar/code sections. |
| 480 | %type <str> program top_list top_item func_def func_open | Declares semantic value types for nonterminals. |
| 481 | %type <str> stmt_list stmt simple_stmt opt_expr postfix_expr postfix_item for_init for_cond for_update | Declares semantic value types for nonterminals. |
| 482 | %type <str> opt_elif opt_else nl | Declares semantic value types for nonterminals. |
| 483 | %type <str> param_list_opt param_list param_decl arg_list_opt arg_list arg_item | Declares semantic value types for nonterminals. |
| 484 | %type <str> call_expr | Declares semantic value types for nonterminals. |
| 485 |  | Blank line for readability and grouping of grammar/code sections. |
| 486 | %start program | Declares grammar entry nonterminal. |
| 487 |  | Blank line for readability and grouping of grammar/code sections. |
| 488 | %% | Bison section delimiter between declarations, grammar, and user code. |
| 489 |  | Blank line for readability and grouping of grammar/code sections. |
| 490 | program | Grammar nonterminal label line introducing productions. |
| 491 |     : top_list { | Beginning of production alternatives for current nonterminal. |
| 492 |         generated_program = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 493 |       } | Closes a semantic action or C code block. |
| 494 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 495 |  | Blank line for readability and grouping of grammar/code sections. |
| 496 | top_list | Grammar nonterminal label line introducing productions. |
| 497 |     : top_item { | Beginning of production alternatives for current nonterminal. |
| 498 |         $$ = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 499 |       } | Closes a semantic action or C code block. |
| 500 |     \| top_list top_item { | Alternative grammar production branch. |
| 501 |         char* merged = concat2($1, $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 502 |         free($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 503 |         free($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 504 |         $$ = merged; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 505 |       } | Closes a semantic action or C code block. |
| 506 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 507 |  | Blank line for readability and grouping of grammar/code sections. |
| 508 | top_item | Grammar nonterminal label line introducing productions. |
| 509 |     : func_def { | Beginning of production alternatives for current nonterminal. |
| 510 |         $$ = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 511 |       } | Closes a semantic action or C code block. |
| 512 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 513 |  | Blank line for readability and grouping of grammar/code sections. |
| 514 | func_def | Grammar nonterminal label line introducing productions. |
| 515 |     : func_open INDENT stmt_list DEDENT { | Beginning of production alternatives for current nonterminal. |
| 516 |         char* indented = indent_code($3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 517 |         $$ = str_printf("%s {\n%s}\n\n", $1, indented); | Opens a semantic action or C code block. |
| 518 |         free(indented); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 519 |       } | Closes a semantic action or C code block. |
| 520 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 521 |  | Blank line for readability and grouping of grammar/code sections. |
| 522 | func_open | Grammar nonterminal label line introducing productions. |
| 523 |     : DEF TYPE IDENT LPAREN param_list_opt RPAREN COLON nl { | Beginning of production alternatives for current nonterminal. |
| 524 |         begin_function($2, $3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 525 |         $$ = str_printf("%s %s(%s)", $2, $3, $5); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 526 |       } | Closes a semantic action or C code block. |
| 527 |     \| TYPE IDENT LPAREN param_list_opt RPAREN COLON nl { | Alternative grammar production branch. |
| 528 |         begin_function($1, $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 529 |         $$ = str_printf("%s %s(%s)", $1, $2, $4); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 530 |       } | Closes a semantic action or C code block. |
| 531 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 532 |  | Blank line for readability and grouping of grammar/code sections. |
| 533 | param_list_opt | Grammar nonterminal label line introducing productions. |
| 534 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 535 |         reset_pending_params(); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 536 |         $$ = xstrdup(""); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 537 |       } | Closes a semantic action or C code block. |
| 538 |     \| param_reset param_list { | Alternative grammar production branch. |
| 539 |         $$ = $2; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 540 |       } | Closes a semantic action or C code block. |
| 541 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 542 |  | Blank line for readability and grouping of grammar/code sections. |
| 543 | param_reset | Grammar nonterminal label line introducing productions. |
| 544 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 545 |         reset_pending_params(); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 546 |       } | Closes a semantic action or C code block. |
| 547 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 548 |  | Blank line for readability and grouping of grammar/code sections. |
| 549 | param_list | Grammar nonterminal label line introducing productions. |
| 550 |     : param_decl { | Beginning of production alternatives for current nonterminal. |
| 551 |         $$ = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 552 |       } | Closes a semantic action or C code block. |
| 553 |     \| param_list COMMA param_decl { | Alternative grammar production branch. |
| 554 |         char* merged = str_printf("%s, %s", $1, $3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 555 |         free($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 556 |         free($3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 557 |         $$ = merged; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 558 |       } | Closes a semantic action or C code block. |
| 559 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 560 |  | Blank line for readability and grouping of grammar/code sections. |
| 561 | param_decl | Grammar nonterminal label line introducing productions. |
| 562 |     : TYPE IDENT { | Beginning of production alternatives for current nonterminal. |
| 563 |         if (parse_type_name($1) == TY_VOID) { | Opens a semantic action or C code block. |
| 564 |             semantic_error("parameter '%s' cannot be of type void", $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 565 |         } | Closes a semantic action or C code block. |
| 566 |         push_pending_param($1, $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 567 |         $$ = str_printf("%s %s", $1, $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 568 |       } | Closes a semantic action or C code block. |
| 569 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 570 |  | Blank line for readability and grouping of grammar/code sections. |
| 571 | stmt_list | Grammar nonterminal label line introducing productions. |
| 572 |     : stmt { | Beginning of production alternatives for current nonterminal. |
| 573 |         $$ = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 574 |       } | Closes a semantic action or C code block. |
| 575 |     \| stmt_list stmt { | Alternative grammar production branch. |
| 576 |         char* merged = concat2($1, $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 577 |         free($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 578 |         free($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 579 |         $$ = merged; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 580 |       } | Closes a semantic action or C code block. |
| 581 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 582 |  | Blank line for readability and grouping of grammar/code sections. |
| 583 | stmt | Grammar nonterminal label line introducing productions. |
| 584 |     : simple_stmt { | Beginning of production alternatives for current nonterminal. |
| 585 |         $$ = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 586 |       } | Closes a semantic action or C code block. |
| 587 |     \| IF postfix_expr COLON nl INDENT stmt_list DEDENT opt_elif opt_else { | Alternative grammar production branch. |
| 588 |         infer_postfix_type($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 589 |         char* cond = postfix_to_c($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 590 |         char* if_body = indent_code($6); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 591 |         char* core = str_printf("if (%s) {\n%s}\n", cond, if_body); | Opens a semantic action or C code block. |
| 592 |  | Blank line for readability and grouping of grammar/code sections. |
| 593 |         char* with_elif = concat2(core, $8); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 594 |         char* full = concat2(with_elif, $9); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 595 |  | Blank line for readability and grouping of grammar/code sections. |
| 596 |         free(cond); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 597 |         free(if_body); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 598 |         free(core); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 599 |         free(with_elif); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 600 |         $$ = full; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 601 |       } | Closes a semantic action or C code block. |
| 602 |     \| WHILE postfix_expr COLON nl INDENT loop_enter stmt_list DEDENT loop_exit { | Alternative grammar production branch. |
| 603 |         char* cond = postfix_to_c($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 604 |         infer_postfix_type($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 605 |         char* body = indent_code($7); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 606 |         $$ = str_printf("while (%s) {\n%s}\n", cond, body); | Opens a semantic action or C code block. |
| 607 |         free(cond); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 608 |         free(body); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 609 |       } | Closes a semantic action or C code block. |
| 610 |     \| FOR for_init SEMI for_cond SEMI for_update COLON nl INDENT loop_enter stmt_list DEDENT loop_exit { | Alternative grammar production branch. |
| 611 |         char* body = indent_code($11); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 612 |         $$ = str_printf("for (%s; %s; %s) {\n%s}\n", $2, $4, $6, body); | Opens a semantic action or C code block. |
| 613 |         free(body); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 614 |       } | Closes a semantic action or C code block. |
| 615 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 616 |  | Blank line for readability and grouping of grammar/code sections. |
| 617 | loop_enter | Grammar nonterminal label line introducing productions. |
| 618 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 619 |         loop_depth++; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 620 |       } | Closes a semantic action or C code block. |
| 621 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 622 |  | Blank line for readability and grouping of grammar/code sections. |
| 623 | loop_exit | Grammar nonterminal label line introducing productions. |
| 624 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 625 |         loop_depth--; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 626 |       } | Closes a semantic action or C code block. |
| 627 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 628 |  | Blank line for readability and grouping of grammar/code sections. |
| 629 | for_init | Grammar nonterminal label line introducing productions. |
| 630 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 631 |         $$ = xstrdup(""); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 632 |       } | Closes a semantic action or C code block. |
| 633 |     \| TYPE IDENT { | Alternative grammar production branch. |
| 634 |         ValueType t = parse_type_name($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 635 |         declare_symbol($2, t); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 636 |         $$ = str_printf("%s %s", $1, $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 637 |       } | Closes a semantic action or C code block. |
| 638 |     \| TYPE IDENT postfix_expr ASSIGN { | Alternative grammar production branch. |
| 639 |         ValueType t = parse_type_name($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 640 |         declare_symbol($2, t); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 641 |         ValueType rhs = infer_postfix_type($3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 642 |         if (rhs != TY_UNKNOWN && !can_assign(t, rhs)) { | Opens a semantic action or C code block. |
| 643 |             semantic_error("cannot assign expression to variable '%s'", $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 644 |         } | Closes a semantic action or C code block. |
| 645 |         char* expr = postfix_to_c($3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 646 |         $$ = str_printf("%s %s = %s", $1, $2, expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 647 |         free(expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 648 |       } | Closes a semantic action or C code block. |
| 649 |     \| IDENT postfix_expr ASSIGN { | Alternative grammar production branch. |
| 650 |         ValueType lhs = symbol_type($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 651 |         if (lhs == TY_UNKNOWN) { | Opens a semantic action or C code block. |
| 652 |             semantic_error("assignment to undeclared variable '%s'", $1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 653 |         } | Closes a semantic action or C code block. |
| 654 |         ValueType rhs = infer_postfix_type($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 655 |         if (lhs != TY_UNKNOWN && rhs != TY_UNKNOWN && !can_assign(lhs, rhs)) { | Opens a semantic action or C code block. |
| 656 |             semantic_error("type mismatch in assignment to '%s'", $1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 657 |         } | Closes a semantic action or C code block. |
| 658 |         char* expr = postfix_to_c($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 659 |         $$ = str_printf("%s = %s", $1, expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 660 |         free(expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 661 |       } | Closes a semantic action or C code block. |
| 662 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 663 |  | Blank line for readability and grouping of grammar/code sections. |
| 664 | for_cond | Grammar nonterminal label line introducing productions. |
| 665 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 666 |         $$ = xstrdup("1"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 667 |       } | Closes a semantic action or C code block. |
| 668 |     \| postfix_expr { | Alternative grammar production branch. |
| 669 |         infer_postfix_type($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 670 |         $$ = postfix_to_c($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 671 |       } | Closes a semantic action or C code block. |
| 672 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 673 |  | Blank line for readability and grouping of grammar/code sections. |
| 674 | for_update | Grammar nonterminal label line introducing productions. |
| 675 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 676 |         $$ = xstrdup(""); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 677 |       } | Closes a semantic action or C code block. |
| 678 |     \| IDENT postfix_expr ASSIGN { | Alternative grammar production branch. |
| 679 |         ValueType lhs = symbol_type($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 680 |         if (lhs == TY_UNKNOWN) { | Opens a semantic action or C code block. |
| 681 |             semantic_error("assignment to undeclared variable '%s'", $1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 682 |         } | Closes a semantic action or C code block. |
| 683 |         ValueType rhs = infer_postfix_type($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 684 |         if (lhs != TY_UNKNOWN && rhs != TY_UNKNOWN && !can_assign(lhs, rhs)) { | Opens a semantic action or C code block. |
| 685 |             semantic_error("type mismatch in assignment to '%s'", $1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 686 |         } | Closes a semantic action or C code block. |
| 687 |         char* expr = postfix_to_c($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 688 |         $$ = str_printf("%s = %s", $1, expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 689 |         free(expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 690 |       } | Closes a semantic action or C code block. |
| 691 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 692 |  | Blank line for readability and grouping of grammar/code sections. |
| 693 | opt_elif | Grammar nonterminal label line introducing productions. |
| 694 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 695 |         $$ = xstrdup(""); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 696 |       } | Closes a semantic action or C code block. |
| 697 |     \| ELIF postfix_expr COLON nl INDENT stmt_list DEDENT opt_elif { | Alternative grammar production branch. |
| 698 |       infer_postfix_type($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 699 |         char* cond = postfix_to_c($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 700 |         char* body = indent_code($6); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 701 |         char* current = str_printf("else if (%s) {\n%s}\n", cond, body); | Opens a semantic action or C code block. |
| 702 |         char* full = concat2(current, $8); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 703 |         free(cond); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 704 |         free(body); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 705 |         free(current); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 706 |         $$ = full; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 707 |       } | Closes a semantic action or C code block. |
| 708 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 709 |  | Blank line for readability and grouping of grammar/code sections. |
| 710 | opt_else | Grammar nonterminal label line introducing productions. |
| 711 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 712 |         $$ = xstrdup(""); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 713 |       } | Closes a semantic action or C code block. |
| 714 |     \| ELSE COLON nl INDENT stmt_list DEDENT { | Alternative grammar production branch. |
| 715 |         char* body = indent_code($5); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 716 |         $$ = str_printf("else {\n%s}\n", body); | Opens a semantic action or C code block. |
| 717 |         free(body); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 718 |       } | Closes a semantic action or C code block. |
| 719 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 720 |  | Blank line for readability and grouping of grammar/code sections. |
| 721 | simple_stmt | Grammar nonterminal label line introducing productions. |
| 722 |     : TYPE IDENT nl { | Beginning of production alternatives for current nonterminal. |
| 723 |         ValueType t = parse_type_name($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 724 |         declare_symbol($2, t); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 725 |         $$ = str_printf("%s %s;\n", $1, $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 726 |       } | Closes a semantic action or C code block. |
| 727 |     \| TYPE IDENT postfix_expr ASSIGN nl { | Alternative grammar production branch. |
| 728 |         ValueType t = parse_type_name($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 729 |         declare_symbol($2, t); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 730 |         ValueType rhs = infer_postfix_type($3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 731 |         if (rhs != TY_UNKNOWN && !can_assign(t, rhs)) { | Opens a semantic action or C code block. |
| 732 |             semantic_error("cannot assign expression to variable '%s'", $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 733 |         } | Closes a semantic action or C code block. |
| 734 |         char* expr = postfix_to_c($3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 735 |         $$ = str_printf("%s %s = %s;\n", $1, $2, expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 736 |         free(expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 737 |       } | Closes a semantic action or C code block. |
| 738 |     \| IDENT postfix_expr ASSIGN nl { | Alternative grammar production branch. |
| 739 |         ValueType lhs = symbol_type($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 740 |         if (lhs == TY_UNKNOWN) { | Opens a semantic action or C code block. |
| 741 |             semantic_error("assignment to undeclared variable '%s'", $1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 742 |         } | Closes a semantic action or C code block. |
| 743 |         ValueType rhs = infer_postfix_type($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 744 |         if (lhs != TY_UNKNOWN && rhs != TY_UNKNOWN && !can_assign(lhs, rhs)) { | Opens a semantic action or C code block. |
| 745 |             semantic_error("type mismatch in assignment to '%s'", $1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 746 |         } | Closes a semantic action or C code block. |
| 747 |         char* expr = postfix_to_c($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 748 |         $$ = str_printf("%s = %s;\n", $1, expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 749 |         free(expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 750 |       } | Closes a semantic action or C code block. |
| 751 |     \| RETURN opt_expr nl { | Alternative grammar production branch. |
| 752 |         if ($2[0] == '\0') { | Opens a semantic action or C code block. |
| 753 |             if (current_function_type != TY_VOID) { | Opens a semantic action or C code block. |
| 754 |                 semantic_error("non-void function must return a value"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 755 |             } | Closes a semantic action or C code block. |
| 756 |             $$ = xstrdup("return;\n"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 757 |         } else { | Opens a semantic action or C code block. |
| 758 |             ValueType r = infer_postfix_type($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 759 |             if (current_function_type == TY_VOID) { | Opens a semantic action or C code block. |
| 760 |                 semantic_error("void function cannot return a value"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 761 |             } else if (r != TY_UNKNOWN && !can_assign(current_function_type, r)) { | Opens a semantic action or C code block. |
| 762 |                 semantic_error("return type mismatch"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 763 |             } | Closes a semantic action or C code block. |
| 764 |             char* expr = postfix_to_c($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 765 |             $$ = str_printf("return %s;\n", expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 766 |             free(expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 767 |         } | Closes a semantic action or C code block. |
| 768 |       } | Closes a semantic action or C code block. |
| 769 |     \| BREAK nl { | Alternative grammar production branch. |
| 770 |         if (loop_depth <= 0) { | Opens a semantic action or C code block. |
| 771 |             semantic_error("'qif'/'break' used outside loop"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 772 |         } | Closes a semantic action or C code block. |
| 773 |         $$ = xstrdup("break;\n"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 774 |       } | Closes a semantic action or C code block. |
| 775 |     \| CONTINUE nl { | Alternative grammar production branch. |
| 776 |         if (loop_depth <= 0) { | Opens a semantic action or C code block. |
| 777 |             semantic_error("'wasil'/'continue' used outside loop"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 778 |         } | Closes a semantic action or C code block. |
| 779 |         $$ = xstrdup("continue;\n"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 780 |       } | Closes a semantic action or C code block. |
| 781 |     \| PRINT postfix_expr nl { | Alternative grammar production branch. |
| 782 |       ValueType t = infer_postfix_type($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 783 |         char* expr = postfix_to_c($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 784 |       if (t == TY_DOUBLE) { | Opens a semantic action or C code block. |
| 785 |             $$ = str_printf("printf(\"%%g\", (double)(%s));\n", expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 786 |       } else if (t == TY_INT \|\| t == TY_CHAR) { | Opens a semantic action or C code block. |
| 787 |             $$ = str_printf("printf(\"%%d\", (int)(%s));\n", expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 788 |       } else { | Opens a semantic action or C code block. |
| 789 |             $$ = str_printf("printf(\"%%g\", (double)(%s));\n", expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 790 |       } | Closes a semantic action or C code block. |
| 791 |         free(expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 792 |       } | Closes a semantic action or C code block. |
| 793 |     \| PRINT STRING nl { | Alternative grammar production branch. |
| 794 |         $$ = str_printf("printf(\"%%s\", %s);\n", $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 795 |       } | Closes a semantic action or C code block. |
| 796 |     \| SATR nl { | Alternative grammar production branch. |
| 797 |         $$ = xstrdup("printf(\"\\n\");\n"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 798 |       } | Closes a semantic action or C code block. |
| 799 |     \| call_expr nl { | Alternative grammar production branch. |
| 800 |       $$ = str_printf("%s;\n", $1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 801 |       } | Closes a semantic action or C code block. |
| 802 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 803 |  | Blank line for readability and grouping of grammar/code sections. |
| 804 |     call_expr | Grammar nonterminal label line introducing productions. |
| 805 |       : IDENT LPAREN arg_list_opt RPAREN { | Beginning of production alternatives for current nonterminal. |
| 806 |         int fi = function_index($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 807 |         last_call_expr_type = TY_UNKNOWN; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 808 |         if (fi < 0) { | Opens a semantic action or C code block. |
| 809 |           semantic_error("call to undefined function '%s'", $1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 810 |         } else { | Opens a semantic action or C code block. |
| 811 |           if (functions[fi].arg_count != pending_call_arg_count) { | Opens a semantic action or C code block. |
| 812 |             semantic_error("function '%s' expects %d args but got %d", $1, functions[fi].arg_count, pending_call_arg_count); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 813 |           } else { | Opens a semantic action or C code block. |
| 814 |             for (int i = 0; i < pending_call_arg_count; i++) { | Opens a semantic action or C code block. |
| 815 |               if (!can_assign(functions[fi].arg_types[i], pending_call_arg_types[i])) { | Opens a semantic action or C code block. |
| 816 |                 semantic_error("type mismatch for arg %d in call to '%s'", i + 1, $1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 817 |                 break; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 818 |               } | Closes a semantic action or C code block. |
| 819 |             } | Closes a semantic action or C code block. |
| 820 |             last_call_expr_type = functions[fi].ret_type; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 821 |           } | Closes a semantic action or C code block. |
| 822 |         } | Closes a semantic action or C code block. |
| 823 |         $$ = str_printf("%s(%s)", $1, $3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 824 |         } | Closes a semantic action or C code block. |
| 825 |       ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 826 |  | Blank line for readability and grouping of grammar/code sections. |
| 827 |   arg_list_opt | Grammar nonterminal label line introducing productions. |
| 828 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 829 |       reset_pending_call_args(); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 830 |       $$ = xstrdup(""); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 831 |       } | Closes a semantic action or C code block. |
| 832 |       \| arg_reset arg_list { | Alternative grammar production branch. |
| 833 |           $$ = $2; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 834 |       } | Closes a semantic action or C code block. |
| 835 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 836 |  | Blank line for readability and grouping of grammar/code sections. |
| 837 |   arg_reset | Grammar nonterminal label line introducing productions. |
| 838 |       : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 839 |           reset_pending_call_args(); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 840 |         } | Closes a semantic action or C code block. |
| 841 |       ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 842 |  | Blank line for readability and grouping of grammar/code sections. |
| 843 |   arg_list | Grammar nonterminal label line introducing productions. |
| 844 |     : arg_item { | Beginning of production alternatives for current nonterminal. |
| 845 |       $$ = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 846 |       } | Closes a semantic action or C code block. |
| 847 |     \| arg_list COMMA arg_item { | Alternative grammar production branch. |
| 848 |       char* merged = str_printf("%s,%s", $1, $3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 849 |       free($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 850 |       free($3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 851 |       $$ = merged; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 852 |       } | Closes a semantic action or C code block. |
| 853 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 854 |  | Blank line for readability and grouping of grammar/code sections. |
| 855 |   arg_item | Grammar nonterminal label line introducing productions. |
| 856 |     : postfix_expr { | Beginning of production alternatives for current nonterminal. |
| 857 |       ValueType t = infer_postfix_type($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 858 |       push_pending_call_arg(t); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 859 |       $$ = postfix_to_c($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 860 |       } | Closes a semantic action or C code block. |
| 861 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 862 |  | Blank line for readability and grouping of grammar/code sections. |
| 863 | opt_expr | Grammar nonterminal label line introducing productions. |
| 864 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 865 |         $$ = xstrdup(""); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 866 |       } | Closes a semantic action or C code block. |
| 867 |     \| postfix_expr { | Alternative grammar production branch. |
| 868 |         $$ = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 869 |       } | Closes a semantic action or C code block. |
| 870 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 871 |  | Blank line for readability and grouping of grammar/code sections. |
| 872 | postfix_expr | Grammar nonterminal label line introducing productions. |
| 873 |     : postfix_item { | Beginning of production alternatives for current nonterminal. |
| 874 |         $$ = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 875 |       } | Closes a semantic action or C code block. |
| 876 |     \| postfix_expr postfix_item { | Alternative grammar production branch. |
| 877 |         char* merged = concat_space($1, $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 878 |         free($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 879 |         free($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 880 |         $$ = merged; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 881 |       } | Closes a semantic action or C code block. |
| 882 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 883 |  | Blank line for readability and grouping of grammar/code sections. |
| 884 | postfix_item | Grammar nonterminal label line introducing productions. |
| 885 |     : IDENT { | Beginning of production alternatives for current nonterminal. |
| 886 |         $$ = xstrdup($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 887 |       } | Closes a semantic action or C code block. |
| 888 |     \| NUMBER { | Alternative grammar production branch. |
| 889 |         $$ = xstrdup($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 890 |       } | Closes a semantic action or C code block. |
| 891 |     \| call_expr { | Alternative grammar production branch. |
| 892 |         $$ = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 893 |       } | Closes a semantic action or C code block. |
| 894 |     \| PLUS { | Alternative grammar production branch. |
| 895 |         $$ = xstrdup("+"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 896 |       } | Closes a semantic action or C code block. |
| 897 |     \| MINUS { | Alternative grammar production branch. |
| 898 |         $$ = xstrdup("-"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 899 |       } | Closes a semantic action or C code block. |
| 900 |     \| STAR { | Alternative grammar production branch. |
| 901 |         $$ = xstrdup("*"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 902 |       } | Closes a semantic action or C code block. |
| 903 |     \| SLASH { | Alternative grammar production branch. |
| 904 |         $$ = xstrdup("/"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 905 |       } | Closes a semantic action or C code block. |
| 906 |     \| MOD { | Alternative grammar production branch. |
| 907 |         $$ = xstrdup("%"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 908 |       } | Closes a semantic action or C code block. |
| 909 |     \| GT { | Alternative grammar production branch. |
| 910 |         $$ = xstrdup(">"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 911 |       } | Closes a semantic action or C code block. |
| 912 |     \| LT { | Alternative grammar production branch. |
| 913 |         $$ = xstrdup("<"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 914 |       } | Closes a semantic action or C code block. |
| 915 |     \| GE { | Alternative grammar production branch. |
| 916 |         $$ = xstrdup(">="); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 917 |       } | Closes a semantic action or C code block. |
| 918 |     \| LE { | Alternative grammar production branch. |
| 919 |         $$ = xstrdup("<="); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 920 |       } | Closes a semantic action or C code block. |
| 921 |     \| EQ { | Alternative grammar production branch. |
| 922 |         $$ = xstrdup("=="); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 923 |       } | Closes a semantic action or C code block. |
| 924 |     \| NE { | Alternative grammar production branch. |
| 925 |         $$ = xstrdup("!="); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 926 |       } | Closes a semantic action or C code block. |
| 927 |     \| AND { | Alternative grammar production branch. |
| 928 |         $$ = xstrdup("&&"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 929 |       } | Closes a semantic action or C code block. |
| 930 |     \| OR { | Alternative grammar production branch. |
| 931 |         $$ = xstrdup("\|\|"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 932 |       } | Closes a semantic action or C code block. |
| 933 |     \| NOT { | Alternative grammar production branch. |
| 934 |         $$ = xstrdup("!"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 935 |       } | Closes a semantic action or C code block. |
| 936 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 937 |  | Blank line for readability and grouping of grammar/code sections. |
| 938 | nl | Grammar nonterminal label line introducing productions. |
| 939 |     : NEWLINE { | Beginning of production alternatives for current nonterminal. |
| 940 |         $$ = xstrdup("\n"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 941 |       } | Closes a semantic action or C code block. |
| 942 |     \| nl NEWLINE { | Alternative grammar production branch. |
| 943 |         $$ = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 944 |       } | Closes a semantic action or C code block. |
| 945 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 946 |  | Blank line for readability and grouping of grammar/code sections. |
| 947 | %% | Bison section delimiter between declarations, grammar, and user code. |
| 948 |  | Blank line for readability and grouping of grammar/code sections. |
| 949 | void yyerror(const char* s) { | Parser error function declaration/definition. |
| 950 |   fprintf(stderr, "Parse error (line %d): %s\n", yylineno, s); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 951 | } | Closes a semantic action or C code block. |
| 952 |  | Blank line for readability and grouping of grammar/code sections. |
| 953 | const char* get_generated_program(void) { | Opens a semantic action or C code block. |
| 954 |     return generated_program; | Returns computed value/token/type from helper code. |
| 955 | } | Closes a semantic action or C code block. |
| 956 |  | Blank line for readability and grouping of grammar/code sections. |
| 957 | int has_semantic_errors(void) { | Opens a semantic action or C code block. |
| 958 |   return semantic_errors > 0; | Returns computed value/token/type from helper code. |
| 959 | } | Closes a semantic action or C code block. |
