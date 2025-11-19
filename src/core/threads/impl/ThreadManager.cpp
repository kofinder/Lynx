#include "ThreadManager.hpp"
#include <pthread.h>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <functional>
#include <condition_variable>
#include <stdexcept>
#include <iostream>


namespace LynxCore {

    namespace {
        std::vector<std::thread> workerThreads;
        std::queue<std::function<void()>> taskQueue;
        std::mutex queueMutex;
        std::condition_variable cv;
        bool running = false;
    }

    void ThreadManager::initialize(size_t threadCount) {
        if (running) {
            std::cerr << "[ThreadManager] Already initialized.\n";
            return;
        }

        running = true;
        std::cout << "[ThreadManager] Initializing with " << threadCount << " threads.\n";

        for (size_t i = 0; i < threadCount; ++i) {
            workerThreads.emplace_back([]() {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        cv.wait(lock, [] { return !taskQueue.empty() || !running; });

                        if (!running && taskQueue.empty())
                            return;

                        task = std::move(taskQueue.front());
                        taskQueue.pop();
                    }
                    try {
                        task();
                    } catch (const std::exception& e) {
                        std::cerr << "[ThreadManager] Task exception: " << e.what() << "\n";
                    }
                }
            });
        }
    }

    void ThreadManager::create(pthread_t* thread, void *(*start_routine)(void*), void* arg) {
        int result = pthread_create(thread, nullptr, start_routine, arg);
        if (result != 0) {
            throw std::runtime_error("[ThreadManager] pthread_create failed with code: " + std::to_string(result));
        }
    }

    int ThreadManager::join(pthread_t thread, void** retval) {
        return pthread_join(thread, retval);
    }

    void ThreadManager::joinAll() {
        // std::cout << "[ThreadManager] Joining all worker threads...\n";
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            running = false;
        }
        cv.notify_all();

        for (auto& t : workerThreads) {
            if (t.joinable()) t.join();
        }

        workerThreads.clear();
        // std::cout << "[ThreadManager] All threads joined.\n";
    }

    pthread_t ThreadManager::self() {
        return pthread_self();
    }

    void ThreadManager::shutdown() {
        joinAll();
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            std::queue<std::function<void()>> empty;
            std::swap(taskQueue, empty);
        }
    }

}