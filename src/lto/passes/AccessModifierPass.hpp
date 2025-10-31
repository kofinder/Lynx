#ifndef LYNX_ACCESS_MODIFIER_PASS_HPP
#define LYNX_ACCESS_MODIFIER_PASS_HPP

#include "llvm/IR/PassManager.h"


namespace LynxLTO {

    class AccessModifierPass : public llvm::PassInfoMixin<AccessModifierPass> {

        private:

            static char ID;

        public:

            llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM);
    };

}

#endif
