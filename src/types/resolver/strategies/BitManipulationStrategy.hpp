/**
 * @file BitManipulationStrategy.hpp
 * @brief Defines the abstraction and template implementations for bit-level operations.
 *
 * This header declares the `BitManipulationStrategy` interface, providing methods
 * for common bit manipulation operations such as pop count, leading/trailing zeros,
 * bit reversal, and bit rotations. These methods are intended to generate corresponding
 * LLVM IR instructions for numeric types.
 *
 * Key components:
 *  - `BitManipulationStrategy`: abstract base class defining bitwise operations.
 *  - `BitManipulationStrategyImpl<T>`: template specialization for integer and 
 *    floating-point types, enabling type-specific LLVM IR generation.
 *  - Type aliases (`ShortBitManipulationStrategy`, `IntBitManipulationStrategy`, etc.) 
 *    for convenience and clarity.
 *
 * Features:
 *  - Integer specialization provides placeholders for LLVM IR generation.
 *  - Floating-point specialization currently returns `nullptr` for all methods.
 *  - Extensible design for supporting additional bitwise operations in the future.
 *
 * Benefits:
 *  - Centralizes bit manipulation logic for numeric types.
 *  - Provides a uniform interface for compiler backend code generation.
 *  - Ensures consistent behavior across all numeric types in the Lynx type system.
 *
 * Notes:
 *  - All method implementations currently return `nullptr` and are intended
 *    for future expansion.
 *
 * @author Ko Thein (Nathan Mratt)
 * @date   November 2, 2024
*/

#ifndef LYNX_RESOLVER_BIT_MINIPULATION_STRATEGY_HPP
#define LYNX_RESOLVER_BIT_MINIPULATION_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Intrinsics.h>
#include "helpers/InstructionHelper.hpp"

namespace LynxTypes {

    using helper::callOfBitManiIntrinsic;

    // ============================================================================
    // Base Interface
    // ============================================================================
    struct BitManipulationStrategy {
        [[nodiscard]] virtual llvm::Value* popCount(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* countLeadingZeros(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* countTrailingZeros(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* bitReverse(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* rotateLeft(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* rotateRight(const StrategyContext&) const noexcept = 0;
        virtual ~BitManipulationStrategy() noexcept = default;
    };

    // ============================================================================
    // Forward declaration for primary template
    // ============================================================================
    template<typename T>
    struct BitManipulationStrategyImpl;

    // ============================================================================
    // Integer Specialization
    // ============================================================================
    template<IntStrategyType T>
    struct BitManipulationStrategyImpl<T> : BitManipulationStrategy {
        [[nodiscard]] llvm::Value* popCount(const StrategyContext& ctx) const noexcept override { return callOfBitManiIntrinsic(ctx, llvm::Intrinsic::ctpop); }
        [[nodiscard]] llvm::Value* countLeadingZeros(const StrategyContext& ctx) const noexcept override { return callOfBitManiIntrinsic(ctx, llvm::Intrinsic::ctlz); }
        [[nodiscard]] llvm::Value* countTrailingZeros(const StrategyContext& ctx) const noexcept override { return callOfBitManiIntrinsic(ctx, llvm::Intrinsic::cttz); }
        [[nodiscard]] llvm::Value* bitReverse(const StrategyContext& ctx) const noexcept override { return callOfBitManiIntrinsic(ctx, llvm::Intrinsic::bitreverse); }
        [[nodiscard]] llvm::Value* rotateLeft(const StrategyContext& ctx) const noexcept override { return callOfBitManiIntrinsic(ctx, llvm::Intrinsic::fshl); }
        [[nodiscard]] llvm::Value* rotateRight(const StrategyContext& ctx) const noexcept override { return callOfBitManiIntrinsic(ctx, llvm::Intrinsic::fshr); }
    };

    // ============================================================================
    // Floating-Point Specialization
    // ============================================================================
    template<FloatStrategyType T>
    struct BitManipulationStrategyImpl<T> : BitManipulationStrategy {
        [[nodiscard]] llvm::Value* popCount(const StrategyContext& ctx) const noexcept override { llvm::report_fatal_error("Unsupported bit intrinsic"); }
        [[nodiscard]] llvm::Value* countLeadingZeros(const StrategyContext& ctx) const noexcept override { llvm::report_fatal_error("Unsupported bit intrinsic"); }
        [[nodiscard]] llvm::Value* countTrailingZeros(const StrategyContext& ctx) const noexcept override { llvm::report_fatal_error("Unsupported bit intrinsic"); }
        [[nodiscard]] llvm::Value* bitReverse(const StrategyContext& ctx) const noexcept override { llvm::report_fatal_error("Unsupported bit intrinsic"); }
        [[nodiscard]] llvm::Value* rotateLeft(const StrategyContext& ctx) const noexcept override { llvm::report_fatal_error("Unsupported bit intrinsic"); }
        [[nodiscard]] llvm::Value* rotateRight(const StrategyContext& ctx) const noexcept override { llvm::report_fatal_error("Unsupported bit intrinsic");}
    };

    // ============================================================================
    // Type Aliases (matching your previous class names exactly)
    // ============================================================================
    using ShortBitManipulationStrategy  = BitManipulationStrategyImpl<short>;
    using IntBitManipulationStrategy    = BitManipulationStrategyImpl<int>;
    using LongBitManipulationStrategy   = BitManipulationStrategyImpl<long>;
    using FloatBitManipulationStrategy  = BitManipulationStrategyImpl<float>;
    using DoubleBitManipulationStrategy = BitManipulationStrategyImpl<double>;
}

#endif 
