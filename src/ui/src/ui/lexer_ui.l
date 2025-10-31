%{
#include "parser_ui.tab.h"
%}

%%

canvas      { return CANVAS; }

[0-9]+      {
                yylval.ival = atoi(yytext);
                return INTEGER;
            }

[ \t\r\n]+  ; // skip whitespace

.           { return *yytext; }

%%
