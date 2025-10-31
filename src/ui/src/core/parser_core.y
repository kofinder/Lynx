%{
#include "ast_core.hpp"
#include <cstdio>
#include <cstdlib>

extern int yylex();
extern int yyparse();
extern FILE *yyin;

ASTNode* root;

void yyerror(const char *s);
%}

%union {
    int ival;
    ASTNode* node;
}

%token <ival> INTEGER
%type <node> expr

%%

program:
    expr { root = $1; }
    ;

expr:
    INTEGER { $$ = new ASTInt($1); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Core parse error: %s\n", s);
}
