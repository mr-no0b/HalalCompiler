%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

static char* generated_program = NULL;
static int semantic_errors = 0;
extern int yylineno;

typedef enum {
  TY_INT,
  TY_DOUBLE,
  TY_CHAR,
  TY_VOID,
  TY_UNKNOWN
} ValueType;

typedef struct {
  char name[64];
  ValueType type;
} Symbol;

typedef struct {
  char name[64];
  ValueType ret_type;
  int arg_count;
  ValueType arg_types[64];
} FunctionInfo;

static Symbol symbols[1024];
static int symbol_count = 0;

static FunctionInfo functions[256];
static int function_count = 0;

static ValueType current_function_type = TY_UNKNOWN;
static int loop_depth = 0;
extern int yylineno;

static int pending_param_count = 0;
static ValueType pending_param_types[64];
static char pending_param_names[64][64];

static int pending_call_arg_count = 0;
static ValueType pending_call_arg_types[64];
static ValueType last_call_expr_type = TY_UNKNOWN;
static int pending_read_count = 0;
static ValueType pending_read_types[64];

static char* xstrdup(const char* s) {
    size_t n = strlen(s) + 1;
    char* out = (char*)malloc(n);
    if (!out) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    memcpy(out, s, n);
    return out;
}

static char* str_printf(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);

    if (n < 0) {
        fprintf(stderr, "Formatting error\n");
        exit(1);
    }

    char* out = (char*)malloc((size_t)n + 1);
    if (!out) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }

    va_start(ap, fmt);
    vsnprintf(out, (size_t)n + 1, fmt, ap);
    va_end(ap);
    return out;
}

static char* concat2(const char* a, const char* b) {
    size_t na = strlen(a);
    size_t nb = strlen(b);
    char* out = (char*)malloc(na + nb + 1);
    if (!out) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    memcpy(out, a, na);
    memcpy(out + na, b, nb);
    out[na + nb] = '\0';
    return out;
}

static char* concat_space(const char* a, const char* b) {
    if (a[0] == '\0') {
        return xstrdup(b);
    }
  return str_printf("%s\t%s", a, b);
}

static char* indent_code(const char* code) {
    size_t n = strlen(code);
    char* out = (char*)malloc(n * 2 + 64);
    if (!out) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }

    size_t j = 0;
    int at_line_start = 1;
    for (size_t i = 0; i < n; i++) {
        if (at_line_start && code[i] != '\n') {
            out[j++] = ' ';
            out[j++] = ' ';
            out[j++] = ' ';
            out[j++] = ' ';
            at_line_start = 0;
        }
        out[j++] = code[i];
        if (code[i] == '\n') {
            at_line_start = 1;
        }
    }
    out[j] = '\0';
    return out;
}

static void semantic_error(const char* fmt, ...) {
  semantic_errors++;
  fprintf(stderr, "Semantic error (line %d): ", yylineno);
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  fprintf(stderr, "\n");
}

static ValueType parse_type_name(const char* t) {
  if (strcmp(t, "int") == 0) return TY_INT;
  if (strcmp(t, "double") == 0) return TY_DOUBLE;
  if (strcmp(t, "char") == 0) return TY_CHAR;
  if (strcmp(t, "void") == 0) return TY_VOID;
  return TY_UNKNOWN;
}

static void clear_symbols(void) {
  symbol_count = 0;
}

static int symbol_index(const char* name) {
  for (int i = symbol_count - 1; i >= 0; i--) {
    if (strcmp(symbols[i].name, name) == 0) {
      return i;
    }
  }
  return -1;
}

static ValueType symbol_type(const char* name) {
  int i = symbol_index(name);
  if (i < 0) return TY_UNKNOWN;
  return symbols[i].type;
}

static int declare_symbol(const char* name, ValueType type) {
  if (symbol_index(name) >= 0) {
    semantic_error("redeclaration of variable '%s'", name);
    return 0;
  }
  if (symbol_count >= 1024) {
    semantic_error("too many symbols");
    return 0;
  }
  strncpy(symbols[symbol_count].name, name, sizeof(symbols[symbol_count].name) - 1);
  symbols[symbol_count].name[sizeof(symbols[symbol_count].name) - 1] = '\0';
  symbols[symbol_count].type = type;
  symbol_count++;
  return 1;
}

