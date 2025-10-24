#ifndef LYNX_NO_SORT_STRATEGY_HPP
#define LYNX_NO_SORT_STRATEGY_HPP

#include <vector>
#include <interfaces/SortingStrategy.hpp>

namespace LynxCollection {

    template <typename T>
    class NoSortStrategy : public SortingStrategy<T> {
        
        public:
            void sort(std::vector<T>&) override {
                // intentionally no-op
            }
    };

}

#endif
