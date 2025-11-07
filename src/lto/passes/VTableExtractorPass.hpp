/**
 * @file VTableExtractorPass.hpp
 * @brief Defines a pass for extracting virtual table (vtable) information from LLVM modules.
 *
 * The `VTableExtractorPass` class uses LLVM's new pass manager and `PassInfoMixin`
 * to analyze and extract vtable layouts from classes in a module. This information
 * can be used for optimizations, code generation, or runtime support for dynamic dispatch.
 *
 * **Key Responsibilities:**
 * - Traverse LLVM modules to identify class vtables.
 * - Extract and organize vtable data for later use.
 * - Preserve analyses that remain valid after extraction.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_VTABLE_EXTRACTOR_PASS_HPP
#define LYNX_VTABLE_EXTRACTOR_PASS_HPP

#include "llvm/IR/PassManager.h"


namespace LynxLTO {

    class VTableExtractorPass : public llvm::PassInfoMixin<VTableExtractorPass> {

        public:

            llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager& MAM);
    };

}

#endif 

