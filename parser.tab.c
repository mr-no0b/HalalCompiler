/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

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

#line 540 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENT = 3,                      /* IDENT  */
  YYSYMBOL_NUMBER = 4,                     /* NUMBER  */
  YYSYMBOL_STRING = 5,                     /* STRING  */
  YYSYMBOL_TYPE = 6,                       /* TYPE  */
  YYSYMBOL_IF = 7,                         /* IF  */
  YYSYMBOL_ELIF = 8,                       /* ELIF  */
  YYSYMBOL_ELSE = 9,                       /* ELSE  */
  YYSYMBOL_WHILE = 10,                     /* WHILE  */
  YYSYMBOL_FOR = 11,                       /* FOR  */
  YYSYMBOL_RETURN = 12,                    /* RETURN  */
  YYSYMBOL_PRINT = 13,                     /* PRINT  */
  YYSYMBOL_SATR = 14,                      /* SATR  */
  YYSYMBOL_BREAK = 15,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 16,                  /* CONTINUE  */
  YYSYMBOL_DEF = 17,                       /* DEF  */
  YYSYMBOL_NEWLINE = 18,                   /* NEWLINE  */
  YYSYMBOL_INDENT = 19,                    /* INDENT  */
  YYSYMBOL_DEDENT = 20,                    /* DEDENT  */
  YYSYMBOL_COLON = 21,                     /* COLON  */
  YYSYMBOL_ASSIGN = 22,                    /* ASSIGN  */
  YYSYMBOL_SEMI = 23,                      /* SEMI  */
  YYSYMBOL_PLUS = 24,                      /* PLUS  */
  YYSYMBOL_MINUS = 25,                     /* MINUS  */
  YYSYMBOL_STAR = 26,                      /* STAR  */
  YYSYMBOL_SLASH = 27,                     /* SLASH  */
  YYSYMBOL_MOD = 28,                       /* MOD  */
  YYSYMBOL_GT = 29,                        /* GT  */
  YYSYMBOL_LT = 30,                        /* LT  */
  YYSYMBOL_GE = 31,                        /* GE  */
  YYSYMBOL_LE = 32,                        /* LE  */
  YYSYMBOL_EQ = 33,                        /* EQ  */
  YYSYMBOL_NE = 34,                        /* NE  */
  YYSYMBOL_AND = 35,                       /* AND  */
  YYSYMBOL_OR = 36,                        /* OR  */
  YYSYMBOL_NOT = 37,                       /* NOT  */
  YYSYMBOL_LPAREN = 38,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 39,                    /* RPAREN  */
  YYSYMBOL_COMMA = 40,                     /* COMMA  */
  YYSYMBOL_YYACCEPT = 41,                  /* $accept  */
  YYSYMBOL_program = 42,                   /* program  */
  YYSYMBOL_top_list = 43,                  /* top_list  */
  YYSYMBOL_top_item = 44,                  /* top_item  */
  YYSYMBOL_func_def = 45,                  /* func_def  */
  YYSYMBOL_func_open = 46,                 /* func_open  */
  YYSYMBOL_param_list_opt = 47,            /* param_list_opt  */
  YYSYMBOL_param_reset = 48,               /* param_reset  */
  YYSYMBOL_param_list = 49,                /* param_list  */
  YYSYMBOL_param_decl = 50,                /* param_decl  */
  YYSYMBOL_stmt_list = 51,                 /* stmt_list  */
  YYSYMBOL_stmt = 52,                      /* stmt  */
  YYSYMBOL_loop_enter = 53,                /* loop_enter  */
  YYSYMBOL_loop_exit = 54,                 /* loop_exit  */
  YYSYMBOL_for_init = 55,                  /* for_init  */
  YYSYMBOL_for_cond = 56,                  /* for_cond  */
  YYSYMBOL_for_update = 57,                /* for_update  */
  YYSYMBOL_opt_elif = 58,                  /* opt_elif  */
  YYSYMBOL_opt_else = 59,                  /* opt_else  */
  YYSYMBOL_simple_stmt = 60,               /* simple_stmt  */
  YYSYMBOL_call_expr = 61,                 /* call_expr  */
  YYSYMBOL_arg_list_opt = 62,              /* arg_list_opt  */
  YYSYMBOL_arg_reset = 63,                 /* arg_reset  */
  YYSYMBOL_arg_list = 64,                  /* arg_list  */
  YYSYMBOL_arg_item = 65,                  /* arg_item  */
  YYSYMBOL_opt_expr = 66,                  /* opt_expr  */
  YYSYMBOL_postfix_expr = 67,              /* postfix_expr  */
  YYSYMBOL_postfix_item = 68,              /* postfix_item  */
  YYSYMBOL_nl = 69                         /* nl  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  10
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   477

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  74
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  149

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   295


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   491,   491,   497,   500,   509,   515,   523,   527,   534,
     538,   544,   550,   553,   562,   572,   575,   584,   587,   602,
     610,   618,   624,   630,   633,   638,   649,   665,   668,   675,
     678,   694,   697,   711,   714,   722,   727,   738,   751,   769,
     775,   781,   793,   796,   799,   805,   828,   832,   838,   844,
     847,   856,   864,   867,   873,   876,   885,   888,   891,   894,
     897,   900,   903,   906,   909,   912,   915,   918,   921,   924,
     927,   930,   933,   939,   942
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDENT", "NUMBER",
  "STRING", "TYPE", "IF", "ELIF", "ELSE", "WHILE", "FOR", "RETURN",
  "PRINT", "SATR", "BREAK", "CONTINUE", "DEF", "NEWLINE", "INDENT",
  "DEDENT", "COLON", "ASSIGN", "SEMI", "PLUS", "MINUS", "STAR", "SLASH",
  "MOD", "GT", "LT", "GE", "LE", "EQ", "NE", "AND", "OR", "NOT", "LPAREN",
  "RPAREN", "COMMA", "$accept", "program", "top_list", "top_item",
  "func_def", "func_open", "param_list_opt", "param_reset", "param_list",
  "param_decl", "stmt_list", "stmt", "loop_enter", "loop_exit", "for_init",
  "for_cond", "for_update", "opt_elif", "opt_else", "simple_stmt",
  "call_expr", "arg_list_opt", "arg_reset", "arg_list", "arg_item",
  "opt_expr", "postfix_expr", "postfix_item", "nl", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-96)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-47)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       6,     1,     0,    11,     6,   -96,   -96,    19,   -20,    39,
     -96,   -96,   292,    37,     9,   118,    41,   377,   377,     2,
     377,   133,    30,    30,    30,   171,   -96,   -96,    30,    13,
      44,    37,    18,   -96,   -96,   -96,   -96,   -96,   -96,   -96,
     -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,    22,   -96,
     168,   -96,   203,   220,   239,   377,    54,    35,    30,   377,
      30,   203,   -96,    45,    45,    45,   -96,   -96,    45,    43,
      59,    25,   -96,    27,    31,   377,    30,   -96,   255,    45,
      30,    30,   290,   377,   377,    45,    45,    45,   -96,    30,
     -96,    44,    48,   -96,    32,   -96,   377,    45,    30,    -9,
       8,   -96,   306,    51,   377,    45,   -96,    30,   377,    45,
     292,   -96,   -96,    68,    45,   -96,   344,   292,   377,    55,
      70,   412,   341,    30,   377,    71,   -96,   -96,    10,   358,
      58,   -96,   -96,   -96,    30,    30,   292,    15,    17,   427,
     292,   292,   -96,   442,   457,   -96,    70,   -96,   -96
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     2,     3,     5,     0,     0,     0,
       1,     4,     0,     9,     0,     0,     0,     0,     0,    23,
      52,     0,     0,     0,     0,     0,    15,    17,     0,     0,
       0,     9,    56,    57,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    48,    58,
       0,    54,     0,     0,     0,     0,     0,     0,     0,    53,
       0,     0,    73,    43,    39,    40,     6,    16,    44,     0,
       0,    10,    12,     0,     0,     0,     0,    55,     0,    35,
       0,     0,     0,    24,    27,    38,    42,    41,    74,     0,
      14,     0,     0,    45,    47,    49,    51,    37,     0,     0,
       0,    26,     0,     0,    28,     8,    13,     0,     0,    36,
       0,    21,    25,    29,     7,    50,     0,     0,     0,     0,
      31,     0,     0,     0,     0,    33,    22,    30,     0,     0,
       0,    18,    19,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,    20,    31,    34,    32
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -96,   -96,   -96,    77,   -96,   -96,    53,   -96,   -96,    -6,
     -95,   -24,   -46,   -54,   -96,   -96,   -96,   -57,   -96,   -96,
     -12,   -96,   -96,   -96,   -18,   -96,    -1,   -29,   -21
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     3,     4,     5,     6,     7,    29,    30,    71,    72,
      25,    26,   117,   132,    57,   103,   119,   125,   131,    27,
      49,    74,    75,    94,    95,    58,    96,    51,    63
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      28,    67,    64,    65,     8,    55,     9,    68,    56,    88,
     110,    10,     1,    28,    50,   116,    53,    54,    13,    59,
      61,    77,   121,     2,    77,    77,    88,   111,    88,   133,
      77,    79,    77,    88,   140,    88,   141,    85,    12,    86,
      87,   139,    14,   -11,    52,   143,   144,    31,    62,    77,
      70,    78,    69,    77,    82,    97,    48,    83,    84,    99,
     100,   -46,    90,    88,    89,    91,    92,    77,   105,   107,
      93,   118,   108,    77,   113,    77,   123,   109,   124,   135,
     130,    11,   102,   104,    73,   106,   114,   136,   145,   148,
     115,     0,    67,    77,     0,     0,     0,    67,    28,     0,
      77,     0,   128,     0,    28,    28,     0,     0,     0,    28,
       0,     0,     0,   137,   138,    67,     0,   122,     0,    67,
      67,    32,    33,   129,    28,     0,     0,    28,    28,    28,
       0,    28,    28,     0,     0,     0,    32,    33,    60,     0,
       0,     0,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    32,    33,     0,    15,     0,     0,    16,    17,     0,
       0,    18,    19,    20,    21,    22,    23,    24,     0,     0,
      76,    66,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    32,    33,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,    32,    33,     0,     0,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    80,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    32,    33,
      81,     0,     0,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    98,     0,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    32,    33,    15,     0,     0,    16,    17,
       0,     0,    18,    19,    20,    21,    22,    23,    24,    32,
      33,     0,   101,     0,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,   112,     0,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    32,    33,     0,    15,     0,     0,
      16,    17,     0,     0,    18,    19,    20,    21,    22,    23,
      24,    32,    33,   127,   120,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,   134,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,     0,     0,     0,     0,
       0,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    15,     0,     0,    16,    17,
       0,     0,    18,    19,    20,    21,    22,    23,    24,     0,
      15,     0,   126,    16,    17,     0,     0,    18,    19,    20,
      21,    22,    23,    24,     0,    15,     0,   142,    16,    17,
       0,     0,    18,    19,    20,    21,    22,    23,    24,     0,
      15,     0,   146,    16,    17,     0,     0,    18,    19,    20,
      21,    22,    23,    24,     0,     0,     0,   147
};

static const yytype_int16 yycheck[] =
{
      12,    25,    23,    24,     3,     3,     6,    28,     6,    18,
      19,     0,     6,    25,    15,   110,    17,    18,    38,    20,
      21,    50,   117,    17,    53,    54,    18,    19,    18,    19,
      59,    52,    61,    18,    19,    18,    19,    58,    19,    60,
      61,   136,     3,     6,     3,   140,   141,    38,    18,    78,
       6,    52,    39,    82,    55,    76,    38,     3,    23,    80,
      81,    39,     3,    18,    21,    40,    39,    96,    89,    21,
      39,     3,    40,   102,    23,   104,    21,    98,     8,    21,
       9,     4,    83,    84,    31,    91,   107,   133,   142,   146,
     108,    -1,   116,   122,    -1,    -1,    -1,   121,   110,    -1,
     129,    -1,   123,    -1,   116,   117,    -1,    -1,    -1,   121,
      -1,    -1,    -1,   134,   135,   139,    -1,   118,    -1,   143,
     144,     3,     4,   124,   136,    -1,    -1,   139,   140,   141,
      -1,   143,   144,    -1,    -1,    -1,     3,     4,     5,    -1,
      -1,    -1,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,     3,     4,    -1,     3,    -1,    -1,     6,     7,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    -1,
      22,    20,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,     3,     4,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    -1,     3,     4,    -1,    -1,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    21,     3,     4,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,     3,     4,
      21,    -1,    -1,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    22,    -1,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,     3,     4,     3,    -1,    -1,     6,     7,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,     3,
       4,    -1,    22,    -1,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    22,    -1,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,     3,     4,    -1,     3,    -1,    -1,
       6,     7,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,     3,     4,    22,    20,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    21,
       3,     4,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    -1,    -1,    -1,    -1,
      -1,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,     3,    -1,    -1,     6,     7,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
       3,    -1,    20,     6,     7,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,     3,    -1,    20,     6,     7,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
       3,    -1,    20,     6,     7,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    20
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     6,    17,    42,    43,    44,    45,    46,     3,     6,
       0,    44,    19,    38,     3,     3,     6,     7,    10,    11,
      12,    13,    14,    15,    16,    51,    52,    60,    61,    47,
      48,    38,     3,     4,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    61,
      67,    68,     3,    67,    67,     3,     6,    55,    66,    67,
       5,    67,    18,    69,    69,    69,    20,    52,    69,    39,
       6,    49,    50,    47,    62,    63,    22,    68,    67,    69,
      21,    21,    67,     3,    23,    69,    69,    69,    18,    21,
       3,    40,    39,    39,    64,    65,    67,    69,    22,    69,
      69,    22,    67,    56,    67,    69,    50,    21,    40,    69,
      19,    19,    22,    23,    69,    65,    51,    53,     3,    57,
      20,    51,    67,    21,     8,    58,    20,    22,    69,    67,
       9,    59,    54,    19,    21,    21,    53,    69,    69,    51,
      19,    19,    20,    51,    51,    54,    20,    20,    58
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    41,    42,    43,    43,    44,    45,    46,    46,    47,
      47,    48,    49,    49,    50,    51,    51,    52,    52,    52,
      52,    53,    54,    55,    55,    55,    55,    56,    56,    57,
      57,    58,    58,    59,    59,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    61,    62,    62,    63,    64,
      64,    65,    66,    66,    67,    67,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    69,    69
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     4,     8,     7,     0,
       2,     0,     1,     3,     2,     1,     2,     1,     9,     9,
      13,     0,     0,     0,     2,     4,     3,     0,     1,     0,
       3,     0,     8,     0,     6,     3,     5,     4,     3,     2,
       2,     3,     3,     2,     2,     4,     0,     2,     0,     1,
       3,     1,     0,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: top_list  */
