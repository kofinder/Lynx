#ifndef LYNX_GLOBAL_DCEP_PASS_HPP
#define LYNX_GLOBAL_DCEP_PASS_HPP

#include "llvm/IR/PassManager.h"


namespace LynxLTO {

    class GlobalDCEPPass : public llvm::PassInfoMixin<GlobalDCEPPass> {
        
        public:
            llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM);

    };

}

#endif