static int function_index(const char* name) {
  for (int i = 0; i < function_count; i++) {
    if (strcmp(functions[i].name, name) == 0) return i;
  }
  return -1;
}

static int function_exists(const char* name) {
  return function_index(name) >= 0;
}

static void reset_pending_params(void) {
  pending_param_count = 0;
}

static void push_pending_param(const char* type_name, const char* param_name) {
  if (pending_param_count >= 64) {
    semantic_error("too many function parameters");
    return;
  }
  pending_param_types[pending_param_count] = parse_type_name(type_name);
  strncpy(pending_param_names[pending_param_count], param_name, sizeof(pending_param_names[pending_param_count]) - 1);
  pending_param_names[pending_param_count][sizeof(pending_param_names[pending_param_count]) - 1] = '\0';
  pending_param_count++;
}

static void reset_pending_call_args(void) {
  pending_call_arg_count = 0;
}

static void push_pending_call_arg(ValueType t) {
  if (pending_call_arg_count >= 64) {
    semantic_error("too many function call arguments");
    return;
  }
  pending_call_arg_types[pending_call_arg_count++] = t;
}

static void reset_pending_read_targets(void) {
  pending_read_count = 0;
}

static void push_pending_read_target(ValueType t, const char* name) {
  if (pending_read_count >= 64) {
    semantic_error("too many input targets in iqra/scanf");
    return;
  }
  pending_read_types[pending_read_count++] = t;
  (void)name;
}

static int parse_scanf_specs(const char* fmt, ValueType out_specs[], int max_specs) {
  if (!fmt) return -1;
  size_t n = strlen(fmt);
  if (n < 2 || fmt[0] != '"' || fmt[n - 1] != '"') return -1;

  int count = 0;
  for (size_t i = 1; i + 1 < n; i++) {
    if (fmt[i] != '%') continue;

    if (i + 1 < n && fmt[i + 1] == '%') {
      i++;
      continue;
    }

    size_t j = i + 1;
    while (j + 1 < n && strchr(" +-#0", fmt[j])) j++;
    while (j + 1 < n && isdigit((unsigned char)fmt[j])) j++;

    if (j + 1 < n && fmt[j] == 'l' && (fmt[j + 1] == 'f' || fmt[j + 1] == 'g' || fmt[j + 1] == 'e')) {
      if (count < max_specs) out_specs[count] = TY_DOUBLE;
      count++;
      i = j + 1;
      continue;
    }

    if (j + 1 < n && (fmt[j] == 'd' || fmt[j] == 'i')) {
      if (count < max_specs) out_specs[count] = TY_INT;
      count++;
      i = j;
      continue;
    }

    if (j + 1 < n && fmt[j] == 'c') {
      if (count < max_specs) out_specs[count] = TY_CHAR;
      count++;
      i = j;
      continue;
    }

    return -1;
  }
  return count;
}

static void register_function(const char* name, ValueType ret_type) {
  if (function_exists(name)) {
    semantic_error("redefinition of function '%s'", name);
    return;
  }
  if (function_count >= 256) {
    semantic_error("too many functions");
    return;
  }
  strncpy(functions[function_count].name, name, sizeof(functions[function_count].name) - 1);
  functions[function_count].name[sizeof(functions[function_count].name) - 1] = '\0';
  functions[function_count].ret_type = ret_type;
  functions[function_count].arg_count = pending_param_count;
  for (int i = 0; i < pending_param_count; i++) {
    functions[function_count].arg_types[i] = pending_param_types[i];
  }
  function_count++;
}

static void begin_function(const char* type_name, const char* name) {
  current_function_type = parse_type_name(type_name);
  clear_symbols();
  register_function(name, current_function_type);
  for (int i = 0; i < pending_param_count; i++) {
    declare_symbol(pending_param_names[i], pending_param_types[i]);
  }
}

static int is_number_token(const char* tok) {
  if (!tok || !*tok) return 0;
  int has_dot = 0;
  int i = 0;
  if (tok[0] == '-') i = 1;
  for (; tok[i]; i++) {
    if (tok[i] == '.') {
      if (has_dot) return 0;
      has_dot = 1;
      continue;
    }
    if (!isdigit((unsigned char)tok[i])) return 0;
  }
  return i > 0;
}

