#ifndef LYNX_RESOLVER_MIN_MAX_STRATEGY_HPP
#define LYNX_RESOLVER_MIN_MAX_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include <context/AstContext.hpp>
#include <logger/Logger.hpp>

namespace LynxResolver {

    using LynxContext::AstContext;
    using namespace LynxLogger;

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

    struct IntMinMaxStrategy : MinMaxStrategy {
        
        [[nodiscard]] llvm::Value* smin(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept override {
            LOG_ERROR("Dispatched ..");
            return nullptr;
        }

        [[nodiscard]] llvm::Value* smax(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept override {
            LOG_ERROR("Dispatched ..");
            return nullptr;
        }

        [[nodiscard]] llvm::Value* umin(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept override {
            LOG_ERROR("Dispatched ..");
            return nullptr;
        }

        [[nodiscard]] llvm::Value* umax(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept override {
            LOG_ERROR("Dispatched ..");
            return nullptr;
        }

        ~IntMinMaxStrategy() noexcept override = default;
    };

}

#endif 
