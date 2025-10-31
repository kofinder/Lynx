#ifndef LYNX_FUNCTION_METADATA_PASS_HPP
#define LYNX_FUNCTION_METADATA_PASS_HPP

#include "llvm/IR/PassManager.h"


namespace LynxLTO {

    class FunctionMetadataPass : public llvm::PassInfoMixin<FunctionMetadataPass> {

        public:

            llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM);
    };

}

#endif 

