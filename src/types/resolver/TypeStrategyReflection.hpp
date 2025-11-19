/**
 * @file TypeStrategyReflection.hpp
 * @brief Generic strategy reflection system for Lynx numeric and other types.
 *
 * This header defines the `StrategyReflection` templates, which map method
 * names (as strings) to member function pointers of strategy classes, enabling
 * dynamic invocation of operations on different types (int, float, bitwise, etc.).
 *
 * Key components:
 * - `StrategyReflectionBase<Strategy>`: Provides the common `FnType` and `Entry` definitions.
 * - `StrategyReflection<Strategy>`: Primary template; empty by default.
 * - Specializations (e.g., `StrategyReflection<BinaryStrategyImpl<T>>`) define
 *   the mapping between method keys (from `TypeResolverConstant.hpp`) and
 *   strategy functions.
 * - `invokeByName()`: Generic helper to call a strategy method by string key.
 *
 * This system allows extending new strategies without repeating the
 * Entry structure or invoke logic, keeping the design DRY and type-safe.
 *
 * @author Ko Thein (Nathan Mratt)
 * @date November 2, 2024
 */

#ifndef LYNX_TYPE_STRATEGY_REFLECTION_HPP
#define LYNX_TYPE_STRATEGY_REFLECTION_HPP

#include "TypeStrategyContext.hpp"
#include "strategies/BinaryStrategy.hpp"
#include "strategies/BitwiseStrategy.hpp"
#include "strategies/ArithmeticStrategy.hpp"
#include "strategies/MinMaxStrategy.hpp"
#include "strategies/ComparisonStrategy.hpp"
#include "strategies/MathStrategy.hpp"
#include "strategies/MemoryStrategy.hpp"
#include "strategies/BitManipulationStrategy.hpp"
#include "strategies/OverflowStrategy.hpp"
#include "strategies/SaturationStrategy.hpp"
#include "strategies/FixedPointStrategy.hpp"
#include "constants/TypeMethodMapConstants.hpp"

#include <array>
#include <vector>
#include <string_view>
#include <type_traits>


namespace LynxTypes {

    // ============================================================================
    // Base template for all strategy reflections
    // ============================================================================
    // Provides the common types for all strategies:
    // - FnType: pointer to a member function taking a StrategyContext.
    // - Entry: pairs a method name (string_view) with the function pointer.
    template<typename Strategy>
    struct StrategyReflectionBase {
        using FnType = llvm::Value* (Strategy::*)(const StrategyContext&) const noexcept;
        struct Entry { std::string_view name; FnType method; };
    };

    // ============================================================================
    // Primary template for StrategyReflection
    // ============================================================================
    // Empty by default; specialized for concrete strategies.
    template<typename Strategy>
    struct StrategyReflection : StrategyReflectionBase<Strategy> {
        static constexpr std::array<typename StrategyReflectionBase<Strategy>::Entry, 0> entries{};
    };

    // ============================================================================
    // Generic helper to invoke a strategy method by string key
    // ============================================================================
    // Loops through the entries of the specialized StrategyReflection and
    // calls the corresponding member function if a match is found.
    template<typename Strategy>
    llvm::Value* invokeByName(const Strategy& strategy, std::string_view method, const StrategyContext& ctx) noexcept {
        for (auto& entry : StrategyReflection<Strategy>::entries) {
            if (entry.name == method) {
                return (strategy.*entry.method)(ctx);
            }
        }
        return nullptr;
    }

    // ============================================================================
    // ABS strategy specialization
    // ============================================================================
    template<typename T>
    requires std::is_arithmetic_v<T>
    struct StrategyReflection<ArithmeticStrategyImpl<T>> : StrategyReflectionBase<ArithmeticStrategyImpl<T>> {
        using Strategy = ArithmeticStrategyImpl<T>;
        using Entry = typename StrategyReflectionBase<Strategy>::Entry;
        static constexpr std::array<Entry, 6> entries{{
            {absKey, &Strategy::abs},
            {negateKey, &Strategy::negate},
            {signKey, &Strategy::sign},
            {clampKey, &Strategy::clamp},
            {isEvenKey, &Strategy::isEven},
            {isOddKey, &Strategy::isOdd}
        }};
    };