#line 491 "parser.y"
               {
        generated_program = (yyvsp[0].str);
      }
#line 1758 "parser.tab.c"
    break;

  case 3: /* top_list: top_item  */
#line 497 "parser.y"
               {
        (yyval.str) = (yyvsp[0].str);
      }
#line 1766 "parser.tab.c"
    break;

  case 4: /* top_list: top_list top_item  */
#line 500 "parser.y"
                        {
        char* merged = concat2((yyvsp[-1].str), (yyvsp[0].str));
        free((yyvsp[-1].str));
        free((yyvsp[0].str));
        (yyval.str) = merged;
      }
#line 1777 "parser.tab.c"
    break;

  case 5: /* top_item: func_def  */
#line 509 "parser.y"
               {
        (yyval.str) = (yyvsp[0].str);
      }
#line 1785 "parser.tab.c"
    break;

  case 6: /* func_def: func_open INDENT stmt_list DEDENT  */
#line 515 "parser.y"
                                        {
        char* indented = indent_code((yyvsp[-1].str));
        (yyval.str) = str_printf("%s {\n%s}\n\n", (yyvsp[-3].str), indented);
        free(indented);
      }
#line 1795 "parser.tab.c"
    break;

  case 7: /* func_open: DEF TYPE IDENT LPAREN param_list_opt RPAREN COLON nl  */
