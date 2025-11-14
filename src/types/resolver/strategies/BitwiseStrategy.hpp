#ifndef LYNX_RESOLVER_BITWISE_STRATEGY_HPP
#define LYNX_RESOLVER_BITWISE_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <context/AstContext.hpp>

namespace LynxTypes {

    using LynxContext::AstContext;

    struct BitwiseStrategy {

        [[nodiscard]] virtual llvm::Value* bitAnd(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* bitOr(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* bitXor(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* shl(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* shr(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* bitNot(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        virtual ~BitwiseStrategy() noexcept = default;
    };


    struct IntBitwiseStrategy : BitwiseStrategy {

        [[nodiscard]] llvm::Value* bitAnd(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* bitOr(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* bitXor(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* shl(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* shr(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* bitNot(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        ~IntBitwiseStrategy() noexcept override = default;
    };
    
    
}

#endif 
