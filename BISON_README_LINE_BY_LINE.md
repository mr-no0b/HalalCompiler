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
| 49 | static int pending_read_count = 0; | Static helper declaration or definition used by parser and semantic phase. |
| 50 | static ValueType pending_read_types[64]; | Static helper declaration or definition used by parser and semantic phase. |
| 51 |  | Blank line for readability and grouping of grammar/code sections. |
| 52 | static char* xstrdup(const char* s) { | Static helper declaration or definition used by parser and semantic phase. |
| 53 |     size_t n = strlen(s) + 1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 54 |     char* out = (char*)malloc(n); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 55 |     if (!out) { | Opens a semantic action or C code block. |
| 56 |         fprintf(stderr, "Out of memory\n"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 57 |         exit(1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 58 |     } | Closes a semantic action or C code block. |
| 59 |     memcpy(out, s, n); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 60 |     return out; | Returns computed value/token/type from helper code. |
| 61 | } | Closes a semantic action or C code block. |
| 62 |  | Blank line for readability and grouping of grammar/code sections. |
| 63 | static char* str_printf(const char* fmt, ...) { | Static helper declaration or definition used by parser and semantic phase. |
| 64 |     va_list ap; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 65 |     va_start(ap, fmt); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 66 |     int n = vsnprintf(NULL, 0, fmt, ap); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 67 |     va_end(ap); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 68 |  | Blank line for readability and grouping of grammar/code sections. |
| 69 |     if (n < 0) { | Opens a semantic action or C code block. |
| 70 |         fprintf(stderr, "Formatting error\n"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 71 |         exit(1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 72 |     } | Closes a semantic action or C code block. |
| 73 |  | Blank line for readability and grouping of grammar/code sections. |
| 74 |     char* out = (char*)malloc((size_t)n + 1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 75 |     if (!out) { | Opens a semantic action or C code block. |
| 76 |         fprintf(stderr, "Out of memory\n"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 77 |         exit(1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 78 |     } | Closes a semantic action or C code block. |
| 79 |  | Blank line for readability and grouping of grammar/code sections. |
| 80 |     va_start(ap, fmt); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 81 |     vsnprintf(out, (size_t)n + 1, fmt, ap); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 82 |     va_end(ap); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 83 |     return out; | Returns computed value/token/type from helper code. |
| 84 | } | Closes a semantic action or C code block. |
| 85 |  | Blank line for readability and grouping of grammar/code sections. |
| 86 | static char* concat2(const char* a, const char* b) { | Static helper declaration or definition used by parser and semantic phase. |
| 87 |     size_t na = strlen(a); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 88 |     size_t nb = strlen(b); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 89 |     char* out = (char*)malloc(na + nb + 1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 90 |     if (!out) { | Opens a semantic action or C code block. |
| 91 |         fprintf(stderr, "Out of memory\n"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 92 |         exit(1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 93 |     } | Closes a semantic action or C code block. |
| 94 |     memcpy(out, a, na); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 95 |     memcpy(out + na, b, nb); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 96 |     out[na + nb] = '\0'; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 97 |     return out; | Returns computed value/token/type from helper code. |
| 98 | } | Closes a semantic action or C code block. |
| 99 |  | Blank line for readability and grouping of grammar/code sections. |
| 100 | static char* concat_space(const char* a, const char* b) { | Static helper declaration or definition used by parser and semantic phase. |
| 101 |     if (a[0] == '\0') { | Opens a semantic action or C code block. |
| 102 |         return xstrdup(b); | Returns computed value/token/type from helper code. |
| 103 |     } | Closes a semantic action or C code block. |
| 104 |   return str_printf("%s\t%s", a, b); | Returns computed value/token/type from helper code. |
| 105 | } | Closes a semantic action or C code block. |
| 106 |  | Blank line for readability and grouping of grammar/code sections. |
| 107 | static char* indent_code(const char* code) { | Static helper declaration or definition used by parser and semantic phase. |
| 108 |     size_t n = strlen(code); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 109 |     char* out = (char*)malloc(n * 2 + 64); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 110 |     if (!out) { | Opens a semantic action or C code block. |
| 111 |         fprintf(stderr, "Out of memory\n"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 112 |         exit(1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 113 |     } | Closes a semantic action or C code block. |
| 114 |  | Blank line for readability and grouping of grammar/code sections. |
| 115 |     size_t j = 0; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 116 |     int at_line_start = 1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 117 |     for (size_t i = 0; i < n; i++) { | Opens a semantic action or C code block. |
| 118 |         if (at_line_start && code[i] != '\n') { | Opens a semantic action or C code block. |
| 119 |             out[j++] = ' '; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 120 |             out[j++] = ' '; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 121 |             out[j++] = ' '; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 122 |             out[j++] = ' '; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 123 |             at_line_start = 0; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 124 |         } | Closes a semantic action or C code block. |
| 125 |         out[j++] = code[i]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 126 |         if (code[i] == '\n') { | Opens a semantic action or C code block. |
| 127 |             at_line_start = 1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 128 |         } | Closes a semantic action or C code block. |
| 129 |     } | Closes a semantic action or C code block. |
| 130 |     out[j] = '\0'; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 131 |     return out; | Returns computed value/token/type from helper code. |
| 132 | } | Closes a semantic action or C code block. |
| 133 |  | Blank line for readability and grouping of grammar/code sections. |
| 134 | static void semantic_error(const char* fmt, ...) { | Static helper declaration or definition used by parser and semantic phase. |
| 135 |   semantic_errors++; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 136 |   fprintf(stderr, "Semantic error (line %d): ", yylineno); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 137 |   va_list ap; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 138 |   va_start(ap, fmt); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 139 |   vfprintf(stderr, fmt, ap); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 140 |   va_end(ap); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 141 |   fprintf(stderr, "\n"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 142 | } | Closes a semantic action or C code block. |
| 143 |  | Blank line for readability and grouping of grammar/code sections. |
| 144 | static ValueType parse_type_name(const char* t) { | Static helper declaration or definition used by parser and semantic phase. |
| 145 |   if (strcmp(t, "int") == 0) return TY_INT; | Conditional semantic logic or helper control flow. |
| 146 |   if (strcmp(t, "double") == 0) return TY_DOUBLE; | Conditional semantic logic or helper control flow. |
| 147 |   if (strcmp(t, "char") == 0) return TY_CHAR; | Conditional semantic logic or helper control flow. |
| 148 |   if (strcmp(t, "void") == 0) return TY_VOID; | Conditional semantic logic or helper control flow. |
| 149 |   return TY_UNKNOWN; | Returns computed value/token/type from helper code. |
| 150 | } | Closes a semantic action or C code block. |
| 151 |  | Blank line for readability and grouping of grammar/code sections. |
| 152 | static void clear_symbols(void) { | Static helper declaration or definition used by parser and semantic phase. |
| 153 |   symbol_count = 0; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 154 | } | Closes a semantic action or C code block. |
| 155 |  | Blank line for readability and grouping of grammar/code sections. |
| 156 | static int symbol_index(const char* name) { | Static helper declaration or definition used by parser and semantic phase. |
| 157 |   for (int i = symbol_count - 1; i >= 0; i--) { | Opens a semantic action or C code block. |
| 158 |     if (strcmp(symbols[i].name, name) == 0) { | Opens a semantic action or C code block. |
| 159 |       return i; | Returns computed value/token/type from helper code. |
| 160 |     } | Closes a semantic action or C code block. |
| 161 |   } | Closes a semantic action or C code block. |
| 162 |   return -1; | Returns computed value/token/type from helper code. |
| 163 | } | Closes a semantic action or C code block. |
| 164 |  | Blank line for readability and grouping of grammar/code sections. |
| 165 | static ValueType symbol_type(const char* name) { | Static helper declaration or definition used by parser and semantic phase. |
| 166 |   int i = symbol_index(name); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 167 |   if (i < 0) return TY_UNKNOWN; | Conditional semantic logic or helper control flow. |
| 168 |   return symbols[i].type; | Returns computed value/token/type from helper code. |
| 169 | } | Closes a semantic action or C code block. |
| 170 |  | Blank line for readability and grouping of grammar/code sections. |
| 171 | static int declare_symbol(const char* name, ValueType type) { | Static helper declaration or definition used by parser and semantic phase. |
| 172 |   if (symbol_index(name) >= 0) { | Opens a semantic action or C code block. |
| 173 |     semantic_error("redeclaration of variable '%s'", name); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 174 |     return 0; | Returns computed value/token/type from helper code. |
| 175 |   } | Closes a semantic action or C code block. |
| 176 |   if (symbol_count >= 1024) { | Opens a semantic action or C code block. |
| 177 |     semantic_error("too many symbols"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 178 |     return 0; | Returns computed value/token/type from helper code. |
| 179 |   } | Closes a semantic action or C code block. |
| 180 |   strncpy(symbols[symbol_count].name, name, sizeof(symbols[symbol_count].name) - 1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 181 |   symbols[symbol_count].name[sizeof(symbols[symbol_count].name) - 1] = '\0'; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 182 |   symbols[symbol_count].type = type; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 183 |   symbol_count++; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 184 |   return 1; | Returns computed value/token/type from helper code. |
| 185 | } | Closes a semantic action or C code block. |
| 186 |  | Blank line for readability and grouping of grammar/code sections. |
| 187 | static int function_index(const char* name) { | Static helper declaration or definition used by parser and semantic phase. |
| 188 |   for (int i = 0; i < function_count; i++) { | Opens a semantic action or C code block. |
| 189 |     if (strcmp(functions[i].name, name) == 0) return i; | Conditional semantic logic or helper control flow. |
| 190 |   } | Closes a semantic action or C code block. |
| 191 |   return -1; | Returns computed value/token/type from helper code. |
| 192 | } | Closes a semantic action or C code block. |
| 193 |  | Blank line for readability and grouping of grammar/code sections. |
| 194 | static int function_exists(const char* name) { | Static helper declaration or definition used by parser and semantic phase. |
| 195 |   return function_index(name) >= 0; | Returns computed value/token/type from helper code. |
| 196 | } | Closes a semantic action or C code block. |
| 197 |  | Blank line for readability and grouping of grammar/code sections. |
| 198 | static void reset_pending_params(void) { | Static helper declaration or definition used by parser and semantic phase. |
| 199 |   pending_param_count = 0; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 200 | } | Closes a semantic action or C code block. |
| 201 |  | Blank line for readability and grouping of grammar/code sections. |
| 202 | static void push_pending_param(const char* type_name, const char* param_name) { | Static helper declaration or definition used by parser and semantic phase. |
| 203 |   if (pending_param_count >= 64) { | Opens a semantic action or C code block. |
| 204 |     semantic_error("too many function parameters"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 205 |     return; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 206 |   } | Closes a semantic action or C code block. |
| 207 |   pending_param_types[pending_param_count] = parse_type_name(type_name); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 208 |   strncpy(pending_param_names[pending_param_count], param_name, sizeof(pending_param_names[pending_param_count]) - 1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 209 |   pending_param_names[pending_param_count][sizeof(pending_param_names[pending_param_count]) - 1] = '\0'; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 210 |   pending_param_count++; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 211 | } | Closes a semantic action or C code block. |
| 212 |  | Blank line for readability and grouping of grammar/code sections. |
| 213 | static void reset_pending_call_args(void) { | Static helper declaration or definition used by parser and semantic phase. |
| 214 |   pending_call_arg_count = 0; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 215 | } | Closes a semantic action or C code block. |
| 216 |  | Blank line for readability and grouping of grammar/code sections. |
| 217 | static void push_pending_call_arg(ValueType t) { | Static helper declaration or definition used by parser and semantic phase. |
| 218 |   if (pending_call_arg_count >= 64) { | Opens a semantic action or C code block. |
| 219 |     semantic_error("too many function call arguments"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 220 |     return; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 221 |   } | Closes a semantic action or C code block. |
| 222 |   pending_call_arg_types[pending_call_arg_count++] = t; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 223 | } | Closes a semantic action or C code block. |
| 224 |  | Blank line for readability and grouping of grammar/code sections. |
| 225 | static void reset_pending_read_targets(void) { | Static helper declaration or definition used by parser and semantic phase. |
| 226 |   pending_read_count = 0; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 227 | } | Closes a semantic action or C code block. |
| 228 |  | Blank line for readability and grouping of grammar/code sections. |
| 229 | static void push_pending_read_target(ValueType t, const char* name) { | Static helper declaration or definition used by parser and semantic phase. |
| 230 |   if (pending_read_count >= 64) { | Opens a semantic action or C code block. |
| 231 |     semantic_error("too many input targets in iqra/scanf"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 232 |     return; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 233 |   } | Closes a semantic action or C code block. |
| 234 |   pending_read_types[pending_read_count++] = t; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 235 |   (void)name; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 236 | } | Closes a semantic action or C code block. |
| 237 |  | Blank line for readability and grouping of grammar/code sections. |
| 238 | static int parse_scanf_specs(const char* fmt, ValueType out_specs[], int max_specs) { | Static helper declaration or definition used by parser and semantic phase. |
| 239 |   if (!fmt) return -1; | Conditional semantic logic or helper control flow. |
| 240 |   size_t n = strlen(fmt); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 241 |   if (n < 2 \|\| fmt[0] != '"' \|\| fmt[n - 1] != '"') return -1; | Conditional semantic logic or helper control flow. |
| 242 |  | Blank line for readability and grouping of grammar/code sections. |
| 243 |   int count = 0; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 244 |   for (size_t i = 1; i + 1 < n; i++) { | Opens a semantic action or C code block. |
| 245 |     if (fmt[i] != '%') continue; | Conditional semantic logic or helper control flow. |
| 246 |  | Blank line for readability and grouping of grammar/code sections. |
| 247 |     if (i + 1 < n && fmt[i + 1] == '%') { | Opens a semantic action or C code block. |
| 248 |       i++; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 249 |       continue; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 250 |     } | Closes a semantic action or C code block. |
| 251 |  | Blank line for readability and grouping of grammar/code sections. |
| 252 |     size_t j = i + 1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 253 |     while (j + 1 < n && strchr(" +-#0", fmt[j])) j++; | Iterative logic for expression parsing/type inference helpers. |
| 254 |     while (j + 1 < n && isdigit((unsigned char)fmt[j])) j++; | Iterative logic for expression parsing/type inference helpers. |
| 255 |  | Blank line for readability and grouping of grammar/code sections. |
| 256 |     if (j + 1 < n && fmt[j] == 'l' && (fmt[j + 1] == 'f' \|\| fmt[j + 1] == 'g' \|\| fmt[j + 1] == 'e')) { | Opens a semantic action or C code block. |
| 257 |       if (count < max_specs) out_specs[count] = TY_DOUBLE; | Conditional semantic logic or helper control flow. |
| 258 |       count++; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 259 |       i = j + 1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 260 |       continue; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 261 |     } | Closes a semantic action or C code block. |
| 262 |  | Blank line for readability and grouping of grammar/code sections. |
| 263 |     if (j + 1 < n && (fmt[j] == 'd' \|\| fmt[j] == 'i')) { | Opens a semantic action or C code block. |
| 264 |       if (count < max_specs) out_specs[count] = TY_INT; | Conditional semantic logic or helper control flow. |
| 265 |       count++; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 266 |       i = j; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 267 |       continue; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 268 |     } | Closes a semantic action or C code block. |
| 269 |  | Blank line for readability and grouping of grammar/code sections. |
| 270 |     if (j + 1 < n && fmt[j] == 'c') { | Opens a semantic action or C code block. |
| 271 |       if (count < max_specs) out_specs[count] = TY_CHAR; | Conditional semantic logic or helper control flow. |
| 272 |       count++; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 273 |       i = j; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 274 |       continue; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 275 |     } | Closes a semantic action or C code block. |
| 276 |  | Blank line for readability and grouping of grammar/code sections. |
| 277 |     return -1; | Returns computed value/token/type from helper code. |
| 278 |   } | Closes a semantic action or C code block. |
| 279 |   return count; | Returns computed value/token/type from helper code. |
| 280 | } | Closes a semantic action or C code block. |
| 281 |  | Blank line for readability and grouping of grammar/code sections. |
| 282 | static void register_function(const char* name, ValueType ret_type) { | Static helper declaration or definition used by parser and semantic phase. |
| 283 |   if (function_exists(name)) { | Opens a semantic action or C code block. |
| 284 |     semantic_error("redefinition of function '%s'", name); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 285 |     return; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 286 |   } | Closes a semantic action or C code block. |
| 287 |   if (function_count >= 256) { | Opens a semantic action or C code block. |
| 288 |     semantic_error("too many functions"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 289 |     return; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 290 |   } | Closes a semantic action or C code block. |
| 291 |   strncpy(functions[function_count].name, name, sizeof(functions[function_count].name) - 1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 292 |   functions[function_count].name[sizeof(functions[function_count].name) - 1] = '\0'; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 293 |   functions[function_count].ret_type = ret_type; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 294 |   functions[function_count].arg_count = pending_param_count; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 295 |   for (int i = 0; i < pending_param_count; i++) { | Opens a semantic action or C code block. |
| 296 |     functions[function_count].arg_types[i] = pending_param_types[i]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 297 |   } | Closes a semantic action or C code block. |
| 298 |   function_count++; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 299 | } | Closes a semantic action or C code block. |
| 300 |  | Blank line for readability and grouping of grammar/code sections. |
| 301 | static void begin_function(const char* type_name, const char* name) { | Static helper declaration or definition used by parser and semantic phase. |
| 302 |   current_function_type = parse_type_name(type_name); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 303 |   clear_symbols(); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 304 |   register_function(name, current_function_type); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 305 |   for (int i = 0; i < pending_param_count; i++) { | Opens a semantic action or C code block. |
| 306 |     declare_symbol(pending_param_names[i], pending_param_types[i]); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 307 |   } | Closes a semantic action or C code block. |
| 308 | } | Closes a semantic action or C code block. |
| 309 |  | Blank line for readability and grouping of grammar/code sections. |
| 310 | static int is_number_token(const char* tok) { | Static helper declaration or definition used by parser and semantic phase. |
| 311 |   if (!tok \|\| !*tok) return 0; | Conditional semantic logic or helper control flow. |
| 312 |   int has_dot = 0; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 313 |   int i = 0; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 314 |   if (tok[0] == '-') i = 1; | Conditional semantic logic or helper control flow. |
| 315 |   for (; tok[i]; i++) { | Opens a semantic action or C code block. |
| 316 |     if (tok[i] == '.') { | Opens a semantic action or C code block. |
| 317 |       if (has_dot) return 0; | Conditional semantic logic or helper control flow. |
| 318 |       has_dot = 1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 319 |       continue; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 320 |     } | Closes a semantic action or C code block. |
| 321 |     if (!isdigit((unsigned char)tok[i])) return 0; | Conditional semantic logic or helper control flow. |
| 322 |   } | Closes a semantic action or C code block. |
| 323 |   return i > 0; | Returns computed value/token/type from helper code. |
| 324 | } | Closes a semantic action or C code block. |
| 325 |  | Blank line for readability and grouping of grammar/code sections. |
| 326 | static int is_identifier_token(const char* tok) { | Static helper declaration or definition used by parser and semantic phase. |
| 327 |   if (!tok \|\| !*tok) return 0; | Conditional semantic logic or helper control flow. |
| 328 |   if (!(isalpha((unsigned char)tok[0]) \|\| tok[0] == '_')) return 0; | Conditional semantic logic or helper control flow. |
| 329 |   for (int i = 1; tok[i]; i++) { | Opens a semantic action or C code block. |
| 330 |     if (!(isalnum((unsigned char)tok[i]) \|\| tok[i] == '_')) return 0; | Conditional semantic logic or helper control flow. |
| 331 |   } | Closes a semantic action or C code block. |
| 332 |   return 1; | Returns computed value/token/type from helper code. |
| 333 | } | Closes a semantic action or C code block. |
| 334 |  | Blank line for readability and grouping of grammar/code sections. |
| 335 | static int is_operator(const char* tok) { | Static helper declaration or definition used by parser and semantic phase. |
| 336 |     return strcmp(tok, "+") == 0 \|\| strcmp(tok, "-") == 0 \|\| strcmp(tok, "*") == 0 \|\| | Returns computed value/token/type from helper code. |
| 337 |        strcmp(tok, "/") == 0 \|\| strcmp(tok, "%") == 0 \|\| strcmp(tok, ">") == 0 \|\| strcmp(tok, "<") == 0 \|\| | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 338 |            strcmp(tok, ">=") == 0 \|\| strcmp(tok, "<=") == 0 \|\| strcmp(tok, "==") == 0 \|\| | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 339 |        strcmp(tok, "!=") == 0 \|\| strcmp(tok, "&&") == 0 \|\| strcmp(tok, "\|\|") == 0 \|\| | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 340 |        strcmp(tok, "!") == 0; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 341 | } | Closes a semantic action or C code block. |
| 342 |  | Blank line for readability and grouping of grammar/code sections. |
| 343 | static int is_call_token(const char* tok) { | Static helper declaration or definition used by parser and semantic phase. |
| 344 |   const char* lp = strchr(tok, '('); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 345 |   size_t n = strlen(tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 346 |   return lp != NULL && n > 2 && tok[n - 1] == ')'; | Returns computed value/token/type from helper code. |
| 347 | } | Closes a semantic action or C code block. |
| 348 |  | Blank line for readability and grouping of grammar/code sections. |
| 349 | static ValueType call_return_type_from_token(const char* tok) { | Static helper declaration or definition used by parser and semantic phase. |
| 350 |   const char* lp = strchr(tok, '('); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 351 |   if (!lp) return TY_UNKNOWN; | Conditional semantic logic or helper control flow. |
| 352 |   size_t len = (size_t)(lp - tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 353 |   if (len == 0 \|\| len >= 64) return TY_UNKNOWN; | Conditional semantic logic or helper control flow. |
| 354 |   char name[64]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 355 |   memcpy(name, tok, len); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 356 |   name[len] = '\0'; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 357 |   int fi = function_index(name); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 358 |   if (fi < 0) { | Opens a semantic action or C code block. |
| 359 |     semantic_error("call to undefined function '%s'", name); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 360 |     return TY_UNKNOWN; | Returns computed value/token/type from helper code. |
| 361 |   } | Closes a semantic action or C code block. |
| 362 |   return functions[fi].ret_type; | Returns computed value/token/type from helper code. |
| 363 | } | Closes a semantic action or C code block. |
| 364 |  | Blank line for readability and grouping of grammar/code sections. |
| 365 | static int op_arity(const char* tok) { | Static helper declaration or definition used by parser and semantic phase. |
| 366 |   if (strcmp(tok, "!") == 0) return 1; | Conditional semantic logic or helper control flow. |
| 367 |   return 2; | Returns computed value/token/type from helper code. |
| 368 | } | Closes a semantic action or C code block. |
| 369 |  | Blank line for readability and grouping of grammar/code sections. |
| 370 | static int is_numeric_type(ValueType t) { | Static helper declaration or definition used by parser and semantic phase. |
| 371 |   return t == TY_INT \|\| t == TY_DOUBLE \|\| t == TY_CHAR; | Returns computed value/token/type from helper code. |
| 372 | } | Closes a semantic action or C code block. |
| 373 |  | Blank line for readability and grouping of grammar/code sections. |
| 374 | static int can_assign(ValueType lhs, ValueType rhs) { | Static helper declaration or definition used by parser and semantic phase. |
| 375 |   if (lhs == TY_UNKNOWN \|\| rhs == TY_UNKNOWN) return 0; | Conditional semantic logic or helper control flow. |
| 376 |   if (lhs == TY_VOID \|\| rhs == TY_VOID) return 0; | Conditional semantic logic or helper control flow. |
| 377 |   if (lhs == rhs) return 1; | Conditional semantic logic or helper control flow. |
| 378 |   return is_numeric_type(lhs) && is_numeric_type(rhs); | Returns computed value/token/type from helper code. |
| 379 | } | Closes a semantic action or C code block. |
| 380 |  | Blank line for readability and grouping of grammar/code sections. |
| 381 | static ValueType number_type(const char* tok) { | Static helper declaration or definition used by parser and semantic phase. |
| 382 |   return strchr(tok, '.') ? TY_DOUBLE : TY_INT; | Returns computed value/token/type from helper code. |
| 383 | } | Closes a semantic action or C code block. |
| 384 |  | Blank line for readability and grouping of grammar/code sections. |
| 385 | static ValueType infer_postfix_type(const char* expr) { | Static helper declaration or definition used by parser and semantic phase. |
| 386 |   char* copy = xstrdup(expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 387 |   ValueType stack[512]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 388 |   int top = 0; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 389 |  | Blank line for readability and grouping of grammar/code sections. |
| 390 |   char* tok = strtok(copy, "\t"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 391 |   while (tok) { | Opens a semantic action or C code block. |
| 392 |     if (!is_operator(tok)) { | Opens a semantic action or C code block. |
| 393 |       if (is_number_token(tok)) { | Opens a semantic action or C code block. |
| 394 |         stack[top++] = number_type(tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 395 |       } else if (tok[0] == '"') { | Opens a semantic action or C code block. |
| 396 |         stack[top++] = TY_CHAR; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 397 |       } else if (is_call_token(tok)) { | Opens a semantic action or C code block. |
| 398 |         ValueType rt = call_return_type_from_token(tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 399 |         if (rt == TY_UNKNOWN) { | Opens a semantic action or C code block. |
| 400 |           free(copy); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 401 |           return TY_UNKNOWN; | Returns computed value/token/type from helper code. |
| 402 |         } | Closes a semantic action or C code block. |
| 403 |         stack[top++] = rt; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 404 |       } else if (is_identifier_token(tok)) { | Opens a semantic action or C code block. |
| 405 |         ValueType t = symbol_type(tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 406 |         if (t == TY_UNKNOWN) { | Opens a semantic action or C code block. |
| 407 |           semantic_error("use of undeclared variable '%s'", tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 408 |           free(copy); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 409 |           return TY_UNKNOWN; | Returns computed value/token/type from helper code. |
| 410 |         } | Closes a semantic action or C code block. |
| 411 |         stack[top++] = t; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 412 |       } else { | Opens a semantic action or C code block. |
| 413 |         semantic_error("invalid token in expression '%s'", tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 414 |         free(copy); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 415 |         return TY_UNKNOWN; | Returns computed value/token/type from helper code. |
| 416 |       } | Closes a semantic action or C code block. |
| 417 |       tok = strtok(NULL, "\t"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 418 |       continue; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 419 |     } | Closes a semantic action or C code block. |
| 420 |  | Blank line for readability and grouping of grammar/code sections. |
| 421 |     int arity = op_arity(tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 422 |     if (top < arity) { | Opens a semantic action or C code block. |
| 423 |       semantic_error("invalid postfix expression '%s'", expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 424 |       free(copy); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 425 |       return TY_UNKNOWN; | Returns computed value/token/type from helper code. |
| 426 |     } | Closes a semantic action or C code block. |
| 427 |  | Blank line for readability and grouping of grammar/code sections. |
| 428 |     if (arity == 1) { | Opens a semantic action or C code block. |
| 429 |       ValueType a = stack[--top]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 430 |       if (!is_numeric_type(a)) { | Opens a semantic action or C code block. |
| 431 |         semantic_error("operator '%s' requires numeric operand", tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 432 |         free(copy); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 433 |         return TY_UNKNOWN; | Returns computed value/token/type from helper code. |
| 434 |       } | Closes a semantic action or C code block. |
| 435 |       stack[top++] = TY_INT; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 436 |       tok = strtok(NULL, "\t"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 437 |       continue; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 438 |     } | Closes a semantic action or C code block. |
| 439 |  | Blank line for readability and grouping of grammar/code sections. |
| 440 |     ValueType rhs = stack[--top]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 441 |     ValueType lhs = stack[--top]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 442 |     if (!is_numeric_type(lhs) \|\| !is_numeric_type(rhs)) { | Opens a semantic action or C code block. |
| 443 |       semantic_error("operator '%s' requires numeric operands", tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 444 |       free(copy); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 445 |       return TY_UNKNOWN; | Returns computed value/token/type from helper code. |
| 446 |     } | Closes a semantic action or C code block. |
| 447 |  | Blank line for readability and grouping of grammar/code sections. |
| 448 |     if (strcmp(tok, "%") == 0) { | Opens a semantic action or C code block. |
| 449 |       if (lhs == TY_DOUBLE \|\| rhs == TY_DOUBLE) { | Opens a semantic action or C code block. |
| 450 |         semantic_error("operator '%%' does not support double operands"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 451 |         free(copy); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 452 |         return TY_UNKNOWN; | Returns computed value/token/type from helper code. |
| 453 |       } | Closes a semantic action or C code block. |
| 454 |       stack[top++] = TY_INT; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 455 |     } else if (strcmp(tok, ">") == 0 \|\| strcmp(tok, "<") == 0 \|\| strcmp(tok, ">=") == 0 \|\| | Closes a semantic action or C code block. |
| 456 |            strcmp(tok, "<=") == 0 \|\| strcmp(tok, "==") == 0 \|\| strcmp(tok, "!=") == 0 \|\| | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 457 |            strcmp(tok, "&&") == 0 \|\| strcmp(tok, "\|\|") == 0) { | Opens a semantic action or C code block. |
| 458 |       stack[top++] = TY_INT; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 459 |     } else { | Opens a semantic action or C code block. |
| 460 |       stack[top++] = (lhs == TY_DOUBLE \|\| rhs == TY_DOUBLE) ? TY_DOUBLE : TY_INT; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 461 |     } | Closes a semantic action or C code block. |
| 462 |  | Blank line for readability and grouping of grammar/code sections. |
| 463 |     tok = strtok(NULL, "\t"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 464 |   } | Closes a semantic action or C code block. |
| 465 |  | Blank line for readability and grouping of grammar/code sections. |
| 466 |   if (top != 1) { | Opens a semantic action or C code block. |
| 467 |     semantic_error("invalid postfix expression '%s'", expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 468 |     free(copy); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 469 |     return TY_UNKNOWN; | Returns computed value/token/type from helper code. |
| 470 |   } | Closes a semantic action or C code block. |
| 471 |  | Blank line for readability and grouping of grammar/code sections. |
| 472 |   ValueType out = stack[0]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 473 |   free(copy); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 474 |   return out; | Returns computed value/token/type from helper code. |
| 475 | } | Closes a semantic action or C code block. |
| 476 |  | Blank line for readability and grouping of grammar/code sections. |
| 477 | static char* postfix_to_c(const char* expr) { | Static helper declaration or definition used by parser and semantic phase. |
| 478 |     char* copy = xstrdup(expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 479 |     char* stack[512]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 480 |     int top = 0; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 481 |  | Blank line for readability and grouping of grammar/code sections. |
| 482 |   char* tok = strtok(copy, "\t"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 483 |   while (tok) { | Opens a semantic action or C code block. |
| 484 |         if (!is_operator(tok)) { | Opens a semantic action or C code block. |
| 485 |             stack[top++] = xstrdup(tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 486 |       tok = strtok(NULL, "\t"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 487 |       continue; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 488 |         } | Closes a semantic action or C code block. |
| 489 |  | Blank line for readability and grouping of grammar/code sections. |
| 490 |         int arity = op_arity(tok); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 491 |         if (top < arity) { | Opens a semantic action or C code block. |
| 492 |             fprintf(stderr, "Invalid postfix expression: %s\n", expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 493 |             free(copy); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 494 |             return xstrdup("0"); | Returns computed value/token/type from helper code. |
| 495 |         } | Closes a semantic action or C code block. |
| 496 |  | Blank line for readability and grouping of grammar/code sections. |
| 497 |         if (arity == 1) { | Opens a semantic action or C code block. |
| 498 |           char* a = stack[--top]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 499 |           char* merged = str_printf("(%s%s)", tok, a); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 500 |           stack[top++] = merged; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 501 |           free(a); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 502 |           tok = strtok(NULL, "\t"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 503 |           continue; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 504 |         } | Closes a semantic action or C code block. |
| 505 |  | Blank line for readability and grouping of grammar/code sections. |
| 506 |         char* rhs = stack[--top]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 507 |         char* lhs = stack[--top]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 508 |         char* merged = str_printf("(%s %s %s)", lhs, tok, rhs); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 509 |         stack[top++] = merged; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 510 |         free(lhs); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 511 |         free(rhs); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 512 |         tok = strtok(NULL, "\t"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 513 |     } | Closes a semantic action or C code block. |
| 514 |  | Blank line for readability and grouping of grammar/code sections. |
| 515 |     char* out = (top == 1) ? xstrdup(stack[0]) : xstrdup("0"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 516 |     for (int i = 0; i < top; i++) { | Opens a semantic action or C code block. |
| 517 |         free(stack[i]); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 518 |     } | Closes a semantic action or C code block. |
| 519 |     free(copy); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 520 |     return out; | Returns computed value/token/type from helper code. |
| 521 | } | Closes a semantic action or C code block. |
| 522 |  | Blank line for readability and grouping of grammar/code sections. |
| 523 | int yylex(void); | Forward declaration of lexer function invoked by parser. |
| 524 | void yyerror(const char* s); | Parser error function declaration/definition. |
| 525 | const char* get_generated_program(void); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 526 | int has_semantic_errors(void); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 527 | %} | End of Bison C prologue block. |
| 528 |  | Blank line for readability and grouping of grammar/code sections. |
| 529 | %union { | Defines YYSTYPE union members for semantic values. |
| 530 |     char* str; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 531 | } | Closes a semantic action or C code block. |
| 532 |  | Blank line for readability and grouping of grammar/code sections. |
| 533 | %token <str> IDENT NUMBER STRING TYPE | Declares lexer tokens consumed by Bison grammar. |
| 534 | %token IF ELIF ELSE WHILE FOR RETURN PRINT READ SATR BREAK CONTINUE DEF | Declares lexer tokens consumed by Bison grammar. |
| 535 | %token NEWLINE INDENT DEDENT COLON ASSIGN SEMI | Declares lexer tokens consumed by Bison grammar. |
| 536 | %token PLUS MINUS STAR SLASH MOD GT LT GE LE EQ NE AND OR NOT | Declares lexer tokens consumed by Bison grammar. |
| 537 | %token LPAREN RPAREN COMMA | Declares lexer tokens consumed by Bison grammar. |
| 538 |  | Blank line for readability and grouping of grammar/code sections. |
| 539 | %type <str> program top_list top_item func_def func_open | Declares semantic value types for nonterminals. |
| 540 | %type <str> stmt_list stmt simple_stmt opt_expr postfix_expr postfix_item for_init for_cond for_update | Declares semantic value types for nonterminals. |
| 541 | %type <str> opt_elif opt_else nl | Declares semantic value types for nonterminals. |
| 542 | %type <str> param_list_opt param_list param_decl arg_list_opt arg_list arg_item | Declares semantic value types for nonterminals. |
| 543 | %type <str> call_expr | Declares semantic value types for nonterminals. |
| 544 | %type <str> read_targets read_target read_targets_reset | Declares semantic value types for nonterminals. |
| 545 |  | Blank line for readability and grouping of grammar/code sections. |
| 546 | %start program | Declares grammar entry nonterminal. |
| 547 |  | Blank line for readability and grouping of grammar/code sections. |
| 548 | %% | Bison section delimiter between declarations, grammar, and user code. |
| 549 |  | Blank line for readability and grouping of grammar/code sections. |
| 550 | program | Grammar nonterminal label line introducing productions. |
| 551 |     : top_list { | Beginning of production alternatives for current nonterminal. |
| 552 |         generated_program = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 553 |       } | Closes a semantic action or C code block. |
| 554 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 555 |  | Blank line for readability and grouping of grammar/code sections. |
| 556 | top_list | Grammar nonterminal label line introducing productions. |
| 557 |     : top_item { | Beginning of production alternatives for current nonterminal. |
| 558 |         $$ = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 559 |       } | Closes a semantic action or C code block. |
| 560 |     \| top_list top_item { | Alternative grammar production branch. |
| 561 |         char* merged = concat2($1, $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 562 |         free($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 563 |         free($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 564 |         $$ = merged; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 565 |       } | Closes a semantic action or C code block. |
| 566 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 567 |  | Blank line for readability and grouping of grammar/code sections. |
| 568 | top_item | Grammar nonterminal label line introducing productions. |
| 569 |     : func_def { | Beginning of production alternatives for current nonterminal. |
| 570 |         $$ = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 571 |       } | Closes a semantic action or C code block. |
| 572 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 573 |  | Blank line for readability and grouping of grammar/code sections. |
| 574 | func_def | Grammar nonterminal label line introducing productions. |
| 575 |     : func_open INDENT stmt_list DEDENT { | Beginning of production alternatives for current nonterminal. |
| 576 |         char* indented = indent_code($3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 577 |         $$ = str_printf("%s {\n%s}\n\n", $1, indented); | Opens a semantic action or C code block. |
| 578 |         free(indented); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 579 |       } | Closes a semantic action or C code block. |
| 580 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 581 |  | Blank line for readability and grouping of grammar/code sections. |
| 582 | func_open | Grammar nonterminal label line introducing productions. |
| 583 |     : DEF TYPE IDENT LPAREN param_list_opt RPAREN COLON nl { | Beginning of production alternatives for current nonterminal. |
| 584 |         begin_function($2, $3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 585 |         $$ = str_printf("%s %s(%s)", $2, $3, $5); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 586 |       } | Closes a semantic action or C code block. |
| 587 |     \| TYPE IDENT LPAREN param_list_opt RPAREN COLON nl { | Alternative grammar production branch. |
| 588 |         begin_function($1, $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 589 |         $$ = str_printf("%s %s(%s)", $1, $2, $4); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 590 |       } | Closes a semantic action or C code block. |
| 591 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 592 |  | Blank line for readability and grouping of grammar/code sections. |
| 593 | param_list_opt | Grammar nonterminal label line introducing productions. |
| 594 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 595 |         reset_pending_params(); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 596 |         $$ = xstrdup(""); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 597 |       } | Closes a semantic action or C code block. |
| 598 |     \| param_reset param_list { | Alternative grammar production branch. |
| 599 |         $$ = $2; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 600 |       } | Closes a semantic action or C code block. |
| 601 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 602 |  | Blank line for readability and grouping of grammar/code sections. |
| 603 | param_reset | Grammar nonterminal label line introducing productions. |
| 604 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 605 |         reset_pending_params(); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 606 |       } | Closes a semantic action or C code block. |
| 607 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 608 |  | Blank line for readability and grouping of grammar/code sections. |
| 609 | param_list | Grammar nonterminal label line introducing productions. |
| 610 |     : param_decl { | Beginning of production alternatives for current nonterminal. |
| 611 |         $$ = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 612 |       } | Closes a semantic action or C code block. |
| 613 |     \| param_list COMMA param_decl { | Alternative grammar production branch. |
| 614 |         char* merged = str_printf("%s, %s", $1, $3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 615 |         free($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 616 |         free($3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 617 |         $$ = merged; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 618 |       } | Closes a semantic action or C code block. |
| 619 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 620 |  | Blank line for readability and grouping of grammar/code sections. |
| 621 | param_decl | Grammar nonterminal label line introducing productions. |
| 622 |     : TYPE IDENT { | Beginning of production alternatives for current nonterminal. |
| 623 |         if (parse_type_name($1) == TY_VOID) { | Opens a semantic action or C code block. |
| 624 |             semantic_error("parameter '%s' cannot be of type void", $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 625 |         } | Closes a semantic action or C code block. |
| 626 |         push_pending_param($1, $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 627 |         $$ = str_printf("%s %s", $1, $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 628 |       } | Closes a semantic action or C code block. |
| 629 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 630 |  | Blank line for readability and grouping of grammar/code sections. |
| 631 | stmt_list | Grammar nonterminal label line introducing productions. |
| 632 |     : stmt { | Beginning of production alternatives for current nonterminal. |
| 633 |         $$ = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 634 |       } | Closes a semantic action or C code block. |
| 635 |     \| stmt_list stmt { | Alternative grammar production branch. |
| 636 |         char* merged = concat2($1, $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 637 |         free($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 638 |         free($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 639 |         $$ = merged; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 640 |       } | Closes a semantic action or C code block. |
| 641 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 642 |  | Blank line for readability and grouping of grammar/code sections. |
| 643 | stmt | Grammar nonterminal label line introducing productions. |
| 644 |     : simple_stmt { | Beginning of production alternatives for current nonterminal. |
| 645 |         $$ = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 646 |       } | Closes a semantic action or C code block. |
| 647 |     \| IF postfix_expr COLON nl INDENT stmt_list DEDENT opt_elif opt_else { | Alternative grammar production branch. |
| 648 |         infer_postfix_type($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 649 |         char* cond = postfix_to_c($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 650 |         char* if_body = indent_code($6); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 651 |         char* core = str_printf("if (%s) {\n%s}\n", cond, if_body); | Opens a semantic action or C code block. |
| 652 |  | Blank line for readability and grouping of grammar/code sections. |
| 653 |         char* with_elif = concat2(core, $8); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 654 |         char* full = concat2(with_elif, $9); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 655 |  | Blank line for readability and grouping of grammar/code sections. |
| 656 |         free(cond); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 657 |         free(if_body); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 658 |         free(core); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 659 |         free(with_elif); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 660 |         $$ = full; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 661 |       } | Closes a semantic action or C code block. |
| 662 |     \| WHILE postfix_expr COLON nl INDENT loop_enter stmt_list DEDENT loop_exit { | Alternative grammar production branch. |
| 663 |         char* cond = postfix_to_c($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 664 |         infer_postfix_type($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 665 |         char* body = indent_code($7); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 666 |         $$ = str_printf("while (%s) {\n%s}\n", cond, body); | Opens a semantic action or C code block. |
| 667 |         free(cond); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 668 |         free(body); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 669 |       } | Closes a semantic action or C code block. |
| 670 |     \| FOR for_init SEMI for_cond SEMI for_update COLON nl INDENT loop_enter stmt_list DEDENT loop_exit { | Alternative grammar production branch. |
| 671 |         char* body = indent_code($11); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 672 |         $$ = str_printf("for (%s; %s; %s) {\n%s}\n", $2, $4, $6, body); | Opens a semantic action or C code block. |
| 673 |         free(body); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 674 |       } | Closes a semantic action or C code block. |
| 675 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 676 |  | Blank line for readability and grouping of grammar/code sections. |
| 677 | loop_enter | Grammar nonterminal label line introducing productions. |
| 678 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 679 |         loop_depth++; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 680 |       } | Closes a semantic action or C code block. |
| 681 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 682 |  | Blank line for readability and grouping of grammar/code sections. |
| 683 | loop_exit | Grammar nonterminal label line introducing productions. |
| 684 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 685 |         loop_depth--; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 686 |       } | Closes a semantic action or C code block. |
| 687 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 688 |  | Blank line for readability and grouping of grammar/code sections. |
| 689 | for_init | Grammar nonterminal label line introducing productions. |
| 690 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 691 |         $$ = xstrdup(""); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 692 |       } | Closes a semantic action or C code block. |
| 693 |     \| TYPE IDENT { | Alternative grammar production branch. |
| 694 |         ValueType t = parse_type_name($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 695 |         declare_symbol($2, t); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 696 |         $$ = str_printf("%s %s", $1, $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 697 |       } | Closes a semantic action or C code block. |
| 698 |     \| TYPE IDENT postfix_expr ASSIGN { | Alternative grammar production branch. |
| 699 |         ValueType t = parse_type_name($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 700 |         declare_symbol($2, t); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 701 |         ValueType rhs = infer_postfix_type($3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 702 |         if (rhs != TY_UNKNOWN && !can_assign(t, rhs)) { | Opens a semantic action or C code block. |
| 703 |             semantic_error("cannot assign expression to variable '%s'", $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 704 |         } | Closes a semantic action or C code block. |
| 705 |         char* expr = postfix_to_c($3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 706 |         $$ = str_printf("%s %s = %s", $1, $2, expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 707 |         free(expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 708 |       } | Closes a semantic action or C code block. |
| 709 |     \| IDENT postfix_expr ASSIGN { | Alternative grammar production branch. |
| 710 |         ValueType lhs = symbol_type($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 711 |         if (lhs == TY_UNKNOWN) { | Opens a semantic action or C code block. |
| 712 |             semantic_error("assignment to undeclared variable '%s'", $1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 713 |         } | Closes a semantic action or C code block. |
| 714 |         ValueType rhs = infer_postfix_type($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 715 |         if (lhs != TY_UNKNOWN && rhs != TY_UNKNOWN && !can_assign(lhs, rhs)) { | Opens a semantic action or C code block. |
| 716 |             semantic_error("type mismatch in assignment to '%s'", $1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 717 |         } | Closes a semantic action or C code block. |
| 718 |         char* expr = postfix_to_c($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 719 |         $$ = str_printf("%s = %s", $1, expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 720 |         free(expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 721 |       } | Closes a semantic action or C code block. |
| 722 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 723 |  | Blank line for readability and grouping of grammar/code sections. |
| 724 | for_cond | Grammar nonterminal label line introducing productions. |
| 725 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 726 |         $$ = xstrdup("1"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 727 |       } | Closes a semantic action or C code block. |
| 728 |     \| postfix_expr { | Alternative grammar production branch. |
| 729 |         infer_postfix_type($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 730 |         $$ = postfix_to_c($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 731 |       } | Closes a semantic action or C code block. |
| 732 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 733 |  | Blank line for readability and grouping of grammar/code sections. |
| 734 | for_update | Grammar nonterminal label line introducing productions. |
| 735 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 736 |         $$ = xstrdup(""); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 737 |       } | Closes a semantic action or C code block. |
| 738 |     \| IDENT postfix_expr ASSIGN { | Alternative grammar production branch. |
| 739 |         ValueType lhs = symbol_type($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 740 |         if (lhs == TY_UNKNOWN) { | Opens a semantic action or C code block. |
| 741 |             semantic_error("assignment to undeclared variable '%s'", $1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 742 |         } | Closes a semantic action or C code block. |
| 743 |         ValueType rhs = infer_postfix_type($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 744 |         if (lhs != TY_UNKNOWN && rhs != TY_UNKNOWN && !can_assign(lhs, rhs)) { | Opens a semantic action or C code block. |
| 745 |             semantic_error("type mismatch in assignment to '%s'", $1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 746 |         } | Closes a semantic action or C code block. |
| 747 |         char* expr = postfix_to_c($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 748 |         $$ = str_printf("%s = %s", $1, expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 749 |         free(expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 750 |       } | Closes a semantic action or C code block. |
| 751 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 752 |  | Blank line for readability and grouping of grammar/code sections. |
| 753 | opt_elif | Grammar nonterminal label line introducing productions. |
| 754 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 755 |         $$ = xstrdup(""); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 756 |       } | Closes a semantic action or C code block. |
| 757 |     \| ELIF postfix_expr COLON nl INDENT stmt_list DEDENT opt_elif { | Alternative grammar production branch. |
| 758 |       infer_postfix_type($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 759 |         char* cond = postfix_to_c($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 760 |         char* body = indent_code($6); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 761 |         char* current = str_printf("else if (%s) {\n%s}\n", cond, body); | Opens a semantic action or C code block. |
| 762 |         char* full = concat2(current, $8); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 763 |         free(cond); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 764 |         free(body); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 765 |         free(current); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 766 |         $$ = full; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 767 |       } | Closes a semantic action or C code block. |
| 768 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 769 |  | Blank line for readability and grouping of grammar/code sections. |
| 770 | opt_else | Grammar nonterminal label line introducing productions. |
| 771 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 772 |         $$ = xstrdup(""); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 773 |       } | Closes a semantic action or C code block. |
| 774 |     \| ELSE COLON nl INDENT stmt_list DEDENT { | Alternative grammar production branch. |
| 775 |         char* body = indent_code($5); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 776 |         $$ = str_printf("else {\n%s}\n", body); | Opens a semantic action or C code block. |
| 777 |         free(body); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 778 |       } | Closes a semantic action or C code block. |
| 779 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 780 |  | Blank line for readability and grouping of grammar/code sections. |
| 781 | simple_stmt | Grammar nonterminal label line introducing productions. |
| 782 |     : TYPE IDENT nl { | Beginning of production alternatives for current nonterminal. |
| 783 |         ValueType t = parse_type_name($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 784 |         declare_symbol($2, t); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 785 |         $$ = str_printf("%s %s;\n", $1, $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 786 |       } | Closes a semantic action or C code block. |
| 787 |     \| TYPE IDENT postfix_expr ASSIGN nl { | Alternative grammar production branch. |
| 788 |         ValueType t = parse_type_name($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 789 |         declare_symbol($2, t); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 790 |         ValueType rhs = infer_postfix_type($3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 791 |         if (rhs != TY_UNKNOWN && !can_assign(t, rhs)) { | Opens a semantic action or C code block. |
| 792 |             semantic_error("cannot assign expression to variable '%s'", $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 793 |         } | Closes a semantic action or C code block. |
| 794 |         char* expr = postfix_to_c($3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 795 |         $$ = str_printf("%s %s = %s;\n", $1, $2, expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 796 |         free(expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 797 |       } | Closes a semantic action or C code block. |
| 798 |     \| IDENT postfix_expr ASSIGN nl { | Alternative grammar production branch. |
| 799 |         ValueType lhs = symbol_type($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 800 |         if (lhs == TY_UNKNOWN) { | Opens a semantic action or C code block. |
| 801 |             semantic_error("assignment to undeclared variable '%s'", $1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 802 |         } | Closes a semantic action or C code block. |
| 803 |         ValueType rhs = infer_postfix_type($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 804 |         if (lhs != TY_UNKNOWN && rhs != TY_UNKNOWN && !can_assign(lhs, rhs)) { | Opens a semantic action or C code block. |
| 805 |             semantic_error("type mismatch in assignment to '%s'", $1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 806 |         } | Closes a semantic action or C code block. |
| 807 |         char* expr = postfix_to_c($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 808 |         $$ = str_printf("%s = %s;\n", $1, expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 809 |         free(expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 810 |       } | Closes a semantic action or C code block. |
| 811 |     \| RETURN opt_expr nl { | Alternative grammar production branch. |
| 812 |         if ($2[0] == '\0') { | Opens a semantic action or C code block. |
| 813 |             if (current_function_type != TY_VOID) { | Opens a semantic action or C code block. |
| 814 |                 semantic_error("non-void function must return a value"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 815 |             } | Closes a semantic action or C code block. |
| 816 |             $$ = xstrdup("return;\n"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 817 |         } else { | Opens a semantic action or C code block. |
| 818 |             ValueType r = infer_postfix_type($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 819 |             if (current_function_type == TY_VOID) { | Opens a semantic action or C code block. |
| 820 |                 semantic_error("void function cannot return a value"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 821 |             } else if (r != TY_UNKNOWN && !can_assign(current_function_type, r)) { | Opens a semantic action or C code block. |
| 822 |                 semantic_error("return type mismatch"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 823 |             } | Closes a semantic action or C code block. |
| 824 |             char* expr = postfix_to_c($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 825 |             $$ = str_printf("return %s;\n", expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 826 |             free(expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 827 |         } | Closes a semantic action or C code block. |
| 828 |       } | Closes a semantic action or C code block. |
| 829 |     \| BREAK nl { | Alternative grammar production branch. |
| 830 |         if (loop_depth <= 0) { | Opens a semantic action or C code block. |
| 831 |             semantic_error("'qif'/'break' used outside loop"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 832 |         } | Closes a semantic action or C code block. |
| 833 |         $$ = xstrdup("break;\n"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 834 |       } | Closes a semantic action or C code block. |
| 835 |     \| CONTINUE nl { | Alternative grammar production branch. |
| 836 |         if (loop_depth <= 0) { | Opens a semantic action or C code block. |
| 837 |             semantic_error("'wasil'/'continue' used outside loop"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 838 |         } | Closes a semantic action or C code block. |
| 839 |         $$ = xstrdup("continue;\n"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 840 |       } | Closes a semantic action or C code block. |
| 841 |     \| PRINT postfix_expr nl { | Alternative grammar production branch. |
| 842 |       ValueType t = infer_postfix_type($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 843 |         char* expr = postfix_to_c($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 844 |       if (t == TY_DOUBLE) { | Opens a semantic action or C code block. |
| 845 |             $$ = str_printf("printf(\"%%g\", (double)(%s));\n", expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 846 |       } else if (t == TY_INT \|\| t == TY_CHAR) { | Opens a semantic action or C code block. |
| 847 |             $$ = str_printf("printf(\"%%d\", (int)(%s));\n", expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 848 |       } else { | Opens a semantic action or C code block. |
| 849 |             $$ = str_printf("printf(\"%%g\", (double)(%s));\n", expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 850 |       } | Closes a semantic action or C code block. |
| 851 |         free(expr); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 852 |       } | Closes a semantic action or C code block. |
| 853 |     \| PRINT STRING nl { | Alternative grammar production branch. |
| 854 |         $$ = str_printf("printf(\"%%s\", %s);\n", $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 855 |       } | Closes a semantic action or C code block. |
| 856 |     \| READ LPAREN STRING COMMA read_targets_reset read_targets RPAREN nl { | Alternative grammar production branch. |
| 857 |       ValueType specs[64]; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 858 |       int spec_count = parse_scanf_specs($3, specs, 64); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 859 |       if (spec_count < 0) { | Opens a semantic action or C code block. |
| 860 |         semantic_error("invalid or unsupported scanf format %s", $3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 861 |       } else if (spec_count != pending_read_count) { | Opens a semantic action or C code block. |
| 862 |         semantic_error("scanf format expects %d inputs but got %d", spec_count, pending_read_count); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 863 |       } else { | Opens a semantic action or C code block. |
| 864 |         for (int i = 0; i < spec_count; i++) { | Opens a semantic action or C code block. |
| 865 |           if (specs[i] != pending_read_types[i]) { | Opens a semantic action or C code block. |
| 866 |             semantic_error("scanf type mismatch at input #%d", i + 1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 867 |             break; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 868 |           } | Closes a semantic action or C code block. |
| 869 |         } | Closes a semantic action or C code block. |
| 870 |       } | Closes a semantic action or C code block. |
| 871 |       $$ = str_printf("scanf(%s, %s);\n", $3, $6); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 872 |       } | Closes a semantic action or C code block. |
| 873 |     \| SATR nl { | Alternative grammar production branch. |
| 874 |         $$ = xstrdup("printf(\"\\n\");\n"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 875 |       } | Closes a semantic action or C code block. |
| 876 |     \| call_expr nl { | Alternative grammar production branch. |
| 877 |       $$ = str_printf("%s;\n", $1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 878 |       } | Closes a semantic action or C code block. |
| 879 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 880 |  | Blank line for readability and grouping of grammar/code sections. |
| 881 | read_targets_reset | Grammar nonterminal label line introducing productions. |
| 882 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 883 |         reset_pending_read_targets(); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 884 |         $$ = xstrdup(""); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 885 |       } | Closes a semantic action or C code block. |
| 886 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 887 |  | Blank line for readability and grouping of grammar/code sections. |
| 888 | read_targets | Grammar nonterminal label line introducing productions. |
| 889 |     : read_target { | Beginning of production alternatives for current nonterminal. |
| 890 |         $$ = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 891 |       } | Closes a semantic action or C code block. |
| 892 |     \| read_targets COMMA read_target { | Alternative grammar production branch. |
| 893 |         char* merged = str_printf("%s, %s", $1, $3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 894 |         free($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 895 |         free($3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 896 |         $$ = merged; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 897 |       } | Closes a semantic action or C code block. |
| 898 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 899 |  | Blank line for readability and grouping of grammar/code sections. |
| 900 | read_target | Grammar nonterminal label line introducing productions. |
| 901 |     : IDENT { | Beginning of production alternatives for current nonterminal. |
| 902 |         ValueType vt = symbol_type($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 903 |         if (vt == TY_UNKNOWN) { | Opens a semantic action or C code block. |
| 904 |             semantic_error("input target '%s' is undeclared", $1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 905 |         } else { | Opens a semantic action or C code block. |
| 906 |             push_pending_read_target(vt, $1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 907 |         } | Closes a semantic action or C code block. |
| 908 |         $$ = str_printf("&%s", $1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 909 |       } | Closes a semantic action or C code block. |
| 910 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 911 |  | Blank line for readability and grouping of grammar/code sections. |
| 912 |     call_expr | Grammar nonterminal label line introducing productions. |
| 913 |       : IDENT LPAREN arg_list_opt RPAREN { | Beginning of production alternatives for current nonterminal. |
| 914 |         int fi = function_index($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 915 |         last_call_expr_type = TY_UNKNOWN; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 916 |         if (fi < 0) { | Opens a semantic action or C code block. |
| 917 |           semantic_error("call to undefined function '%s'", $1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 918 |         } else { | Opens a semantic action or C code block. |
| 919 |           if (functions[fi].arg_count != pending_call_arg_count) { | Opens a semantic action or C code block. |
| 920 |             semantic_error("function '%s' expects %d args but got %d", $1, functions[fi].arg_count, pending_call_arg_count); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 921 |           } else { | Opens a semantic action or C code block. |
| 922 |             for (int i = 0; i < pending_call_arg_count; i++) { | Opens a semantic action or C code block. |
| 923 |               if (!can_assign(functions[fi].arg_types[i], pending_call_arg_types[i])) { | Opens a semantic action or C code block. |
| 924 |                 semantic_error("type mismatch for arg %d in call to '%s'", i + 1, $1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 925 |                 break; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 926 |               } | Closes a semantic action or C code block. |
| 927 |             } | Closes a semantic action or C code block. |
| 928 |             last_call_expr_type = functions[fi].ret_type; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 929 |           } | Closes a semantic action or C code block. |
| 930 |         } | Closes a semantic action or C code block. |
| 931 |         $$ = str_printf("%s(%s)", $1, $3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 932 |         } | Closes a semantic action or C code block. |
| 933 |       ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 934 |  | Blank line for readability and grouping of grammar/code sections. |
| 935 |   arg_list_opt | Grammar nonterminal label line introducing productions. |
| 936 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 937 |       reset_pending_call_args(); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 938 |       $$ = xstrdup(""); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 939 |       } | Closes a semantic action or C code block. |
| 940 |       \| arg_reset arg_list { | Alternative grammar production branch. |
| 941 |           $$ = $2; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 942 |       } | Closes a semantic action or C code block. |
| 943 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 944 |  | Blank line for readability and grouping of grammar/code sections. |
| 945 |   arg_reset | Grammar nonterminal label line introducing productions. |
| 946 |       : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 947 |           reset_pending_call_args(); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 948 |         } | Closes a semantic action or C code block. |
| 949 |       ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 950 |  | Blank line for readability and grouping of grammar/code sections. |
| 951 |   arg_list | Grammar nonterminal label line introducing productions. |
| 952 |     : arg_item { | Beginning of production alternatives for current nonterminal. |
| 953 |       $$ = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 954 |       } | Closes a semantic action or C code block. |
| 955 |     \| arg_list COMMA arg_item { | Alternative grammar production branch. |
| 956 |       char* merged = str_printf("%s,%s", $1, $3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 957 |       free($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 958 |       free($3); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 959 |       $$ = merged; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 960 |       } | Closes a semantic action or C code block. |
| 961 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 962 |  | Blank line for readability and grouping of grammar/code sections. |
| 963 |   arg_item | Grammar nonterminal label line introducing productions. |
| 964 |     : postfix_expr { | Beginning of production alternatives for current nonterminal. |
| 965 |       ValueType t = infer_postfix_type($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 966 |       push_pending_call_arg(t); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 967 |       $$ = postfix_to_c($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 968 |       } | Closes a semantic action or C code block. |
| 969 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 970 |  | Blank line for readability and grouping of grammar/code sections. |
| 971 | opt_expr | Grammar nonterminal label line introducing productions. |
| 972 |     : /* empty */ { | Beginning of production alternatives for current nonterminal. |
| 973 |         $$ = xstrdup(""); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 974 |       } | Closes a semantic action or C code block. |
| 975 |     \| postfix_expr { | Alternative grammar production branch. |
| 976 |         $$ = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 977 |       } | Closes a semantic action or C code block. |
| 978 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 979 |  | Blank line for readability and grouping of grammar/code sections. |
| 980 | postfix_expr | Grammar nonterminal label line introducing productions. |
| 981 |     : postfix_item { | Beginning of production alternatives for current nonterminal. |
| 982 |         $$ = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 983 |       } | Closes a semantic action or C code block. |
| 984 |     \| postfix_expr postfix_item { | Alternative grammar production branch. |
| 985 |         char* merged = concat_space($1, $2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 986 |         free($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 987 |         free($2); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 988 |         $$ = merged; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 989 |       } | Closes a semantic action or C code block. |
| 990 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 991 |  | Blank line for readability and grouping of grammar/code sections. |
| 992 | postfix_item | Grammar nonterminal label line introducing productions. |
| 993 |     : IDENT { | Beginning of production alternatives for current nonterminal. |
| 994 |         $$ = xstrdup($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 995 |       } | Closes a semantic action or C code block. |
| 996 |     \| NUMBER { | Alternative grammar production branch. |
| 997 |         $$ = xstrdup($1); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 998 |       } | Closes a semantic action or C code block. |
| 999 |     \| call_expr { | Alternative grammar production branch. |
| 1000 |         $$ = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 1001 |       } | Closes a semantic action or C code block. |
| 1002 |     \| PLUS { | Alternative grammar production branch. |
| 1003 |         $$ = xstrdup("+"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 1004 |       } | Closes a semantic action or C code block. |
| 1005 |     \| MINUS { | Alternative grammar production branch. |
| 1006 |         $$ = xstrdup("-"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 1007 |       } | Closes a semantic action or C code block. |
| 1008 |     \| STAR { | Alternative grammar production branch. |
| 1009 |         $$ = xstrdup("*"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 1010 |       } | Closes a semantic action or C code block. |
| 1011 |     \| SLASH { | Alternative grammar production branch. |
| 1012 |         $$ = xstrdup("/"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 1013 |       } | Closes a semantic action or C code block. |
| 1014 |     \| MOD { | Alternative grammar production branch. |
| 1015 |         $$ = xstrdup("%"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 1016 |       } | Closes a semantic action or C code block. |
| 1017 |     \| GT { | Alternative grammar production branch. |
| 1018 |         $$ = xstrdup(">"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 1019 |       } | Closes a semantic action or C code block. |
| 1020 |     \| LT { | Alternative grammar production branch. |
| 1021 |         $$ = xstrdup("<"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 1022 |       } | Closes a semantic action or C code block. |
| 1023 |     \| GE { | Alternative grammar production branch. |
| 1024 |         $$ = xstrdup(">="); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 1025 |       } | Closes a semantic action or C code block. |
| 1026 |     \| LE { | Alternative grammar production branch. |
| 1027 |         $$ = xstrdup("<="); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 1028 |       } | Closes a semantic action or C code block. |
| 1029 |     \| EQ { | Alternative grammar production branch. |
| 1030 |         $$ = xstrdup("=="); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 1031 |       } | Closes a semantic action or C code block. |
| 1032 |     \| NE { | Alternative grammar production branch. |
| 1033 |         $$ = xstrdup("!="); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 1034 |       } | Closes a semantic action or C code block. |
| 1035 |     \| AND { | Alternative grammar production branch. |
| 1036 |         $$ = xstrdup("&&"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 1037 |       } | Closes a semantic action or C code block. |
| 1038 |     \| OR { | Alternative grammar production branch. |
| 1039 |         $$ = xstrdup("\|\|"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 1040 |       } | Closes a semantic action or C code block. |
| 1041 |     \| NOT { | Alternative grammar production branch. |
| 1042 |         $$ = xstrdup("!"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 1043 |       } | Closes a semantic action or C code block. |
| 1044 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 1045 |  | Blank line for readability and grouping of grammar/code sections. |
| 1046 | nl | Grammar nonterminal label line introducing productions. |
| 1047 |     : NEWLINE { | Beginning of production alternatives for current nonterminal. |
| 1048 |         $$ = xstrdup("\n"); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 1049 |       } | Closes a semantic action or C code block. |
| 1050 |     \| nl NEWLINE { | Alternative grammar production branch. |
| 1051 |         $$ = $1; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 1052 |       } | Closes a semantic action or C code block. |
| 1053 |     ; | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 1054 |  | Blank line for readability and grouping of grammar/code sections. |
| 1055 | %% | Bison section delimiter between declarations, grammar, and user code. |
| 1056 |  | Blank line for readability and grouping of grammar/code sections. |
| 1057 | void yyerror(const char* s) { | Parser error function declaration/definition. |
| 1058 |   fprintf(stderr, "Parse error (line %d): %s\n", yylineno, s); | Parser implementation/grammar line used for code generation, semantic checks, or rule definition. |
| 1059 | } | Closes a semantic action or C code block. |
| 1060 |  | Blank line for readability and grouping of grammar/code sections. |
| 1061 | const char* get_generated_program(void) { | Opens a semantic action or C code block. |
| 1062 |     return generated_program; | Returns computed value/token/type from helper code. |
| 1063 | } | Closes a semantic action or C code block. |
| 1064 |  | Blank line for readability and grouping of grammar/code sections. |
| 1065 | int has_semantic_errors(void) { | Opens a semantic action or C code block. |
| 1066 |   return semantic_errors > 0; | Returns computed value/token/type from helper code. |
| 1067 | } | Closes a semantic action or C code block. |