#line 523 "parser.y"
                                                           {
        begin_function((yyvsp[-6].str), (yyvsp[-5].str));
        (yyval.str) = str_printf("%s %s(%s)", (yyvsp[-6].str), (yyvsp[-5].str), (yyvsp[-3].str));
      }
#line 1804 "parser.tab.c"
    break;

  case 8: /* func_open: TYPE IDENT LPAREN param_list_opt RPAREN COLON nl  */
#line 527 "parser.y"
                                                       {
        begin_function((yyvsp[-6].str), (yyvsp[-5].str));
        (yyval.str) = str_printf("%s %s(%s)", (yyvsp[-6].str), (yyvsp[-5].str), (yyvsp[-3].str));
      }
#line 1813 "parser.tab.c"
    break;

  case 9: /* param_list_opt: %empty  */
#line 534 "parser.y"
                  {
        reset_pending_params();
        (yyval.str) = xstrdup("");
      }
#line 1822 "parser.tab.c"
    break;

  case 10: /* param_list_opt: param_reset param_list  */
#line 538 "parser.y"
                             {
        (yyval.str) = (yyvsp[0].str);
      }
#line 1830 "parser.tab.c"
    break;

  case 11: /* param_reset: %empty  */
#line 544 "parser.y"
                  {
        reset_pending_params();
      }
