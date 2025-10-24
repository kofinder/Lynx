#ifndef LYNX_DEVIRTUALIZE_PASS_HPP
#define LYNX_DEVIRTUALIZE_PASS_HPP

#include "llvm/IR/PassManager.h"

namespace LynxLTO {

    class DevirtualizePass : public llvm::PassInfoMixin<DevirtualizePass> {
        public:
            llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM);
    };

}

#endif
