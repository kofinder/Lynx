#ifndef LYNX_RESOLVER_ABS_STRATEGY_HPP
#define LYNX_RESOLVER_ABS_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <context/AstContext.hpp>

namespace LynxResolver {

    using LynxContext::AstContext;

    struct AbsStrategy {

        /// Absolute value
        [[nodiscard]] virtual llvm::Value* abs(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        /// Negate value (-val)
        [[nodiscard]] virtual llvm::Value* negate(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        /// Return +1 for positive, -1 for negative, 0 for zero
        [[nodiscard]] virtual llvm::Value* sign(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        /// Clamp value between min and max
        [[nodiscard]] virtual llvm::Value* clamp(const AstContext& ctx, llvm::Value* val, llvm::Value* minVal, llvm::Value* maxVal) const noexcept = 0;

        /// Check if value is even (for integer types)
        [[nodiscard]] virtual llvm::Value* isEven(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        /// Check if value is odd (for integer types)
        [[nodiscard]] virtual llvm::Value* isOdd(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        virtual ~AbsStrategy() noexcept = default;

    };

    struct IntAbsStrategy : AbsStrategy {

        [[nodiscard]] llvm::Value* abs(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* negate(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* sign(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* clamp(const AstContext& ctx, llvm::Value* val, llvm::Value* minVal, llvm::Value* maxVal) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* isEven(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* isOdd(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        ~IntAbsStrategy() noexcept override = default;
    }; 
}

#endif 