#line 1838 "parser.tab.c"
    break;

  case 12: /* param_list: param_decl  */
#line 550 "parser.y"
                 {
        (yyval.str) = (yyvsp[0].str);
      }
#line 1846 "parser.tab.c"
    break;

  case 13: /* param_list: param_list COMMA param_decl  */
#line 553 "parser.y"
                                  {
        char* merged = str_printf("%s, %s", (yyvsp[-2].str), (yyvsp[0].str));
        free((yyvsp[-2].str));
        free((yyvsp[0].str));
        (yyval.str) = merged;
      }
#line 1857 "parser.tab.c"
    break;

  case 14: /* param_decl: TYPE IDENT  */
#line 562 "parser.y"
                 {
        if (parse_type_name((yyvsp[-1].str)) == TY_VOID) {
            semantic_error("parameter '%s' cannot be of type void", (yyvsp[0].str));
        }
        push_pending_param((yyvsp[-1].str), (yyvsp[0].str));
        (yyval.str) = str_printf("%s %s", (yyvsp[-1].str), (yyvsp[0].str));
      }
#line 1869 "parser.tab.c"
    break;

  case 15: /* stmt_list: stmt  */
#line 572 "parser.y"
           {
        (yyval.str) = (yyvsp[0].str);
      }
#line 1877 "parser.tab.c"
    break;

  case 16: /* stmt_list: stmt_list stmt  */
#line 575 "parser.y"
                     {
        char* merged = concat2((yyvsp[-1].str), (yyvsp[0].str));
        free((yyvsp[-1].str));
        free((yyvsp[0].str));
        (yyval.str) = merged;
      }
#line 1888 "parser.tab.c"
    break;

  case 17: /* stmt: simple_stmt  */
#line 584 "parser.y"
                  {
        (yyval.str) = (yyvsp[0].str);
      }
