#ifndef LYNX_RESOLVER_OVERFLOW_STRATEGY_HPP
#define LYNX_RESOLVER_OVERFLOW_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include "utils/TypeResolverConstant.hpp"

namespace LynxTypes {

    // ============================================================================
    // Base Interface
    // ============================================================================
    struct OverflowStrategy {
        [[nodiscard]] virtual llvm::Value* saddWithOverflow(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* uaddWithOverflow(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* ssubWithOverflow(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* usubWithOverflow(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* smulWithOverflow(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* umulWithOverflow(const StrategyContext&) const noexcept = 0;

        virtual ~OverflowStrategy() noexcept = default;
    };

    // ============================================================================
    // Forward declaration for primary template
    // ============================================================================
    template<typename T>
    struct OverflowStrategyImpl;

    // ============================================================================
    // Integer Specialization
    // ============================================================================
    template<IntStrategyType T>
    struct OverflowStrategyImpl<T> : OverflowStrategy {
        [[nodiscard]] llvm::Value* saddWithOverflow(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* uaddWithOverflow(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* ssubWithOverflow(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* usubWithOverflow(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* smulWithOverflow(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* umulWithOverflow(const StrategyContext&) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Floating-Point Specialization
    // ============================================================================
    template<FloatStrategyType T>
    struct OverflowStrategyImpl<T> : OverflowStrategy {
        [[nodiscard]] llvm::Value* saddWithOverflow(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* uaddWithOverflow(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* ssubWithOverflow(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* usubWithOverflow(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* smulWithOverflow(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* umulWithOverflow(const StrategyContext&) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Type Aliases (matching your previous class names exactly)
    // ============================================================================
    using ShortOverflowStrategy  = OverflowStrategyImpl<short>;
    using IntOverflowStrategy    = OverflowStrategyImpl<int>;
    using LongOverflowStrategy   = OverflowStrategyImpl<long>;
    using FloatOverflowStrategy  = OverflowStrategyImpl<float>;
    using DoubleOverflowStrategy = OverflowStrategyImpl<double>;

}

#endif 
