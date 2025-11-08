/**
 * @file SchedulerManager.hpp
 * @brief Declares the SchedulerManager class for task scheduling and execution in the Lynx runtime.
 * 
 * The SchedulerManager class provides an asynchronous task execution environment.
 * It maintains a task queue, runs a background worker thread, and supports graceful shutdown
 * and synchronization. It is designed to manage lightweight tasks efficiently in the runtime.
 * 
 * **Key Responsibilities:**
 * - Enqueue and dispatch tasks for asynchronous execution.
 * - Run a dedicated background thread for task processing.
 * - Support graceful shutdown, ensuring all queued tasks are completed.
 * 
 * **Used By:**
 * - Runtime modules requiring asynchronous task execution.
 * - Task scheduling subsystems and parallel execution utilities.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_CORE_SCHEDULER_MANAGER_HPP
#define LYNX_CORE_SCHEDULER_MANAGER_HPP

#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>

namespace LynxCore {

    /**
     * @class SchedulerManager
     * @brief Manages task scheduling and execution in the Lynx runtime.
     *
     * Responsibilities:
     * - Enqueue and dispatch lightweight tasks.
     * - Run a background thread to process tasks.
     * - Support graceful shutdown and task synchronization.
    */

    class SchedulerManager {
        
        public:

            using Task = std::function<void()>;
        
        private:

            std::queue<Task> taskQueue;

            std::mutex queueMutex;

            std::condition_variable cv;

            std::thread workerThread;

            std::atomic<bool> running{false};
        
            void workerLoop();
        
        public:

            SchedulerManager() = default;
            ~SchedulerManager() { shutdown(); }
        
            void initialize();

            void enqueueTask(Task task);

            void start();
            
            void shutdown();
    };
        
}

#endif
