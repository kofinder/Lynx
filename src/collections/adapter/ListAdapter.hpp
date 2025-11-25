#ifndef LYNX_LIST_ADAPTER_HPP
#define LYNX_LIST_ADAPTER_HPP

#include <list>
#include <interfaces/ICollection.hpp>

namespace LynxCollection {

    template <typename T>
    class ListAdapter : public ICollection<T> {
        std::list<T> data;

    public:
        void add(const T& value) override {
            data.push_back(value);
        }

        void remove(const T& value) override {
            data.remove(value);
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
