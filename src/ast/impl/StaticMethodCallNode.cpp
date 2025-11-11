#include "StaticMethodCallNode.hpp"
#include "tmpl/CloneNodeTemplate.hpp"

namespace LynxAst {

    llvm::Value* StaticMethodCallNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_ERROR("Invoked...");
        return nullptr;
    }

    std::unique_ptr<Node> StaticMethodCallNode::clone() const  {
        using namespace Cloneable;
        auto clonedArgs = cloneNodeVector(arguments);
        auto clonedNode = std::make_unique<StaticMethodCallNode>(typeName, methodName, std::move(clonedArgs));
        return clonedNode;
    }
}