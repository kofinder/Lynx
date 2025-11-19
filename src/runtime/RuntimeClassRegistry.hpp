/**
 * @file RuntimeClassRegistry.hpp
 * @brief Centralized registry for Lynx runtime classes.
 * 
 * The `RuntimeClassRegistry` acts as a singleton container for runtime-level
 * class definitions within the Lynx Runtime Library. It allows dynamic registration,
 * lookup, and retrieval of runtime-exposed classes.
 * 
 * **Key Features:**
 * - Singleton-based access to all registered runtime classes.
 * - Enables modules to dynamically expose runtime class implementations.
 * - Provides safe lookups and registration via shared pointers.
 * 
 * **Usage Example:**
 * @code
 * auto& registry = RuntimeClassRegistry::getInstance();
 * registry.registerClass("File", std::make_shared<FileRuntimeClass>());
 * auto fileClass = registry.resolveClass("File");
 * @endcode
 * 
 * @see RuntimeClass, RuntimeFunctionRegistry, RuntimeModule
 * 
 * @namespace LynxRuntime
 * Provides runtime-level class and function registration and lookup mechanisms.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_RUNTIME_CLASS_REGISTRY_HPP
#define LYNX_RUNTIME_CLASS_REGISTRY_HPP

#include <string>
#include "RuntimeClass.hpp"

namespace LynxRuntime {
    
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
