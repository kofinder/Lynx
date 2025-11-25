#ifndef LYNX_STACK_ADAPTER_HPP
#define LYNX_STACK_ADAPTER_HPP

#include <stack>
#include <interfaces/IStack.hpp>

namespace LynxCollection {

    template <typename T>
    class StackAdapter : public IStack<T> {
        std::stack<T> data;

    public:
        void push(const T& value) override {
            data.push(value);
        }

        void pop() override {
            if (!data.empty())
                data.pop();
        }

        T& top() override {
            if (data.empty()) throw std::runtime_error("Stack is empty");
            return data.top();
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
