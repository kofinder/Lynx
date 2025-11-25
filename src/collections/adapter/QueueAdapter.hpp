#ifndef LYNX_QUEUE_ADAPTER_HPP
#define LYNX_QUEUE_ADAPTER_HPP

#include <queue>
#include <interfaces/IQueue.hpp>

namespace LynxCollection {

    template <typename T>
    class QueueAdapter : public IQueue<T> {
        std::queue<T> data;

    public:
        void enqueue(const T& value) override {
            data.push(value);
        }

        void dequeue() override {
            if (!data.empty())
                data.pop();
        }

        T& front() override {
            if (data.empty()) throw std::runtime_error("Queue is empty");
            return data.front();
        }

        bool empty() const override {
            return data.empty();
        }

        std::size_t size() const override {
            return data.size();
        }

        void clear() override {
            while (!data.empty()) data.pop();
        }
    };

}

#endif
