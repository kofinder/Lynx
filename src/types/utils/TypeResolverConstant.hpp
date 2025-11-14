#ifndef LYNX_TYPE_METHOD_CONSTANTS_HPP
#define LYNX_TYPE_METHOD_CONSTANTS_HPP

#include <unordered_map>
#include <string>

namespace LynxTypes {

    // -----------------------------
    // Numeric method keys
    // -----------------------------
    inline constexpr const char* absKey = "abs";
    inline constexpr const char* negateKey = "negate";
    inline constexpr const char* signKey = "sign";
    inline constexpr const char* clampKey = "clamp";
    inline constexpr const char* isEvenKey = "isEven";
    inline constexpr const char* isOddKey = "isOdd";
    inline constexpr const char* fromStringKey = "fromString";
    inline constexpr const char* zeroKey = "zero";
    inline constexpr const char* oneKey = "one";
    inline constexpr const char* maxKey = "max";
    inline constexpr const char* minKey = "min";
    inline constexpr const char* isPositiveKey = "isPositive";
    inline constexpr const char* isNegativeKey = "isNegative";
    inline constexpr const char* bitCountKey = "bitCount";
    inline constexpr const char* leadingZerosKey = "leadingZeros";
    inline constexpr const char* trailingZerosKey = "trailingZeros";
    inline constexpr const char* ceilKey = "ceil";
    inline constexpr const char* floorKey = "floor";
    inline constexpr const char* roundKey = "round";
    inline constexpr const char* truncKey = "trunc";

    // -----------------------------
    // String method keys
    // -----------------------------
    inline constexpr const char* lengthKey = "length";
    inline constexpr const char* isEmptyKey = "isEmpty";
    inline constexpr const char* charAtKey = "charAt";
    inline constexpr const char* substringKey = "substring";
    inline constexpr const char* toUpperCaseKey = "toUpperCase";
    inline constexpr const char* toLowerCaseKey = "toLowerCase";
    inline constexpr const char* containsKey = "contains";
    inline constexpr const char* startsWithKey = "startsWith";
    inline constexpr const char* endsWithKey = "endsWith";
    inline constexpr const char* replaceKey = "replace";
    inline constexpr const char* splitKey = "split";
    inline constexpr const char* trimKey = "trim";
    inline constexpr const char* concatKey = "concat";
    inline constexpr const char* fromCharKey = "fromChar";
    inline constexpr const char* emptyKey = "empty";
    inline constexpr const char* formatKey = "format";
    inline constexpr const char* joinKey = "join";

    // -----------------------------
    // Char method keys
    // -----------------------------
    inline constexpr const char* isDigitKey = "isDigit";
    inline constexpr const char* isLetterKey = "isLetter";
    inline constexpr const char* isLetterOrDigitKey = "isLetterOrDigit";
    inline constexpr const char* isWhitespaceKey = "isWhitespace";

    // -----------------------------
    // Boolean method keys
    // -----------------------------
    inline constexpr const char* toStringKey = "toString";
    inline constexpr const char* TRUEKey = "TRUE";
    inline constexpr const char* FALSEKey = "FALSE";

    // -----------------------------
    // Byte type methods
    // -----------------------------
    static inline const std::unordered_map<std::string, int> byteMethods = {
        {absKey, 0}, {negateKey, 0}, {signKey, 0}, {clampKey, 2},
        {isEvenKey, 0}, {isOddKey, 0}, {fromStringKey, 1},
        {zeroKey, 0}, {oneKey, 0}, {maxKey, 0}, {minKey, 0}
    };

    // -----------------------------
    // Short type methods
    // -----------------------------
    static inline const std::unordered_map<std::string, int> shortMethods = byteMethods;

    // -----------------------------
    // Integer type methods
    // -----------------------------
    static inline const std::unordered_map<std::string, int> intMethods = {
        {absKey, 0}, {negateKey, 0}, {signKey, 0}, {clampKey, 2},
        {isEvenKey, 0}, {isOddKey, 0}, {fromStringKey, 1},
        {zeroKey, 0}, {oneKey, 0}, {maxKey, 0}, {minKey, 0},
        {isPositiveKey, 0}, {isNegativeKey, 0}, {bitCountKey, 0},
        {leadingZerosKey, 0}, {trailingZerosKey, 0}
    };

    // -----------------------------
    // Long type methods
    // -----------------------------
    static inline const std::unordered_map<std::string, int> longMethods = intMethods;

    // -----------------------------
    // Float type methods
    // -----------------------------
    static inline const std::unordered_map<std::string, int> floatMethods = {
        {absKey, 0}, {negateKey, 0}, {signKey, 0}, {clampKey, 2},
        {fromStringKey, 1}, {zeroKey, 0}, {oneKey, 0},
        {maxKey, 0}, {minKey, 0}, {isPositiveKey, 0}, {isNegativeKey, 0},
        {ceilKey, 0}, {floorKey, 0}, {roundKey, 0}, {truncKey, 0}
    };

    // -----------------------------
    // Double type methods
    // -----------------------------
    static inline const std::unordered_map<std::string, int> doubleMethods = floatMethods;

    // -----------------------------
    // String type methods
    // -----------------------------
    static inline const std::unordered_map<std::string, int> stringMethods = {
        {lengthKey, 0}, {isEmptyKey, 0}, {charAtKey, 1}, {substringKey, 2},
        {toUpperCaseKey, 0}, {toLowerCaseKey, 0}, {containsKey, 1}, {startsWithKey, 1},
        {endsWithKey, 1}, {replaceKey, 2}, {splitKey, 1}, {trimKey, 0},
        {concatKey, 1}, {fromCharKey, 1}, {emptyKey, 0}, {formatKey, 1}, {joinKey, 2}
    };

    // -----------------------------
    // Char type methods
    // -----------------------------
    static inline const std::unordered_map<std::string, int> charMethods = {
        {toUpperCaseKey, 0}, {toLowerCaseKey, 0}, {isDigitKey, 0},
        {isLetterKey, 0}, {isLetterOrDigitKey, 0}, {isWhitespaceKey, 0}, {fromStringKey, 1},
        {minKey, 0}, {maxKey, 0}
    };

    // -----------------------------
    // Boolean type methods
    // -----------------------------
    static inline const std::unordered_map<std::string, int> boolMethods = {
        {negateKey, 0}, {toStringKey, 0}, {TRUEKey, 0}, {FALSEKey, 0}
    };

} // namespace LynxTypes

#endif
