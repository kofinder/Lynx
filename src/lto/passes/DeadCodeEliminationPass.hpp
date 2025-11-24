/**
 * @file DeadCodeEliminationPass.hpp
 * @brief Defines a pass to remove dead or unreachable code in LLVM functions.
 *
 * The `DeadCodeEliminationPass` class leverages LLVM's new pass manager and
 * `PassInfoMixin` to analyze function instructions and eliminate those that
 * are never executed or have no observable effect. This optimization reduces
 * code size and improves performance.
 *
 * **Key Responsibilities:**
 * - Detect instructions that do not affect program behavior.
 * - Safely remove dead or unreachable code.
 * - Preserve analyses that remain valid after the pass.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_DEADCODE_ELIMINATION_PASS_HPP
#define LYNX_DEADCODE_ELIMINATION_PASS_HPP

#include <llvm/IR/PassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>

namespace LynxLTO {

    struct DeadCodeEliminationPass : public llvm::PassInfoMixin<DeadCodeEliminationPass> {
        llvm::PreservedAnalyses run(llvm::Function& F, llvm::FunctionAnalysisManager& FAM);
    };

}

#endif
