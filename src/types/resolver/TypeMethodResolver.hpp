/**
 * @file TypeMethodResolver.hpp
 * @brief Abstract base class for resolving methods on specific types in Lynx.
 *
 * The `TypeMethodResolver` provides an interface for type-specific method resolution.
 * Subclasses implement logic to map method names and arguments to LLVM IR operations
 * appropriate for the type they handle.
 *
 * Example usage:
 * @code
 * std::unique_ptr<TypeMethodResolver> resolver = TypeResolverFactory::forType(DataType::INT);
 * llvm::Value* result = resolver->resolveMethod("toString", instanceValue, argValues, astContext);
 * @endcode
 *
 * Each concrete resolver (e.g., IntMethodResolver, StringMethodResolver) implements
 * the `resolveMethod` function according to the semantics of the type.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_TYPE_METHOD_RESOLVER_HPP
#define LYNX_TYPE_METHOD_RESOLVER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <llvm/IR/Value.h>
#include <logger/Logger.hpp>
#include <context/AstContext.hpp>

namespace LynxTypes {

    using LynxContext::AstContext;
    
    struct TypeMethodResolver {

        public:

            virtual llvm::Value* resolveMethod(
                const AstContext& ctx,
                llvm::Value* instance,
                llvm::Value* instancePtr,
                const std::string& method, 
                const std::vector<llvm::Value*>& args
            ) noexcept = 0;

            virtual ~TypeMethodResolver() noexcept = default;

            // Rule of Five compliance
            TypeMethodResolver(const TypeMethodResolver&) = delete;
            TypeMethodResolver& operator=(const TypeMethodResolver&) = delete;
            TypeMethodResolver(TypeMethodResolver&&) = delete;
            TypeMethodResolver& operator=(TypeMethodResolver&&) = delete;

        protected:

            TypeMethodResolver() = default;  // allow construction by derived classes
    };

}

#endif
