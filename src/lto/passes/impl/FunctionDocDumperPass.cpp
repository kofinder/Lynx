#include <passes/FunctionDocDumperPass.hpp>
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

namespace LynxLTO {
    llvm::PreservedAnalyses FunctionDocDumperPass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {
        llvm::errs() << "Running FunctionDocDumperPass on module: " << M.getName() << "\n";
    
        // Iterate functions and dump documentation metadata or other info here
    
        return llvm::PreservedAnalyses::all();
    }
    
}

