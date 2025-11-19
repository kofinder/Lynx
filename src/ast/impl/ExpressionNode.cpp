#include "ExpressionNode.hpp"
#include "tmpl/CloneNodeTemplate.hpp"
namespace LynxAst {

    llvm::Value* ExpressionNode::generateCode(std::shared_ptr<AstContext> astContext) {
        return expressionNode->generateCode(astContext);
    }

    std::unique_ptr<Node> ExpressionNode::clone() const {   
        using namespace Cloneable;  
        auto clonedOperand = cloneNode(expressionNode);
        return std::make_unique<ExpressionNode>(expressionType, std::move(clonedOperand));
    }
}
