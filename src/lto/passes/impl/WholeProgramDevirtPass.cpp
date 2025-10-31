#include <passes/WholeProgramDevirtPass.hpp>

namespace LynxLTO {
    llvm::PreservedAnalyses WholeProgramDevirtPass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {
        llvm::errs() << "Running FunctionInliningPass on module: " << M.getName() << "\n";
    
        // Iterate functions and dump documentation metadata or other info here
    
        return llvm::PreservedAnalyses::all();
    }
    
}
