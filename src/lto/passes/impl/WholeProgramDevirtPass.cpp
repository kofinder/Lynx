#include <passes/WholeProgramDevirtPass.hpp>
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"


namespace LynxLTO {

    llvm::PreservedAnalyses WholeProgramDevirtPass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {

        llvm::errs() << "Running FunctionInliningPass on module: " << "\n";
    
        bool Changed = false;

        for (auto &F : M) {
            if (F.isDeclaration()) continue;
    
            for (auto &BB : F) {
                for (auto &I : BB) {
                    if (auto *Call = llvm::dyn_cast<llvm::CallBase>(&I)) {
    
                        llvm::Function *Callee = Call->getCalledFunction();
    
                        if (!Callee) {
                            // Indirect call detected
                            llvm::errs() << "[WholeDevirt] Indirect call in function " 
                                         << F.getName() << ": ";
                            Call->print(llvm::errs());
                            llvm::errs() << "\n";
    
                            // Whole-program heuristic: check if only one possible target exists
                            llvm::Function *KnownCallee = nullptr;
    
                            if (auto *ConstFunc = llvm::dyn_cast<llvm::Function>(Call->getCalledOperand()->stripPointerCasts())) {
                                KnownCallee = ConstFunc;
                            }
    
                            if (KnownCallee) {
                                Call->setCalledFunction(KnownCallee); // replace indirect call
                                Changed = true;
                                llvm::errs() << "[WholeDevirt] Replaced with direct call to: " 
                                             << KnownCallee->getName() << "\n";
                            }
                        }
                    }
                }
            }
        }
    
        return Changed ? llvm::PreservedAnalyses::none() : llvm::PreservedAnalyses::all();
    }
    
}
