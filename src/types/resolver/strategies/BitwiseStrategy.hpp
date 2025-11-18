/**
 * @file BitwiseStrategy.hpp
 * @brief Defines the abstraction and template implementations for standard bitwise operations.
 *
 * This header declares the `BitwiseStrategy` interface, providing methods for
 * common bitwise operations such as AND, OR, XOR, NOT, and bit shifts (left/right).
 * The interface is intended to support LLVM IR code generation for numeric types.
 *
 * Key components:
 *  - `BitwiseStrategy`: abstract base class defining the interface for bitwise operations.
 *  - `BitwiseStrategyImpl<T>`: template specialization for integer and floating-point types.
 *  - Type aliases (`ShortBitwiseStrategy`, `IntBitwiseStrategy`, etc.) for convenience.
 *
 * Features:
 *  - Integer specialization currently provides placeholders returning `nullptr`.
 *  - Floating-point specialization also returns `nullptr` for all methods.
 *  - Extensible design allows future implementations for LLVM IR code generation.
 *
 * Benefits:
 *  - Centralizes bitwise operation logic for numeric types.
 *  - Provides a uniform interface for backend compiler generation.
 *  - Promotes consistency and maintainability across the Lynx type system.
 *
 * Notes:
 *  - All method implementations are placeholders and intended for future expansion.
 *
 * @author Ko Thein (Nathan Mratt)
 * @date   November 2, 2024
*/

#ifndef LYNX_RESOLVER_BITWISE_STRATEGY_HPP
#define LYNX_RESOLVER_BITWISE_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include "helpers/InstructionHelper.hpp"

namespace LynxTypes {

    using helper::callOfBitwiseIntrisic;
    using helper::BitwiseOp;

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
        [[nodiscard]] llvm::Value* bitAnd(const StrategyContext& ctx) const noexcept override { return callOfBitwiseIntrisic(ctx, BitwiseOp::And); }
        [[nodiscard]] llvm::Value* bitOr(const StrategyContext& ctx) const noexcept override { return callOfBitwiseIntrisic(ctx, BitwiseOp::Or); }
        [[nodiscard]] llvm::Value* bitXor(const StrategyContext& ctx) const noexcept override { return callOfBitwiseIntrisic(ctx, BitwiseOp::Xor); }
        [[nodiscard]] llvm::Value* shl(const StrategyContext& ctx) const noexcept override { return callOfBitwiseIntrisic(ctx, BitwiseOp::Shl); }
        [[nodiscard]] llvm::Value* shr(const StrategyContext& ctx) const noexcept override { return callOfBitwiseIntrisic(ctx, BitwiseOp::Shr); }
        [[nodiscard]] llvm::Value* bitNot(const StrategyContext& ctx) const noexcept override { return callOfBitwiseIntrisic(ctx, BitwiseOp::Not); }
    };

    // ============================================================================
    // Floating-Point Specialization
    // ============================================================================
    template<FloatStrategyType T>
    struct BitwiseStrategyImpl<T> : BitwiseStrategy {
        [[nodiscard]] llvm::Value* bitAnd(const StrategyContext& ctx) const noexcept override { return callOfBitwiseIntrisic(ctx, BitwiseOp::And); }
        [[nodiscard]] llvm::Value* bitOr(const StrategyContext& ctx) const noexcept override { return callOfBitwiseIntrisic(ctx, BitwiseOp::Or); }
        [[nodiscard]] llvm::Value* bitXor(const StrategyContext& ctx) const noexcept override { return callOfBitwiseIntrisic(ctx, BitwiseOp::Xor); }
        [[nodiscard]] llvm::Value* shl(const StrategyContext& ctx) const noexcept override { return callOfBitwiseIntrisic(ctx, BitwiseOp::Shl); }
        [[nodiscard]] llvm::Value* shr(const StrategyContext& ctx) const noexcept override { return callOfBitwiseIntrisic(ctx, BitwiseOp::Shr); }
        [[nodiscard]] llvm::Value* bitNot(const StrategyContext& ctx) const noexcept override { return callOfBitwiseIntrisic(ctx, BitwiseOp::Not); }
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
