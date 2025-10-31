#ifndef LYNX_FUNCTION_ATTRIBUTES_TYPE_HPP
#define LYNX_FUNCTION_ATTRIBUTES_TYPE_HPP

#include <string>
#include <algorithm>
#include <stdexcept>

namespace LynxConstants {

    enum class FunctionAttributeType {
        ALWAYS_INLINE         = 0,  // Function is always inlined
        NO_INLINE             = 1,  // Function is never inlined
        NO_RETURN             = 2,  // Function does not return to the caller
        READ_ONLY             = 3,  // Function does not write to memory
        READ_NONE             = 4,  // Function does not read or write memory
        NO_CAPTURE            = 5,  // Pointer arguments are not captured
        NO_UNWIND             = 6,  // Function does not unwind the stack (no exceptions)
        SANITIZE_ADDRESS      = 7,  // Enable Address Sanitizer for this function
        OPTIMIZE_FOR_SIZE     = 8,  // Optimize function for code size
        INLINE_HINT           = 9,  // Suggest the compiler to inline the function
        STACK_PROTECTOR       = 10, // Enable stack protection for the function
        NO_INLINE_HINT        = 11, // Suggest the compiler not to inline the function
        NO_DEAD_STRIP         = 12, // Prevent linker from removing this function if unused
        INTRINSIC             = 13, // Function is an intrinsic and has special codegen
        NO_RECURSE            = 14, // Function does not recurse
        SANITIZE_THREAD       = 15, // Enable Thread Sanitizer for this function
        SANITIZE_MEMORY       = 16, // Enable Memory Sanitizer for this function
        SANITIZE_HWADDRESS    = 17, // Enable HWAddress Sanitizer for this function
        NO_INLINE_ASM         = 18, // Disallow inline assembly in the function
        UW_TABLE              = 19, // Emit unwind table for stack unwinding
        ZERO_EXTEND           = 20, // Return value is zero-extended
        SIGN_EXTEND           = 21, // Return value is sign-extended
        ARGMEMONLY            = 22, // Function arguments point to memory only
        RETURNS_TWICE         = 23, // Function may return more than once (like setjmp)
        STRICT_FP             = 24, // Strict floating point model for function
        SANITIZE_COVERAGE     = 25, // Enable coverage sanitizer
        ASSUME_SAME_ALIGNED   = 26, // Arguments are assumed to be aligned the same
        NO_SANITIZE_MEMORY    = 27, // Disable memory sanitizer for this function
        NO_SANITIZE_THREAD    = 28, // Disable thread sanitizer for this function
        NO_SANITIZE_ADDRESS   = 29, // Disable address sanitizer for this function
    };

    inline FunctionAttributeType operator++(FunctionAttributeType& attr, int) {
        FunctionAttributeType old = attr;
        int val = static_cast<int>(attr);
        if (val < static_cast<int>(FunctionAttributeType::NO_SANITIZE_ADDRESS)) {
            attr = static_cast<FunctionAttributeType>(val + 1);
        } else {
            attr = static_cast<FunctionAttributeType>(0);
        }
        return old;
    }

