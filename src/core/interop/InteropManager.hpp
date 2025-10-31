#ifndef LYNX_CORE_INTEROP_MANAGER_HPP
#define LYNX_CORE_INTEROP_MANAGER_HPP

#include <string>
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

            InteropManager() = delete;
            InteropManager(const InteropManager&) = delete;
            InteropManager& operator=(const InteropManager&) = delete;

            /**
             * Register a native function pointer with a name.
             * @param name Identifier for the function.
             * @param ptr Pointer to the native function.
            */
           static std::unordered_map<std::string, void*>& getFunctionMap();

           static std::mutex& getMutex();

           /**
             * Register a native function pointer with a name.
             * @param name Identifier for the function.
             * @param ptr Pointer to the native function.
            */
           static void registerFunction(const std::string& name, void* ptr);

        public:

            /**
             * Initialize interop layer (load libraries, setup env).
            */
            static void initialize();

            /**
             * Get registered native function pointer by name.
             * Returns nullptr if not found.
            */
            static void* getFunction(const std::string& name);

            /**
             * Returns a const ref to the whole map of all registered functions.
            */
            static const std::unordered_map<std::string, void*>& getAll();

            /**
             * Shutdown interop layer (cleanup).
             */
            static void shutdown();
            
        };   
}

#endif