    // ============================================================================
    // Arithmetic strategy specialization
    // ============================================================================
    template<typename T>
    requires std::is_arithmetic_v<T>
    struct StrategyReflection<BinaryStrategyImpl<T>> : StrategyReflectionBase<BinaryStrategyImpl<T>> {
        using Strategy = BinaryStrategyImpl<T>;
        using Entry = typename StrategyReflectionBase<Strategy>::Entry;
        static constexpr std::array<Entry, 5> entries{{
            {addKey, &Strategy::add},
            {subKey, &Strategy::sub},
            {mulKey, &Strategy::mul},
            {divKey, &Strategy::div},
            {modKey, &Strategy::mod}
        }};
    };

    // ============================================================================
    // Bit manipulation strategy specialization
    // ============================================================================
    template<typename T>
    requires std::is_arithmetic_v<T>
    struct StrategyReflection<BitManipulationStrategyImpl<T>> : StrategyReflectionBase<BitManipulationStrategyImpl<T>> {
        using Strategy = BitManipulationStrategyImpl<T>;
        using Entry = typename StrategyReflectionBase<Strategy>::Entry;
        static constexpr std::array<Entry, 6> entries{{
            {popCountKey, &Strategy::popCount},
            {countLeadingZerosKey, &Strategy::countLeadingZeros},
            {countTrailingZerosKey, &Strategy::countTrailingZeros},
            {bitReverseKey, &Strategy::bitReverse},
            {rotateLeftKey, &Strategy::rotateLeft},
            {rotateRightKey, &Strategy::rotateRight}
        }};
    };

    // ============================================================================
    // Bitwise logic strategy specialization
    // ============================================================================
    template<typename T>
    requires std::is_arithmetic_v<T>
    struct StrategyReflection<BitwiseStrategyImpl<T>> : StrategyReflectionBase<BitwiseStrategyImpl<T>> {
        using Strategy = BitwiseStrategyImpl<T>;
        using Entry = typename StrategyReflectionBase<Strategy>::Entry;    
        static constexpr std::array<Entry, 6> entries{{
            {bitAndKey, &Strategy::bitAnd},
            {bitOrKey, &Strategy::bitOr},
            {bitXorKey, &Strategy::bitXor},
            {shlKey, &Strategy::shl},
            {shrKey, &Strategy::shr},
            {bitNotKey, &Strategy::bitNot}
        }};
    };

    // ============================================================================
    // Comparison strategy specialization
    // ============================================================================
    template<typename T>
    requires std::is_arithmetic_v<T>
    struct StrategyReflection<ComparisonStrategyImpl<T>> : StrategyReflectionBase<ComparisonStrategyImpl<T>> {
        using Strategy = ComparisonStrategyImpl<T>;
        using Entry = typename StrategyReflectionBase<Strategy>::Entry;    
        static constexpr std::array<Entry, 6> entries{{
            {eqKey, &Strategy::eq},
            {neKey, &Strategy::ne},
            {ltKey, &Strategy::lt},
            {leKey, &Strategy::le},
            {gtKey, &Strategy::gt},
            {geKey, &Strategy::ge}
        }};
    };

    // ============================================================================
    // Fixed Point strategy specialization
    // ============================================================================
    template<typename T>
    requires std::is_arithmetic_v<T>
    struct StrategyReflection<FixedPointStrategyImpl<T>> : StrategyReflectionBase<FixedPointStrategyImpl<T>> {
        using Strategy = FixedPointStrategyImpl<T>;
        using Entry = typename StrategyReflectionBase<Strategy>::Entry;    
        static constexpr std::array<Entry, 4> entries{{
            {fpMulKey, &Strategy::smul},
            {fpUMulKey, &Strategy::umul},
            {fpDivKey, &Strategy::sdiv},
            {fpUDivKey, &Strategy::udiv}
        }};
    };

