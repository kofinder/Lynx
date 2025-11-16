#ifndef LYNX_RESOLVER_BITWISE_STRATEGY_HPP
#define LYNX_RESOLVER_BITWISE_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include "utils/TypeResolverConstant.hpp"

namespace LynxTypes {

    // ============================================================================
    // Base Interface
    // ============================================================================
    struct BitwiseStrategy {
        [[nodiscard]] virtual llvm::Value* bitAnd(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* bitOr(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* bitXor(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* shl(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* shr(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* bitNot(const StrategyContext&) const noexcept = 0;

        virtual ~BitwiseStrategy() noexcept = default;
    };

    // ============================================================================
    // Forward declaration for primary template
    // ============================================================================
    template<typename T>
    struct BitwiseStrategyImpl;

    // ============================================================================
    // Integer Specialization
    // ============================================================================
    template<IntStrategyType T>
    struct BitwiseStrategyImpl<T> : BitwiseStrategy {
        [[nodiscard]] llvm::Value* bitAnd(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* bitOr(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* bitXor(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* shl(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* shr(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* bitNot(const StrategyContext&) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Floating-Point Specialization
    // ============================================================================
    template<FloatStrategyType T>
    struct BitwiseStrategyImpl<T> : BitwiseStrategy {
        [[nodiscard]] llvm::Value* bitAnd(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* bitOr(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* bitXor(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* shl(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* shr(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* bitNot(const StrategyContext&) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Type Aliases (matching your previous class names exactly)
    // ============================================================================
    using ShortBitwiseStrategy  = BitwiseStrategyImpl<short>;
    using IntBitwiseStrategy    = BitwiseStrategyImpl<int>;
    using LongBitwiseStrategy   = BitwiseStrategyImpl<long>;
    using FloatBitwiseStrategy  = BitwiseStrategyImpl<float>;
    using DoubleBitwiseStrategy = BitwiseStrategyImpl<double>;
}

#endif 
