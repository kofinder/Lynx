#include <passes/RequireAnalysisPass.hpp>
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

namespace LynxLTO {

    llvm::PreservedAnalyses RequireAnalysisPass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {
        llvm::errs() << "Running RequireAnalysisPass on module: " << M.getName() << "\n";

        // Example analysis: print all function names
        for (auto &F : M) {
            llvm::errs() << "Function: " << F.getName() << "\n";
            // You can add your custom analysis logic here
        }

        // This pass does not modify the module, so preserve all analyses
        return llvm::PreservedAnalyses::all();
    }
}