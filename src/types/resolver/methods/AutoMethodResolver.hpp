/**
 * @file AutoMethodResolver.hpp
 * @brief Concrete TypeMethodResolver for the `Auto` type.
 *
 * `AutoMethodResolver` implements the `resolveMethod` interface to handle
 * method calls on `Auto`-typed instances in Lynx. It resolves methods
 * dynamically based on the inferred runtime type.
 *
 * Example usage:
 * @code
 * AutoMethodResolver resolver;
 * llvm::Value* result = resolver.resolveMethod("someMethod", autoInstance, {}, astContext);
 * @endcode
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_AUTO_METHOD_RESOLVER_HPP
#define LYNX_AUTO_METHOD_RESOLVER_HPP

#include "resolver/TypeMethodResolver.hpp"

namespace LynxTypes {

    struct AutoMethodResolver : public TypeMethodResolver {

        llvm::Value* resolveMethod(
            const AstContext& ctx,
            llvm::Value* instance,
            llvm::Value* instancePtr,
            const std::string& method, 
            const std::vector<llvm::Value*>& args       
        ) noexcept override;

    };
}

#endif
