/**
 * @file RequireAnalysisPass.hpp
 * @brief Defines a pass to enforce or verify module-level analysis requirements.
 *
 * The `RequireAnalysisPass` class uses LLVM's new pass manager and `PassInfoMixin`
 * to ensure that specific analyses are performed or available for a module. It
 * can be used to validate dependencies between passes and maintain correctness
 * in the optimization pipeline.
 *
 * **Key Responsibilities:**
 * - Verify the presence of required analyses for a module.
 * - Ensure correctness and consistency in pass dependencies.
 * - Preserve analyses that remain valid after the pass.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_REQURIE_ANALYSIS_PASS_HPP
#define LYNX_REQURIE_ANALYSIS_PASS_HPP

#include "llvm/IR/PassManager.h"

namespace LynxLTO {

    struct RequireAnalysisPass : public llvm::PassInfoMixin<RequireAnalysisPass> {
        llvm::PreservedAnalyses run(llvm::Module& M, llvm::ModuleAnalysisManager& MAM);
    };

}

#endif
