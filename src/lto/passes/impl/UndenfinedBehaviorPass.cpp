#include <passes/UndenfinedBehaviorPass.hpp>
#include "llvm/IR/Verifier.h"
#include <ast/utils/UndefinedBehaviorUtils.hpp>
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"


namespace LynxLTO {

    using namespace llvm;
    using namespace LynxAst::UB;

    
    llvm::PreservedAnalyses UndenfinedBehaviorPass::run(llvm::Module& M, llvm::ModuleAnalysisManager& MAM) {
        llvm::errs() << "[Lynx] Running UndefinedBehaviorPass on module: " << M.getName() << "\n";

        bool HasUB = false;

        for (auto &F : M) {
            if (F.isDeclaration()) continue;
    
            for (auto &BB : F) {
                for (auto &I : BB) {
    
                    // Check division by zero (integer division)
                    if (auto *BinOp = llvm::dyn_cast<llvm::BinaryOperator>(&I)) {
                        if (BinOp->getOpcode() == llvm::Instruction::SDiv ||
                            BinOp->getOpcode() == llvm::Instruction::UDiv) {
    
                            if (auto *ConstRHS = llvm::dyn_cast<llvm::ConstantInt>(BinOp->getOperand(1))) {
                                if (ConstRHS->isZero()) {
                                    llvm::errs() << "[UB] Division by zero in function " << F.getName() << "\n";
                                    HasUB = true;
                                }
                            }
                        }
                    }
    
                    // Check unreachable instructions
                    if (llvm::isa<llvm::UnreachableInst>(&I)) {
                        llvm::errs() << "[UB] Unreachable instruction in function " << F.getName() << "\n";
                        HasUB = true;
                    }
    
                    // Additional UB checks can be added here:
                    // - Poison/undef values
                    // - Misaligned loads/stores
                    // - Out-of-bounds GEPs
                }
            }
        }
    
        if (!HasUB) {
            llvm::errs() << "[Lynx] No obvious undefined behavior detected\n";
        }
    
        return llvm::PreservedAnalyses::all(); // read-only pass
    

        // LLVMContext &Ctx = M.getContext();

        // const char *DemoFuncName = "ub_demo_function";
        // if (M.getFunction(DemoFuncName)) {
        //   errs() << "[UndenfinedBehaviorPass] Demo function already exists. Skipping.\n";
        //   return PreservedAnalyses::all();
        // }


        // IRBuilder<> B(Ctx);

        // // Create prototype: i32 @ub_demo_function()
        // FunctionType *FT = FunctionType::get(B.getInt32Ty(), false);
        // Function *F = Function::Create(FT, Function::ExternalLinkage, DemoFuncName, &M);
    
        // // Entry block
        // BasicBlock *entryBB = BasicBlock::Create(Ctx, "entry", F);
        // B.SetInsertPoint(entryBB);
    
        // // Construct UB builder that wraps IRBuilder
        // Builder UB(B);
    
        // // --- Create basic values ---
        // // Indeterminate and undef values (demonstrate both)
        // Value *lhs = UB.makeIndeterminate(Type::getInt32Ty(Ctx)); // defaults to undef
        // Value *rhs = UB.getUndef(Type::getInt32Ty(Ctx));
    
        // // Freeze both so subsequent operations are deterministic
        // lhs = UB.freeze(lhs, "lhs_frozen");
        // rhs = UB.freeze(rhs, "rhs_frozen");
    
        // // --- Arithmetic examples ---
        // Value *sumNSW  = UB.addNSW(lhs, rhs, "nsw_sum");
        // Value *sumNUW  = UB.addNUW(lhs, rhs, "nuw_sum");
        // Value *sumSafe = UB.safeAddNSW(lhs, rhs, "safe_sum");
        // Value *divSafe = UB.safeSDiv(lhs, rhs, "safe_div");
        // Value *shift   = UB.shlNSWNUW(lhs, rhs, "safe_shl");
    
        // // Use an instruction to prevent unused-value optimizations from removing them
        // // (we'll fold them into a chain for demonstration)
        // Value *accum = B.CreateAdd(sumNSW, sumNUW, "accum0");
        // accum = B.CreateAdd(accum, sumSafe, "accum1");
        // accum = B.CreateAdd(accum, divSafe, "accum2");
        // accum = B.CreateAdd(accum, shift, "accum3");
    
        // // --- Select example ---
        // Value *cond = B.CreateICmpEQ(lhs, rhs, "cond");
        // Value *sel  = UB.safeSelect(cond, sumSafe, divSafe, "safe_sel");
    
        // // Include select result into accumulator
        // accum = B.CreateAdd(accum, sel, "accum4");
    
        // // --- Memory ops ---
        // AllocaInst *slot = B.CreateAlloca(Type::getInt32Ty(Ctx), nullptr, "slot");
        // UB.safeStore(accum, slot);
        // Value *loaded = UB.safeLoad(Type::getInt32Ty(Ctx), slot, "loaded");
    
        // // --- PHI node example with control flow ---
        // BasicBlock *bbThen = BasicBlock::Create(Ctx, "then", F);
        // BasicBlock *bbElse = BasicBlock::Create(Ctx, "else", F);
        // BasicBlock *bbMerge = BasicBlock::Create(Ctx, "merge", F);
    
        // // Branch based on cond built earlier
        // B.CreateCondBr(cond, bbThen, bbElse);
    
        // // then block
        // B.SetInsertPoint(bbThen);
        // Value *thenVal = UB.addNSW(loaded, B.getInt32(1), "then_add");
        // B.CreateBr(bbMerge);
    
        // // else block
        // B.SetInsertPoint(bbElse);
        // Value *elseVal = UB.safeSDiv(loaded, B.getInt32(2), "else_div");
        // B.CreateBr(bbMerge);
    
        // // merge block
        // B.SetInsertPoint(bbMerge);
        // PHINode *phi = UB.safePhi(Type::getInt32Ty(Ctx), 2, "phi");
        // // addIncoming expects incoming value and its originating BasicBlock
        // phi->addIncoming(thenVal, bbThen);
        // phi->addIncoming(elseVal, bbElse);
    
        // // final return: freeze phi before returning to be explicit
        // Value *retVal = UB.freeze(phi, "ret_frozen");
        // B.CreateRet(retVal);
    
        // // Verify module (structure correctness)
        // if (verifyModule(M, &errs())) {
        //   errs() << "[UndenfinedBehaviorPass] Module verification failed!\n";
        //   // We still return none, but in practice you may abort or remove the function.
        //   return PreservedAnalyses::none();
        // }
    
        // errs() << "[UndenfinedBehaviorPass] Created demo function '" << DemoFuncName << "'\n";
        // return PreservedAnalyses::none();   

    }

}