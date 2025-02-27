%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

/* Token Definitions */
KEYWORD     int|return|if|else|while|for|break|continue|void|char|float|double
IDENTIFIER  [a-zA-Z_][a-zA-Z0-9_]*
/* ERRNUM must come before CONSTANT so that a number immediately followed by a letter is flagged */
ERRNUM      [0-9]+[a-zA-Z_]+
CONSTANT    [0-9]+(\.[0-9]+)?
/* STRING rule now accepts both double- and single-quoted strings */
STRING      (\"(\\.|[^\\"])\")|('(\\.|[^\\'])')
OPERATOR    [==|!=|<=|>=|&&|\|\||!|=|\+|-|\*|/|%]
PUNCTUATION [;,.{}()\[\]]

%%

{KEYWORD}      { printf("Keyword: %s\n", yytext); }
{IDENTIFIER}   { printf("Identifier: %s\n", yytext); }
{ERRNUM}       { printf("Lexical Error: %s\n", yytext); }
{CONSTANT}     { printf("Constant: %s\n", yytext); }
{STRING}       { printf("String: %s\n", yytext); }
{OPERATOR}     { printf("Operator: %s\n", yytext); }
{PUNCTUATION}  { printf("Punctuation: %s\n", yytext); }
"//".*         { printf("Comment: %s\n", yytext); }
"/"([^]|\+[^/])\+"/" { printf("Comment: MULTI-LINE\n"); }
[ \t\n]+       { /* Ignore whitespace */ }
.              { printf("Lexical Error: %s\n", yytext); }

%%

int main() {
    FILE *fp = fopen("input.c", "r");
    if(fp == NULL){
        printf("Could not open file %s\n", "input.c");
        exit(1);
    }
    yyin = fp;
    yylex();
    fclose(fp);
    return 0;
}

int yywrap() {
    return 1;
}