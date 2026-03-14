%option noyywrap nodefault yylineno

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

#define YY_DECL int next_token(void)

static int indent_stack[256] = {0};
static int indent_top = 0;
static int pending_dedents = 0;
static int current_indent = 0;

static char* dup_text(const char* s) {
    size_t n = strlen(s) + 1;
    char* out = (char*)malloc(n);
    if (!out) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    memcpy(out, s, n);
    return out;
}

#define YY_USER_INIT do { BEGIN(BOL); } while (0)

static int process_indent(int indent) {
    int current = indent_stack[indent_top];
    if (indent > current) {
        indent_stack[++indent_top] = indent;
        return INDENT;
    }
    if (indent < current) {
        while (indent_top > 0 && indent < indent_stack[indent_top]) {
            indent_top--;
            pending_dedents++;
        }
        if (indent != indent_stack[indent_top]) {
            fprintf(stderr, "Indentation error at line %d\n", yylineno);
            exit(1);
        }
        if (pending_dedents > 0) {
            pending_dedents--;
            return DEDENT;
        }
    }
    return 0;
}
%}

%x BOL

%%

<BOL>[ ]+                 { current_indent += yyleng; }
<BOL>\t+                 { current_indent += 4 * yyleng; }
<BOL>\r                  { }
<BOL>\n                  { current_indent = 0; return NEWLINE; }
<BOL>[#][^\n]*            { current_indent = 0; }
<BOL>[\/][\/][^\n]*      { current_indent = 0; }
<BOL>.                    {
                            unput(yytext[0]);
                            int t = process_indent(current_indent);
                            current_indent = 0;
                            BEGIN(INITIAL);
                            if (t) return t;
                         }

<INITIAL>[ ]+             { }
<INITIAL>\t+             { }
<INITIAL>\r              { }
<INITIAL>\n              { BEGIN(BOL); return NEWLINE; }
<INITIAL>[#][^\n]*        { }
<INITIAL>[\/][\/][^\n]*  { }

<INITIAL>":"            { return COLON; }
<INITIAL>"("            { return LPAREN; }
<INITIAL>")"            { return RPAREN; }
<INITIAL>","            { return COMMA; }

<INITIAL>"=="           { return EQ; }
<INITIAL>"!="           { return NE; }
<INITIAL>">="           { return GE; }
<INITIAL>"<="           { return LE; }
<INITIAL>"="            { return ASSIGN; }
<INITIAL>">"            { return GT; }
<INITIAL>"<"            { return LT; }
<INITIAL>"+"            { return PLUS; }
<INITIAL>"-"            { return MINUS; }
<INITIAL>"*"            { return STAR; }
<INITIAL>"/"            { return SLASH; }

<INITIAL>"\""([^\\\n]|(\\.))*"\""  {
                                     yylval.str = dup_text(yytext);
                            return STRING;
                         }

<INITIAL>[0-9]+(\.[0-9]+)? {
                                     yylval.str = dup_text(yytext);
                            return NUMBER;
                         }

<INITIAL>[A-Za-z_][A-Za-z0-9_]* {
                            if (strcmp(yytext, "if") == 0 || strcmp(yytext, "iza") == 0) return IF;
                            if (strcmp(yytext, "elif") == 0 || strcmp(yytext, "iza_ila") == 0) return ELIF;
                            if (strcmp(yytext, "else") == 0 || strcmp(yytext, "wailla") == 0) return ELSE;
                            if (strcmp(yytext, "while") == 0 || strcmp(yytext, "ma_dama") == 0) return WHILE;
                            if (strcmp(yytext, "return") == 0 || strcmp(yytext, "irji") == 0) return RETURN;
                            if (strcmp(yytext, "print") == 0 || strcmp(yytext, "qul") == 0) return PRINT;
                            if (strcmp(yytext, "def") == 0) return DEF;

                            if (strcmp(yytext, "int") == 0 || strcmp(yytext, "adad") == 0) {
                                yylval.str = dup_text("int");
                                return TYPE;
                            }
                            if (strcmp(yytext, "double") == 0 || strcmp(yytext, "haqiqi") == 0) {
                                yylval.str = dup_text("double");
                                return TYPE;
                            }
                            if (strcmp(yytext, "char") == 0 || strcmp(yytext, "harf") == 0) {
                                yylval.str = dup_text("char");
                                return TYPE;
                            }
                            if (strcmp(yytext, "void") == 0 || strcmp(yytext, "fadi") == 0) {
                                yylval.str = dup_text("void");
                                return TYPE;
                            }

                            yylval.str = dup_text(yytext);
                            return IDENT;
                         }

<INITIAL>.                {
                            fprintf(stderr, "Invalid token '%s' at line %d\n", yytext, yylineno);
                            exit(1);
                         }

%%

int yylex(void) {
    if (pending_dedents > 0) {
        pending_dedents--;
        return DEDENT;
    }

    int tok = next_token();

    if (tok == 0 && indent_top > 0) {
        indent_top--;
        return DEDENT;
    }

    return tok;
}