static int is_identifier_token(const char* tok) {
  if (!tok || !*tok) return 0;
  if (!(isalpha((unsigned char)tok[0]) || tok[0] == '_')) return 0;
  for (int i = 1; tok[i]; i++) {
    if (!(isalnum((unsigned char)tok[i]) || tok[i] == '_')) return 0;
  }
  return 1;
}

static int is_operator(const char* tok) {
    return strcmp(tok, "+") == 0 || strcmp(tok, "-") == 0 || strcmp(tok, "*") == 0 ||
       strcmp(tok, "/") == 0 || strcmp(tok, "%") == 0 || strcmp(tok, ">") == 0 || strcmp(tok, "<") == 0 ||
           strcmp(tok, ">=") == 0 || strcmp(tok, "<=") == 0 || strcmp(tok, "==") == 0 ||
       strcmp(tok, "!=") == 0 || strcmp(tok, "&&") == 0 || strcmp(tok, "||") == 0 ||
       strcmp(tok, "!") == 0;
}

static int is_call_token(const char* tok) {
  const char* lp = strchr(tok, '(');
  size_t n = strlen(tok);
  return lp != NULL && n > 2 && tok[n - 1] == ')';
}

static ValueType call_return_type_from_token(const char* tok) {
  const char* lp = strchr(tok, '(');
  if (!lp) return TY_UNKNOWN;
  size_t len = (size_t)(lp - tok);
  if (len == 0 || len >= 64) return TY_UNKNOWN;
  char name[64];
  memcpy(name, tok, len);
  name[len] = '\0';
  int fi = function_index(name);
  if (fi < 0) {
    semantic_error("call to undefined function '%s'", name);
    return TY_UNKNOWN;
  }
  return functions[fi].ret_type;
}

static int op_arity(const char* tok) {
  if (strcmp(tok, "!") == 0) return 1;
  return 2;
}

static int is_numeric_type(ValueType t) {
  return t == TY_INT || t == TY_DOUBLE || t == TY_CHAR;
}

static int can_assign(ValueType lhs, ValueType rhs) {
  if (lhs == TY_UNKNOWN || rhs == TY_UNKNOWN) return 0;
  if (lhs == TY_VOID || rhs == TY_VOID) return 0;
  if (lhs == rhs) return 1;
  return is_numeric_type(lhs) && is_numeric_type(rhs);
}

static ValueType number_type(const char* tok) {
  return strchr(tok, '.') ? TY_DOUBLE : TY_INT;
}

static ValueType infer_postfix_type(const char* expr) {
  char* copy = xstrdup(expr);
  ValueType stack[512];
  int top = 0;

  char* tok = strtok(copy, "\t");
  while (tok) {
    if (!is_operator(tok)) {
      if (is_number_token(tok)) {
        stack[top++] = number_type(tok);
      } else if (tok[0] == '"') {
        stack[top++] = TY_CHAR;
      } else if (is_call_token(tok)) {
        ValueType rt = call_return_type_from_token(tok);
        if (rt == TY_UNKNOWN) {
          free(copy);
          return TY_UNKNOWN;
        }
        stack[top++] = rt;
      } else if (is_identifier_token(tok)) {
        ValueType t = symbol_type(tok);
        if (t == TY_UNKNOWN) {
          semantic_error("use of undeclared variable '%s'", tok);
          free(copy);
          return TY_UNKNOWN;
        }
        stack[top++] = t;
      } else {
        semantic_error("invalid token in expression '%s'", tok);
        free(copy);
        return TY_UNKNOWN;
      }
      tok = strtok(NULL, "\t");
      continue;
    }

    int arity = op_arity(tok);
    if (top < arity) {
      semantic_error("invalid postfix expression '%s'", expr);
      free(copy);
      return TY_UNKNOWN;
    }

    if (arity == 1) {
      ValueType a = stack[--top];
      if (!is_numeric_type(a)) {
        semantic_error("operator '%s' requires numeric operand", tok);
        free(copy);
        return TY_UNKNOWN;
      }
      stack[top++] = TY_INT;
      tok = strtok(NULL, "\t");
      continue;
    }

    ValueType rhs = stack[--top];
    ValueType lhs = stack[--top];
    if (!is_numeric_type(lhs) || !is_numeric_type(rhs)) {
      semantic_error("operator '%s' requires numeric operands", tok);
      free(copy);
      return TY_UNKNOWN;
    }

    if (strcmp(tok, "%") == 0) {
      if (lhs == TY_DOUBLE || rhs == TY_DOUBLE) {
        semantic_error("operator '%%' does not support double operands");
        free(copy);
        return TY_UNKNOWN;
      }
      stack[top++] = TY_INT;
    } else if (strcmp(tok, ">") == 0 || strcmp(tok, "<") == 0 || strcmp(tok, ">=") == 0 ||
           strcmp(tok, "<=") == 0 || strcmp(tok, "==") == 0 || strcmp(tok, "!=") == 0 ||
           strcmp(tok, "&&") == 0 || strcmp(tok, "||") == 0) {
      stack[top++] = TY_INT;
    } else {
      stack[top++] = (lhs == TY_DOUBLE || rhs == TY_DOUBLE) ? TY_DOUBLE : TY_INT;
    }

    tok = strtok(NULL, "\t");
  }

  if (top != 1) {
    semantic_error("invalid postfix expression '%s'", expr);
    free(copy);
    return TY_UNKNOWN;
  }

  ValueType out = stack[0];
  free(copy);
  return out;
}

