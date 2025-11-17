/**
 * @file LongMethodResolver.hpp
 * @brief Concrete TypeMethodResolver for the `long` type.
 *
 * `LongMethodResolver` implements the `resolveMethod` interface to handle
 * method calls on `long` instances in Lynx. It maps method names to their
 * corresponding LLVM IR instructions or runtime operations.
 *
 * Example usage:
 * @code
 * LongMethodResolver resolver;
 * llvm::Value* result = resolver.resolveMethod("toString", longInstance, {}, astContext);
 * @endcode
 *
 * Methods supported include arithmetic operations, comparisons, and
 * type conversions specific to the Lynx `long` type.
*/


#ifndef LYNX_LONG_METHOD_RESOLVER_HPP
#define LYNX_LONG_METHOD_RESOLVER_HPP

#include "resolver/TypeMethodResolver.hpp"

namespace LynxTypes {

    struct LongMethodResolver : public TypeMethodResolver {

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
