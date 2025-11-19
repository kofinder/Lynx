#include "PrimaryExpressionNode.hpp"

namespace LynxAst {

    using namespace LynxContext;

    llvm::Value* PrimaryExpressionNode::generateCode(std::shared_ptr<AstContext> astContext) {
        return innerExpNode->generateCode(astContext->createContext());
    }

    std::unique_ptr<Node> PrimaryExpressionNode::clone() const {
        std::unique_ptr<Node> clonedInnerExp = innerExpNode ? innerExpNode->clone() : nullptr;
        return std::make_unique<PrimaryExpressionNode>(primaryExpType, std::move(clonedInnerExp));            
    }
}



