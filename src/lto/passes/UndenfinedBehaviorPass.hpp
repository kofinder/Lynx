#ifndef LYNX_UNDEFINED_BEHAVIOR_PASS_HPP
#define LYNX_UNDEFINED_BEHAVIOR_PASS_HPP


#include "llvm/IR/PassManager.h"

namespace LynxLTO {

    class UndenfinedBehaviorPass : public llvm::PassInfoMixin<UndenfinedBehaviorPass> {

        public:

            llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM);
            
    };

}


#endif 
