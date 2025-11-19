#include <passes/AttributorPass.hpp>
#include "llvm/IR/Verifier.h"
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

namespace LynxLTO {
    
    using namespace llvm;

    llvm::PreservedAnalyses AttributorPass::run(Module &M, ModuleAnalysisManager &MAM) {
        errs() << "Running Lynx Attributor Pass\n";
        for (auto& F : M) {
            if (F.isDeclaration()) continue;
    
            bool hasSideEffects = false;
            for (auto& BB : F) {
                for (auto& I : BB) {
                    if (!I.mayReadOrWriteMemory()) continue;
                    hasSideEffects = true;
                    break;
                }
                if (hasSideEffects) break;
            }
    
            if (!hasSideEffects) {
                F.addFnAttr(llvm::Attribute::ReadOnly);
            }
        }

        return PreservedAnalyses::all();
    }

}
