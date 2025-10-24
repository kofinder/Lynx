#ifndef LYNX_DEADCODE_ELIMINATION_PASS_HPP
#define LYNX_DEADCODE_ELIMINATION_PASS_HPP

#include "llvm/IR/PassManager.h"


namespace LynxLTO {

    class DeadCodeEliminationPass : public llvm::PassInfoMixin<DeadCodeEliminationPass> {
        public:
            llvm::PreservedAnalyses run(llvm::Function &F, llvm::FunctionAnalysisManager &FAM);
    };

}

#endif
