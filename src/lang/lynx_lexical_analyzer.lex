%{
    #include <string>        // For string manipulation
    #include <cstdlib>       // For std::atoi function
    #include "parser.hpp"     // Include the Bison parser header
    #include "LynxDriver.hpp"  // Include the driver class for scanner control
    #include "LynxScanner.hpp" // Include the custom scanner header
    #include "LynxTokenizer.hpp"

    #undef YY_DECL
    #define YY_DECL int LynxLang::LynxScanner::yylex(LynxLang::Parser::semantic_type* yylval, LynxLang::Parser::location_type* yylloc, LynxLang::LynxDriver& driver)
%}

%option stack
%option debug
%option c++
%option noyywrap
%option yywrap nounput
%option never-interactive
%option yylineno
%option batch
%option prefix="LynxLang"
%x COMMENT


%%

"import"                                { return token::TOK_IMPORT; }
"from"                                  { return token::TOK_FROM; }
"as"                                    { return token::TOK_AS; }
"sizeof"                                { return token::TOK_SIZE_OF; }
"alignof"                               { return token::TOK_ALIGN_OF; }
"alignas"                               { return token::TOK_ALIGN_AS; }
"nullptr"                               { return token::TOK_NULL_PTR; }



[0-9]{4}-[0-9]{2}-[0-9]{2}T[0-9]{2}:[0-9]{2}:[0-9]{2} { return interpretDateTimeLiteral(yylval, yytext, driver); }

[0-9]{4}-[0-9]{2}-[0-9]{2}                            { return interpretDateLiteral(yylval, yytext, driver);}


([0-9]+\.[0-9]*|\.[0-9]+)[fF]?([eE][+-]?[0-9]+)?    { return interpretFloatLiteral(yylval, yytext, driver); }
([0-9]+\.[0-9]*|\.[0-9]+)([eE][+-]?[0-9]+)?         { return interpretDoubleLiteral(yylval, yytext, driver); }

[0-9]+[bB]                                          { return interpretByteLiteral(yylval, yytext, driver); }
[0-9]+[sS]                                          { return interpretShortLiteral(yylval, yytext, driver); }
[0-9]+[lL]                                          { return interpretLongLiteral(yylval, yytext, driver); }
[0-9]+                                              { return interpretNumberLiteral(yylval, yytext, driver); }


'[^\'\\]'                                           { return interpretCharLiteral(yylval, yytext, driver); }
\"([^\"\\]|\\.)*\"                                  { return interpretStringLiteral(yylval, yytext, yyleng, driver); }
true|false|0|1                                      { return interpretBooleanLiteral(yylval, yytext, driver); }
[a-zA-Z_][a-zA-Z0-9_]*                              { return interpretKeywordsAndSpecifiers(yylval, yytext, driver); }



"+"                                     { return token::TOK_PLUS; }
"-"                                     { return token::TOK_MINUS; }
"*"                                     { return token::TOK_MULTIPLY; }
"/"                                     { return token::TOK_DIVIDE; }
"%"                                     { return token::TOK_MODULO; }

"="                                     { lexer_state = STATE_EXPR; return token::TOK_ASSIGN; }
"["                                     { return token::TOK_LBRACKET; }
"]"                                     { return token::TOK_RBRACKET; }

"{"                                     { 
                                            if (lexer_state == STATE_COMPOUND) {
                                                std::cerr << "Initialize compound expression" << std::endl;
                                                return token::TOK_LBRACE_COMPOUND;
                                            } else if (lexer_state == STATE_EXPR) {
                                                std::cerr << "Initialize literal expression" << std::endl;
                                                return token::TOK_LBRACE_LITERAL;
                                            } else {
                                                std::cerr << "Initialize literal expression" << std::endl;
                                                return token::TOK_LBRACE_COMPOUND;
                                            }
                                        }

