/**
 * @file FunctionInliningPass.hpp
 * @brief Defines a pass to perform function inlining in LLVM modules.
 *
 * The `FunctionInliningPass` class leverages LLVM's new pass manager and
 * `PassInfoMixin` to inline eligible function calls within a module. This
 * optimization reduces call overhead and can improve performance by enabling
 * further compiler optimizations.
 *
 * **Key Responsibilities:**
 * - Identify functions suitable for inlining.
 * - Replace call sites with function bodies where applicable.
 * - Preserve analyses that remain valid after the pass.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
 */

#ifndef LYNX_FUNCTION_INLINING_PASS_HPP
#define LYNX_FUNCTION_INLINING_PASS_HPP

#include <llvm/IR/PassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>

namespace LynxLTO {

    struct FunctionInliningPass : public llvm::PassInfoMixin<FunctionInliningPass> {
        llvm::PreservedAnalyses run(llvm::Module& M, llvm::ModuleAnalysisManager& MAM);
    };

}

#endif
