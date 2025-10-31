#include <passes/AttributorPass.hpp>
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace LynxLTO {

    llvm::PreservedAnalyses AttributorPass::run(Module &M, ModuleAnalysisManager &MAM) {
        // errs() << "Running Lynx Attributor Pass\n";

        // // Create Attributor instance for this module
        // Attributor A(M);

        // // Add default function-level info inferers, e.g. noalias, readonly, etc.
        // for (Function &F : M) {
        //     if (F.isDeclaration()) continue;

        //     // Register known info inferers for function
        //     A.registerFunctionInfo(F);
        // }

        // // Run the attributor to infer attributes
        // A.run();

        // // After inference, apply attributes to functions
        // for (Function &F : M) {
        //     if (F.isDeclaration()) continue;

        //     // Update function attributes inferred by Attributor
        //     A.apply(F);
        // }

        // errs() << "Lynx Attributor Pass completed\n";

        return PreservedAnalyses::none(); // we modified the IR
    }

}
