#include <logger/Logger.hpp>
#include <helper/ValuePlaceholderNode.hpp>
#include <constants/metadata/LabelTypeConstants.hpp>
#include "IfStatementNode.hpp"

using namespace LynxLogger;
using namespace LynxContext;
using namespace LynxLabelTypeConstants;

namespace LynxAst {

    llvm::Value* IfStatementNode::generateCode(std::shared_ptr<AstContext> astContext) {
        return generateIfElseIf(*astContext);
    }
    
    llvm::Value* IfStatementNode::generateIfElseIf(const AstContext& astContext) {
        auto& context = astContext.getLLVMContext();
        auto& builder = astContext.getBuilder();

        auto* currenctFunc = builder.GetInsertBlock()->getParent();
        auto* mergeBlock = llvm::BasicBlock::Create(context, LynxLabelTypeConstants::lynxIfElseMerge);

        for (unsigned long i = 0; i < this->conditions.size(); ++i) {
            auto* elseIfBlock = llvm::BasicBlock::Create(context, LynxLabelTypeConstants::lynxElseIfCondition);
            auto* thenBlock = llvm::BasicBlock::Create(context, LynxLabelTypeConstants::lynxIfThenBranch);

            llvm::Value* condValue = nullptr;

            if (this->conditions[i]) {
                condValue = this->conditions[i]->generateCode(astContext.createContext());
            } else {
                auto trueNode = std::make_unique<ValuePlaceholderNode>(llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), 1, false));
                condValue = trueNode->generateCode(astContext.createContext());
            }
    
            builder.CreateCondBr(condValue, thenBlock, elseIfBlock);

            currenctFunc->getBasicBlockList().push_back(thenBlock);
            builder.SetInsertPoint(thenBlock);
            this->statements[i]->generateCode(astContext.createContext());

            if (builder.GetInsertBlock()->getTerminator() == nullptr) {
                builder.CreateBr(mergeBlock);
            }

            currenctFunc->getBasicBlockList().push_back(elseIfBlock);
            builder.SetInsertPoint(elseIfBlock);

            if (i == this->conditions.size() - 1 && builder.GetInsertBlock()->getTerminator() == nullptr) {
                builder.CreateBr(mergeBlock);
            }
        }

        currenctFunc->getBasicBlockList().push_back(mergeBlock);
        builder.SetInsertPoint(mergeBlock);

        return nullptr;
    }

    void IfStatementNode::addBranch(std::unique_ptr<Node> condNode, std::unique_ptr<Node> stmtNode) {
        this->conditions.push_back(std::move(condNode));
        this->statements.push_back(std::move(stmtNode));
    }

    void IfStatementNode::addBranch(std::unique_ptr<IfStatementNode> ifNode) {
        
        for (auto& cond : ifNode->conditions) {
            this->conditions.push_back(std::move(cond));
        }

        for (auto& stmt : ifNode->statements) {
            this->statements.push_back(std::move(stmt));
        }
    
        ifNode->conditions.clear();
        ifNode->statements.clear();
    }

    void IfStatementNode::addElseBranch(std::unique_ptr<Node> stmtNode) {
        this->statements.push_back(std::move(stmtNode));
        this->conditions.push_back(nullptr);
    }

    std::unique_ptr<Node> IfStatementNode::clone() const {
        auto clonedNode = std::make_unique<IfStatementNode>();
    
        for (size_t i = 0; i < conditions.size(); ++i) {
            auto clonedCond = conditions[i] ? conditions[i]->clone() : nullptr;
            auto clonedStmt = statements[i] ? statements[i]->clone() : nullptr;
            if (clonedCond) {
                clonedNode->addBranch(std::move(clonedCond), std::move(clonedStmt));
            } else {
                clonedNode->addElseBranch(std::move(clonedStmt));
            }
        }
    
        return clonedNode;
    }

}
