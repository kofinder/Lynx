/**
 * @file DefaultLTOOptimizationStrategy.hpp
 * @brief Defines the base interface for LTO optimization strategies.
 *
 * The `DefaultLTOOptimizationStrategy` class provides an abstract interface for
 * link-time optimization (LTO) strategies. Derived classes, such as `ThinLTO`
 * and `FullLTO`, implement specific optimization pipelines for LLVM modules.
 *
 * **Key Responsibilities:**
 * - Provide a standard interface for applying LTO optimizations to LLVM modules.
 * - Enable flexible strategy-based optimization in the LTOFacade workflow.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_DEFAULT_LTO_OPTIMIZATION_STRATEGY_HPP
#define LYNX_DEFAULT_LTO_OPTIMIZATION_STRATEGY_HPP

#include <llvm/IR/Value.h>

namespace LynxLTO {
    
    class DefaultLTOOptimizationStrategy {

        public:

            virtual void optimize(llvm::Module &M) = 0;
            
            virtual ~DefaultLTOOptimizationStrategy() = default;

    };
}

#endif 
