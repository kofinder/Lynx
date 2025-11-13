#include <passes/FunctionInliningPass.hpp>
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Support/raw_ostream.h"

namespace LynxLTO {

    using namespace llvm;

    llvm::PreservedAnalyses FunctionInliningPass::run(llvm::Module& M, llvm::ModuleAnalysisManager& MAM) {
        
        llvm::errs() << "Running FunctionInliningPass on module: " << "\n";

        bool Changed = false;

        for (auto &F : M) {
            if (F.isDeclaration()) continue;
    
            for (auto &BB : F) {
                for (auto InstIter = BB.begin(); InstIter != BB.end(); ) {
                    Instruction &I = *InstIter++;
    
                    if (auto *Call = llvm::dyn_cast<llvm::CallBase>(&I)) {
                        llvm::Function *Callee = Call->getCalledFunction();
    
                        if (!Callee || Callee->isDeclaration()) continue;
    
                        // Skip recursive inlining for now
                        if (Callee == &F) continue;
    
                        llvm::InlineFunctionInfo IFI;
                        if (llvm::InlineFunction(*Call, IFI).isSuccess()) {
                            Changed = true;
                            llvm::errs() << "Inlined call to " << Callee->getName()
                                         << " in function " << F.getName() << "\n";
                        }
                    }
                }
            }
        }
    
        return Changed ? llvm::PreservedAnalyses::none()
                       : llvm::PreservedAnalyses::all();    
    }
    
}
