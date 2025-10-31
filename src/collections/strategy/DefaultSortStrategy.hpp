#ifndef LYNX_DEFAULT_SORTING_STRATEGY_HPP
#define LYNX_DEFAULT_SORTING_STRATEGY_HPP


#include <interfaces/SortingStrategy.hpp>

namespace LynxCollection {
    
    template <typename T>
    class DefaultSortStrategy : public SortingStrategy<T> {

        public:

            void sort(std::vector<T>& vec) override {
                std::sort(vec.begin(), vec.end());
            }

    };
}

#endif 
