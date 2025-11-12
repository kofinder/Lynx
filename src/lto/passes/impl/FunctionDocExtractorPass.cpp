#include <passes/FunctionDocExtractorPass.hpp>

namespace LynxLTO {
    llvm::PreservedAnalyses FunctionDocExtractorPass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {
        llvm::errs() << "Running FunctionDocExtractorPass on module: " << "\n";
    
        // Iterate functions and dump documentation metadata or other info here
    
        return llvm::PreservedAnalyses::all();
    }
    
}
