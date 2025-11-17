/**
 * @file MinMaxStrategy.hpp
 * @brief Defines the MinMaxStrategy interface and template specializations for numeric types.
 *
 * This header provides an abstraction for computing minimum and maximum values
 * for both signed and unsigned contexts, enabling LLVM IR code generation for
 * type-specific min/max operations across the Lynx type system.
 *
 * Key components:
 *  - `MinMaxStrategy`: abstract base class defining signed and unsigned min/max methods.
 *  - `MinMaxStrategyImpl<T>`: template specializations for integer and floating-point types.
 *  - Type aliases (`ShortMinMaxStrategy`, `IntMinMaxStrategy`, etc.) for convenience.
 *
 * Features:
 *  - Default implementations return `nullptr` as placeholders.
 *  - Designed to integrate with backend LLVM IR generation for compiler operations.
 *
 * Benefits:
 *  - Centralizes min/max operation logic for numeric types.
 *  - Ensures a uniform interface for compiler code generation.
 *  - Improves maintainability and consistency in numeric computations.
 *
 * @author Ko Thein (Nathan Mratt)
 * @date   November 2, 2024
*/

#ifndef LYNX_RESOLVER_MIN_MAX_STRATEGY_HPP
#define LYNX_RESOLVER_MIN_MAX_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include "resolver/TypeStrategyContext.hpp"

namespace LynxTypes {

