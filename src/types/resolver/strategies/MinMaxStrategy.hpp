#ifndef LYNX_RESOLVER_MIN_MAX_STRATEGY_HPP
#define LYNX_RESOLVER_MIN_MAX_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include "utils/TypeResolverConstant.hpp"

namespace LynxTypes {

    // ============================================================================
    // Base Interface
    // ============================================================================
    struct MinMaxStrategy {
        [[nodiscard]] virtual llvm::Value* smin(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* smax(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* umin(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* umax(const StrategyContext&) const noexcept = 0;

        virtual ~MinMaxStrategy() noexcept = default;
    };

    // ============================================================================
    // Forward declaration for primary template
    // ============================================================================
    template<typename T>
    struct MinMaxStrategyImpl;

    // ============================================================================
    // Integer Specialization
    // ============================================================================
    template<IntStrategyType T>
    struct MinMaxStrategyImpl<T> : MinMaxStrategy {
        [[nodiscard]] llvm::Value* smin(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* smax(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* umin(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* umax(const StrategyContext&) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Floating-Point Specialization
    // ============================================================================
    template<FloatStrategyType T>
    struct MinMaxStrategyImpl<T> : MinMaxStrategy {
        [[nodiscard]] llvm::Value* smin(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* smax(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* umin(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* umax(const StrategyContext&) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Type Aliases (matching your previous class names exactly)
    // ============================================================================
    using ShortMinMaxStrategy  = MinMaxStrategyImpl<short>;
    using IntMinMaxStrategy    = MinMaxStrategyImpl<int>;
    using LongMinMaxStrategy   = MinMaxStrategyImpl<long>;
    using FloatMinMaxStrategy  = MinMaxStrategyImpl<float>;
    using DoubleMinMaxStrategy = MinMaxStrategyImpl<double>;
}

#endif 
