#include "PointerCaptureAnalysisPass.hpp"
#include "llvm/Analysis/CaptureTracking.h"
#include "llvm/IR/Verifier.h"
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>
#include <set>


using namespace llvm;

namespace LynxLTO {

    llvm::PreservedAnalyses PointerCaptureAnalysisPass::run(llvm::Module& M, llvm::ModuleAnalysisManager& MAM) {
        
        llvm::errs() << "[Lynx] Running PointerCaptureAnalysisPass using PointerMayBeCaptured on module: " << M.getName() << "\n";

        std::set<const llvm::Value*> escapedPointers;

        for (auto &F : M) {
            if (F.isDeclaration()) continue;
    
            for (auto &BB : F) {
                for (auto &I : BB) {
                    for (auto &Op : I.operands()) {
                        if (Op->getType()->isPointerTy()) {
                            if (llvm::PointerMayBeCaptured(Op, /*ReturnCaptures=*/true, /*StoreCaptures=*/true)) {
                                escapedPointers.insert(Op);
                                llvm::errs() << "[Escaped] Pointer in function " << F.getName() << ": ";
                                Op->print(llvm::errs());
                                llvm::errs() << "\n";
                            }
                        }
                    }
                }
            }
        }
    
        llvm::errs() << "[Lynx] Total escaped pointers: " << escapedPointers.size() << "\n";

        return llvm::PreservedAnalyses::all(); // read-only    
    }
}
