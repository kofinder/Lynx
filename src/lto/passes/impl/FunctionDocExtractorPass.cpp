#include <passes/FunctionDocExtractorPass.hpp>
#include "llvm/IR/Verifier.h"
#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

namespace LynxLTO {
  
    llvm::PreservedAnalyses FunctionDocExtractorPass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {
        llvm::errs() << "Running FunctionDocExtractorPass on module: " << "\n";
        
        funcInfo.clear();

        for (auto &F : M) {
            if (F.isDeclaration()) continue;
    
            FunctionInfo FI;
            FI.Name = F.getName().str();
    
            std::string RetTypeStr;
            llvm::raw_string_ostream RetStream(RetTypeStr);
            F.getReturnType()->print(RetStream);
            FI.ReturnType = RetStream.str();
    
            for (auto &Arg : F.args()) {
                std::string ArgTypeStr;
                llvm::raw_string_ostream ArgStream(ArgTypeStr);
                Arg.getType()->print(ArgStream);
                FI.Args.emplace_back(Arg.getName().str(), ArgStream.str());
            }
    
            if (F.hasFnAttribute(llvm::Attribute::ReadOnly)) {
                FI.Attributes.push_back("readonly");
            }
                
            if (F.hasFnAttribute(llvm::Attribute::NoInline)) {
                FI.Attributes.push_back("noinline");
            }
                
    
            funcInfo.push_back(std::move(FI));
        }

        return llvm::PreservedAnalyses::all();
    }
    
}
