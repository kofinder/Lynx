#include <passes/FunctionSignatureAuditPass.hpp>
#include "llvm/IR/Verifier.h"
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

namespace LynxLTO {

    llvm::PreservedAnalyses FunctionSignatureAuditPass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {
        llvm::errs() << "Running FunctionSignatureAudit on module: "  << "\n";
        
        for (auto &F : M) {
            if (F.isDeclaration()) continue;
    
            llvm::errs() << "Auditing function: " << F.getName() << "\n";
    
            // Print return type
            llvm::errs() << "  Return Type: ";
            F.getReturnType()->print(llvm::errs());
            llvm::errs() << "\n";
    
            // Print arguments
            llvm::errs() << "  Arguments (" << F.arg_size() << "):\n";
            for (auto &Arg : F.args()) {
                llvm::errs() << "    " << Arg.getName() << " : ";
                Arg.getType()->print(llvm::errs());
                llvm::errs() << "\n";
            }
    
            // Check calling convention
            llvm::errs() << "  Calling Conv: " << F.getCallingConv() << "\n";
    
            // Example audit checks (expandable)
            if (F.arg_size() > 10) {
                llvm::errs() << "  [Warning] Function has more than 10 arguments!\n";
            }
            if (F.getReturnType()->isPointerTy()) {
                llvm::errs() << "  [Note] Function returns a pointer type.\n";
            }
    
            llvm::errs() << "\n";
        }
    
        return llvm::PreservedAnalyses::all(); // read-only    
    }
    
}
