/**
 * @file OverflowStrategy.hpp
 * @brief Defines the OverflowStrategy interface and template specializations for numeric types.
 *
 * This header provides an abstraction for performing arithmetic operations
 * with overflow detection. It allows LLVM IR code generation for signed and
 * unsigned addition, subtraction, and multiplication that safely tracks overflow.
 *
 * Key components:
 *  - `OverflowStrategy`: abstract base class defining signed/unsigned arithmetic with overflow methods.
 *  - `OverflowStrategyImpl<T>`: template specializations for integer and floating-point types.
 *  - Type aliases (`ShortOverflowStrategy`, `IntOverflowStrategy`, etc.) for convenience.
 *
 * Features:
 *  - Default implementations return `nullptr` as placeholders.
 *  - Designed to integrate with backend LLVM IR generation for compiler operations.
 *
 * Benefits:
 *  - Centralizes overflow-aware arithmetic logic for numeric types.
 *  - Ensures a uniform interface for compiler code generation.
 *  - Improves safety and maintainability in numeric computations.
 *
 * @author Ko Thein (Nathan Mratt)
 * @date   November 2, 2024
*/

#ifndef LYNX_RESOLVER_OVERFLOW_STRATEGY_HPP
#define LYNX_RESOLVER_OVERFLOW_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include "resolver/TypeOfInstructionHelper.hpp"

namespace LynxTypes {

    using helper::callOfOverflowIntrinsic;


    // ============================================================================
    // Base Interface
    // ============================================================================
    struct OverflowStrategy {
        [[nodiscard]] virtual llvm::Value* saddWithOverflow(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* uaddWithOverflow(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* ssubWithOverflow(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* usubWithOverflow(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* smulWithOverflow(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* umulWithOverflow(const StrategyContext&) const noexcept = 0;
        virtual ~OverflowStrategy() noexcept = default;
    };

    // ============================================================================
    // Forward declaration for primary template
    // ============================================================================
    template<typename T>
    struct OverflowStrategyImpl;

    // ============================================================================
    // Integer Specialization
    // ============================================================================
    template<IntStrategyType T>
    struct OverflowStrategyImpl<T> : OverflowStrategy {
        [[nodiscard]] llvm::Value* saddWithOverflow(const StrategyContext& ctx) const noexcept override { return callOfOverflowIntrinsic(ctx, llvm::Intrinsic::sadd_with_overflow); }
        [[nodiscard]] llvm::Value* uaddWithOverflow(const StrategyContext& ctx) const noexcept override { return callOfOverflowIntrinsic(ctx, llvm::Intrinsic::uadd_with_overflow); }
        [[nodiscard]] llvm::Value* ssubWithOverflow(const StrategyContext& ctx) const noexcept override { return callOfOverflowIntrinsic(ctx, llvm::Intrinsic::ssub_with_overflow); }
        [[nodiscard]] llvm::Value* usubWithOverflow(const StrategyContext& ctx) const noexcept override { return callOfOverflowIntrinsic(ctx, llvm::Intrinsic::usub_with_overflow); }
        [[nodiscard]] llvm::Value* smulWithOverflow(const StrategyContext& ctx) const noexcept override { return callOfOverflowIntrinsic(ctx, llvm::Intrinsic::smul_with_overflow); }
        [[nodiscard]] llvm::Value* umulWithOverflow(const StrategyContext& ctx) const noexcept override { return callOfOverflowIntrinsic(ctx, llvm::Intrinsic::umul_with_overflow); }
    };

    // ============================================================================
    // Floating-Point Specialization
    // ============================================================================
    template<FloatStrategyType T>
    struct OverflowStrategyImpl<T> : OverflowStrategy {
        [[nodiscard]] llvm::Value* saddWithOverflow(const StrategyContext& ctx) const noexcept override { llvm::report_fatal_error("Unsupported overflow intrinsic");}
        [[nodiscard]] llvm::Value* uaddWithOverflow(const StrategyContext& ctx) const noexcept override { llvm::report_fatal_error("Unsupported overflow intrinsic"); }
        [[nodiscard]] llvm::Value* ssubWithOverflow(const StrategyContext& ctx) const noexcept override { llvm::report_fatal_error("Unsupported overflow intrinsic"); }
        [[nodiscard]] llvm::Value* usubWithOverflow(const StrategyContext& ctx) const noexcept override { llvm::report_fatal_error("Unsupported overflow intrinsic"); }
        [[nodiscard]] llvm::Value* smulWithOverflow(const StrategyContext& ctx) const noexcept override { llvm::report_fatal_error("Unsupported overflow intrinsic"); }
        [[nodiscard]] llvm::Value* umulWithOverflow(const StrategyContext& ctx) const noexcept override { llvm::report_fatal_error("Unsupported overflow intrinsic"); }
    };

    // ============================================================================
    // Type Aliases (matching your previous class names exactly)
    // ============================================================================
    using ShortOverflowStrategy  = OverflowStrategyImpl<short>;
    using IntOverflowStrategy    = OverflowStrategyImpl<int>;
    using LongOverflowStrategy   = OverflowStrategyImpl<long>;
    using FloatOverflowStrategy  = OverflowStrategyImpl<float>;
    using DoubleOverflowStrategy = OverflowStrategyImpl<double>;

}

#endif 
