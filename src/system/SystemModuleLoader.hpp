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
