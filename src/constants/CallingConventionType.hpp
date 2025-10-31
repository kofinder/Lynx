#ifndef LYNX_CALLING_CONVENTION_TYPE_HPP
#define LYNX_CALLING_CONVENTION_TYPE_HPP

#include <string>
#include <unordered_map>
#include <algorithm>

namespace LynxConstants {

    /**
     * @enum CallingConventionType
     * @brief Enumeration of all LLVM-supported calling conventions, along with their intent and use cases.
     */
    enum class CallingConventionType {
        /**
         * C Calling Convention - default, matches platform ABI
         * - Supports varargs
         * - Tolerates some type mismatches
         */
        C,

        /**
         * Fast Calling Convention - maximizes performance
         * - Passes arguments in registers
         * - No varargs
         * - Must match exact prototypes
         */
        Fast,

        /**
         * Cold Calling Convention - rare/hot/cold path separation
         * - Optimized for rarely executed code (e.g., error handlers)
         * - Preserves all registers
         * - No inlining
         */
        Cold,

        /**
         * GHC Calling Convention - for Glasgow Haskell Compiler
         * - Heavy register use, disables callee-saves
         * - Tail call optimized
         * - Architecture-specific limits
         */
        GHC,

        /**
         * HiPE Calling Convention - for Erlang's native compiler
         * - Extensive register passing
         * - Tail call optimization
         * - Only supported on x86 (32/64)
         */
        HiPE,

        /**
         * AnyReg Calling Convention - for patchpoint and stack maps
         * - Arguments in arbitrary/dynamic registers
         * - Used with llvm.experimental.patchpoint
         */
        AnyReg,

        /**
         * PreserveMost Calling Convention - hot path calls
         * - Minimizes caller state saving
         * - Callee saves most registers
         * - Not for infrequent calls
         */
        PreserveMost,

        /**
         * PreserveAll Calling Convention - max caller transparency
         * - Callee preserves all registers
         * - Used in runtimes and signal handlers
         */
        PreserveAll,

        /**
         * PreserveNone Calling Convention
         * - No GPRs preserved
         * - GPRs are all caller-saved
         * - Only for AArch64 and x86_64
         */
        PreserveNone,

        /**
         * CXXFastTLS Calling Convention
         * - For accessing C++-style Thread Local Storage
         * - Callee preserves registers on fast paths
         */
        CXXFastTLS,

        /**
         * Tail Calling Convention - guaranteed tail calls
         * - Requires exact prototype match
         * - Enables always tail call optimization
         */
        Tail,

        /**
         * Swift Calling Convention - used by Swift language
         * - Optimized return register usage
         * - Matches AAPCS-VFP on iOS
         */
        Swift,

        /**
         * SwiftTail Calling Convention - Swift with tail calls
         * - Like swiftcc, but enables tail call pop
         */
        SwiftTail,

        /**
         * CFGuard Check Calling Convention
         * - Used by Windows Control Flow Guard
         * - Checks indirect call targets
         * - Target passed via specific register per arch
         */
        CFGuardCheck,

        /**
         * StdCall - Windows x86 system calls
         * - Callee cleans up stack
         */
        StdCall,

        /**
         * FastCall - Windows x86 fast calls
         * - Arguments in ECX, EDX
         */
        FastCall,

        /**
         * ThisCall - Windows x86 instance method calling
         * - `this` pointer in ECX
         */
        ThisCall,

        /**
         * Numbered Calling Convention
         * - Generic entry for target-specific conventions
         * - Custom convention via number (cc <n>)
         */
        Numbered
    };

    /**
     * @brief Post-increment operator for iterating over CallingConventionType enum values.
     * @param ccType Enum reference to increment.
     * @return The next CallingConventionType in sequence.
     */
    inline CallingConventionType operator++(CallingConventionType& ccType, int) {
        int val = static_cast<int>(ccType);
        if (++val >= static_cast<int>(CallingConventionType::Numbered) + 1) {
            throw std::out_of_range("CallingConventionType overflow");
        }
        ccType = static_cast<CallingConventionType>(val);
        return ccType;
    }

    /**
     * @brief Convert string name to CallingConventionType enum.
     * @param name The name to parse (e.g. "Fast", "GHC").
     * @return Corresponding CallingConventionType.
     * @throws std::invalid_argument on unknown name.
     */
    inline CallingConventionType parseStringToConvType(const std::string &name) {
        static const std::unordered_map<std::string, CallingConventionType> map = {
            {"C", CallingConventionType::C},
            {"Fast", CallingConventionType::Fast},
            {"Cold", CallingConventionType::Cold},
            {"GHC", CallingConventionType::GHC},
            {"HiPE", CallingConventionType::HiPE},
            {"AnyReg", CallingConventionType::AnyReg},
            {"PreserveMost", CallingConventionType::PreserveMost},
            {"PreserveAll", CallingConventionType::PreserveAll},
            {"PreserveNone", CallingConventionType::PreserveNone},
            {"CXXFastTLS", CallingConventionType::CXXFastTLS},
            {"Tail", CallingConventionType::Tail},
            {"Swift", CallingConventionType::Swift},
            {"SwiftTail", CallingConventionType::SwiftTail},
            {"CFGuardCheck", CallingConventionType::CFGuardCheck},
            {"StdCall", CallingConventionType::StdCall},
            {"FastCall", CallingConventionType::FastCall},
            {"ThisCall", CallingConventionType::ThisCall},
            {"Numbered", CallingConventionType::Numbered}
        };

        auto it = map.find(name);
        if (it == map.end()) {
            throw std::invalid_argument("Unknown CallingConventionType: " + name);
        }
        return it->second;
    }

    /**
     * @brief Convert CallingConventionType to string representation.
     * @param cc The CallingConventionType to stringify.
     * @return A string name of the calling convention.
     */
    inline std::string parseConvToString(CallingConventionType cc) {
        switch (cc) {
            case CallingConventionType::C: return "C";
            case CallingConventionType::Fast: return "Fast";
            case CallingConventionType::Cold: return "Cold";
            case CallingConventionType::GHC: return "GHC";
            case CallingConventionType::HiPE: return "HiPE";
            case CallingConventionType::AnyReg: return "AnyReg";
            case CallingConventionType::PreserveMost: return "PreserveMost";
            case CallingConventionType::PreserveAll: return "PreserveAll";
            case CallingConventionType::PreserveNone: return "PreserveNone";
            case CallingConventionType::CXXFastTLS: return "CXXFastTLS";
            case CallingConventionType::Tail: return "Tail";
            case CallingConventionType::Swift: return "Swift";
            case CallingConventionType::SwiftTail: return "SwiftTail";
            case CallingConventionType::CFGuardCheck: return "CFGuardCheck";
            case CallingConventionType::StdCall: return "StdCall";
            case CallingConventionType::FastCall: return "FastCall";
            case CallingConventionType::ThisCall: return "ThisCall";
            case CallingConventionType::Numbered: return "Numbered";
            default: return "Unknown";
        }
    }
}

#endif
