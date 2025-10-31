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
