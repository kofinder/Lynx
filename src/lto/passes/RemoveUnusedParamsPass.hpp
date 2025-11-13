
/**
 * @file RemoveUnusedParamsPass.hpp
 * @brief Defines a pass to eliminate unused function parameters in LLVM functions.
 *
 * The `RemoveUnusedParamsPass` class leverages LLVM's new pass manager and
 * `PassInfoMixin` to analyze functions and remove parameters that are never used.
 * This optimization helps reduce function signature complexity, improves code
 * efficiency, and enables further compiler optimizations.
 *
 * **Key Responsibilities:**
 * - Identify unused function parameters within LLVM functions.
 * - Remove or mark parameters that are not referenced.
 * - Preserve analyses that remain valid after the pass.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_REMOVE_UNUSED_PARAMS_PASS_HPP
#define LYNX_REMOVE_UNUSED_PARAMS_PASS_HPP

#include <llvm/IR/PassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>

namespace LynxLTO {

    struct RemoveUnusedParamsPass : public llvm::PassInfoMixin<RemoveUnusedParamsPass> {
        llvm::PreservedAnalyses run(llvm::Function& F, llvm::FunctionAnalysisManager& FAM);
    };

}

#endif
