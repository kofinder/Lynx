#ifndef LYNX_TYPE_RESOLVER_MIXINS_HPP
#define LYNX_TYPE_RESOLVER_MIXINS_HPP

#include <concepts>
#include <string>
#include <llvm/IR/Value.h>
#include <context/AstContext.hpp>

namespace LynxTypes {

    using LynxContext::AstContext;

    // Concept definitions
    template<typename T>
    concept ToStringCapable = requires(T t, llvm::Value* val, std::shared_ptr<AstContext> ctx) {
        { T::convertToString(val, ctx) } -> std::same_as<llvm::Value*>;
    };

    template<typename T>
    concept CloneCapable = requires(T t, llvm::Value* val, std::shared_ptr<AstContext> ctx) {
        { T::performClone(val, ctx) } -> std::same_as<llvm::Value*>;
    };

    template<typename T>
    concept TypeCastCapable = requires(T t, llvm::Value* val, llvm::Type* type, std::shared_ptr<AstContext> ctx) {
        { T::performTypeCast(val, type, ctx) } -> std::same_as<llvm::Value*>;
    };

    // Mixins using concepts
    template<ToStringCapable DerivedT>
    struct ToStringMixin {
        [[nodiscard]] llvm::Value* toString(llvm::Value* instance, std::shared_ptr<AstContext> ctx) const noexcept {
            return DerivedT::convertToString(instance, ctx);
        }
    };

    template<CloneCapable DerivedT>
    struct CloneMixin {
        [[nodiscard]] llvm::Value* clone(llvm::Value* instance, std::shared_ptr<AstContext> ctx) const noexcept {
            return DerivedT::performClone(instance, ctx);
        }
    };

    template<TypeCastCapable DerivedT>
    struct TypeCastMixin {
        [[nodiscard]] llvm::Value* typeCast(llvm::Value* instance, llvm::Type* targetType, std::shared_ptr<AstContext> ctx) const noexcept {
            return DerivedT::performTypeCast(instance, targetType, ctx);
        }
    };

} 

#endif