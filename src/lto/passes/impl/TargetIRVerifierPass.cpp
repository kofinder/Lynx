#include <passes/TargetIRVerifierPass.hpp>
#include "llvm/IR/Verifier.h"
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>


namespace LynxLTO {

    bool TargetIRVerifierPass::verifyModuleForTarget(llvm::Module &M) {
        bool Valid = true;
    
        for (auto &F : M) {
            if (F.isDeclaration()) continue;
            if (!verifyFunctionForTarget(F))
                Valid = false;
        }

        for (auto &GV : M.globals()) {
            llvm::Type *Ty = GV.getType();
            if (Ty->isPointerTy()) {
                llvm::errs() << "[Note] Global pointer (cannot inspect element type in opaque mode): "
                             << GV.getName() << "\n";
                Valid = false;
            }
        }
        
        return Valid;
    }
    
    bool TargetIRVerifierPass::verifyFunctionForTarget(llvm::Function &F) {
        bool Valid = true;
    
        // Example: disallow functions returning floating point types for this target
        if (F.getReturnType()->isFloatingPointTy()) {
            llvm::errs() << "[Error] Function " << F.getName()
                         << " returns a floating-point type, which is unsupported for target\n";
            Valid = false;
        }
    
        // Example: disallow vararg functions
        if (F.isVarArg()) {
            llvm::errs() << "[Error] Function " << F.getName()
                         << " is vararg, which is unsupported for target\n";
            Valid = false;
        }
    
        return Valid;
    }
    

    llvm::PreservedAnalyses TargetIRVerifierPass::run(
        llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {
    
        llvm::errs() << "[Lynx] Running TargetIRVerifierPass on module: " << M.getName() << "\n";
    
        bool Valid = verifyModuleForTarget(M);
    
        if (!Valid) {
            llvm::errs() << "[Lynx] Target IR verification failed. Module contains invalid constructs.\n";
        } else {
            llvm::errs() << "[Lynx] Target IR verification passed.\n";
        }
    
        return llvm::PreservedAnalyses::all(); // read-only
    }    

}
