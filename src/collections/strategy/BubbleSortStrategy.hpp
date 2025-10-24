#ifndef LYNX_BUBBLE_SORT_STRATEGY_HPP
#define LYNX_BUBBLE_SORT_STRATEGY_HPP

#include <vector>
#include <interfaces/SortingStrategy.hpp>

namespace LynxCollection {

    template <typename T>
    class BubbleSortStrategy : public SortingStrategy<T> {
        public:
            void sort(std::vector<T>& vec) override {
                bool swapped;
                for (std::size_t i = 0; i < vec.size(); ++i) {
                    swapped = false;
                    for (std::size_t j = 1; j < vec.size() - i; ++j) {
                        if (vec[j - 1] > vec[j]) {
                            std::swap(vec[j - 1], vec[j]);
                            swapped = true;
                        }
                    }
                    if (!swapped) break;
                }
            }
    };

}

#endif
