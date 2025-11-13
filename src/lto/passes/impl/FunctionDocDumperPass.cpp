#include <passes/FunctionDocDumperPass.hpp>
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/raw_ostream.h"

namespace LynxLTO {

    using namespace llvm;

    llvm::PreservedAnalyses FunctionDocDumperPass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {
        llvm::errs() << "Running FunctionDocDumperPass on module: " << "\n";
        
        for (auto &F : M) {
            if (F.isDeclaration()) continue;
    
            llvm::errs() << "Function: " << F.getName() << "\n";
            llvm::errs() << "  Return Type: ";
            F.getReturnType()->print(llvm::errs());
            llvm::errs() << "\n";
    
            llvm::errs() << "  Arguments (" << F.arg_size() << "):\n";
            for (auto &Arg : F.args()) {
                llvm::errs() << "    " << Arg.getName() << " : ";
                Arg.getType()->print(llvm::errs());
                llvm::errs() << "\n";
            }
    
            if (F.hasFnAttribute(llvm::Attribute::ReadOnly))
                llvm::errs() << "  Attribute: readonly\n";
            if (F.hasFnAttribute(llvm::Attribute::NoInline))
                llvm::errs() << "  Attribute: noinline\n";
    
            llvm::errs() << "\n";
        }
    
        return llvm::PreservedAnalyses::all(); // Read-only pass    
    }
    
}

