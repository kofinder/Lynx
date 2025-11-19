#include "ExpressionStatementNode.hpp"

namespace LynxAst {
    
    llvm::Value* ExpressionStatementNode::generateCode(std::shared_ptr<AstContext> astContext) {
        return expressionNode->generateCode(astContext);
    }

    std::unique_ptr<Node> ExpressionStatementNode::clone() const {
        return std::make_unique<ExpressionStatementNode>(expressionNode->clone());
    }

}
