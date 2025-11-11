#ifndef LYNX_RESOLVER_OVERFLOW_STRATEGY_HPP
#define LYNX_RESOLVER_OVERFLOW_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include <context/AstContext.hpp>

namespace LynxResolver {

    using LynxContext::AstContext;

    struct OverflowStrategy {
        
        /// Signed addition with overflow
        [[nodiscard]] virtual llvm::Value* saddWithOverflow(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        /// Unsigned addition with overflow
        [[nodiscard]] virtual llvm::Value* uaddWithOverflow(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        /// Signed subtraction with overflow
        [[nodiscard]] virtual llvm::Value* ssubWithOverflow(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        /// Unsigned subtraction with overflow
        [[nodiscard]] virtual llvm::Value* usubWithOverflow(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        /// Signed multiplication with overflow
        [[nodiscard]] virtual llvm::Value* smulWithOverflow(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        /// Unsigned multiplication with overflow
        [[nodiscard]] virtual llvm::Value* umulWithOverflow(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        virtual ~OverflowStrategy() noexcept = default;

    };

}

#endif 
