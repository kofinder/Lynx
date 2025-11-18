/**
 * @file BinaryStrategy.hpp
 * @brief Defines the abstraction and implementations for arithmetic operations.
 *
 * This header declares the `BinaryStrategy` interface, providing methods
 * for basic arithmetic operations: addition, subtraction, multiplication,
 * division, and modulo. These operations are intended to generate LLVM IR
 * instructions corresponding to the operation on a given type.
 *
 * Key components:
 *  - `BinaryStrategy`: abstract base class defining arithmetic operations.
 *  - `BinaryStrategyImpl<T>`: template specialization for integer and
 *    floating-point types, enabling type-specific LLVM IR generation.
 *  - Type aliases (`ShortBinaryStrategy`, `IntBinaryStrategy`, etc.) 
 *    for convenience and clarity.
 *
 * Features:
 *  - Integer specialization demonstrates LLVM IR code generation for addition.
 *  - Floating-point specialization currently uses placeholders (`nullptr`).
 *  - Extensible design for implementing other arithmetic operations safely.
 *
 * Benefits:
 *  - Centralizes arithmetic logic for numeric types.
 *  - Provides a type-safe, uniform interface across different numeric types.
 *  - Facilitates LLVM IR code generation for compiler backends.
 *
 * Notes:
 *  - Only the integer `add` operation has a concrete implementation in this template.
 *  - Other operations and floating-point specializations are intended for future expansion.
 *
 * @author Ko Thein (Nathan Mratt)
 * @date   November 2, 2024
*/

#ifndef LYNX_RESOLVER_BINARY_STRATEGY_HPP
#define LYNX_RESOLVER_BINARY_STRATEGY_HPP

#include <type_traits>
#include <vector>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include "helpers/InstructionHelper.hpp"

namespace LynxTypes {

    using helper::ArithmeticOp;
    using helper::callOfArithmeticIntrisic;

    // ============================================================================
    // Base Interface
    // ============================================================================
    struct BinaryStrategy {
        [[nodiscard]] virtual llvm::Value* add(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* sub(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* mul(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* div(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* mod(const StrategyContext&) const noexcept = 0;
        virtual ~BinaryStrategy() noexcept = default;
    };

    // ============================================================================
    // Forward declaration for primary template
    // ============================================================================
    template<typename T>
    struct BinaryStrategyImpl;

    // ============================================================================
    // Integer Specialization
    // ============================================================================
    template<IntStrategyType T>
    struct BinaryStrategyImpl<T> : BinaryStrategy {
        [[nodiscard]] llvm::Value* add(const StrategyContext& ctx) const noexcept override { return callOfArithmeticIntrisic(ctx, ArithmeticOp::Add);}
        [[nodiscard]] llvm::Value* sub(const StrategyContext& ctx) const noexcept override { return callOfArithmeticIntrisic(ctx, ArithmeticOp::Sub); }
        [[nodiscard]] llvm::Value* mul(const StrategyContext& ctx) const noexcept override { return callOfArithmeticIntrisic(ctx, ArithmeticOp::Mul); }
        [[nodiscard]] llvm::Value* div(const StrategyContext& ctx) const noexcept override { return callOfArithmeticIntrisic(ctx, ArithmeticOp::Div); }
        [[nodiscard]] llvm::Value* mod(const StrategyContext& ctx) const noexcept override { return callOfArithmeticIntrisic(ctx, ArithmeticOp::Mod); }
    };

    // ============================================================================
    // Floating-Point Specialization
    // ============================================================================
    template<FloatStrategyType T>
    struct BinaryStrategyImpl<T> : BinaryStrategy {
        [[nodiscard]] llvm::Value* add(const StrategyContext& ctx) const noexcept override { return callOfArithmeticIntrisic(ctx, ArithmeticOp::Add); }
        [[nodiscard]] llvm::Value* sub(const StrategyContext& ctx) const noexcept override { return callOfArithmeticIntrisic(ctx, ArithmeticOp::Sub); }
        [[nodiscard]] llvm::Value* mul(const StrategyContext& ctx) const noexcept override { return callOfArithmeticIntrisic(ctx, ArithmeticOp::Mul); }
        [[nodiscard]] llvm::Value* div(const StrategyContext& ctx) const noexcept override { return callOfArithmeticIntrisic(ctx, ArithmeticOp::Div); }
        [[nodiscard]] llvm::Value* mod(const StrategyContext& ctx) const noexcept override { return callOfArithmeticIntrisic(ctx, ArithmeticOp::Mod); }
    };

    // ============================================================================
    // Type Aliases (matching your previous class names exactly)
    // ============================================================================
    using ShortBinaryStrategy  = BinaryStrategyImpl<short>;
    using IntBinaryStrategy    = BinaryStrategyImpl<int>;
    using LongBinaryStrategy   = BinaryStrategyImpl<long>;
    using FloatBinaryStrategy  = BinaryStrategyImpl<float>;
    using DoubleBinaryStrategy = BinaryStrategyImpl<double>;
}

#endif
