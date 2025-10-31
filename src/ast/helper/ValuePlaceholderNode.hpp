#ifndef LYNX_VALUE_PLACEHOLDER_NODE_HPP
#define LYNX_VALUE_PLACEHOLDER_NODE_HPP

#include "Node.hpp"

using namespace LynxConstants;

namespace LynxAst {

    class ValuePlaceholderNode : public Node {

        llvm::Value* value;

    public:

        ValuePlaceholderNode(llvm::Value* tempValue) : value(tempValue) {}

        NodeType getNodeType() override { return NodeType::VALUE_PLACEHOLDER_NODE;  }

        llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override { return value; }

        std::unique_ptr<Node> clone() const override { return std::make_unique<ValuePlaceholderNode>(*this); }

        ~ValuePlaceholderNode() override = default;
    };

}

#endif
