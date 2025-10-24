#include "PrimaryExpressionNode.hpp"
#include <logger/Logger.hpp>


using namespace LynxLogger;
using namespace LynxContext;

namespace LynxAst {
    llvm::Value* PrimaryExpressionNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_INFO("Invoked..");
        return innerExpNode->generateCode(astContext->createContext());
    }

    std::unique_ptr<Node> PrimaryExpressionNode::clone() const {
        std::unique_ptr<Node> clonedInnerExp = innerExpNode ? innerExpNode->clone() : nullptr;
        return std::make_unique<PrimaryExpressionNode>(primaryExpType, std::move(clonedInnerExp));            
    }
}



