/**
 * @file TargetIRVerifierPass.hpp
 * @brief Defines a pass for verifying LLVM IR correctness for a specific target.
 *
 * The `TargetIRVerifierPass` class uses LLVM's new pass manager and `PassInfoMixin`
 * to ensure that LLVM modules and functions are valid and conform to target-specific
 * constraints. It provides detailed verification for both module-level and
 * function-level IR.
 *
 * **Key Responsibilities:**
 * - Verify LLVM modules and functions for target-specific correctness.
 * - Detect inconsistencies or invalid IR constructs.
 * - Preserve analyses that remain valid after verification.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/



#ifndef LYNX_TARGET_IR_VERIFIER_PASS_HPP
#define LYNX_TARGET_IR_VERIFIER_PASS_HPP

#include "llvm/IR/PassManager.h"


namespace LynxLTO {

    class TargetIRVerifierPass : public llvm::PassInfoMixin<TargetIRVerifierPass> {

        private:

            bool verifyModuleForTarget(llvm::Module &M);

            bool verifyFunctionForTarget(llvm::Function &F);

        public:

            llvm::PreservedAnalyses run(llvm::Module& M, llvm::ModuleAnalysisManager& MAM);
    };

}

#endif
