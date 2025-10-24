#ifndef LYNX_FUNCTION_SECURITY_PASS_HPP
#define LYNX_FUNCTION_SECURITY_PASS_HPP

#include "llvm/IR/PassManager.h"


namespace LynxLTO {

    class FunctionSecurityPass : public llvm::PassInfoMixin<FunctionSecurityPass> {

        public:

            llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM);
    };

}

#endif 

