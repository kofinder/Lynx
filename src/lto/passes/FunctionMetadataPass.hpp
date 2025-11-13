/**
 * @file FunctionMetadataPass.hpp
 * @brief Defines a pass to attach or analyze metadata for functions in LLVM modules.
 *
 * The `FunctionMetadataPass` class uses LLVM's new pass manager and `PassInfoMixin`
 * to manage function-level metadata, which can be used for optimization,
 * instrumentation, debugging, or code analysis purposes.
 *
 * **Key Responsibilities:**
 * - Attach metadata to functions for optimization or analysis.
 * - Inspect existing function metadata.
 * - Preserve analyses that remain valid after the pass.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_FUNCTION_METADATA_PASS_HPP
#define LYNX_FUNCTION_METADATA_PASS_HPP

#include "llvm/IR/PassManager.h"

namespace LynxLTO {

    struct FunctionMetadataPass : public llvm::PassInfoMixin<FunctionMetadataPass> {
        llvm::PreservedAnalyses run(llvm::Module& M, llvm::ModuleAnalysisManager& MAM);
    };

}

#endif 

