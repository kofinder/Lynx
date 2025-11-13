#include <passes/DevirtualizePass.hpp>
#include "llvm/IR/Verifier.h"
#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

namespace LynxLTO {
    
    using namespace llvm;

    llvm::PreservedAnalyses DevirtualizePass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {
        llvm::errs() << "Running DevirtualizePass on module: " << "\n";

        bool Changed = false;

        for (auto &F : M) {
            if (F.isDeclaration()) continue;
    
            for (auto &BB : F) {
                for (auto InstIter = BB.begin(); InstIter != BB.end(); ) {
                    Instruction &I = *InstIter++;
    
                    if (auto *Call = llvm::dyn_cast<llvm::CallBase>(&I)) {
                        llvm::Function *Callee = Call->getCalledFunction();
    
                        // Indirect call detected
                        if (!Callee) {
                            llvm::errs() << "[Devirt] Indirect call in function "
                                         << F.getName() << ": ";
                            Call->print(llvm::errs());
                            llvm::errs() << "\n";
    
                            // Attempt simple devirtualization via global initializer
                            if (auto *GV = llvm::dyn_cast<llvm::GlobalVariable>(
                                    Call->getCalledOperand()->stripPointerCasts())) {
                                if (GV->hasInitializer()) {
                                    if (auto *Fn = llvm::dyn_cast<llvm::Function>(
                                            GV->getInitializer()->stripPointerCasts())) {
    
                                        llvm::IRBuilder<> Builder(Call);
                                        llvm::SmallVector<llvm::Value*, 4> Args(Call->args());
    
                                        llvm::CallInst *NewCall = Builder.CreateCall(
                                            Fn->getFunctionType(), Fn, Args);
    
                                        // Preserve calling convention
                                        NewCall->setCallingConv(Call->getCallingConv());
    
                                        // Replace old call with new direct call
                                        Call->replaceAllUsesWith(NewCall);
                                        Call->eraseFromParent();
                                        Changed = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    
        return Changed ? llvm::PreservedAnalyses::none()
                       : llvm::PreservedAnalyses::all();    
    }
}