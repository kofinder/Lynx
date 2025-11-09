#include <gc/gc.h>
#include <iostream>
#include <mutex>
#include "GCLogger.hpp"
#include "MemoryManager.hpp"
#include "GCIntegration.hpp"

namespace LynxCore {


    void MemoryManager::initialize(bool enableDashboard) {
        if (initialized) return;
        std::cout << "[MemoryManager] Initializing LynxGC subsystems...\n";
    
        // Initialize GCIntegration (Boehm GC)
        GCIntegration::initialize(enableDashboard);
    
        // Create collector and mutator
        collector = std::make_unique<GCCollector>();
        mutator   = std::make_unique<GCMutator>();
    
        if (enableDashboard) {
            dashboard = std::make_unique<GCDashboard>(*collector);
            dashboard->start();
        }
    
        initialized = true;
        std::cout << "[MemoryManager] GC initialization complete.\n";
    }
   
    void MemoryManager::reportLeaks() {
        if (!initialized) return;
        std::lock_guard<std::mutex> lock(mtx);

        if (!collector) {
            GCLogger::warn("[MemoryManager] Cannot report leaks: Collector not initialized.");
            return;
        }

        // // Trigger GC manually before snapshot
        // GCIntegration::triggerMinorGC();
        // GCIntegration::triggerMajorGC();

        // For now, delegate to dashboard snapshot for a quick overview
        if (dashboard) {
            GCLogger::info("[MemoryManager] Reporting memory usage via dashboard snapshot:");
            dashboard->printSnapshot();
        } else {
            GCLogger::warn("[MemoryManager] Dashboard not enabled; cannot visualize heap.");
        }
    }
    
    void MemoryManager::shutdown() {
        if (!initialized) return;
    
        if (dashboard) dashboard->stop();
    
        mutator.reset();
        collector.reset();
        dashboard.reset();
    
        GCIntegration::shutdown();
        initialized = false;
    }
}
