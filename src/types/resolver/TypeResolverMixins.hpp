/**
 * @file TypeResolverMixins.hpp
 * @brief Provides mixin utilities enabling type-specific behaviors in the Lynx type system.
 *
 * This header defines CRTP-based mixins and concepts that extend type resolvers with additional
 * capabilities—such as string conversion, cloning, and type casting. These mixins ensure that
 * derived types implement the required static methods while providing a clean, uniform interface
 * for behavior injection into Lynx type resolvers.
 *
 * The design promotes modularity and code reuse by allowing type-specific logic to be added
 * selectively through capability-based mixins, without requiring large inheritance hierarchies
 * or modifying the core resolver logic.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_TYPE_RESOLVER_MIXINS_HPP
#define LYNX_TYPE_RESOLVER_MIXINS_HPP

#include <concepts>
#include <string>
#include <llvm/IR/Value.h>
#include <context/AstContext.hpp>

namespace LynxTypes {

    using LynxContext::AstContext;

    template<typename T>
    concept ToStringCapable = requires(T t, const AstContext& ctx, llvm::Value* val) {
        { T::convertToString(ctx, val) } -> std::same_as<llvm::Value*>;
    };

    template<typename T>
    concept CloneCapable = requires(T t, const AstContext& ctx, llvm::Value* val) {
        { T::performClone(ctx, val) } -> std::same_as<llvm::Value*>;
    };

    template<typename T>
    concept TypeCastCapable = requires(T t, const AstContext& ctx, llvm::Value* val, llvm::Type* type) {
        { T::performTypeCast(ctx, val, type) } -> std::same_as<llvm::Value*>;
    };

    template<ToStringCapable DerivedT>
    struct ToStringMixin {
        [[nodiscard]] llvm::Value* toString(const AstContext& ctx, llvm::Value* instance) const noexcept {
            return DerivedT::convertToString(ctx, instance);
        }
    };

    template<CloneCapable DerivedT>
    struct CloneMixin {
        [[nodiscard]] llvm::Value* clone(const AstContext& ctx, llvm::Value* instance) const noexcept {
            return DerivedT::performClone(ctx, instance);
        }
    };

    template<TypeCastCapable DerivedT>
    struct TypeCastMixin {
        [[nodiscard]] llvm::Value* typeCast(const AstContext& ctx, llvm::Value* instance, llvm::Type* targetType) const noexcept {
            return DerivedT::performTypeCast(ctx, instance);
        }
    };

} 

#endif