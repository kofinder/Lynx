/**
 * @file BoolMethodResolver.hpp
 * @brief Concrete TypeMethodResolver for the `Boolean` type.
 *
 * `BoolMethodResolver` implements the `resolveMethod` interface to handle
 * method calls on `Boolean` instances in Lynx. It maps boolean-specific methods
 * to their corresponding LLVM IR instructions or runtime operations.
 *
 * Example usage:
 * @code
 * BoolMethodResolver resolver;
 * llvm::Value* result = resolver.resolveMethod("negate", boolInstance, {}, astContext);
 * @endcode
 *
 * * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
 */

#ifndef LYNX_BOOL_METHOD_RESOLVER_HPP
#define LYNX_BOOL_METHOD_RESOLVER_HPP

#include "TypeMethodResolver.hpp"

namespace LynxResolver {

    class BoolMethodResolver : public TypeMethodResolver {

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
