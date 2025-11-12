/**
 * @file PointerCaptureAnalysisPass.hpp
 * @brief Declares a pass for analyzing pointer capture behavior in LLVM modules.
 *
 * The `PointerCaptureAnalysisPass` class uses LLVM’s new pass manager
 * and `PassInfoMixin` to detect whether pointer values (especially function
 * arguments or globals) are captured within a module. This analysis helps
 * guide optimizations such as alias analysis, escape analysis, and
 * interprocedural optimization during LTO.
 *
 * **Key Responsibilities:**
 * - Traverse functions in a module to check for captured pointers.
 * - Report whether each pointer is captured or remains local.
 * - Preserve analyses that remain valid after running the pass.
 *
 * @author Ko Thein (Nathan Mratt)
 * @date November 2, 2024
*/

#ifndef LYNX_POINTER_CAPTURE_ANALYSIS_PASS_HPP
#define LYNX_POINTER_CAPTURE_ANALYSIS_PASS_HPP

#include "llvm/IR/PassManager.h"


namespace LynxLTO {

    class PointerCaptureAnalysisPass : public llvm::PassInfoMixin<PointerCaptureAnalysisPass> {
        
        public:

            llvm::PreservedAnalyses run(llvm::Module& M, llvm::ModuleAnalysisManager& MAM);

    };

}

#endif
