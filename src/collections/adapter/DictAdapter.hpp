#ifndef LYNX_DICT_ADAPTER_HPP
#define LYNX_DICT_ADAPTER_HPP

#include <interfaces/IMap.hpp>

namespace LynxCollection {

    template <typename K, typename V>
    class DictAdapter : public IMap<K, V> {
        std::unordered_map<K, V> data;

    public:
        void put(const K& key, const V& value) override {
            data[key] = value;
        }

        bool containsKey(const K& key) const override {
            return data.find(key) != data.end();
        }

        V get(const K& key) const override {
            auto it = data.find(key);
            if (it == data.end()) throw std::runtime_error("Key not found");
            return it->second;
        }

        void remove(const K& key) override {
            data.erase(key);
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
