#include <iostream>
#include "CoreManager.hpp"

namespace LynxCore {
    
    CoreManager::CoreManager() 
    : memoryManager(std::make_unique<MemoryManager>()),
      threadManager(std::make_unique<ThreadManager>()),
      schedulerManager(std::make_unique<SchedulerManager>()),
      interopManager(std::make_unique<InteropManager>()),
      bindingManager(std::make_unique<RuntimeBindingManager>()) {}

    CoreManager::~CoreManager() {
        shutdown();
    }
    
    // ------------------------------
    // Initialize All Core Subsystems
    // ------------------------------
    void CoreManager::initialize() {
        if (initialized) return;

        // 1. Initialize Memory Manager (GC subsystem)
        //    - sets up Boehm GC
        //    - initializes GCMutator and GCCollector
        //    - optionally starts live GC dashboard
        // memoryManager->initialize();

        if (bindingManager) {
            RuntimeBindingManager::setMemoryManager(memoryManager.get());
        }        

        // 2. Initialize Thread Manager
        //    - creates thread pool
        //    - threads can now safely use GC for allocations
        threadManager->initialize();

        // 3. Initialize Scheduler Manager
        //    - sets up task scheduler for async/runtime tasks
        schedulerManager->initialize();

        // 4. Initialize Interop Manager
        //    - prepares runtime bridges, system calls, or foreign function interfaces
        interopManager->initialize();

        // 5. Initialize Runtime Binding Manager
        //    - registers all runtime symbols, classes, and GC bindings
        bindingManager->registerAll();

        initialized = true;
    }

    void CoreManager::shutdown() {
        if (!initialized) return;

        if (schedulerManager) schedulerManager->shutdown();
    
        if (threadManager) threadManager->shutdown();
    
        if (interopManager) interopManager->shutdown();
    
        if (memoryManager) memoryManager->shutdown();
        
        initialized = false;

    }

}