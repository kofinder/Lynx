#ifndef LYNX_LIB_RUNTIME_MODULE_LOADER_HPP
#define LYNX_LIB_RUNTIME_MODULE_LOADER_HPP

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <logger/Logger.hpp>
#include "RuntimeModuleFactory.hpp"
#include <constants/runtime/RumtimeModuleType.hpp>

using namespace LynxLogger;
using namespace LynxConstants;


namespace LynxLibRuntime {

    /**
     * @class RuntimeModuleLoader
     * @brief Dynamically loads runtime modules and registers their functions once.
     *
     * The RuntimeModuleLoader ensures that each module (e.g., filesystem, datetime)
     * is only loaded once per program execution. When a module is loaded, it registers
     * its exposed runtime functions with the global RuntimeFunctionRegistry.
     *
     * This enables import-style semantics in the language and maintains
     * a consistent function resolution environment.
     */

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
