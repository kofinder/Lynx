/**
 * @file DevirtualizeFunctionCallsPass.hpp
 * @brief Defines a pass to devirtualize function calls in LLVM modules.
 *
 * The `DevirtualizeFunctionCallsPass` class uses LLVM's new pass manager and
 * `PassInfoMixin` to transform indirect (virtual) calls into direct calls
 * when possible. This optimization improves runtime performance by enabling
 * further inlining and reducing dynamic dispatch overhead.
 *
 * **Key Responsibilities:**
 * - Identify virtual or indirect function calls.
 * - Replace eligible calls with direct calls.
 * - Preserve analyses that remain valid after the pass.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_DEVIRTUALIZE_FUNCTION_CALLS_PASS_HPP
#define LYNX_DEVIRTUALIZE_FUNCTION_CALLS_PASS_HPP

#include <llvm/IR/PassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>

namespace LynxLTO {

    struct DevirtualizeFunctionCallsPass : public llvm::PassInfoMixin<DevirtualizeFunctionCallsPass> {
        llvm::PreservedAnalyses run(llvm::Module& M, llvm::ModuleAnalysisManager& MAM);
    };

}

#endif
