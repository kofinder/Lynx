#include "ExpressionStatementNode.hpp"
#include <logger/Logger.hpp>

using namespace LynxLogger;
namespace LynxAst {
    
    llvm::Value* ExpressionStatementNode::generateCode(std::shared_ptr<AstContext> astContext) {
         LOG_INFO("Initialize ExpressionStatementNode consturctor with node value");
        return this->expressionNode->generateCode(astContext);
    }

    std::unique_ptr<Node> ExpressionStatementNode::clone() const {
        // return std::make_unique<ExpressionStatementNode>(*this); 
        return std::make_unique<ExpressionStatementNode>(
            this->expressionNode->clone() // recursively clone operand
        );
    
    }

}
