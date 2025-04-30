%{
#include <stdio.h>
%}

%%
I|i|is|am|are|my|he|she|it|they|we|you                                      { printf("PRONOUN: %s ", yytext);printf("\n"); }
charusat|depstar|cricket|hostel|chess|apple|friends|books|football|class    { printf("NOUN: %s", yytext); printf("\n");}
intelligent|name |fruit|go|read|write|sleep|play|talk|walk|study|beautiful  { printf("VERB: %s ", yytext);printf("\n"); }
singing|dancing|playing|writting|reading|studying|sleeping|walking|writing  { printf("ADVERB: %s ", yytext);printf("\n"); }
happy|fast|strong|healthy|good|best|light                                   { printf("ADJECTIVE: %s ", yytext);printf("\n"); }
on|in|at|by|with|about|under|over|between|beside                            { printf("PREPOSITION: %s ", yytext);printf("\n"); }
and|but|for                                                                 { printf("CONJUNCTION: %s", yytext);printf("\n");}
[a-zA-Z]+                                                                   { printf("UNKNOWN: %s", yytext);printf("\n"); }
\n { printf("\n"); }
. { }
%%
int main() {
    printf("Enter text:\n");
    yylex();
    return 0;
}

int yywrap(){
    return 1;
}


