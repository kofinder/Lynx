#include <passes/RemoveUnusedParamsPass.hpp>
#include "llvm/IR/Verifier.h"
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>
#include <vector>
#include <unordered_map>

using namespace llvm;

namespace LynxLTO {

    llvm::PreservedAnalyses RemoveUnusedParamsPass::run(Function &F, FunctionAnalysisManager &FAM) {
        errs() << "Running RemoveUnusedParamsPass on function: " << F.getName() << "\n";

        if (F.isDeclaration() || F.arg_empty()) {
            return llvm::PreservedAnalyses::all();
        }
            
        // Identify used arguments
        std::vector<Argument*> usedArgs;
        for (auto& arg : F.args()) {
            if (!arg.use_empty()) {
                usedArgs.push_back(&arg);
            }
        }

        // Nothing to remove
        if (usedArgs.size() == F.arg_size()) {
            return llvm::PreservedAnalyses::all();
        }
            
        errs() << "  Removing " << (F.arg_size() - usedArgs.size()) << " unused arguments\n";

        // Create new function type
        std::vector<Type*> paramTypes;
        for (auto* arg : usedArgs) {
            paramTypes.push_back(arg->getType());
        }

        auto* newFuncTy = FunctionType::get(F.getReturnType(), paramTypes, F.isVarArg());
        auto* newFunc = Function::Create(
            newFuncTy,
            F.getLinkage(),
            F.getName() + ".trimmed",
            F.getParent()
        );
        newFunc->copyAttributesFrom(&F);

        // Map old arguments to new arguments
        std::unordered_map<Value*, Value*> vMap;
        auto newArgIt = newFunc->arg_begin();
        for (auto* arg : usedArgs) {
            newArgIt->setName(arg->getName());
            vMap[arg] = &*newArgIt++;
        }

        // Manually clone instructions into new blocks using push_back on BB iterator
        for (auto& BB : F) {
            auto* newBB = BasicBlock::Create(F.getContext(), BB.getName(), newFunc);
            for (auto &Inst : BB) {
                auto* newInst = Inst.clone();
                // Remap operands
                for (unsigned i = 0; i < Inst.getNumOperands(); ++i) {
                    Value *Op = Inst.getOperand(i);
                    auto It = vMap.find(Op);
                    if (It != vMap.end()) {
                        newInst->setOperand(i, It->second);
                    }
                    
                }
                vMap[&Inst] = newInst;
                newInst->insertInto(newBB, newBB->end());
            }
        }
        
        F.replaceAllUsesWith(newFunc);
        F.eraseFromParent();

        return llvm::PreservedAnalyses::none();
    }

} 
