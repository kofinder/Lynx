#include <passes/VirtualCallOptimizationPass.hpp>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

namespace LynxLTO {

    llvm::PreservedAnalyses VirtualCallOptimizationPass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {
    
        llvm::errs() << "[Lynx] Running VirtualCallOptimizationPass on module: " << M.getName() << "\n";

        bool Changed = false;

        for (auto &F : M) {
            if (F.isDeclaration()) continue;
    
            for (auto &BB : F) {
                for (auto &I : BB) {
                    if (auto *Call = llvm::dyn_cast<llvm::CallBase>(&I)) {
    
                        llvm::Function *Callee = Call->getCalledFunction();
    
                        if (!Callee) {
                            // Indirect call detected — attempt devirtualization
                            llvm::errs() << "[Devirt] Indirect call in function " 
                                         << F.getName() << " at instruction: ";
                            Call->print(llvm::errs());
                            llvm::errs() << "\n";
    
                            // Heuristic: if only one possible target exists (known from metadata or global function table)
                            // Replace indirect call with direct call
                            llvm::Function *KnownCallee = nullptr;
    
                            // Example placeholder: in a real compiler, this should consult type info, vtables, or whole-program analysis
                            // Here we check if the call operand is a constant function pointer
                            if (auto *ConstFunc = llvm::dyn_cast<llvm::Function>(Call->getCalledOperand()->stripPointerCasts())) {
                                KnownCallee = ConstFunc;
                            }
    
                            if (KnownCallee) {
                                Call->setCalledFunction(KnownCallee); // replace indirect call
                                Changed = true;
                                llvm::errs() << "[Devirt] Replaced with direct call to: " 
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


