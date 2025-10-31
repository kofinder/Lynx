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
