#include <gc/gc.h>
#include <iostream>
#include <mutex>

#include "MemoryManager.hpp"

namespace LynxCore {

    static std::mutex allocMapMutex;

    static std::unordered_map<void*, std::string>& getAllocMapInstance() {
        static std::unordered_map<void*, std::string> allocationMap;
        return allocationMap;
    }

    std::unordered_map<void*, std::string>& MemoryManager::getAllocMap() {
        return getAllocMapInstance();
    }

    void* MemoryManager::allocate(std::size_t size, const std::string& label) {

        void* ptr = GC_malloc(size);

        if (!ptr) {
            std::cerr << "[MemoryManager] Allocation failed for size: " << size << '\n';
            throw std::bad_alloc();
        }

        {
            std::lock_guard<std::mutex> lock(allocMapMutex);
            getAllocMap()[ptr] = label;
        }

        return ptr;
    }

    void MemoryManager::deallocate(void* ptr) {
        if (!ptr) return;

        std::lock_guard<std::mutex> lock(allocMapMutex);
        auto& map = getAllocMap();
        auto it = map.find(ptr);
        if (it != map.end()) {
            map.erase(it);
        } else {
            std::cerr << "[MemoryManager] Warning: Attempted to deallocate untracked pointer " << ptr << '\n';
        }

        // Note: Actual memory freeing handled by GC, so no explicit free here.
    }

    void MemoryManager::track(const std::string& label, void* ptr) {
        if (!ptr) return;
        std::lock_guard<std::mutex> lock(allocMapMutex);
        getAllocMap()[ptr] = label;
    }

    void MemoryManager::reportLeaks() {
        std::lock_guard<std::mutex> lock(allocMapMutex);

        auto& map = getAllocMap();
        if (map.empty()) {
            std::cerr << "[MemoryManager] No memory leaks detected.\n";
            return;
        }

        std::cerr << "[MemoryManager] Detected potential memory leaks:\n";
        for (const auto& [ptr, label] : map) {
            std::cerr << " - Leak: '" << label << "' at address " << ptr << '\n';
        }
    }
}
