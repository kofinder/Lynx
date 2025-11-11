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
 * * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_AUTO_METHOD_RESOLVER_HPP
#define LYNX_AUTO_METHOD_RESOLVER_HPP

#include "resolver/TypeMethodResolver.hpp"

namespace LynxResolver {

    class AutoMethodResolver : public TypeMethodResolver {

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
