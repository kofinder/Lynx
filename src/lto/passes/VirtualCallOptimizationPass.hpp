/**
 * @file VirtualCallOptimizationPass.hpp
 * @brief Defines a pass for optimizing virtual function calls in LLVM modules.
 *
 * The `VirtualCallOptimizationPass` class leverages LLVM's new pass manager and
 * `PassInfoMixin` to analyze virtual calls within a module and replace them with
 * optimized alternatives where possible, improving runtime performance by
 * reducing indirection and enabling further compiler optimizations.
 *
 * **Key Responsibilities:**
 * - Identify virtual calls across the module.
 * - Replace eligible virtual calls with direct calls.
 * - Preserve analyses that remain valid after optimization.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/


#ifndef LYNX_VIRTUAL_CALL_OPTIMIZATION_PASS_HPP
#define LYNX_VIRTUAL_CALL_OPTIMIZATION_PASS_HPP

#include <llvm/IR/PassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>

namespace LynxLTO {

    struct VirtualCallOptimizationPass : public llvm::PassInfoMixin<VirtualCallOptimizationPass> {
        llvm::PreservedAnalyses run(llvm::Module& M, llvm::ModuleAnalysisManager& MAM);
    };

}

#endif
