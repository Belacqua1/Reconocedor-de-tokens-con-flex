%{
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void print_token(const char *type, const char *lexeme);
int es_keyword(const char *lexeme_upper);
int es_logical_op(const char *lexeme_upper);
%}

%option noyywrap

LETTER  [A-Za-z_]
DIGIT   [0-9]

%%

"--"[^\n]*        { print_token("COMMENT", yytext); }

[ \t\r\n]+        { /* ignorar espacios */ }

{DIGIT}+"."{DIGIT}+   { print_token("DECIMAL", yytext); }
{DIGIT}+              { print_token("INTEGER", yytext); }

\'[^\']*\'        { print_token("STRING", yytext); }
\"[^\"]*\"        { print_token("STRING", yytext); }

"<="|">="|"="|"<"|">"   { print_token("RELATIONAL_OP", yytext); }

[;,()]            { print_token("DELIMITER", yytext); }

{LETTER}({LETTER}|{DIGIT}|_)* {
    char upper[256];
    int i, len = yyleng < 255 ? yyleng : 255;

    for (i = 0; i < len; i++) {
        upper[i] = (char) toupper((unsigned char) yytext[i]);
    }
    upper[len] = '\0';

    if (es_keyword(upper)) {
        print_token("KEYWORD", yytext);
    } else if (es_logical_op(upper)) {
        print_token("LOGICAL_OP", yytext);
    } else {
        print_token("IDENTIFIER", yytext);
    }
}

.   { print_token("UNKNOWN", yytext); }

%%

void print_token(const char *type, const char *lexeme) {
    printf("<%s>: %s\n", type, lexeme);
}

int es_keyword(const char *lexeme_upper) {
    return
        strcmp(lexeme_upper, "SELECT") == 0 ||
        strcmp(lexeme_upper, "INSERT") == 0 ||
        strcmp(lexeme_upper, "WHERE")  == 0 ||
        strcmp(lexeme_upper, "UPDATE") == 0 ||
        strcmp(lexeme_upper, "DELETE") == 0 ||
        strcmp(lexeme_upper, "FROM")   == 0;
}

int es_logical_op(const char *lexeme_upper) {
    return
        strcmp(lexeme_upper, "AND") == 0 ||
        strcmp(lexeme_upper, "OR")  == 0 ||
        strcmp(lexeme_upper, "NOT") == 0;
}
