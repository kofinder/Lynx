/**
 * @file RuntimeBindingManager.hpp
 * @brief Declares the RuntimeBindingManager class for managing runtime external function bindings in Lynx.
 * 
 * The RuntimeBindingManager is responsible for declaring and registering all required runtime functions
 * (e.g., memory allocation, threading primitives) in LLVM IR modules. This ensures that JIT-compiled code
 * can correctly link and execute against the runtime environment.
 * 
 * **Key Responsibilities:**
 * - Declare external runtime functions in LLVM modules.
 * - Register declared runtime functions with the JIT or runtime system.
 * - Ensure all runtime symbols are available for LLVM code generation.
 * 
 * **Used By:**
 * - IR generation and JIT execution subsystems.
 * - Any LLVM modules requiring runtime function calls (e.g., GC_malloc, pthreads).
 * 
 * * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_CORE_RUNTIME_BINDING_MANAGER_HPP
#define LYNX_CORE_RUNTIME_BINDING_MANAGER_HPP

#include <llvm/IR/Module.h>

namespace LynxCore {
    /**
     * @class RuntimeBindingManager
     * @brief Manages declaration and registration of runtime external functions and bindings.
     * 
     * Responsible for declaring external functions (e.g., GC_malloc, pthread functions)
     * in LLVM IR modules and registering them with the JIT or runtime system.
     * 
     * Ensures all required runtime symbols are available to LLVM code generation and execution.
    */
    class RuntimeBindingManager {

        private:
        
            RuntimeBindingManager() = delete;
            RuntimeBindingManager(const RuntimeBindingManager&) = delete;
            RuntimeBindingManager& operator=(const RuntimeBindingManager&) = delete;


        public:

            /**
             * Declares all required runtime external functions in the provided LLVM module.
             * @param module LLVM module where declarations will be added.
            */
            static void declareAll(llvm::Module* module);

            /**
             * Registers all declared runtime functions with the JIT or runtime.
             * Typically used after declaration to bind symbols.
            */
            static void registerAll();
    };
        
}

#endif
