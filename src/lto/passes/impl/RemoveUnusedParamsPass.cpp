#include <passes/RemoveUnusedParamsPass.hpp>
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Support/raw_ostream.h"
#include <vector>

using namespace llvm;

namespace LynxLTO {

    llvm::PreservedAnalyses RemoveUnusedParamsPass::run(Function &F, FunctionAnalysisManager &FAM) {
        errs() << "Running RemoveUnusedParamsPass on function: " << F.getName() << "\n";

        if (F.isDeclaration() || F.arg_empty()) return llvm::PreservedAnalyses::all();
        std::vector<llvm::Argument*> UsedArgs;
        for (auto &Arg : F.args()) {
            if (!Arg.use_empty())
                UsedArgs.push_back(&Arg);
        }
    
        // Nothing to remove
        if (UsedArgs.size() == F.arg_size())
            return llvm::PreservedAnalyses::all();
    
        llvm::errs() << "  Removing " << (F.arg_size() - UsedArgs.size())
                     << " unused arguments\n";
    
        // Create new function type
        std::vector<llvm::Type*> ParamTypes;
        for (auto *Arg : UsedArgs)
            ParamTypes.push_back(Arg->getType());
    
        llvm::FunctionType *NewFTy =
            llvm::FunctionType::get(F.getReturnType(), ParamTypes, F.isVarArg());
    
        llvm::Function *NewF = llvm::Function::Create(
            NewFTy, F.getLinkage(), F.getName() + ".trimmed", F.getParent());
    
        NewF->copyAttributesFrom(&F);
    
        // Map old arguments to new arguments
        llvm::ValueToValueMapTy VMap;
        auto NewArgIt = NewF->arg_begin();
        for (auto *Arg : UsedArgs) {
            NewArgIt->setName(Arg->getName());
            VMap[Arg] = &*NewArgIt++;
        }
    
        // Clone function body using LLVM 21 enum
        llvm::SmallVector<llvm::ReturnInst*, 8> Returns;
        llvm::CloneFunctionInto(
            NewF,
            &F,
            VMap,
            llvm::CloneFunctionChangeType::LocalChangesOnly, // Correct for LLVM 21
            Returns
        );
    
        // Replace all uses of the old function
        F.replaceAllUsesWith(NewF);
        F.eraseFromParent();
    
        return llvm::PreservedAnalyses::none();
      
    }

}
