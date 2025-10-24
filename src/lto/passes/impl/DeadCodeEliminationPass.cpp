#include <passes/DeadCodeEliminationPass.hpp>
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Analysis/DeadCodeElimination.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace LynxLTO {

    llvm::PreservedAnalyses DeadCodeEliminationPass::run(Function &F, FunctionAnalysisManager &FAM) {
        errs() << "Running Lynx Dead Code Elimination Pass on function: " << F.getName() << "\n";

        // Use LLVM's utility to eliminate dead instructions inside the function
        bool Changed = llvm::eliminateDeadCode(F);

        if (Changed) {
            errs() << "Dead code eliminated in function: " << F.getName() << "\n";
            // Indicate IR changed
            return PreservedAnalyses::none();
        } else {
            // No changes, preserve all analyses
            return PreservedAnalyses::all();
        }
    }

}
