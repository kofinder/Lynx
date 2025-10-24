#ifndef LYNX_CORE_MEMORY_MANAGER_HPP
#define LYNX_CORE_MEMORY_MANAGER_HPP

#include <string>
#include <unordered_map>

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

            MemoryManager() = delete;
            MemoryManager(const MemoryManager&) = delete;
            MemoryManager& operator=(const MemoryManager&) = delete;

            /** 
             * Internal map storing allocated pointers and their labels.
             * Used to track live allocations for diagnostics.
            */
            static std::unordered_map<void*, std::string>& getAllocMap();

        public:

            /**
             * Allocates `size` bytes of heap memory labeled by `label` for diagnostics.
             * @param size Number of bytes to allocate.
             * @param label Optional string to identify the allocation in logs.
             * @return Pointer to allocated memory (never nullptr if GC_malloc succeeds).
            */
            static void* allocate(std::size_t size, const std::string& label = "unnamed");

            /**
             * Deallocates memory at `ptr` and removes tracking info.
             * Should be called when memory is freed to avoid false leak reports.
             * @param ptr Pointer to the allocated memory to deallocate.
            */
            static void deallocate(void* ptr);
            
            /**
             * Tracks an allocation manually by associating `label` with `ptr`.
             * Useful if allocation occurs outside `allocate()`.
             * @param label Human-readable identifier for the allocation.
             * @param ptr Pointer to the allocated memory.
            */
            static void track(const std::string& label, void* ptr);

            /**
             * Reports currently tracked allocations for potential leaks.
             * Intended for debugging and testing.
             * Outputs to standard error by default.
            */
            static void reportLeaks();
            
        };   
}

#endif
