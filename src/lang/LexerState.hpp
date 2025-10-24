#ifndef LEXER_STATE_HPP
#define LEXER_STATE_HPP

enum LexerState {
    STATE_DEFAULT,
    STATE_EXPR,
    STATE_COMPOUND,
};

// Declare lexer_state as extern so parser can see it
extern LexerState lexer_state;

#endif
