#ifndef LYNX_FUNCTION_SIGNATURE_AUDIT_PASS_HPP
#define LYNX_FUNCTION_SIGNATURE_AUDIT_PASS_HPP

#include "llvm/IR/PassManager.h"


namespace LynxLTO {

    class FunctionSignatureAuditPass : public llvm::PassInfoMixin<FunctionSignatureAuditPass> {

        public:

            llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM);
    };

}

#endif 

