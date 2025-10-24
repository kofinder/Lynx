#ifndef LYNX_REMOVE_UNUSED_PARAMS_PASS_HPP
#define LYNX_REMOVE_UNUSED_PARAMS_PASS_HPP

#include "llvm/IR/PassManager.h"

namespace LynxLTO {

    class RemoveUnusedParamsPass : public llvm::PassInfoMixin<RemoveUnusedParamsPass> {
        public:
            llvm::PreservedAnalyses run(llvm::Function &F, llvm::FunctionAnalysisManager &FAM);
    };

}

#endif
