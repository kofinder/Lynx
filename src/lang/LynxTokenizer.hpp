#ifndef LYNX_TOKENIZER_HPP
#define LYNX_TOKENIZER_HPP

#include <sstream>
#include <iomanip>
#include <unordered_set>
#include <cstdio>
#include <regex>
#include <limits>
#include <stdexcept>
#include <parser.hpp>
#include <LynxScanner.hpp>
#include <constants/LValueType.hpp>
#include <constants/runtimetypes/DateDataType.hpp>
#include <constants/runtimetypes/DateTimeDataType.hpp>


// Define macros for stepping and managing scanner location
#define STEP() driver.cursor->step();
#define COL(Col) driver.cursor->columns(Col);
#define LINE(Line) driver.cursor->lines(Line);
#define YY_USER_ACTION COL(yyleng);

typedef LynxLang::Parser::token token;
typedef LynxLang::Parser::token_type token_type;

#define yyterminate() return token::TOK_EOF
#define IS_TOKEN(a) strcmp(a, yytext) == 0

void yyerror(const char* message) {
    fprintf(stderr, "Error: %s\n", message);
}

bool isCharLiteral(const std::string& yytext) {
    return yytext.length() >= 3 && yytext.front() == '\'' && yytext.back() == '\'';
}

int interpretDateTimeLiteral(LynxLang::Parser::semantic_type* yylval, const char* yytext, LynxLang::LynxDriver& driver);
int interpretDateLiteral(LynxLang::Parser::semantic_type* yylval, const char* yytext, LynxLang::LynxDriver& driver);
int interpretShortLiteral(LynxLang::Parser::semantic_type* yylval, const char* yytext, LynxLang::LynxDriver& driver);
int interpretIntLiteral(LynxLang::Parser::semantic_type* yylval, const char* yytext, LynxLang::LynxDriver& driver);
int interpretLongLiteral(LynxLang::Parser::semantic_type* yylval, const char* yytext, LynxLang::LynxDriver& driver);
int interpretBooleanLiteral(LynxLang::Parser::semantic_type* yylval, const char* yytext, LynxLang::LynxDriver& driver);
int interpretFloatLiteral(LynxLang::Parser::semantic_type* yylval, const char* yytext, LynxLang::LynxDriver& driver);
int interpretDoubleLiteral(LynxLang::Parser::semantic_type* yylval, const char* yytext, LynxLang::LynxDriver& driver);
int interpretCharLiteral(LynxLang::Parser::semantic_type* yylval, const char* yytext, LynxLang::LynxDriver& driver);


int interpretNumberLiteral(LynxLang::Parser::semantic_type* yylval, const char* yytext, LynxLang::LynxDriver& driver) {
    DataType expectedType = driver.getExpectedType();
    
    std::cerr << "interpret number literal expected type ===>" << toString(driver.getExpectedType()) << std::endl;

    if (expectedType == DataType::SHORT) {
        return interpretShortLiteral(yylval, yytext, driver);
    } else if (expectedType == DataType::INT) {
        return interpretIntLiteral(yylval, yytext, driver);
    } else if (expectedType == DataType::LONG) {
        return interpretLongLiteral(yylval, yytext, driver);
    } else if (expectedType == DataType::CHAR && isCharLiteral(yytext)) {
        return interpretCharLiteral(yylval, yytext, driver);
    } 
    return interpretIntLiteral(yylval, yytext, driver);
}

int interpretByteLiteral(LynxLang::Parser::semantic_type* yylval, const char* yytext, LynxLang::LynxDriver&) {
    try {
        auto value = static_cast<uint8_t>(std::stoul(yytext));
        yylval->emplace<uint8_t>(value);
        return token::TOK_BYTE;
    } catch (...) {
        yyerror("Unknown error occurred while interpreting BYTE literal");
        return -1;
    }
}

int interpretShortLiteral(LynxLang::Parser::semantic_type* yylval, const char* yytext, LynxLang::LynxDriver&) {
    try {
        long value = std::stol(yytext);
        if (value < std::numeric_limits<short>::min() || value > std::numeric_limits<short>::max()) {
            yyerror("Short literal out of range");
            return -1;
        }
        yylval->emplace<short>(static_cast<short>(value));
        return token::TOK_SHORT;
    } catch (...) {
        yyerror("Invalid Short literal format or out of range");
        return -1;
    }
}

int interpretIntLiteral(LynxLang::Parser::semantic_type* yylval, const char* yytext, LynxLang::LynxDriver&) {
    try {
        int value = std::stoi(yytext);
        yylval->emplace<int>(value);
        return token::TOK_INTEGER;
    } catch (...) {
        yyerror("Invalid Integer literal format or out of range");
        return -1;
    }
}

