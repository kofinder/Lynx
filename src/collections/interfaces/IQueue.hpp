#ifndef LYNX_IQUEUE_HPP
#define LYNX_IQUEUE_HPP

namespace LynxCollection {
    
    template <typename T>
    class IQueue {

        public:

            virtual ~IQueue() {}

            virtual void enqueue(const T& value) = 0;

            virtual void dequeue() = 0;

            virtual T front() const = 0;

            virtual bool isEmpty() const = 0;

            virtual std::size_t size() const = 0;
    };

}

#endif 
