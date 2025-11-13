/**
 * @file AttributorPass.hpp
 * @brief Defines a pass to apply and propagate LLVM IR attributes for optimization.
 *
 * The `AttributorPass` class leverages LLVM's new pass manager and `PassInfoMixin`
 * to analyze and annotate functions, global variables, and instructions with
 * attributes that enable better optimization and code generation.
 *
 * **Key Responsibilities:**
 * - Apply LLVM IR attributes to functions and variables.
 * - Propagate inferred attributes to improve optimization opportunities.
 * - Preserve analyses that remain valid after the pass.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_ATTRIBUTOR_PASS_HPP
#define LYNX_ATTRIBUTOR_PASS_HPP

#include "llvm/IR/PassManager.h"


namespace LynxLTO {

    struct AttributorPass : public llvm::PassInfoMixin<AttributorPass> {
        llvm::PreservedAnalyses run(llvm::Module& M, llvm::ModuleAnalysisManager& MAM);
    };

}

#endif
