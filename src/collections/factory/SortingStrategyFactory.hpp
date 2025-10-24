#ifndef LYNX_SORTING_STRATEGY_FACTORY_HPP
#define LYNX_SORTING_STRATEGY_FACTORY_HPP

#include <memory>
#include <stdexcept>
#include "kinds/SortingStrategyKind.hpp"
#include "interfaces/SortingStrategy.hpp"
#include "strategy/DefaultSortStrategy.hpp"
#include "strategy/BubbleSortStrategy.hpp"


namespace LynxCollection {


    class SortingStrategyFactory {

        public:
            SortingStrategyFactory() = delete;

            template <typename T>
            static std::unique_ptr<SortingStrategy<T>> createSortingStrategy(SortingStrategyKind kind) {
                switch (kind) {
                    case SortingStrategyKind::DEFAULT:
                        return std::make_unique<DefaultSortStrategy<T>>();
                    case SortingStrategyKind::BUBBLE:
                        return std::make_unique<BubbleSortStrategy<T>>();
                    default:
                        throw std::runtime_error("Unsupported sorting strategy");
                }
        }
    };

}

#endif
