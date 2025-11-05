/**
 * @file ProgramSyntaxColor.hpp
 * @brief Defines ANSI color codes for syntax highlighting in Lynx programs.
 * 
 * This header provides a set of macros for safely coloring different elements of Lynx source code
 * when outputting to the terminal. It is intended for use in program visualization, REPLs, logs,
 * or any textual output that benefits from syntax differentiation.
 * 
 * * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
 */

#ifndef LYNX_PROGRAM_SYNTAX_COLOR_HPP
#define LYNX_PROGRAM_SYNTAX_COLOR_HPP

// ─────────────────────────────────────────────
//  ProgramSyntaxColor.hpp (Safe Version)
//  Colors for syntax highlighting in your programs.
//  Use these prefixed macros for coloring syntax elements.
// ─────────────────────────────────────────────

// Keywords (e.g., if, else, for, while)
#define LYNX_SYNTAX_KEYWORD       "\033[95m"   /* Magenta */

// Types (e.g., int, float, char)
#define LYNX_SYNTAX_TYPE          "\033[94m"   /* Blue */

// Strings (e.g., "Hello World")
#define LYNX_SYNTAX_STRING        "\033[92m"   /* Green */

// Numbers (e.g., 123, 3.14)
#define LYNX_SYNTAX_NUMBER        "\033[96m"   /* Cyan */

// Comments (e.g., // comment or /* comment */)
#define LYNX_SYNTAX_COMMENT       "\033[90m"   /* Bright Black (Gray) */

// Function names and identifiers
#define LYNX_SYNTAX_FUNCTION      "\033[93m"   /* Yellow */

// Operators and punctuation
#define LYNX_SYNTAX_OPERATOR      "\033[91m"   /* Red */

// Preprocessor directives (e.g., #include, #define)
#define LYNX_SYNTAX_PREPROCESSOR  "\033[33m"   /* Brown/Yellow */

// Classes
#define LYNX_SYNTAX_CLASS         "\033[38;5;208m" /* Orange */

// Enums
#define LYNX_SYNTAX_ENUM          "\033[38;5;214m" /* Gold */

// Interfaces
#define LYNX_SYNTAX_INTERFACE     "\033[38;5;39m"  /* Deep Sky Blue */

// Lambda expressions (usually treated like functions)
#define LYNX_SYNTAX_LAMBDA        "\033[35m"       /* Purple */

// Reset to default color
#define LYNX_SYNTAX_RESET         "\033[0m"

#endif // LYNX_PROGRAM_SYNTAX_COLOR_HPP
