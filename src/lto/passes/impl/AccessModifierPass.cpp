#include <passes/AccessModifierPass.hpp>
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"


namespace LynxLTO {

    char AccessModifierPass::ID = 0;

    llvm::PreservedAnalyses AccessModifierPass::run(llvm::Module &M, llvm::ModuleAnalysisManager &MAM) {
        llvm::errs() << "[Lynx] Access modifer Passessing.......\n";
        return llvm::PreservedAnalyses::all();
    }

}