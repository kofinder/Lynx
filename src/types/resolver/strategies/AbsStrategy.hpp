#ifndef LYNX_RESOLVER_ABS_STRATEGY_HPP
#define LYNX_RESOLVER_ABS_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include "utils/TypeResolverConstant.hpp"

namespace LynxTypes {
    
    // ============================================================================
    // Base Interface
    // ============================================================================
    struct AbsStrategy {
        [[nodiscard]] virtual llvm::Value* abs(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* negate(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* sign(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* clamp(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* isEven(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* isOdd(const StrategyContext&) const noexcept = 0;
        virtual ~AbsStrategy() noexcept = default;
    };


    // ============================================================================
    // Forward declaration for primary template
    // ============================================================================
    template<typename T>
    struct AbsStrategyImpl;

    // ============================================================================
    // Integer Specialization
    // ============================================================================
    template<IntStrategyType T>
    struct AbsStrategyImpl<T> : AbsStrategy {
        [[nodiscard]] llvm::Value* abs(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* negate(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* sign(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* clamp(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* isEven(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* isOdd(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Floating-Point Specialization
    // ============================================================================
    template<FloatStrategyType T>
    struct AbsStrategyImpl<T> : AbsStrategy {
        [[nodiscard]] llvm::Value* abs(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* negate(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* sign(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* clamp(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* isEven(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* isOdd(const StrategyContext& stgCtx) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Type Aliases (matching your previous class names exactly)
    // ============================================================================
    using ShortAbsStrategy  = AbsStrategyImpl<short>;
    using IntAbsStrategy    = AbsStrategyImpl<int>;
    using LongAbsStrategy   = AbsStrategyImpl<long>;
    using FloatAbsStrategy  = AbsStrategyImpl<float>;
    using DoubleAbsStrategy = AbsStrategyImpl<double>;
}

#endif 
