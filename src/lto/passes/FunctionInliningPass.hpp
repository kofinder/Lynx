#ifndef LYNX_FUNCTION_INLINING_PASS_HPP
#define LYNX_FUNCTION_INLINING_PASS_HPP

#include "llvm/IR/PassManager.h"

namespace LynxLTO {

    class FunctionInliningPass : public llvm::PassInfoMixin<FunctionInliningPass> {
        public:
            llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM);
    };

}

#endif
