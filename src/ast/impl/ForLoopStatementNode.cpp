#include "ForLoopStatementNode.hpp"
#include "tmpl/CloneNodeTemplate.hpp"
#include <logger/Logger.hpp>


namespace LynxAst {

    using namespace LynxLogger;
    using namespace LynxContext;
    using namespace Cloneable;

    llvm::Value* ForLoopStatementNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_WARN("IR Code Generation");
        auto& context = astContext->getLLVMContext();
        auto& builder = astContext->getBuilder();

        if (preLoopNode != nullptr) {
            preLoopNode->generateCode(astContext->createContext());
        }

        auto* insertFunction = builder.GetInsertBlock()->getParent();
        auto* conditionBlock = llvm::BasicBlock::Create(context, "loopCondition", insertFunction);
        auto* bodyBlock = llvm::BasicBlock::Create(context, "loopBody", insertFunction);
        auto* mergeBlock = llvm::BasicBlock::Create(context, "loopMerge", insertFunction);

        builder.CreateBr(conditionBlock);

        builder.SetInsertPoint(conditionBlock);
        auto* condValue = conditionNode->generateCode(astContext->createContext());
        builder.CreateCondBr(condValue, bodyBlock, mergeBlock);

        builder.SetInsertPoint(bodyBlock);
        if (beforeNode != nullptr) {
            beforeNode->generateCode(astContext->createContext());
        }
        statementNode->generateCode(astContext->createContext());
        if (afterNode != nullptr) {
            afterNode->generateCode(astContext->createContext());
        }
        if (builder.GetInsertBlock()->getTerminator() == nullptr) {
            builder.CreateBr(conditionBlock);
        }

        builder.SetInsertPoint(mergeBlock);

        return nullptr;
    }

    std::unique_ptr<Node> ForLoopStatementNode::clone() const {
        auto clonedCond = cloneNode(conditionNode);
        auto clonedStmts = cloneNode(statementNode);
        auto clonedLoop = std::make_unique<ForLoopStatementNode>(
            std::move(clonedCond),
            std::move(clonedStmts)
        );

        clonedLoop->preLoopNode  = cloneNode(preLoopNode);
        clonedLoop->postLoopNode = cloneNode(postLoopNode);
        clonedLoop->beforeNode   = cloneNode(beforeNode);
        clonedLoop->afterNode    = cloneNode(afterNode);

        return clonedLoop;
    }

}