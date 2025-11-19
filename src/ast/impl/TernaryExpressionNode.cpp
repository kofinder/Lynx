#include <logger/Logger.hpp>
#include <constants/metadata/LabelTypeConstants.hpp>
#include "TernaryExpressionNode.hpp"


namespace LynxAst {

    using namespace LynxLogger;
    using namespace LynxContext;
    using namespace LynxLabelTypeConstants;

    llvm::Value* TernaryExpressionNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_ERROR("TernaryOperatorStatement::generateCode...");

        auto& context = astContext->getLLVMContext();
        auto& builder = astContext->getBuilder();

        auto* function = builder.GetInsertBlock()->getParent();

        // Generate code for the condition and expressions
        auto* condition = conditionExpression->generateCode(astContext->createContext());

        // Create two basic blocks (true and false branches)
        auto* trueBB = llvm::BasicBlock::Create(context, lynxTernaryTrueBB, function);
        auto* falseBB = llvm::BasicBlock::Create(context, lynxTernaryFalseBB, function);
        auto* mergeBB = llvm::BasicBlock::Create(context, lynxTernaryMergeBB, function);  // Merge block for PHI node

        // Manually create the conditional branch
        llvm::BranchInst::Create(trueBB, falseBB, condition, builder.GetInsertBlock());

        // Insert code for the true expression
        builder.SetInsertPoint(trueBB);
        auto* trueExp = this->thenExpression->generateCode(astContext->createContext());
        llvm::BranchInst::Create(mergeBB, trueBB);  // Branch to merge block after true expression

        // Insert code for the false expression
        builder.SetInsertPoint(falseBB);
        auto* falseExp = this->endExpression->generateCode(astContext->createContext());
        llvm::BranchInst::Create(mergeBB, falseBB);  // Branch to merge block after false expression

        // Insert PHI node in the merge block
        builder.SetInsertPoint(mergeBB);
        llvm::Type* retType = trueExp->getType(); // Assuming trueExp and falseExp have the same type

        // Create PHI node to merge true and false values
        llvm::PHINode *PN = llvm::PHINode::Create(retType, 2, lynxTernaryResult, builder.GetInsertBlock());

        // Add incoming values to the PHI node from both branches
        PN->addIncoming(trueExp, trueBB);
        PN->addIncoming(falseExp, falseBB);

        return PN;
    }

    std::unique_ptr<Node> TernaryExpressionNode::clone() const {
        return std::make_unique<TernaryExpressionNode>(
            conditionExpression->clone(),
            thenExpression->clone(),
            endExpression->clone()
        ); 
    }
}




// llvm::Value* TernaryExpressionNode::generateCode(std::shared_ptr<AstContext> astContext) {
//     LOG_ERROR("TernaryOperatorStatement::generateCode...");
    
//     auto& context = GlobalContext::getInstance().getContext();
//     auto& builder = GlobalContext::getInstance().getBuilder();
    
//     llvm::Function* function = builder.GetInsertBlock()->getParent();
    
//     // Generate code for the condition and the two expressions
//     llvm::Value* condition = this->conditionExpression->generateCode(astContext);
//     llvm::Value* trueExp = this->thenExpression->generateCode(astContext);  // 10
//     llvm::Value* falseExp = this->endExpression->generateCode(astContext);   // 5

//     // Create the basic blocks for the true and false branches
//     llvm::BasicBlock* trueBB = llvm::BasicBlock::Create(context, "ternaryTrue", function);
//     llvm::BasicBlock* falseBB = llvm::BasicBlock::Create(context, "ternaryFalse", function);
//     llvm::BasicBlock* mergeBB = llvm::BasicBlock::Create(context, "ternaryMerge", function);

//     // Create a conditional branch based on the condition
//     builder.CreateCondBr(condition, trueBB, falseBB);

//     // True branch: set insert point to trueBB and assign value 10
//     builder.SetInsertPoint(trueBB);
//     builder.CreateBr(mergeBB);  // Jump to mergeBB after the true expression

//     // False branch: set insert point to falseBB and assign value 5
//     builder.SetInsertPoint(falseBB);
//     builder.CreateBr(mergeBB);  // Jump to mergeBB after the false expression

//     // Insert the PHI node in the merge block
//     builder.SetInsertPoint(mergeBB);
//     llvm::Type* retType = trueExp->getType();  // Assuming both trueExp and falseExp are of the same type

//     llvm::PHINode* PN = llvm::PHINode::Create(retType, 2, "ternaryResult", mergeBB);

//     // Add incoming values for both branches
//     PN->addIncoming(trueExp, trueBB);  // If true, take value from trueBB
//     PN->addIncoming(falseExp, falseBB);  // If false, take value from falseBB

//     // Return the PHI node value (this is the final result of the ternary expression)
//     return PN;
// }