#ifndef LYNX_RESOLVER_COMPRASION_STRATEGY_HPP
#define LYNX_RESOLVER_COMPRASION_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include "utils/TypeResolverConstant.hpp"

namespace LynxTypes {

    // ============================================================================
    // Base Interface
    // ============================================================================
    struct ComparisonStrategy {
        [[nodiscard]] virtual llvm::Value* eq(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* ne(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* lt(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* le(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* gt(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* ge(const StrategyContext&) const noexcept = 0;

        virtual ~ComparisonStrategy() noexcept = default;
    };


    // ============================================================================
    // Forward declaration for primary template
    // ============================================================================
    template<typename T>
    struct ComparisonStrategyImpl;

    // ============================================================================
    // Integer Specialization
    // ============================================================================
    template<IntStrategyType T>
    struct ComparisonStrategyImpl<T> : ComparisonStrategy {
        [[nodiscard]] llvm::Value* eq(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* ne(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* lt(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* le(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* gt(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* ge(const StrategyContext&) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Floating-Point Specialization
    // ============================================================================
    template<FloatStrategyType T>
    struct ComparisonStrategyImpl<T> : ComparisonStrategy {
        [[nodiscard]] llvm::Value* eq(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* ne(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* lt(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* le(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* gt(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* ge(const StrategyContext&) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Type Aliases (matching your previous class names exactly)
    // ============================================================================
    using ShortComparisonStrategy  = ComparisonStrategyImpl<short>;
    using IntComparisonStrategy    = ComparisonStrategyImpl<int>;
    using LongComparisonStrategy   = ComparisonStrategyImpl<long>;
    using FloatComparisonStrategy  = ComparisonStrategyImpl<float>;
    using DoubleComparisonStrategy = ComparisonStrategyImpl<double>;
}

#endif 
