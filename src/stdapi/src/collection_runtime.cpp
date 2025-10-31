#include "collection_runtime.h"
#include <collections/factory/CollectionFactory.hpp>
#include <collections/kinds/CollectionKind.hpp>
#include <collections/interfaces/ICollection.hpp>
#include <collections/interfaces/IMap.hpp>
#include <collections/adapter/DictAdapter.hpp>


#include <unordered_map>
#include <string>
#include <any>
#include <stdexcept>
#include <cstring>

using namespace LynxCollection;

struct CollectionWrapper {
    CollectionKind kind;
    std::unique_ptr<ICollection<int>> intCollection;
    std::unique_ptr<IMap<std::string, int>> map;
};


extern "C" {

    collection_t collection_create(int kind) {
        auto* wrapper = new CollectionWrapper();
        wrapper->kind = static_cast<CollectionKind>(kind);
        switch (wrapper->kind) {
            case CollectionKind::VECTOR:
            case CollectionKind::SET:
            case CollectionKind::LIST:
                wrapper->intCollection = CollectionFactory::createCollection<int>(wrapper->kind);
                break;
            case CollectionKind::MAP:
            case CollectionKind::DICT:
                wrapper->map = CollectionFactory::createMap<std::string, int>();
                break;
            default:
                delete wrapper;
                return nullptr;
        }
    
        return reinterpret_cast<collection_t>(wrapper);
    }
    
    
    void collection_add_int(collection_t col, int value) {
        auto* wrapper = reinterpret_cast<CollectionWrapper*>(col);
        if (wrapper && wrapper->intCollection)
            wrapper->intCollection->add(value);
    }
    
    int collection_contains_int(collection_t col, int value) {
        auto* wrapper = reinterpret_cast<CollectionWrapper*>(col);
        return (wrapper && wrapper->intCollection && wrapper->intCollection->contains(value)) ? 1 : 0;
    }
    
    void collection_remove_int(collection_t col, int value) {
        auto* wrapper = reinterpret_cast<CollectionWrapper*>(col);
        if (wrapper && wrapper->intCollection)
            wrapper->intCollection->remove(value);
    }
    
    int collection_size(collection_t col) {
        auto* wrapper = reinterpret_cast<CollectionWrapper*>(col);
        return wrapper && wrapper->intCollection ? static_cast<int>(wrapper->intCollection->size()) : 0;
    }
    
    void collection_clear(collection_t col) {
        auto* wrapper = reinterpret_cast<CollectionWrapper*>(col);
        if (wrapper && wrapper->intCollection)
            wrapper->intCollection->clear();
    }
    
    void collection_destroy(collection_t col) {
        auto* wrapper = reinterpret_cast<CollectionWrapper*>(col);
        delete wrapper;
    }
    
    // Map-specific
    void collection_put_str_int(collection_t col, const char* key, int value) {
        auto* wrapper = reinterpret_cast<CollectionWrapper*>(col);
        if (wrapper && wrapper->map)
            wrapper->map->put(key, value);
    }
    
    int collection_get_str_int(collection_t col, const char* key) {
        auto* wrapper = reinterpret_cast<CollectionWrapper*>(col);
        if (wrapper && wrapper->map)
            return wrapper->map->get(key);
        return 0;
    }
    
    int collection_has_key_str(collection_t col, const char* key) {
        auto* wrapper = reinterpret_cast<CollectionWrapper*>(col);
        return (wrapper && wrapper->map && wrapper->map->containsKey(key)) ? 1 : 0;
    }
    
    void collection_remove_key_str(collection_t col, const char* key) {
        auto* wrapper = reinterpret_cast<CollectionWrapper*>(col);
        if (wrapper && wrapper->map)
            wrapper->map->remove(key);
    }
    
}
