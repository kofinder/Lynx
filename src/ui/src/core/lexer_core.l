%{
#include "parser_core.tab.h"
%}

%%

[0-9]+      {
                yylval.ival = atoi(yytext);
                return INTEGER;
            }

[ \t\r\n]+  ; // skip whitespace

.           { return *yytext; }

%%
