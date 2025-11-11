#ifndef LYNX_RESOLVER_ARITHMETIC_STRATEGY_HPP
#define LYNX_RESOLVER_ARITHMETIC_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <context/AstContext.hpp>

namespace LynxResolver {

    using LynxContext::AstContext;

    struct ArithmeticStrategy {

        [[nodiscard]] virtual llvm::Value* add(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* sub(const AstContext& ctx, llvm::IRBuilder<>& builder, llvm::Module* module, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* mul(const AstContext& ctx, llvm::IRBuilder<>& builder, llvm::Module* module, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* div(const AstContext& ctx, llvm::IRBuilder<>& builder, llvm::Module* module, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* mod(const AstContext& ctx, llvm::IRBuilder<>& builder, llvm::Module* module, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;
        
        virtual ~ArithmeticStrategy() noexcept = default;

    };
}

#endif 
