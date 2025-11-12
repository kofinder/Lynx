#include <passes/FunctionSecurityPass.hpp>

namespace LynxLTO {
    llvm::PreservedAnalyses FunctionSecurityPass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {
        llvm::errs() << "Running FunctionSecurityPass on module: " << "\n";
    
        // Iterate functions and dump documentation metadata or other info here
    
        return llvm::PreservedAnalyses::all();
    }
    
}
