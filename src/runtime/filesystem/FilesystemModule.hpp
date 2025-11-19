/**
 * @file FilesystemModule.hpp
 * @brief Provides runtime filesystem operations and utilities for Lynx.
 * 
 * The `FilesystemModule` exposes file and directory manipulation functionality
 * to the Lynx runtime environment. It provides access to standard filesystem
 * operations such as reading, writing, creating, and checking file existence,
 * along with basic path utilities.
 * 
 * **Responsibilities:**
 * - Registers runtime functions under the `fs::` namespace (e.g., `fs::open`, `fs::read`).
 * - Provides file I/O classes and related utilities for the Lynx runtime.
 * - Integrates with the `RuntimeFunctionRegistry` and `RuntimeClassRegistry` to make
 *   filesystem features accessible to Lynx code.
 * 
 * **Used By:**
 * - Runtime module loader and initialization system.
 * - Lynx standard library for I/O operations.
 * 
 * @see RuntimeModule, RuntimeFunctionRegistry, RuntimeClassRegistry
 * 
 * @namespace LynxRuntime
 * Contains runtime modules and tools that extend the Lynx language with system-level
 * and library-level capabilities such as filesystem, collections, and logging.
 * 
 * @note This module is automatically loaded when the Lynx runtime initializes filesystem support.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_RUNTIME_FILE_SYSTEM_MODULE_HPP
#define LYNX_RUNTIME_FILE_SYSTEM_MODULE_HPP

#include <string>
#include "runtime/RuntimeModule.hpp"

namespace LynxRuntime {

    class FilesystemModule : public RuntimeModule {

        public:

            /**
             * @brief Registers all filesystem-related functions into the runtime registry.
             * 
             * This typically includes functions from `fs::` namespace like:
             * - fs::open
             * - fs::write
             * - fs::read
             * - fs::exists
             * - path, directory and utilities
             *
             * @param registry A reference to the shared RuntimeFunctionRegistry.
             */
            void registerFunctions(RuntimeFunctionRegistry& registry) override;

            void registerClasses(RuntimeClassRegistry& registry) override;

            /**
             * @brief Returns the module name identifier.
             * 
             * This is used for lookup, import handling, and debugging.
             * 
             * @return The string literal "filesystem".
             */
            std::string name() const override { return "filesystem"; }

        };
        
    
}

#endif 
