#include "ExpressionNode.hpp"
#include <logger/Logger.hpp>

using namespace LynxLogger;
namespace LynxAst {

    llvm::Value* ExpressionNode::generateCode(std::shared_ptr<AstContext> astContext) {
        return this->expressionNode->generateCode(astContext);
    }

    std::unique_ptr<Node> ExpressionNode::clone() const {
        //LOG_INFO("Cloned Expression Node ...");
        
        std::unique_ptr<ExpressionNode> clonedChild;
        if (expressionNode) {
            // Clone returns std::unique_ptr<Node>
            std::unique_ptr<Node> baseClone = expressionNode->clone();
    
            // Manually cast the pointer from Node* to ExpressionNode*
            ExpressionNode* derivedPtr = static_cast<ExpressionNode*>(baseClone.release());
    
            // Now take ownership with unique_ptr<ExpressionNode>
            clonedChild.reset(derivedPtr);
        }
    
        // Make a new ExpressionNode copy
        return std::make_unique<ExpressionNode>(expressionType, std::move(clonedChild));
    }
}
