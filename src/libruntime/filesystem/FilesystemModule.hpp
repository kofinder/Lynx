#ifndef LYNX_LIB_RUNTIME_FILE_SYSTEM_MODULE_HPP
#define LYNX_LIB_RUNTIME_FILE_SYSTEM_MODULE_HPP

#include <string>
#include <RuntimeModule.hpp>

namespace LynxLibRuntime {

    /**
     * @class FilesystemModule
     * @brief Provides the standard runtime functions for the filesystem module.
     *
     * This module is responsible for registering runtime file I/O functions
     * (e.g., open, write, read, path manipulation) into the RuntimeFunctionRegistry.
     *
     * It extends the base RuntimeModule interface and implements the required methods.
     */
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