int interpretLongLiteral(LynxLang::Parser::semantic_type* yylval, const char* yytext, LynxLang::LynxDriver&) {
    try {
        long value = std::stol(yytext);
        yylval->emplace<long>(value);
        return token::TOK_LONG;
    } catch (...) {
        yyerror("Invalid Long literal format or out of range");
        return -1;
    }
}

int interpretFloatLiteral(LynxLang::Parser::semantic_type* yylval, const char* yytext, LynxLang::LynxDriver& driver) {
    try {
        if (driver.getExpectedType() == DataType::DOUBLE) {
            double value = std::stod(yytext);
            yylval->emplace<double>(value);
            return token::TOK_DOUBLE;
        } else {
            float value = std::stof(yytext);
            yylval->emplace<float>(value);
            return token::TOK_FLOAT;
        }
    } catch (...) {
        yyerror("Invalid float/double literal");
        return -1;
    }
}

int interpretDoubleLiteral(LynxLang::Parser::semantic_type* yylval, const char* yytext, LynxLang::LynxDriver& driver) {
    try {
        if (driver.getExpectedType() == DataType::FLOAT) {
            float value = std::stof(yytext);
            yylval->emplace<float>(value);
            return token::TOK_FLOAT;
        } else {
            double value = std::stod(yytext);
            yylval->emplace<double>(value);
            return token::TOK_DOUBLE;
        }
    } catch (...) {
        yyerror("Invalid double/float literal");
        return -1;
    }
}

int interpretCharLiteral(LynxLang::Parser::semantic_type* yylval, const char* yytext, LynxLang::LynxDriver&) {
    try {
        int len = strlen(yytext);
        if(len > 3) {
            yyerror("Invalid char literal");
            return -1; 
        }
        yylval->emplace<char>(yytext[1]);
        return token::TOK_CHAR;    
    } catch (...) {
        std::cerr << "Char literal error: " << "\n";
        yyerror("Invalid char literal");
        return -1;
    }
}

int interpretBooleanLiteral(LynxLang::Parser::semantic_type* yylval, const char* yytext, LynxLang::LynxDriver&) {
    try {
        bool value;
        if (strcmp(yytext, "true") == 0 || strcmp(yytext, "1") == 0) {
            value = true;
        } else if (strcmp(yytext, "false") == 0 || strcmp(yytext, "0") == 0) {
            value = false;
        } else {
            yyerror("Invalid boolean literal. Expected 'true', 'false', '0', or '1'.");
            return -1;
        }
        yylval->emplace<bool>(value);
        return token::TOK_BOOLEAN;
    } catch (...) {
        yyerror("Unknown error while interpreting Boolean literal");
        return -1;
    }
}

int interpretStringLiteral(LynxLang::Parser::semantic_type* yylval, const char* yytext, int yyleng, LynxLang::LynxDriver&) {
    try {
        std::string value(yytext + 1, yyleng - 2);  // Strip quotes
        yylval->emplace<std::string>(std::move(value));
        return token::TOK_STRING;
    } catch (...) {
        yyerror("Unknown error occurred while interpreting String literal");
        return -1;
    }
}

int interpretDateTimeLiteral(LynxLang::Parser::semantic_type* yylval, const char* yytext, LynxLang::LynxDriver& driver) {
    using namespace LynxConstants;
    try {
        int y, m, d, h, min, s;
        sscanf(yytext, "%4d-%2d-%2dT%2d:%2d:%2d", &y, &m, &d, &h, &min, &s);
        yylval->emplace<DateTime>(DateTime{y, m, d, h, min, s });
        return token::TOK_DATE_TIME;
    } catch(...) {
        yyerror("Unknown error occurred while interpreting DATETIME literal");
        return -1;
    }
}


int interpretDateLiteral(LynxLang::Parser::semantic_type* yylval, const char* yytext, LynxLang::LynxDriver& driver) {
    using namespace LynxConstants;
    try {
        int y, m, d;
        sscanf(yytext, "%4d-%2d-%2d", &y, &m, &d);
        yylval->emplace<Date>(Date{y, m, d});
        return token::TOK_DATE;
    } catch(...) {
        yyerror("Unknown error occurred while interpreting DATE literal");
        return -1;
    }
}

