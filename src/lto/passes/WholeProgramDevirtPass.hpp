/**
 * @file AttributorPass.hpp
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
 * @date: November 2, 2024
*/

#ifndef LYNX_ATTRIBUTOR_PASS_HPP
#define LYNX_ATTRIBUTOR_PASS_HPP

#include "llvm/IR/PassManager.h"

namespace LynxLTO {

    class WholeProgramDevirtPass : public llvm::PassInfoMixin<WholeProgramDevirtPass> {

        public:

            llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager& MAM);
    };

}

#endif
