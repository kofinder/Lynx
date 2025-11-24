/**
 * @file FunctionDocExtractorPass.hpp
 * @brief Defines a pass to extract function-level documentation from LLVM modules.
 *
 * The `FunctionDocExtractorPass` class leverages LLVM's new pass manager and
 * `PassInfoMixin` to analyze and extract documentation or metadata associated
 * with functions within a module. This can be used for tooling, reporting,
 * or automated documentation generation.
 *
 * **Key Responsibilities:**
 * - Traverse all functions in a module.
 * - Extract documentation, comments, or metadata.
 * - Preserve analyses that remain valid after the pass.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_FUNCTION_DOC_EXTRACTOR_PASS_HPP
#define LYNX_FUNCTION_DOC_EXTRACTOR_PASS_HPP

#include <llvm/IR/PassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <vector>
#include <string>

namespace LynxLTO {

    struct FunctionInfo {
        std::string Name;
        std::string ReturnType;
        std::vector<std::pair<std::string, std::string>> Args; // Name, Type
        std::vector<std::string> Attributes;
    };    

    struct FunctionDocExtractorPass : public llvm::PassInfoMixin<FunctionDocExtractorPass> {

        std::vector<FunctionInfo> funcInfo;

        llvm::PreservedAnalyses run(llvm::Module& M, llvm::ModuleAnalysisManager& MAM);

    };

}

#endif
