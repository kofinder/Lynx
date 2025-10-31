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
            /// Type alias for a scheduled task
            using Task = std::function<void()>;

        private:

            std::queue<Task> taskQueue;

            std::mutex queueMutex;

            std::condition_variable cv;

            std::thread workerThread;

            std::atomic<bool> running;

            /**
             * @brief Internal function that continuously executes queued tasks.
             *
             * Blocks until a task is available or the shutdown signal is received.
            */
            void workerLoop();

        public:

            /**
             * @brief Constructs a new SchedulerManager instance.
             *
             * Initializes internal state and sets the running flag to false.
             * The scheduler thread is not started until `start()` is called.
            */
           SchedulerManager();

            /**
             * @brief Destructor ensures a clean shutdown.
             *
             * If the scheduler is still running, it will be gracefully shut down.
             */
            ~SchedulerManager();

            /**
             * @brief Enqueues a new task for asynchronous execution.
             *
             * @param task A callable (lambda, function, etc.) to be executed in the scheduler thread.
            */
            void enqueueTask(Task task);

            /**
             * @brief Starts the background scheduler thread.
             *
             * This will spawn a worker thread that processes tasks from the queue.
             * Calling start() more than once has no effect.
            */
            void start();

            /**
             * @brief Gracefully shuts down the scheduler thread.
             *
             * Waits for the worker thread to finish all pending tasks before terminating.
            */
            void shutdown();

    };   
}

#endif
