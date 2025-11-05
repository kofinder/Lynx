/**
 * @file SystemModuleLoader.hpp
 * @brief Declares the SystemModuleLoader class for managing the lifecycle of system modules.
 * 
 * The SystemModuleLoader class provides static methods to handle the initialization,
 * validation, and shutdown of system modules within the Lynx system. It coordinates
 * the loading of both core and custom modules, ensuring that all required modules
 * are properly registered and initialized before use.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_SYSTEM_MODULE_LOADER_HPP
#define LYNX_SYSTEM_MODULE_LOADER_HPP

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "ISystemModule.hpp"

namespace LynxSystem {

    class SystemModuleLoader {

        public:

            static void initializeCoreModules();

            static void loadCustomModule(const std::string& name, std::function<std::unique_ptr<ISystemModule>()> factory);

            static void validateModules(const std::vector<std::string>& requiredModules);

            static void shutdown();
        
        private:

            static bool initialized;
    };
    
}
    

#endif
