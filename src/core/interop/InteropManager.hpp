/**
 * @file InteropManager.hpp
 * @brief Declares the InteropManager class for managing FFI and data interoperability in the Lynx runtime.
 * 
 * The InteropManager provides a centralized system for registering and invoking native or external functions,
 * handling type conversions, and enabling seamless data exchange between Lynx runtime objects and native code.
 * 
 * **Key Responsibilities:**
 * - Register and manage external/native function pointers.
 * - Provide utility methods to invoke registered functions.
 * - Handle data marshalling and type conversion for interoperability.
 * - Manage initialization and shutdown of the interop layer.
 * 
 * **Used By:**
 * - Runtime components requiring FFI.
 * - External library integrations for extending Lynx programs.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_CORE_INTEROP_MANAGER_HPP
#define LYNX_CORE_INTEROP_MANAGER_HPP

#include <string>
#include <mutex>
#include <unordered_map>

namespace LynxCore {
    
    /**
     * @class InteropManager
     * @brief Manages foreign function interfaces (FFI) and data interoperability.
     * 
     * Responsibilities:
     * - Register external function pointers.
     * - Provide invocation utilities for external/native functions.
     * - Data marshalling and type conversion between Lynx runtime and native code.
    */

    class InteropManager {

        private:

            std::unordered_map<std::string, void*> funcMap;

            std::mutex mtx;

        private:

            std::unordered_map<std::string, void*>& getFunctionMap();

            std::mutex& getMutex();
        
        public:

            InteropManager() = default;
            ~InteropManager() { shutdown(); }
        
            void initialize();

            void registerFunction(const std::string& name, void* ptr);

            void* getFunction(const std::string& name);

            const std::unordered_map<std::string, void*>& getAll();

            void shutdown();
    };
        
}

#endif
