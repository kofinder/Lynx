/**
 * @file FunctionSignatureAuditPass.hpp
 * @brief Defines a pass to audit and validate function signatures in LLVM modules.
 *
 * The `FunctionSignatureAuditPass` class leverages LLVM's new pass manager and
 * `PassInfoMixin` to inspect function signatures within a module, ensuring
 * correctness, consistency, and adherence to expected calling conventions.
 *
 * **Key Responsibilities:**
 * - Analyze function signatures for consistency and correctness.
 * - Detect signature mismatches or potential issues.
 * - Preserve analyses that remain valid after auditing.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_FUNCTION_SIGNATURE_AUDIT_PASS_HPP
#define LYNX_FUNCTION_SIGNATURE_AUDIT_PASS_HPP

#include <llvm/IR/PassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>

namespace LynxLTO {

    struct FunctionSignatureAuditPass : public llvm::PassInfoMixin<FunctionSignatureAuditPass> {
        llvm::PreservedAnalyses run(llvm::Module& M, llvm::ModuleAnalysisManager& MAM);
    };

}

#endif 

