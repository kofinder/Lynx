#ifndef LYNX_SCANNER_HPP
#define LYNX_SCANNER_HPP

#include "parser.hpp"  // Ensure Parser is defined in LynxLang namespace
#include "LynxDriver.hpp" // Include LynxDriver if not already included in parser.hpp
#include "LexerState.hpp"

// Include FlexLexer and define yyFlexLexer to avoid conflicts
#ifndef __FLEX_LEXER_H
#define yyFlexLexer LynxLangFlexLexer // Renaming FlexLexer class
#include <FlexLexer.h>
#undef yyFlexLexer
#endif

namespace LynxLang {
    class LynxScanner : public LynxLangFlexLexer {

        public:
        
            LexerState lexer_state = STATE_DEFAULT;
    
            LynxScanner();

            virtual ~LynxScanner();

            void setDebug(bool b);

            using LynxLangFlexLexer::yylex;

            virtual int yylex(Parser::semantic_type* yylval, Parser::location_type* yylloc, LynxDriver& driver);

            void setLexerState(LexerState state) { lexer_state = state; }

            LexerState getLexerState() const { return lexer_state; }
        
    };
}

#endif // LYNX_SCANNER_HPP