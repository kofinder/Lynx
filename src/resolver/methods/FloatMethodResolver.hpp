/**
 * @file FloatMethodResolver.hpp
 * @brief Concrete TypeMethodResolver for the `float` type.
 *
 * `FloatMethodResolver` implements the `resolveMethod` interface to handle
 * method calls on `float` instances in Lynx. It maps method names to their
 * corresponding LLVM IR instructions or runtime operations.
 *
 * Example usage:
 * @code
 * FloatMethodResolver resolver;
 * llvm::Value* result = resolver.resolveMethod("sqrt", floatInstance, {}, astContext);
 * @endcode
 *
 * Methods supported include arithmetic operations, comparisons, and
 * type conversions specific to the Lynx `float` type.
 *
 * * @author Ko Thein (Nathan Mratt)
 * @date November 2, 2024
*/


#ifndef LYNX_FLOAT_METHOD_RESOLVER_HPP
#define LYNX_FLOAT_METHOD_RESOLVER_HPP

#include "resolver/TypeMethodResolver.hpp"

namespace LynxResolver {

    class FloatMethodResolver : public TypeMethodResolver {

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
