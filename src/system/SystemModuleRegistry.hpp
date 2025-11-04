#ifndef LYNX_SYSTEM_MODULE_REGISTRY_HPP
#define LYNX_SYSTEM_MODULE_REGISTRY_HPP

#include <functional>
#include <string>
#include <memory>
#include "ISystemModule.hpp"

namespace LynxSystem {

    class SystemModuleRegistry {

        public:

            using ModuleFactory = std::function<std::unique_ptr<ISystemModule>()>;
        
            static void registerModule(const std::string& name, ModuleFactory factory) {
                getRegistry()[name] = std::move(factory);
            }

            static std::unique_ptr<ISystemModule> getModule(const std::string& name) {
                auto it = getRegistry().find(name);
                if (it != getRegistry().end()) {
                    return it->second();
                }
                return nullptr;        
            }

            static void clear() {
                getRegistry().clear();
            }        

        private:

            static std::unordered_map<std::string, ModuleFactory>& getRegistry() {
                static std::unordered_map<std::string, ModuleFactory> registry;
                return registry;
            }
    };
    
        
}

#endif
