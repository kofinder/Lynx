/**
 * @file WholeProgramDevirtPass.hpp
 * @brief Defines a whole-program devirtualization pass using LLVM's new pass manager.
 *
 * The `WholeProgramDevirtPass` class provides a pass for devirtualizing virtual
 * calls across an entire LLVM module. It leverages LLVM's PassInfoMixin and
 * ModuleAnalysisManager to perform optimizations that improve runtime performance
 * by reducing indirection in virtual function calls.
 *
 * **Key Responsibilities:**
 * - Analyze module-wide virtual calls.
 * - Replace virtual calls with direct calls where possible.
 * - Preserve analysis results that remain valid after the pass.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_WHOLE_PROGRAM_DEVIRT_PASS_HPP
#define LYNX_WHOLE_PROGRAM_DEVIRT_PASS_HPP

#include <llvm/IR/PassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>

namespace LynxLTO {

    struct WholeProgramDevirtPass : public llvm::PassInfoMixin<WholeProgramDevirtPass> {
        llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager& MAM);
    };

}

#endif
