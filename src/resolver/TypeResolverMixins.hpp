#ifndef LYNX_TYPE_RESOLVER_MIXINS_HPP
#define LYNX_TYPE_RESOLVER_MIXINS_HPP

#include <concepts>
#include <string>
#include <llvm/IR/Value.h>
#include <context/AstContext.hpp>

namespace LynxResolver {

    using LynxContext::AstContext;

    // =======================
    // ToString Mixin
    // =======================
    template<typename DerivedT>
    concept ToStringCapable = requires(llvm::Value* val, std::shared_ptr<AstContext> ctx) {
        { DerivedT::convertToString(val, ctx) } -> std::same_as<llvm::Value*>;
    };

    template<ToStringCapable DerivedT>
    struct ToStringMixin {
        [[nodiscard]] llvm::Value* toString(llvm::Value* instance, std::shared_ptr<AstContext> ctx) const noexcept {
            return DerivedT::convertToString(instance, ctx);
        }
    };

    // =======================
    // Clone Mixin
    // =======================
    template<typename DerivedT>
    concept CloneCapable = requires(llvm::Value* val, std::shared_ptr<AstContext> ctx) {
        { DerivedT::performClone(val, ctx) } -> std::same_as<llvm::Value*>;
    };

    template<CloneCapable DerivedT>
    struct CloneMixin {
        [[nodiscard]] llvm::Value* clone(llvm::Value* instance, std::shared_ptr<AstContext> ctx) const noexcept {
            return DerivedT::performClone(instance, ctx);
        }
    };

    // =======================
    // TypeCast Mixin
    // =======================
    template<typename DerivedT>
    concept TypeCastCapable = requires(llvm::Value* val, llvm::Type* type, std::shared_ptr<AstContext> ctx) {
        { DerivedT::performTypeCast(val, type, ctx) } -> std::same_as<llvm::Value*>;
    };

    template<TypeCastCapable DerivedT>
    struct TypeCastMixin {
        [[nodiscard]] llvm::Value* typeCast(llvm::Value* instance, llvm::Type* targetType, std::shared_ptr<AstContext> ctx) const noexcept {
            return DerivedT::performTypeCast(instance, targetType, ctx);
        }
    };

} 

#endif