/**
 * @file ShortMethodResolver.hpp
 * @brief Concrete TypeMethodResolver for the `short` type.
 *
 * `ShortMethodResolver` implements the `resolveMethod` interface to handle
 * method calls on `short` instances in Lynx. It maps method names to their
 * corresponding LLVM IR implementations.
 *
 * Example usage:
 * @code
 * ShortMethodResolver resolver;
 * llvm::Value* result = resolver.resolveMethod("toString", shortInstance, {}, astContext);
 * @endcode
 *
 * Methods like arithmetic operations, comparisons, and type conversions
 * are resolved according to the semantics of the Lynx `short` type.
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/


#ifndef LYNX_SHORT_METHOD_RESOLVER_HPP
#define LYNX_SHORT_METHOD_RESOLVER_HPP

#include "resolver/TypeMethodResolver.hpp"

namespace LynxTypes {

    class ShortMethodResolver : public TypeMethodResolver {

        public:

            llvm::Value* resolveMethod(
                AstContext& ctx,
                llvm::Value* instance,
                const std::string& method, 
                const std::vector<llvm::Value*>& args
            ) noexcept override;

    };
}

#endif
