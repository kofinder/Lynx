#ifndef LYNX_RESOLVER_COMPRASION_STRATEGY_HPP
#define LYNX_RESOLVER_COMPRASION_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include <context/AstContext.hpp>

namespace LynxResolver {

    using LynxContext::AstContext;

    struct ComparisonStrategy {
        /// Equal (==)
        [[nodiscard]] virtual llvm::Value* eq(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        /// Not equal (!=)
        [[nodiscard]] virtual llvm::Value* ne(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        /// Less than (<)
        [[nodiscard]] virtual llvm::Value* lt(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        /// Less than or equal (<=)
        [[nodiscard]] virtual llvm::Value* le(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        /// Greater than (>)
        [[nodiscard]] virtual llvm::Value* gt(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        /// Greater than or equal (>=)
        [[nodiscard]] virtual llvm::Value* ge(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

    };

}

#endif 