"}"                                     { lexer_state = STATE_COMPOUND; return token::TOK_RBRACE; }
"("                                     { lexer_state = STATE_COMPOUND; return token::TOK_LPAREN; }
")"                                     { return token::TOK_RPAREN; }

"?"                                     { return token::TOK_QUESTION_MARK; }
"."                                     { return token::TOK_DOT; }  

","                                     { return token::TOK_COMMA; }  
";"                                     { return token::TOK_SEMICOLON; }  
":"                                     { return token::TOK_COLON; }
"::"                                    { return token::TOK_DOUBLE_COLON; }

"<<"                                    { return token::TOK_LSHIFT; }
">>"                                    { return token::TOK_RSHIFT; }

"&"                                    { return token::TOK_BIT_AND; }
"|"                                    { return token::TOK_BIT_OR; }
"^"                                    { return token::TOK_BITWISE_XOR; }


"<<="                                   { return token::TOK_LSHIFT_ASSIGN; }
">>="                                   { return token::TOK_RSHIFT_ASSIGN; }
"&="                                    { return token::TOK_AND_ASSIGN; }
"^="                                    { return token::TOK_XOR_ASSIGN; }


">>>"                                   { unput('>'); unput('>'); return token::TOK_GREATER_THAN; }
">>"                                    { unput('>'); return token::TOK_GREATER_THAN; }
">"                                     { return token::TOK_GREATER_THAN; }

"<"                                     { return token::TOK_LESS_THAN; }

">="                                    { return token::TOK_GTE; }
"<="                                    { return token::TOK_LTE; }
"=="                                    { return token::TOK_EQUALITY; }
"!="                                    { return token::TOK_NEQUALITY; }

"&&"                                    { return token::TOK_L_AND; }
"||"                                    { return token::TOK_L_OR; }


"++"                                    { return token::TOK_INCREMENT; }  
"--"                                    { return token::TOK_DECREMENT; }  
"!"                                     { return token::TOK_LOGICAL_NOT; }  
"??"                                    { return token::TOK_NULL_COALESCE; }  


"+="                                    { return token::TOK_PLUS_ASSIGN; }
"-="                                    { return token::TOK_MINUS_ASSIGN; }
"*="                                    { return token::TOK_MULTIPLY_ASSIGN; }
"/="                                    { return token::TOK_DIVIDE_ASSIGN; }
"%="                                    { return token::TOK_MODULO_ASSIGN; }

[ \t]+                                  { STEP(); /* Handle whitespace */}

[\n\r]+                                 { STEP(); LINE(1); /* Handle newlines */}

"//".*                                  { /* Skip single-line comment */ }
"/*"                                    { BEGIN(COMMENT); }
<COMMENT>"*/"                           { BEGIN(INITIAL); }
<COMMENT>.|\n                           { /* Skip comment content */ }
.|\n                                    { /* Catch-all: skip unknown tokens */ }

%%


namespace LynxLang {
    LynxScanner::LynxScanner() : LynxLangFlexLexer() {}

    LynxScanner::~LynxScanner() {}

    void LynxScanner::setDebug(bool b) { yy_flex_debug = b; }
}

/* This implementation of LynxFlexLexer::yylex() is required to fill the
 * vtable of the class LynxFlexLexer. We define the scanner's main yylex
 * function via YY_DECL to reside in the Scanner class instead. */
int LynxLangFlexLexer::yylex() {
    std::cerr << "call LynxLangFlexLexer::yylex()!" << std::endl;
    throw "Lexer returned some errors";
    return 0;
}


/* When the scanner receives an end-of-file indication from YY_INPUT, it then
 * checks the yywrap() function. If yywrap() returns false (zero), then it is
 * assumed that the function has gone ahead and set up `yyin' to point to
 * another input file, and scanning continues. If it returns true (non-zero),
 * then the scanner terminates, returning 0 to its caller. */
int LynxLangFlexLexer::yywrap() {
    return 1; // EOF handling
}