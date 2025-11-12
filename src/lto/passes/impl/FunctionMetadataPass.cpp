#include <passes/FunctionMetadataPass.hpp>

namespace LynxLTO {
    llvm::PreservedAnalyses FunctionMetadataPass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {
        llvm::errs() << "Running FunctionMetadataPass on module: " << "\n";
    
        // Iterate functions and dump documentation metadata or other info here
    
        return llvm::PreservedAnalyses::all();
    }
    
}
