/**
 * @file FunctionDocDumperPass.hpp
 * @brief Defines a pass to extract and dump function documentation from LLVM modules.
 *
 * The `FunctionDocDumperPass` class uses LLVM's new pass manager and `PassInfoMixin`
 * to traverse functions in a module and collect or output associated documentation,
 * comments, or metadata for analysis or tooling purposes.
 *
 * **Key Responsibilities:**
 * - Traverse all functions within a module.
 * - Extract and output relevant documentation or metadata.
 * - Preserve analyses that remain valid after the pass.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_FUNCTION_DOC_DUMPER_PASS_HPP
#define LYNX_FUNCTION_DOC_DUMPER_PASS_HPP

#include "llvm/IR/PassManager.h"

namespace LynxLTO {

    struct FunctionDocDumperPass : public llvm::PassInfoMixin<FunctionDocDumperPass> {
        llvm::PreservedAnalyses run(llvm::Module& M, llvm::ModuleAnalysisManager& MAM);
    };

}

#endif
