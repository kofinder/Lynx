#include <passes/FunctionMetadataPass.hpp>
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/MDBuilder.h"
#include "llvm/Support/raw_ostream.h"

namespace LynxLTO {

    llvm::PreservedAnalyses FunctionMetadataPass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {
        llvm::errs() << "Running FunctionMetadataPass on module: " << "\n";
        bool Changed = false;

        llvm::LLVMContext &Ctx = M.getContext();
        llvm::MDBuilder MDB(Ctx);
    
        for (auto &F : M) {
            if (F.isDeclaration()) continue;
            llvm::MDString *Str = MDB.createString("LynxFunctionMetadata");
            llvm::MDNode *Node = llvm::MDNode::get(Ctx, Str);
            F.setMetadata("lynx.meta", Node);
            Changed = true;
            llvm::errs() << "Metadata attached to function: " << F.getName() << "\n";
        }
    
        return Changed ? llvm::PreservedAnalyses::none()
                       : llvm::PreservedAnalyses::all();
    }
    
}
