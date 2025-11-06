#include "StatementNode.hpp"
#include "FunctionNode.hpp"
#include <logger/Logger.hpp>
#include <context/GlobalSymbolContext.hpp>

namespace LynxAst {

    llvm::Value* StatementNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_WARN("IR Code Generation ....");
        if(statementType == StatementType::RETURN_STATEMENTS) {
            return generateReturnStatment(*astContext);
        } else if(statementType == StatementType::THROW_STATEMENTS) {
            return generateThrowStatment(*astContext);
        }
        return nullptr;
    }

    llvm::Value* StatementNode::generateReturnStatment(const AstContext& astContext) {

        auto* value = operands[0]->generateCode(astContext.createContext());

        auto symbol = astContext.getGlobalContext();
        auto functionNode = symbol->getFunctionNode();

        if (!functionNode) {
            LOG_ERROR("return should be inside a function....");
            throw std::runtime_error("return should be inside a function.");
        }

        auto fnNode = static_cast<FunctionNode*>(functionNode);
        if (!fnNode) {
            LOG_ERROR("Expected FunctionNode in current context, but static_cast failed....");
            throw std::runtime_error("Expected FunctionNode in current context, but static_cast failed.");
        }

        auto* result = fnNode->setReturnValue(astContext, value);
        
        return result;
    }

    llvm::Value* StatementNode::generateThrowStatment(const AstContext& astContext) {
        return astContext.getBuilder().CreateUnreachable();
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