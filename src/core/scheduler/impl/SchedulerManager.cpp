#include <iostream>
#include "SchedulerManager.hpp"

namespace LynxCore {

    void SchedulerManager::initialize() {
        if (running) {
            std::cerr << "[SchedulerManager] Already initialized.\n";
            return;
        }

        running = true;
        std::cout << "[SchedulerManager] Starting background worker thread...\n";
        workerThread = std::thread(&SchedulerManager::workerLoop, this);
    }

    void SchedulerManager::workerLoop() {
        while (running) {
            Task task;

            {
                std::unique_lock<std::mutex> lock(queueMutex);
                cv.wait(lock, [this] { return !taskQueue.empty() || !running; });

                if (!running && taskQueue.empty())
                    return;

                task = std::move(taskQueue.front());
                taskQueue.pop();
            }

            try {
                task();
            } catch (const std::exception& e) {
                std::cerr << "[SchedulerManager] Task exception: " << e.what() << "\n";
            } catch (...) {
                std::cerr << "[SchedulerManager] Unknown error during task execution.\n";
            }
        }
    }

    void SchedulerManager::enqueueTask(Task task) {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (!running) {
                std::cerr << "[SchedulerManager] Attempted to enqueue task while scheduler is stopped.\n";
                return;
            }
            taskQueue.push(std::move(task));
        }
        cv.notify_one();
    }

    void SchedulerManager::start() {
        if (!running) {
            running = true;
            std::cout << "[SchedulerManager] Starting scheduler thread...\n";
            workerThread = std::thread(&SchedulerManager::workerLoop, this);
        }
    }

    void SchedulerManager::shutdown() {
        if (!running) {
            std::cout << "[SchedulerManager] Already shut down.\n";
            return;
        }

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            running = false;
        }
        cv.notify_all();

        if (workerThread.joinable()) {
            workerThread.join();
        }

        std::cout << "[SchedulerManager] Shutdown complete. All tasks processed.\n";
    }

} // namespace LynxCore
