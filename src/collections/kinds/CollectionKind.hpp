#ifndef LYNX_COLLECTION_KIND_HPP
#define LYNX_COLLECTION_KIND_HPP


#include <string>
#include <unordered_map>

namespace LynxCollection {
    
    enum class CollectionKind {
        COLLECTION = 18,
        ARRAY      = 19,
        VECTOR     = 20,
        LIST       = 21,
        SET        = 22,
        MAP        = 23,
        STACK      = 24,
        QUEUE      = 25,
        TREE       = 26,
        GRAPH      = 27,
        HEAP       = 28,
        DICT       = 29,
    };

    inline CollectionKind operator++(CollectionKind& kind, int num) {
        CollectionKind old = kind;
        kind = static_cast<CollectionKind>(static_cast<int>(kind) + 1);
        return old;
    }

    inline CollectionKind parseCollectionKind(const std::string& type) {
        static const std::unordered_map<std::string, CollectionKind> mapping = {
            { "array",    CollectionKind::ARRAY },
            { "vector",   CollectionKind::VECTOR },
            { "list",     CollectionKind::LIST },
            { "set",      CollectionKind::SET },
            { "map",      CollectionKind::MAP },
            { "stack",    CollectionKind::STACK },
            { "queue",    CollectionKind::QUEUE },
            { "tree",     CollectionKind::TREE },
            { "graph",    CollectionKind::GRAPH },
            { "heap",     CollectionKind::HEAP },
            { "dict",     CollectionKind::DICT },
        };

        auto it = mapping.find(type);
        if (it != mapping.end()) {
            return it->second;
        }

        return CollectionKind::COLLECTION;
    }

    inline std::string toString(CollectionKind type) {
        switch (type) {
            case CollectionKind::ARRAY:      return "array";
            case CollectionKind::VECTOR:     return "vector";
            case CollectionKind::LIST:       return "list";
            case CollectionKind::SET:        return "set";
            case CollectionKind::MAP:        return "map";
            case CollectionKind::STACK:      return "stack";
            case CollectionKind::QUEUE:      return "queue";
            case CollectionKind::TREE:       return "tree";
            case CollectionKind::GRAPH:      return "graph";
            case CollectionKind::HEAP:       return "heap";
            case CollectionKind::DICT:       return "dict";
            default:                         return "collection";
        }
    }
}

#endif 
