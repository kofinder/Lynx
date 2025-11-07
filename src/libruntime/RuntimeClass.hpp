/**
 * @file RuntimeClass.hpp
 * @brief Base abstraction for Lynx runtime-exposed classes.
 * 
 * The `RuntimeClass` represents a dynamically registered class within the Lynx
 * runtime environment. Runtime classes encapsulate methods that can be invoked
 * from within the language runtime, providing bindings to native functionality
 * such as file handling, collections, or networking.
 * 
 * **Key Features:**
 * - Serves as a base for all runtime-exposed classes.
 * - Allows registration of native methods into the runtime function registry.
 * - Supports lookup and invocation of registered methods at runtime.
 * 
 * **Usage Example:**
 * @code
 * class FileRuntimeClass : public RuntimeClass {
 * public:
 *     std::string getName() const override { return "File"; }
 *     
 *     void registerMethods(RuntimeFunctionRegistry& registry) override {
 *         registry.registerFunction("File::open", std::make_shared<FileOpenFunction>());
 *         registry.registerFunction("File::close", std::make_shared<FileCloseFunction>());
 *     }
 * };
 * @endcode
 * 
 * @see RuntimeFunctionRegistry, RuntimeFunction, RuntimeValue
 * 
 * @namespace LynxLibRuntime
 * Contains components related to the Lynx Runtime Library, including modules,
 * runtime functions, and dynamic class registration.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/


#ifndef LYNX_LIB_RUNTIME_CLASS_HPP
#define LYNX_LIB_RUNTIME_CLASS_HPP

#include <string>
#include <vector>
#include <memory>
#include "RuntimeValue.hpp"
#include <logger/Logger.hpp>
#include <llvm/IR/Value.h>
#include <ast/ExpressionNode.hpp>
#include <context/AstContext.hpp>
#include "RuntimeFunctionRegistry.hpp"
#include <constants/runtime/QualifiedFileFunctionType.hpp>

using namespace LynxLogger;
using namespace LynxAst;
using namespace LynxContext;
using namespace LynxTypes;

namespace LynxLibRuntime {

    using ArgumentExprs = std::vector<std::unique_ptr<ExpressionNode>>;

    class RuntimeClass {

        protected:

            std::unordered_map<std::string, std::shared_ptr<RuntimeFunction>> methods;

        public:

            /**
             * @brief Returns the name of the runtime class.
             * 
             * This is typically used for identification or registration purposes.
            */
            virtual std::string getName() const = 0;

            /**
             * @brief Registers this class's methods with the runtime function registry.
             * 
             * This allows the class's methods to be discovered and called at runtime.
             *
             * @param registry The runtime function registry for registering available class methods.
            */
            virtual void registerMethods(RuntimeFunctionRegistry& registry) = 0;

            /**
             * @brief Retrieves a method registered by this class (e.g., "read").
             *
             * Enables instance-bound method resolution (e.g., obj.getClass()->getMethod("read")).
             *
             * @param name The name of the method (not prefixed).
             * @return Shared pointer to the method function, or nullptr if not found.
             */
            std::shared_ptr<RuntimeFunction> getMethod(const std::string& name) {
                auto it = methods.find(name);
                return it != methods.end() ? it->second : nullptr;
            }

            /// Virtual destructor to allow proper cleanup in derived classes.
            virtual ~RuntimeClass() = default;

        };

}

#endif
