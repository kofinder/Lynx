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

    // A shorthand type for a list of expression arguments passed to instance creation
    using ArgumentExprs = std::vector<std::unique_ptr<ExpressionNode>>;

    /**
     * @brief Abstract base class for all runtime types.
     *
     * This interface defines how user-defined types (e.g., File, Image, Resource)
     * are constructed, initialized, and registered in the runtime system.
    */
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
