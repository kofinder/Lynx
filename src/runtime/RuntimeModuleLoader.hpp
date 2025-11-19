/**
 * @file RuntimeModuleLoader.hpp
 * @brief Declares the RuntimeModuleLoader class responsible for dynamic runtime module loading in Lynx.
 * 
 * The RuntimeModuleLoader class manages the initialization and registration of Lynx runtime modules 
 * (e.g., filesystem, networking, time) during program execution. It ensures each runtime module 
 * is only loaded once and delegates actual instantiation and registration to the 
 * `RuntimeModuleFactory`.
 * 
 * **Key Responsibilities:**
 * - Load and register runtime modules dynamically by type.
 * - Prevent redundant loading through module-type tracking.
 * - Integrate module creation and registration with `RuntimeModuleFactory`.
 * 
 * **Design Notes:**
 * - Uses a static `std::unordered_set` to track loaded modules.
 * - Provides a static interface for easy access across runtime systems.
 * - Supports extension via new `RuntimeModuleType` entries.
 * 
 * **Used By:**
 * - Lynx runtime environment during module initialization.
 * - Runtime systems that depend on specific library modules (e.g., file I/O, math utilities).
 * 
 * **Example Usage:**
 * @code
 * using namespace LynxRuntime;
 * RuntimeModuleLoader::loadModule(RuntimeModuleType::FILESYSTEM);
 * @endcode
 * 
 * @see RuntimeModuleFactory, RuntimeModuleType, Logger
 * 
 * @namespace LynxRuntime
 * Provides runtime utilities for module management and dynamic linking of built-in Lynx modules.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_RUNTIME_MODULE_LOADER_HPP
#define LYNX_RUNTIME_MODULE_LOADER_HPP

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <logger/Logger.hpp>
#include "RuntimeModuleFactory.hpp"
#include <constants/runtime/RumtimeModuleType.hpp>



namespace LynxRuntime {

    using namespace LynxLogger;
    using namespace LynxConstants;

    class RuntimeModuleLoader {

        private:

            /// Set of already loaded module types to prevent redundant initialization
            static std::unordered_set<RuntimeModuleType> loadedModules;

        public:
            /**
             * @brief Loads a module by type and registers its functions.
             *
             * If the module has already been loaded, this call is a no-op.
             * Otherwise, it instantiates the module via the factory,
             * registers its functions, and marks it as loaded.
             *
             * @param moduleType The enum type of the module (e.g., FILESYSTEM)
             * @return true if the module was successfully loaded or already loaded;
             *         false if the module type is unsupported.
            */
            static bool loadModule(RuntimeModuleType moduleType);
    };
    
}

#endif 
