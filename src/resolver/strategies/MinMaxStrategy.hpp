#ifndef LYNX_RESOLVER_MIN_MAX_STRATEGY_HPP
#define LYNX_RESOLVER_MIN_MAX_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include <context/AstContext.hpp>

namespace LynxResolver {

    using LynxContext::AstContext;

    struct MinMaxStrategy {
        /// Signed minimum
        [[nodiscard]] virtual llvm::Value* smin(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        /// Signed maximum
        [[nodiscard]] virtual llvm::Value* smax(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        /// Unsigned minimum
        [[nodiscard]] virtual llvm::Value* umin(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        /// Unsigned maximum
        [[nodiscard]] virtual llvm::Value* umax(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        virtual ~MinMaxStrategy() noexcept = default;
    };

}

#endif 
