/**
 * @file UndefinedBehaviorPass.hpp
 * @brief Defines a pass for detecting and handling potential undefined behavior in LLVM modules.
 *
 * The `UndenfinedBehaviorPass` class uses LLVM's new pass manager and `PassInfoMixin`
 * to analyze a module for constructs that may lead to undefined behavior. It can
 * be used to improve code safety, assist in optimization, and provide diagnostic
 * information during compilation.
 *
 * **Key Responsibilities:**
 * - Analyze LLVM modules for patterns that may cause undefined behavior.
 * - Enable compiler optimizations by identifying safe assumptions.
 * - Preserve analyses that remain valid after the pass.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_UNDEFINED_BEHAVIOR_PASS_HPP
#define LYNX_UNDEFINED_BEHAVIOR_PASS_HPP

#include "llvm/IR/PassManager.h"

namespace LynxLTO {

    struct UndenfinedBehaviorPass : public llvm::PassInfoMixin<UndenfinedBehaviorPass> {
        llvm::PreservedAnalyses run(llvm::Module& M, llvm::ModuleAnalysisManager& MAM);
    };

}


#endif 
