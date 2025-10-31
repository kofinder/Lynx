#ifndef LYNX_FUNCTION_DOC_EXTRACTOR_PASS_HPP
#define LYNX_FUNCTION_DOC_EXTRACTOR_PASS_HPP

#include "llvm/IR/PassManager.h"

namespace LynxLTO {

    class FunctionDocExtractorPass : public llvm::PassInfoMixin<FunctionDocExtractorPass> {
        public:
            llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM);
    };

}

#endif
