/**
 * @file ByteMethodResolver.hpp
 * @brief Concrete TypeMethodResolver for the `Byte` type.
 *
 * `ByteMethodResolver` implements the `resolveMethod` interface to handle
 * method calls on `Byte` instances in Lynx. It maps byte-specific methods
 * to their corresponding LLVM IR instructions or runtime operations.
 *
 * Example usage:
 * @code
 * ByteMethodResolver resolver;
 * llvm::Value* result = resolver.resolveMethod("toUnsigned", byteInstance, {}, astContext);
 * @endcode
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_BYTE_METHOD_RESOLVER_HPP
#define LYNX_BYTE_METHOD_RESOLVER_HPP

#include "resolver/TypeMethodResolver.hpp"

namespace LynxTypes {

    struct ByteMethodResolver : public TypeMethodResolver {

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
