#include "StatementNode.hpp"
#include "FunctionNode.hpp"
#include <logger/Logger.hpp>
#include <context/GlobalSymbolContext.hpp>

namespace LynxAst {

    llvm::Value* StatementNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_INFO("Executed ....");
        if(statementType == StatementType::RETURN_STATEMENTS) {
            return generateReturnStatment(astContext);
        } else if(statementType == StatementType::THROW_STATEMENTS) {
            return generateThrowStatment(astContext);
        }

        return nullptr;
    }

    llvm::Value* StatementNode::generateReturnStatment(std::shared_ptr<AstContext> astContext) {
        LOG_INFO("Invoked... ");

        llvm::Value* value = this->operands[0]->generateCode(astContext->createContext());

        auto symbol = astContext->getGlobalContext();
        auto functionNode = symbol->getFunctionNode();
        auto& builder = astContext->getBuilder();

        if (!functionNode) {
            LOG_ERROR("return should be inside a function....");
            throw std::runtime_error("return should be inside a function.");
        }

        auto fnNode = static_cast<FunctionNode*>(functionNode);
        if (!fnNode) {
            LOG_ERROR("Expected FunctionNode in current context, but static_cast failed....");
            throw std::runtime_error("Expected FunctionNode in current context, but static_cast failed.");
        }

        llvm::Value* result = fnNode->setReturnValue(std::move(astContext), value);
        
        return result;
    }

    llvm::Value* StatementNode::generateThrowStatment(std::shared_ptr<AstContext> astContext) {
        LOG_INFO("START PROCESSING THROW STATEMENT... ");
        auto& builder = astContext->getBuilder();
        return builder.CreateUnreachable();
    }

    std::unique_ptr<Node> StatementNode::clone() const {
        std::vector<std::unique_ptr<Node>> clonedOperands;
        
        for (const auto& operand : operands) {
            clonedOperands.push_back(operand->clone());
        }

        auto clonedNode = std::make_unique<StatementNode>(statementType, std::move(clonedOperands[0]));
        
        return clonedNode;
    }

}