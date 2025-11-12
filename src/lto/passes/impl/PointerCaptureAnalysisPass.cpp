#include "PointerCaptureAnalysisPass.hpp"
#include "PointerCaptureAnalysisPass.hpp"
#include "llvm/Analysis/CaptureTracking.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/raw_ostream.h"


using namespace llvm;

namespace LynxLTO {

    llvm::PreservedAnalyses PointerCaptureAnalysisPass::run(llvm::Module& M, llvm::ModuleAnalysisManager& MAM) {
        errs() << "=== Pointer Capture Analysis ===\n";
        errs() << "Analyzing module: " << M.getName() << "\n\n";
        for (Function &F : M) {
            // Skip declarations (no body)
            if (F.isDeclaration())
                continue;
    
            errs() << "Function: " << F.getName() << "\n";
    
            for (Argument &Arg : F.args()) {
                if (!Arg.getType()->isPointerTy())
                    continue;
    
                bool Captured = PointerMayBeCaptured(&Arg,
                                                     /*ReturnCaptures=*/true,
                                                     /*StoreCaptures=*/true);
    
                errs() << "  Argument %" << Arg.getName() << " : "
                       << (Captured ? "Captured" : "Not Captured") << "\n";
            }
    
            errs() << "\n";
        }
    
        errs() << "=== End of Pointer Capture Analysis ===\n\n";
    
        // This is a pure analysis — it doesn’t modify the IR.
        return PreservedAnalyses::all();    
    }
}
