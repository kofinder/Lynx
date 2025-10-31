#ifndef LYNX_FUNCTION_DOC_DUMPER_PASS_HPP
#define LYNX_FUNCTION_DOC_DUMPER_PASS_HPP

#include "llvm/IR/PassManager.h"

namespace LynxLTO {

    class FunctionDocDumperPass : public llvm::PassInfoMixin<FunctionDocDumperPass> {
        public:
            llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM);
    };

}

#endif
