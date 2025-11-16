/**
 * @file AbsStrategy.hpp
 * @brief Defines the abstraction and implementations for absolute value and sign operations.
 *
 * This header declares the `AbsStrategy` interface, which provides methods for
 * computing absolute value, negation, sign, clamping, and parity checks (`isEven`,
 * `isOdd`) for different numeric types in the Lynx type system.
 *
 * Key components:
 *  - `AbsStrategy`: abstract base class defining the common operations.
 *  - `AbsStrategyImpl<T>`: CRTP-style template for type-specific implementations.
 *    Specializations exist for integer and floating-point types.
 *  - Type aliases (`ShortAbsStrategy`, `IntAbsStrategy`, etc.) for convenience.
 *
 * Benefits:
 *  - Provides a uniform interface for numeric operations across multiple types.
 *  - Allows compile-time specialization via template concepts (`IntStrategyType`, `FloatStrategyType`).
 *  - Supports extension for additional numeric types or strategies.
 *
 * Notes:
 *  - Currently, the implementations return `nullptr` as placeholders.
 *  - Concrete LLVM IR generation should be provided in the respective specializations.
 *
 * @author Ko Thein (Nathan Mratt)
 * @date   November 2, 2024
*/

#ifndef LYNX_RESOLVER_ABS_STRATEGY_HPP
#define LYNX_RESOLVER_ABS_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include "resolver/TypeStrategyContext.hpp"

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