    // ============================================================================
    // Base Interface
    // ============================================================================
    struct MinMaxStrategy {
        [[nodiscard]] virtual llvm::Value* smin(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* smax(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* umin(const StrategyContext&) const noexcept = 0;
        [[nodiscard]] virtual llvm::Value* umax(const StrategyContext&) const noexcept = 0;

        virtual ~MinMaxStrategy() noexcept = default;
    };

    // ============================================================================
    // Forward declaration for primary template
    // ============================================================================
    template<typename T>
    struct MinMaxStrategyImpl;

    // ============================================================================
    // Integer Specialization
    // ============================================================================
    template<IntStrategyType T>
    struct MinMaxStrategyImpl<T> : MinMaxStrategy {
        [[nodiscard]] llvm::Value* smin(const StrategyContext& stgCtx) const noexcept override {
            if (stgCtx.args.size() < 1) return nullptr;

            llvm::IRBuilder<>& builder = stgCtx.ctx.getBuilder();
            llvm::Value* lhs = stgCtx.instance;
            llvm::Value* rhs = stgCtx.args[0];

            llvm::Value* cmp = builder.CreateICmpSLT(lhs, rhs); // lhs < rhs (signed)
            llvm::Value* result = builder.CreateSelect(cmp, lhs, rhs);
            if (stgCtx.instancePtr) builder.CreateStore(result, stgCtx.instancePtr);
            return result;    
        }

        [[nodiscard]] llvm::Value* smax(const StrategyContext& stgCtx) const noexcept override { 
            if (stgCtx.args.size() < 1) return nullptr;

            llvm::IRBuilder<>& builder = stgCtx.ctx.getBuilder();
            llvm::Value* lhs = stgCtx.instance;
            llvm::Value* rhs = stgCtx.args[0];

            llvm::Value* cmp = builder.CreateICmpSGT(lhs, rhs); // lhs > rhs (signed)
            llvm::Value* result = builder.CreateSelect(cmp, lhs, rhs);
            if (stgCtx.instancePtr) builder.CreateStore(result, stgCtx.instancePtr);
            return result;    
        }

        [[nodiscard]] llvm::Value* umin(const StrategyContext& stgCtx) const noexcept override { 
            if (stgCtx.args.size() < 1) return nullptr;

            llvm::IRBuilder<>& builder = stgCtx.ctx.getBuilder();
            llvm::Value* lhs = stgCtx.instance;
            llvm::Value* rhs = stgCtx.args[0];

            llvm::Value* cmp = builder.CreateICmpULT(lhs, rhs); // lhs < rhs (unsigned)
            llvm::Value* result = builder.CreateSelect(cmp, lhs, rhs);
            if (stgCtx.instancePtr) builder.CreateStore(result, stgCtx.instancePtr);
            return result;    
        }

        [[nodiscard]] llvm::Value* umax(const StrategyContext& stgCtx) const noexcept override { 
            if (stgCtx.args.size() < 1) return nullptr;

            llvm::IRBuilder<>& builder = stgCtx.ctx.getBuilder();
            llvm::Value* lhs = stgCtx.instance;
            llvm::Value* rhs = stgCtx.args[0];

            llvm::Value* cmp = builder.CreateICmpUGT(lhs, rhs); // lhs > rhs (unsigned)
            llvm::Value* result = builder.CreateSelect(cmp, lhs, rhs);
            if (stgCtx.instancePtr) builder.CreateStore(result, stgCtx.instancePtr);
            return result;    
        }
    };

    // ============================================================================
    // Floating-Point Specialization
    // ============================================================================
    template<FloatStrategyType T>
    struct MinMaxStrategyImpl<T> : MinMaxStrategy {

        private:

            llvm::Function* getFMinNumIntrinsic(llvm::IRBuilder<>& builder, llvm::Type* fpTy) const noexcept {
                llvm::Module* M = builder.GetInsertBlock()->getModule();
                return llvm::Intrinsic::getOrInsertDeclaration(M, llvm::Intrinsic::minnum, {fpTy});
            }
            llvm::Function* getFMaxNumIntrinsic(llvm::IRBuilder<>& builder, llvm::Type* fpTy) const noexcept {
                llvm::Module* M = builder.GetInsertBlock()->getModule();
                return llvm::Intrinsic::getOrInsertDeclaration(M, llvm::Intrinsic::maxnum, {fpTy});
            }
            
        public:

            [[nodiscard]] llvm::Value* smin(const StrategyContext& stgCtx) const noexcept override {
                if (stgCtx.args.size() < 1) return nullptr;

                llvm::IRBuilder<>& builder = stgCtx.ctx.getBuilder();
                llvm::Value* lhs = stgCtx.instance;
                llvm::Value* rhs = stgCtx.args[0];
                llvm::Function* fminnum = getFMinNumIntrinsic(builder, lhs->getType());
                llvm::Value* result = builder.CreateCall(fminnum, {lhs, rhs});
                if (stgCtx.instancePtr) builder.CreateStore(result, stgCtx.instancePtr);
                return result;
            }

            [[nodiscard]] llvm::Value* smax(const StrategyContext& stgCtx) const noexcept override {
                if (stgCtx.args.size() < 1) return nullptr;
                llvm::IRBuilder<>& builder = stgCtx.ctx.getBuilder();
                llvm::Value* lhs = stgCtx.instance;
                llvm::Value* rhs = stgCtx.args[0];
                llvm::Function* fmaxnum = getFMaxNumIntrinsic(builder, lhs->getType());
                llvm::Value* result = builder.CreateCall(fmaxnum, {lhs, rhs});
                if (stgCtx.instancePtr) builder.CreateStore(result, stgCtx.instancePtr);
                return result;
            }

            [[nodiscard]] llvm::Value* umin(const StrategyContext& stgCtx) const noexcept override { return smin(stgCtx); }
            [[nodiscard]] llvm::Value* umax(const StrategyContext& stgCtx) const noexcept override { return smax(stgCtx); }
    };

    // ============================================================================
    // Type Aliases (matching your previous class names exactly)
    // ============================================================================
    using ShortMinMaxStrategy  = MinMaxStrategyImpl<short>;
    using IntMinMaxStrategy    = MinMaxStrategyImpl<int>;
    using LongMinMaxStrategy   = MinMaxStrategyImpl<long>;
    using FloatMinMaxStrategy  = MinMaxStrategyImpl<float>;
    using DoubleMinMaxStrategy = MinMaxStrategyImpl<double>;
}

#endif 