#line 1896 "parser.tab.c"
    break;

  case 18: /* stmt: IF postfix_expr COLON nl INDENT stmt_list DEDENT opt_elif opt_else  */
#line 587 "parser.y"
                                                                         {
        infer_postfix_type((yyvsp[-7].str));
        char* cond = postfix_to_c((yyvsp[-7].str));
        char* if_body = indent_code((yyvsp[-3].str));
        char* core = str_printf("if (%s) {\n%s}\n", cond, if_body);

        char* with_elif = concat2(core, (yyvsp[-1].str));
        char* full = concat2(with_elif, (yyvsp[0].str));

        free(cond);
        free(if_body);
        free(core);
        free(with_elif);
        (yyval.str) = full;
      }
#line 1916 "parser.tab.c"
    break;

  case 19: /* stmt: WHILE postfix_expr COLON nl INDENT loop_enter stmt_list DEDENT loop_exit  */
#line 602 "parser.y"
                                                                               {
        char* cond = postfix_to_c((yyvsp[-7].str));
        infer_postfix_type((yyvsp[-7].str));
        char* body = indent_code((yyvsp[-2].str));
        (yyval.str) = str_printf("while (%s) {\n%s}\n", cond, body);
        free(cond);
        free(body);
      }
#line 1929 "parser.tab.c"
    break;

  case 20: /* stmt: FOR for_init SEMI for_cond SEMI for_update COLON nl INDENT loop_enter stmt_list DEDENT loop_exit  */
#line 610 "parser.y"
                                                                                                       {
        char* body = indent_code((yyvsp[-2].str));
        (yyval.str) = str_printf("for (%s; %s; %s) {\n%s}\n", (yyvsp[-11].str), (yyvsp[-9].str), (yyvsp[-7].str), body);
        free(body);
      }
#line 1939 "parser.tab.c"
    break;

  case 21: /* loop_enter: %empty  */
#line 618 "parser.y"
                  {
        loop_depth++;
      }
#line 1947 "parser.tab.c"
    break;

  case 22: /* loop_exit: %empty  */
#line 624 "parser.y"
                  {
        loop_depth--;
      }
#line 1955 "parser.tab.c"
    break;

  case 23: /* for_init: %empty  */
#line 630 "parser.y"
                  {
        (yyval.str) = xstrdup("");
      }
#line 1963 "parser.tab.c"
    break;

  case 24: /* for_init: TYPE IDENT  */
#line 633 "parser.y"
                 {
        ValueType t = parse_type_name((yyvsp[-1].str));
        declare_symbol((yyvsp[0].str), t);
        (yyval.str) = str_printf("%s %s", (yyvsp[-1].str), (yyvsp[0].str));
      }
#line 1973 "parser.tab.c"
    break;

  case 25: /* for_init: TYPE IDENT postfix_expr ASSIGN  */
#line 638 "parser.y"
                                     {
        ValueType t = parse_type_name((yyvsp[-3].str));
        declare_symbol((yyvsp[-2].str), t);
        ValueType rhs = infer_postfix_type((yyvsp[-1].str));
        if (rhs != TY_UNKNOWN && !can_assign(t, rhs)) {
            semantic_error("cannot assign expression to variable '%s'", (yyvsp[-2].str));
        }
        char* expr = postfix_to_c((yyvsp[-1].str));
        (yyval.str) = str_printf("%s %s = %s", (yyvsp[-3].str), (yyvsp[-2].str), expr);
        free(expr);
      }
#line 1989 "parser.tab.c"
    break;

  case 26: /* for_init: IDENT postfix_expr ASSIGN  */
#line 649 "parser.y"
                                {
        ValueType lhs = symbol_type((yyvsp[-2].str));
        if (lhs == TY_UNKNOWN) {
            semantic_error("assignment to undeclared variable '%s'", (yyvsp[-2].str));
        }
        ValueType rhs = infer_postfix_type((yyvsp[-1].str));
        if (lhs != TY_UNKNOWN && rhs != TY_UNKNOWN && !can_assign(lhs, rhs)) {
            semantic_error("type mismatch in assignment to '%s'", (yyvsp[-2].str));
        }
        char* expr = postfix_to_c((yyvsp[-1].str));
        (yyval.str) = str_printf("%s = %s", (yyvsp[-2].str), expr);
        free(expr);
      }
#line 2007 "parser.tab.c"
    break;

  case 27: /* for_cond: %empty  */
#line 665 "parser.y"
                  {
        (yyval.str) = xstrdup("1");
      }
#line 2015 "parser.tab.c"
    break;

  case 28: /* for_cond: postfix_expr  */
#line 668 "parser.y"
                   {
        infer_postfix_type((yyvsp[0].str));
        (yyval.str) = postfix_to_c((yyvsp[0].str));
      }
#line 2024 "parser.tab.c"
    break;

  case 29: /* for_update: %empty  */
#line 675 "parser.y"
                  {
        (yyval.str) = xstrdup("");
      }
