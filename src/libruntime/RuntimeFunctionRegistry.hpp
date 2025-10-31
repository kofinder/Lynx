#ifndef LYNX_LIB_RUNTIME_FUNCTIN_REGISTRY_HPP
#define LYNX_LIB_RUNTIME_FUNCTIN_REGISTRY_HPP

#include <string>
#include "RuntimeFunction.hpp"

namespace LynxLibRuntime {

    /**
     * @class RuntimeFunctionRegistry
     * @brief Singleton registry for storing and resolving runtime functions.
     *
     * This class allows dynamic registration and lookup of runtime functions by name.
     * It is used to enable runtime-level dispatching for standard library modules,
     * such as filesystem, collections, etc.
     *
     * Implements the Singleton and Registry design patterns.
     */
    class RuntimeFunctionRegistry {

        private:

            // Private constructor to enforce Singleton usage.
            RuntimeFunctionRegistry() = default;

            // Registered runtime functions, mapped by name
            std::unordered_map<std::string, std::shared_ptr<RuntimeFunction>> functions;

        public:

            /**
             * @brief Retrieves the singleton instance of the registry.
             * @return A reference to the global RuntimeFunctionRegistry.
            */
            static RuntimeFunctionRegistry& getInstance() {
                static RuntimeFunctionRegistry instance;
                return instance;
            }

            /**
             * @brief Registers a runtime function under a given name.
             * @param name The string key for the function (e.g., "fs::open").
             * @param func A shared_ptr to the RuntimeFunction implementation.
            */
            void registerFunction(const std::string& name, std::shared_ptr<RuntimeFunction> func) {
                functions[name] = std::move(func);
            }

            /**
             * @brief Resolves a function by name.
             * @param name The string name of the function.
             * @return A shared_ptr to the RuntimeFunction if found, nullptr otherwise.
            */
            std::shared_ptr<RuntimeFunction> resolveFunction(const std::string& name) {
                auto it = functions.find(name);
                return it != functions.end() ? it->second : nullptr;
            }
    };        
    
}

#endif 
