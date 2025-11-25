#ifndef LYNX_INSERTION_SORT_STRATEGY_HPP
#define LYNX_INSERTION_SORT_STRATEGY_HPP

#include <vector>
#include <interfaces/SortingStrategy.hpp>

namespace LynxCollection {

    template <typename T>
    class InsertionSortStrategy : public SortingStrategy<T> {
        public:
            void sort(std::vector<T>& vec) override {
                for (std::size_t i = 1; i < vec.size(); ++i) {
                    T key = vec[i];
                    int j = static_cast<int>(i) - 1;
                    while (j >= 0 && vec[j] > key) {
                        vec[j + 1] = vec[j];
                        --j;
                    }
                    vec[j + 1] = key;
                }
            }
    };

}

#endif
