#ifndef LYNX_TYPE_RESOLVER_MIXINS_HPP
#define LYNX_TYPE_RESOLVER_MIXINS_HPP

#include <concepts>
#include <string>
#include <llvm/IR/Value.h>
#include <context/AstContext.hpp>

namespace LynxTypes {

    using LynxContext::AstContext;

    template<typename T>
    concept ToStringCapable = requires(T t, AstContext& ctx, llvm::Value* val) {
        { T::convertToString(ctx, val) } -> std::same_as<llvm::Value*>;
    };

    template<typename T>
    concept CloneCapable = requires(T t, AstContext& ctx, llvm::Value* val) {
        { T::performClone(ctx, val) } -> std::same_as<llvm::Value*>;
    };

    template<typename T>
    concept TypeCastCapable = requires(T t, AstContext& ctx, llvm::Value* val, llvm::Type* type) {
        { T::performTypeCast(ctx, val, type) } -> std::same_as<llvm::Value*>;
    };

    template<ToStringCapable DerivedT>
    struct ToStringMixin {
        [[nodiscard]] llvm::Value* toString(AstContext& ctx, llvm::Value* instance) const noexcept {
            return DerivedT::convertToString(ctx, instance);
        }
    };

    template<CloneCapable DerivedT>
    struct CloneMixin {
        [[nodiscard]] llvm::Value* clone(AstContext& ctx, llvm::Value* instance) const noexcept {
            return DerivedT::performClone(ctx, instance);
        }
    };

    template<TypeCastCapable DerivedT>
    struct TypeCastMixin {
        [[nodiscard]] llvm::Value* typeCast(AstContext& ctx, llvm::Value* instance, llvm::Type* targetType) const noexcept {
            return DerivedT::performTypeCast(ctx, instance);
        }
    };

} 

#endif