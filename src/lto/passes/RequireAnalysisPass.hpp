#ifndef LYNX_REQURIE_ANALYSIS_PASS_HPP
#define LYNX_REQURIE_ANALYSIS_PASS_HPP

#include "llvm/IR/PassManager.h"


namespace LynxLTO {

    class RequireAnalysisPass : public llvm::PassInfoMixin<RequireAnalysisPass> {
        
        public:
            llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM);
    };

}

#endif
