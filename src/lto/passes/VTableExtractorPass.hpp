#ifndef LYNX_VTABLE_EXTRACTOR_PASS_HPP
#define LYNX_VTABLE_EXTRACTOR_PASS_HPP

#include "llvm/IR/PassManager.h"


namespace LynxLTO {

    class VTableExtractorPass : public llvm::PassInfoMixin<VTableExtractorPass> {

        public:

            llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM);
    };

}

#endif 

