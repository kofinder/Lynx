#ifndef LYNX_RESOLVER_SATURATION_STRATEGY_HPP
#define LYNX_RESOLVER_SATURATION_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include <context/AstContext.hpp>

namespace LynxResolver {

    using LynxContext::AstContext;

    struct SaturationStrategy {
        /// Signed addition with saturation
        [[nodiscard]] virtual llvm::Value* saddSat(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        /// Unsigned addition with saturation
        [[nodiscard]] virtual llvm::Value* uaddSat(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        /// Signed subtraction with saturation
        [[nodiscard]] virtual llvm::Value* ssubSat(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        /// Unsigned subtraction with saturation
        [[nodiscard]] virtual llvm::Value* usubSat(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        /// Signed shift left with saturation
        [[nodiscard]] virtual llvm::Value* sshlSat(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        /// Unsigned shift left with saturation
        [[nodiscard]] virtual llvm::Value* ushLSat(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        virtual ~SaturationStrategy() noexcept = default;
    };

    struct IntSaturationStrategy : SaturationStrategy {

        [[nodiscard]] llvm::Value* saddSat(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* uaddSat(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* ssubSat(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* usubSat(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* sshlSat(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* ushLSat(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept override {
            return nullptr;
        }

        ~IntSaturationStrategy() noexcept override = default;
    };
}

#endif 
