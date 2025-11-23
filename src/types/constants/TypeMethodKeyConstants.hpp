/**
 * @file TypeMethodKeyConstants.hpp
 * @brief Defines constant string keys representing all method names in the Lynx type system.
 *
 * This header consolidates every method identifier used across Lynx's numeric, string, character,
 * boolean, and strategy-based operations. These keys are used throughout the resolver and
 * strategy dispatch system to provide a consistent and centralized reference, minimizing 
 * hard-coded strings scattered across the codebase.
 *
 * The constants defined here cover numeric operations, arithmetic and bitwise logic, mathematical
 * functions, memory operations, fixed-point arithmetic, and high-level string and character
 * handling. By keeping these keys unified in one place, the system becomes easier to maintain,
 * extend, and validate.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_TYPE_METHOD_KEY_CONSTANTS_HPP
#define LYNX_TYPE_METHOD_KEY_CONSTANTS_HPP

#include <string_view>

namespace LynxTypes {

    // -----------------------------
    // Numeric method keys
    // -----------------------------
    inline constexpr std::string_view absKey       = "abs";
    inline constexpr std::string_view negateKey    = "negate";
    inline constexpr std::string_view signKey      = "sign";
    inline constexpr std::string_view clampKey     = "clamp";
    inline constexpr std::string_view isEvenKey    = "isEven";
    inline constexpr std::string_view isOddKey     = "isOdd";
    inline constexpr std::string_view fromStringKey= "fromString";
    inline constexpr std::string_view zeroKey      = "zero";
    inline constexpr std::string_view oneKey       = "one";
    inline constexpr std::string_view maxKey       = "max";
    inline constexpr std::string_view minKey       = "min";
    inline constexpr std::string_view isPositiveKey= "isPositive";
    inline constexpr std::string_view isNegativeKey= "isNegative";
    inline constexpr std::string_view bitCountKey  = "bitCount";
    inline constexpr std::string_view leadingZerosKey  = "leadingZeros";
    inline constexpr std::string_view trailingZerosKey = "trailingZeros";
    inline constexpr std::string_view ceilKey      = "ceil";
    inline constexpr std::string_view floorKey     = "floor";
    inline constexpr std::string_view roundKey     = "round";
    inline constexpr std::string_view truncKey     = "trunc";

    // -----------------------------
    // Arithmetic Strategy
    // -----------------------------
    inline constexpr std::string_view addKey = "add";
    inline constexpr std::string_view subKey = "sub";
    inline constexpr std::string_view mulKey = "mul";
    inline constexpr std::string_view divKey = "div";
    inline constexpr std::string_view modKey = "mod";

    // -----------------------------
    // Bitwise Strategy
    // -----------------------------
    inline constexpr std::string_view bitAndKey = "bitAnd";
    inline constexpr std::string_view bitOrKey  = "bitOr";
    inline constexpr std::string_view bitXorKey = "bitXor";
    inline constexpr std::string_view bitNotKey = "bitNot";
    inline constexpr std::string_view shlKey    = "shl";
    inline constexpr std::string_view shrKey    = "shr";

    // -----------------------------
    // MinMax Strategy
    // -----------------------------
    inline constexpr std::string_view sminKey = "smin";
    inline constexpr std::string_view smaxKey = "smax";
    inline constexpr std::string_view uminKey = "umin";
    inline constexpr std::string_view umaxKey = "umax";

    // -----------------------------
    // Comparison Strategy
    // -----------------------------
    inline constexpr std::string_view eqKey = "eq";
    inline constexpr std::string_view neKey = "ne";
    inline constexpr std::string_view ltKey = "lt";
    inline constexpr std::string_view leKey = "le";
    inline constexpr std::string_view gtKey = "gt";
    inline constexpr std::string_view geKey = "ge";

    // -----------------------------
    // Math Strategy
    // -----------------------------
    inline constexpr std::string_view sqrtKey   = "sqrt";
    inline constexpr std::string_view powKey    = "pow";
    inline constexpr std::string_view expKey    = "exp";
    inline constexpr std::string_view exp2Key   = "exp2";
    inline constexpr std::string_view exp10Key  = "exp10";
    inline constexpr std::string_view logKey    = "log";
    inline constexpr std::string_view log2Key   = "log2";
    inline constexpr std::string_view log10Key  = "log10";
    inline constexpr std::string_view sinKey    = "sin";
    inline constexpr std::string_view cosKey    = "cos";
    inline constexpr std::string_view tanKey    = "tan";
    inline constexpr std::string_view asinKey   = "asin";
    inline constexpr std::string_view acosKey   = "acos";
    inline constexpr std::string_view atanKey   = "atan";
    inline constexpr std::string_view atan2Key  = "atan2";
    inline constexpr std::string_view sinhKey   = "sinh";
    inline constexpr std::string_view coshKey   = "cosh";
    inline constexpr std::string_view tanhKey   = "tanh";
    inline constexpr std::string_view fabsKey   = "fabs";

    // -----------------------------
    // Memory Strategy
    // -----------------------------
    inline constexpr std::string_view memcpyKey = "memcpy";
    inline constexpr std::string_view memmoveKey= "memmove";
    inline constexpr std::string_view memsetKey = "memset";
    inline constexpr std::string_view memsetPatternKey = "memsetPattern";

    // -----------------------------
    // Bit Manipulation Strategy
    // -----------------------------
    inline constexpr std::string_view popCountKey       = "popCount";
    inline constexpr std::string_view countLeadingZerosKey  = "countLeadingZeros";
    inline constexpr std::string_view countTrailingZerosKey = "countTrailingZeros";
    inline constexpr std::string_view bitReverseKey     = "bitReverse";
    inline constexpr std::string_view rotateLeftKey     = "rotateLeft";
    inline constexpr std::string_view rotateRightKey    = "rotateRight";

    // -----------------------------
    // Overflow Strategy
    // -----------------------------
    inline constexpr std::string_view saddOverflowKey = "saddWithOverflow";
    inline constexpr std::string_view uaddOverflowKey = "uaddWithOverflow";
    inline constexpr std::string_view ssubOverflowKey = "ssubWithOverflow";
    inline constexpr std::string_view usubOverflowKey = "usubWithOverflow";
    inline constexpr std::string_view smulOverflowKey = "smulWithOverflow";
    inline constexpr std::string_view umulOverflowKey = "umulWithOverflow";

    // -----------------------------
    // Saturation Strategy
    // -----------------------------
    inline constexpr std::string_view saddSatKey = "saddSat";
    inline constexpr std::string_view uaddSatKey  = "uaddSat";
    inline constexpr std::string_view ssubSatKey  = "ssubSat";
    inline constexpr std::string_view usubSatKey  = "usubSat";
    inline constexpr std::string_view sshlSatKey  = "sshlSat";
    inline constexpr std::string_view ushLSatKey  = "ushLSat";

    // -----------------------------
    // FixedPoint Strategy
    // -----------------------------
    inline constexpr std::string_view fpMulKey  = "smul";
    inline constexpr std::string_view fpUMulKey = "umul";
    inline constexpr std::string_view fpDivKey  = "sdiv";
    inline constexpr std::string_view fpUDivKey = "udiv";

    // -----------------------------
    // String method keys
    // -----------------------------
    inline constexpr std::string_view lengthKey = "length";
    inline constexpr std::string_view isEmptyKey = "isEmpty";
    inline constexpr std::string_view charAtKey = "charAt";
    inline constexpr std::string_view substringKey = "substring";
    inline constexpr std::string_view toUpperCaseKey = "toUpperCase";
    inline constexpr std::string_view toLowerCaseKey = "toLowerCase";
    inline constexpr std::string_view containsKey = "contains";
    inline constexpr std::string_view startsWithKey = "startsWith";
    inline constexpr std::string_view endsWithKey = "endsWith";
    inline constexpr std::string_view replaceKey = "replace";
    inline constexpr std::string_view splitKey = "split";
    inline constexpr std::string_view trimKey = "trim";
    inline constexpr std::string_view concatKey = "concat";
    inline constexpr std::string_view fromCharKey = "fromChar";
    inline constexpr std::string_view emptyKey = "empty";
    inline constexpr std::string_view formatKey = "format";
    inline constexpr std::string_view joinKey = "join";

    // -----------------------------
    // Char method keys
    // -----------------------------
    inline constexpr std::string_view isDigitKey = "isDigit";
    inline constexpr std::string_view isLetterKey = "isLetter";
    inline constexpr std::string_view isLetterOrDigitKey = "isLetterOrDigit";
    inline constexpr std::string_view isWhitespaceKey = "isWhitespace";

    // -----------------------------
    // Boolean method keys
    // -----------------------------
    inline constexpr std::string_view toStringKey = "toString";
    inline constexpr std::string_view TRUEKey = "TRUE";
    inline constexpr std::string_view FALSEKey = "FALSE";
}

#endif



// // -----------------------------
// // String method keys
// // -----------------------------
// inline constexpr std::string_view lengthKey = "length";
// inline constexpr std::string_view isEmptyKey = "isEmpty";
// inline constexpr std::string_view charAtKey = "charAt";
// inline constexpr std::string_view substringKey = "substring";
// inline constexpr std::string_view toUpperCaseKey = "toUpperCase";
// inline constexpr std::string_view toLowerCaseKey = "toLowerCase";
// inline constexpr std::string_view containsKey = "contains";
// inline constexpr std::string_view startsWithKey = "startsWith";
// inline constexpr std::string_view endsWithKey = "endsWith";
// inline constexpr std::string_view replaceKey = "replace";
// inline constexpr std::string_view splitKey = "split";
// inline constexpr std::string_view trimKey = "trim";
// inline constexpr std::string_view concatKey = "concat";
// inline constexpr std::string_view fromCharKey = "fromChar";
// inline constexpr std::string_view emptyKey = "empty";
// inline constexpr std::string_view formatKey = "format";
// inline constexpr std::string_view joinKey = "join";

// // -----------------------------
// // Char method keys
// // -----------------------------
// inline constexpr std::string_view isDigitKey = "isDigit";
// inline constexpr std::string_view isLetterKey = "isLetter";
// inline constexpr std::string_view isLetterOrDigitKey = "isLetterOrDigit";
// inline constexpr std::string_view isWhitespaceKey = "isWhitespace";

// // -----------------------------
// // Boolean method keys
// // -----------------------------
// inline constexpr std::string_view toStringKey = "toString";
// inline constexpr std::string_view TRUEKey = "TRUE";
// inline constexpr std::string_view FALSEKey = "FALSE";