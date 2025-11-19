#include <passes/GlobalDCEPPass.hpp>
#include "llvm/IR/Verifier.h"
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

namespace LynxLTO {

    using namespace llvm;

    llvm::PreservedAnalyses GlobalDCEPPass::run(Module &M, ModuleAnalysisManager &MAM) {
        errs() << "Running Lynx Global Dead Code Elimination Pass on module\n";

        bool Changed = false;    
        // Remove unused functions
        for (auto FuncIter = M.begin(); FuncIter != M.end(); ) {
            llvm::Function &F = *FuncIter++;
            if (F.isDeclaration()) continue;
            if (F.use_empty()) {
                llvm::errs() << "Removing unused function: " << F.getName() << "\n";
                F.eraseFromParent();
                Changed = true;
            }
        }
    
        // Remove unused global variables
        for (auto GVIter = M.global_begin(); GVIter != M.global_end(); ) {
            llvm::GlobalVariable &GV = *GVIter++;
            if (GV.use_empty() && !GV.hasInitializer()) continue; // skip essential
            if (GV.use_empty()) {
                llvm::errs() << "Removing unused global variable: " << GV.getName() << "\n";
                GV.eraseFromParent();
                Changed = true;
            }
        }
    
        // Remove unused aliases
        for (auto AI = M.alias_begin(); AI != M.alias_end(); ) {
            llvm::GlobalAlias &GA = *AI++;
            if (GA.use_empty()) {
                llvm::errs() << "Removing unused global alias: " << GA.getName() << "\n";
                GA.eraseFromParent();
                Changed = true;
            }
        }
    
        return Changed ? llvm::PreservedAnalyses::none()
                       : llvm::PreservedAnalyses::all();
    }

}
