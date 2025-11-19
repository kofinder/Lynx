/**
 * @file FunctionSecurityPass.hpp
 * @brief Defines a pass to enforce security checks on functions in LLVM modules.
 *
 * The `FunctionSecurityPass` class uses LLVM's new pass manager and `PassInfoMixin`
 * to analyze functions for potential security vulnerabilities. It can help detect
 * unsafe patterns, enforce constraints, and prepare the module for secure execution.
 *
 * **Key Responsibilities:**
 * - Inspect functions for security-related issues.
 * - Enforce safe coding and calling patterns.
 * - Preserve analyses that remain valid after the pass.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_FUNCTION_SECURITY_PASS_HPP
#define LYNX_FUNCTION_SECURITY_PASS_HPP

#include <llvm/IR/PassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>

namespace LynxLTO {

    struct FunctionSecurityPass : public llvm::PassInfoMixin<FunctionSecurityPass> {
        llvm::PreservedAnalyses run(llvm::Module& M, llvm::ModuleAnalysisManager& MAM);
    };

}

#endif 

