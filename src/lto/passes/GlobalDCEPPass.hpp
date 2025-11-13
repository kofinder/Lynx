/**
 * @file GlobalDCEPPass.hpp
 * @brief Defines a pass for performing global dead code elimination in LLVM modules.
 *
 * The `GlobalDCEPPass` class uses LLVM's new pass manager and `PassInfoMixin`
 * to identify and remove unused global variables, functions, and other module-level
 * elements. This optimization reduces code size and improves performance.
 *
 * **Key Responsibilities:**
 * - Detect unused globals and functions in a module.
 * - Eliminate dead code to reduce module size.
 * - Preserve analyses that remain valid after the pass.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_GLOBAL_DCEP_PASS_HPP
#define LYNX_GLOBAL_DCEP_PASS_HPP

#include "llvm/IR/PassManager.h"

namespace LynxLTO {

    struct GlobalDCEPPass : public llvm::PassInfoMixin<GlobalDCEPPass> {
        llvm::PreservedAnalyses run(llvm::Module& M, llvm::ModuleAnalysisManager& MAM);
    };

}

#endif
