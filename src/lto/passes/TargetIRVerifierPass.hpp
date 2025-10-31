#ifndef LYNX_TARGET_IR_VERIFIER_PASS_HPP
#define LYNX_TARGET_IR_VERIFIER_PASS_HPP

#include "llvm/IR/PassManager.h"


namespace LynxLTO {

    class TargetIRVerifierPass : public llvm::PassInfoMixin<TargetIRVerifierPass> {

        private:

            // Target-specific IR checks (you extend this for your target)
            bool verifyModuleForTarget(llvm::Module &M);

            // Example: Check individual functions for target-specific rules
            bool verifyFunctionForTarget(llvm::Function &F);

        public:

            llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM);
    };

}

#endif
