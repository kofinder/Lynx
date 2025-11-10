/**
 * @file lynx_runtime_io.h
 * @brief Declares the Lynx runtime I/O functions for basic type input and output.
 * 
 * This header provides a set of C ABI-compatible functions to handle input and output
 * operations for the Lynx programming language, including support for primitive types
 * such as byte, short, int, long, float, double, char, string, and boolean.
 * 
 * **Key Responsibilities:**
 * - Declare functions to read values from standard input for each supported type.
 * - Declare functions to print values to standard output for each supported type.
 * - Provide general formatted printing and string output utilities.
 * - Ensure compatibility with LLVM IR-generated calls in the Lynx compiler.
 * 
 * **Used By:**
 * - The Lynx compiler’s LLVM IR code generator for system I/O commands.
 * - Runtime programs executed by the Lynx runtime library.
 * 
 * **Naming Convention:**
 * - All functions use upper snake case prefixed with `LYNX_IO_` for consistency.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_RUNTIME_IO_H
#define LYNX_RUNTIME_IO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <cstring>
#include <limits>

/**
 * @brief Reads a validated char from input (only allowed characters).
 * @param allowedChars Null-terminated string of allowed characters.
 */
char IO_GET_CHAR_VALIDATED(const char* allowedChars);


#ifdef __cplusplus
}
#endif

#endif