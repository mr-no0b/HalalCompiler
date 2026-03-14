%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static char* generated_program = NULL;

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
    return str_printf("%s %s", a, b);
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

static int is_operator(const char* tok) {
    return strcmp(tok, "+") == 0 || strcmp(tok, "-") == 0 || strcmp(tok, "*") == 0 ||
           strcmp(tok, "/") == 0 || strcmp(tok, ">") == 0 || strcmp(tok, "<") == 0 ||
           strcmp(tok, ">=") == 0 || strcmp(tok, "<=") == 0 || strcmp(tok, "==") == 0 ||
           strcmp(tok, "!=") == 0;
}

static char* postfix_to_c(const char* expr) {
    char* copy = xstrdup(expr);
    char* stack[512];
    int top = 0;

  char* tok = strtok(copy, " ");
  while (tok) {
        if (!is_operator(tok)) {
            stack[top++] = xstrdup(tok);
      tok = strtok(NULL, " ");
      continue;
        }

        if (top < 2) {
            fprintf(stderr, "Invalid postfix expression: %s\n", expr);
            free(copy);
            return xstrdup("0");
        }

        char* rhs = stack[--top];
        char* lhs = stack[--top];
        char* merged = str_printf("(%s %s %s)", lhs, tok, rhs);
        stack[top++] = merged;
        free(lhs);
        free(rhs);
        tok = strtok(NULL, " ");
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
%}

%union {
    char* str;
}

%token <str> IDENT NUMBER STRING TYPE
%token IF ELIF ELSE WHILE RETURN PRINT DEF
%token NEWLINE INDENT DEDENT COLON ASSIGN
%token PLUS MINUS STAR SLASH GT LT GE LE EQ NE
%token LPAREN RPAREN COMMA

%type <str> program top_list top_item func_def
%type <str> stmt_list stmt simple_stmt opt_expr postfix_expr postfix_item
%type <str> opt_elif opt_else nl

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
    : DEF TYPE IDENT LPAREN RPAREN COLON nl INDENT stmt_list DEDENT {
        char* indented = indent_code($9);
        $$ = str_printf("%s %s() {\n%s}\n\n", $2, $3, indented);
        free(indented);
      }
    | TYPE IDENT LPAREN RPAREN COLON nl INDENT stmt_list DEDENT {
        char* indented = indent_code($8);
        $$ = str_printf("%s %s() {\n%s}\n\n", $1, $2, indented);
        free(indented);
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
    | WHILE postfix_expr COLON nl INDENT stmt_list DEDENT {
        char* cond = postfix_to_c($2);
        char* body = indent_code($6);
        $$ = str_printf("while (%s) {\n%s}\n", cond, body);
        free(cond);
        free(body);
      }
    ;

opt_elif
    : /* empty */ {
        $$ = xstrdup("");
      }
    | ELIF postfix_expr COLON nl INDENT stmt_list DEDENT opt_elif {
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
        $$ = str_printf("%s %s;\n", $1, $2);
      }
    | TYPE IDENT postfix_expr ASSIGN nl {
        char* expr = postfix_to_c($3);
        $$ = str_printf("%s %s = %s;\n", $1, $2, expr);
        free(expr);
      }
    | IDENT postfix_expr ASSIGN nl {
        char* expr = postfix_to_c($2);
        $$ = str_printf("%s = %s;\n", $1, expr);
        free(expr);
      }
    | RETURN opt_expr nl {
        if ($2[0] == '\0') {
            $$ = xstrdup("return;\n");
        } else {
            char* expr = postfix_to_c($2);
            $$ = str_printf("return %s;\n", expr);
            free(expr);
        }
      }
    | PRINT postfix_expr nl {
        char* expr = postfix_to_c($2);
        $$ = str_printf("printf(\"%%g\\n\", (double)(%s));\n", expr);
        free(expr);
      }
    | IDENT LPAREN RPAREN nl {
        $$ = str_printf("%s();\n", $1);
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
    | STRING {
        $$ = xstrdup($1);
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
    fprintf(stderr, "Parse error: %s\n", s);
}

const char* get_generated_program(void) {
    return generated_program;
}
