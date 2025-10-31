#ifndef LYNX_SORTING_STRATEGY_HPP
#define LYNX_SORTING_STRATEGY_HPP

namespace LynxCollection {

    template <typename T>
    class SortingStrategy {

        public:

            virtual ~SortingStrategy() {}
            
            virtual void sort(std::vector<T>& vec) = 0;
    };
    
}

#endif 
