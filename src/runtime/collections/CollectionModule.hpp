/**
 * @file CollectionModule.hpp
 * @brief Provides runtime support for collection-based operations in Lynx.
 * 
 * The `CollectionModule` exposes collection-related utilities (such as lists, maps, and sets)
 * to the Lynx runtime environment. It integrates collection types and functions that can be 
 * used within Lynx programs at runtime.
 * 
 * **Responsibilities:**
 * - Registers built-in collection-related classes (e.g., `List`, `Map`).
 * - Registers global collection utility functions (e.g., sorting, filtering, mapping).
 * - Acts as the bridge between the Lynx language runtime and C++ collection implementations.
 * 
 * @see RuntimeModule, RuntimeFunctionRegistry, RuntimeClassRegistry
 * 
 * @namespace LynxRuntime
 * Contains runtime modules and utilities that provide dynamic class/function registration
 * for Lynx's standard library extensions.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_RUNTIME_COLLECTION_MODULE_HPP
#define LYNX_RUNTIME_COLLECTION_MODULE_HPP


#include <string>
#include "runtime/RuntimeModule.hpp"


namespace LynxRuntime {

    class CollectionModule : public RuntimeModule {

        public:
        
            void registerFunctions(RuntimeFunctionRegistry& registry) override;

            void registerClasses(RuntimeClassRegistry& registry) override;

            std::string name() const override { return "collections"; }
    };
}

#endif
