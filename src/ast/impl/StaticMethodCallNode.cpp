#include "StaticMethodCallNode.hpp"

namespace LynxAst {

    llvm::Value* StaticMethodCallNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_ERROR("Invoked...");
        return nullptr;
    }

    std::unique_ptr<Node> StaticMethodCallNode::clone() const  {
        LOG_ERROR("Cloning FunctionCallNode...");
        auto clonedArgs = std::make_unique<std::vector<std::unique_ptr<ExpressionNode>>>();
        if(arguments) {
             clonedArgs->reserve(arguments->size());
             for (const auto& arg : *arguments) {
                  if (arg) {
                      auto clonedArg = arg->clone();
                      auto exprPtr = dynamic_cast<ExpressionNode*>(clonedArg.release());
                      assert(exprPtr && "Cloned node is not an ExpressionNode");
                      clonedArgs->push_back(std::unique_ptr<ExpressionNode>(exprPtr));
                  } else {
                      clonedArgs->push_back(nullptr);
                  }
              }
        }
    
        auto clonedNode = std::make_unique<StaticMethodCallNode>(typeName, methodName, std::move(clonedArgs));
        return clonedNode;
    }
}