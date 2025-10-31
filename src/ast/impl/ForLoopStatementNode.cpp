#include "ForLoopStatementNode.hpp"
#include <logger/Logger.hpp>



using namespace LynxLogger;
using namespace LynxContext;

namespace LynxAst {


    llvm::Value* ForLoopStatementNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_WARN("Generating LLVM code for IfStatementNode. generateCode");
        auto& context = astContext->getLLVMContext();
        auto& builder = astContext->getBuilder();

        if (this->preLoopNode != nullptr) {
            this->preLoopNode->generateCode(astContext->createContext());
        }

        llvm::Function* insertFunction = builder.GetInsertBlock()->getParent();
        llvm::BasicBlock* conditionBlock = llvm::BasicBlock::Create(context, "loopCondition", insertFunction);
        llvm::BasicBlock* bodyBlock = llvm::BasicBlock::Create(context, "loopBody", insertFunction);
        llvm::BasicBlock* mergeBlock = llvm::BasicBlock::Create(context, "loopMerge", insertFunction);

        builder.CreateBr(conditionBlock);

        builder.SetInsertPoint(conditionBlock);
        builder.CreateCondBr(
                this->conditionNode->generateCode(astContext->createContext()),
                bodyBlock,
                mergeBlock
        );

        builder.SetInsertPoint(bodyBlock);
        if (this->beforeNode != nullptr) {
            this->beforeNode->generateCode(astContext->createContext());
        }
        this->statementNode->generateCode(astContext->createContext());
        if (this->afterNode != nullptr) {
            this->afterNode->generateCode(astContext->createContext());
        }
        if (builder.GetInsertBlock()->getTerminator() == nullptr) {
            builder.CreateBr(conditionBlock);
        }

        builder.SetInsertPoint(mergeBlock);

        return nullptr;
    }

    std::unique_ptr<Node> ForLoopStatementNode::clone() const {
        auto clonedCond = dynamic_cast<ExpressionNode*>(conditionNode->clone().release());
        auto clonedStmts = dynamic_cast<StatementListNode*>(statementNode->clone().release());
        if (!clonedCond || !clonedStmts)
            throw std::runtime_error("Invalid node type in ForLoopStatementNode::clone");

        return std::make_unique<ForLoopStatementNode>(
            std::unique_ptr<ExpressionNode>(clonedCond),
            std::unique_ptr<StatementListNode>(clonedStmts)
        );

    }

}