#line 2032 "parser.tab.c"
    break;

  case 30: /* for_update: IDENT postfix_expr ASSIGN  */
#line 678 "parser.y"
                                {
        ValueType lhs = symbol_type((yyvsp[-2].str));
        if (lhs == TY_UNKNOWN) {
            semantic_error("assignment to undeclared variable '%s'", (yyvsp[-2].str));
        }
        ValueType rhs = infer_postfix_type((yyvsp[-1].str));
        if (lhs != TY_UNKNOWN && rhs != TY_UNKNOWN && !can_assign(lhs, rhs)) {
            semantic_error("type mismatch in assignment to '%s'", (yyvsp[-2].str));
        }
        char* expr = postfix_to_c((yyvsp[-1].str));
        (yyval.str) = str_printf("%s = %s", (yyvsp[-2].str), expr);
        free(expr);
      }
#line 2050 "parser.tab.c"
    break;

  case 31: /* opt_elif: %empty  */
#line 694 "parser.y"
                  {
        (yyval.str) = xstrdup("");
      }
#line 2058 "parser.tab.c"
    break;

  case 32: /* opt_elif: ELIF postfix_expr COLON nl INDENT stmt_list DEDENT opt_elif  */
#line 697 "parser.y"
                                                                  {
      infer_postfix_type((yyvsp[-6].str));
        char* cond = postfix_to_c((yyvsp[-6].str));
        char* body = indent_code((yyvsp[-2].str));
        char* current = str_printf("else if (%s) {\n%s}\n", cond, body);
        char* full = concat2(current, (yyvsp[0].str));
        free(cond);
        free(body);
        free(current);
        (yyval.str) = full;
      }
#line 2074 "parser.tab.c"
    break;

  case 33: /* opt_else: %empty  */
#line 711 "parser.y"
                  {
        (yyval.str) = xstrdup("");
      }
#line 2082 "parser.tab.c"
    break;

  case 34: /* opt_else: ELSE COLON nl INDENT stmt_list DEDENT  */
#line 714 "parser.y"
                                            {
        char* body = indent_code((yyvsp[-1].str));
        (yyval.str) = str_printf("else {\n%s}\n", body);
        free(body);
      }
#line 2092 "parser.tab.c"
    break;

  case 35: /* simple_stmt: TYPE IDENT nl  */
#line 722 "parser.y"
                    {
        ValueType t = parse_type_name((yyvsp[-2].str));
        declare_symbol((yyvsp[-1].str), t);
        (yyval.str) = str_printf("%s %s;\n", (yyvsp[-2].str), (yyvsp[-1].str));
      }
#line 2102 "parser.tab.c"
    break;

  case 36: /* simple_stmt: TYPE IDENT postfix_expr ASSIGN nl  */
#line 727 "parser.y"
                                        {
        ValueType t = parse_type_name((yyvsp[-4].str));
        declare_symbol((yyvsp[-3].str), t);
        ValueType rhs = infer_postfix_type((yyvsp[-2].str));
        if (rhs != TY_UNKNOWN && !can_assign(t, rhs)) {
            semantic_error("cannot assign expression to variable '%s'", (yyvsp[-3].str));
        }
        char* expr = postfix_to_c((yyvsp[-2].str));
        (yyval.str) = str_printf("%s %s = %s;\n", (yyvsp[-4].str), (yyvsp[-3].str), expr);
        free(expr);
      }
#line 2118 "parser.tab.c"
    break;

  case 37: /* simple_stmt: IDENT postfix_expr ASSIGN nl  */
#line 738 "parser.y"
                                   {
        ValueType lhs = symbol_type((yyvsp[-3].str));
        if (lhs == TY_UNKNOWN) {
            semantic_error("assignment to undeclared variable '%s'", (yyvsp[-3].str));
        }
        ValueType rhs = infer_postfix_type((yyvsp[-2].str));
        if (lhs != TY_UNKNOWN && rhs != TY_UNKNOWN && !can_assign(lhs, rhs)) {
            semantic_error("type mismatch in assignment to '%s'", (yyvsp[-3].str));
        }
        char* expr = postfix_to_c((yyvsp[-2].str));
        (yyval.str) = str_printf("%s = %s;\n", (yyvsp[-3].str), expr);
        free(expr);
      }
#line 2136 "parser.tab.c"
    break;

  case 38: /* simple_stmt: RETURN opt_expr nl  */
#line 751 "parser.y"
                         {
        if ((yyvsp[-1].str)[0] == '\0') {
            if (current_function_type != TY_VOID) {
                semantic_error("non-void function must return a value");
            }
            (yyval.str) = xstrdup("return;\n");
        } else {
            ValueType r = infer_postfix_type((yyvsp[-1].str));
            if (current_function_type == TY_VOID) {
                semantic_error("void function cannot return a value");
            } else if (r != TY_UNKNOWN && !can_assign(current_function_type, r)) {
                semantic_error("return type mismatch");
            }
            char* expr = postfix_to_c((yyvsp[-1].str));
            (yyval.str) = str_printf("return %s;\n", expr);
            free(expr);
        }
      }
