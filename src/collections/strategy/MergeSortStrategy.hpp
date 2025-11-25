#ifndef LYNX_MERGE_SORT_STRATEGY_HPP
#define LYNX_MERGE_SORT_STRATEGY_HPP

#include <vector>
#include <interfaces/SortingStrategy.hpp>

namespace LynxCollection {

    template <typename T>
    class MergeSortStrategy : public SortingStrategy<T> {
        
        public:
            void sort(std::vector<T>& vec) override {
                if (vec.size() <= 1) return;
                mergeSort(vec, 0, vec.size() - 1);
            }

        private:
            void mergeSort(std::vector<T>& vec, int left, int right) {
                if (left >= right) return;
                int mid = left + (right - left) / 2;
                mergeSort(vec, left, mid);
                mergeSort(vec, mid + 1, right);
                merge(vec, left, mid, right);
            }

            void merge(std::vector<T>& vec, int left, int mid, int right) {
                std::vector<T> temp(right - left + 1);
                int i = left, j = mid + 1, k = 0;
                while (i <= mid && j <= right) {
                    if (vec[i] <= vec[j]) {
                        temp[k++] = vec[i++];
                    } else {
                        temp[k++] = vec[j++];
                    }
                }
                while (i <= mid) temp[k++] = vec[i++];
                while (j <= right) temp[k++] = vec[j++];
                for (int idx = 0; idx < (int)temp.size(); ++idx) {
                    vec[left + idx] = temp[idx];
                }
            }
    };

}

#endif
