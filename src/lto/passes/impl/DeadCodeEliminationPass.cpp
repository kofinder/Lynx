#include <passes/DeadCodeEliminationPass.hpp>
#include "llvm/IR/Verifier.h"
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/Utils/Local.h>


namespace LynxLTO {
    using namespace llvm;

    llvm::PreservedAnalyses DeadCodeEliminationPass::run(Function &F, FunctionAnalysisManager &FAM) {
        errs() << "Running Lynx Dead Code Elimination Pass on function: " << F.getName() << "\n";

        bool Changed = false;

        // LLVM 21: manually iterate over basic blocks and instructions
        for (auto &BB : F) {
            for (auto InstIter = BB.begin(); InstIter != BB.end(); ) {
                Instruction &I = *InstIter++;
                if (isInstructionTriviallyDead(&I)) {
                    I.eraseFromParent();
                    Changed = true;
                }
            }
        }
    
        if (Changed) {
            errs() << "Dead code eliminated in function: " << F.getName() << "\n";
            return PreservedAnalyses::none(); // IR changed
        } else {
            return PreservedAnalyses::all();  // No changes
        }
    }

}
