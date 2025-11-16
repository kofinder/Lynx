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
 * - Specializations (e.g., `StrategyReflection<ArithmeticStrategyImpl<T>>`) define
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
#include "strategies/ArithmeticStrategy.hpp"
#include "strategies/BitwiseStrategy.hpp"
#include "strategies/AbsStrategy.hpp"
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
    // Generic helper function to invoke a strategy method by name
    // ============================================================================
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
    // Specialization for arithmetic strategies (int, float, etc.)
    // ============================================================================
    template<typename T>
    requires std::is_arithmetic_v<T>
    struct StrategyReflection<ArithmeticStrategyImpl<T>> : StrategyReflectionBase<ArithmeticStrategyImpl<T>> {
        using Strategy = ArithmeticStrategyImpl<T>;
        using Entry = typename StrategyReflectionBase<Strategy>::Entry;
        static constexpr std::array<Entry, 5> entries{{
            {addKey, &Strategy::add},
            {subKey, &Strategy::sub},
            {mulKey, &Strategy::mul},
            {divKey, &Strategy::div},
            {modKey, &Strategy::mod}
        }};
    };
}

#endif 
