/**
 * @file RuntimeFunctionRegistry.hpp
 * @brief Defines the global registry for all Lynx runtime functions.
 * 
 * The `RuntimeFunctionRegistry` class provides a centralized system for
 * registering and retrieving runtime functions exposed by Lynx modules.
 * It follows the Singleton pattern to ensure all runtime modules share
 * a common global function namespace.
 * 
 * **Key Responsibilities:**
 * - Manage registration and lookup of runtime functions by name.
 * - Maintain ownership of registered `RuntimeFunction` objects.
 * - Provide thread-safe singleton access to the global registry.
 * 
 * **Usage Example:**
 * @code
 * auto& registry = RuntimeFunctionRegistry::getInstance();
 * registry.registerFunction("fs::open", std::make_shared<RuntimeFunction>(...));
 * auto func = registry.resolveFunction("fs::open");
 * @endcode
 * 
 * **Design Notes:**
 * - Functions are stored in an `unordered_map` for O(1) average lookup.
 * - The registry is non-copyable and non-movable to maintain global state consistency.
 * 
 * @see RuntimeFunction, RuntimeModule, RuntimeModuleFactory, RuntimeModuleLoader
 * 
 * @namespace LynxLibRuntime
 * Provides runtime-level dynamic linking and function management utilities for Lynx modules.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_LIB_RUNTIME_FUNCTIN_REGISTRY_HPP
#define LYNX_LIB_RUNTIME_FUNCTIN_REGISTRY_HPP

#include <string>
#include "RuntimeFunction.hpp"

namespace LynxLibRuntime {

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
