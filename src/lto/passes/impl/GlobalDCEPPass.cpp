#include <passes/GlobalDCEPPass.hpp>
#include "llvm/IR/Module.h"
#include "llvm/Transforms/IPO/GlobalDCE.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace LynxLTO {

    llvm::PreservedAnalyses GlobalDCEPPass::run(Module &M, ModuleAnalysisManager &MAM) {
        errs() << "Running Lynx Global Dead Code Elimination Pass on module\n";

        // bool Changed = llvm::GlobalDCE(M);

        // if (Changed) {
        //     errs() << "Global Dead Code Elimination made changes to the module\n";
        //     return PreservedAnalyses::none();
        // }

        return PreservedAnalyses::all();
    }

}