    inline FunctionAttributeType ParseAttribute(const std::string& attr) {
        std::string attr_lower = attr;
        std::transform(attr_lower.begin(), attr_lower.end(), attr_lower.begin(), ::tolower);

        if (attr_lower == "always_inline" || attr_lower == "alwaysinline")
            return FunctionAttributeType::ALWAYS_INLINE;
        else if (attr_lower == "no_inline" || attr_lower == "noinline")
            return FunctionAttributeType::NO_INLINE;
        else if (attr_lower == "no_return" || attr_lower == "noreturn")
            return FunctionAttributeType::NO_RETURN;
        else if (attr_lower == "read_only" || attr_lower == "readonly")
            return FunctionAttributeType::READ_ONLY;
        else if (attr_lower == "read_none" || attr_lower == "readnone")
            return FunctionAttributeType::READ_NONE;
        else if (attr_lower == "no_capture" || attr_lower == "nocapture")
            return FunctionAttributeType::NO_CAPTURE;
        else if (attr_lower == "no_unwind" || attr_lower == "nounwind")
            return FunctionAttributeType::NO_UNWIND;
        else if (attr_lower == "sanitize_address")
            return FunctionAttributeType::SANITIZE_ADDRESS;
        else if (attr_lower == "optimize_for_size")
            return FunctionAttributeType::OPTIMIZE_FOR_SIZE;
        else if (attr_lower == "inline_hint")
            return FunctionAttributeType::INLINE_HINT;
        else if (attr_lower == "stack_protector")
            return FunctionAttributeType::STACK_PROTECTOR;
        else if (attr_lower == "no_inline_hint")
            return FunctionAttributeType::NO_INLINE_HINT;
        else if (attr_lower == "no_dead_strip")
            return FunctionAttributeType::NO_DEAD_STRIP;
        else if (attr_lower == "intrinsic")
            return FunctionAttributeType::INTRINSIC;
        else if (attr_lower == "no_recurse")
            return FunctionAttributeType::NO_RECURSE;
        else if (attr_lower == "sanitize_thread")
            return FunctionAttributeType::SANITIZE_THREAD;
        else if (attr_lower == "sanitize_memory")
            return FunctionAttributeType::SANITIZE_MEMORY;
        else if (attr_lower == "sanitize_hwaddress")
            return FunctionAttributeType::SANITIZE_HWADDRESS;
        else if (attr_lower == "no_inline_asm")
            return FunctionAttributeType::NO_INLINE_ASM;
        else if (attr_lower == "uw_table")
            return FunctionAttributeType::UW_TABLE;
        else if (attr_lower == "zero_extend")
            return FunctionAttributeType::ZERO_EXTEND;
        else if (attr_lower == "sign_extend")
            return FunctionAttributeType::SIGN_EXTEND;
        else if (attr_lower == "argmemonly")
            return FunctionAttributeType::ARGMEMONLY;
        else if (attr_lower == "returns_twice")
            return FunctionAttributeType::RETURNS_TWICE;
        else if (attr_lower == "strict_fp")
            return FunctionAttributeType::STRICT_FP;
        else if (attr_lower == "sanitize_coverage")
            return FunctionAttributeType::SANITIZE_COVERAGE;
        else if (attr_lower == "assume_same_aligned")
            return FunctionAttributeType::ASSUME_SAME_ALIGNED;
        else if (attr_lower == "no_sanitize_memory")
            return FunctionAttributeType::NO_SANITIZE_MEMORY;
        else if (attr_lower == "no_sanitize_thread")
            return FunctionAttributeType::NO_SANITIZE_THREAD;
        else if (attr_lower == "no_sanitize_address")
            return FunctionAttributeType::NO_SANITIZE_ADDRESS;

        throw std::invalid_argument("Unknown function attribute string: " + attr);
    }

    inline std::string FunctionAttributeToString(FunctionAttributeType attr) {
        switch (attr) {
            case FunctionAttributeType::ALWAYS_INLINE:      return "alwaysinline";
            case FunctionAttributeType::NO_INLINE:          return "noinline";
            case FunctionAttributeType::NO_RETURN:          return "noreturn";
            case FunctionAttributeType::READ_ONLY:          return "readonly";
            case FunctionAttributeType::READ_NONE:          return "readnone";
            case FunctionAttributeType::NO_CAPTURE:         return "nocapture";
            case FunctionAttributeType::NO_UNWIND:          return "nounwind";
            case FunctionAttributeType::SANITIZE_ADDRESS:   return "sanitize_address";
            case FunctionAttributeType::OPTIMIZE_FOR_SIZE:  return "optimize_for_size";
            case FunctionAttributeType::INLINE_HINT:        return "inline_hint";
            case FunctionAttributeType::STACK_PROTECTOR:    return "stack_protector";
            case FunctionAttributeType::NO_INLINE_HINT:     return "noinline_hint";
            case FunctionAttributeType::NO_DEAD_STRIP:      return "no_dead_strip";
            case FunctionAttributeType::INTRINSIC:          return "intrinsic";
            case FunctionAttributeType::NO_RECURSE:         return "norecurse";
            case FunctionAttributeType::SANITIZE_THREAD:    return "sanitize_thread";
            case FunctionAttributeType::SANITIZE_MEMORY:    return "sanitize_memory";
            case FunctionAttributeType::SANITIZE_HWADDRESS: return "sanitize_hwaddress";
            case FunctionAttributeType::NO_INLINE_ASM:      return "noinline_asm";
            case FunctionAttributeType::UW_TABLE:           return "uwtable";
            case FunctionAttributeType::ZERO_EXTEND:        return "zeroext";
            case FunctionAttributeType::SIGN_EXTEND:        return "signext";
            case FunctionAttributeType::ARGMEMONLY:         return "argmemonly";
            case FunctionAttributeType::RETURNS_TWICE:      return "returns_twice";
            case FunctionAttributeType::STRICT_FP:          return "strictfp";
            case FunctionAttributeType::SANITIZE_COVERAGE:  return "sanitize_coverage";
            case FunctionAttributeType::ASSUME_SAME_ALIGNED:return "assume_sane_aligned";
            case FunctionAttributeType::NO_SANITIZE_MEMORY: return "no_sanitize_memory";
            case FunctionAttributeType::NO_SANITIZE_THREAD: return "no_sanitize_thread";
            case FunctionAttributeType::NO_SANITIZE_ADDRESS:return "no_sanitize_address";
            default: return "unknown";
        }
    }

}

#endif
