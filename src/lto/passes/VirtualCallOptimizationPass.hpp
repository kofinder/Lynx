#ifndef LYNX_VIRTUAL_CALL_OPTIMIZATION_PASS_HPP
#define LYNX_VIRTUAL_CALL_OPTIMIZATION_PASS_HPP

#include "llvm/IR/PassManager.h"

namespace LynxLTO {

    class VirtualCallOptimizationPass : public llvm::PassInfoMixin<VirtualCallOptimizationPass> {
        public:
            llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM);
    };

}

#endif
