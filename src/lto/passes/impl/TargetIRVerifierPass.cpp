#include <passes/TargetIRVerifierPass.hpp>
#include "llvm/IR/Verifier.h"
#include "llvm/IR/CallSite.h"
#include "llvm/Support/raw_ostream.h"


namespace LynxLTO {

    llvm::PreservedAnalyses TargetIRVerifierPass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {
        bool hasErrors = false;

        // Step 1: Run standard LLVM verification (basic IR correctness)
        if (llvm::verifyModule(M, &llvm::errs())) {
            llvm::errs() << "[TargetIRVerifierPass] Standard LLVM verification failed\n";
            hasErrors = true;
        }

        // Step 2: Run your target-specific IR checks
        if (!verifyModuleForTarget(M)) {
            llvm::errs() << "[TargetIRVerifierPass] Target-specific verification failed\n";
            hasErrors = true;
        }

        // Return preserved analyses if no errors, else none
        return hasErrors ? llvm::PreservedAnalyses::none() : llvm::PreservedAnalyses::all();
    }

    bool TargetIRVerifierPass::verifyModuleForTarget(llvm::Module &M) {
        bool allOk = true;

        for (llvm::Function &F : M) {
            if (!verifyFunctionForTarget(F)) {
               llvm::errs() << "[TargetIRVerifierPass] Function '" << F.getName() << "' failed target verification\n";
                allOk = false;
            }
        }

        // Add more module-level checks here if needed

        return allOk;
    }

    bool TargetIRVerifierPass::verifyFunctionForTarget(llvm::Function &F) {
        // Example target-specific checks:

        // 1. Disallow calls to unsupported intrinsics (customize per target)
        for (auto &BB : F) {
            for (auto &I : BB) {
                if (auto *callInst = dyn_cast<llvm::CallBase>(&I)) {
                    llvm::Function *calledFunc = callInst->getCalledFunction();
                    if (calledFunc && calledFunc->isIntrinsic()) {
                        unsigned intrinsicID = calledFunc->getIntrinsicID();
                        // Example: disallow some intrinsic (replace with your target logic)
                        // if (intrinsicID == llvm::Intrinsic::x86_sse_add_ps) {
                        //     llvm::errs() << "[TargetIRVerifierPass] Disallowed SSE intrinsic found\n";
                        //     return false;
                        // }
                    }
                }
            }
        }

        // 2. Enforce calling convention (example: only C calling convention allowed)
        if (F.getCallingConv() != llvm::CallingConv::C) {
            llvm::errs() << "[TargetIRVerifierPass] Function '" << F.getName() << "' uses disallowed calling convention\n";
            return false;
        }

        // 3. Check for unsupported types (example: no floating point on embedded)
        for (auto &BB : F) {
            for (auto &I : BB) {
                for (unsigned i = 0; i < I.getNumOperands(); ++i) {
                    llvm::Type *operandType = I.getOperand(i)->getType();
                    if (operandType->isFloatingPointTy()) {
                        llvm::errs() << "[TargetIRVerifierPass] Floating point type detected in function '" << F.getName() << "'\n";
                        return false;
                    }
                }
            }
        }

        // Add more target-specific checks here...

        return true;
    }

}
