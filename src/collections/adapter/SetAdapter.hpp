#ifndef LYNX_SET_ADAPTER_HPP
#define LYNX_SET_ADAPTER_HPP

#include <set>
#include <interfaces/ICollection.hpp>

namespace LynxCollection {

    template <typename T>
    class SetAdapter : public ICollection<T> {
        std::set<T> data;

    public:
        void add(const T& value) override {
            data.insert(value);
        }

        void remove(const T& value) override {
            data.erase(value);
        }

        bool contains(const T& value) const override {
            return data.find(value) != data.end();
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
