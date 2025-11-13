#include <passes/FunctionSecurityPass.hpp>
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/MDBuilder.h"
#include "llvm/Support/raw_ostream.h"

namespace LynxLTO {

    llvm::PreservedAnalyses FunctionSecurityPass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {
        llvm::errs() << "Running FunctionSecurityPass on module: " << "\n";

        bool Changed = false;

        
        llvm::LLVMContext &Ctx = M.getContext();
        llvm::MDBuilder MDB(Ctx);
    
        for (auto &F : M) {
            if (F.isDeclaration()) continue;
            llvm::MDString *Str = MDB.createString("security-critical");
            llvm::MDNode *Node = llvm::MDNode::get(Ctx, Str);
    
            F.setMetadata("lynx.security", Node);
            Changed = true;
    
            F.addFnAttr(llvm::Attribute::NoInline); // prevent certain attacks
            F.addFnAttr(llvm::Attribute::SanitizeAddress); // if enabled in compiler
            llvm::errs() << "Security metadata attached to function: "  << F.getName() << "\n";
        }
    
        return Changed ? llvm::PreservedAnalyses::none()
                       : llvm::PreservedAnalyses::all();
    
    }
    
}
