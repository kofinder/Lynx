#ifndef LYNX_SORTING_STRATEGY_KIND_HPP
#define LYNX_SORTING_STRATEGY_KIND_HPP


#include <string>
#include <unordered_map>

namespace LynxCollection {
    
    enum class SortingStrategyKind {
        DEFAULT = 1,
        BUBLE   = 2,
        MERGE   = 3,
        INSERT  = 4,
        NO      = 5,
    };

    inline SortingStrategyKind operator++(SortingStrategyKind& kind, int num) {
        SortingStrategyKind old = kind;
        kind = static_cast<SortingStrategyKind>(static_cast<int>(kind) + 1);
        return old;
    }

    inline SortingStrategyKind parseCollectionKind(const std::string& type) {
        static const std::unordered_map<std::string, SortingStrategyKind> mapping = {
            { "default",    SortingStrategyKind::DEFAULT },
            { "buble",      SortingStrategyKind::BUBLE },
            { "merge",      SortingStrategyKind::MERGE },
            { "insert",     SortingStrategyKind::INSERT },
        };

        auto it = mapping.find(type);
        if (it != mapping.end()) {
            return it->second;
        }

        return SortingStrategyKind::NO;
    }

    inline std::string toString(SortingStrategyKind type) {
        switch (type) {
            case SortingStrategyKind::DEFAULT:      return "default";
            case SortingStrategyKind::BUBLE:        return "buble";
            case SortingStrategyKind::MERGE:        return "merge";
            case SortingStrategyKind::INSERT:       return "insert";
            default:                                return "no";
        }
    }
}

#endif 
