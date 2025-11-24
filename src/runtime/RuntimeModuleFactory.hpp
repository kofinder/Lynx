/**
 * @file RuntimeModuleFactory.hpp
 * @brief Declares the RuntimeModuleFactory class responsible for creating Lynx runtime module instances.
 * 
 * The RuntimeModuleFactory provides a centralized mechanism for instantiating built-in 
 * runtime modules based on their enumerated type (`RuntimeModuleType`). Each supported 
 * module type corresponds to a specialized `RuntimeModule` subclass (e.g., `FilesystemModule`, 
 * `CollectionModule`).
 * 
 * **Key Responsibilities:**
 * - Encapsulate creation logic for all core runtime modules.
 * - Provide an extensible switch-based factory method.
 * - Return module instances as smart pointers for safe lifetime management.
 * 
 * **Design Notes:**
 * - Uses `std::unique_ptr` to ensure exclusive ownership of created modules.
 * - Modules that are unsupported or unimplemented return `nullptr`.
 * - Easily extendable by adding new cases to the `create()` function.
 * 
 * **Used By:**
 * - `RuntimeModuleLoader` for dynamic module initialization.
 * - Lynx runtime core systems requiring module registration.
 * 
 * **Example Usage:**
 * @code
 * auto module = RuntimeModuleFactory::create(RuntimeModuleType::FILESYSTEM);
 * if (module) module->registerFunctions();
 * @endcode
 * 
 * @see RuntimeModuleLoader, RuntimeModule, RuntimeModuleType
 * 
 * @namespace LynxRuntime
 * Provides runtime module management utilities for the Lynx execution environment.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_RUNTIME_MODULE_FACTORY_HPP
#define LYNX_RUNTIME_MODULE_FACTORY_HPP

#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "RuntimeModule.hpp"
#include <constants/runtime/RumtimeModuleType.hpp>
#include "filesystem/FilesystemModule.hpp"
#include "collections/CollectionModule.hpp"

using namespace LynxConstants;

namespace LynxRuntime {

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
