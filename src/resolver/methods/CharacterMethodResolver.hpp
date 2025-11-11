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
 * * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/


#ifndef LYNX_CHARACTER_METHOD_RESOLVER_HPP
#define LYNX_CHARACTER_METHOD_RESOLVER_HPP

#include "resolver/TypeMethodResolver.hpp"

namespace LynxResolver {

    class CharacterMethodResolver : public TypeMethodResolver {

        public:

            llvm::Value* resolveMethod(
                const std::string& name, 
                llvm::Value* instance,
                const std::vector<llvm::Value*>& args,
                std::shared_ptr<AstContext> astContext
            ) noexcept override;

    };
}

#endif
