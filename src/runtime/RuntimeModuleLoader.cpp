#include "RuntimeModuleLoader.hpp"

namespace LynxRuntime {

    // Definition of the static member
    std::unordered_set<RuntimeModuleType> RuntimeModuleLoader::loadedModules;


    bool RuntimeModuleLoader::loadModule(RuntimeModuleType moduleType) {
        // Skip if already loaded
        if (loadedModules.contains(moduleType)) {
            return true;
        }
        
        // Create the module instance
        auto module = RuntimeModuleFactory::create(moduleType);
        if (!module) {
            LOG_ERROR("Unsupported module type.");
            return false;
        }

        // Register its functions to the central registry
        auto& fnRegistry = RuntimeFunctionRegistry::getInstance();
        module->registerFunctions(fnRegistry);


        auto& clazzRegistry = RuntimeClassRegistry::getInstance();
        module->registerClasses(clazzRegistry);
        
        // Mark as loaded
        loadedModules.insert(moduleType);

        return true;
    }

}