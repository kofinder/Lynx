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
 * * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_CORE_THREAD_MANAGER_HPP
#define LYNX_CORE_THREAD_MANAGER_HPP

#include <string>

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
            ThreadManager() = delete;
            ThreadManager(const ThreadManager&) = delete;
            ThreadManager& operator=(const ThreadManager&) = delete;

        public:

            /**
             * Creates a new thread running the given start routine.
             * @param thread Pointer to pthread_t variable to store the new thread ID.
             * @param start_routine Function pointer for the thread's start routine.
             * @param arg Argument passed to the start routine.
             * @throws std::runtime_error if pthread_create fails.
            */
            static void create(pthread_t* thread, void *(*start_routine)(void*), void* arg);

            /**
             * Joins all created threads that have not yet been joined.
             * Intended to be called at shutdown or synchronization points.
            */
            static void joinAll(); 

            /**
             * Joins a specific thread.
             * @param thread The thread to join.
             * @param retval Optional pointer to store the thread's return value.
             * @return 0 on success, or pthread error code.
            */
            static int join(pthread_t thread, void** retval);

            /**
             * Returns the identifier of the calling thread.
             * @return pthread_t representing the current thread.
            */
            static pthread_t self();
        
    };   
}

#endif