static char* postfix_to_c(const char* expr) {
    char* copy = xstrdup(expr);
    char* stack[512];
    int top = 0;

  char* tok = strtok(copy, "\t");
  while (tok) {
        if (!is_operator(tok)) {
            stack[top++] = xstrdup(tok);
      tok = strtok(NULL, "\t");
      continue;
        }

        int arity = op_arity(tok);
        if (top < arity) {
            fprintf(stderr, "Invalid postfix expression: %s\n", expr);
            free(copy);
            return xstrdup("0");
        }

        if (arity == 1) {
          char* a = stack[--top];
          char* merged = str_printf("(%s%s)", tok, a);
          stack[top++] = merged;
          free(a);
          tok = strtok(NULL, "\t");
          continue;
        }

        char* rhs = stack[--top];
        char* lhs = stack[--top];
        char* merged = str_printf("(%s %s %s)", lhs, tok, rhs);
        stack[top++] = merged;
        free(lhs);
        free(rhs);
        tok = strtok(NULL, "\t");
    }

    char* out = (top == 1) ? xstrdup(stack[0]) : xstrdup("0");
    for (int i = 0; i < top; i++) {
        free(stack[i]);
    }
    free(copy);
    return out;
}

int yylex(void);
void yyerror(const char* s);
const char* get_generated_program(void);
int has_semantic_errors(void);
%}

%union {
    char* str;
}

%token <str> IDENT NUMBER STRING TYPE
%token IF ELIF ELSE WHILE FOR RETURN PRINT READ SATR BREAK CONTINUE DEF
%token NEWLINE INDENT DEDENT COLON ASSIGN SEMI
%token PLUS MINUS STAR SLASH MOD GT LT GE LE EQ NE AND OR NOT
%token LPAREN RPAREN COMMA

%type <str> program top_list top_item func_def func_open
%type <str> stmt_list stmt simple_stmt opt_expr postfix_expr postfix_item for_init for_cond for_update
%type <str> opt_elif opt_else nl
%type <str> param_list_opt param_list param_decl arg_list_opt arg_list arg_item
%type <str> call_expr
%type <str> read_targets read_target read_targets_reset

%start program

%%

program
    : top_list {
        generated_program = $1;
      }
    ;

top_list
    : top_item {
        $$ = $1;
      }
    | top_list top_item {
        char* merged = concat2($1, $2);
        free($1);
        free($2);
        $$ = merged;
      }
    ;

top_item
    : func_def {
        $$ = $1;
      }
    ;

func_def
    : func_open INDENT stmt_list DEDENT {
        char* indented = indent_code($3);
        $$ = str_printf("%s {\n%s}\n\n", $1, indented);
        free(indented);
      }
    ;

func_open
    : DEF TYPE IDENT LPAREN param_list_opt RPAREN COLON nl {
        begin_function($2, $3);
        $$ = str_printf("%s %s(%s)", $2, $3, $5);
      }
    | TYPE IDENT LPAREN param_list_opt RPAREN COLON nl {
        begin_function($1, $2);
        $$ = str_printf("%s %s(%s)", $1, $2, $4);
      }
    ;

param_list_opt
    : /* empty */ {
        reset_pending_params();
        $$ = xstrdup("");
      }
    | param_reset param_list {
        $$ = $2;
      }
    ;

