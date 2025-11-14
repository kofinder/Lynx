#ifndef LYNX_RESOLVER_MATH_STRATEGY_HPP
#define LYNX_RESOLVER_MATH_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include <context/AstContext.hpp>

namespace LynxTypes {

    using LynxContext::AstContext;

    struct MathStrategy {

        [[nodiscard]] virtual llvm::Value* sqrt(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* pow(const AstContext& ctx, llvm::Value* base, llvm::Value* exp) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* exp(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* exp2(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* exp10(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* log(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* log2(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* log10(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* sin(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* cos(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* tan(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* asin(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* acos(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* atan(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* atan2(const AstContext& ctx, llvm::Value* y, llvm::Value* x) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* sinh(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* cosh(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* tanh(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* floor(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* ceil(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* trunc(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* round(const AstContext& ctx, llvm::Value* val) const noexcept = 0;
        
        [[nodiscard]] virtual llvm::Value* fabs(const AstContext& ctx, llvm::Value* val) const noexcept = 0;
    
        virtual ~MathStrategy() noexcept = default;
    
    };


    struct IntMathStrategy : MathStrategy {

        [[nodiscard]] llvm::Value* sqrt(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* pow(const AstContext& ctx, llvm::Value* base, llvm::Value* exp) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* exp(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* exp2(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* exp10(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* log(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* log2(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* log10(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* sin(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* cos(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* tan(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* asin(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* acos(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* atan(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* atan2(const AstContext& ctx, llvm::Value* y, llvm::Value* x) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* sinh(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* cosh(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* tanh(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* floor(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* ceil(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* trunc(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* round(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }
        
        [[nodiscard]] llvm::Value* fabs(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }
    
        ~IntMathStrategy() noexcept override = default;
    
    };

}

#endif 