    // ============================================================================
    // Math strategy specialization
    // ============================================================================
    template<typename T>
    requires std::is_arithmetic_v<T>
    struct StrategyReflection<MathStrategyImpl<T>> : StrategyReflectionBase<MathStrategyImpl<T>> {
        using Strategy = MathStrategyImpl<T>;
        using Entry = typename StrategyReflectionBase<Strategy>::Entry;    
        static constexpr std::array<Entry, 23> entries{{
            {sqrtKey, &Strategy::sqrt},
            {powKey, &Strategy::pow},
            {expKey, &Strategy::exp},
            {exp2Key, &Strategy::exp2},
            {exp10Key, &Strategy::exp10},
            {logKey, &Strategy::log},
            {log2Key, &Strategy::log2},
            {log10Key, &Strategy::log10},
            {sinKey, &Strategy::sin},
            {cosKey, &Strategy::cos},
            {tanKey, &Strategy::tan},
            {asinKey, &Strategy::asin},
            {acosKey, &Strategy::acos},
            {atanKey, &Strategy::atan},
            {atan2Key, &Strategy::atan2},
            {sinhKey, &Strategy::sinh},
            {coshKey, &Strategy::cosh},
            {tanhKey, &Strategy::tanh},
            {floorKey, &Strategy::floor},
            {ceilKey, &Strategy::ceil},
            {truncKey, &Strategy::trunc},
            {roundKey, &Strategy::round},
            {fabsKey, &Strategy::fabs}
        }};
    };

    // ============================================================================
    // Memory strategy specialization
    // ============================================================================
    template<typename T>
    requires std::is_arithmetic_v<T>
    struct StrategyReflection<MemoryStrategyImpl<T>> : StrategyReflectionBase<MemoryStrategyImpl<T>> {
        using Strategy = MemoryStrategyImpl<T>;
        using Entry = typename StrategyReflectionBase<Strategy>::Entry;
        static constexpr std::array<Entry, 4> entries{{
            {memcpyKey, &Strategy::memcpy},
            {memmoveKey, &Strategy::memmove},
            {memsetKey, &Strategy::memset},
            {memsetPatternKey, &Strategy::memsetPattern}
        }};
    };

    // ============================================================================
    // MinMax strategy specialization
    // ============================================================================
    template<typename T>
    requires std::is_arithmetic_v<T>
    struct StrategyReflection<MinMaxStrategyImpl<T>> : StrategyReflectionBase<MinMaxStrategyImpl<T>> {
        using Strategy = MinMaxStrategyImpl<T>;
        using Entry = typename StrategyReflectionBase<Strategy>::Entry;    
        static constexpr std::array<Entry, 4> entries{{
            {sminKey, &Strategy::smin},
            {smaxKey, &Strategy::smax},
            {uminKey, &Strategy::umin},
            {umaxKey, &Strategy::umax}
        }};
    };

    // ============================================================================
    // Overflow strategy specialization
    // ============================================================================
    template<typename T>
    requires std::is_arithmetic_v<T>
    struct StrategyReflection<OverflowStrategyImpl<T>> : StrategyReflectionBase<OverflowStrategyImpl<T>> {
        using Strategy = OverflowStrategyImpl<T>;
        using Entry = typename StrategyReflectionBase<Strategy>::Entry;
        static constexpr std::array<Entry, 6> entries{{
            {saddOverflowKey, &Strategy::saddWithOverflow},
            {uaddOverflowKey, &Strategy::uaddWithOverflow},
            {ssubOverflowKey, &Strategy::ssubWithOverflow},
            {usubOverflowKey, &Strategy::usubWithOverflow},
            {smulOverflowKey, &Strategy::smulWithOverflow},
            {umulOverflowKey, &Strategy::umulWithOverflow}
        }};
    };

    // ============================================================================
    // Saturation strategy specialization
    // ============================================================================
    template<typename T>
    requires std::is_arithmetic_v<T>
    struct StrategyReflection<SaturationStrategyImpl<T>> : StrategyReflectionBase<SaturationStrategyImpl<T>> {
        using Strategy = SaturationStrategyImpl<T>;
        using Entry = typename StrategyReflectionBase<Strategy>::Entry;
        static constexpr std::array<Entry, 6> entries{{
            {saddSatKey, &Strategy::saddSat},
            {uaddSatKey, &Strategy::uaddSat},
            {ssubSatKey, &Strategy::ssubSat},
            {usubSatKey, &Strategy::usubSat},
            {sshlSatKey, &Strategy::sshlSat},
            {ushLSatKey, &Strategy::ushLSat}
        }};
    };

}

#endif 