param_reset
    : /* empty */ {
        reset_pending_params();
      }
    ;

param_list
    : param_decl {
        $$ = $1;
      }
    | param_list COMMA param_decl {
        char* merged = str_printf("%s, %s", $1, $3);
        free($1);
        free($3);
        $$ = merged;
      }
    ;

param_decl
    : TYPE IDENT {
        if (parse_type_name($1) == TY_VOID) {
            semantic_error("parameter '%s' cannot be of type void", $2);
        }
        push_pending_param($1, $2);
        $$ = str_printf("%s %s", $1, $2);
      }
    ;

stmt_list
    : stmt {
        $$ = $1;
      }
    | stmt_list stmt {
        char* merged = concat2($1, $2);
        free($1);
        free($2);
        $$ = merged;
      }
    ;

stmt
    : simple_stmt {
        $$ = $1;
      }
    | IF postfix_expr COLON nl INDENT stmt_list DEDENT opt_elif opt_else {
        infer_postfix_type($2);
        char* cond = postfix_to_c($2);
        char* if_body = indent_code($6);
        char* core = str_printf("if (%s) {\n%s}\n", cond, if_body);

        char* with_elif = concat2(core, $8);
        char* full = concat2(with_elif, $9);

        free(cond);
        free(if_body);
        free(core);
        free(with_elif);
        $$ = full;
      }
    | WHILE postfix_expr COLON nl INDENT loop_enter stmt_list DEDENT loop_exit {
        char* cond = postfix_to_c($2);
        infer_postfix_type($2);
        char* body = indent_code($7);
        $$ = str_printf("while (%s) {\n%s}\n", cond, body);
        free(cond);
        free(body);
      }
    | FOR for_init SEMI for_cond SEMI for_update COLON nl INDENT loop_enter stmt_list DEDENT loop_exit {
        char* body = indent_code($11);
        $$ = str_printf("for (%s; %s; %s) {\n%s}\n", $2, $4, $6, body);
        free(body);
      }
    ;

loop_enter
    : /* empty */ {
        loop_depth++;
      }
    ;

loop_exit
    : /* empty */ {
        loop_depth--;
      }
    ;

for_init
    : /* empty */ {
        $$ = xstrdup("");
      }
    | TYPE IDENT {
        ValueType t = parse_type_name($1);
        declare_symbol($2, t);
        $$ = str_printf("%s %s", $1, $2);
      }
    | TYPE IDENT postfix_expr ASSIGN {
        ValueType t = parse_type_name($1);
        declare_symbol($2, t);
        ValueType rhs = infer_postfix_type($3);
        if (rhs != TY_UNKNOWN && !can_assign(t, rhs)) {
            semantic_error("cannot assign expression to variable '%s'", $2);
        }
        char* expr = postfix_to_c($3);
        $$ = str_printf("%s %s = %s", $1, $2, expr);
        free(expr);
      }
    | IDENT postfix_expr ASSIGN {
        ValueType lhs = symbol_type($1);
        if (lhs == TY_UNKNOWN) {
            semantic_error("assignment to undeclared variable '%s'", $1);
        }
        ValueType rhs = infer_postfix_type($2);
        if (lhs != TY_UNKNOWN && rhs != TY_UNKNOWN && !can_assign(lhs, rhs)) {
            semantic_error("type mismatch in assignment to '%s'", $1);
        }
        char* expr = postfix_to_c($2);
        $$ = str_printf("%s = %s", $1, expr);
        free(expr);
      }
    ;

for_cond
    : /* empty */ {
        $$ = xstrdup("1");
      }
    | postfix_expr {
        infer_postfix_type($1);
        $$ = postfix_to_c($1);
      }
    ;

for_update
    : /* empty */ {
        $$ = xstrdup("");
      }
    | IDENT postfix_expr ASSIGN {
        ValueType lhs = symbol_type($1);
        if (lhs == TY_UNKNOWN) {
            semantic_error("assignment to undeclared variable '%s'", $1);
        }
        ValueType rhs = infer_postfix_type($2);
        if (lhs != TY_UNKNOWN && rhs != TY_UNKNOWN && !can_assign(lhs, rhs)) {
            semantic_error("type mismatch in assignment to '%s'", $1);
        }
        char* expr = postfix_to_c($2);
        $$ = str_printf("%s = %s", $1, expr);
        free(expr);
      }
    ;