int interpretKeywordsAndSpecifiers(LynxLang::Parser::semantic_type* yylval, const char* yytext, LynxLang::LynxDriver& driver) {
    if(IS_TOKEN("byte")) {
        driver.setExpectedType(DataType::BYTE);
        return token::KW_BYTE;
    } else if(IS_TOKEN("short")) {
        driver.setExpectedType(DataType::SHORT);
        return token::KW_SHORT;
    } else if(IS_TOKEN("int")) {
        driver.setExpectedType(DataType::INT);
        return token::KW_INT;
    } else if(IS_TOKEN("long")) {
        driver.setExpectedType(DataType::LONG);
        return token::KW_LONG;
    } else if(IS_TOKEN("float")) {
        driver.setExpectedType(DataType::FLOAT);
        return token::KW_FLOAT;
    } else if(IS_TOKEN("double")) {
        driver.setExpectedType(DataType::DOUBLE);
        return token::KW_DOUBLE;
    } else if(IS_TOKEN("boolean")) {
        driver.setExpectedType(DataType::BOOLEAN);
        return token::KW_BOOLEAN;
    } else if(IS_TOKEN("char")) {
        driver.setExpectedType(DataType::CHAR);
        return token::KW_CHAR;
    } else if(IS_TOKEN("string")) {
        driver.setExpectedType(DataType::STRING);
        return token::KW_STRING;
    } else if(IS_TOKEN("new")) {
        return token::TOK_NEW;
    } else if(IS_TOKEN("extends")) {
        return token::KW_EXTENDS;
    } else if(IS_TOKEN("implements")) {
        return token::KW_IMPLEMENTS;
    }  else if(IS_TOKEN("with")) {
        return token::KW_WITH;
    } else if(IS_TOKEN("super")) {
        return token::KW_SUPER;
    } else if(IS_TOKEN("override")) {
        return token::KW_OVERRIDE;
    } else if(IS_TOKEN("private")) {
        return token::KW_PRIVATE;
    } else if(IS_TOKEN("public")) {
        return token::KW_PUBLIC;
    } else if(IS_TOKEN("protected")) {
        return token::KW_PROTECTED;
    } else if(IS_TOKEN("static")) {
        return token::KW_STATIC;
    } else if(IS_TOKEN("const")) {
        return token::KW_CONST;
    } else if(IS_TOKEN("virtual")) {
        return token::KW_VIRTUAL;
    } else if(IS_TOKEN("inline")) {
        return token::KW_INLINE;
    } else if(IS_TOKEN("return")) {
        driver.setExpectedType(DataType::OTHER);
        return token::KW_RETURN;
    } else if(IS_TOKEN("try")) {
        return token::KW_TRY;
    } else if(IS_TOKEN("catch")) {
        return token::KW_CATCH;
    } else if(IS_TOKEN("finally")) {
        return token::KW_FINALLY;
    } else if(IS_TOKEN("throws")) {
        return token::KW_THROW;
    } else if(IS_TOKEN("this")) {
        return token::KW_THIS;
    } else if (IS_TOKEN("prettyPrint")) {
        return token::KW_LOG;
    } else if (IS_TOKEN("system")) {
        return token::KW_CONSOLE;
    } else if (IS_TOKEN("if")) {
        driver.setLexerState(LexerState::STATE_COMPOUND);
        return token::KW_IF;
    } else if (IS_TOKEN("else")) {
        driver.setLexerState(LexerState::STATE_COMPOUND);
        return token::KW_ELSE;
    } else if (IS_TOKEN("while")) {
        return token::KW_WHILE;
    } else if (IS_TOKEN("for")) {
        return token::KW_FOR;
    } else if(IS_TOKEN("switch")) {
        return token::KW_SWITCH;
    } else if(IS_TOKEN("case")) {
        driver.setLexerState(LexerState::STATE_COMPOUND);
        return token::KW_CASE;
    } else if(IS_TOKEN("break")) {
        driver.setLexerState(LexerState::STATE_COMPOUND);
        return token::KW_BREAK;
    } else if(IS_TOKEN("default")) {
        driver.setLexerState(LexerState::STATE_COMPOUND);
        return token::KW_DEFAULT;
    } else if(IS_TOKEN("enum")) {
        return token::KW_ENUM;
    } else if(IS_TOKEN("interface")) {
        return token::KW_INTERFACE;
    } else if(IS_TOKEN("mixin")) {
        return token::KW_MIXIN;
    } else if(IS_TOKEN("class")) {
        return token::KW_CLASS;
    } else if(IS_TOKEN("abstract")) {
        return token::KW_ABSTRACT;
    } else if(IS_TOKEN("void")) {
        return token::KW_VOID;
    } else if(IS_TOKEN("auto")) {
        return token::KW_AUTO;
    } else if(IS_TOKEN("on")) {
        return token::KW_ON;
    }

    yylval->emplace<std::string>(yytext);
    return token::TOK_IDENTIFIER;  
}

#endif
