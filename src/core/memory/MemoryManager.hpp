/**
 * @file MemoryManager.hpp
 * @brief Declares the MemoryManager class for allocation tracking and memory management in the Lynx runtime.
 * 
 * The MemoryManager class wraps a garbage-collected allocator (e.g., Boehm GC) to provide centralized
 * memory allocation, tracking, and diagnostics across the core runtime. It supports optional labeling
 * of allocations for debugging and leak detection.
 * 
 * **Key Responsibilities:**
 * - Allocate heap memory for runtime objects with optional labels.
 * - Track live allocations to facilitate memory diagnostics.
 * - Deallocate memory and maintain allocation metadata.
 * - Provide utilities to report memory leaks or usage statistics.
 * 
 * **Used By:**
 * - Core runtime components requiring dynamic memory allocation.
 * - Debugging and testing subsystems for memory tracking.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_CORE_MEMORY_MANAGER_HPP
#define LYNX_CORE_MEMORY_MANAGER_HPP

#include <string>
#include <mutex>
#include <unordered_map>
#include "GCMutator.hpp"
#include "GCAllocator.hpp"
#include "GCCollector.hpp"
#include "GCDashboard.hpp"

namespace LynxCore {

    /**
     * @class MemoryManager
     * @brief Provides memory allocation and tracking for the core runtime.
     * 
     * This manager wraps the GC_malloc allocator (Boehm GC) to provide
     * allocation tracking and potential memory leak diagnostics.
     * 
     * Responsibilities:
     * - Allocate heap memory for runtime objects.
     * - Track allocations by label for debugging.
     * - Provide interfaces to report leaks or usage stats.
     * 
     * Designed for use across the core runtime system.
    */
    class MemoryManager {

        private:

            std::mutex mtx;

            bool initialized = false;

            std::unique_ptr<GCCollector> collector;

            std::unique_ptr<GCMutator> mutator;

            std::unique_ptr<GCDashboard> dashboard;
    
        public:

            MemoryManager() = default; 
            ~MemoryManager() { shutdown(); }
        
            /** Initialize all GC subsystems */
            void initialize(bool enableDashboard = true);

            /** Report leaks and usage statistics */
            void reportLeaks();

            /** Shutdown GC subsystems safely */
            void shutdown();

            /** Access to underlying mutator (for IR bindings) */
            GCMutator& getMutator() { return *mutator; }

            /** Access to collector for dashboard or logging */
            GCCollector& getCollector() { return *collector; }

    };   
}

#endif
