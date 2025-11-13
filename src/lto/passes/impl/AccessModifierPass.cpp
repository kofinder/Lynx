#include <passes/AccessModifierPass.hpp>
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"


namespace LynxLTO {

    llvm::PreservedAnalyses AccessModifierPass::run(llvm::Module& M, llvm::ModuleAnalysisManager& MAM) {
        llvm::errs() << "[Lynx] Access modifer Passessing.......\n";

        for (auto &F : M) {
            if (!F.isDeclaration() && !F.hasPrivateLinkage()) {
                F.setLinkage(llvm::GlobalValue::InternalLinkage);
            }
        }

        for (auto &GV : M.globals()) {
            if (!GV.hasPrivateLinkage()) {
                GV.setLinkage(llvm::GlobalValue::InternalLinkage);
            }
        }
    
        return llvm::PreservedAnalyses::all();
    }

}