#line 2159 "parser.tab.c"
    break;

  case 39: /* simple_stmt: BREAK nl  */
#line 769 "parser.y"
               {
        if (loop_depth <= 0) {
            semantic_error("'qif'/'break' used outside loop");
        }
        (yyval.str) = xstrdup("break;\n");
      }
#line 2170 "parser.tab.c"
    break;

  case 40: /* simple_stmt: CONTINUE nl  */
#line 775 "parser.y"
                  {
        if (loop_depth <= 0) {
            semantic_error("'wasil'/'continue' used outside loop");
        }
        (yyval.str) = xstrdup("continue;\n");
      }
#line 2181 "parser.tab.c"
    break;

  case 41: /* simple_stmt: PRINT postfix_expr nl  */
#line 781 "parser.y"
                            {
      ValueType t = infer_postfix_type((yyvsp[-1].str));
        char* expr = postfix_to_c((yyvsp[-1].str));
      if (t == TY_DOUBLE) {
            (yyval.str) = str_printf("printf(\"%%g\", (double)(%s));\n", expr);
      } else if (t == TY_INT || t == TY_CHAR) {
            (yyval.str) = str_printf("printf(\"%%d\", (int)(%s));\n", expr);
      } else {
            (yyval.str) = str_printf("printf(\"%%g\", (double)(%s));\n", expr);
      }
        free(expr);
      }
#line 2198 "parser.tab.c"
    break;

  case 42: /* simple_stmt: PRINT STRING nl  */
#line 793 "parser.y"
                      {
        (yyval.str) = str_printf("printf(\"%%s\", %s);\n", (yyvsp[-1].str));
      }
#line 2206 "parser.tab.c"
    break;

  case 43: /* simple_stmt: SATR nl  */
#line 796 "parser.y"
              {
        (yyval.str) = xstrdup("printf(\"\\n\");\n");
      }
#line 2214 "parser.tab.c"
    break;

  case 44: /* simple_stmt: call_expr nl  */
#line 799 "parser.y"
                   {
      (yyval.str) = str_printf("%s;\n", (yyvsp[-1].str));
      }
#line 2222 "parser.tab.c"
    break;

  case 45: /* call_expr: IDENT LPAREN arg_list_opt RPAREN  */
#line 805 "parser.y"
                                         {
        int fi = function_index((yyvsp[-3].str));
        last_call_expr_type = TY_UNKNOWN;
        if (fi < 0) {
          semantic_error("call to undefined function '%s'", (yyvsp[-3].str));
        } else {
          if (functions[fi].arg_count != pending_call_arg_count) {
            semantic_error("function '%s' expects %d args but got %d", (yyvsp[-3].str), functions[fi].arg_count, pending_call_arg_count);
          } else {
            for (int i = 0; i < pending_call_arg_count; i++) {
              if (!can_assign(functions[fi].arg_types[i], pending_call_arg_types[i])) {
                semantic_error("type mismatch for arg %d in call to '%s'", i + 1, (yyvsp[-3].str));
                break;
              }
            }
            last_call_expr_type = functions[fi].ret_type;
          }
        }
        (yyval.str) = str_printf("%s(%s)", (yyvsp[-3].str), (yyvsp[-1].str));
        }
#line 2247 "parser.tab.c"
    break;

  case 46: /* arg_list_opt: %empty  */
#line 828 "parser.y"
                  {
      reset_pending_call_args();
      (yyval.str) = xstrdup("");
      }
#line 2256 "parser.tab.c"
    break;

  case 47: /* arg_list_opt: arg_reset arg_list  */
#line 832 "parser.y"
                           {
          (yyval.str) = (yyvsp[0].str);
      }
#line 2264 "parser.tab.c"
    break;

  case 48: /* arg_reset: %empty  */
#line 838 "parser.y"
                    {
          reset_pending_call_args();
        }
#line 2272 "parser.tab.c"
    break;

  case 49: /* arg_list: arg_item  */
#line 844 "parser.y"
               {
      (yyval.str) = (yyvsp[0].str);
      }
#line 2280 "parser.tab.c"
    break;

  case 50: /* arg_list: arg_list COMMA arg_item  */
#line 847 "parser.y"
                              {
      char* merged = str_printf("%s,%s", (yyvsp[-2].str), (yyvsp[0].str));
      free((yyvsp[-2].str));
      free((yyvsp[0].str));
      (yyval.str) = merged;
      }
#line 2291 "parser.tab.c"
    break;

  case 51: /* arg_item: postfix_expr  */
#line 856 "parser.y"
                   {
      ValueType t = infer_postfix_type((yyvsp[0].str));
      push_pending_call_arg(t);
      (yyval.str) = postfix_to_c((yyvsp[0].str));
      }
