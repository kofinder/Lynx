/**
 * @file StringMethodResolver.hpp
 * @brief Concrete TypeMethodResolver for `String` type.
 *
 * `StringMethodResolver` implements the `resolveMethod` interface to handle
 * method calls on `String` instances in Lynx. It maps method names to their
 * corresponding LLVM IR implementations.
 *
 * Example usage:
 * @code
 * StringMethodResolver resolver;
 * llvm::Value* result = resolver.resolveMethod("length", stringInstance, {}, astContext);
 * @endcode
 *
 * Methods like `length`, `substring`, `concat`, and others are expected to be
 * resolved according to the semantics of the Lynx `String` type.
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024

*/


#ifndef LYNX_STRING_METHOD_RESOLVER_HPP
#define LYNX_STRING_METHOD_RESOLVER_HPP

#include "resolver/TypeMethodResolver.hpp"

namespace LynxResolver {

    class StringMethodResolver : public TypeMethodResolver {

        public:

            llvm::Value* resolveMethod(
                const std::string& name, 
                llvm::Value* instance,
                const std::vector<llvm::Value*>& args,
                std::shared_ptr<AstContext> astContext
            ) override;

    };
}

#endif
