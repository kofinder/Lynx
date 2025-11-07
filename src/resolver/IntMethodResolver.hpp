/**
 * @file IntMethodResolver.hpp
 * @brief Concrete TypeMethodResolver for the `int` type.
 *
 * `IntMethodResolver` implements the `resolveMethod` interface to handle
 * method calls on `int` instances in Lynx. It maps method names to their
 * corresponding LLVM IR instructions or runtime operations.
 *
 * Example usage:
 * @code
 * IntMethodResolver resolver;
 * llvm::Value* result = resolver.resolveMethod("abs", intInstance, {}, astContext);
 * @endcode
 *
 * Methods supported include arithmetic operations, comparisons, and
 * type conversions specific to the Lynx `int` type.
 *
 * * @author Ko Thein (Nathan Mratt)
 * @date November 2, 2024
 */


#ifndef LYNX_INT_METHOD_RESOLVER_HPP
#define LYNX_INT_METHOD_RESOLVER_HPP

#include "TypeMethodResolver.hpp"

namespace LynxResolver {

    class IntMethodResolver : public TypeMethodResolver {

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
