#ifndef LYNX_IMAP_HPP
#define LYNX_IMAP_HPP

namespace LynxCollection {

    template <typename K, typename V>
    class IMap {

        public:
            virtual ~IMap() {}

            virtual void put(const K& key, const V& value) = 0;

            virtual bool containsKey(const K& key) const = 0;

            virtual V get(const K& key) const = 0;

            virtual void remove(const K& key) = 0;

            virtual std::size_t size() const = 0;
            
            virtual void clear() = 0;
    };

}

#endif
