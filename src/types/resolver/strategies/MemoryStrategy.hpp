#ifndef LYNX_RESOLVER_MEMORY_STRATEGY_HPP
#define LYNX_RESOLVER_MEMORY_STRATEGY_HPP

#include <llvm/IR/Value.h>
#include <context/AstContext.hpp>

namespace LynxTypes {

    using LynxContext::AstContext;

    struct MemoryStrategy {
        
        /// Copy memory: dst <- src
        [[nodiscard]] virtual llvm::Value* memcpy(const AstContext& ctx, llvm::Value* dst, llvm::Value* src, llvm::Value* size) const noexcept = 0;

        /// Move memory: like memcpy but handles overlapping regions
        [[nodiscard]] virtual llvm::Value* memmove(const AstContext& ctx, llvm::Value* dst, llvm::Value* src, llvm::Value* size) const noexcept = 0;

        /// Set memory: fill region with a value
        [[nodiscard]] virtual llvm::Value* memset(const AstContext& ctx, llvm::Value* dst, llvm::Value* value, llvm::Value* size) const noexcept = 0;

        /// Experimental memset with pattern
        [[nodiscard]] virtual llvm::Value* memsetPattern(const AstContext& ctx, llvm::Value* dst, llvm::Value* pattern, llvm::Value* size) const noexcept = 0;

        virtual ~MemoryStrategy() noexcept = default;

    };


    struct IntMemoryStrategy : MemoryStrategy {
        
        /// Copy memory: dst <- src
        [[nodiscard]] llvm::Value* memcpy(const AstContext& ctx, llvm::Value* dst, llvm::Value* src, llvm::Value* size) const noexcept override {
            return nullptr;
        }

        /// Move memory: like memcpy but handles overlapping regions
        [[nodiscard]] llvm::Value* memmove(const AstContext& ctx, llvm::Value* dst, llvm::Value* src, llvm::Value* size) const noexcept override {
            return nullptr;
        }

        /// Set memory: fill region with a value
        [[nodiscard]] llvm::Value* memset(const AstContext& ctx, llvm::Value* dst, llvm::Value* value, llvm::Value* size) const noexcept override {
            return nullptr;
        }

        /// Experimental memset with pattern
        [[nodiscard]] llvm::Value* memsetPattern(const AstContext& ctx, llvm::Value* dst, llvm::Value* pattern, llvm::Value* size) const noexcept override {
            return nullptr;
        }

        ~IntMemoryStrategy() noexcept override = default;

    };

}

#endif 
