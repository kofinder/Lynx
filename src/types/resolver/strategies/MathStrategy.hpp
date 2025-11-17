/**
 * @file MathStrategy.hpp
 * @brief Declares the MathStrategy interface and template specializations for numeric types.
 *
 * This header provides an abstraction for common mathematical operations such as
 * square root, exponentials, logarithms, trigonometric functions, and rounding functions.
 * It is designed to support LLVM IR code generation for both integer and floating-point types
 * within the Lynx type system.
 *
 * Key components:
 *  - `MathStrategy`: abstract base class defining the interface for math operations.
 *  - `MathStrategyImpl<T>`: template specializations for integer and floating-point types.
 *  - Type aliases (`ShortMathStrategy`, `IntMathStrategy`, etc.) for convenience.
 *
 * Features:
 *  - Placeholder implementations return `nullptr`.
 *  - Supports extension for LLVM IR code generation for backend compiler logic.
 *
 * Benefits:
 *  - Centralizes math operation logic across numeric types.
 *  - Provides a uniform interface for compiler code generation.
 *  - Improves maintainability and consistency in numeric computations.
 *
 * @author Ko Thein (Nathan Mratt)
 * @date   November 2, 2024
*/

#ifndef LYNX_RESOLVER_MATH_STRATEGY_HPP
#define LYNX_RESOLVER_MATH_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include "resolver/TypeStrategyContext.hpp"
#include "helpers/InstructionHelper.hpp"

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
        using RawT = T;
        static constexpr bool isSigned = std::numeric_limits<RawT>::is_signed;
        [[nodiscard]] llvm::Value* sqrt(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::sqrt, isSigned); }
        [[nodiscard]] llvm::Value* pow(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::pow, isSigned);}
        [[nodiscard]] llvm::Value* exp(const StrategyContext& ctx) const noexcept override {  return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::exp, isSigned);}
        [[nodiscard]] llvm::Value* exp2(const StrategyContext& ctx) const noexcept override {  return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::exp2, isSigned);}
        [[nodiscard]] llvm::Value* exp10(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::exp10, isSigned);}
        [[nodiscard]] llvm::Value* log(const StrategyContext& ctx) const noexcept override {  return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::log, isSigned);}
        [[nodiscard]] llvm::Value* log2(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::log2, isSigned);}
        [[nodiscard]] llvm::Value* log10(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::log10, isSigned);}
        [[nodiscard]] llvm::Value* sin(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::sin, isSigned);}
        [[nodiscard]] llvm::Value* cos(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::cos, isSigned);}
        [[nodiscard]] llvm::Value* tan(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::tan, isSigned);}
        [[nodiscard]] llvm::Value* asin(const StrategyContext& ctx) const noexcept override {  return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::asin, isSigned);}
        [[nodiscard]] llvm::Value* acos(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::acos, isSigned);}
        [[nodiscard]] llvm::Value* atan(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::atan, isSigned);}
        [[nodiscard]] llvm::Value* atan2(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::atan2, isSigned);}
        [[nodiscard]] llvm::Value* sinh(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::sinh, isSigned);}
        [[nodiscard]] llvm::Value* cosh(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::cosh, isSigned);}
        [[nodiscard]] llvm::Value* tanh(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::tanh, isSigned);}
        [[nodiscard]] llvm::Value* floor(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::floor, isSigned);}
        [[nodiscard]] llvm::Value* ceil(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::ceil, isSigned);}
        [[nodiscard]] llvm::Value* trunc(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::trunc, isSigned);}
        [[nodiscard]] llvm::Value* round(const StrategyContext& ctx) const noexcept override { return helper::callOfBinaryIntrinsic(ctx, llvm::Intrinsic::round, isSigned);}
        [[nodiscard]] llvm::Value* fabs(const StrategyContext& ctx) const noexcept override { return helper::callOfBinaryIntrinsic(ctx, llvm::Intrinsic::fabs, isSigned);}
    };

    // ============================================================================
    // Floating-Point Specialization
    // ============================================================================
    template<FloatStrategyType T>
    struct MathStrategyImpl<T> : MathStrategy {
        [[nodiscard]] llvm::Value* sqrt(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::sqrt);}
        [[nodiscard]] llvm::Value* pow(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::pow); }
        [[nodiscard]] llvm::Value* exp(const StrategyContext& ctx) const noexcept override {  return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::exp); }
        [[nodiscard]] llvm::Value* exp2(const StrategyContext& ctx) const noexcept override {  return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::exp2); }
        [[nodiscard]] llvm::Value* exp10(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::exp10); }
        [[nodiscard]] llvm::Value* log(const StrategyContext& ctx) const noexcept override {  return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::log); }
        [[nodiscard]] llvm::Value* log2(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::log2); }
        [[nodiscard]] llvm::Value* log10(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::log10); }
        [[nodiscard]] llvm::Value* sin(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::sin); }
        [[nodiscard]] llvm::Value* cos(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::cos); }
        [[nodiscard]] llvm::Value* tan(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::tan); }
        [[nodiscard]] llvm::Value* asin(const StrategyContext& ctx) const noexcept override {  return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::asin); }
        [[nodiscard]] llvm::Value* acos(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::acos); }
        [[nodiscard]] llvm::Value* atan(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::atan); }
        [[nodiscard]] llvm::Value* atan2(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::atan2); }
        [[nodiscard]] llvm::Value* sinh(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::sinh); }
        [[nodiscard]] llvm::Value* cosh(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::cosh); }
        [[nodiscard]] llvm::Value* tanh(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::tanh); }
        [[nodiscard]] llvm::Value* floor(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::floor); }
        [[nodiscard]] llvm::Value* ceil(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::ceil); }
        [[nodiscard]] llvm::Value* trunc(const StrategyContext& ctx) const noexcept override { return helper::callOfUnaryIntrinsic(ctx, llvm::Intrinsic::trunc); }
        [[nodiscard]] llvm::Value* round(const StrategyContext& ctx) const noexcept override { return helper::callOfBinaryIntrinsic(ctx, llvm::Intrinsic::round); }
        [[nodiscard]] llvm::Value* fabs(const StrategyContext& ctx) const noexcept override { return helper::callOfBinaryIntrinsic(ctx, llvm::Intrinsic::fabs); }
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
