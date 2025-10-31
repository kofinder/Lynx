#ifndef LYNX_LIB_RUNTIME_CLASS_REGISTRY_HPP
#define LYNX_LIB_RUNTIME_CLASS_REGISTRY_HPP

#include <string>
#include "RuntimeClass.hpp"

namespace LynxLibRuntime {
    
    class RuntimeClassRegistry {

        private:
        
            RuntimeClassRegistry() = default;
    
            std::unordered_map<std::string, std::shared_ptr<RuntimeClass>> classes;
    
        public:
    
            static RuntimeClassRegistry& getInstance() {
                static RuntimeClassRegistry instance;
                return instance;
            }
    
            void registerClass(const std::string& name, std::shared_ptr<RuntimeClass> clazz) {
                classes[name] = std::move(clazz);
            }
    
            std::shared_ptr<RuntimeClass> resolveClass(const std::string& name) {
                auto it = classes.find(name);
                return it != classes.end() ? it->second : nullptr;
            }
    };
    
}

#endif
