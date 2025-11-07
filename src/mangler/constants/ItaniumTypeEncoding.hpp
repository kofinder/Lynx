/**
 * @file ItaniumTypeEncoding.hpp
 * @brief Defines string constants for Itanium C++ ABI type encoding.
 *
 * This header provides the canonical encodings for primitive types, user-defined
 * types, container types, and other common constructs as used in Itanium C++ name
 * mangling. These constants are referenced when constructing mangled names for
 * functions, methods, classes, and templates.
 *
 * Categories of Constants:
 * - **Fundamental Types:** VOID, BOOL, BYTE, CHAR, STRING, SHORT, INT, LONG, FLOAT, DOUBLE.
 * - **Date/Time and Other Special Types:** DATE, DATETIME, ENUM, CLAZZ, INTERFACE, FILE, AUTO.
 * - **Containers and Collections:** COLLECTION, ARRAY, VECTOR, LIST, SET, MAP, STACK, QUEUE, TREE, GRAPH, HEAP, DICT.
 * - **Pointers, References, and Functions:** FUNCTION, REFERENCE, POINTER, LVALUE_REF, RVALUE_REF.
 * - **Signed/Unsigned Variants and Extended Types:** SIGNED_CHAR, UNSIGNED_CHAR, UNSIGNED_SHORT, UNSIGNED_INT, UNSIGNED_LONG, LONG_LONG, UNSIGNED_LONG_LONG, LONG_DOUBLE, FLOAT128, PPC_DOUBLE_DOUBLE.
 * - **Struct and Token Encodings:** STRUCT_PREFIX, STRUCT_SUFFIX, TOKEN.
 * - **Function Start/End Encodings:** FUNCTION_START, FUNCTION_END.
 *
 * Example Usage:
 * @code
 * std::string typeCode = INT;  // Encodes 'int' type
 * std::string ptrCode = POINTER + INT; // Encodes 'int*'
 * @endcode
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_ITANIUM_TYPE_ENCODING_HPP
#define LYNX_ITANIUM_TYPE_ENCODING_HPP

namespace LynxMangler {

    // Fundamental types
    constexpr const char* VOID       = "v";
    constexpr const char* BOOL       = "b";
    constexpr const char* BYTE       = "h"; 
    constexpr const char* CHAR       = "c";
    constexpr const char* STRING     = "Ss";
    constexpr const char* SHORT      = "s";
    constexpr const char* INT        = "i";
    constexpr const char* LONG       = "l";
    constexpr const char* FLOAT      = "f";
    constexpr const char* DOUBLE     = "d";

    constexpr const char* DATE       = "Dt";
    constexpr const char* DATETIME   = "DT";
    constexpr const char* ENUM       = "E";
    constexpr const char* CLAZZ      = "C";
    constexpr const char* INTERFACE  = "I";
    constexpr const char* FILE       = "F";
    constexpr const char* AUTO       = "A";
    constexpr const char* COLLECTION = "Co";
    constexpr const char* ARRAY      = "Ar";
    constexpr const char* VECTOR     = "VR";
    constexpr const char* LIST       = "L";
    constexpr const char* SET        = "S";
    constexpr const char* MAP        = "M";
    constexpr const char* STACK      = "St";
    constexpr const char* QUEUE      = "Q";
    constexpr const char* TREE       = "T";
    constexpr const char* GRAPH      = "G";
    constexpr const char* HEAP       = "Hp";
    constexpr const char* DICT       = "D";
    constexpr const char* FUNCTION   = "Fn";
    constexpr const char* REFERENCE  = "R";
    constexpr const char* POINTER    = "P";
    constexpr const char* OTHER      = "O";
    constexpr const char* UNKNOWN    = "V";    


    constexpr const char* SIGNED_CHAR          = "a";
    constexpr const char* UNSIGNED_CHAR        = "h";
    constexpr const char* UNSIGNED_SHORT       = "t";
    constexpr const char* UNSIGNED_INT         = "j";
    constexpr const char* UNSIGNED_LONG        = "m";
    constexpr const char* LONG_LONG            = "x";
    constexpr const char* UNSIGNED_LONG_LONG   = "y";
    constexpr const char* LONG_DOUBLE          = "e";
    constexpr const char* FLOAT128             = "g";
    constexpr const char* PPC_DOUBLE_DOUBLE    = "G";

    // Function types
    constexpr const char* FUNCTION_START       = "F";
    constexpr const char* FUNCTION_END         = "E";

    // Other types
    constexpr const char* LVALUE_REF           = "R";
    constexpr const char* RVALUE_REF           = "O";
    constexpr const char* STRUCT_PREFIX        = "N"; 
    constexpr const char* STRUCT_SUFFIX        = "E";
    constexpr const char* TOKEN                = "T";
}

#endif
