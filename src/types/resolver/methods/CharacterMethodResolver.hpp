/**
 * @file CharacterMethodResolver.hpp
 * @brief Concrete TypeMethodResolver for the `Char` type.
 *
 * `CharacterMethodResolver` implements the `resolveMethod` interface to handle
 * method calls on `Char` instances in Lynx. It maps character-specific methods
 * to their corresponding LLVM IR instructions or runtime operations.
 *
 * Example usage:
 * @code
 * CharacterMethodResolver resolver;
 * llvm::Value* result = resolver.resolveMethod("toUpperCase", charInstance, {}, astContext);
 * @endcode
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/


#ifndef LYNX_CHARACTER_METHOD_RESOLVER_HPP
#define LYNX_CHARACTER_METHOD_RESOLVER_HPP

#include "resolver/TypeMethodResolver.hpp"

namespace LynxTypes {

    struct CharacterMethodResolver : public TypeMethodResolver {

        llvm::Value* resolveMethod(
            const AstContext& ctx,
            llvm::Value* instance,
            llvm::Value* instancePtr,
            const std::string& method, 
            const std::vector<llvm::Value*>& args
        )  noexcept override;
            

    };
}

#endif
