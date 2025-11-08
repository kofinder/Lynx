/**
 * @file ThreadManager.hpp
 * @brief Declares the ThreadManager class for centralized thread handling in the Lynx runtime.
 * 
 * The ThreadManager class provides static methods to create, join, and query threads.
 * It centralizes runtime thread management, ensuring safe creation, joining, and retrieval
 * of thread identifiers. Designed for simplicity and consistency across the Lynx core.
 * 
 * **Key Responsibilities:**
 * - Create threads with a specified start routine and argument.
 * - Join all threads or a specific thread as needed.
 * - Retrieve the current thread identifier.
 * 
 * **Used By:**
 * - Lynx runtime systems requiring multi-threaded execution.
 * - Task scheduling and parallel execution modules.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_CORE_THREAD_MANAGER_HPP
#define LYNX_CORE_THREAD_MANAGER_HPP

#include <string>
#include <thread>
#include <vector>

namespace LynxCore {

    /**
     * @class ThreadManager
     * @brief Manages thread creation, joining, and querying for the runtime.
     * 
     * Provides a simple interface for creating and joining threads
     * and obtaining the current thread identifier.
     * 
     * Designed to centralize thread management in the core runtime.
    */
   class ThreadManager {

        private:

            std::vector<pthread_t> threads;
    
        public:

            ThreadManager() = default; 
            ~ThreadManager() { shutdown(); }
        
            void initialize(size_t threadCount = std::thread::hardware_concurrency());

            void create(pthread_t* thread, void* (*start_routine)(void*), void* arg);

            void joinAll();

            int join(pthread_t thread, void** retval);

            pthread_t self();

            void shutdown();
    };
    
}

#endif
