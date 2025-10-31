#ifndef LYNX_ARRAY_ADAPTER_HPP
#define LYNX_ARRAY_ADAPTER_HPP

#include <algorithm>
#include <stdexcept>

#include <interfaces/ICollection.hpp>

namespace LynxCollection {

    template <typename T, std::size_t N>
    class ArrayAdapter : public ICollection<T> {
        T data[N];
        std::size_t count = 0;  // current number of elements

        public:
            void add(const T& value) override {
                if (count >= N) {
                    throw std::runtime_error("ArrayAdapter overflow: cannot add more elements");
                }
                data[count++] = value;
            }

            void remove(const T& value) override {
                auto pos = std::find(data, data + count, value);
                if (pos != data + count) {
                    std::move(pos + 1, data + count, pos);
                    --count;
                }
            }

            bool contains(const T& value) const override {
                return std::find(data, data + count, value) != data + count;
            }

            std::size_t size() const override {
                return count;
            }

            void clear() override {
                count = 0;
            }
    };

} 

#endif
