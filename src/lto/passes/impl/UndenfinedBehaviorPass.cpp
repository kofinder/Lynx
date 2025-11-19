#include <passes/UndenfinedBehaviorPass.hpp>
#include "llvm/IR/Verifier.h"
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

namespace LynxLTO {

    using namespace llvm;
    
    llvm::PreservedAnalyses UndenfinedBehaviorPass::run(llvm::Module& M, llvm::ModuleAnalysisManager& MAM) {
        llvm::errs() << "[Lynx] Running UndefinedBehaviorPass on module: " << M.getName() << "\n";

        bool HasUB = false;

        for (auto &F : M) {
            if (F.isDeclaration()) continue;
    
            for (auto &BB : F) {
                for (auto &I : BB) {
    
                    // Check division by zero (integer division)
                    if (auto *BinOp = llvm::dyn_cast<llvm::BinaryOperator>(&I)) {
                        if (BinOp->getOpcode() == llvm::Instruction::SDiv ||
                            BinOp->getOpcode() == llvm::Instruction::UDiv) {
    
                            if (auto *ConstRHS = llvm::dyn_cast<llvm::ConstantInt>(BinOp->getOperand(1))) {
                                if (ConstRHS->isZero()) {
                                    llvm::errs() << "[UB] Division by zero in function " << F.getName() << "\n";
                                    HasUB = true;
                                }
                            }
                        }
                    }
    
                    // Check unreachable instructions
                    if (llvm::isa<llvm::UnreachableInst>(&I)) {
                        llvm::errs() << "[UB] Unreachable instruction in function " << F.getName() << "\n";
                        HasUB = true;
                    }
    
                    // Additional UB checks can be added here:
                    // - Poison/undef values
                    // - Misaligned loads/stores
                    // - Out-of-bounds GEPs
                }
            }
        }
    
        if (!HasUB) {
            llvm::errs() << "[Lynx] No obvious undefined behavior detected\n";
        }
    
        return llvm::PreservedAnalyses::all(); // read-only pass
    }

}