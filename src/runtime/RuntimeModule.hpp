/**
 * @file RuntimeModule.hpp
 * @brief Defines the abstract base class for all Lynx runtime modules.
 * 
 * The `RuntimeModule` class serves as the foundational interface for all 
 * dynamically loadable Lynx runtime modules (e.g., Filesystem, Collections, Datetime). 
 * Each derived module must implement class and function registration routines 
 * used to extend the Lynx runtime environment with new functionality.
 * 
 * **Key Responsibilities:**
 * - Provide virtual methods for class and function registration.
 * - Define a uniform interface for module identification via `name()`.
 * - Enable polymorphic behavior for all runtime modules.
 * 
 * **Usage Notes:**
 * - Concrete implementations should override `registerClasses()` and `registerFunctions()`
 *   to populate runtime registries with available classes and functions.
 * - The `name()` method must return a consistent module identifier string.
 * - Intended to be instantiated via the `RuntimeModuleFactory`.
 * 
 * **Example Implementation:**
 * @code
 * class FilesystemModule : public RuntimeModule {
 * public:
 *     void registerClasses(RuntimeClassRegistry& registry) override { ... }
 *     void registerFunctions(RuntimeFunctionRegistry& registry) override { ... }
 *     std::string name() const override { return "filesystem"; }
 * };
 * @endcode
 * 
 * @see RuntimeModuleFactory, RuntimeModuleLoader, RuntimeClassRegistry, RuntimeFunctionRegistry
 * 
 * @namespace LynxRuntime
 * Provides runtime module management and dynamic extension utilities for the Lynx execution environment.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_RUNTIME_MODULE_HPP
#define LYNX_RUNTIME_MODULE_HPP

#include <string>
#include <vector>
#include "RuntimeClassRegistry.hpp"
#include "RuntimeFunctionRegistry.hpp"

namespace LynxRuntime {

    class RuntimeModule {

        public:

            /**
             * @brief Registers this module’s class into the central function registry.
             * @param registry A reference to the RuntimeFunctionRegistry where functions are stored.
             */
            virtual void registerClasses(RuntimeClassRegistry& registry) = 0;
        
            /**
             * @brief Registers this module’s functions into the central function registry.
             * @param registry A reference to the RuntimeFunctionRegistry where functions are stored.
             */
            virtual void registerFunctions(RuntimeFunctionRegistry& registry) = 0;

            /**
             * @brief Returns the name of the module (e.g., "filesystem", "datetime").
             * @return The string identifier of the module.
            */
            virtual std::string name() const = 0;

            /**
             * @brief Virtual destructor to allow safe polymorphic cleanup.
            */
            virtual ~RuntimeModule() = default;
    };
}

#endif 
