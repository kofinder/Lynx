#include "ThreadManager.hpp"
#include <pthread.h>
#include <stdexcept>
#include <vector>
#include <mutex>
#include <iostream>
#include <algorithm>


namespace LynxCore {

    static std::vector<pthread_t> activeThreads;
    static std::mutex threadsMutex;


    void ThreadManager::create(pthread_t* thread, void *(*start_routine)(void*), void* arg) {
        if (!thread || !start_routine) {
            throw std::invalid_argument("Null pointer passed to ThreadManager::create");
        }
    
        int result = pthread_create(thread, nullptr, start_routine, arg);
        if (result != 0) {
            throw std::runtime_error("Failed to create thread: error " + std::to_string(result));
        }

        {
            std::lock_guard<std::mutex> lock(threadsMutex);
            activeThreads.push_back(*thread);
        }
    }
    
    int ThreadManager::join(pthread_t thread, void** retval) {
        int result = pthread_join(thread, retval);
    
        if (result == 0) {
            std::lock_guard<std::mutex> lock(threadsMutex);
            // Remove joined thread from activeThreads vector
            auto it = std::find(activeThreads.begin(), activeThreads.end(), thread);
            if (it != activeThreads.end()) {
                activeThreads.erase(it);
            }
        } else {
            std::cerr << "[ThreadManager] Warning: Failed to join thread with error code " << result << "\n";
        }
    
        return result;
    }
    
    void ThreadManager::joinAll() {

        std::vector<pthread_t> threadsCopy;
        
        {
            std::lock_guard<std::mutex> lock(threadsMutex);
            threadsCopy = activeThreads;
        }
    
        for (const auto& thread : threadsCopy) {
            void* retval = nullptr;
            int result = join(thread, &retval);
            if (result != 0) {
                std::cerr << "[ThreadManager] Warning: Failed to join thread during joinAll with error code " << result << "\n";
            }
        }
    }
    
    pthread_t ThreadManager::self() {
        return pthread_self();
    }
}