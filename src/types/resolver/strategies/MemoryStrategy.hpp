#ifndef LYNX_RESOLVER_MEMORY_STRATEGY_HPP
#define LYNX_RESOLVER_MEMORY_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include "utils/TypeResolverConstant.hpp"

namespace LynxTypes {

    // ============================================================================
    // Base Interface
    // ============================================================================
    struct MemoryStrategy {
        [[nodiscard]] virtual llvm::Value* memcpy(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* memmove(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* memset(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* memsetPattern(const StrategyContext&) const noexcept = 0;

        virtual ~MemoryStrategy() noexcept = default;
    };


    // ============================================================================
    // Forward declaration for primary template
    // ============================================================================
    template<typename T>
    struct MemoryStrategyImpl;

    // ============================================================================
    // Integer Specialization
    // ============================================================================
    template<IntStrategyType T>
    struct MemoryStrategyImpl<T> : MemoryStrategy {
        [[nodiscard]] llvm::Value* memcpy(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* memmove(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* memset(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* memsetPattern(const StrategyContext&) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Floating-Point Specialization
    // ============================================================================
    template<FloatStrategyType T>
    struct MemoryStrategyImpl<T> : MemoryStrategy {
        [[nodiscard]] llvm::Value* memcpy(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* memmove(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* memset(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* memsetPattern(const StrategyContext&) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Type Aliases (matching your previous class names exactly)
    // ============================================================================
    using ShortMemoryStrategy  = MemoryStrategyImpl<short>;
    using IntMemoryStrategy    = MemoryStrategyImpl<int>;
    using LongMemoryStrategy   = MemoryStrategyImpl<long>;
    using FloatMemoryStrategy  = MemoryStrategyImpl<float>;
    using DoubleMemoryStrategy = MemoryStrategyImpl<double>;
}

#endif 
