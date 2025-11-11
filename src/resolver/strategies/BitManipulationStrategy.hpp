#ifndef LYNX_RESOLVER_BIT_MINIPULATION_STRATEGY_HPP
#define LYNX_RESOLVER_BIT_MINIPULATION_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <context/AstContext.hpp>

namespace LynxResolver {

    using LynxContext::AstContext;

    struct BitManipulationStrategy {

        /// Count number of set bits (population count)
        [[nodiscard]] virtual llvm::Value* popCount(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        /// Count leading zeros
        [[nodiscard]] virtual llvm::Value* countLeadingZeros(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        /// Count trailing zeros
        [[nodiscard]] virtual llvm::Value* countTrailingZeros(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        /// Bitwise reverse (mirror bits)
        [[nodiscard]] virtual llvm::Value* bitReverse(const AstContext& ctx, llvm::Value* val) const noexcept = 0;

        /// Rotate left
        [[nodiscard]] virtual llvm::Value* rotateLeft(const AstContext& ctx, llvm::Value* val, llvm::Value* shift) const noexcept = 0;

        /// Rotate right
        [[nodiscard]] virtual llvm::Value* rotateRight(const AstContext& ctx, llvm::Value* val, llvm::Value* shift) const noexcept = 0;

        virtual ~BitManipulationStrategy() noexcept = default;
        
    };


    struct IntBitManipulationStrategy : BitManipulationStrategy {

        [[nodiscard]] llvm::Value* popCount(const AstContext& ctx, llvm::Value* val) const noexcept override {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* countLeadingZeros(const AstContext& ctx, llvm::Value* val) const noexcept {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* countTrailingZeros(const AstContext& ctx, llvm::Value* val) const noexcept {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* bitReverse(const AstContext& ctx, llvm::Value* val) const noexcept {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* rotateLeft(const AstContext& ctx, llvm::Value* val, llvm::Value* shift) const noexcept {
            return nullptr;
        }

        [[nodiscard]] llvm::Value* rotateRight(const AstContext& ctx, llvm::Value* val, llvm::Value* shift) const noexcept {
            return nullptr;
        }

        ~IntBitManipulationStrategy() noexcept override = default;
        
    };

}

#endif 
