#include <passes/FunctionSignatureAuditPass.hpp>

namespace LynxLTO {

    llvm::PreservedAnalyses FunctionSignatureAuditPass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {
        llvm::errs() << "Running FunctionSignatureAudit on module: "  << "\n";
    
        // Iterate functions and dump documentation metadata or other info here
    
        return llvm::PreservedAnalyses::all();
    }
    
}
