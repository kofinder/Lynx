/**
 * @file DoubleMethodResolver.hpp
 * @brief Concrete TypeMethodResolver for the `double` type.
 *
 * `DoubleMethodResolver` implements the `resolveMethod` interface to handle
 * method calls on `double` instances in Lynx. It maps method names to their
 * corresponding LLVM IR instructions or runtime operations.
 *
 * Example usage:
 * @code
 * DoubleMethodResolver resolver;
 * llvm::Value* result = resolver.resolveMethod("sqrt", doubleInstance, {}, astContext);
 * @endcode
 *
 * Methods supported include arithmetic operations, comparisons, and
 * type conversions specific to the Lynx `double` type.
 * 
 * * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/


#ifndef LYNX_DOUBLE_METHOD_RESOLVER_HPP
#define LYNX_DOUBLE_METHOD_RESOLVER_HPP

#include "resolver/TypeMethodResolver.hpp"

namespace LynxResolver {

    class DoubleMethodResolver : public TypeMethodResolver {

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