opt_elif
    : /* empty */ {
        $$ = xstrdup("");
      }
    | ELIF postfix_expr COLON nl INDENT stmt_list DEDENT opt_elif {
      infer_postfix_type($2);
        char* cond = postfix_to_c($2);
        char* body = indent_code($6);
        char* current = str_printf("else if (%s) {\n%s}\n", cond, body);
        char* full = concat2(current, $8);
        free(cond);
        free(body);
        free(current);
        $$ = full;
      }
    ;

opt_else
    : /* empty */ {
        $$ = xstrdup("");
      }
    | ELSE COLON nl INDENT stmt_list DEDENT {
        char* body = indent_code($5);
        $$ = str_printf("else {\n%s}\n", body);
        free(body);
      }
    ;

simple_stmt
    : TYPE IDENT nl {
        ValueType t = parse_type_name($1);
        declare_symbol($2, t);
        $$ = str_printf("%s %s;\n", $1, $2);
      }
    | TYPE IDENT postfix_expr ASSIGN nl {
        ValueType t = parse_type_name($1);
        declare_symbol($2, t);
        ValueType rhs = infer_postfix_type($3);
        if (rhs != TY_UNKNOWN && !can_assign(t, rhs)) {
            semantic_error("cannot assign expression to variable '%s'", $2);
        }
        char* expr = postfix_to_c($3);
        $$ = str_printf("%s %s = %s;\n", $1, $2, expr);
        free(expr);
      }
    | IDENT postfix_expr ASSIGN nl {
        ValueType lhs = symbol_type($1);
        if (lhs == TY_UNKNOWN) {
            semantic_error("assignment to undeclared variable '%s'", $1);
        }
        ValueType rhs = infer_postfix_type($2);
        if (lhs != TY_UNKNOWN && rhs != TY_UNKNOWN && !can_assign(lhs, rhs)) {
            semantic_error("type mismatch in assignment to '%s'", $1);
        }
        char* expr = postfix_to_c($2);
        $$ = str_printf("%s = %s;\n", $1, expr);
        free(expr);
      }
    | RETURN opt_expr nl {
        if ($2[0] == '\0') {
            if (current_function_type != TY_VOID) {
                semantic_error("non-void function must return a value");
            }
            $$ = xstrdup("return;\n");
        } else {
            ValueType r = infer_postfix_type($2);
            if (current_function_type == TY_VOID) {
                semantic_error("void function cannot return a value");
            } else if (r != TY_UNKNOWN && !can_assign(current_function_type, r)) {
                semantic_error("return type mismatch");
            }
            char* expr = postfix_to_c($2);
            $$ = str_printf("return %s;\n", expr);
            free(expr);
        }
      }
    | BREAK nl {
        if (loop_depth <= 0) {
            semantic_error("'qif'/'break' used outside loop");
        }
        $$ = xstrdup("break;\n");
      }
    | CONTINUE nl {
        if (loop_depth <= 0) {
            semantic_error("'wasil'/'continue' used outside loop");
        }
        $$ = xstrdup("continue;\n");
      }
    | PRINT postfix_expr nl {
      ValueType t = infer_postfix_type($2);
        char* expr = postfix_to_c($2);
      if (t == TY_DOUBLE) {
            $$ = str_printf("printf(\"%%g\", (double)(%s));\n", expr);
      } else if (t == TY_INT || t == TY_CHAR) {
            $$ = str_printf("printf(\"%%d\", (int)(%s));\n", expr);
      } else {
            $$ = str_printf("printf(\"%%g\", (double)(%s));\n", expr);
      }
        free(expr);
      }
    | PRINT STRING nl {
        $$ = str_printf("printf(\"%%s\", %s);\n", $2);
      }
    | READ LPAREN STRING COMMA read_targets_reset read_targets RPAREN nl {
      ValueType specs[64];
      int spec_count = parse_scanf_specs($3, specs, 64);
      if (spec_count < 0) {
        semantic_error("invalid or unsupported scanf format %s", $3);
      } else if (spec_count != pending_read_count) {
        semantic_error("scanf format expects %d inputs but got %d", spec_count, pending_read_count);
      } else {
        for (int i = 0; i < spec_count; i++) {
          if (specs[i] != pending_read_types[i]) {
            semantic_error("scanf type mismatch at input #%d", i + 1);
            break;
          }
        }
      }
      $$ = str_printf("scanf(%s, %s);\n", $3, $6);
      }
    | SATR nl {
        $$ = xstrdup("printf(\"\\n\");\n");
      }
    | call_expr nl {
      $$ = str_printf("%s;\n", $1);
      }
    ;

