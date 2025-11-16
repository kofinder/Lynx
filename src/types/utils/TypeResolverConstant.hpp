/**
 * @file TypeMethodConstants.hpp
 * @brief Predefined method keys and type-specific method maps for Lynx types.
 *
 * This header defines all the constant method keys (as `std::string_view`) 
 * for numeric, string, character, boolean, and byte types in the Lynx language. 
 * It also provides type-specific method maps (`std::unordered_map`) that map 
 * method names to their argument count or function pointers. 
 *
 * These constants and maps are used by TypeMethodResolvers to:
 * - Validate method calls at compile or runtime.
 * - Lookup the corresponding LLVM IR generation functions.
 * - Provide a centralized and consistent source of method definitions.
 *
 * The header uses modern C++23 features:
 * - `inline constexpr` for constant strings
 * - `static inline const` for type method maps
 * - Concepts and function pointer types for method dispatch
 * - `noexcept` guarantees for all method function pointers
 * @author Ko Thein (Nathan Mratt)
 * @date November 15, 2025
*/

#ifndef LYNX_TYPE_METHOD_CONSTANTS_HPP
#define LYNX_TYPE_METHOD_CONSTANTS_HPP

#include <unordered_map>
#include <string>
#include <llvm/IR/Value.h>
#include <type_traits>
#include <vector>
#include "TypeMethodKeys.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    using LynxContext::AstContext;

    // Integer concept (excludes bool)
    template<typename T>
    concept IntStrategyType = std::is_integral_v<T> && !std::is_same_v<T, bool>;

    // Floating point concept
    template<typename T>
    concept FloatStrategyType = std::is_floating_point_v<T>;

    struct StrategyContext {
        const AstContext& ctx;
        llvm::Value* instance;
        const std::vector<llvm::Value*>& args;
    };

    
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


} // namespace LynxTypes

#endif



    
    // // -----------------------------
    // // Numeric method keys
    // // -----------------------------
    // inline constexpr std::string_view  absKey = "abs";
    // inline constexpr std::string_view negateKey = "negate";
    // inline constexpr std::string_view signKey = "sign";
    // inline constexpr std::string_view clampKey = "clamp";
    // inline constexpr std::string_view isEvenKey = "isEven";
    // inline constexpr std::string_view isOddKey = "isOdd";
    // inline constexpr std::string_view fromStringKey = "fromString";
    // inline constexpr std::string_view zeroKey = "zero";
    // inline constexpr std::string_view oneKey = "one";
    // inline constexpr std::string_view maxKey = "max";
    // inline constexpr std::string_view minKey = "min";
    // inline constexpr std::string_view isPositiveKey = "isPositive";
    // inline constexpr std::string_view isNegativeKey = "isNegative";
    // inline constexpr std::string_view bitCountKey = "bitCount";
    // inline constexpr std::string_view leadingZerosKey = "leadingZeros";
    // inline constexpr std::string_view trailingZerosKey = "trailingZeros";
    // inline constexpr std::string_view ceilKey = "ceil";
    // inline constexpr std::string_view floorKey = "floor";
    // inline constexpr std::string_view roundKey = "round";
    // inline constexpr std::string_view truncKey = "trunc";

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