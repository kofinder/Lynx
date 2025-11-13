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

        [[nodiscard]] virtual llvm::Value* sub(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* mul(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* div(const AstContext& ctx, llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;

        [[nodiscard]] virtual llvm::Value* mod(const AstContext& ctx,  llvm::Value* lhs, llvm::Value* rhs) const noexcept = 0;
        
        virtual ~ArithmeticStrategy() noexcept = default;

    };


    struct IntArithmeticStrategy : ArithmeticStrategy {

        [[nodiscard]] llvm::Value* add(const AstContext& ctx, llvm::Value* lhsPtr, llvm::Value* rhs) const noexcept override {
            LOG_ERROR("Emit method correctly");

            if (!lhsPtr || !rhs) return nullptr;

            auto& builder = ctx.getBuilder();
            auto& llvmCtx = ctx.getLLVMContext();


            auto* i8PtrTy = llvm::PointerType::get(llvmCtx, 0);

            // Load the current value
            llvm::Value* lhsVal = builder.CreateLoad(i8PtrTy, lhsPtr, "load_lhs");
        
            // Perform addition
            llvm::Value* result = builder.CreateAdd(lhsVal, rhs, "int_add");
        
            // Store the result back to the original variable
            builder.CreateStore(result, lhsPtr);
        
            return result; // optionally return the new value
        
    
            // auto& builder = ctx.getBuilder();
        
            // // Emit LLVM IR for integer addition
            // llvm::Value* result = builder.CreateAdd(lhs, rhs, "int_add");
    
            // builder.CreateStore(result, lhs);

            // LOG_INFO("Emitted integer addition for {} + {}", lhs->getName().str(), rhs->getName().str());
            // return result;
        }

        [[nodiscard]] llvm::Value* sub(const AstContext& ctx,  llvm::Value* lhs, llvm::Value* rhs) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* mul(const AstContext& ctx,  llvm::Value* lhs, llvm::Value* rhs) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* div(const AstContext& ctx,  llvm::Value* lhs, llvm::Value* rhs) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* mod(const AstContext& ctx,  llvm::Value* lhs, llvm::Value* rhs) const noexcept override {
            return nullptr;
        }

        ~IntArithmeticStrategy() noexcept override = default;

    };
}

#endif 
