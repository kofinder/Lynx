#ifndef LYNX_VECTOR_ADAPTER_HPP
#define LYNX_VECTOR_ADAPTER_HPP

#include <vector>
#include <algorithm>
#include <interfaces/ICollection.hpp>

namespace LynxCollection {

    template <typename T>
    class VectorAdapter : public ICollection<T> {
        std::vector<T> data;

    public:
        void add(const T& value) override {
            data.push_back(value);
        }

        void remove(const T& value) override {
            data.erase(std::remove(data.begin(), data.end(), value), data.end());
        }

        bool contains(const T& value) const override {
            return std::find(data.begin(), data.end(), value) != data.end();
        }

        std::size_t size() const override {
            return data.size();
        }

        void clear() override {
            data.clear();
        }
    };

}

#endif
