#ifndef LYNX_ISTACK_HPP
#define LYNX_ISTACK_HPP

namespace LynxCollection {

        template <typename T>
        class IStack {

        public:
            virtual ~IStack() {}

            virtual void push(const T& value) = 0;

            virtual void pop() = 0;

            virtual T top() const = 0;

            virtual bool isEmpty() const = 0;

            virtual std::size_t size() const = 0;
    };

}

#endif
