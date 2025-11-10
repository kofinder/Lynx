/**
 * @file CoreManager.hpp
 * @brief Central coordinator for Lynx runtime subsystems.
 *
 * The CoreManager is responsible for initializing, configuring, and
 * shutting down all major Lynx runtime components — such as memory
 * management, threading, scheduling, interop bindings, and runtime
 * symbol registration (LLVM interop).
 *
 * This class ensures modular initialization and proper teardown
 * for all runtime systems used during JIT or native execution.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_CORE_MANAGER_HPP
#define LYNX_CORE_MANAGER_HPP

#include <llvm/IR/Module.h>
#include "interop/InteropManager.hpp"
#include "memory/MemoryManager.hpp"
#include "threads/ThreadManager.hpp"
#include "scheduler/SchedulerManager.hpp"
#include "bindings/RuntimeBindingManager.hpp"

namespace LynxCore {

    class CoreManager {

        private:

            std::unique_ptr<InteropManager> interopManager;

            std::unique_ptr<SchedulerManager> schedulerManager;

            std::unique_ptr<MemoryManager> memoryManager;

            std::unique_ptr<ThreadManager> threadManager;

            std::unique_ptr<RuntimeBindingManager> bindingManager;

            bool initialized = false;

        public:

            CoreManager();
            ~CoreManager();
        /**
         * @brief Initializes all LynxCore runtime modules.
         *
         * This function sets up:
         * - Garbage collector and memory tracking
         * - Thread subsystem
         * - Task scheduler
         * - Interoperability and runtime bindings
         *
         * Should be called once per runtime session (e.g., before IR generation or JIT).
        */
        void initialize();

        /**
         * @brief Shuts down all runtime modules gracefully.
         *
         * Ensures threads, schedulers, and memory subsystems are terminated safely.
         */
        void shutdown();

        // Accessors
        MemoryManager& getMemoryManager() { return *memoryManager; }
        ThreadManager& getThreadManager() { return *threadManager; }
        SchedulerManager& getSchedulerManager() { return *schedulerManager; }
        InteropManager& getInteropManager() { return *interopManager; }
        RuntimeBindingManager& getRuntimeBindingManager() { return *bindingManager; }

    };
        
}

#endif
