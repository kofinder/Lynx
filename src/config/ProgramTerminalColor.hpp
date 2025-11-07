/**
 * @file ProgramTerminalColor.hpp
 * @brief Provides ANSI escape sequences for terminal text coloring and styling.
 * 
 * This header defines a comprehensive set of macros for foreground colors, background colors,
 * and text styles (bold, underline, blink, reverse, hidden) compatible with most UNIX-like terminals.
 * It is intended to be used for formatting console output in Lynx programs, CLI tools, and utilities.
 * 
 * **Features:**
 * - Standard and bright foreground colors.
 * - Standard and bright background colors.
 * - Text styles: bold, dim, underline, blink, reverse, hidden.
 * - Reset macro to revert all styles.
 * - Separator macro for visually dividing terminal sections.
 * 
 * **Usage Example:**
 * ```cpp
 * std::cout << LYNX_BOLD << LYNX_RED << "Error: File not found!" << LYNX_RESET << std::endl;
 * ```
 * 
 * Author: Ko Thein (Nathan Mratt)
 * Date: November 2, 2024
 */


#ifndef LYNX_PROGRAM_TERMINAL_COLOR_HPP
#define LYNX_PROGRAM_TERMINAL_COLOR_HPP

// ──────────────────────────────────────────────────────────────
//  ProgramTerminalColor.hpp (Safe Version with Prefixes)
//  Terminal ANSI escape sequences for coloring and styling text.
//  Compatible with most UNIX-like terminals.
// ──────────────────────────────────────────────────────────────

#define LYNX_SEPARATOR() "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"

// ── Reset ──────────────────────────────────────────────────────

#define LYNX_RESET           "\033[0m"
#define LYNX_BOLD            "\033[1m"
#define LYNX_DIM             "\033[2m"
#define LYNX_UNDERLINE       "\033[4m"
#define LYNX_BLINK           "\033[5m"
#define LYNX_REVERSE         "\033[7m"
#define LYNX_HIDDEN          "\033[8m"

// ── Standard Foreground Colors ─────────────────────────────────
#define LYNX_BLACK           "\033[30m"
#define LYNX_RED             "\033[31m"
#define LYNX_GREEN           "\033[32m"
#define LYNX_YELLOW          "\033[33m"
#define LYNX_BLUE            "\033[34m"
#define LYNX_MAGENTA         "\033[35m"
#define LYNX_CYAN            "\033[36m"
#define LYNX_WHITE           "\033[37m"

// ── Bright Foreground Colors ──────────────────────────────────
#define LYNX_BRIGHT_BLACK    "\033[90m"
#define LYNX_BRIGHT_RED      "\033[91m"
#define LYNX_BRIGHT_GREEN    "\033[92m"
#define LYNX_BRIGHT_YELLOW   "\033[93m"
#define LYNX_BRIGHT_BLUE     "\033[94m"
#define LYNX_BRIGHT_MAGENTA  "\033[95m"
#define LYNX_BRIGHT_CYAN     "\033[96m"
#define LYNX_BRIGHT_WHITE    "\033[97m"

// ── Standard Background Colors ────────────────────────────────
#define LYNX_BG_BLACK        "\033[40m"
#define LYNX_BG_RED          "\033[41m"
#define LYNX_BG_GREEN        "\033[42m"
#define LYNX_BG_YELLOW       "\033[43m"
#define LYNX_BG_BLUE         "\033[44m"
#define LYNX_BG_MAGENTA      "\033[45m"
#define LYNX_BG_CYAN         "\033[46m"
#define LYNX_BG_WHITE        "\033[47m"

// ── Bright Background Colors ──────────────────────────────────
#define LYNX_BG_BRIGHT_BLACK    "\033[100m"
#define LYNX_BG_BRIGHT_RED      "\033[101m"
#define LYNX_BG_BRIGHT_GREEN    "\033[102m"
#define LYNX_BG_BRIGHT_YELLOW   "\033[103m"
#define LYNX_BG_BRIGHT_BLUE     "\033[104m"
#define LYNX_BG_BRIGHT_MAGENTA  "\033[105m"
#define LYNX_BG_BRIGHT_CYAN     "\033[106m"
#define LYNX_BG_BRIGHT_WHITE    "\033[107m"

#endif // LYNX_PROGRAM_TERMINAL_COLOR_HPP
