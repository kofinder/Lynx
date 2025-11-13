#include <passes/DevirtualizeFunctionCallsPass.hpp>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include "llvm/IR/Function.h"
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

namespace LynxLTO {

    using namespace llvm;

    llvm::PreservedAnalyses DevirtualizeFunctionCallsPass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {
        llvm::errs() << "Running DevirtualizeFunctionCallsPass on module: " << "\n";

        bool Changed = false;

        for (auto &F : M) {
            if (F.isDeclaration()) continue;
    
            for (auto &BB : F) {
                for (auto InstIter = BB.begin(); InstIter != BB.end(); ) {
                    Instruction &I = *InstIter++;
    
                    if (auto* Call = llvm::dyn_cast<llvm::CallBase>(&I)) {
                        llvm::Function* Callee = Call->getCalledFunction();
    
                        if (!Callee) {
                            // Indirect call detected
                            llvm::errs() << "[Devirt] Indirect call in function "  << F.getName() << ": ";
                            Call->print(llvm::errs());
                            llvm::errs() << "\n";
                            // If the callee is a known function pointer global, replace it
                            if (auto *GV = llvm::dyn_cast<llvm::GlobalVariable>(Call->getCalledOperand()->stripPointerCasts())) {
                                if (GV->hasInitializer()) {
                                    if (auto *Fn = llvm::dyn_cast<llvm::Function>(GV->getInitializer()->stripPointerCasts())) {
                                        // Replace the indirect call with a direct call
                                        llvm::IRBuilder<> Builder(Call);
                                        llvm::SmallVector<llvm::Value*, 4> Args(Call->args());
                                        llvm::CallBase* newCall = Builder.CreateCall(Fn->getFunctionType(), Fn, Args);
                                        newCall->setCallingConv(Call->getCallingConv());
                                        Call->replaceAllUsesWith(newCall);
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

