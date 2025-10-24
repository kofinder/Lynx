%{
#include "ast_ui.hpp"
#include <cstdio>
#include <cstdlib>

extern int yylex();
extern int yyparse();
extern FILE *yyin;

UIASTNode* root_ui;

void yyerror(const char *s);
%}

%union {
    int ival;
    UIASTNode* node;
}

%token <ival> INTEGER
%token CANVAS

%type <node> ui_expr canvas

%%

ui_program:
    ui_expr { root_ui = $1; }
    ;

ui_expr:
    canvas { $$ = $1; }
    ;

canvas:
    CANVAS '(' INTEGER ',' INTEGER ')' { $$ = new UICanvas($3, $5); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "UI parse error: %s\n", s);
}
