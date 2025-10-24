#ifndef LYNX_ATTRIBUTOR_PASS_HPP
#define LYNX_ATTRIBUTOR_PASS_HPP

#include "llvm/IR/PassManager.h"


namespace LynxLTO {

    class AttributorPass : public llvm::PassInfoMixin<AttributorPass> {

        public:

            llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM);
    };

}

#endif
