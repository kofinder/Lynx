#include <passes/RemoveUnusedParamsPass.hpp>
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Transforms/Utils/Local.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace LynxLTO {

    llvm::PreservedAnalyses RemoveUnusedParamsPass::run(Function &F, FunctionAnalysisManager &FAM) {
        bool Changed = false;
        errs() << "Running RemoveUnusedParamsPass on function: " << F.getName() << "\n";

        // Iterate parameters and check if they are used
        for (auto &Arg : F.args()) {
            if (Arg.use_empty()) {
                errs() << "Parameter " << Arg.getName() << " is unused.\n";
                // You could do more complex removal here, but LLVM IR
                // does not allow removing params directly.
                // Usually, this requires function cloning or rewriting.
                // For now, just note unused params.
                // Implement parameter removal with cloning if needed.
                Changed = true;
            }
        }

        return (Changed ? PreservedAnalyses::none() : PreservedAnalyses::all());
    }

}
