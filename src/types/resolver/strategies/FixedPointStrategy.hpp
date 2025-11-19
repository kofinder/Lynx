/**
 * @file FixedPointStrategy.hpp
 * @brief Defines the abstraction and template implementations for fixed-point arithmetic operations.
 *
 * This header declares the `FixedPointStrategy` interface, which provides
 * methods for fixed-point multiplication and division, including unsigned variants.
 * It is intended to support LLVM IR code generation for numeric types within the Lynx type system.
 *
 * Key components:
 *  - `FixedPointStrategy`: abstract base class defining the interface for fixed-point operations.
 *  - `FixedPointStrategyImpl<T>`: template specialization for integer and floating-point types.
 *  - Type aliases (`ShortFixedPointStrategy`, `IntFixedPointStrategy`, etc.) for convenience.
 *
 * Features:
 *  - Integer and floating-point specializations currently provide placeholders returning `nullptr`.
 *  - Extensible design allows future implementations for LLVM IR code generation.
 *
 * Benefits:
 *  - Centralizes fixed-point arithmetic logic.
 *  - Provides a uniform interface for backend compiler generation.
 *  - Promotes consistency and maintainability across numeric types.
 *
 * Notes:
 *  - All method implementations are placeholders and intended for future expansion.
 *
 * @author Ko Thein (Nathan Mratt)
 * @date   November 2, 2024
*/

#ifndef LYNX_RESOLVER_FIXED_POINT_STRATEGY_HPP
#define LYNX_RESOLVER_FIXED_POINT_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include "helpers/InstructionHelper.hpp"

namespace LynxTypes {

    using helper::callOfFixedPointIntrinsic;

    // ============================================================================
    // Base Interface
    // ============================================================================
    struct FixedPointStrategy {
        [[nodiscard]] virtual llvm::Value* smul(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* umul(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* sdiv(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* udiv(const StrategyContext&) const noexcept = 0;
        virtual ~FixedPointStrategy() noexcept = default;
    };


    // ============================================================================
    // Forward declaration for primary template
    // ============================================================================
    template<typename T>
    struct FixedPointStrategyImpl;


    // ============================================================================
    // Integer Specialization
    // ============================================================================
    template<IntStrategyType T>
    struct FixedPointStrategyImpl<T> : FixedPointStrategy {
        [[nodiscard]] llvm::Value* smul(const StrategyContext& ctx) const noexcept override { return callOfFixedPointIntrinsic(ctx, llvm::Intrinsic::smul_fix); }
        [[nodiscard]] llvm::Value* umul(const StrategyContext& ctx) const noexcept override { return callOfFixedPointIntrinsic(ctx, llvm::Intrinsic::umul_fix); }
        [[nodiscard]] llvm::Value* sdiv(const StrategyContext& ctx) const noexcept override { return callOfFixedPointIntrinsic(ctx, llvm::Intrinsic::sdiv_fix); }
        [[nodiscard]] llvm::Value* udiv(const StrategyContext& ctx) const noexcept override { return callOfFixedPointIntrinsic(ctx, llvm::Intrinsic::udiv_fix); }
    };

    // ============================================================================
    // Floating-Point Specialization
    // ============================================================================
    template<FloatStrategyType T>
    struct FixedPointStrategyImpl<T> : FixedPointStrategy {
        [[nodiscard]] llvm::Value* smul(const StrategyContext& ctx) const noexcept override { return callOfFixedPointIntrinsic(ctx, llvm::Intrinsic::smul_fix); }
        [[nodiscard]] llvm::Value* umul(const StrategyContext& ctx) const noexcept override { return callOfFixedPointIntrinsic(ctx, llvm::Intrinsic::umul_fix); }
        [[nodiscard]] llvm::Value* sdiv(const StrategyContext& ctx) const noexcept override { return callOfFixedPointIntrinsic(ctx, llvm::Intrinsic::sdiv_fix); }
        [[nodiscard]] llvm::Value* udiv(const StrategyContext& ctx) const noexcept override { return callOfFixedPointIntrinsic(ctx, llvm::Intrinsic::udiv_fix); }
    };

    // ============================================================================
    // Type Aliases (matching your previous class names exactly)
    // ============================================================================
    using ShortFixedPointStrategy  = FixedPointStrategyImpl<short>;
    using IntFixedPointStrategy    = FixedPointStrategyImpl<int>;
    using LongFixedPointStrategy   = FixedPointStrategyImpl<long>;
    using FloatFixedPointStrategy  = FixedPointStrategyImpl<float>;
    using DoubleFixedPointStrategy = FixedPointStrategyImpl<double>;
}

#endif 
