#include <passes/DevirtualizeFunctionCallsPass.hpp>
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CallSite.h"
#include "llvm/Support/raw_ostream.h"

namespace LynxLTO {
    llvm::PreservedAnalyses DevirtualizeFunctionCallsPass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {
        llvm::errs() << "Running DevirtualizeFunctionCallsPass on module: " << M.getName() << "\n";
    
        for (auto &F : M) {
            if (F.isDeclaration()) continue;
    
            for (auto &BB : F) {
                for (auto &I : BB) {
                    if (auto *callInst = llvm::dyn_cast<llvm::CallBase>(&I)) {
                        llvm::Function *calledFn = callInst->getCalledFunction();
                        if (!calledFn) {
                            // Indirect call detected (likely virtual)
                            // TODO: Implement devirtualization heuristic or analysis
                            llvm::errs() << "Indirect call in function " << F.getName() << " at instruction: ";
                            callInst->print(llvm::errs());
                            llvm::errs() << "\n";
                        }
                    }
                }
            }
        }
    
        return llvm::PreservedAnalyses::all();
    }    
}

