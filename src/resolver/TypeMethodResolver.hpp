#ifndef LYNX_TYPE_METHOD_RESOLVER_HPP
#define LYNX_TYPE_METHOD_RESOLVER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <llvm/IR/Value.h>
#include <context/AstContext.hpp>
#include <logger/Logger.hpp>

using namespace LynxLogger;
using namespace LynxContext;

namespace LynxResolver {

    /**
     * @class TypeMethodResolver
     * @brief Abstract interface for resolving method calls on specific types at compile time.
     *
     * Subclasses of this interface handle type-specific method dispatch during
     * IR generation (e.g., `int.abs()`, `datetime.getYear()`, etc.).
     * 
     * Each type (int, float, DateTime, etc.) provides its own implementation.
    */
    class TypeMethodResolver {

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
                const std::string& name, 
                llvm::Value* instance,
                const std::vector<llvm::Value*>& args,
                std::shared_ptr<AstContext> astContext
            ) = 0;

            virtual ~TypeMethodResolver() = default;
    };

}

#endif
