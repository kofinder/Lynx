#include <passes/RequireAnalysisPass.hpp>
#include "llvm/IR/Verifier.h"
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

namespace LynxLTO {

    llvm::PreservedAnalyses RequireAnalysisPass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {
        llvm::errs() << "Running RequireAnalysisPass on module: " << "\n";
        return llvm::PreservedAnalyses::none();

        // bool requirementsSatisfied = true;

        // // Example requirement: module must contain a function named "main"
        // if (!M.getFunction("main")) {
        //     llvm::errs() << "[Error] Module is missing required function 'main'\n";
        //     requirementsSatisfied = false;
        // }
    
        // // Example requirement: module must have at least one global variable
        // if (M.global_empty()) {
        //     llvm::errs() << "[Warning] Module contains no global variables\n";
        // }
    
        // // Custom checks can be added here (attributes, metadata, security constraints, etc.)
        // for (auto &F : M) {
        //     if (F.isDeclaration()) continue;
    
        //     // Example: check function attributes
        //     if (!F.hasFnAttribute("alwaysinline")) {
        //         llvm::errs() << "[Note] Function '" << F.getName()
        //                      << "' is not marked as alwaysinline\n";
        //     }
        // }
    
        // if (!requirementsSatisfied) {
        //     llvm::errs() << "[Lynx] Module failed required analysis checks\n";
        // } else {
        //     llvm::errs() << "[Lynx] Module satisfies all required checks\n";
        // }
    
        //return llvm::PreservedAnalyses::all(); // read-only
    
    }
}