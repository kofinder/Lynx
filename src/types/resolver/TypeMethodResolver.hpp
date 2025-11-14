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
 * @date: November 2, 2024
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
    
    using namespace LynxLogger;
    using namespace LynxContext;
    
    class TypeMethodResolver {

        protected:

            std::unordered_map<std::string, size_t> methodSignatures;

        public:
        
            /**
             * @brief Resolve a method for a specific type instance.
             *
             * @param name The name of the method being invoked.
             * @param instance The LLVM IR value representing the instance on which the method is called.
             * @param args A vector of LLVM IR values representing arguments passed to the method.
             * @param astContext Shared pointer to the AST context for code generation utilities.
             * @return The resulting LLVM IR Value, or nullptr if the method is not found or not supported.
            */
            virtual llvm::Value* resolveMethod(
                AstContext& ctx,
                llvm::Value* instance,
                const std::string& method, 
                const std::vector<llvm::Value*>& args
            ) noexcept = 0;

            void registerMethodName(const std::string& name, size_t paramCount) {
                methodSignatures[name] = paramCount;
            }
        
            bool hasMethod(const std::string& name) const {
                return methodSignatures.find(name) != methodSignatures.end();
            }
        
            bool validateMethodCall(const std::string& name, size_t argCount) const {
                auto it = methodSignatures.find(name);
                if(it == methodSignatures.end()) return false;
                return it->second == argCount;
            }
        
            size_t getExpectedParamCount(const std::string& name) const {
                auto it = methodSignatures.find(name);
                return it == methodSignatures.end() ? 0 : it->second;
            }
        
            virtual ~TypeMethodResolver() noexcept = default;
    };

}

#endif
