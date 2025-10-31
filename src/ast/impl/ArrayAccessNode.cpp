#include "ArrayAccessNode.hpp"
#include <logger/Logger.hpp>

using namespace LynxLogger;
using namespace LynxContext;

namespace LynxAst {
    llvm::Value* ArrayAccessNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_ERROR("ArrayAccessNode Dereference Object Access Node ............");
        return nullptr;
    }


    std::unique_ptr<Node> ArrayAccessNode::clone() const {
        std::unique_ptr<Node> clonedInExp = indexNode ? indexNode->clone() : nullptr;
    
        auto castedExpr = dynamic_cast<ExpressionNode*>(clonedInExp.release());
        if (!castedExpr) {
            throw std::runtime_error("ArrayAccessNode::clone() - indexNode is not an ExpressionNode");
        }
    
        return std::make_unique<ArrayAccessNode>(arrayName, std::unique_ptr<ExpressionNode>(castedExpr));
    }
    
}