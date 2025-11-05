/**
 * @file AccessModifierPass.hpp
 * @brief Defines a pass to enforce and validate access modifiers in LLVM modules.
 *
 * The `AccessModifierPass` class uses LLVM's new pass manager and `PassInfoMixin`
 * to analyze class members and functions, ensuring that access specifiers
 * (e.g., public, private, protected) are correctly applied and enforced.
 *
 * **Key Responsibilities:**
 * - Inspect and validate access modifiers on classes, methods, and fields.
 * - Detect violations of visibility rules.
 * - Preserve analyses that remain valid after the pass.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_ACCESS_MODIFIER_PASS_HPP
#define LYNX_ACCESS_MODIFIER_PASS_HPP

#include "llvm/IR/PassManager.h"


namespace LynxLTO {

    class AccessModifierPass : public llvm::PassInfoMixin<AccessModifierPass> {

        private:

            static char ID;

        public:

            llvm::PreservedAnalyses run(llvm::Module& M, llvm::ModuleAnalysisManager& MAM);
    };

}

#endif
