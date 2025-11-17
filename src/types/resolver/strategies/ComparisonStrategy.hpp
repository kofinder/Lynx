/**
 * @file ComparisonStrategy.hpp
 * @brief Defines the abstraction and template implementations for comparison operations.
 *
 * This header declares the `ComparisonStrategy` interface, providing methods
 * for standard comparison operations such as equality, inequality, less than,
 * less than or equal, greater than, and greater than or equal.
 * The interface is intended to support LLVM IR code generation for numeric types.
 *
 * Key components:
 *  - `ComparisonStrategy`: abstract base class defining the interface for comparisons.
 *  - `ComparisonStrategyImpl<T>`: template specialization for integer and floating-point types.
 *  - Type aliases (`ShortComparisonStrategy`, `IntComparisonStrategy`, etc.) for convenience.
 *
 * Features:
 *  - Integer and floating-point specializations currently provide placeholders returning `nullptr`.
 *  - Extensible design allows future implementations for LLVM IR code generation.
 *
 * Benefits:
 *  - Centralizes comparison logic for numeric types.
 *  - Provides a uniform interface for backend compiler generation.
 *  - Promotes consistency and maintainability across the Lynx type system.
 *
 * Notes:
 *  - All method implementations are placeholders and intended for future expansion.
 *
 * @author Ko Thein (Nathan Mratt)
 * @date   November 2, 2024
*/

#ifndef LYNX_RESOLVER_COMPRASION_STRATEGY_HPP
#define LYNX_RESOLVER_COMPRASION_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include "helpers/CompareStrategyHelper.hpp"
#include "resolver/TypeStrategyContext.hpp"

namespace LynxTypes {

    // ============================================================================
    // Base Interface
    // ============================================================================
    struct ComparisonStrategy {
        [[nodiscard]] virtual llvm::Value* eq(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* ne(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* lt(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* le(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* gt(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* ge(const StrategyContext&) const noexcept = 0;
        virtual ~ComparisonStrategy() noexcept = default;
    };


    // ============================================================================
    // Forward declaration for primary template
    // ============================================================================
    template<typename T>
    struct ComparisonStrategyImpl;

    // ============================================================================
    // Integer Specialization
    // ============================================================================
    template<IntStrategyType T>
    struct ComparisonStrategyImpl<T> : ComparisonStrategy {
        [[nodiscard]] llvm::Value* eq(const StrategyContext& stgCtx) const noexcept override {
            return helper::compareEq(stgCtx.ctx.getBuilder(), stgCtx.instance, stgCtx.args[0], false);
        }
        [[nodiscard]] llvm::Value* ne(const StrategyContext& stgCtx) const noexcept override {
            return helper::compareNe(stgCtx.ctx.getBuilder(), stgCtx.instance, stgCtx.args[0], false);
        }
        [[nodiscard]] llvm::Value* lt(const StrategyContext& stgCtx) const noexcept override {
            return helper::compareLt(stgCtx.ctx.getBuilder(), stgCtx.instance, stgCtx.args[0], false);
        }
        [[nodiscard]] llvm::Value* le(const StrategyContext& stgCtx) const noexcept override {
            return helper::compareLe(stgCtx.ctx.getBuilder(), stgCtx.instance, stgCtx.args[0], false);
        }
        [[nodiscard]] llvm::Value* gt(const StrategyContext& stgCtx) const noexcept override {
            return helper::compareGt(stgCtx.ctx.getBuilder(), stgCtx.instance, stgCtx.args[0], false);
        }
        [[nodiscard]] llvm::Value* ge(const StrategyContext& stgCtx) const noexcept override {
            return helper::compareGe(stgCtx.ctx.getBuilder(), stgCtx.instance, stgCtx.args[0], false);
        }
    };

    // ============================================================================
    // Floating-Point Specialization
    // ============================================================================
    template<FloatStrategyType T>
    struct ComparisonStrategyImpl<T> : ComparisonStrategy {
        [[nodiscard]] llvm::Value* eq(const StrategyContext& stgCtx) const noexcept override {
            return helper::compareEq(stgCtx.ctx.getBuilder(), stgCtx.instance, stgCtx.args[0], true);
        }
        [[nodiscard]] llvm::Value* ne(const StrategyContext& stgCtx) const noexcept override {
            return helper::compareNe(stgCtx.ctx.getBuilder(), stgCtx.instance, stgCtx.args[0], true);
        }
        [[nodiscard]] llvm::Value* lt(const StrategyContext& stgCtx) const noexcept override {
            return helper::compareLt(stgCtx.ctx.getBuilder(), stgCtx.instance, stgCtx.args[0], true);
        }
        [[nodiscard]] llvm::Value* le(const StrategyContext& stgCtx) const noexcept override {
            return helper::compareLe(stgCtx.ctx.getBuilder(), stgCtx.instance, stgCtx.args[0], true);
        }
        [[nodiscard]] llvm::Value* gt(const StrategyContext& stgCtx) const noexcept override {
            return helper::compareGt(stgCtx.ctx.getBuilder(), stgCtx.instance, stgCtx.args[0], true);
        }
        [[nodiscard]] llvm::Value* ge(const StrategyContext& stgCtx) const noexcept override {
            return helper::compareGe(stgCtx.ctx.getBuilder(), stgCtx.instance, stgCtx.args[0], true);
        }    
    };

    // ============================================================================
    // Type Aliases (matching your previous class names exactly)
    // ============================================================================
    using ShortComparisonStrategy  = ComparisonStrategyImpl<short>;
    using IntComparisonStrategy    = ComparisonStrategyImpl<int>;
    using LongComparisonStrategy   = ComparisonStrategyImpl<long>;
    using FloatComparisonStrategy  = ComparisonStrategyImpl<float>;
    using DoubleComparisonStrategy = ComparisonStrategyImpl<double>;
}

#endif 