read_targets_reset
    : /* empty */ {
        reset_pending_read_targets();
        $$ = xstrdup("");
      }
    ;

read_targets
    : read_target {
        $$ = $1;
      }
    | read_targets COMMA read_target {
        char* merged = str_printf("%s, %s", $1, $3);
        free($1);
        free($3);
        $$ = merged;
      }
    ;

read_target
    : IDENT {
        ValueType vt = symbol_type($1);
        if (vt == TY_UNKNOWN) {
            semantic_error("input target '%s' is undeclared", $1);
        } else {
            push_pending_read_target(vt, $1);
        }
        $$ = str_printf("&%s", $1);
      }
    ;

    call_expr
      : IDENT LPAREN arg_list_opt RPAREN {
        int fi = function_index($1);
        last_call_expr_type = TY_UNKNOWN;
        if (fi < 0) {
          semantic_error("call to undefined function '%s'", $1);
        } else {
          if (functions[fi].arg_count != pending_call_arg_count) {
            semantic_error("function '%s' expects %d args but got %d", $1, functions[fi].arg_count, pending_call_arg_count);
          } else {
            for (int i = 0; i < pending_call_arg_count; i++) {
              if (!can_assign(functions[fi].arg_types[i], pending_call_arg_types[i])) {
                semantic_error("type mismatch for arg %d in call to '%s'", i + 1, $1);
                break;
              }
            }
            last_call_expr_type = functions[fi].ret_type;
          }
        }
        $$ = str_printf("%s(%s)", $1, $3);
        }
      ;

  arg_list_opt
    : /* empty */ {
      reset_pending_call_args();
      $$ = xstrdup("");
      }
      | arg_reset arg_list {
          $$ = $2;
      }
    ;

  arg_reset
      : /* empty */ {
          reset_pending_call_args();
        }
      ;

  arg_list
    : arg_item {
      $$ = $1;
      }
    | arg_list COMMA arg_item {
      char* merged = str_printf("%s,%s", $1, $3);
      free($1);
      free($3);
      $$ = merged;
      }
    ;

  arg_item
    : postfix_expr {
      ValueType t = infer_postfix_type($1);
      push_pending_call_arg(t);
      $$ = postfix_to_c($1);
      }
    ;

opt_expr
    : /* empty */ {
        $$ = xstrdup("");
      }
    | postfix_expr {
        $$ = $1;
      }
    ;

postfix_expr
    : postfix_item {
        $$ = $1;
      }
    | postfix_expr postfix_item {
        char* merged = concat_space($1, $2);
        free($1);
        free($2);
        $$ = merged;
      }
    ;

postfix_item
    : IDENT {
        $$ = xstrdup($1);
      }
    | NUMBER {
        $$ = xstrdup($1);
      }
    | call_expr {
        $$ = $1;
      }
    | PLUS {
        $$ = xstrdup("+");
      }
    | MINUS {
        $$ = xstrdup("-");
      }
    | STAR {
        $$ = xstrdup("*");
      }
    | SLASH {
        $$ = xstrdup("/");
      }
    | MOD {
        $$ = xstrdup("%");
      }
    | GT {
        $$ = xstrdup(">");
      }
    | LT {
        $$ = xstrdup("<");
      }
    | GE {
        $$ = xstrdup(">=");
      }
    | LE {
        $$ = xstrdup("<=");
      }
    | EQ {
        $$ = xstrdup("==");
      }
    | NE {
        $$ = xstrdup("!=");
      }
    | AND {
        $$ = xstrdup("&&");
      }
    | OR {
        $$ = xstrdup("||");
      }
    | NOT {
        $$ = xstrdup("!");
      }
    ;

nl
    : NEWLINE {
        $$ = xstrdup("\n");
      }
    | nl NEWLINE {
        $$ = $1;
      }
    ;

%%

void yyerror(const char* s) {
  fprintf(stderr, "Parse error (line %d): %s\n", yylineno, s);
}

const char* get_generated_program(void) {
    return generated_program;
}

int has_semantic_errors(void) {
  return semantic_errors > 0;
}
