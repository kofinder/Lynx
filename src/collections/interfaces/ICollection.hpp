#ifndef LYNX_ICOLLECTION_HPP
#define LYNX_ICOLLECTION_HPP

namespace LynxCollection {

    template <typename T>
    class ICollection {

        public:

            virtual ~ICollection() {}

            virtual void add(const T& value) = 0;

            virtual void remove(const T& value) = 0;

            virtual bool contains(const T& value) const = 0;

            virtual std::size_t size() const = 0;
            
            virtual void clear() = 0;
    };

}

#endif 
