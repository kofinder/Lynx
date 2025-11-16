#ifndef LYNX_RESOLVER_MATH_STRATEGY_HPP
#define LYNX_RESOLVER_MATH_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include "utils/TypeResolverConstant.hpp"

namespace LynxTypes {

    // ============================================================================
    // Base Interface
    // ============================================================================
    struct MathStrategy {
        [[nodiscard]] virtual llvm::Value* sqrt(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* pow(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* exp(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* exp2(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* exp10(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* log(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* log2(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* log10(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* sin(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* cos(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* tan(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* asin(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* acos(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* atan(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* atan2(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* sinh(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* cosh(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* tanh(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* floor(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* ceil(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* trunc(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* round(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* fabs(const StrategyContext&) const noexcept = 0;
    
        virtual ~MathStrategy() noexcept = default;
    };

    // ============================================================================
    // Forward declaration for primary template
    // ============================================================================
    template<typename T>
    struct MathStrategyImpl;

    // ============================================================================
    // Integer Specialization
    // ============================================================================
    template<IntStrategyType T>
    struct MathStrategyImpl<T> : MathStrategy {
        [[nodiscard]] llvm::Value* sqrt(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* pow(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* exp(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* exp2(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* exp10(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* log(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* log2(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* log10(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* sin(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* cos(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* tan(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* asin(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* acos(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* atan(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* atan2(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* sinh(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* cosh(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* tanh(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* floor(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* ceil(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* trunc(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* round(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* fabs(const StrategyContext&) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Floating-Point Specialization
    // ============================================================================
    template<FloatStrategyType T>
    struct MathStrategyImpl<T> : MathStrategy {
        [[nodiscard]] llvm::Value* sqrt(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* pow(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* exp(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* exp2(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* exp10(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* log(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* log2(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* log10(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* sin(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* cos(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* tan(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* asin(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* acos(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* atan(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* atan2(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* sinh(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* cosh(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* tanh(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* floor(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* ceil(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* trunc(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* round(const StrategyContext&) const noexcept override { return nullptr; }
        [[nodiscard]] llvm::Value* fabs(const StrategyContext&) const noexcept override { return nullptr; }
    };

    // ============================================================================
    // Type Aliases (matching your previous class names exactly)
    // ============================================================================
    using ShortMathStrategy  = MathStrategyImpl<short>;
    using IntMathStrategy    = MathStrategyImpl<int>;
    using LongMathStrategy   = MathStrategyImpl<long>;
    using FloatMathStrategy  = MathStrategyImpl<float>;
    using DoubleMathStrategy = MathStrategyImpl<double>;
}

#endif 
