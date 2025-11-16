#ifndef LYNX_RESOLVER_FIXED_POINT_STRATEGY_HPP
#define LYNX_RESOLVER_FIXED_POINT_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include "utils/TypeResolverConstant.hpp"

namespace LynxTypes {

    // ============================================================================
    // Base Interface
    // ============================================================================
    struct FixedPointStrategy {
        [[nodiscard]] virtual llvm::Value* mul(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* umul(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* div(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* udiv(const StrategyContext&) const noexcept = 0;

        virtual ~FixedPointStrategy() noexcept = default;
    };


    // ============================================================================
    // Forward declaration for primary template
    // ============================================================================
    template<typename T>
    struct FixedPointStrategyImpl;

    // ============================================================================
    // Integer Specialization
    // ============================================================================
    template<IntStrategyType T>
    struct FixedPointStrategyImpl<T> : FixedPointStrategy {
        [[nodiscard]] llvm::Value* mul(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* umul(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* div(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* udiv(const StrategyContext&) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Floating-Point Specialization
    // ============================================================================
    template<FloatStrategyType T>
    struct FixedPointStrategyImpl<T> : FixedPointStrategy {
        [[nodiscard]] llvm::Value* mul(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* umul(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* div(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* udiv(const StrategyContext&) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Type Aliases (matching your previous class names exactly)
    // ============================================================================
    using ShortFixedPointStrategy  = FixedPointStrategyImpl<short>;
    using IntFixedPointStrategy    = FixedPointStrategyImpl<int>;
    using LongFixedPointStrategy   = FixedPointStrategyImpl<long>;
    using FloatFixedPointStrategy  = FixedPointStrategyImpl<float>;
    using DoubleFixedPointStrategy = FixedPointStrategyImpl<double>;

}

#endif 
