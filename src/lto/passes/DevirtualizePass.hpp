/**
 * @file DevirtualizePass.hpp
 * @brief Defines a pass to perform module-level devirtualization in LLVM.
 *
 * The `DevirtualizePass` class leverages LLVM's new pass manager and
 * `PassInfoMixin` to transform virtual or indirect calls into direct calls
 * across an entire module when possible. This optimization enables inlining,
 * reduces runtime dispatch overhead, and improves overall performance.
 *
 * **Key Responsibilities:**
 * - Analyze virtual or indirect function calls module-wide.
 * - Replace eligible calls with direct calls.
 * - Preserve analyses that remain valid after the pass.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/


#ifndef LYNX_DEVIRTUALIZE_PASS_HPP
#define LYNX_DEVIRTUALIZE_PASS_HPP

#include "llvm/IR/PassManager.h"

namespace LynxLTO {

    struct DevirtualizePass : public llvm::PassInfoMixin<DevirtualizePass> {
        llvm::PreservedAnalyses run(llvm::Module& M, llvm::ModuleAnalysisManager& MAM);
    };

}

#endif
