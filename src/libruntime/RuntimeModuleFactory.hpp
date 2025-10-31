#ifndef LYNX_LIB_RUNTIME_MODULE_FACTORY_HPP
#define LYNX_LIB_RUNTIME_MODULE_FACTORY_HPP

#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "RuntimeModule.hpp"
#include <constants/runtime/RumtimeModuleType.hpp>
#include "filesystem/FilesystemModule.hpp"
#include "collections/CollectionModule.hpp"

using namespace LynxConstants;

namespace LynxLibRuntime {

    /**
     * @class RuntimeModuleFactory
     * @brief Factory for creating instances of runtime modules based on their type.
     *
     * This factory encapsulates the logic of instantiating different types of
     * runtime modules. It decouples creation logic from module consumers,
     * following the Factory design pattern.
     */
    class RuntimeModuleFactory {

        public:

            /**
             * @brief Creates a runtime module instance from the given enum type.
             *
             * @param type The module type (e.g., FILESYSTEM, DATETIME)
             * @return A unique_ptr to the corresponding RuntimeModule,
             *         or nullptr if the type is unsupported or not yet implemented.
             */
            static std::unique_ptr<RuntimeModule> create(RuntimeModuleType type) {
                switch (type) {
                    case RuntimeModuleType::FILESYSTEM: return std::make_unique<FilesystemModule>();
                    case RuntimeModuleType::COLLECTIONS: return std::make_unique<CollectionModule>();
                    case RuntimeModuleType::DATETIME:
                    case RuntimeModuleType::LOGGER:
                    default: return nullptr;
                }
            }
    };
    
}

#endif 
