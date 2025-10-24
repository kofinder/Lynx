#include "scheduler/SchedulerManager.hpp"

namespace LynxCore {

   
    SchedulerManager::SchedulerManager() : running(false) {}

   
    SchedulerManager::~SchedulerManager() { shutdown(); }

    void SchedulerManager::start() {
        if (running.load()) return;

        running.store(true);
        workerThread = std::thread(&SchedulerManager::workerLoop, this);
    }

    void SchedulerManager::enqueueTask(Task task) {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            taskQueue.push(std::move(task));
        }
        cv.notify_one();
    }

    void SchedulerManager::shutdown() {
        if (!running.load()) return;

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            running.store(false);
        }

        cv.notify_one();

        if (workerThread.joinable())
            workerThread.join();
    }

    void SchedulerManager::workerLoop() {
        while (true) {
            Task task;

            {
                std::unique_lock<std::mutex> lock(queueMutex);

                cv.wait(lock, [&] {
                    return !taskQueue.empty() || !running.load();
                });

                if (!running.load() && taskQueue.empty())
                    break;

                task = std::move(taskQueue.front());
                taskQueue.pop();
            }

            if (task)
                task();
        }
    }

}
