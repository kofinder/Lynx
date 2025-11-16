/**
 * @file TypeMethodMapConstants.hpp
 * @brief Defines method signature maps for built-in Lynx types.
 *
 * This header provides compile-time constant maps associating method names with their expected
 * argument counts for all fundamental Lynx types—including numeric types, strings, characters, 
 * and booleans. These maps assist the resolver in validating method calls and selecting the 
 * appropriate strategy for execution.
 * 
 * This file consolidates method metadata for the type system, keeping resolver logic clean and
 * minimizing duplication across type implementations.
 *
 * author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_TYPE_METHOD_MAP_CONSTANTS_HPP
#define LYNX_TYPE_METHOD_MAP_CONSTANTS_HPP

#include <unordered_map>
#include <string>
#include "TypeMethodKeyConstants.hpp"

namespace LynxTypes {
    
    // -----------------------------
    // Byte type methods
    // -----------------------------
    static inline const std::unordered_map<std::string_view, int> byteMethods {
        {absKey, 0}, {negateKey, 0}, {signKey, 0}, {clampKey, 2},
        {isEvenKey, 0}, {isOddKey, 0}, {fromStringKey, 1},
        {zeroKey, 0}, {oneKey, 0}, {maxKey, 0}, {minKey, 0}
    };

    // -----------------------------
    // Short type methods
    // -----------------------------
    static inline const std::unordered_map<std::string_view, int> shortMethods = byteMethods;

    // -----------------------------
    // Integer type methods
    // -----------------------------
    static inline const std::unordered_map<std::string_view, int> intMethods {
        {addKey, 1}, {absKey, 0}, {negateKey, 0}, {signKey, 0}, {clampKey, 2},
        {isEvenKey, 0}, {isOddKey, 0}, {fromStringKey, 1},
        {zeroKey, 0}, {oneKey, 0}, {maxKey, 0}, {minKey, 0},
        {isPositiveKey, 0}, {isNegativeKey, 0}, {bitCountKey, 0},
        {leadingZerosKey, 0}, {trailingZerosKey, 0}
    };

    // -----------------------------
    // Long type methods
    // -----------------------------
    static inline const std::unordered_map<std::string_view, int> longMethods = intMethods;

    // -----------------------------
    // Float type methods
    // -----------------------------
    static inline const std::unordered_map<std::string_view, int> floatMethods {
        {absKey, 0}, {negateKey, 0}, {signKey, 0}, {clampKey, 2},
        {fromStringKey, 1}, {zeroKey, 0}, {oneKey, 0},
        {maxKey, 0}, {minKey, 0}, {isPositiveKey, 0}, {isNegativeKey, 0},
        {ceilKey, 0}, {floorKey, 0}, {roundKey, 0}, {truncKey, 0}
    };

    // -----------------------------
    // Double type methods
    // -----------------------------
    static inline const std::unordered_map<std::string_view, int> doubleMethods = floatMethods;

    // -----------------------------
    // String type methods
    // -----------------------------
    static inline const std::unordered_map<std::string_view, int> stringMethods {
        {lengthKey, 0}, {isEmptyKey, 0}, {charAtKey, 1}, {substringKey, 2},
        {toUpperCaseKey, 0}, {toLowerCaseKey, 0}, {containsKey, 1}, {startsWithKey, 1},
        {endsWithKey, 1}, {replaceKey, 2}, {splitKey, 1}, {trimKey, 0},
        {concatKey, 1}, {fromCharKey, 1}, {emptyKey, 0}, {formatKey, 1}, {joinKey, 2}
    };

    // -----------------------------
    // Char type methods
    // -----------------------------
    static inline const std::unordered_map<std::string_view, int> charMethods {
        {toUpperCaseKey, 0}, {toLowerCaseKey, 0}, {isDigitKey, 0},
        {isLetterKey, 0}, {isLetterOrDigitKey, 0}, {isWhitespaceKey, 0}, {fromStringKey, 1},
        {minKey, 0}, {maxKey, 0}
    };

    // -----------------------------
    // Boolean type methods
    // -----------------------------
    static inline const std::unordered_map<std::string_view, int> boolMethods {
        {negateKey, 0}, {toStringKey, 0}, {TRUEKey, 0}, {FALSEKey, 0}
    };


}

#endif
