#ifndef LYNX_DEVIRTUALIZE_FUNCTION_CALLS_PASS_HPP
#define LYNX_DEVIRTUALIZE_FUNCTION_CALLS_PASS_HPP

#include "llvm/IR/PassManager.h"

namespace LynxLTO {

    class DevirtualizeFunctionCallsPass : public llvm::PassInfoMixin<DevirtualizeFunctionCallsPass> {
        public:
            llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM);
    };

}

#endif
