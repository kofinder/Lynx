#ifndef LYNX_RESOLVER_FIXED_POINT_STRATEGY_HPP
#define LYNX_RESOLVER_FIXED_POINT_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include <context/AstContext.hpp>

namespace LynxResolver {

    using LynxContext::AstContext;

    struct FixedPointStrategy {
        
        /// Signed multiply fixed-point: sMul
        [[nodiscard]] virtual llvm::Value* mul(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs, bool saturate = false) const noexcept = 0;

        /// Unsigned multiply fixed-point: uMul
        [[nodiscard]] virtual llvm::Value* umul(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs, bool saturate = false) const noexcept = 0;

        /// Signed divide fixed-point: sDiv
        [[nodiscard]] virtual llvm::Value* div(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs, bool saturate = false) const noexcept = 0;

        /// Unsigned divide fixed-point: uDiv
        [[nodiscard]] virtual llvm::Value* udiv(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs, bool saturate = false) const noexcept = 0;

        virtual ~FixedPointStrategy() noexcept = default;

    };


    struct IntFixedPointStrategy : FixedPointStrategy {
        
        [[nodiscard]] llvm::Value* mul(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs, bool saturate = false) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* umul(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs, bool saturate = false) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* div(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs, bool saturate = false) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* udiv(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs, bool saturate = false) const noexcept override {
            return nullptr;
        }

        ~IntFixedPointStrategy() noexcept override = default;

    };

}

#endif 
