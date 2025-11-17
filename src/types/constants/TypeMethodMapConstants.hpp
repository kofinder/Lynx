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
        // ============================================================================
        // ABS Methods Mapping
        // ============================================================================
        {absKey, 0}, {negateKey, 0}, {signKey, 0}, {clampKey, 2}, {isEvenKey, 0}, {isOddKey, 0}, 

        // ============================================================================
        // Arithmetic Methods Mapping
        // ============================================================================
        {addKey, 1}, {subKey, 1}, {mulKey, 1}, {divKey, 1}, {modKey, 1},

        // ============================================================================
        // Bit manipulation Methods Mapping
        // ============================================================================
        {popCountKey, 1}, {countLeadingZerosKey, 1}, {countTrailingZerosKey, 1}, {bitReverseKey, 1}, {rotateLeftKey, 1}, {rotateRightKey, 1},

        // ============================================================================
        // Bitwise logic Methods Mapping
        // ============================================================================
        {bitAndKey, 1}, {bitOrKey, 1}, {bitXorKey, 1}, {shlKey, 1}, {shrKey, 1}, {bitNotKey, 1},

        // ============================================================================
        // Comparison Methods Mapping
        // ============================================================================
        {eqKey, 1}, {neKey, 1}, {ltKey, 1}, {leKey, 1}, {gtKey, 1}, {geKey, 1},

        // ============================================================================
        // Fixed Point Methods Mapping
        // ============================================================================
        {fpMulKey, 1}, {fpUMulKey, 1}, {fpDivKey, 1}, {fpUDivKey, 1},

        // ============================================================================
        // Math Methods Mapping
        // ============================================================================
        {sqrtKey, 1}, {powKey, 1}, {expKey, 1}, {exp2Key, 1}, {exp10Key, 1}, {logKey, 1}, {log2Key, 1}, {log10Key, 1}, {sinKey, 1}, {cosKey, 1}, {tanKey, 1},
        {asinKey, 1}, {acosKey, 1}, {atanKey, 1}, {atan2Key, 1}, {sinhKey, 1}, {coshKey, 1}, {tanhKey, 1}, {floorKey, 1}, {ceilKey, 1}, {truncKey, 1}, {roundKey, 1}, {fabsKey, 1},

        // ============================================================================
        // Memory Methods Mapping
        // ============================================================================
        {memcpyKey, 1}, {memmoveKey, 1}, {memsetKey, 1}, {memsetPatternKey, 1},

        // ============================================================================
        // MinMax Methods Mapping
        // ============================================================================
        {sminKey,1}, {smaxKey, 1}, {uminKey, 1}, {umaxKey, 1},

        // ============================================================================
        // Overflow Methods Mapping
        // ============================================================================
        {saddOverflowKey, 1}, {uaddOverflowKey, 1}, {ssubOverflowKey, 1}, {usubOverflowKey, 1}, {smulOverflowKey, 1}, {umulOverflowKey, 1},

        // ============================================================================
        // Saturation Methods Mapping
        // ============================================================================
        {saddSatKey, 1}, {uaddSatKey, 1}, {ssubSatKey, 1}, {usubSatKey, 1}, {sshlSatKey, 1}, {ushLSatKey, 1},

        // ============================================================================
        // Others specialization
        // ============================================================================
        {fromStringKey, 1}, {zeroKey, 0}, {oneKey, 0}, {maxKey, 0}, {minKey, 0},
        {isPositiveKey, 0}, {isNegativeKey, 0}, {bitCountKey, 0}, {leadingZerosKey, 0}, 
        {trailingZerosKey, 0}
    };

    // -----------------------------
    // Long type methods
    // -----------------------------
    static inline const std::unordered_map<std::string_view, int> longMethods = intMethods;

    // -----------------------------
    // Float type methods
    // -----------------------------
    static inline const std::unordered_map<std::string_view, int> floatMethods {
        {addKey, 1}, {subKey, 1}, {mulKey, 1}, {divKey, 1}, {modKey, 1},
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