#line 2301 "parser.tab.c"
    break;

  case 52: /* opt_expr: %empty  */
#line 864 "parser.y"
                  {
        (yyval.str) = xstrdup("");
      }
#line 2309 "parser.tab.c"
    break;

  case 53: /* opt_expr: postfix_expr  */
#line 867 "parser.y"
                   {
        (yyval.str) = (yyvsp[0].str);
      }
#line 2317 "parser.tab.c"
    break;

  case 54: /* postfix_expr: postfix_item  */
#line 873 "parser.y"
                   {
        (yyval.str) = (yyvsp[0].str);
      }
#line 2325 "parser.tab.c"
    break;

  case 55: /* postfix_expr: postfix_expr postfix_item  */
#line 876 "parser.y"
                                {
        char* merged = concat_space((yyvsp[-1].str), (yyvsp[0].str));
        free((yyvsp[-1].str));
        free((yyvsp[0].str));
        (yyval.str) = merged;
      }
#line 2336 "parser.tab.c"
    break;

  case 56: /* postfix_item: IDENT  */
#line 885 "parser.y"
            {
        (yyval.str) = xstrdup((yyvsp[0].str));
      }
#line 2344 "parser.tab.c"
    break;

  case 57: /* postfix_item: NUMBER  */
#line 888 "parser.y"
             {
        (yyval.str) = xstrdup((yyvsp[0].str));
      }
#line 2352 "parser.tab.c"
    break;

  case 58: /* postfix_item: call_expr  */
#line 891 "parser.y"
                {
        (yyval.str) = (yyvsp[0].str);
      }
#line 2360 "parser.tab.c"
    break;

  case 59: /* postfix_item: PLUS  */
#line 894 "parser.y"
           {
        (yyval.str) = xstrdup("+");
      }
#line 2368 "parser.tab.c"
    break;

  case 60: /* postfix_item: MINUS  */
#line 897 "parser.y"
            {
        (yyval.str) = xstrdup("-");
      }
#line 2376 "parser.tab.c"
    break;

  case 61: /* postfix_item: STAR  */
#line 900 "parser.y"
           {
        (yyval.str) = xstrdup("*");
      }
#line 2384 "parser.tab.c"
    break;

  case 62: /* postfix_item: SLASH  */
#line 903 "parser.y"
            {
        (yyval.str) = xstrdup("/");
      }
#line 2392 "parser.tab.c"
    break;

  case 63: /* postfix_item: MOD  */
#line 906 "parser.y"
          {
        (yyval.str) = xstrdup("%");
      }
#line 2400 "parser.tab.c"
    break;

  case 64: /* postfix_item: GT  */
#line 909 "parser.y"
         {
        (yyval.str) = xstrdup(">");
      }
#line 2408 "parser.tab.c"
    break;

  case 65: /* postfix_item: LT  */
#line 912 "parser.y"
         {
        (yyval.str) = xstrdup("<");
      }
#line 2416 "parser.tab.c"
    break;

  case 66: /* postfix_item: GE  */
#line 915 "parser.y"
         {
        (yyval.str) = xstrdup(">=");
      }
#line 2424 "parser.tab.c"
    break;

  case 67: /* postfix_item: LE  */
#line 918 "parser.y"
         {
        (yyval.str) = xstrdup("<=");
      }
#line 2432 "parser.tab.c"
    break;

  case 68: /* postfix_item: EQ  */
#line 921 "parser.y"
         {
        (yyval.str) = xstrdup("==");
      }
#line 2440 "parser.tab.c"
    break;

  case 69: /* postfix_item: NE  */
#line 924 "parser.y"
         {
        (yyval.str) = xstrdup("!=");
      }
#line 2448 "parser.tab.c"
    break;

  case 70: /* postfix_item: AND  */
#line 927 "parser.y"
          {
        (yyval.str) = xstrdup("&&");
      }
#line 2456 "parser.tab.c"
    break;

  case 71: /* postfix_item: OR  */
#line 930 "parser.y"
         {
        (yyval.str) = xstrdup("||");
      }
#line 2464 "parser.tab.c"
    break;

  case 72: /* postfix_item: NOT  */
#line 933 "parser.y"
          {
        (yyval.str) = xstrdup("!");
      }
#line 2472 "parser.tab.c"
    break;

  case 73: /* nl: NEWLINE  */
#line 939 "parser.y"
              {
        (yyval.str) = xstrdup("\n");
      }
#line 2480 "parser.tab.c"
    break;

  case 74: /* nl: nl NEWLINE  */
#line 942 "parser.y"
                 {
        (yyval.str) = (yyvsp[-1].str);
      }
#line 2488 "parser.tab.c"
    break;


#line 2492 "parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 947 "parser.y"


void yyerror(const char* s) {
  fprintf(stderr, "Parse error (line %d): %s\n", yylineno, s);
}

const char* get_generated_program(void) {
    return generated_program;
}

int has_semantic_errors(void) {
  return semantic_errors > 0;
}
