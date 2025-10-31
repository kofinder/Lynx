#ifndef LYNX_SEMANTIC_THREAD_POOL_HPP
#define LYNX_SEMANTIC_THREAD_POOL_HPP

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <stdexcept>
#include <type_traits>

namespace LynxAnalyzer {

    /**
     * @brief Thread pool specialized for semantic analysis tasks.
     * 
     * Efficiently distributes AST semantic checks across multiple worker threads.
     */
    class SemanticThreadPool {
        private:
            std::vector<std::thread> workers;                     ///< Worker threads
            std::queue<std::function<void()>> tasks;              ///< Task queue

            std::mutex queueMutex;                                ///< Protects task queue
            std::condition_variable condition;                    ///< Worker condition
            std::condition_variable finishedCondition;            ///< Signals when queue is empty
            bool stop = false;                                    ///< Pool shutdown flag
            size_t activeTasks = 0;                               ///< Number of currently running tasks

        public:
            /**
             * @brief Construct a new SemanticThreadPool
             * 
             * @param threads Number of worker threads (defaults to hardware concurrency).
             */
            explicit SemanticThreadPool(size_t threads = std::thread::hardware_concurrency()) {
                if (threads == 0) {
                    threads = 2; // fallback if hardware_concurrency() returns 0
                }
                for (size_t i = 0; i < threads; ++i) {
                    workers.emplace_back([this] {
                        while (true) {
                            std::function<void()> task;
                            {
                                std::unique_lock<std::mutex> lock(queueMutex);
                                condition.wait(lock, [this] { return stop || !tasks.empty(); });

                                if (stop && tasks.empty())
                                    return;

                                task = std::move(tasks.front());
                                tasks.pop();
                                ++activeTasks;
                            }

                            // Execute the task outside lock
                            task();

                            {
                                std::unique_lock<std::mutex> lock(queueMutex);
                                --activeTasks;
                                if (tasks.empty() && activeTasks == 0) {
                                    finishedCondition.notify_all();
                                }
                            }
                        }
                    });
                }
            }

            /**
             * @brief Enqueue a new task into the pool.
             * 
             * @tparam F Callable type.
             * @param f Callable to enqueue.
             * @return std::future<return_type> Future result of the task.
             */
            template<typename F>
            auto enqueue(F&& f) -> std::future<std::invoke_result_t<F>> {
                using return_type = std::invoke_result_t<F>;

                auto task = std::make_shared<std::packaged_task<return_type()>>(std::forward<F>(f));
                std::future<return_type> res = task->get_future();

                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    if (stop) {
                        throw std::runtime_error("enqueue on stopped SemanticThreadPool");
                    }
                    tasks.emplace([task]() { (*task)(); });
                }

                condition.notify_one();
                return res;
            }

            /**
             * @brief Wait for all currently enqueued tasks to finish.
             */
            void waitForCompletion() {
                std::unique_lock<std::mutex> lock(queueMutex);
                finishedCondition.wait(lock, [this] { return tasks.empty() && activeTasks == 0; });
            }

            /**
             * @brief Destroy the SemanticThreadPool and join all threads.
             */
            ~SemanticThreadPool() {
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    stop = true;
                }
                condition.notify_all();
                for (std::thread &worker : workers) {
                    if (worker.joinable()) {
                        worker.join();
                    }
                }
            }
        };

}

#endif
